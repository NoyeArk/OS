#include "kernel.h"

Kernel::Kernel() {
	// ����Ŀ¼����ʼ������λ��ͼ
	//for (const auto& file : )
	this->Create("test");
}


std::unique_ptr<PCB> Kernel::Fork() {
	// ʹ�� C++11 ���������
	std::random_device rd;   // ��ȡ����豸
	std::mt19937 mt(rd());   // ʹ�� Mersenne Twister ����
	std::uniform_int_distribution<int> dist(1, 10000);  // ���巶Χ

	// ����ָ�뽫���䳬����Χʱ�Զ��ͷ��ڴ�
	std::unique_ptr<PCB> p = std::make_unique<PCB>();
	p->pid = dist(mt);  // �������ID
	std::vector<int> allocMem = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (!allocMem.empty()) {
		for (int ii = 0; ii < InitProcessBlockNum; ii++) {
			// ��ҳ�� ��������
			int virtualPageNumber = ii;
			p->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
		}
	}
	else {
		std::cout << "���̴���ʧ��" << std::endl;
		return nullptr;
	}
	return p;
}


void Kernel::Exit(const std::unique_ptr<PCB>& pcb) {
	// �ͷŽ����е��ڴ�ռ�
	std::vector<int> pcbBlocksId;
	std::transform(pcb->pageTable.begin(), pcb->pageTable.end(), std::back_inserter(pcbBlocksId),
		[](const std::pair<int, int>& pair) { return pair.second; });
	memorysystem.Free(pcb->pid, pcbBlocksId);
}


void Kernel::Open(const std::string& fileName) {
	// ��Ӧ���̻����Ӧ�ļ���Ȩ��
	auto targetFcbPoint = directory.OpenFile(fileName);
	sysOpenFiles.push_back(targetFcbPoint);
}


void Kernel::Read(const std::string& fileName) {
	FCB* targetFcbPoint = nullptr;
	for (auto fcb : sysOpenFiles) {
		if (fcb->name == fileName) {
			targetFcbPoint = fcb;
			break;
		}
	}
	if (targetFcbPoint == nullptr) {
		std::cout << "�����ȴ򿪸��ļ���" << std::endl;
		return;
	}
	std::unique_ptr<PCB> pcb = Fork();  // ��������
	if (!pcb) {  // ���̴���ʧ��
		std::cout << "���ļ�" << fileName << "ʧ��" << std::endl;
	}
	else {
		if (targetFcbPoint->len > pcb->pageTable.size()) {
			// 8���ڴ治������������
			int requestBlockNum = targetFcbPoint->len - pcb->pageTable.size();
			std::vector<int> allocMem = memorysystem.Alloc(pcb->pid, requestBlockNum);
			if (requestBlockNum > allocMem.size()) {
				// ��Ҫ����ҳ�û���LRU
				size_t pageNumToReplace = requestBlockNum - allocMem.size();
				auto replaceAllocMem = this->PageReplaceInterrupt(pcb->pid, pageNumToReplace);
				allocMem.insert(allocMem.end(), replaceAllocMem.begin(), replaceAllocMem.end());
			}
			pcb->AppendMmu(allocMem);
		}
		// ���ļ����ݴ��������ڴ�
		auto blockDatas = disk.ReadFile(targetFcbPoint->idxBlocksId, targetFcbPoint->len);
		// ������ý��̵��ڴ����ܲ�������������Ҫһ��һ�鸴��
		for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
			// offset�������mem��ƫ����
			int toWriteBlockId = pcb->pageTable[ii].second;
			char* blockData = new char[BLOCK_SIZE];
			std::memcpy(blockData, blockDatas[ii], BLOCK_SIZE);
			memorysystem.WriteMem(toWriteBlockId, blockData);
			delete[] blockData;
		}
	}
	this->Exit(pcb);  // �˳�����
}


void Kernel::Write(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();  // ��������
	std::string dataToWrite;                 // Ҫд�������
	if (!pcbPoint) {  // ���̴���ʧ��
	
	}
	else {
		auto targetFcbPoint = directory.OpenFile(fileName);
		std::getline(std::cin, dataToWrite);
		// �õ�Ҫд�����ݵĴ�С
		size_t sizeInBytes = sizeof(dataToWrite);
		if (targetFcbPoint->len * BLOCK_SIZE < sizeInBytes) {  // �ļ���С����Ҫ�Ӵ�������ռ�
			// �����Ҫ����Ĵ��̿���
			size_t applyBlockNum = (targetFcbPoint->len * BLOCK_SIZE - sizeInBytes) / 40 + 1;
			std::vector<short> newIdxBlocksId = disk.AllocDisk(targetFcbPoint->idxBlocksId.back(), targetFcbPoint->len, applyBlockNum);
			if (newIdxBlocksId.empty()) {
				std::cout << "д��ʧ�ܣ�" << std::endl;
				return;
			}
			// ���¸��ļ���Ӧ��FCB��������
			targetFcbPoint->ExpandFileLen(newIdxBlocksId, applyBlockNum);
		}
		disk.WriteFile(targetFcbPoint->idxBlocksId, dataToWrite);
	}
	this->Exit(pcbPoint);  // �˳�����
}


void Kernel::Close(const std::string& fileName) {
	for (size_t ii = 0; ii < sysOpenFiles.size(); ii++) {
		if (sysOpenFiles[ii]->name == fileName) {
			sysOpenFiles.erase(sysOpenFiles.begin() + ii);
			return;
		}
	}
	std::cout << "��δ�򿪸��ļ���" << std::endl;
}


void Kernel::Delete(const std::string& fileName) {
	auto toRemoveFileIdxBlocksId = directory.Rm(fileName);
	if (!toRemoveFileIdxBlocksId.empty())
		disk.FreeDisk(toRemoveFileIdxBlocksId);
}


void Kernel::Create(const std::string& fileName) {
	auto idxBlocksId = disk.AllocFileBlock();
	directory.Create(fileName, idxBlocksId);
}


std::vector<int> Kernel::PageReplaceInterrupt(int pid, int pageNumToReplace) {
	std::vector<int> replaceBlocksId = memorysystem.PagesReplace(pid, pageNumToReplace);
	// replaceBlocksId�еĿ��Ӧ�ڴ�����ݻ������һ���
	std::vector<char*> blocksData;
	// ������һ��һ��Ķ���
	for (auto blockId : replaceBlocksId) {
		char* blockData = memorysystem.ReadMem(blockId);
		blocksData.push_back(blockData);
	}
	// д����̶һ���
	disk.WriteSwap(blocksData);

	return replaceBlocksId;
}


std::string Kernel::getCurPath() {
	return directory.getCurPath();
}


// ����ʹ��command�������ж����ͺţ�Ӧ�ú궨�弸���ж�����
void Kernel::SysCall(COMMAND command, const std::string eax) {
	std::string targetFile = eax.substr(eax.find(" ") + 1);
	switch (command)
	{
	case FILE_FORMAT:
		directory.format();       break;
	case FILE_MKDIR:
		directory.mkdir();        break;
	case FILE_RMDIR:
		directory.rmdir();        break;
	case FILE_LS:
		directory.Ls();           break;
	case FILE_CD: 
		directory.cd(targetFile); break;
	case FILE_CREATE:
		this->Create(targetFile); break;
	case FILE_BACK:
		directory.Back();         break;
	case FILE_OPEN:
		this->Open(targetFile);   break;
	case FILE_CLOSE:
		this->Close(targetFile);  break;
	case FILE_WRITE:
		this->Write(targetFile);  break;
	case FILE_READ:
		this->Read(targetFile);   break;
	case FILE_RM:
		this->Delete(targetFile); break;
	default:
		break;
	}
}

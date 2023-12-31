#include "kernel.h"

Kernel::Kernel() {
	// ����Ŀ¼����ʼ������λ��ͼ
	this->Create("test");
	this->Create("a");
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
			p->pageTable.push_back(PageTableItem(allocMem[ii]));
		}
	}
	else {
		std::cout << "���̴���ʧ��" << std::endl;
		return nullptr;
	}
	return p;
}


bool Kernel::Exit(const std::unique_ptr<PCB>& pcb) {
	// �ͷŽ����е��ڴ�ռ�
	std::vector<int> pcbBlocksId;
	std::transform(pcb->pageTable.begin(), pcb->pageTable.end(), std::back_inserter(pcbBlocksId),
		[](const std::pair<int, int>& pair) { return pair.second; });
	memorysystem.Free(pcb->pid, pcbBlocksId);
	return true;
}


bool Kernel::Open(const std::string& fileName) {
	// ��Ӧ���̻����Ӧ�ļ���Ȩ��
	auto targetFcbPoint = directory.OpenFile(fileName);
	sysOpenFiles.push_back(targetFcbPoint);
	return true;
}


bool Kernel::Read(const std::string& fileName) {
	FCB* targetFcbPoint = nullptr;
	for (auto fcb : sysOpenFiles) {
		if (fcb->name == fileName) {
			targetFcbPoint = fcb;
			break;
		}
	}
	if (targetFcbPoint == nullptr) {
		std::cout << "�����ȴ򿪸��ļ���" << std::endl;
		return false;
	}
	std::unique_ptr<PCB> pcb = Fork();  // ��������
	if (!pcb) {  // ���̴���ʧ��
		std::cout << "���ļ�" << fileName << "ʧ��" << std::endl;
		return false;
	}
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

	// ���ļ����ݴ�������
	std::string dataReadFromDisk;
	Message.Send("DISK", "Read", targetFcbPoint->idxBlocksId);
	Message.Receive(dataReadFromDisk);

	if (dataReadFromDisk == "Error") {
		return false;
	}

	// ���ļ�����д���ڴ�
	for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
		std::string msgReceiveFromMem;
		short toWriteMemBlockId = pcb->pageTable[ii].memBlockId;
		std::vector<short> memBlockIdToWrite = { toWriteMemBlockId };
		std::string blockDataToWrite = dataReadFromDisk.substr(ii * BLOCK_SIZE, BLOCK_SIZE);
		
		Message.Send("MEMORY", "Write", memBlockIdToWrite, blockDataToWrite);
		Message.Receive(msgReceiveFromMem);

		if (msgReceiveFromMem == "Error") {
			return false;
		}
	}
	this->readyQueue.push_back(pcb);
	//this->Exit(pcb);  // �˳�����
	return true;
}


bool Kernel::Write(const std::string& fileName) {
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
				return false;
			}
			// ���¸��ļ���Ӧ��FCB��������
			targetFcbPoint->ExpandFileLen(newIdxBlocksId, applyBlockNum);
		}
		std::string msgReceiveFromDisk;
		Message.Send("DISK", "Write", targetFcbPoint->idxBlocksId, dataToWrite);
		Message.Receive(msgReceiveFromDisk);

		if (msgReceiveFromDisk == "Error") {
			return false;
		}
	}
	this->Exit(pcbPoint);  // �˳�����
	return true;
}


bool Kernel::Close(const std::string& fileName) {
	for (size_t ii = 0; ii < sysOpenFiles.size(); ii++) {
		if (sysOpenFiles[ii]->name == fileName) {
			sysOpenFiles.erase(sysOpenFiles.begin() + ii);
			return true;
		}
	}
	std::cout << "��δ�򿪸��ļ���" << std::endl;
	return false;
}


bool Kernel::Delete(const std::string& fileName) {
	// �õ�Ҫɾ���ļ�������������
	auto toRemoveFileIdxBlocksId = directory.Rm(fileName);
	if (!toRemoveFileIdxBlocksId.empty())
		disk.FreeDisk(toRemoveFileIdxBlocksId);
	return true;
}


bool Kernel::Create(const std::string& fileName) {
	auto idxBlocksId = disk.AllocFileBlock();
	directory.Create(fileName, idxBlocksId);
	return true;
}


std::vector<int> Kernel::PageReplaceInterrupt(int pageNumToReplace) {
	std::vector<int> replaceBlocksId = memorysystem.PagesReplace(this->readyQueue, pageNumToReplace);
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
bool Kernel::SysCall(COMMAND command, const std::string eax) {
	bool isCommandSuccess = false;
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
		isCommandSuccess = this->Create(targetFile); 
		break;
	case FILE_BACK:
		directory.Back();         
		break;
	case FILE_OPEN:
		isCommandSuccess = this->Open(targetFile);   
		break;
	case FILE_CLOSE:
		isCommandSuccess = this->Close(targetFile);  
		break;
	case FILE_WRITE:
		isCommandSuccess = this->Write(targetFile);  
		break;
	case FILE_READ:
		isCommandSuccess = this->Read(targetFile);   
		break;
	case FILE_RM:
		isCommandSuccess = this->Delete(targetFile); 
		break;
	default:
		break;
	}
	return isCommandSuccess;
}

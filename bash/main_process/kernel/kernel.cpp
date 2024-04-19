#include "kernel.h"

Kernel::Kernel() {
	// ����Ŀ¼����ʼ������λ��ͼ
	//this->SysCreateFile("test");
	//this->SysCreateFile("a");
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
			p->pageTable.push_back(PageTableItem(allocMem[ii], std::chrono::system_clock::now()));
		}
	}
	else {
		std::cout << "���̴���ʧ��" << std::endl;
		return nullptr;
	}
	return p;
}


bool Kernel::Exit(const std::unique_ptr<PCB>& pcb) {
	std::vector<int> pcbBlocksId;
	// �õ��ý���ҳ���е�ʵ���ڴ���
	for (auto const& item : pcb->pageTable) {
		pcbBlocksId.push_back(item.memBlockId);
	}
	memorysystem.Free(pcb->pid, pcbBlocksId);
	return true;
}


bool Kernel::SysOpenFile(const std::string& fileName) {
	// ��Ӧ���̻����Ӧ�ļ���Ȩ��
	auto targetFcbPoint = directory.OpenFile(fileName);
	sysOpenFiles.push_back(targetFcbPoint);
	return true;
}


bool Kernel::SysReadFile(const std::string& fileName) {
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
	std::shared_ptr<PCB> pcb = Fork();  // ��������
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
			auto replaceAllocMem = this->PageReplaceInterrupt(pageNumToReplace);
			allocMem.insert(allocMem.end(), replaceAllocMem.begin(), replaceAllocMem.end());
		}
		pcb->AppendMmu(allocMem);
	}

	// ���ļ����ݴ�������
	ProcessCommunication::Send("DISK", "Read", targetFcbPoint->idxBlocksId);
	std::string dataReadFromDisk = ProcessCommunication::Receive("DISK");

	if (dataReadFromDisk == "Error") {
		return false;
	}

	// Ready 
	std::string subString = dataReadFromDisk.substr(0, 6);

	// ���ļ�����д���ڴ�
	for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
		short toWriteMemBlockId = pcb->pageTable[ii].memBlockId;
		std::vector<short> memBlockIdToWrite = { toWriteMemBlockId };

		try {
			std::string blockDataToWrite = subString.substr(ii * BLOCK_SIZE, BLOCK_SIZE);
			ProcessCommunication::Send("MEMORY", "Write", memBlockIdToWrite, blockDataToWrite);
			std::string msgReceiveFromMem = ProcessCommunication::Receive("MEMORY");

			if (msgReceiveFromMem == "Error") {
				return false;
			}
		}
		catch (const std::exception&) {
			break;
		}
	}
	this->readyQueue.push_back(pcb);
	//this->Exit(pcb);  // �˳�����
	return true;
}


bool Kernel::SysWriteFile(const std::string& fileName) {
	std::shared_ptr<PCB> pcbPoint = Fork();  // ��������
	std::string dataToWrite;                 // Ҫд�������
	if (!pcbPoint) {  // ���̴���ʧ��
	
	}
	else {
		auto targetFcbPoint = directory.OpenFile(fileName);
		if (targetFcbPoint == nullptr) {
			return false;
		}
		std::cout << "������Ҫд������ݣ�" << std::endl;
		std::getline(std::cin, dataToWrite);
		// �õ�Ҫд�����ݵĴ�С
		size_t sizeInBytes = sizeof(dataToWrite);
		if (targetFcbPoint->len * BLOCK_SIZE < sizeInBytes) {  // �ļ���С����Ҫ�Ӵ�������ռ�
			std::cout << "�ļ���С����Ҫ�Ӵ�������ռ�" << std::endl;
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

		ProcessCommunication::Send("DISK", "WriteFileBlock", targetFcbPoint->idxBlocksId, dataToWrite);
		std::cout << "����Ҫ��ʼ��������" << std::endl;
		std::string msgReceiveFromDisk = ProcessCommunication::Receive("DISK");

		if (msgReceiveFromDisk == "Error") {
			return false;
		}
	}
	this->readyQueue.push_back(pcbPoint);
	// this->Exit(pcbPoint);  // �˳�����
	return true;
}


bool Kernel::SysCloseFile(const std::string& fileName) {
	for (size_t ii = 0; ii < sysOpenFiles.size(); ii++) {
		if (sysOpenFiles[ii]->name == fileName) {
			sysOpenFiles.erase(sysOpenFiles.begin() + ii);
			return true;
		}
	}
	std::cout << "��δ�򿪸��ļ���" << std::endl;
	return false;
}


bool Kernel::SysDeleteFile(const std::string& fileName) {
	// �õ�Ҫɾ���ļ�������������
	auto toRemoveFileIdxBlocksId = directory.Rm(fileName);
	if (!toRemoveFileIdxBlocksId.empty())
		disk.FreeDisk(toRemoveFileIdxBlocksId);
	return true;
}


bool Kernel::SysCreateFile(const std::string& fileName) {
	auto idxBlocksId = disk.AllocFileBlock();
	directory.Create(fileName, idxBlocksId);
	return true;
}


bool Kernel::SysMoveFile(const std::string& targetMoveFile, const std::string& targetFolder) {

	return false;
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
bool Kernel::SysCall(Command command, const std::string eax) {
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
	case FILE_CREATE: {
		if (targetFile == "create") {
			isCommandSuccess = false;
		}
		else isCommandSuccess = this->SysCreateFile(targetFile);
		break;
	}
	case FILE_BACK:
		directory.Back();         
		break;
	case FILE_OPEN:
		isCommandSuccess = this->SysOpenFile(targetFile);
		break;
	case FILE_CLOSE:
		isCommandSuccess = this->SysCloseFile(targetFile);
		break;
	case FILE_WRITE:
		isCommandSuccess = this->SysWriteFile(targetFile);
		break;
	case FILE_READ:
		isCommandSuccess = this->SysReadFile(targetFile);
		break;
	case FILE_RM:
		isCommandSuccess = this->SysDeleteFile(targetFile);
		break;
	case FILE_MOVE:
	{
		std::istringstream iss(eax);
		std::vector<std::string> tokens;

		std::string token;
		while (iss >> token) {
			tokens.push_back(token);
		}
		isCommandSuccess = this->SysMoveFile(tokens[1], tokens[2]);
		break;
	}
	default:
		break;
	}
	return isCommandSuccess;
}
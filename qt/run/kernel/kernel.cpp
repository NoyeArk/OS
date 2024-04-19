#include "kernel.h"

Kernel::Kernel() {
	// ����Ŀ¼����ʼ������λ��ͼ
	//this->SysCreateFile("test");
	//this->SysCreateFile("a");
}


std::unique_ptr<PCB> Kernel::Fork() {
	// ʹ�� C++11 ����������
	std::random_device rd;   // ��ȡ�����豸
	std::mt19937 mt(rd());   // ʹ�� Mersenne Twister ����
	std::uniform_int_distribution<int> dist(1, 10000);  // ���巶Χ

	// ����ָ�뽫���䳬����Χʱ�Զ��ͷ��ڴ�
    std::unique_ptr<PCB> p(new PCB);
	p->pid = dist(mt);  // ��������ID
	std::vector<int> allocMem = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (!allocMem.empty()) {
		for (int ii = 0; ii < InitProcessBlockNum; ii++) {
			// ��ҳ�� ����������
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
	// �õ��ý���ҳ���е�ʵ���ڴ�����
	for (auto const& item : pcb->pageTable) {
		pcbBlocksId.push_back(item.memBlockId);
	}
	memorysystem.Free(pcb->pid, pcbBlocksId);
	return true;
}


FCB* Kernel::SysOpenFile(FCB* &parent_fcb, const std::string& fileName) {
    // ��Ӧ���̻����Ӧ�ļ���Ȩ��
    //auto targetFcbPoint = directory.OpenFile(fileName);
    std::cout << "Ҫ�򿪵��ļ���" << fileName << std::endl;
    for (size_t ii = 0; ii < parent_fcb->childFiles.size(); ii++) {
        if (parent_fcb->childFiles[ii]->name == fileName) {
            parent_fcb->childFiles[ii]->authorization = emDelDenied;
            sysOpenFiles.push_back(parent_fcb->childFiles[ii]);
            break;
        }
        std::cout << parent_fcb->childFiles[ii]->name << std::endl;
    }
    std::cout << "num of open files:" << sysOpenFiles.size() << std::endl;
    return parent_fcb;
}


bool Kernel::SysReadFile(const std::string& fileName, std::vector<std::string>& fileData) {
	FCB* targetFcbPoint = nullptr;
    std::shared_ptr<PCB> targetPcb;
    std::vector<std::string> dataReadFromMem;
    qDebug() << "fileName:" << QString::fromStdString(fileName);
    std::cout << "num of open files:" << sysOpenFiles.size() << std::endl;
	for (auto fcb : sysOpenFiles) {
        qDebug() << "fcb->name:" << QString::fromStdString(fcb->name);
		if (fcb->name == fileName) {
			targetFcbPoint = fcb;
			break;
		}
	}
	if (targetFcbPoint == nullptr) {
        std::cout << "�����ȴ򿪸��ļ���" << std::endl;
		return false;
	}
    if (targetFcbPoint->isInMem) {
        for (auto pcb : readyQueue) {
            for (auto fcb : pcb->openFiles) {
                if (fcb->name == fileName) {
                    targetPcb = pcb;
                    break;
                }
            }
        }
        for (size_t ii = 0; ii < targetPcb->pageTable.size(); ii++) {
            char* data = memorysystem.ReadMem(targetPcb->pageTable[ii].memBlockId);
            dataReadFromMem.push_back(std::string(data));
        }
        fileData = dataReadFromMem;
        std::cout << "read from mem" << std::endl;
        return true;
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
    auto blockDatas = disk.ReadFile(targetFcbPoint->idxBlocksId, targetFcbPoint->len);
    // ������ý��̵��ڴ����ܲ�������������Ҫһ��һ�鸴��
    for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
        // offset�������mem��ƫ����
        int toWriteBlockId = pcb->pageTable[ii].memBlockId;
        char* blockData = new char[BLOCK_SIZE];
        std::memcpy(blockData, blockDatas[ii].c_str(), BLOCK_SIZE);
        memorysystem.WriteMem(toWriteBlockId, blockData);
        delete[] blockData;
    }
    //	ProcessCommunication::Send("DISK", "Read", targetFcbPoint->idxBlocksId);
//	std::string dataReadFromDisk = ProcessCommunication::Receive("DISK");

//	if (dataReadFromDisk == "Error") {
//		return false;
//	}

	// Ready 
//    std::string fileContent = dataReadFromDisk.substr(6);
    fileData = blockDatas;

    // ���ļ�����д���ڴ�
//	for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
//		short toWriteMemBlockId = pcb->pageTable[ii].memBlockId;
//		std::vector<short> memBlockIdToWrite = { toWriteMemBlockId };

//		try {
//            std::string blockDataToWrite = fileContent.substr(ii * BLOCK_SIZE, BLOCK_SIZE);
//			ProcessCommunication::Send("MEMORY", "Write", memBlockIdToWrite, blockDataToWrite);
//			std::string msgReceiveFromMem = ProcessCommunication::Receive("MEMORY");

//			if (msgReceiveFromMem == "Error") {
//				return false;
//			}
//		}
//		catch (const std::exception&) {
//			break;
//		}
//	}

	this->readyQueue.push_back(pcb);
	return true;
}


bool Kernel::SysWriteFile(FCB* &fcb, const std::string& fileName, const std::string& dataToWrite) {
    std::unique_ptr<PCB> pcbPoint = Fork();  // ��������
    if (!pcbPoint) {  // ���̴���ʧ��

    }
    else {
        FCB* targetFcbPoint = nullptr;
        // auto targetFcbPoint = directory.OpenFile(fileName);
//        std::cout << "num of open files:" << sysOpenFiles.size() << std::endl;
//        for (auto fcb : sysOpenFiles) {
//            qDebug() << "fcb->name:" << QString::fromStdString(fcb->name);
//            if (fcb->name == fileName) {
//                targetFcbPoint = fcb;
//                break;
//            }
//        }
//        if (targetFcbPoint == nullptr) {
//            return false;
//        }
        std::cout << "������Ҫд������ݣ�" << std::endl;
        // �õ�Ҫд�����ݵĴ�С
        size_t sizeInBytes = sizeof(dataToWrite);
        if (fcb->len * BLOCK_SIZE < sizeInBytes) {  // �ļ���С����Ҫ�Ӵ�������ռ�
            std::cout << "�ļ���С����Ҫ�Ӵ�������ռ�" << std::endl;
            // �����Ҫ����Ĵ��̿���
            size_t applyBlockNum = (fcb->len * BLOCK_SIZE - sizeInBytes) / 40 + 1;
            std::vector<short> newIdxBlocksId = disk.AllocDisk(fcb->idxBlocksId.back(), fcb->len, applyBlockNum);
            if (newIdxBlocksId.empty()) {
                std::cout << "д��ʧ�ܣ�" << std::endl;
                return false;
            }
            // ���¸��ļ���Ӧ��FCB��������
            fcb->ExpandFileLen(newIdxBlocksId, applyBlockNum);
        }

        disk.WriteFile(fcb->idxBlocksId, dataToWrite);

//        ProcessCommunication::Send("DISK", "WriteFileBlock", targetFcbPoint->idxBlocksId, dataToWrite);
//        std::cout << "����Ҫ��ʼ��������" << std::endl;
//        std::string msgReceiveFromDisk = ProcessCommunication::Receive("DISK");

//        if (msgReceiveFromDisk == "Error") {
//            return false;
//        }
    }
    this->Exit(pcbPoint);  // �˳�����
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


bool Kernel::SysDeleteFile(FCB* &fcb, const std::string& fileName) {
    // �õ�Ҫɾ���ļ�������������
    auto toRemoveFileIdxBlocksId = directory.Rm(fcb, fileName);
    if (!toRemoveFileIdxBlocksId.empty() && toRemoveFileIdxBlocksId[0] != -1) {
        // ɾ�������ļ�
        disk.FreeDisk(toRemoveFileIdxBlocksId);
        return false;
    }
    else if (!toRemoveFileIdxBlocksId.empty() && toRemoveFileIdxBlocksId[0] == -1){
        // ɾ��Ŀ¼�ļ�
        return true;
    }
    return false;
}


bool Kernel::SysCreateFile(FCB* &fcb, const std::string& fileName) {
	auto idxBlocksId = disk.AllocFileBlock();
    directory.Create(fcb, fileName, idxBlocksId);
	return true;
}


bool Kernel::SysMoveFile(const std::string& targetMoveFile, const std::string& targetFolder) {

    return false;
}


std::vector<int> Kernel::PageReplaceInterrupt(int pageNumToReplace) {
    std::vector<std::pair<int, int>> replaceBlocksId = memorysystem.PagesReplace(this->readyQueue, pageNumToReplace);
    // replaceBlocksId�еĿ��Ӧ�ڴ�����ݻ������һ���
	std::vector<char*> blocksData;
    std::vector<int> ids;
    // ������һ��һ��Ķ���
	for (auto blockId : replaceBlocksId) {
        char* blockData = memorysystem.ReadMem(blockId.second);
		blocksData.push_back(blockData);
        ids.push_back(blockId.second);
        for (size_t ii = 0; ii < readyQueue.size(); ii++) {
            if (readyQueue[ii]->pid == blockId.first) {
                for (size_t jj = 0; jj < readyQueue[ii]->pageTable.size(); jj++) {
                    if (readyQueue[ii]->pageTable[jj].memBlockId == blockId.second) {
                        readyQueue[ii]->pageTable[jj].isInMem = false;
                    }
                }
            }
        }
	}
    // д����̶һ���
	disk.WriteSwap(blocksData);

    return ids;
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
//        directory.mkdir(targetFile);        break;
	case FILE_RMDIR:
		directory.rmdir();        break;
	case FILE_LS:
		directory.Ls();           break;
	case FILE_CD: 
		directory.cd(targetFile); break;
    case _FILE_CREATE: {
		if (targetFile == "create") {
			isCommandSuccess = false;
		}
//		else isCommandSuccess = this->SysCreateFile(targetFile);
		break;
	}
	case FILE_BACK:
		directory.Back();         
		break;
	case FILE_OPEN:
//		isCommandSuccess = this->SysOpenFile(targetFile);
		break;
	case FILE_CLOSE:
		isCommandSuccess = this->SysCloseFile(targetFile);
		break;
	case FILE_WRITE:
//        isCommandSuccess = this->SysWriteFile(targetFile, "");
		break;
	case FILE_READ:
//        isCommandSuccess = this->SysReadFile(targetFile, targetFile);
		break;
	case FILE_RM:
//        isCommandSuccess = this->SysDeleteFile(targetFile);
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

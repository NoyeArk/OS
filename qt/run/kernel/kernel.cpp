#include "kernel.h"

Kernel::Kernel() {
	// 锟斤拷锟斤拷目录锟斤拷锟斤拷始锟斤拷锟斤拷锟斤拷位锟斤拷图
	//this->SysCreateFile("test");
	//this->SysCreateFile("a");
}


std::unique_ptr<PCB> Kernel::Fork() {
	// 使锟斤拷 C++11 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
	std::random_device rd;   // 锟斤拷取锟斤拷锟斤拷锟借备
	std::mt19937 mt(rd());   // 使锟斤拷 Mersenne Twister 锟斤拷锟斤拷
	std::uniform_int_distribution<int> dist(1, 10000);  // 锟斤拷锟藉范围

	// 锟斤拷锟斤拷指锟诫将锟斤拷锟戒超锟斤拷锟斤拷围时锟皆讹拷锟酵凤拷锟节达拷
    std::unique_ptr<PCB> p(new PCB);
	p->pid = dist(mt);  // 锟斤拷锟斤拷锟斤拷锟斤拷ID
	std::vector<int> allocMem = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (!allocMem.empty()) {
		for (int ii = 0; ii < InitProcessBlockNum; ii++) {
			// 锟斤拷页锟斤拷 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
			int virtualPageNumber = ii;
			p->pageTable.push_back(PageTableItem(allocMem[ii], std::chrono::system_clock::now()));
		}
	}
	else {
		std::cout << "锟斤拷锟教达拷锟斤拷失锟斤拷" << std::endl;
		return nullptr;
	}
	return p;
}


bool Kernel::Exit(const std::unique_ptr<PCB>& pcb) {
	std::vector<int> pcbBlocksId;
	// 锟矫碉拷锟矫斤拷锟斤拷页锟斤拷锟叫碉拷实锟斤拷锟节达拷锟斤拷锟斤拷
	for (auto const& item : pcb->pageTable) {
		pcbBlocksId.push_back(item.memBlockId);
	}
	memorysystem.Free(pcb->pid, pcbBlocksId);
	return true;
}


FCB* Kernel::SysOpenFile(FCB* &parent_fcb, const std::string& fileName) {
    // 对应进程获得相应文件的权限
    //auto targetFcbPoint = directory.OpenFile(fileName);
    std::cout << "要打开的文件是" << fileName << std::endl;
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
        std::cout << "·需先打开该文件！" << std::endl;
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
    std::shared_ptr<PCB> pcb = Fork();  // 创建进程
    if (!pcb) {  // 进程创建失败
        std::cout << "打开文件" << fileName << "失败" << std::endl;
		return false;
	}
	if (targetFcbPoint->len > pcb->pageTable.size()) {
        // 8块内存不够，继续申请
		int requestBlockNum = targetFcbPoint->len - pcb->pageTable.size();
		std::vector<int> allocMem = memorysystem.Alloc(pcb->pid, requestBlockNum);
		if (requestBlockNum > allocMem.size()) {
            // 需要进行页置换：LRU
			size_t pageNumToReplace = requestBlockNum - allocMem.size();
			auto replaceAllocMem = this->PageReplaceInterrupt(pageNumToReplace);
			allocMem.insert(allocMem.end(), replaceAllocMem.begin(), replaceAllocMem.end());
		}
		pcb->AppendMmu(allocMem);
	}

    // 将文件数据从外存读出
    auto blockDatas = disk.ReadFile(targetFcbPoint->idxBlocksId, targetFcbPoint->len);
    // 分配给该进程的内存块可能并不连续，所以要一块一块复制
    for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
        // offset是相对于mem的偏移量
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

    // 将文件数据写入内存
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
    std::unique_ptr<PCB> pcbPoint = Fork();  // 创建进程
    if (!pcbPoint) {  // 进程创建失败

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
        std::cout << "请输入要写入的数据：" << std::endl;
        // 得到要写入数据的大小
        size_t sizeInBytes = sizeof(dataToWrite);
        if (fcb->len * BLOCK_SIZE < sizeInBytes) {  // 文件过小，需要从磁盘申请空间
            std::cout << "文件过小，需要从磁盘申请空间" << std::endl;
            // 算出需要申请的磁盘块数
            size_t applyBlockNum = (fcb->len * BLOCK_SIZE - sizeInBytes) / 40 + 1;
            std::vector<short> newIdxBlocksId = disk.AllocDisk(fcb->idxBlocksId.back(), fcb->len, applyBlockNum);
            if (newIdxBlocksId.empty()) {
                std::cout << "写入失败！" << std::endl;
                return false;
            }
            // 更新该文件对应的FCB中索引块
            fcb->ExpandFileLen(newIdxBlocksId, applyBlockNum);
        }

        disk.WriteFile(fcb->idxBlocksId, dataToWrite);

//        ProcessCommunication::Send("DISK", "WriteFileBlock", targetFcbPoint->idxBlocksId, dataToWrite);
//        std::cout << "马上要开始接受数据" << std::endl;
//        std::string msgReceiveFromDisk = ProcessCommunication::Receive("DISK");

//        if (msgReceiveFromDisk == "Error") {
//            return false;
//        }
    }
    this->Exit(pcbPoint);  // 退出进程
    return true;
}


bool Kernel::SysCloseFile(const std::string& fileName) {
	for (size_t ii = 0; ii < sysOpenFiles.size(); ii++) {
		if (sysOpenFiles[ii]->name == fileName) {
			sysOpenFiles.erase(sysOpenFiles.begin() + ii);
			return true;
		}
	}
	std::cout << "锟斤拷未锟津开革拷锟侥硷拷锟斤拷" << std::endl;
	return false;
}


bool Kernel::SysDeleteFile(FCB* &fcb, const std::string& fileName) {
    // 得到要删除文件的外存索引块号
    auto toRemoveFileIdxBlocksId = directory.Rm(fcb, fileName);
    if (!toRemoveFileIdxBlocksId.empty() && toRemoveFileIdxBlocksId[0] != -1) {
        // 删除数据文件
        disk.FreeDisk(toRemoveFileIdxBlocksId);
        return false;
    }
    else if (!toRemoveFileIdxBlocksId.empty() && toRemoveFileIdxBlocksId[0] == -1){
        // 删除目录文件
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
    // replaceBlocksId中的块对应内存的数据换到外存兑换区
	std::vector<char*> blocksData;
    std::vector<int> ids;
    // 将数据一块一块的读出
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
    // 写入磁盘兑换区
	disk.WriteSwap(blocksData);

    return ids;
}


std::string Kernel::getCurPath() {
	return directory.getCurPath();
}


// 锟斤拷锟斤拷使锟斤拷command锟斤拷锟斤拷锟斤拷锟叫讹拷锟斤拷锟酵号ｏ拷应锟矫宏定锟藉几锟斤拷锟叫讹拷锟斤拷锟斤拷
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

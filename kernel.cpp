#include "kernel.h"

Kernel::Kernel() {
	// 根据目录来初始化磁盘位视图
	//for (const auto& file : )
	this->Create("test");
}


std::unique_ptr<PCB> Kernel::Fork() {
	// 使用 C++11 随机数引擎
	std::random_device rd;   // 获取随机设备
	std::mt19937 mt(rd());   // 使用 Mersenne Twister 引擎
	std::uniform_int_distribution<int> dist(1, 10000);  // 定义范围

	// 智能指针将在其超出范围时自动释放内存
	std::unique_ptr<PCB> p = std::make_unique<PCB>();
	p->pid = dist(mt);  // 分配进程ID
	std::vector<int> allocMem = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (!allocMem.empty()) {
		for (int ii = 0; ii < InitProcessBlockNum; ii++) {
			// 虚页号 ：物理块号
			int virtualPageNumber = ii;
			p->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
		}
	}
	else {
		std::cout << "进程创建失败" << std::endl;
		return nullptr;
	}
	return p;
}


void Kernel::Exit(const std::unique_ptr<PCB>& pcb) {
	// 释放进程中的内存空间
	std::vector<int> pcbBlocksId;
	std::transform(pcb->pageTable.begin(), pcb->pageTable.end(), std::back_inserter(pcbBlocksId),
		[](const std::pair<int, int>& pair) { return pair.second; });
	memorysystem.Free(pcb->pid, pcbBlocksId);
}


void Kernel::Open(const std::string& fileName) {
	// 对应进程获得相应文件的权限
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
		std::cout << "·需先打开该文件！" << std::endl;
		return;
	}
	std::unique_ptr<PCB> pcb = Fork();  // 创建进程
	if (!pcb) {  // 进程创建失败
		std::cout << "打开文件" << fileName << "失败" << std::endl;
	}
	else {
		if (targetFcbPoint->len > pcb->pageTable.size()) {
			// 8块内存不够，继续申请
			int requestBlockNum = targetFcbPoint->len - pcb->pageTable.size();
			std::vector<int> allocMem = memorysystem.Alloc(pcb->pid, requestBlockNum);
			if (requestBlockNum > allocMem.size()) {
				// 需要进行页置换：LRU
				size_t pageNumToReplace = requestBlockNum - allocMem.size();
				auto replaceAllocMem = this->PageReplaceInterrupt(pcb->pid, pageNumToReplace);
				allocMem.insert(allocMem.end(), replaceAllocMem.begin(), replaceAllocMem.end());
			}
			pcb->AppendMmu(allocMem);
		}
		// 将文件数据从外存读入内存
		auto blockDatas = disk.ReadFile(targetFcbPoint->idxBlocksId, targetFcbPoint->len);
		// 分配给该进程的内存块可能并不连续，所以要一块一块复制
		for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
			// offset是相对于mem的偏移量
			int toWriteBlockId = pcb->pageTable[ii].second;
			char* blockData = new char[BLOCK_SIZE];
			std::memcpy(blockData, blockDatas[ii], BLOCK_SIZE);
			memorysystem.WriteMem(toWriteBlockId, blockData);
			delete[] blockData;
		}
	}
	this->Exit(pcb);  // 退出进程
}


void Kernel::Write(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();  // 创建进程
	std::string dataToWrite;                 // 要写入的数据
	if (!pcbPoint) {  // 进程创建失败
	
	}
	else {
		auto targetFcbPoint = directory.OpenFile(fileName);
		std::getline(std::cin, dataToWrite);
		// 得到要写入数据的大小
		size_t sizeInBytes = sizeof(dataToWrite);
		if (targetFcbPoint->len * BLOCK_SIZE < sizeInBytes) {  // 文件过小，需要从磁盘申请空间
			// 算出需要申请的磁盘块数
			size_t applyBlockNum = (targetFcbPoint->len * BLOCK_SIZE - sizeInBytes) / 40 + 1;
			std::vector<short> newIdxBlocksId = disk.AllocDisk(targetFcbPoint->idxBlocksId.back(), targetFcbPoint->len, applyBlockNum);
			if (newIdxBlocksId.empty()) {
				std::cout << "写入失败！" << std::endl;
				return;
			}
			// 更新该文件对应的FCB中索引块
			targetFcbPoint->ExpandFileLen(newIdxBlocksId, applyBlockNum);
		}
		disk.WriteFile(targetFcbPoint->idxBlocksId, dataToWrite);
	}
	this->Exit(pcbPoint);  // 退出进程
}


void Kernel::Close(const std::string& fileName) {
	for (size_t ii = 0; ii < sysOpenFiles.size(); ii++) {
		if (sysOpenFiles[ii]->name == fileName) {
			sysOpenFiles.erase(sysOpenFiles.begin() + ii);
			return;
		}
	}
	std::cout << "·未打开该文件！" << std::endl;
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
	// replaceBlocksId中的块对应内存的数据换到外存兑换区
	std::vector<char*> blocksData;
	// 将数据一块一块的读出
	for (auto blockId : replaceBlocksId) {
		char* blockData = memorysystem.ReadMem(blockId);
		blocksData.push_back(blockData);
	}
	// 写入磁盘兑换区
	disk.WriteSwap(blocksData);

	return replaceBlocksId;
}


std::string Kernel::getCurPath() {
	return directory.getCurPath();
}


// 这里使用command来代替中断类型号，应该宏定义几个中断类型
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

#include "kernel.h"


std::unique_ptr<PCB> Kernel::Fork() {
	// 使用 C++11 随机数引擎
	std::random_device rd;   // 获取随机设备
	std::mt19937 mt(rd());   // 使用 Mersenne Twister 引擎
	std::uniform_int_distribution<int> dist(1, 10000);  // 定义范围

	// 智能指针将在其超出范围时自动释放内存
	std::unique_ptr<PCB> p = std::make_unique<PCB>();
	p->pid = dist(mt);  // 分配进程ID
	int beginPhysicalBlockNumber = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (beginPhysicalBlockNumber != INFINITY) {
		for (size_t ii = 0; ii < InitProcessBlockNum; ii++) {
			// 虚页号 ：物理块号
			int virtualPageNumber = ii;
			p->pageTable.push_back(std::make_pair(virtualPageNumber, beginPhysicalBlockNumber + ii));
		}
	}
	else {
		std::cout << "进程创建失败" << std::endl;
		return nullptr;
	}
	return p;
}


void Kernel::Exit(const std::unique_ptr<PCB>& process) {
	// 释放进程中的内存空间，一块一块回收
	for (auto item : process->pageTable) {
		memorysystem.Free(process->pid, );
	}
}


void Kernel::Open(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();
	if (!pcbPoint) {  // 进程创建失败
		std::cout << "打开文件" << fileName << "失败" << std::endl;
	}
	else {
		auto targetFcbPoint = directory.open(fileName);
		if (targetFcbPoint->len > pcbPoint->pageTable.size()) {
			// 8块内存不够，继续申请
			int applyBlockNum = targetFcbPoint->len - pcbPoint->pageTable.size();
			int beginAddr = memorysystem.Alloc(pcbPoint->pid, applyBlockNum);
			pcbPoint->AppendMmu(beginAddr, applyBlockNum);
		}
		// 将文件数据从外存读入内存
		auto buff = disk.Read(targetFcbPoint->addr, targetFcbPoint->len);
		// 分配给该进程的内存块可能并不连续，所以要一块一块复制
		for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
			// offset是相对于mem的偏移量
			int offset = pcbPoint->pageTable[ii].second * BLOCK_SIZE;
			char* blockData = new char[BLOCK_SIZE];
			std::memcpy(blockData, buff + ii, BLOCK_SIZE);
			memorysystem.AssignMem(offset, blockData);
		}
		memorysystem.DisplayMemUsage();
	}
}


void Kernel::Close() {

}


std::string Kernel::getCurPath() {
	return directory.getCurPath();
}


// 这里使用command来代替中断类型号，应该宏定义几个中断类型
void Kernel::SysCall(COMMAND command, const std::string eax) {
	std::string targetFile = eax.substr(eax.find(" ") + 1);
	switch (command)
	{
	// File指令
	case FILE_FORMAT:
		directory.format();
		break;
	case FILE_MKDIR:
		directory.mkdir();
		break;
	case FILE_RMDIR:
		directory.rmdir();
		break;
	case FILE_LS:
		directory.ls();
		break;
	case FILE_CD: 
		directory.cd(targetFile);
		break;
	case FILE_CREATE:
		directory.create();
		break;
	case FILE_BACK:
		directory.back();
		break;
	case FILE_OPEN:
		Open(targetFile);
		break;
	case FILE_CLOSE:
		directory.close();
		break;
	case FILE_WRITE:
		directory.write();
		break;
	case FILE_READ:
		directory.read();
		break;
	case FILE_RM:
		directory.rm();
		break;
	default:
		break;
	}
}

#include "my_memory.h"

void Memory::DisplayMemUsage() {  
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "====" << ++memScheTime << "====" << std::endl;

	std::cout << "  高地址" << std::endl;

	for (int ii = memAllocList.size() - 1; ii >= 0; --ii) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "|       |" << std::endl;
		if (memAllocList[ii].blockNum < 100)
			std::cout << "|" << memAllocList[ii].pid << ":" << memAllocList[ii].blockNum << "kb |" << std::endl;
		else std::cout << "|" << memAllocList[ii].pid << ":" << memAllocList[ii].blockNum << "kb|" << std::endl;
		std::cout << "|_______|" << std::endl;
	}
}


void Memory::PrintMemUsage() {
	for (auto it = memAllocList.begin(); it != memAllocList.end(); ++it) {
		std::cout << it->pid;
		if (it != std::prev(memAllocList.end()))
			std::cout << "→";
	}
	std::cout << std::endl;
}


int Memory::FirstFit(int requestBlockNum) {  // 首次适应
	for (int ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == NULL && memAllocList[ii].blockNum >= requestBlockNum) {
			return ii;
		}
	}
	return INFINITY;
}


int Memory::BestFit(int allocSize) {  // 最佳适应
	int bestLoc = -1;
	int bestSize = 1000000000;
	for (int ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == 0 && memAllocList[ii].blockNum >= allocSize)
			if (memAllocList[ii].blockNum <= bestSize) {
				bestLoc = ii;
				bestSize = memAllocList[ii].blockNum;
			}
	}
	return bestLoc;
}


int Memory::WorstFit(int allocSize) {  // 最坏适应
	int worstLoc = -1;
	int worstSize = 0;
	for (int ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == 0 && memAllocList[ii].blockNum >= allocSize)
			if (memAllocList[ii].blockNum > worstSize) {
				worstLoc = ii;
				worstSize = memAllocList[ii].blockNum;
			}
	}
	return worstLoc;
}


void Memory::MergeAvailableBlock() {
	// 从头开始遍历，找第一个空闲分区
	for (auto it = memAllocList.begin(); it != memAllocList.end(); ++it) {
		if (it->pid != NULL)
			continue;
		auto nextIt = std::next(it);
		// 下一个位置不为结束 并且 也为空闲分区则进行合并
		if (nextIt != memAllocList.end() && nextIt->pid == NULL) {
			it->blockNum += nextIt->blockNum;
			it = memAllocList.erase(nextIt);
			--it;
		}
	}
}


Memory::Memory() {
	// 申请一块大小为2560字节的内存
	mem = new char[MEM_SIZE];

	// 对内存进行二进制操作，初始化所有位设置为0
	std::memset(mem, 0x00, MEM_SIZE);

	memAllocList.push_back(MCB(NULL, 0, BLOCK_NUM));
	memScheTime = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


Memory::~Memory() {
	// 释放动态申请的内存
	delete mem;
}


/**
 * \name Alloc             进程申请内存
 * \type std::vector<int>  申请的内存物理块号集合
 * \param pid              申请进程号
 * \param requestBlockNum  申请物理块数
 * \brief                  申请得到的内存块可能并不连续
 */
std::vector<int> Memory::Alloc(int pid, int requestBlockNum) {
	std::vector<int> allocateMem;
	// firstFit  bestFit  worstFit
	int listIdx = FirstFit(requestBlockNum);
	if (listIdx != INFINITY) {
		// 对互斥量memAllocList进行操作 加锁
		std::lock_guard<std::mutex> lock(listMutex);
		// 更新内存使用链表
		int beginAddr = memAllocList[listIdx].beginLoc + memAllocList[listIdx].blockNum;
		memAllocList.insert(memAllocList.begin() + listIdx, MCB(pid, beginAddr, requestBlockNum));
		memAllocList[listIdx + 1].blockNum -= requestBlockNum;
		
		for (size_t ii = 0; ii < requestBlockNum; ii++) {
			allocateMem.push_back(beginAddr + ii);
		}
		return allocateMem;
	}
	else {
		std::cout << "memory is not available!" << std::endl;
		return {};
	}
	//DisplayMemUsage(loc, true);
}

/**
 * \name Free          释放进程所拥有的内存
 * \param pid		   进程号
 * \param blockNumber  释放内存块数
 * \brief              一次性释放全部的内存
 */
void Memory::Free(const int& pid) {
	std::lock_guard<std::mutex> lock(listMutex);
	for (size_t ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == pid) {
			memAllocList[ii].pid = NULL;
			this->MergeAvailableBlock();
		}
	}
}


void Memory::AssignMem(int offset, char* blockData) {
	std::lock_guard<std::mutex> lock(memMutex);
	std::memcpy(mem, blockData, BLOCK_SIZE);
}
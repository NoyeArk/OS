#include "my_memory.h"

MCB::MCB() {
	this->pid = NULL_PROCESS;
	this->lastUseTime = INFINITY;
}

MCB::MCB(int pid, std::chrono::system_clock::time_point lastUseTime) : pid(pid) {
	// 将当前时间转换为整数
	auto currentTimeInt = std::chrono::duration_cast<std::chrono::seconds>(lastUseTime.time_since_epoch()).count();
	this->lastUseTime = static_cast<int>(currentTimeInt);
}


/**
 * \name LRU 找据此时时间间隔最长的一个内存块
 * \type int 找到的内存块下标
 * \return 
 */
int Memory::LRU(int pid) {
	int currentTime = GetCurrentIntTime();
	int maxTimeDifference = -1;
	int lruBlockId;
	for (size_t blockId = 0; blockId < MEM_BLOCK_NUM; blockId++) {
		if (memAllocList[blockId].pid != pid && 
			currentTime - memAllocList[blockId].lastUseTime > maxTimeDifference) {
			maxTimeDifference = currentTime - memAllocList[blockId].lastUseTime;
			lruBlockId = blockId;
		}
	}
	return lruBlockId;
}


int Memory::GetCurrentIntTime() {
	auto currentTime = std::chrono::system_clock::now();
	auto currentTimeInt = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
	int intTime = static_cast<int>(currentTimeInt);
	return intTime;
}


std::vector<int> Memory::PagesReplace(int pid, int pageNumToReplace) {
	std::vector<int> replaceBlocksId;  // 需要被置换到外存的内存块号
	for (size_t ii = 0; ii < pageNumToReplace; ii++) {
		int lruBlockId = this->LRU(pid);
		replaceBlocksId.push_back(lruBlockId);
	}
	return replaceBlocksId;
}


Memory::Memory() {
	// 初始包含64个块
	memAllocList.resize(MEM_BLOCK_NUM);
	// 申请一块大小为2560字节的内存
	mem = new char[MEM_SIZE];

	// 对内存进行二进制操作，初始化所有位设置为0
	std::memset(mem, 0x00, MEM_SIZE);

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
	// 对互斥量memAllocList进行操作 加锁
	std::lock_guard<std::mutex> lock(listMutex);
	for (size_t blockId = 0; blockId < MEM_BLOCK_NUM; blockId++) {
		if (memAllocList[blockId].pid == NULL) {
			memAllocList[blockId].pid = pid;
			memAllocList[blockId].lastUseTime = GetCurrentIntTime();
			allocateMem.push_back(blockId);
			if (allocateMem.size() == requestBlockNum)
				break;
		}
	}
	if (allocateMem.size() < requestBlockNum) {
		// 内存不够，进行页面全局置换LRU（最近最久未使用） —— 系统调用PageReplaceInterrupt
		std::cout << "内存不够，进行页面全局置换LRU" << std::endl;
	}
	return allocateMem;
}


/**
 * \name Free          释放进程所拥有的内存
 * \param pid		   进程号
 * \param blockNumber  释放内存块数
 * \brief              一次性释放全部的内存
 */
void Memory::Free(const int& pid, std::vector<int> toFreeBlocksId) {
	std::lock_guard<std::mutex> lock(listMutex);
	for (auto blockId : toFreeBlocksId) {
		if (memAllocList[blockId].pid == pid)
			memAllocList[blockId].pid = NULL_PROCESS;
		else
			std::cout << "进程页表描述项与内存使用链表中描述出现冲突！" << std::endl;
	}
}


void Memory::WriteMem(int blockId, char* blockData) {
	int offset = blockId * MEM_BLOCK_SIZE;
	std::lock_guard<std::mutex> lock(memMutex);
	std::memcpy(mem + offset, blockData, MEM_BLOCK_SIZE);
}


char* Memory::ReadMem(int blockId) {
	int offset = blockId * MEM_BLOCK_SIZE;
	// 创建一个缓冲区来存储从 mem 指定位置开始的数据
	char blockData[MEM_BLOCK_SIZE];  // 以字节为单位
	// 使用 std::memcpy 从 mem 的指定位置复制数据到 buffer 中
	std::memcpy(blockData, mem + offset, MEM_BLOCK_SIZE);
	return blockData;
}
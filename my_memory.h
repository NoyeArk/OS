#ifndef MY_MEMORY_H
#define MY_MEMORY_H

/*********************************************************************
 * \file   my_memory.h
 * \brief  内存实现
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <vector>
#include <Windows.h>

#include "my_thread.h"

#define MEM_SIZE       2560
#define MEM_BLOCK_SIZE 40   // 块大小：40B
#define MEM_BLOCK_NUM  64   // 64块：0~63
#define NULL_PROCESS   0

// 使用空闲链表法组织空闲块
struct MCB {
	int pid;			// 分配进程号
	int lastUseTime;    // 最后一次使用时间

	MCB();
	MCB(int pid, std::chrono::system_clock::time_point lastUseTime);
};


class Memory {
private:
	char* mem;			   // 临界资源
	HANDLE hConsole;
	int memScheTime;
	std::mutex memMutex;   // 内存互斥量
	std::mutex listMutex;  // 内存分配链表互斥量
	std::vector<MCB> memAllocList;  // 临界资源

private:
	int GetCurrentIntTime();
	int LRU(const std::vector<std::shared_ptr<PCB>>& pcbs);

public:
	Memory();
	~Memory();
	
	std::vector<int> Alloc(int pid, int requestBlockNum);
	void Free(const int& pid, std::vector<int> toFreeBlocksId);

	void WriteMem(int blockId, char* blockData);
	char* ReadMem(int blockId);
	std::vector<int> PagesReplace(const std::vector<std::unique_ptr<PCB>>& pcbs, int pageNumToReplace);
};

#endif
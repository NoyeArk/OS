#ifndef MY_MEMORY_H
#define MY_MEMORY_H

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <mutex>
#include <cmath>

#define MEM_SIZE   2560
#define BLOCK_SIZE 320   // 块大小：40B
#define BLOCK_NUM  64    // 64块：0~63
#define NULL       0

// 使用空闲链表法组织空闲块
struct MCB {
	int pid;       // 分配进程号
	int beginLoc;  // 内存起始块号
	int blockNum;  // 申请内存块数
	MCB(int pid, int beginLoc, int blockNum) : pid(pid), beginLoc(beginLoc), blockNum(blockNum)
	{}
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
	void PrintMemUsage();

	int FirstFit(int allocSize);
	int BestFit(int allocSize);
	int WorstFit(int allocSize);

public:
	Memory();
	~Memory();
	
	void DisplayMemUsage();
	int Alloc(int pid, int requestBlockNum);
	void Free(int pid, int freeSize);

	void AssignMem(int offset, char* blockData);
};

#endif
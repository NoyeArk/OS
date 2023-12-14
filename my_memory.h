#ifndef MY_MEMORY_H
#define MY_MEMORY_H


#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <cmath>

#define MEM_SIZE   2560
#define BLOCK_SIZE 320   // 块大小：40B
#define BLOCK_NUM  64    // 64块：0~63

#define NULL       0

// 使用空闲链表法组织空闲块
struct MCB {
	int pid;       // 分配进程号
	int addr;      // 内存起始地址
	int blockNum;  // 申请内存块数
	MCB(int pid, int addr, int blockNum) : pid(pid), addr(addr), blockNum(blockNum) 
	{}
};

class Memory {
private:
	char* mem;

	int memScheTime;
	std::vector<MCB> memAllocList;
	HANDLE hConsole;

	void PrintMemUsage();

	int FirstFit(int allocSize);
	int BestFit(int allocSize);
	int WorstFit(int allocSize);
public:
	Memory();
	~Memory();
	
	void DisplayMemUsage(int loc, bool type);
	void Alloc(int pid, int requestBlockNum);
	void Free(int pid, int freeSize);
};

#endif
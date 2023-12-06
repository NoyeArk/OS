#ifndef MEMORY_H
#define MEMORY_H


#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <cmath>

struct MCB {
	int pid;      // 进程号
	int memSize;  // 申请内存数

	MCB(int pid, int memSize) : pid(pid), memSize(memSize)
	{}
};

class Memory {
private:
	int memScheTime;
	std::vector<MCB> memVector;
	HANDLE hConsole;

	void PrintMemUsage();

	int FirstFit(int allocSize);
	int BestFit(int allocSize);
	int WorstFit(int allocSize);
public:
	void DisplayMemUsage(int loc, bool type);
	Memory();

	void Alloc(int pid, int allocSize);
	void Free(int pid, int freeSize);
};

#endif
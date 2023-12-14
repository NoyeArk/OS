#ifndef MY_MEMORY_H
#define MY_MEMORY_H


#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <cmath>

#define MEM_SIZE   2560
#define BLOCK_SIZE 320   // ���С��40B
#define BLOCK_NUM  64    // 64�飺0~63

#define NULL       0

// ʹ�ÿ���������֯���п�
struct MCB {
	int pid;       // ������̺�
	int addr;      // �ڴ���ʼ��ַ
	int blockNum;  // �����ڴ����
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
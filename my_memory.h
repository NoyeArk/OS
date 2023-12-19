#ifndef MY_MEMORY_H
#define MY_MEMORY_H

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <mutex>
#include <cmath>

#define MEM_SIZE   2560
#define BLOCK_SIZE 320   // ���С��40B
#define BLOCK_NUM  64    // 64�飺0~63
#define NULL       0

// ʹ�ÿ���������֯���п�
struct MCB {
	int pid;       // ������̺�
	int beginLoc;  // �ڴ���ʼ���
	int blockNum;  // �����ڴ����
	MCB(int pid, int beginLoc, int blockNum) : pid(pid), beginLoc(beginLoc), blockNum(blockNum)
	{}
};

class Memory {
private:
	char* mem;			   // �ٽ���Դ
	HANDLE hConsole;
	int memScheTime;
	std::mutex memMutex;   // �ڴ滥����
	std::mutex listMutex;  // �ڴ������������
	std::vector<MCB> memAllocList;  // �ٽ���Դ

private:
	void PrintMemUsage();
	int FirstFit(int allocSize);
	int BestFit(int allocSize);
	int WorstFit(int allocSize);
	void MergeAvailableBlock();

public:
	Memory();
	~Memory();
	
	void DisplayMemUsage();
	std::vector<int> Alloc(int pid, int requestBlockNum);
	void Free(const int& pid);

	void AssignMem(int offset, char* blockData);
};

#endif
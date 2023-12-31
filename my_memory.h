#ifndef MY_MEMORY_H
#define MY_MEMORY_H

/*********************************************************************
 * \file   my_memory.h
 * \brief  �ڴ�ʵ��
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
#define MEM_BLOCK_SIZE 40   // ���С��40B
#define MEM_BLOCK_NUM  64   // 64�飺0~63
#define NULL_PROCESS   0

// ʹ�ÿ���������֯���п�
struct MCB {
	int pid;			// ������̺�
	int lastUseTime;    // ���һ��ʹ��ʱ��

	MCB();
	MCB(int pid, std::chrono::system_clock::time_point lastUseTime);
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
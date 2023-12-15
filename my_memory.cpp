#include "my_memory.h"

void Memory::DisplayMemUsage() {  
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "====" << ++memScheTime << "====" << std::endl;

	std::cout << "  �ߵ�ַ" << std::endl;

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
			std::cout << "��";
	}
	std::cout << std::endl;
}

int Memory::FirstFit(int requestBlockNum) {  // �״���Ӧ
	for (int ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == NULL && memAllocList[ii].blockNum >= requestBlockNum) {
			return ii;
		}
	}
	return INFINITY;
}

int Memory::BestFit(int allocSize) {  // �����Ӧ
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

int Memory::WorstFit(int allocSize) {  // ���Ӧ
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


Memory::Memory() {
	// ����һ���СΪ2560�ֽڵ��ڴ�
	mem = new char[MEM_SIZE];

	// ���ڴ���ж����Ʋ�������ʼ������λ����Ϊ0
	std::memset(mem, 0x00, MEM_SIZE);

	memAllocList.push_back(MCB(NULL, 0, BLOCK_NUM));
	memScheTime = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


Memory::~Memory() {
	// �ͷŶ�̬������ڴ�
	delete mem;
}


int Memory::Alloc(int pid, int requestBlockNum) {
	// firstFit  bestFit  worstFit
	int loc = FirstFit(requestBlockNum);
	if (loc != INFINITY) {
		std::lock_guard<std::mutex> lock(listMutex);

		int beginAddr = memAllocList[loc].beginLoc + memAllocList[loc].blockNum;
		memAllocList.insert(memAllocList.begin() + loc, MCB(pid, beginAddr, requestBlockNum));
		memAllocList[loc + 1].blockNum -= requestBlockNum;
		
		return beginAddr;
	}
	else {
		std::cout << "memory is not available!" << std::endl;
		return INFINITY;
	}
	//DisplayMemUsage(loc, true);
}


void Memory::Free(int pid, int freeSize) {
	int freeLoc = -1;

	std::vector<MCB> tempVector;
	for (size_t ii = 0; ii < memAllocList.size(); ii++) {
		if (memAllocList[ii].pid == pid) {
			memAllocList[ii].pid = 0;
			freeLoc = ii;
			for (auto it = memAllocList.begin(); it != memAllocList.end(); ++it) {
				if (it->pid != 0)
					continue;
				auto nextIt = std::next(it);
				if (nextIt != memAllocList.end() && nextIt->pid == 0) {
					freeLoc = std::distance(memAllocList.begin(), it);

					it->blockNum += nextIt->blockNum;
					it = memAllocList.erase(nextIt);
					--it;
				}
			}
			break;
		}
	}
	//DisplayMemUsage(freeLoc, false);
}

void Memory::AssignMem(int offset, char* blockData) {
	std::lock_guard<std::mutex> lock(memMutex);
	std::memcpy(mem, blockData, BLOCK_SIZE);
}

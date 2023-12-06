
#include "memory.h"

void Memory::DisplayMemUsage(int loc, bool type) {  // true-alloc false-free
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "====" << ++memScheTime << "====" << std::endl;

	std::cout << "  高地址" << std::endl;

	for (int ii = memVector.size() - 1; ii >= 0; --ii) {
		if (ii == loc) {
			if (type) SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			else SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << "|       |" << std::endl;
		if (memVector[ii].memSize < 100)
			std::cout << "|" << memVector[ii].pid << ":" << memVector[ii].memSize << "kb |" << std::endl;
		else std::cout << "|" << memVector[ii].pid << ":" << memVector[ii].memSize << "kb|" << std::endl;
		std::cout << "|_______|" << std::endl;
	}
}

void Memory::PrintMemUsage() {
	for (auto it = memVector.begin(); it != memVector.end(); ++it) {
		std::cout << it->pid;
		if (it != std::prev(memVector.end()))
			std::cout << "→";
	}
	std::cout << std::endl;
}

int Memory::FirstFit(int allocSize) {  // 首次适应
	for (int ii = 0; ii < memVector.size(); ii++) {
		if (memVector[ii].pid == 0 && memVector[ii].memSize >= allocSize) {
			return ii;
		}
	}
	return INFINITY;
}

int Memory::BestFit(int allocSize) {  // 最佳适应
	int bestLoc = -1;
	int bestSize = 1000000000;
	for (int ii = 0; ii < memVector.size(); ii++) {
		if (memVector[ii].pid == 0 && memVector[ii].memSize >= allocSize)
			if (memVector[ii].memSize <= bestSize) {
				bestLoc = ii;
				bestSize = memVector[ii].memSize;
			}
	}
	return bestLoc;
}

int Memory::WorstFit(int allocSize) {  // 最坏适应
	int worstLoc = -1;
	int worstSize = 0;
	for (int ii = 0; ii < memVector.size(); ii++) {
		if (memVector[ii].pid == 0 && memVector[ii].memSize >= allocSize)
			if (memVector[ii].memSize > worstSize) {
				worstLoc = ii;
				worstSize = memVector[ii].memSize;
			}
	}
	return worstLoc;
}

Memory::Memory() {
	memVector.push_back(MCB(0, 640));
	memScheTime = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Memory::Alloc(int pid, int allocSize) {
	// firstFit  bestFit  worstFit
	int loc = FirstFit(allocSize);
	if (loc != INFINITY) {
		memVector.insert(memVector.begin() + loc, MCB(pid, allocSize));
		memVector[loc + 1].memSize -= allocSize;
	}
	else std::cout << "mem is not available!" << std::endl;
	DisplayMemUsage(loc, true);
}

void Memory::Free(int pid, int freeSize) {
	int freeLoc = -1;

	std::vector<MCB> tempVector;
	for (size_t ii = 0; ii < memVector.size(); ii++) {
		if (memVector[ii].pid == pid) {
			memVector[ii].pid = 0;
			freeLoc = ii;
			for (auto it = memVector.begin(); it != memVector.end(); ++it) {
				if (it->pid != 0)
					continue;
				auto nextIt = std::next(it);
				if (nextIt != memVector.end() && nextIt->pid == 0) {
					freeLoc = std::distance(memVector.begin(), it);

					it->memSize += nextIt->memSize;
					it = memVector.erase(nextIt);
					--it;
				}
			}
			break;
		}
	}
	DisplayMemUsage(freeLoc, false);
}
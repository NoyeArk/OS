#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  ���̹���
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <bitset>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>
#include <sstream>

#define BLOCK_NUM      1024  // 1024�飺0~1023
#define FILE_BLOCK_NUM 900   // 900�飺 0~899
#define SWAP_BLOCK_NUM 124   // 124�飺 900~1023
#define BLOCK_SIZE     320   // ���С��40B

class Disk
{
private:
	std::fstream rwCursor;
	std::bitset<FILE_BLOCK_NUM> fileBitMap;
	std::bitset<SWAP_BLOCK_NUM> swapBitMap;
	std::mutex fileBitMapMutex;

private:
	std::vector<int> GetFreeBlocks(int applyBlockNum);
	// �õ��������ж�Ӧ���������ڿ��
	std::vector<int> GetDataBlocksId(const std::vector<int>& idxBlocksNum);
	void UpdateIdxBlock();
	char* ReadSingleBlockFromDisk(const int& addr);
	void WriteSingleBlockToDisk(const int& addr, const std::string& dataToWrite);

public:
	Disk();
	~Disk();

	std::vector<int> QueryFreeFileBlock();
	void SetBitMap(int blockId, bool data);
	std::vector<int> AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum);
	std::vector<int> AllocFileBlock();
	void FreeDisk(const std::vector<int>& toFreeIdxBlocksId);
	void CreateDisk();
	std::vector<char*> Read(const std::vector<int>& idxBlocksNum, const int& blockNum);
	void Write(const std::vector<int>& idxBlocksNum, const std::string& dataToWrite);
};

#endif

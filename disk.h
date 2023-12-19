#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  磁盘管理
 * 
 * 关于索引块：
 *	 1. 文件区一共有900块，用2^10次方可以表示，故用10位
 *   2. 以字节为单位，用2个字节来表示一个索引
 *   3. 一个索引块40B，故可以存放20个索引
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

#define BLOCK_NUM      1024  // 1024块：0~1023
#define FILE_BLOCK_NUM 900   // 900块： 0~899
#define SWAP_BLOCK_NUM 124   // 124块： 900~1023
#define BLOCK_SIZE     40    // 块大小：40B
#define IDX_SIZE       2     // 索引块中索引大小：2B

#define Debug 1

class Disk
{
private:
	std::fstream rwCursor;
	std::bitset<FILE_BLOCK_NUM> fileBitMap;
	std::bitset<SWAP_BLOCK_NUM> swapBitMap;
	std::mutex fileBitMapMutex;

private:
	std::vector<int> GetFreeBlocks(int applyBlockNum);
	// 得到索引块中对应的数据所在块号
	std::vector<int> GetDataBlocksId(const std::vector<int>& idxBlocksNum);
	char* ReadSingleBlockFromDisk(const int& addr);
	void WriteSingleBlockToDisk(const int& effectiveAddr, const int& writeLen, const char* dataToWrite);

	// Debug
	void DebugCout(std::string info);

public:
	Disk();
	~Disk();

	void WriteSwap(const std::vector<char*>& toWriteBlocksData);
	void ReadSwap();

	std::vector<int> QueryFreeFileBlock();
	void SetBitMap(int blockId, bool data);
	std::vector<int> AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum);
	std::vector<int> AllocFileBlock();
	void FreeDisk(const std::vector<int>& toFreeIdxBlocksId);
	void CreateDisk();
	std::vector<char*> ReadFile(const std::vector<int>& idxBlocksNum, const int& blockNum);
	void WriteFile(const std::vector<int>& idxBlocksNum, const std::string& dataToWrite);
};

#endif

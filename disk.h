#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  ���̹���
 * 
 * ���������飺
 *	 1. �ļ���һ����900�飬��2^10�η����Ա�ʾ������10λ
 *   2. ���ֽ�Ϊ��λ����2���ֽ�����ʾһ������
 *   3. һ��������40B���ʿ��Դ��20������
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
#define BLOCK_SIZE     40    // ���С��40B
#define IDX_SIZE       2     // ��������������С��2B

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
	// �õ��������ж�Ӧ���������ڿ��
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

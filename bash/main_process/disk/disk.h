#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  ���̹���
 * 
 * ���������飺
 *	 1. �ļ���һ����900�飬��2^10�η����Ա�ʾ������10λ
 *   2. ���ֽ�Ϊ��λ����2���ֽ�����ʾһ��������short����
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

#include <cstring>
#include <fcntl.h>
#include <cerrno>

#include "../thread/my_thread.h"

#define BLOCK_NUM         1024 // 1024�飺0~1023
#define FILE_BLOCK_NUM    900  // 900�飺 0~899
#define SWAP_BLOCK_NUM    124  // 124�飺 900~1023
#define BLOCK_SIZE        40   // ���С��40B
#define IDX_SIZE          2    // ��������������С��2B
#define IDX_BLOCK_IDX_NUM 20   // �������д������������20

#define Debug 0


class Disk
{
private:
	std::fstream rwCursor;
	std::bitset<FILE_BLOCK_NUM> fileBitMap;
	std::bitset<SWAP_BLOCK_NUM> swapBitMap;
	std::mutex fileBitMapMutex;

private:
	std::vector<short> GetFreeBlocks(int applyBlockNum);
	// �õ��������ж�Ӧ���������ڿ��
	std::vector<short> GetDataBlocksId(const std::vector<short>& idxBlocksNum);
	void UpdateIdxBlock(int idxBlockId, int offset, std::vector<short> dataBlocksId);
	
	char* ReadSingleBlockFromDisk(const int& blockIdToRead);
	void WriteSingleBlockToDisk(const int& effectiveAddr, const int& writeLen, const char* dataToWrite);

	void FlushBuffer();
	
	// Debug
	void DebugCout(std::string info);

public:	
	Disk();
	~Disk();

	void WriteSwap(const std::vector<char*>& toWriteBlocksData);
	std::vector<char*> ReadSwap(const int& blockNum);

	std::vector<short> QueryFreeFileBlock();
	void SetBitMap(int blockId, bool data);
	std::vector<short> AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum);
	std::vector<short> AllocFileBlock();
	void FreeDisk(const std::vector<short>& toFreeIdxBlocksId);
	void CreateDisk();
	std::vector<char*> ReadFile(const std::vector<short>& idxBlocksNum, const int& blockNum);
	void WriteFile(const std::vector<short>& idxBlocksNum, const std::string& dataToWrite);
};

#endif

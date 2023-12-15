#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  磁盘管理
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

#define BLOCK_NUM      1024  // 1024块：0~1023
#define AVAILABLE_NUM  900   // 900块： 0~899
#define BLOCK_SIZE     320   // 块大小：40B

class Disk
{
private:
	std::fstream rwCursor;
	std::bitset<BLOCK_NUM> bitmap;

private:
	// 位示图相关操作
	void GetFreeLoc();
	void Alloc();

public:
	Disk();
	~Disk();

	char* Read(const int& addr, const int& blockNum);
	void Write(const int& addr, const std::string& dataToWrite);
	void CreateDisk();
};

#endif

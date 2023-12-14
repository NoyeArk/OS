#ifndef DISK_H
#define DISK_H

/*****************************************************************//**
 * \file   disk.h
 * \brief  ���̹���
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
//#include <afx.h>

#define BLOCK_NUM      1024  // 1024�飺0~1023
#define AVAILABLE_NUM  900   // 900�飺 0~899
#define BLOCK_SIZE     320   // ���С��40B

class Disk
{
private:
	std::bitset<BLOCK_NUM> bitmap;

private:
	// λʾͼ��ز���
	void GetFreeLoc();
	void Alloc();

public:
	Disk();

	void Read();
	void Write();
	void Test();
};

#endif

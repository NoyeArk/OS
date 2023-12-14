#ifndef KERNEL_H
#define KERNEL_H

/*********************************************************************
 * \file   kernel.h
 * \brief  模拟系统调用实现
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <iostream>
#include <random>

#include "directory.h"
#include "my_memory.h"
#include "my_thread.h"
#include "disk.h"

enum COMMAND
{
	// 文件系统相关指令
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// 终端相关指令
	CMD_CLS, CMD_HELP, CMD_EXIT
};

class Kernel
{
private:
	Directory directory;  // 文件管理系统
	Memory memorysystem;  // 内存管理系统
	Disk disk;            // 磁盘管理系统

private:
	PCB* Fork();

public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

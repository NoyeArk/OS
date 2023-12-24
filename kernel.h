#ifndef KERNEL_H
#define KERNEL_H

/*********************************************************************
 * \file   kernel.h
 * \brief  系统内核实现
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <cstring>
#include <iostream>
#include <string>
#include <random>

#include "directory.h"
#include "disk.h"
#include "my_memory.h"
#include "my_thread.h"

#define InitProcessBlockNum 8
#define BLOCK_SIZE     40   // 块大小：40B

enum COMMAND
{
	// 文件系统相关指令
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// 终端相关指令
	CMD_CLS, CMD_HELP, CMD_EXIT, CMD_NULL
};

class Kernel
{
private:
	Disk disk;            // 磁盘管理系统
	Memory memorysystem;  // 内存管理系统
	Directory directory;  // 文件管理系统
	std::vector<FCB*> sysOpenFiles;  // 系统打开文件表
	ProcessCommucation Message;

private:
	std::unique_ptr<PCB> Fork();                    // 创建进程
	bool Exit(const std::unique_ptr<PCB>& process);	// 释放进程
	bool Open(const std::string& fileName);         // 打开文件
	bool Read(const std::string& fileName);         // 读出文件
	bool Write(const std::string& fileName);        // 写入文件
	bool Close(const std::string& fileName);        // 关闭文件
	bool Delete(const std::string& fileName);       // 删除文件
	bool Create(const std::string& fileName);       // 打开文件

	std::vector<int> PageReplaceInterrupt(int pid, int pageNumToReplace);
	
public:
	Kernel();

	std::string getCurPath();
	bool SysCall(COMMAND command, const std::string eax);
};

#endif
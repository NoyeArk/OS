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

#include "../directory/directory.h"
#include "../disk/disk.h"
#include "../memory/my_memory.h"
#include "../thread/my_thread.h"

#define InitProcessBlockNum 8
#define BLOCK_SIZE     40   // 块大小：40B

enum Command
{
	null,
	// 编译程序
	CODE_RUN,
	// 文件系统相关指令
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,  FILE_MOVE,
	// 终端相关指令
	CMD_CLS, CMD_HELP, CMD_EXIT, CMD_NULL,

	// 进程通信指令类型
	//nan, Read, WriteFileBlock, WriteIdxBlock
};


class Kernel
{
private:
	PCB* curProcess;      // 当前获得CPU的进程

	Disk disk;            // 磁盘管理系统
	Memory memorysystem;  // 内存管理系统
	Directory directory;  // 文件管理系统
	std::vector<FCB*> sysOpenFiles;  // 系统打开文件表
	std::vector<std::shared_ptr<PCB>> readyQueue;  // 进程就绪队列

private:
	std::unique_ptr<PCB> Fork();                    // 创建进程
	bool Exit(const std::unique_ptr<PCB>& pcb);	// 释放进程
	bool SysOpenFile(const std::string& fileName);         // 打开文件
	bool SysReadFile(const std::string& fileName);         // 读出文件
	bool SysWriteFile(const std::string& fileName);        // 写入文件
	bool SysCloseFile(const std::string& fileName);        // 关闭文件
	bool SysDeleteFile(const std::string& fileName);       // 删除文件
	bool SysCreateFile(const std::string& fileName);       // 打开文件
	bool SysMoveFile(const std::string& targetMoveFile, const std::string& targetFolder);

	std::vector<int> PageReplaceInterrupt(int pageNumToReplace);
	
public:
	Kernel();

	std::string getCurPath();
	bool SysCall(Command command, const std::string eax);
};

#endif
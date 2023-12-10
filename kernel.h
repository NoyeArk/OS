#ifndef KERNEL_H
#define KERNEL_H

#include <iostream>
#include "file_manager.h"
#include "my_memory.h"

enum COMMAND
{
	// 文件系统相关指令
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,   FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// 终端相关指令
	CMD_CLS, CMD_HELP, CMD_EXIT
};

class Kernel
{
private:
	FileSystem filesystem;  // 文件管理系统
	Memory memorysystem;    // 内存管理系统
public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

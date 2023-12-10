#ifndef KERNEL_H
#define KERNEL_H

#include <iostream>
#include "file_manager.h"
#include "my_memory.h"

enum COMMAND
{
	// �ļ�ϵͳ���ָ��
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,   FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// �ն����ָ��
	CMD_CLS, CMD_HELP, CMD_EXIT
};

class Kernel
{
private:
	FileSystem filesystem;  // �ļ�����ϵͳ
	Memory memorysystem;    // �ڴ����ϵͳ
public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

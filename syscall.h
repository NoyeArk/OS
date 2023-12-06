#ifndef SYSCALL_H
#define SYSCALL_H

#include <iostream>
#include "file_manager.h"
#include "memory.h"

enum COMMAND
{
	// �ļ�ϵͳ���ָ��
	FILE_FORMAT, FILE_MAKDIR, FILE_RMDIR, FILE_LS, FILE_CD, FILE_CREATE, FILE_OPEN, FILE_CLOSE, FILE_WRITE, FILE_READ, FILE_RM,
	// �ն����ָ��
	CMD_CLS, CMD_HELP, CMD_EXIT
};

class Syscall
{
private:
	FileSystem filesystem;  // �ļ�����ϵͳ
	Memory memorysystem;    // �ڴ����ϵͳ
public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

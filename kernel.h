#ifndef KERNEL_H
#define KERNEL_H

/*********************************************************************
 * \file   kernel.h
 * \brief  ģ��ϵͳ����ʵ��
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
	// �ļ�ϵͳ���ָ��
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// �ն����ָ��
	CMD_CLS, CMD_HELP, CMD_EXIT
};

class Kernel
{
private:
	Directory directory;  // �ļ�����ϵͳ
	Memory memorysystem;  // �ڴ����ϵͳ
	Disk disk;            // ���̹���ϵͳ

private:
	PCB* Fork();

public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

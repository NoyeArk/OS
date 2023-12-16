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
#include <cstring>
#include <random>

#include "directory.h"
#include "disk.h"
#include "my_memory.h"
#include "my_thread.h"

#define InitProcessBlockNum 8

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
	Disk disk;            // ���̹���ϵͳ
	Memory memorysystem;  // �ڴ����ϵͳ
	Directory directory;  // �ļ�����ϵͳ

private:
	std::unique_ptr<PCB> Fork();   // ��������
	void Exit(const std::unique_ptr<PCB>& process);				   // �ͷŽ���
	void Open(const std::string& fileName);   // ���ļ�
	void Close();  // �ر��ļ�

public:
	std::string getCurPath();
	void SysCall(COMMAND command, const std::string eax);
};

#endif

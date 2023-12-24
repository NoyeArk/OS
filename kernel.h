#ifndef KERNEL_H
#define KERNEL_H

/*********************************************************************
 * \file   kernel.h
 * \brief  ϵͳ�ں�ʵ��
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
#define BLOCK_SIZE     40   // ���С��40B

enum COMMAND
{
	// �ļ�ϵͳ���ָ��
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,
	// �ն����ָ��
	CMD_CLS, CMD_HELP, CMD_EXIT, CMD_NULL
};

class Kernel
{
private:
	Disk disk;            // ���̹���ϵͳ
	Memory memorysystem;  // �ڴ����ϵͳ
	Directory directory;  // �ļ�����ϵͳ
	std::vector<FCB*> sysOpenFiles;  // ϵͳ���ļ���
	ProcessCommucation Message;

private:
	std::unique_ptr<PCB> Fork();                    // ��������
	bool Exit(const std::unique_ptr<PCB>& process);	// �ͷŽ���
	bool Open(const std::string& fileName);         // ���ļ�
	bool Read(const std::string& fileName);         // �����ļ�
	bool Write(const std::string& fileName);        // д���ļ�
	bool Close(const std::string& fileName);        // �ر��ļ�
	bool Delete(const std::string& fileName);       // ɾ���ļ�
	bool Create(const std::string& fileName);       // ���ļ�

	std::vector<int> PageReplaceInterrupt(int pid, int pageNumToReplace);
	
public:
	Kernel();

	std::string getCurPath();
	bool SysCall(COMMAND command, const std::string eax);
};

#endif
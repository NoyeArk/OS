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

#include "../directory/directory.h"
#include "../disk/disk.h"
#include "../memory/my_memory.h"
#include "../thread/my_thread.h"

#define InitProcessBlockNum 8
#define BLOCK_SIZE     40   // ���С��40B

enum Command
{
	null,
	// �������
	CODE_RUN,
	// �ļ�ϵͳ���ָ��
	FILE_FORMAT, FILE_MKDIR, FILE_RMDIR, FILE_LS,    FILE_CD, 
	FILE_CREATE, FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ, 
	FILE_RM,     FILE_BACK,  FILE_MOVE,
	// �ն����ָ��
	CMD_CLS, CMD_HELP, CMD_EXIT, CMD_NULL,

	// ����ͨ��ָ������
	//nan, Read, WriteFileBlock, WriteIdxBlock
};


class Kernel
{
private:
	PCB* curProcess;      // ��ǰ���CPU�Ľ���

	Disk disk;            // ���̹���ϵͳ
	Memory memorysystem;  // �ڴ����ϵͳ
	Directory directory;  // �ļ�����ϵͳ
	std::vector<FCB*> sysOpenFiles;  // ϵͳ���ļ���
	std::vector<std::shared_ptr<PCB>> readyQueue;  // ���̾�������

private:
	std::unique_ptr<PCB> Fork();                    // ��������
	bool Exit(const std::unique_ptr<PCB>& pcb);	// �ͷŽ���
	bool SysOpenFile(const std::string& fileName);         // ���ļ�
	bool SysReadFile(const std::string& fileName);         // �����ļ�
	bool SysWriteFile(const std::string& fileName);        // д���ļ�
	bool SysCloseFile(const std::string& fileName);        // �ر��ļ�
	bool SysDeleteFile(const std::string& fileName);       // ɾ���ļ�
	bool SysCreateFile(const std::string& fileName);       // ���ļ�
	bool SysMoveFile(const std::string& targetMoveFile, const std::string& targetFolder);

	std::vector<int> PageReplaceInterrupt(int pageNumToReplace);
	
public:
	Kernel();

	std::string getCurPath();
	bool SysCall(Command command, const std::string eax);
};

#endif
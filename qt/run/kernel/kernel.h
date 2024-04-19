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
#include <memory>

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
    _FILE_CREATE, _FILE_OPEN,  FILE_CLOSE, FILE_WRITE, FILE_READ,
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
	Memory memorysystem;  // �ڴ����ϵͳ

	std::vector<FCB*> sysOpenFiles;  // ϵͳ���ļ���
	std::vector<std::shared_ptr<PCB>> readyQueue;  // ���̾�������

public:
    Disk disk;            // ���̹���ϵͳ
    Directory directory;          // �ļ�����ϵͳ
    std::unique_ptr<PCB> Fork();  // ��������
	
public:
	Kernel();

    bool Exit(const std::unique_ptr<PCB>& pcb);	           // �ͷŽ���
    FCB* SysOpenFile(FCB* &parent_fcb, const std::string& fileName);         // ���ļ�
    bool SysReadFile(const std::string& fileName, std::vector<std::string>& fileData);         // �����ļ�
    bool SysWriteFile(FCB* &fcb, const std::string& fileName, const std::string& dataToWrite);        // д���ļ�
    bool SysCloseFile(const std::string& fileName);        // �ر��ļ�
    bool SysDeleteFile(FCB* &fcb, const std::string& fileName);       // ɾ���ļ�
    bool SysCreateFile(FCB* &fcb, const std::string& fileName);       // ���ļ�
    bool SysMoveFile(const std::string& targetMoveFile, const std::string& targetFolder);

    std::vector<int> PageReplaceInterrupt(int pageNumToReplace);

	std::string getCurPath();
	bool SysCall(Command command, const std::string eax);
};

static Kernel kernel;

#endif

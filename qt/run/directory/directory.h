#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <stack>
#include <string>

/*****************************************************************//**
 * \file   directory.h
 * \brief  Ŀ¼����
 *	 format�����ļ��洢�����и�ʽ�����������ļ�ϵͳ�Խṹ��������̿ռ���в��֣��������ϴ�����Ŀ¼�Լ����ڹ����ļ��洢�ռ�ȵ����ݽṹ��
 *	 mkdir�� ���ڴ�����Ŀ¼��
 *	 rmdir�� ����ɾ��Ŀ¼��
 *	 ls��    ������ʾĿ¼��
 *	 cd��    ���ڸ��ĵ�ǰĿ¼��
 *	 create�����ڴ����ļ���
 *	 open��  ���ڴ��ļ���
 *	 close�� ���ڹر��ļ���
 *	 write�� ����д�ļ���
 *	 read��  ���ڶ��ļ�
 *	 rm��    ����ɾ���ļ���
 *	 back:   ������һ��Ŀ¼
 * \author horiki
 * \date   December 2023
 *********************************************************************/

enum Permiss {
	emWriteOk,
	emReadOk,
	emDelDenied,
	emNone
};

enum FILE_TYPE {
	DATA, DIR, NONE
};

struct FCB {

	int len;				           // �ļ�����-�ļ���ռ����
	int owner;                         // �ļ�������
	FILE_TYPE type;				       // �ļ�����
	bool isModified;				   // �Ƿ��޸�
	std::string name;		           // �ļ���
	std::string path;			       // �ļ�·��
	int authorization;				   // 1:��д  0:�ɶ�
	std::vector<FCB*> childFiles;      // �������ļ�
	std::vector<short> idxBlocksId;	   // �������
	std::chrono::system_clock::time_point createTime;    // ����ʱ��
	std::chrono::system_clock::time_point lastEditTime;  // ����޸�ʱ��

        bool isInMem;

    std::string parentPath;

	FCB();
	FCB(const std::string name, const std::string parentPath, FILE_TYPE type);
	void ExpandFileLen(std::vector<short> newIdxBlocksId, int newApplyBlockNum);
	void AddIdxBlockId(const short& idxBlockId);
    void UpdateFilePath();
};


class Directory {
private:
	FCB rootFile;
	FCB* curFile;
	std::stack<FCB*> opFilePath;

private:
	// �޸�����˽�б���
	void UpdateCurFilePoint(const int& idx);

	// ��������
	void Error();  // ��ʵ��

	// ���ߺ���
	inline std::string GetFileName();
	inline void OutMsg(const std::string msg);
	int QueryDirectory(const std::string queryFile);

	// ������
	void TestRead();

public:
	Directory();
	~Directory();

	void move(const std::string& targetMoveFile, const std::string& targetFolder);
	void format();
    void mkdir(FCB* &fcb, const std::string& fileName);
	void rmdir();
	void Ls();
	void cd(std::string fileName);
    void Create(FCB* &fcb, const std::string& fileName, std::vector<short> idxBlocksId);
	// ����Ŀ���ļ���FCB
	FCB* OpenFile(const std::string& fileName);
	void CloseFile(const std::string& fileName);
	FCB* WriteFile(const std::string& fileName);
	void ReadFile();
    std::vector<short> Rm(FCB* &fcb, const std::string& toRemoveFile);
	void Back();

    // �ļ�������
    bool Rename(FCB* &fcb, std::string srcFile, std::string targetFile);

	std::string getCurPath();
    FCB* GetRootFcb();
};

#endif

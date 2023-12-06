#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

/*****************************************************************//**
 * \file   file_manager.h
 * \brief
 *	 format�����ļ��洢�����и�ʽ�����������ļ�ϵͳ�Խṹ��������̿ռ���в��֣��������ϴ�����Ŀ¼�Լ����ڹ����ļ��洢�ռ�ȵ����ݽṹ��
 *	 mkdir�����ڴ�����Ŀ¼��
 *	 rmdir������ɾ��Ŀ¼��
 *	 ls��������ʾĿ¼��
 *	 cd�����ڸ��ĵ�ǰĿ¼��
 *	 create�����ڴ����ļ���
 *	 open�����ڴ��ļ���
 *	 close�����ڹر��ļ���
 *	 write������д�ļ���
 *	 read�����ڶ��ļ�
 *	 rm������ɾ���ļ���
 * \author horiki
 * \date   December 2023
 *********************************************************************/

/**

*/

enum Permiss {
	emWriteOk,
	emReadOk,
	emNone
};

struct FCB {

	int len;				      // �ļ�����
	int addr;					  // �����ַ
	int flag;					  // 1:��д  0:�ɶ�  
	std::string name;		      // �ļ���
	std::string path;			  // �ļ�·��
	std::vector<FCB> childFiles;  // �������ļ�
	std::chrono::system_clock::time_point createTime;       // ����ʱ��
	std::chrono::system_clock::time_point lastEditTime;     // ����޸�ʱ��

	FCB() {
		this->len = 0;
		this->addr = 0;
		this->flag = 0;
		this->name = "";
		this->path = "";
		this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;
	}

	FCB(const std::string name, const std::string parentPath) : 
		name(name) {
		this->len = 0;
		this->addr = 0;
		this->flag = 0;
		//this->name = name;
		this->path = parentPath + "/" + this->name;
		this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;
	}

};

struct DIRTECOTY {
	int idx;  // ����
	std::string name;  // �ļ���

	DIRTECOTY(const int idx, const std::string name) : idx(idx), name(name)
	{}
};


class FileSystem {
private:
	FCB rootFile;
	std::string curPath;
	std::vector<DIRTECOTY> directory;
	std::vector<FCB>* curDirFiles;

	// �޸�����˽�б���
	void UpdateCurFilePoint(const int& idx);

	// ��������
	void Error();  // ��ʵ��

	// ���ߺ���
	int QueryDirectory(const std::string queryFile);
	inline std::string GetFileName();
	inline void OutMsg(const std::string msg);

public:
	void format();
	void mkdir();
	void rmdir();
	void ls();
	void cd(std::string fileName);
	void create();
	void open();
	void close();
	void write();
	void read();
	void rm();

	FileSystem();

	std::string getCurPath();

};

#endif

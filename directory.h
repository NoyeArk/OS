#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <stack>

/*****************************************************************//**
 * \file   directory.h
 * \brief  Ŀ¼����
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
 *	 back: ������һ��Ŀ¼
 * \author horiki
 * \date   December 2023
 *********************************************************************/

enum Permiss {
	emWriteOk,
	emReadOk,
	emNone
};

enum FILE_TYPE {
	DATA, DIR, NONE
};

struct FCB {

	int len;				           // �ļ�����-�ļ���ռ����
	int addr;					       // �������
	FILE_TYPE type;				       // �ļ�����
	bool isModified;				   // �Ƿ��޸�
	std::string name;		           // �ļ���
	std::string path;			       // �ļ�·��
	int authorization;				   // 1:��д  0:�ɶ�
	std::vector<FCB> childFiles;       // �������ļ�
	std::chrono::system_clock::time_point createTime;    // ����ʱ��
	std::chrono::system_clock::time_point lastEditTime;  // ����޸�ʱ��

	FCB() {
		this->len = 0;
		this->addr = 0;
		this->type = NONE;
		this->name = "";
		this->path = "";
		this->authorization = 0;
		//this->directory = std::vector<DIRTECOTY>();
		//this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;
	}

	FCB(const std::string name, const std::string parentPath, FILE_TYPE type) :
		name(name), type(type) {
		this->len = 0;
		this->addr = 0;
		//this->type = NONE;
		//this->name = name;
		this->authorization = 0;
		//this->directory = std::vector<DIRTECOTY>();
		this->path = parentPath + "/" + this->name;
		this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;

		if (type == DATA) this->name += ".data";
	}
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
	int QueryDirectory(const std::string queryFile);
	inline std::string GetFileName();
	inline void OutMsg(const std::string msg);

	// ������
	void TestRead();

public:
	Directory();
	~Directory();

	void format();
	void mkdir();
	void rmdir();
	void ls();
	void cd(std::string fileName);
	void create();
	// ����Ŀ���ļ���FCB
	FCB* open(const std::string& fileName);
	void close();
	void write();
	void read();
	void rm();
	void back();

	std::string getCurPath();

};

#endif

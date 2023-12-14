#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <stack>

/*****************************************************************//**
 * \file   directory.h
 * \brief  目录管理
 *	 format：对文件存储器进行格式化，即按照文件系统对结构对虚拟磁盘空间进行布局，并在其上创建根目录以及用于管理文件存储空间等的数据结构。
 *	 mkdir：用于创建子目录；
 *	 rmdir：用于删除目录；
 *	 ls：用于显示目录；
 *	 cd：用于更改当前目录；
 *	 create：用于创建文件；
 *	 open：用于打开文件；
 *	 close：用于关闭文件；
 *	 write：用于写文件；
 *	 read：用于读文件
 *	 rm：用于删除文件。
 *	 back: 返回上一级目录
 * \author horiki
 * \date   December 2023
 *********************************************************************/

enum Permiss {
	emWriteOk,
	emReadOk,
	emNone
};

enum FILE_TYPE {
	TXT, DIR, NONE
};

struct FCB {

	int len;				           // 文件长度
	int loc;					       // 索引块号
	int flag;					       // 1:可写  0:可读
	FILE_TYPE type;				       // 文件类型
	std::string name;		           // 文件名
	std::string path;			       // 文件路径
	std::vector<FCB> childFiles;       // 链接子文件
	std::chrono::system_clock::time_point createTime;       // 创建时间
	std::chrono::system_clock::time_point lastEditTime;     // 最后修改时间

	FCB() {
		this->len = 0;
		this->loc = 0;
		this->flag = 0;
		this->type = NONE;
		this->name = "";
		this->path = "";
		//this->directory = std::vector<DIRTECOTY>();
		//this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;
	}

	FCB(const std::string name, const std::string parentPath, FILE_TYPE type) :
		name(name), type(type) {
		this->len = 0;
		this->loc = 0;
		this->flag = 0;
		//this->type = NONE;
		//this->name = name;
		//this->directory = std::vector<DIRTECOTY>();
		this->path = parentPath + "/" + this->name;
		this->createTime = std::chrono::system_clock::now();
		this->lastEditTime = this->createTime;

		if (type == TXT) this->name += ".txt";
	}

};


class Directory {
private:
	FCB rootFile;
	FCB* curFile;
	std::stack<FCB*> opFilePath;

private:
	// 修改类中私有变量
	void UpdateCurFilePoint(const int& idx);

	// 错误处理函数
	void Error();  // 待实现

	// 工具函数
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
	void back();

	Directory();

	std::string getCurPath();

};

#endif

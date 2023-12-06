#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

/*****************************************************************//**
 * \file   file_manager.h
 * \brief
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

	int len;				      // 文件长度
	int addr;					  // 物理地址
	int flag;					  // 1:可写  0:可读  
	std::string name;		      // 文件名
	std::string path;			  // 文件路径
	std::vector<FCB> childFiles;  // 链接子文件
	std::chrono::system_clock::time_point createTime;       // 创建时间
	std::chrono::system_clock::time_point lastEditTime;     // 最后修改时间

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
	int idx;  // 索引
	std::string name;  // 文件名

	DIRTECOTY(const int idx, const std::string name) : idx(idx), name(name)
	{}
};


class FileSystem {
private:
	FCB rootFile;
	std::string curPath;
	std::vector<DIRTECOTY> directory;
	std::vector<FCB>* curDirFiles;

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

	FileSystem();

	std::string getCurPath();

};

#endif

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
 * \brief  目录管理
 *	 format：对文件存储器进行格式化，即按照文件系统对结构对虚拟磁盘空间进行布局，并在其上创建根目录以及用于管理文件存储空间等的数据结构。
 *	 mkdir： 用于创建子目录；
 *	 rmdir： 用于删除目录；
 *	 ls：    用于显示目录；
 *	 cd：    用于更改当前目录；
 *	 create：用于创建文件；
 *	 open：  用于打开文件；
 *	 close： 用于关闭文件；
 *	 write： 用于写文件；
 *	 read：  用于读文件
 *	 rm：    用于删除文件。
 *	 back:   返回上一级目录
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

	int len;				           // 文件长度-文件所占块数
	int owner;                         // 文件所有者
	FILE_TYPE type;				       // 文件类型
	bool isModified;				   // 是否被修改
	std::string name;		           // 文件名
	std::string path;			       // 文件路径
	int authorization;				   // 1:可写  0:可读
	std::vector<FCB*> childFiles;      // 链接子文件
	std::vector<short> idxBlocksId;	   // 索引块号
	std::chrono::system_clock::time_point createTime;    // 创建时间
	std::chrono::system_clock::time_point lastEditTime;  // 最后修改时间

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
	// 修改类中私有变量
	void UpdateCurFilePoint(const int& idx);

	// 错误处理函数
	void Error();  // 待实现

	// 工具函数
	inline std::string GetFileName();
	inline void OutMsg(const std::string msg);
	int QueryDirectory(const std::string queryFile);

	// 测试用
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
	// 返回目标文件的FCB
	FCB* OpenFile(const std::string& fileName);
	void CloseFile(const std::string& fileName);
	FCB* WriteFile(const std::string& fileName);
	void ReadFile();
    std::vector<short> Rm(FCB* &fcb, const std::string& toRemoveFile);
	void Back();

    // 文件重命名
    bool Rename(FCB* &fcb, std::string srcFile, std::string targetFile);

	std::string getCurPath();
    FCB* GetRootFcb();
};

#endif

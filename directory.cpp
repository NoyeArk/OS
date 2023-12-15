#include "directory.h"

// -----------------------------常量声明-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------构造函数-----------------------------------
Directory::Directory() {
	rootFile.name = "root";
	rootFile.path = "root";
	curFile = &rootFile;

	TestRead();
}
// -----------------------------析构函数-----------------------------------
Directory::~Directory() {
	// 释放指针
	/*if (curFile) {
		delete curFile;
	}*/
}


// -----------------------------公开函数-----------------------------------
void Directory::format() {

}


void Directory::mkdir() {
	OutMsg("请输入要创建的文件夹名：");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, DIR));
}


void Directory::rmdir() {

}


void Directory::ls() {
	if ((*curFile).childFiles.size() == 0) {
		std::cout << "  " << std::endl;
	}

	for (auto file : curFile->childFiles) {
		std::cout << "  " << file.name << std::endl;
	}
}

void Directory::cd(std::string fileName) {
	std::string name = GetFileName();
	int idx = QueryDirectory(name);
	if (idx == NOT_FIND) {
		Error();
		return;
	}
	UpdateCurFilePoint(idx);
}

void Directory::create() {
	OutMsg("请输入要创建的文件：");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, DATA));
}

FCB* Directory::open(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++)
		if (curFile->childFiles[ii].name == fileName) {
			OutMsg("文件" + fileName + "：");
			return &(curFile->childFiles[ii]);
		}
	OutMsg("错误：未找到相关文件");
	Error();
}

void Directory::close() {

}

void Directory::write() {

}

void Directory::read() {

}

void Directory::back() {
	curFile = opFilePath.top();
	opFilePath.pop();
}

void Directory::rm() {
	// 删除文件
	OutMsg("请输入要删除的文件名：");
	std::string fileName = GetFileName();

}
// -----------------------------修改类中私有变量相关函数--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = &(curFile->childFiles[idx]);
}
// -----------------------------错误处理-----------------------------------
void Directory::Error() {

}
// -----------------------------工具函数-----------------------------------
int Directory::QueryDirectory(const std::string queryFile) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) 
		if (curFile->childFiles[ii].name == queryFile)
			return ii;
	return NOT_FIND;
}

inline std::string Directory::GetFileName() {
	std::string fileName;
	std::cin >> fileName;
	return fileName;
}

inline void Directory::OutMsg(const std::string msg) {
	std::cout << msg;
}


void Directory::TestRead() {
	FCB initFile("boot", curFile->path, DATA);
	initFile.len = 1;  // 占用1块
	initFile.addr = 0; // 0块开始
	curFile->childFiles.push_back(initFile);
}


// -----------------------------公开接口-----------------------------------
std::string Directory::getCurPath() {
	return curFile->path;
}

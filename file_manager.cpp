#include "file_manager.h"

// -----------------------------常量声明-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------构造函数-----------------------------------
FileSystem::FileSystem() {
	rootFile.name = "root";
	rootFile.path = "root";
	curPath = rootFile.path;
	curDirFiles = &rootFile.childFiles;
}


// -----------------------------私有函数-----------------------------------
void FileSystem::format() {

}

void FileSystem::mkdir() {
	std::string targetFile = GetFileName();
	std::string createFileName = GetFileName();
}

void FileSystem::rmdir() {

}

void FileSystem::ls() {
	if ((*curDirFiles).size() == 0) {
		std::cout << "  " << std::endl;
	}

	for (auto file : *curDirFiles) {
		std::cout << "  " << file.name << std::endl;
	}
}

void FileSystem::cd(std::string fileName) {
	std::string name = GetFileName();
	int idx = QueryDirectory(name);
	if (idx == NOT_FIND) {
		Error();
		return;
	}
	UpdateCurFilePoint(idx);
}

void FileSystem::create() {
	OutMsg("请输入要创建的文件名：");
	std::string fileName = GetFileName();

	(*curDirFiles).push_back(FCB(fileName, rootFile.path));
	directory.push_back(DIRTECOTY(rootFile.childFiles.size() - 1, fileName));
	
	OutMsg("create successful!!!");
	ls();
	std::system("pause");
	std::system("cls");
}

void FileSystem::open() {

}

void FileSystem::close() {

}

void FileSystem::write() {

}

void FileSystem::read() {

}

void FileSystem::rm() {

}
// -----------------------------修改类中私有变量相关函数--------------------
void FileSystem::UpdateCurFilePoint(const int& idx) {
	curPath = (*curDirFiles)[idx].path;
	curDirFiles = &(*curDirFiles)[idx].childFiles;
}
// -----------------------------错误处理-----------------------------------
void FileSystem::Error() {

}
// -----------------------------工具函数-----------------------------------
int FileSystem::QueryDirectory(const std::string queryFile) {
	for (auto item : directory) 
		if (item.name == queryFile)
			return item.idx;
	return NOT_FIND;
}

inline std::string FileSystem::GetFileName() {
	std::string fileName;
	std::cin >> fileName;
	return fileName;
}

inline void FileSystem::OutMsg(const std::string msg) {
	std::cout << msg;
}


// -----------------------------公开接口-----------------------------------
std::string FileSystem::getCurPath() {
	return curPath;
}

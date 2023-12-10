#include "file_manager.h"

// -----------------------------常量声明-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------构造函数-----------------------------------
FileSystem::FileSystem() {
	rootFile.name = "root";
	rootFile.path = "root";
	//curPath = rootFile.path;
	curFile = &rootFile;
}


// -----------------------------公开函数-----------------------------------
void FileSystem::format() {

}

void FileSystem::mkdir() {
	OutMsg("请输入要创建的文件夹名：");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, DIR));
	curFile->directory.push_back(DIRTECOTY(curFile->childFiles.size() - 1, fileName));
}

void FileSystem::rmdir() {

}

void FileSystem::ls() {
	if ((*curFile).childFiles.size() == 0) {
		std::cout << "  " << std::endl;
	}

	for (auto file : curFile->childFiles) {
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
	OutMsg("请输入要创建的文件：");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, TXT));
	curFile->directory.push_back(DIRTECOTY(curFile->childFiles.size() - 1, fileName));
}

void FileSystem::open() {

}

void FileSystem::close() {

}

void FileSystem::write() {

}

void FileSystem::read() {

}

void FileSystem::back() {
	curFile = opFilePath.top();
	opFilePath.pop();
}

void FileSystem::rm() {
	// 删除文件
	OutMsg("请输入要删除的文件名：");
	std::string fileName = GetFileName();

}
// -----------------------------修改类中私有变量相关函数--------------------
void FileSystem::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = &(curFile->childFiles[idx]);
}
// -----------------------------错误处理-----------------------------------
void FileSystem::Error() {

}
// -----------------------------工具函数-----------------------------------
int FileSystem::QueryDirectory(const std::string queryFile) {
	for (auto item : curFile->directory)
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
	return curFile->path;
}

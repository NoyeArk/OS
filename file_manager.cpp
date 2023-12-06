#include "file_manager.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------���캯��-----------------------------------
FileSystem::FileSystem() {
	rootFile.name = "root";
	rootFile.path = "root";
	curPath = rootFile.path;
	curDirFiles = &rootFile.childFiles;
}


// -----------------------------˽�к���-----------------------------------
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
	OutMsg("������Ҫ�������ļ�����");
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
// -----------------------------�޸�����˽�б�����غ���--------------------
void FileSystem::UpdateCurFilePoint(const int& idx) {
	curPath = (*curDirFiles)[idx].path;
	curDirFiles = &(*curDirFiles)[idx].childFiles;
}
// -----------------------------������-----------------------------------
void FileSystem::Error() {

}
// -----------------------------���ߺ���-----------------------------------
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


// -----------------------------�����ӿ�-----------------------------------
std::string FileSystem::getCurPath() {
	return curPath;
}

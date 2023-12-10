#include "file_manager.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------���캯��-----------------------------------
FileSystem::FileSystem() {
	rootFile.name = "root";
	rootFile.path = "root";
	//curPath = rootFile.path;
	curFile = &rootFile;
}


// -----------------------------��������-----------------------------------
void FileSystem::format() {

}

void FileSystem::mkdir() {
	OutMsg("������Ҫ�������ļ�������");
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
	OutMsg("������Ҫ�������ļ���");
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
	// ɾ���ļ�
	OutMsg("������Ҫɾ�����ļ�����");
	std::string fileName = GetFileName();

}
// -----------------------------�޸�����˽�б�����غ���--------------------
void FileSystem::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = &(curFile->childFiles[idx]);
}
// -----------------------------������-----------------------------------
void FileSystem::Error() {

}
// -----------------------------���ߺ���-----------------------------------
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


// -----------------------------�����ӿ�-----------------------------------
std::string FileSystem::getCurPath() {
	return curFile->path;
}

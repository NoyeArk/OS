#include "directory.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------���캯��-----------------------------------
Directory::Directory() {
	rootFile.name = "root";
	rootFile.path = "root";
	//curPath = rootFile.path;
	curFile = &rootFile;
}


// -----------------------------��������-----------------------------------
void Directory::format() {

}

void Directory::mkdir() {
	OutMsg("������Ҫ�������ļ�������");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, DIR));
	//curFile->directory.push_back(DIRTECOTY(curFile->childFiles.size() - 1, fileName));
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
	OutMsg("������Ҫ�������ļ���");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, TXT));
	//curFile->directory.push_back(DIRTECOTY(curFile->childFiles.size() - 1, fileName));
}

void Directory::open() {

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
	// ɾ���ļ�
	OutMsg("������Ҫɾ�����ļ�����");
	std::string fileName = GetFileName();

}
// -----------------------------�޸�����˽�б�����غ���--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = &(curFile->childFiles[idx]);
}
// -----------------------------������-----------------------------------
void Directory::Error() {

}
// -----------------------------���ߺ���-----------------------------------
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


// -----------------------------�����ӿ�-----------------------------------
std::string Directory::getCurPath() {
	return curFile->path;
}

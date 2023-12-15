#include "directory.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// -----------------------------���캯��-----------------------------------
Directory::Directory() {
	rootFile.name = "root";
	rootFile.path = "root";
	curFile = &rootFile;

	TestRead();
}
// -----------------------------��������-----------------------------------
Directory::~Directory() {
	// �ͷ�ָ��
	/*if (curFile) {
		delete curFile;
	}*/
}


// -----------------------------��������-----------------------------------
void Directory::format() {

}


void Directory::mkdir() {
	OutMsg("������Ҫ�������ļ�������");
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
	OutMsg("������Ҫ�������ļ���");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(FCB(fileName, curFile->path, DATA));
}

FCB* Directory::open(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++)
		if (curFile->childFiles[ii].name == fileName) {
			OutMsg("�ļ�" + fileName + "��");
			return &(curFile->childFiles[ii]);
		}
	OutMsg("����δ�ҵ�����ļ�");
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


void Directory::TestRead() {
	FCB initFile("boot", curFile->path, DATA);
	initFile.len = 1;  // ռ��1��
	initFile.addr = 0; // 0�鿪ʼ
	curFile->childFiles.push_back(initFile);
}


// -----------------------------�����ӿ�-----------------------------------
std::string Directory::getCurPath() {
	return curFile->path;
}

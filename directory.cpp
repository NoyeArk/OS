#include "directory.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// ----------------------�ṹ��FCB����ʵ��--------------------------
FCB::FCB() {
	this->len = 0;
	this->type = NONE;
	this->name = "";
	this->path = "";
	this->idxBlocksId = {};
	this->authorization = emNone;
	this->lastEditTime = this->createTime;
}

FCB::FCB(const std::string name, const std::string parentPath, FILE_TYPE type) :
	name(name), type(type) {
	this->len = 0;
	this->idxBlocksId = {};
	this->authorization = emNone;
	this->path = parentPath + "/" + this->name;
	this->createTime = std::chrono::system_clock::now();
	this->lastEditTime = this->createTime;

	if (type == DATA) this->name += ".data";
}

void FCB::ExpandFileLen(std::vector<int> newIdxBlocksId, int newApplyBlockNum) {
	this->len += newApplyBlockNum;
	this->lastEditTime = std::chrono::system_clock::now();
	this->idxBlocksId.insert(this->idxBlocksId.end(), newIdxBlocksId.begin(), newIdxBlocksId.end());
}

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
	OutMsg("������Ҫ������Ŀ¼����");
	std::string fileName = GetFileName();

	curFile->childFiles.push_back(&FCB(fileName, curFile->path, DIR));
}


void Directory::rmdir() {

}


void Directory::ls() {
	if ((*curFile).childFiles.size() == 0) {
		std::cout << "  " << std::endl;
	}

	for (auto file : curFile->childFiles) {
		std::cout << "  " << file->name << std::endl;
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


void Directory::Create(const std::string& fileName, std::vector<int> idxBlocksId) {
	FCB* fcb = new FCB(fileName, curFile->path, DATA);
	fcb->idxBlocksId = idxBlocksId;
	curFile->childFiles.push_back(fcb);
}


FCB* Directory::OpenFile(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++)
		if (curFile->childFiles[ii]->name == fileName) {
			OutMsg("�ļ�" + fileName + "��");
			curFile->childFiles[ii]->authorization = emDelDenied;
			return curFile->childFiles[ii];
		}
	OutMsg("����δ�ҵ�����ļ�");
	Error();
}


void Directory::CloseFile() {

}


FCB* Directory::WriteFile(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++)
		if (curFile->childFiles[ii]->name == fileName) {
			OutMsg("�ļ�" + fileName + "��");
			return curFile->childFiles[ii];
		}
	OutMsg("����δ�ҵ�����ļ�");
	Error();
}


void Directory::ReadFile() {

}


void Directory::Back() {
	curFile = opFilePath.top();
	opFilePath.pop();
}


/**
 * \name rm  ɾ���ļ�
 * \brief    ɾ���ļ���Ӧ��FCB
 */
std::vector<int> Directory::Rm(const std::string& toRemoveFile) {
	std::vector<int> idxBlocksId = {};

	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		FCB* fcb = curFile->childFiles[ii];
		if (fcb->name != toRemoveFile)
			continue;
		if (fcb->authorization == emDelDenied) {
			OutMsg("��ǰ�ļ����ڴ��У�������ɾ����");
			return std::vector<int>();
		}
		if (fcb->type == DIR) {  // Ŀ¼�ļ�
			// ����������
		}
		else if (fcb->type == DATA) {  // �����ļ�
			// �õ����ļ���Ӧ�����п��
			idxBlocksId = fcb->idxBlocksId;
		}
		curFile->childFiles.erase(curFile->childFiles.begin() + ii);
		delete fcb;
		return idxBlocksId;
	}
	OutMsg("Ҫɾ�����ļ�������!");
	return std::vector<int>();
}


// -----------------------------�޸�����˽�б�����غ���--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = curFile->childFiles[idx];
}


// -----------------------------������-----------------------------------
void Directory::Error() {

}


// -----------------------------���ߺ���-----------------------------------
int Directory::QueryDirectory(const std::string queryFile) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) 
		if (curFile->childFiles[ii]->name == queryFile)
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
	initFile.idxBlocksId.push_back(0); // ������Ϊ0
	curFile->childFiles.push_back(&initFile);
}


// -----------------------------�����ӿ�-----------------------------------
std::string Directory::getCurPath() {
	return curFile->path;
}

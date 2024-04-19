#include "directory.h"

// -----------------------------��������-----------------------------------
const int NOT_FIND = 99999;

// ----------------------�ṹ��FCB����ʵ��--------------------------
FCB::FCB() {
	this->len = 0;
	this->owner = -1;
	this->type = NONE;
	this->name = "";
	this->path = "";
    this->isInMem = false;
	this->idxBlocksId = {};
	this->isModified = false;
	this->authorization = emNone;
    this->createTime = std::chrono::system_clock::now();
	this->lastEditTime = this->createTime;

    this->parentPath = "";
}

FCB::FCB(const std::string name, const std::string parentPath, FILE_TYPE type) :
	name(name), type(type) {
	this->len = 0;
	this->owner = -1;
    this->isInMem = false;
	this->idxBlocksId = {};
	this->isModified = false;
	this->authorization = emNone;
    this->path = parentPath + "\\" + this->name;
	this->createTime = std::chrono::system_clock::now();
	this->lastEditTime = this->createTime;

    this->parentPath = parentPath;

//	if (type == DATA)
//		this->name += ".data";
}

void FCB::ExpandFileLen(std::vector<short> newIdxBlocksId, int newApplyBlockNum) {
	this->len += newApplyBlockNum;
	this->lastEditTime = std::chrono::system_clock::now();
	this->idxBlocksId.insert(this->idxBlocksId.end(), newIdxBlocksId.begin(), newIdxBlocksId.end());
}

void FCB::AddIdxBlockId(const short& idxBlockId) {
    this->idxBlocksId.push_back(idxBlockId);
}

void FCB::UpdateFilePath()
{
    this->path = this->parentPath + "\\" + this->name;
}

// -----------------------------���캯��-----------------------------------
Directory::Directory() {
    rootFile.name = "C:";
    rootFile.path = "C:";
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


void Directory::move(const std::string& targetMoveFile, const std::string& targetFolder) {

}


// -----------------------------��������-----------------------------------
void Directory::format() {

}


void Directory::mkdir(FCB* &fcb, const std::string& fileName) {
    std::cout << "�´������ļ���Ϊ" << fileName << std::endl;
    FCB* new_fcb = new FCB(fileName, fcb->path, DIR);
    fcb->childFiles.push_back(new_fcb);
    std::cout << "child number:" << fcb->childFiles.size();
}
 

void Directory::rmdir() {
    OutMsg("������Ҫɾ����Ŀ¼");
	std::string fileName = GetFileName();
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		if (curFile->childFiles[ii]->name == fileName) {
			curFile->childFiles.erase(curFile->childFiles.begin() + ii);
			return;
		}
	}
    OutMsg("Ҫɾ����Ŀ¼�����ڣ�");
}


void Directory::Ls() {
	if (curFile->childFiles.size() == 0) {
		std::cout << "            0���ļ�" << std::endl;
	}

	for (auto file : curFile->childFiles) {
		std::cout << "  " << file->name << std::endl;
	}
}


void Directory::cd(std::string fileName) {
    int idx = QueryDirectory(fileName);
	if (idx == NOT_FIND) {
		Error();
		return;
	}
	UpdateCurFilePoint(idx);
}


void Directory::Create(FCB* &fcb, const std::string& fileName, std::vector<short> idxBlocksId) {
    FCB* new_fcb = new FCB(fileName, curFile->path, DATA);
    new_fcb->len = 8;  // Ĭ�ϳ�ʼΪ8
    new_fcb->idxBlocksId = idxBlocksId;
    fcb->childFiles.push_back(new_fcb);
}


FCB* Directory::OpenFile(const std::string& fileName) {
    std::cout << "��ǰ�ļ���Ϊ" << curFile->name << std::endl;
    std::cout << "Ҫ�򿪵��ļ���" << fileName << std::endl;
    for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
        if (curFile->childFiles[ii]->name == fileName) {
            curFile->childFiles[ii]->authorization = emDelDenied;
            return curFile->childFiles[ii];
        }
        std::cout << curFile->childFiles[ii]->name << std::endl;
    }

    OutMsg("����δ�ҵ�����ļ�");
	std::cout << std::endl;
	Error();
	return nullptr;
}


void Directory::CloseFile(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		if (curFile->childFiles[ii]->name == fileName) {
			curFile->childFiles[ii]->authorization = emNone;
			break;
		}
	}
}


FCB* Directory::WriteFile(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		if (curFile->childFiles[ii]->name == fileName) {
            OutMsg("�ļ�" + fileName + "��");
			return curFile->childFiles[ii];
		}
	}
    OutMsg("������δ�ҵ������ļ�");
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
std::vector<short> Directory::Rm(FCB* &my_fcb, const std::string& toRemoveFile) {
	std::vector<short> idxBlocksId = {};

    for (size_t ii = 0; ii < my_fcb->childFiles.size(); ii++) {
        FCB* fcb = my_fcb->childFiles[ii];
		if (fcb->name != toRemoveFile)
			continue;
        if (fcb->type == DATA && fcb->authorization == emDelDenied) {
            OutMsg("��ǰ�ļ����ڴ��У�������ɾ����");
			return std::vector<short>();
		}
        if (fcb->type == DIR) {  // Ŀ¼�ļ�
            // ����������
            if (fcb->childFiles.size() != 0) {
                 OutMsg("��ǰ�ļ��ǿգ�������ɾ����");
                 return std::vector<short>();
            }
            else {
                idxBlocksId.push_back(-1);
                return idxBlocksId;
            }
		}
        else if (fcb->type == DATA ) {  // �����ļ�
            // �õ����ļ���Ӧ�����п��
			idxBlocksId = fcb->idxBlocksId;
		}
        my_fcb->childFiles.erase(my_fcb->childFiles.begin() + ii);
		delete fcb;
		return idxBlocksId;
	}
    OutMsg("Ҫɾ�����ļ������ڣ�");
	return std::vector<short>();
}


// -----------------------------�޸�����˽�б������غ���--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = curFile->childFiles[idx];
}


// -----------------------------��������-----------------------------------
void Directory::Error() {

}


// -----------------------------���ߺ���-----------------------------------
int Directory::QueryDirectory(const std::string queryFile) {
	for (int ii = 0; ii < curFile->childFiles.size(); ii++) 
		if (curFile->childFiles[ii]->name == queryFile)
			return ii;
	return NOT_FIND;
}


inline std::string Directory::GetFileName() {
	std::string fileName;
	std::getline(std::cin, fileName);
	return fileName;
}


inline void Directory::OutMsg(const std::string msg) {
	std::cout << msg;
}


void Directory::TestRead() {
	//FCB* fcb = new FCB("boot", curFile->path, DATA);
    //fcb->len = 8;  // ռ��1��


    //fcb->idxBlocksId.push_back(0); // ������Ϊ0
	//curFile->childFiles.push_back(fcb);
}


bool Directory::Rename(FCB* &fcb, std::string srcFile, std::string targetFile)
{
    for (int ii = 0; ii < fcb->childFiles.size(); ii++) {
        if (fcb->childFiles[ii]->name == srcFile) {
            fcb->childFiles[ii]->name = targetFile;

            return true;
        }
        std::cout << fcb->childFiles[ii]->name << std::endl;
    }

    std::cout << "Directory::Rename::û���ҵ������ļ�" << std::endl;
    return false;
}


// -----------------------------�����ӿ�-----------------------------------
std::string Directory::getCurPath() {
    return curFile->path;
}

FCB *Directory::GetRootFcb()
{
    return &rootFile;
}

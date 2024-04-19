#include "directory.h"

// -----------------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½-----------------------------------
const int NOT_FIND = 99999;

// ----------------------ï¿½á¹¹ï¿½ï¿½FCBï¿½ï¿½ï¿½ï¿½Êµï¿½ï¿½--------------------------
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

// -----------------------------ï¿½ï¿½ï¿½ìº¯ï¿½ï¿½-----------------------------------
Directory::Directory() {
    rootFile.name = "C:";
    rootFile.path = "C:";
	curFile = &rootFile;

	TestRead();
}


// -----------------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½-----------------------------------
Directory::~Directory() {
    // ï¿½Í·ï¿½Ö¸ï¿½ï¿½
	/*if (curFile) {
		delete curFile;
	}*/
}


void Directory::move(const std::string& targetMoveFile, const std::string& targetFolder) {

}


// -----------------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½-----------------------------------
void Directory::format() {

}


void Directory::mkdir(FCB* &fcb, const std::string& fileName) {
    std::cout << "ÐÂ´´½¨µÄÎÄ¼þ¼ÐÎª" << fileName << std::endl;
    FCB* new_fcb = new FCB(fileName, fcb->path, DIR);
    fcb->childFiles.push_back(new_fcb);
    std::cout << "child number:" << fcb->childFiles.size();
}
 

void Directory::rmdir() {
    OutMsg("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÒªÉ¾ï¿½ï¿½ï¿½ï¿½Ä¿Â¼");
	std::string fileName = GetFileName();
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		if (curFile->childFiles[ii]->name == fileName) {
			curFile->childFiles.erase(curFile->childFiles.begin() + ii);
			return;
		}
	}
    OutMsg("ÒªÉ¾ï¿½ï¿½ï¿½ï¿½Ä¿Â¼ï¿½ï¿½ï¿½ï¿½ï¿½Ú£ï¿½");
}


void Directory::Ls() {
	if (curFile->childFiles.size() == 0) {
		std::cout << "            0ï¿½ï¿½ï¿½Ä¼ï¿½" << std::endl;
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
    new_fcb->len = 8;  // Ä¬ï¿½Ï³ï¿½Ê¼Îª8
    new_fcb->idxBlocksId = idxBlocksId;
    fcb->childFiles.push_back(new_fcb);
}


FCB* Directory::OpenFile(const std::string& fileName) {
    std::cout << "µ±Ç°ÎÄ¼þ¼ÐÎª" << curFile->name << std::endl;
    std::cout << "Òª´ò¿ªµÄÎÄ¼þÊÇ" << fileName << std::endl;
    for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
        if (curFile->childFiles[ii]->name == fileName) {
            curFile->childFiles[ii]->authorization = emDelDenied;
            return curFile->childFiles[ii];
        }
        std::cout << curFile->childFiles[ii]->name << std::endl;
    }

    OutMsg("´íÎó£ºÎ´ÕÒµ½Ïà¹ØÎÄ¼þ");
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
            OutMsg("ï¿½Ä¼ï¿½" + fileName + "ï¿½ï¿½");
			return curFile->childFiles[ii];
		}
	}
    OutMsg("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î´ï¿½Òµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½");
	Error();
}


void Directory::ReadFile() {

}


void Directory::Back() {
	curFile = opFilePath.top();
	opFilePath.pop();
}


/**
 * \name rm  É¾³ýÎÄ¼þ
 * \brief    É¾³ýÎÄ¼þ¶ÔÓ¦µÄFCB
 */
std::vector<short> Directory::Rm(FCB* &my_fcb, const std::string& toRemoveFile) {
	std::vector<short> idxBlocksId = {};

    for (size_t ii = 0; ii < my_fcb->childFiles.size(); ii++) {
        FCB* fcb = my_fcb->childFiles[ii];
		if (fcb->name != toRemoveFile)
			continue;
        if (fcb->type == DATA && fcb->authorization == emDelDenied) {
            OutMsg("µ±Ç°ÎÄ¼þÔÚÄÚ´æÖÐ£¬²»ÔÊÐíÉ¾³ý£¡");
			return std::vector<short>();
		}
        if (fcb->type == DIR) {  // Ä¿Â¼ÎÄ¼þ
            // ÎÞÆäËû²Ù×÷
            if (fcb->childFiles.size() != 0) {
                 OutMsg("µ±Ç°ÎÄ¼þ·Ç¿Õ£¬²»ÔÊÐíÉ¾³ý£¡");
                 return std::vector<short>();
            }
            else {
                idxBlocksId.push_back(-1);
                return idxBlocksId;
            }
		}
        else if (fcb->type == DATA ) {  // Êý¾ÝÎÄ¼þ
            // µÃµ½¸ÃÎÄ¼þ¶ÔÓ¦´ÅÅÌÖÐ¿éºÅ
			idxBlocksId = fcb->idxBlocksId;
		}
        my_fcb->childFiles.erase(my_fcb->childFiles.begin() + ii);
		delete fcb;
		return idxBlocksId;
	}
    OutMsg("ÒªÉ¾³ýµÄÎÄ¼þ²»´æÔÚ£¡");
	return std::vector<short>();
}


// -----------------------------ï¿½Þ¸ï¿½ï¿½ï¿½ï¿½ï¿½Ë½ï¿½Ð±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Øºï¿½ï¿½ï¿½--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = curFile->childFiles[idx];
}


// -----------------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½-----------------------------------
void Directory::Error() {

}


// -----------------------------ï¿½ï¿½ï¿½ßºï¿½ï¿½ï¿½-----------------------------------
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
    //fcb->len = 8;  // Õ¼ï¿½ï¿½1ï¿½ï¿½


    //fcb->idxBlocksId.push_back(0); // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îª0
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

    std::cout << "Directory::Rename::Ã»ï¿½ï¿½ï¿½Òµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½" << std::endl;
    return false;
}


// -----------------------------ï¿½ï¿½ï¿½ï¿½ï¿½Ó¿ï¿½-----------------------------------
std::string Directory::getCurPath() {
    return curFile->path;
}

FCB *Directory::GetRootFcb()
{
    return &rootFile;
}

#include "directory.h"

// -----------------------------常量声明-----------------------------------
const int NOT_FIND = 99999;

// ----------------------结构体FCB函数实现--------------------------
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

	if (type == DATA) 
		this->name += ".data";
}

void FCB::ExpandFileLen(std::vector<short> newIdxBlocksId, int newApplyBlockNum) {
	this->len += newApplyBlockNum;
	this->lastEditTime = std::chrono::system_clock::now();
	this->idxBlocksId.insert(this->idxBlocksId.end(), newIdxBlocksId.begin(), newIdxBlocksId.end());
}

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
	OutMsg("请输入要创建的目录名：");
	std::string fileName = GetFileName();
	FCB* fcb = new FCB(fileName, curFile->path, DIR);
	curFile->childFiles.push_back(fcb);
}


void Directory::rmdir() {

}


void Directory::Ls() {
	if (curFile->childFiles.size() == 0) {
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


void Directory::Create(const std::string& fileName, std::vector<short> idxBlocksId) {
	FCB* fcb = new FCB(fileName, curFile->path, DATA);
	fcb->idxBlocksId = idxBlocksId;
	curFile->childFiles.push_back(fcb);
}


FCB* Directory::OpenFile(const std::string& fileName) {
	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++)
		if (curFile->childFiles[ii]->name == fileName) {
			curFile->childFiles[ii]->authorization = emDelDenied;
			return curFile->childFiles[ii];
		}
	OutMsg("错误：未找到相关文件");
	Error();
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
			OutMsg("文件" + fileName + "：");
			return curFile->childFiles[ii];
		}
	}
	OutMsg("错误：未找到相关文件");
	Error();
}


void Directory::ReadFile() {

}


void Directory::Back() {
	curFile = opFilePath.top();
	opFilePath.pop();
}


/**
 * \name rm  删除文件
 * \brief    删除文件对应的FCB
 */
std::vector<short> Directory::Rm(const std::string& toRemoveFile) {
	std::vector<short> idxBlocksId = {};

	for (size_t ii = 0; ii < curFile->childFiles.size(); ii++) {
		FCB* fcb = curFile->childFiles[ii];
		if (fcb->name != toRemoveFile)
			continue;
		if (fcb->authorization == emDelDenied) {
			OutMsg("当前文件在内存中，不允许删除！");
			return std::vector<short>();
		}
		if (fcb->type == DIR) {  // 目录文件
			// 无其他操作
		}
		else if (fcb->type == DATA) {  // 数据文件
			// 得到该文件对应磁盘中块号
			idxBlocksId = fcb->idxBlocksId;
		}
		curFile->childFiles.erase(curFile->childFiles.begin() + ii);
		delete fcb;
		return idxBlocksId;
	}
	OutMsg("要删除的文件不存在！");
	return std::vector<short>();
}


// -----------------------------修改类中私有变量相关函数--------------------
void Directory::UpdateCurFilePoint(const int& idx) {
	opFilePath.push(curFile);
	curFile = curFile->childFiles[idx];
}


// -----------------------------错误处理-----------------------------------
void Directory::Error() {

}


// -----------------------------工具函数-----------------------------------
int Directory::QueryDirectory(const std::string queryFile) {
	for (int ii = 0; ii < curFile->childFiles.size(); ii++) 
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
	//FCB* fcb = new FCB("boot", curFile->path, DATA);
	//fcb->len = 8;  // 占用1块
	//fcb->idxBlocksId.push_back(0); // 索引块为0
	//curFile->childFiles.push_back(fcb);
}


// -----------------------------公开接口-----------------------------------
std::string Directory::getCurPath() {
	return curFile->path;
}

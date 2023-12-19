#include "kernel.h"

Kernel::Kernel() {
	// ����Ŀ¼����ʼ������λ��ͼ
	//for (const auto& file : )
}


std::unique_ptr<PCB> Kernel::Fork() {
	// ʹ�� C++11 ���������
	std::random_device rd;   // ��ȡ����豸
	std::mt19937 mt(rd());   // ʹ�� Mersenne Twister ����
	std::uniform_int_distribution<int> dist(1, 10000);  // ���巶Χ

	// ����ָ�뽫���䳬����Χʱ�Զ��ͷ��ڴ�
	std::unique_ptr<PCB> p = std::make_unique<PCB>();
	p->pid = dist(mt);  // �������ID
	std::vector<int> allocMem = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (!allocMem.empty()) {
		for (size_t ii = 0; ii < InitProcessBlockNum; ii++) {
			// ��ҳ�� ��������
			int virtualPageNumber = ii;
			p->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
		}
	}
	else {
		std::cout << "���̴���ʧ��" << std::endl;
		return nullptr;
	}
	return p;
}


void Kernel::Exit(const std::unique_ptr<PCB>& process) {
	// �ͷŽ����е��ڴ�ռ�
	for (auto item : process->pageTable) {
		memorysystem.Free(process->pid);
	}
}


void Kernel::Open(const std::string& fileName) {
	// ��Ӧ���̻����Ӧ�ļ���Ȩ��
	sysProcess.openFiles.push_back(fileName);
}


void Kernel::Read(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();  // ��������
	if (!pcbPoint) {  // ���̴���ʧ��
		std::cout << "���ļ�" << fileName << "ʧ��" << std::endl;
	}
	else {
		auto targetFcbPoint = directory.OpenFile(fileName);
		if (targetFcbPoint->len > pcbPoint->pageTable.size()) {
			// 8���ڴ治������������
			int applyBlockNum = targetFcbPoint->len - pcbPoint->pageTable.size();
			std::vector<int> allocMem = memorysystem.Alloc(pcbPoint->pid, applyBlockNum);
			pcbPoint->AppendMmu(allocMem);
		}
		// ���ļ����ݴ��������ڴ�
		auto blockDatas = disk.Read(targetFcbPoint->idxBlocksId, targetFcbPoint->len);
		// ������ý��̵��ڴ����ܲ�������������Ҫһ��һ�鸴��
		for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
			// offset�������mem��ƫ����
			int offset = pcbPoint->pageTable[ii].second * BLOCK_SIZE;
			char* blockData = new char[BLOCK_SIZE];
			std::memcpy(blockData, blockDatas[ii], BLOCK_SIZE);
			memorysystem.AssignMem(offset, blockData);
		}
		memorysystem.DisplayMemUsage();
	}
	this->Exit(pcbPoint);  // �˳�����
}


void Kernel::Write(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();  // ��������
	std::string dataToWrite;
	if (!pcbPoint) {  // ���̴���ʧ��
	
	}
	else {
		auto targetFcbPoint = directory.OpenFile(fileName);
		std::getline(std::cin, dataToWrite);
		// �õ�Ҫд�����ݵĴ�С
		size_t sizeInBytes = sizeof(dataToWrite);
		if (targetFcbPoint->len * BLOCK_SIZE < sizeInBytes) {  // �ļ���С����Ҫ�Ӵ�������ռ�
			// �����Ҫ����Ĵ��̿���
			int applyBlockNum = (targetFcbPoint->len * BLOCK_SIZE - sizeInBytes) / 40 + 1;
			std::vector<int> newIdxBlocksId = disk.AllocDisk(targetFcbPoint->idxBlocksId.back(), targetFcbPoint->len, applyBlockNum);
			// ���¸��ļ���Ӧ��FCB��������
			targetFcbPoint->ExpandFileLen(newIdxBlocksId, applyBlockNum);
		}
		disk.Write(targetFcbPoint->idxBlocksId, dataToWrite);
	}
	this->Exit(pcbPoint);  // �˳�����
}


void Kernel::Close(const std::string& fileName) {
	sysProcess.openFiles.push_back(fileName);
}


void Kernel::Delete(const std::string& fileName) {
	auto toRemoveFileIdxBlocksId = directory.Rm(fileName);
	if (!toRemoveFileIdxBlocksId.empty())
		disk.FreeDisk(toRemoveFileIdxBlocksId);
}


void Kernel::Create(const std::string& fileName) {
	auto idxBlocksId = disk.AllocFileBlock();
	directory.Create(fileName, idxBlocksId);
}


std::string Kernel::getCurPath() {
	return directory.getCurPath();
}


// ����ʹ��command�������ж����ͺţ�Ӧ�ú궨�弸���ж�����
void Kernel::SysCall(COMMAND command, const std::string eax) {
	std::string targetFile = eax.substr(eax.find(" ") + 1);
	switch (command)
	{
	// Fileָ��
	case FILE_FORMAT:
		directory.format();
		break;
	case FILE_MKDIR:
		directory.mkdir();
		break;
	case FILE_RMDIR:
		directory.rmdir();
		break;
	case FILE_LS:
		directory.ls();
		break;
	case FILE_CD: 
		directory.cd(targetFile);
		break;
	case FILE_CREATE:
		this->Create(targetFile); break;
	case FILE_BACK:
		directory.Back(); break;
	case FILE_OPEN:
		this->Open(targetFile);   break;
	case FILE_CLOSE:			  
		this->Close(targetFile);  break;
	case FILE_WRITE:			  
		this->Write(targetFile);  break;
	case FILE_READ:				  
		this->Read(targetFile);   break;
	case FILE_RM:
		this->Delete(targetFile); break;
	default:
		break;
	}
}

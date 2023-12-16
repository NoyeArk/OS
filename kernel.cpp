#include "kernel.h"


std::unique_ptr<PCB> Kernel::Fork() {
	// ʹ�� C++11 ���������
	std::random_device rd;   // ��ȡ����豸
	std::mt19937 mt(rd());   // ʹ�� Mersenne Twister ����
	std::uniform_int_distribution<int> dist(1, 10000);  // ���巶Χ

	// ����ָ�뽫���䳬����Χʱ�Զ��ͷ��ڴ�
	std::unique_ptr<PCB> p = std::make_unique<PCB>();
	p->pid = dist(mt);  // �������ID
	int beginPhysicalBlockNumber = memorysystem.Alloc(p->pid, InitProcessBlockNum);
	if (beginPhysicalBlockNumber != INFINITY) {
		for (size_t ii = 0; ii < InitProcessBlockNum; ii++) {
			// ��ҳ�� ��������
			int virtualPageNumber = ii;
			p->pageTable.push_back(std::make_pair(virtualPageNumber, beginPhysicalBlockNumber + ii));
		}
	}
	else {
		std::cout << "���̴���ʧ��" << std::endl;
		return nullptr;
	}
	return p;
}


void Kernel::Exit(const std::unique_ptr<PCB>& process) {
	// �ͷŽ����е��ڴ�ռ䣬һ��һ�����
	for (auto item : process->pageTable) {
		memorysystem.Free(process->pid, );
	}
}


void Kernel::Open(const std::string& fileName) {
	std::unique_ptr<PCB> pcbPoint = Fork();
	if (!pcbPoint) {  // ���̴���ʧ��
		std::cout << "���ļ�" << fileName << "ʧ��" << std::endl;
	}
	else {
		auto targetFcbPoint = directory.open(fileName);
		if (targetFcbPoint->len > pcbPoint->pageTable.size()) {
			// 8���ڴ治������������
			int applyBlockNum = targetFcbPoint->len - pcbPoint->pageTable.size();
			int beginAddr = memorysystem.Alloc(pcbPoint->pid, applyBlockNum);
			pcbPoint->AppendMmu(beginAddr, applyBlockNum);
		}
		// ���ļ����ݴ��������ڴ�
		auto buff = disk.Read(targetFcbPoint->addr, targetFcbPoint->len);
		// ������ý��̵��ڴ����ܲ�������������Ҫһ��һ�鸴��
		for (size_t ii = 0; ii < targetFcbPoint->len; ii++) {
			// offset�������mem��ƫ����
			int offset = pcbPoint->pageTable[ii].second * BLOCK_SIZE;
			char* blockData = new char[BLOCK_SIZE];
			std::memcpy(blockData, buff + ii, BLOCK_SIZE);
			memorysystem.AssignMem(offset, blockData);
		}
		memorysystem.DisplayMemUsage();
	}
}


void Kernel::Close() {

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
		directory.create();
		break;
	case FILE_BACK:
		directory.back();
		break;
	case FILE_OPEN:
		Open(targetFile);
		break;
	case FILE_CLOSE:
		directory.close();
		break;
	case FILE_WRITE:
		directory.write();
		break;
	case FILE_READ:
		directory.read();
		break;
	case FILE_RM:
		directory.rm();
		break;
	default:
		break;
	}
}

#include "my_memory.h"

MCB::MCB() {
	this->pid = NULL_PROCESS;
	this->lastUseTime = INFINITY;
}


MCB::MCB(int pid, std::chrono::system_clock::time_point lastUseTime) : pid(pid) {
	// ����ǰʱ��ת��Ϊ����
	auto currentTimeInt = std::chrono::duration_cast<std::chrono::seconds>(lastUseTime.time_since_epoch()).count();
	this->lastUseTime = static_cast<int>(currentTimeInt);
}


/**
 * \name LRU �Ҿݴ�ʱʱ�������һ���ڴ��
 * \type int �ҵ����ڴ���±�
 * \return 
 */
int Memory::LRU(const std::vector<std::shared_ptr<PCB>>& pcbs) {
	int currentTime = GetCurrentIntTime();
	int pid = -1;                     // LRU�û����ڴ��������Ľ���
	int LRUBlockId;              // LRU�㷨ѡ�е��ڴ���
	int maxTimeDifference = -1;  // ��¼���ʱ���

	for (size_t ii = 0; ii < pcbs.size(); ii++) {
		auto pcb = pcbs[ii];
		// �����ý��̵�ҳ��
		for (auto const& item : pcb->pageTable) {
			if (currentTime - item.lastUseTime > maxTimeDifference) {
				pid = pcb->pid;
				maxTimeDifference = currentTime - item.lastUseTime;
				LRUBlockId = item.memBlockId;
			}
		}
	}

	// �޸�pid��Ӧ�Ľ��̵�LRUBlockId��ҳ����isInMemΪFalse
	for (size_t ii = 0; ii < pcbs.size(); ii++) {
		auto pcb = pcbs[ii];
		if (pcb->pid != pid)
			continue;
		for (size_t jj = 0; jj < pcb->pageTable.size(); ii++) {
			if (pcb->pageTable[jj].memBlockId == LRUBlockId) {
				pcb->pageTable[jj].isInMem = false;
				break;
			}
		}
		break;
	}
	return LRUBlockId;
}


int Memory::GetCurrentIntTime() {
	auto currentTime = std::chrono::system_clock::now();
	auto currentTimeInt = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
	int intTime = static_cast<int>(currentTimeInt);
	return intTime;
}


std::vector<int> Memory::PagesReplace(const std::vector<std::shared_ptr<PCB>>& pcbs, int pageNumToReplace) {
	std::vector<int> replaceBlocksId;  // ��Ҫ���û��������ڴ���
	for (size_t ii = 0; ii < pageNumToReplace; ii++) {
		int lruBlockId = this->LRU(pcbs);
		replaceBlocksId.push_back(lruBlockId);
	}
	return replaceBlocksId;
}


Memory::Memory() {
	// ��ʼ����64����
	memAllocList.resize(MEM_BLOCK_NUM);
	// ����һ���СΪ2560�ֽڵ��ڴ�
	mem = new char[MEM_SIZE];

	// ���ڴ���ж����Ʋ�������ʼ������λ����Ϊ0
	std::memset(mem, 0x00, MEM_SIZE);

	memScheTime = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


Memory::~Memory() {
	// �ͷŶ�̬������ڴ�
	delete mem;
}


/**
 * \name Alloc             ���������ڴ�
 * \type std::vector<int>  ������ڴ������ż���
 * \param pid              ������̺�
 * \param requestBlockNum  �����������
 * \brief                  ����õ����ڴ����ܲ�������
 */
std::vector<int> Memory::Alloc(int pid, int requestBlockNum) {
	std::vector<int> allocateMem;
	// �Ի�����memAllocList���в��� ����
	std::lock_guard<std::mutex> lock(listMutex);
	for (size_t blockId = 0; blockId < MEM_BLOCK_NUM; blockId++) {
		if (memAllocList[blockId].pid == NULL) {
			memAllocList[blockId].pid = pid;
			memAllocList[blockId].lastUseTime = GetCurrentIntTime();
			allocateMem.push_back(blockId);
			if (allocateMem.size() == requestBlockNum)
				break;
		}
	}
	if (allocateMem.size() < requestBlockNum) {
		// �ڴ治��������ҳ��ȫ���û�LRU��������δʹ�ã� ���� ϵͳ����PageReplaceInterrupt
		std::cout << "�ڴ治��������ҳ��ȫ���û�LRU" << std::endl;
	}
	return allocateMem;
}


/**
 * \name Free          �ͷŽ�����ӵ�е��ڴ�
 * \param pid		   ���̺�
 * \param blockNumber  �ͷ��ڴ����
 * \brief              һ�����ͷ�ȫ�����ڴ�
 */
void Memory::Free(const int& pid, std::vector<int> toFreeBlocksId) {
	std::lock_guard<std::mutex> lock(listMutex);
	for (auto blockId : toFreeBlocksId) {
		if (memAllocList[blockId].pid == pid)
			memAllocList[blockId].pid = NULL_PROCESS;
		else
			std::cout << "����ҳ�����������ڴ�ʹ���������������ֳ�ͻ��" << std::endl;
	}
}


void Memory::WriteMem(int blockId, char* blockData) {
	int offset = blockId * MEM_BLOCK_SIZE;
	std::lock_guard<std::mutex> lock(memMutex);
	std::memcpy(mem + offset, blockData, MEM_BLOCK_SIZE);
}


char* Memory::ReadMem(int blockId) {
	int offset = blockId * MEM_BLOCK_SIZE;
	// ����һ�����������洢�� mem ָ��λ�ÿ�ʼ������
	char blockData[MEM_BLOCK_SIZE];  // ���ֽ�Ϊ��λ
	// ʹ�� std::memcpy �� mem ��ָ��λ�ø������ݵ� buffer ��
	std::memcpy(blockData, mem + offset, MEM_BLOCK_SIZE);
	return blockData;
}
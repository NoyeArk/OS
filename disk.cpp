#include "disk.h"

std::vector<int> Disk::GetFreeBlocks(int applyBlockNum) {
    std::vector<int> freeLoc;
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    if (fileBitMap.none()) {
        std::cout << "�޿��п�" << std::endl;
        return {};
    }
    for (size_t blockId = 0; blockId < FILE_BLOCK_NUM; blockId++) {
        if (fileBitMap.test(blockId)) {
            fileBitMap.reset(blockId);
            freeLoc.push_back(blockId);
        }
        if (freeLoc.size() == applyBlockNum)
            break;
    }
    return freeLoc;
}

std::vector<int> Disk::GetDataBlocksId(const std::vector<int>& idxBlocksNum) {
    std::vector<int> dataBlocksId;
    for (const auto& idxBlock : idxBlocksNum) {
        char* binaryDataBlocksId = this->ReadSingleBlockFromDisk(idxBlock);
        // �Ѷ������ַ���ת��Ϊint��
        std::stringstream ss(binaryDataBlocksId);

        int num;
        while (ss >> num) {
            dataBlocksId.push_back(num);
        }
    }
    if (dataBlocksId.empty())
        return std::vector<int>();
    return dataBlocksId;
}


char* Disk::ReadSingleBlockFromDisk(const int& addr) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    // ���ö�ȡλ��
    rwCursor.seekg(addr * BLOCK_SIZE);

    // ��ȡ1 * 40b������
    char* buff = new char[1 * BLOCK_SIZE];
    rwCursor.read(buff, 1 * BLOCK_SIZE);

    //std::cout << "Read " << rwCursor.gcount() << " bytes from the file." << std::endl;
    //std::cout.write(buff, rwCursor.gcount()) << std::endl;

    return buff;
}


void Disk::WriteSingleBlockToDisk(const int& addr, const std::string& dataToWrite) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    rwCursor.seekg(addr * BLOCK_SIZE);
    rwCursor.write(dataToWrite.c_str(), dataToWrite.size());

    std::cout << "Write Successful!!!" << std::endl;
}


Disk::Disk() {
    std::string diskPath = R"(F:\\code\\os\\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);

    // ��λʾͼ����λ����1
    fileBitMap.set();
    swapBitMap.set();
}


Disk::~Disk() {
    // �ر��ļ�
    rwCursor.close();
}


std::vector<int> Disk::QueryFreeFileBlock() {
    std::vector<int> freeLoc;
    std::lock_guard<std::mutex> lock(fileBitMapMutex);

    for (size_t blockId = 0; blockId < FILE_BLOCK_NUM; blockId++) {
        if (fileBitMap.test(blockId)) {
            freeLoc.push_back(blockId);
        }
    }
    return freeLoc;
}


void Disk::SetBitMap(int blockId, bool data) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);

    if (data) fileBitMap.set(blockId);
    else fileBitMap.reset(blockId);
}


std::vector<int> Disk::AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum) {
    std::vector<int> newIdxBlocksId;
    auto dataBlocksId = this->GetFreeBlocks(applyBlockNum);
    // ����·���Ĵ��̿����������һ�������飨���10������ʣ��λ�ã���Ҫ�ٷ���һ���µ�������
    // ��μ������һ����������ʣ��λ�ã��ļ����� % 10
    int availablePosNum = curFileLen % 10;
    if (applyBlockNum > availablePosNum) {
        // ��Ҫ�ٷ���applyIdxBlockNum������������
        int applyIdxBlockNum = (applyBlockNum - availablePosNum) / 10 + 1;
        newIdxBlocksId = this->GetFreeBlocks(applyIdxBlockNum);
    }
    else {
        // ����Ҫ����
    }
    size_t curWriteIdx = 0;
    // ����ִ�е��⣬�������ǹ��ģ�������Ҫ���·�������ݿ��д��������
    if (availablePosNum >= applyBlockNum) {
        // �����һ����������д��applyBlockNum������
        for (size_t ii = 0; ii < applyBlockNum; ii++) {
            this->WriteSingleBlockToDisk(lastIdxBlockNum, std::to_string(dataBlocksId[curWriteIdx]));
            ++curWriteIdx;
        }
    }
    else {
        // ���һ���������������һ��д��availablePosNum������
        for (size_t ii = 0; ii < availablePosNum; ii++) {
            this->WriteSingleBlockToDisk(lastIdxBlockNum, std::to_string(dataBlocksId[curWriteIdx]));
            ++curWriteIdx;
        }
        // ���µ���������д������������
        int remainIdxToWriteNum = dataBlocksId.size() - curWriteIdx;
        for (auto idxBlockId : newIdxBlocksId) {
            // ÿ����д10������
            for (size_t ii = 0; ii < 10; ii++) {
                this->WriteSingleBlockToDisk(idxBlockId, std::to_string(dataBlocksId[curWriteIdx]));
                ++curWriteIdx;
                if (curWriteIdx == dataBlocksId.size()) {
                    // д�����
                    return newIdxBlocksId;
                }
            }
        }
    }
    return std::vector<int>();
}


std::vector<int> Disk::AllocFileBlock() {
    // ����һ���������8�����ݿ�
    auto dataBlocksId = this->GetFreeBlocks(8);
    auto newIdxBlocksId = this->GetFreeBlocks(1);
    // ÿ����д8������
    for (size_t ii = 0; ii < dataBlocksId.size(); ii++) {
        this->WriteSingleBlockToDisk(newIdxBlocksId.back(), std::to_string(dataBlocksId[ii]));
    }
    return newIdxBlocksId;
}


void Disk::FreeDisk(const std::vector<int>& toFreeIdxBlocksId) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    auto dataBlocksId = this->GetDataBlocksId(toFreeIdxBlocksId);
    for (const auto& blockId : dataBlocksId) {
        // ����λʾͼ
        fileBitMap.reset(blockId);
        // ��Ӧ��������0
        this->WriteSingleBlockToDisk(blockId, "");
    }
}


/**
* \param addr��    Ҫ������ʼ��
* \param blockNum��Ҫ���Ŀ����
*/
std::vector<char*> Disk::Read(const std::vector<int>& idxBlocksNum, const int& blockNum) {
    std::vector<char*> blockDatas;
    std::vector<int> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    // У��
    if (blockNum != dataBlocksId.size()) {
        std::cout << "Ŀ¼���ļ�����������������ļ�������ƥ�䣡" << std::endl;
        return;
    }
    for (auto blockId : dataBlocksId) {
        char* data = this->ReadSingleBlockFromDisk(blockId);
        if (data != nullptr)
            blockDatas.push_back(data);
        delete data;
    }
    return blockDatas;
}


void Disk::Write(const std::vector<int>& idxBlocksNum, const std::string& dataToWrite) {
    std::vector<int> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    std::vector<std::string> blocksData;

    int dataSize = dataToWrite.size();
    int start = 0;

    while (start < dataSize) {
        size_t length = std::min(40, dataSize - start); // ÿ�ε���󳤶�Ϊ segmentSize
        std::string segment = dataToWrite.substr(start, length); // ��ȡ���ַ���
        blocksData.push_back(segment); // �����ַ�����ӵ������

        start += length; // ������һ��Ƭ�ε���ʼλ��
    }
    for (size_t ii = 0; ii < blocksData.size(); ii++) {
        this->WriteSingleBlockToDisk(dataBlocksId[ii], blocksData[ii]);
    }
}


void Disk::CreateDisk() {
    std::size_t diskSize = 40 * BLOCK_NUM;  // 1 KB = 1024 bytes

    if (rwCursor.is_open()) {
        // ���ļ�ָ���ƶ���ָ����С
        rwCursor.seekp(diskSize - 1, std::ios::beg);

        // д��һ���ֽڣ���ȷ���ļ�ռ��ָ����С
        rwCursor.write("", 1);
        std::cout << "���̴����ɹ���" << std::endl;
    }
    else {
        std::cerr << "�޷��򿪴��̣�" <<  std::endl;
    }
}
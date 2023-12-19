#include "disk.h"

std::vector<int> Disk::GetFreeBlocks(int applyBlockNum) {
    std::vector<int> freeLoc;
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    if (fileBitMap.none()) {
        std::cout << "无空闲块" << std::endl;
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
        // 把二进制字符串转换为int型
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
    // 设置读取位置
    rwCursor.seekg(addr * BLOCK_SIZE);

    // 读取1 * 40b的内容
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

    // 将位示图所有位均置1
    fileBitMap.set();
    swapBitMap.set();
}


Disk::~Disk() {
    // 关闭文件
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
    // 如果新分配的磁盘块数大于最后一个索引块（最多10个）中剩余位置，需要再分配一个新的索引块
    // 如何计算最后一个索引块中剩余位置：文件长度 % 10
    int availablePosNum = curFileLen % 10;
    if (applyBlockNum > availablePosNum) {
        // 需要再分配applyIdxBlockNum个块用于索引
        int applyIdxBlockNum = (applyBlockNum - availablePosNum) / 10 + 1;
        newIdxBlocksId = this->GetFreeBlocks(applyIdxBlockNum);
    }
    else {
        // 不需要分配
    }
    size_t curWriteIdx = 0;
    // 程序执行到这，索引块是够的，现在需要把新分配的数据块号写入索引块
    if (availablePosNum >= applyBlockNum) {
        // 在最后一个索引块中写入applyBlockNum个索引
        for (size_t ii = 0; ii < applyBlockNum; ii++) {
            this->WriteSingleBlockToDisk(lastIdxBlockNum, std::to_string(dataBlocksId[curWriteIdx]));
            ++curWriteIdx;
        }
    }
    else {
        // 最后一个不够，先在最后一个写入availablePosNum个索引
        for (size_t ii = 0; ii < availablePosNum; ii++) {
            this->WriteSingleBlockToDisk(lastIdxBlockNum, std::to_string(dataBlocksId[curWriteIdx]));
            ++curWriteIdx;
        }
        // 在新的索引块中写入其他的索引
        int remainIdxToWriteNum = dataBlocksId.size() - curWriteIdx;
        for (auto idxBlockId : newIdxBlocksId) {
            // 每个块写10个数据
            for (size_t ii = 0; ii < 10; ii++) {
                this->WriteSingleBlockToDisk(idxBlockId, std::to_string(dataBlocksId[curWriteIdx]));
                ++curWriteIdx;
                if (curWriteIdx == dataBlocksId.size()) {
                    // 写入结束
                    return newIdxBlocksId;
                }
            }
        }
    }
    return std::vector<int>();
}


std::vector<int> Disk::AllocFileBlock() {
    // 分配一个索引块和8个数据块
    auto dataBlocksId = this->GetFreeBlocks(8);
    auto newIdxBlocksId = this->GetFreeBlocks(1);
    // 每个块写8个数据
    for (size_t ii = 0; ii < dataBlocksId.size(); ii++) {
        this->WriteSingleBlockToDisk(newIdxBlocksId.back(), std::to_string(dataBlocksId[ii]));
    }
    return newIdxBlocksId;
}


void Disk::FreeDisk(const std::vector<int>& toFreeIdxBlocksId) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    auto dataBlocksId = this->GetDataBlocksId(toFreeIdxBlocksId);
    for (const auto& blockId : dataBlocksId) {
        // 更新位示图
        fileBitMap.reset(blockId);
        // 对应块内容置0
        this->WriteSingleBlockToDisk(blockId, "");
    }
}


/**
* \param addr：    要读的起始块
* \param blockNum：要读的块个数
*/
std::vector<char*> Disk::Read(const std::vector<int>& idxBlocksNum, const int& blockNum) {
    std::vector<char*> blockDatas;
    std::vector<int> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    // 校验
    if (blockNum != dataBlocksId.size()) {
        std::cout << "目录中文件块个数与索引块中文件个数不匹配！" << std::endl;
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
        size_t length = std::min(40, dataSize - start); // 每段的最大长度为 segmentSize
        std::string segment = dataToWrite.substr(start, length); // 获取子字符串
        blocksData.push_back(segment); // 将子字符串添加到结果中

        start += length; // 更新下一个片段的起始位置
    }
    for (size_t ii = 0; ii < blocksData.size(); ii++) {
        this->WriteSingleBlockToDisk(dataBlocksId[ii], blocksData[ii]);
    }
}


void Disk::CreateDisk() {
    std::size_t diskSize = 40 * BLOCK_NUM;  // 1 KB = 1024 bytes

    if (rwCursor.is_open()) {
        // 将文件指针移动到指定大小
        rwCursor.seekp(diskSize - 1, std::ios::beg);

        // 写入一个字节，以确保文件占据指定大小
        rwCursor.write("", 1);
        std::cout << "磁盘创建成功：" << std::endl;
    }
    else {
        std::cerr << "无法打开磁盘：" <<  std::endl;
    }
}
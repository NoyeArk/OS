#include "disk.h"


Disk::Disk() {
    std::string diskPath = R"(D:\\qtonline\\run\\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);
    CreateDisk();

    // 将位示图所有位均置1
    fileBitMap.set();
    swapBitMap.set();

    // ReadSingleBlockFromDisk(0);
}


Disk::~Disk() {
    // 关闭文件
    rwCursor.close();
}

std::vector<short> Disk::GetFreeBlockId()
{
    std::vector<short> freeLoc;
    if (fileBitMap.none()) {
        std::cout << "无空闲块" << std::endl;
        return {};
    }
    for (size_t blockId = 0; blockId < FILE_BLOCK_NUM; blockId++) {
        if (fileBitMap.test(blockId)) {
            freeLoc.push_back(blockId);
        }
    }
    return freeLoc;
}


/**
 * \name GetFreeBlocks       从磁盘文件区中申请applyBlockNum个空闲块
 * \type std::vector<short>  申请得到的空闲文件块号集合
 * \param applyBlockNum      申请的空闲块个数
 */
std::vector<short> Disk::GetFreeBlocks(int applyBlockNum) {
    std::vector<short> freeLoc;
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


/**
 * \name GetDataBlocksId     得到索引块中索引对应的文件块号
 * \type std::vector<short>  文件块号集合
 * \param idxBlocksNum       索引块号链表
 */
std::vector<short> Disk::GetDataBlocksId(const std::vector<short>& idxBlocksNum) {
    std::vector<short> dataBlocksId;
    for (const auto& idxBlock : idxBlocksNum) {
        char* binaryDataBlocksId = this->ReadSingleBlockFromDisk(idxBlock);
        short* byteInAscii = new short[BLOCK_SIZE];
        int blockId = -1;

        for (int ii = 0; ii < BLOCK_SIZE; ++ii) {
            byteInAscii[ii] = static_cast<short>(binaryDataBlocksId[ii]);
            byteInAscii[ii] -= 48;
            if (ii % 2 == 1) {  // 奇数位置
                if (byteInAscii[ii - 1] < 0 && byteInAscii[ii] < 0) {  // 无效数字
                    // 跳过
                    blockId = byteInAscii[ii - 1];
                }
                else if (byteInAscii[ii - 1] > 0 && byteInAscii[ii] < 0) {  // 个位数
                    blockId = byteInAscii[ii - 1];
                }
                else if (byteInAscii[ii - 1] > 0 && byteInAscii[ii] > 0) {  // 个位数
                    blockId = byteInAscii[ii - 1] * 10 + byteInAscii[ii];
                }
                if (blockId >= 0)
                    dataBlocksId.push_back(blockId);
            }
        }
        delete binaryDataBlocksId;
        delete[] byteInAscii;
    }
    if (dataBlocksId.empty())
        return std::vector<short>();
    return dataBlocksId;
}


/**
 * \name UpdateIdxBlock   更新索引块中索引
 * \param idxBlockId      要更新的索引块号
 * \param beginOffset     要写入的起始块内地址 即块内偏移
 * \param dataBlocksId    要写入的索引号
 */
void Disk::UpdateIdxBlock(int idxBlockId, int beginOffset, std::vector<short> dataBlocksId) {
    for (size_t ii = 0; ii < dataBlocksId.size(); ii++) {
        int blockIdToWrite = dataBlocksId[ii];
        int offset = beginOffset + ii * IDX_SIZE;
        int effectiveAddr = idxBlockId * BLOCK_SIZE + offset;
        this->WriteSingleBlockToDisk(effectiveAddr, IDX_SIZE, std::to_string(blockIdToWrite).c_str());
    }
    this->FlushBuffer();
}


/**
 * \name ReadSingleBlockFromDisk  从磁盘中读出一个块的数据
 * \type char *                   读出的数据
 * \param blockIdToRead           要读的块号
 */
char* Disk::ReadSingleBlockFromDisk(const int& blockIdToRead) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    rwCursor.seekg(blockIdToRead * BLOCK_SIZE);

    char* buff = new char[BLOCK_SIZE];
    rwCursor.read(buff, BLOCK_SIZE);

    //std::cout << "Read " << rwCursor.gcount() << " bytes from the file." << std::endl;
    //std::cout.write(buff, rwCursor.gcount()) << std::endl;
    return buff;
}


/**
 * \name WriteSingleBlockToDisk  对磁盘中一个块的内容进行改写
 * \param effectiveAddr          要写入的有效地址
 * \param writeLen               要写入的数据长度，以字节为单位
 * \param dataToWrite            要写入的数据
 * \brief                        细致到对该块的第几个字节开始写入,offset以字节为单位
 */
void Disk::WriteSingleBlockToDisk(const int& effectiveAddr, const int& writeLen, const char* dataToWrite) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening disk.data" << std::endl;
        exit(-1);
    }
    rwCursor.seekg(effectiveAddr);
    rwCursor.write(dataToWrite, writeLen);

    DebugCout("此次写入的数据为");
}



void Disk::FlushBuffer() {
    this->WriteSingleBlockToDisk(0, 0, "");
}


void Disk::DebugCout(std::string info) {
    if (!Debug) return;
    std::cout << "Disk::" << info << std::endl;
}


/**
 * \name WriteSwap           对换区进行写操作
 * \param toWriteBlocksData  要写入的数据集合
 */
void Disk::WriteSwap(const std::vector<char*>& toWriteBlocksData) {
    int swapBlockId = 900;
    for (auto blockData : toWriteBlocksData) {
        swapBitMap.reset(swapBlockId++);
        int effectiveAddr = swapBlockId * BLOCK_SIZE;
        this->WriteSingleBlockToDisk(effectiveAddr, BLOCK_SIZE, blockData);
        ++swapBlockId;
        delete blockData;
    }
    this->FlushBuffer();
}


/**
 * \name ReadSwap            对换区进行读操作
 * \type std::vector<char*>  返回读出的数据集合
 * \param blockNum           要读的块个数
 */
std::vector<char*> Disk::ReadSwap(const int& blockNum) {
    std::vector<char*> blockDatas;
    for (size_t ii = 0; ii < blockNum; ii++) {
        int blockIdToRead = 900 + ii;
        char* data = this->ReadSingleBlockFromDisk(blockIdToRead);
        if (data != nullptr)
            blockDatas.push_back(data);
        delete data;
    }
    return blockDatas;
}


/**
 * \name QueryFreeFileBlock  查询当前磁盘文件区的空闲块号
 * \return                   返回当前空闲块号集合
 */
std::vector<short> Disk::QueryFreeFileBlock() {
    std::vector<short> freeLoc;
    std::lock_guard<std::mutex> lock(fileBitMapMutex);

    for (size_t blockId = 0; blockId < FILE_BLOCK_NUM; blockId++) {
        if (fileBitMap.test(blockId)) {
            freeLoc.push_back(blockId);
        }
    }
    return freeLoc;
}


/**
 * \name SetBitMap  对位示图进行写操作
 * \param blockId   写入位示图中对应的块号
 * \param data      要写入的信息 true-1 false-0
 */
void Disk::SetBitMap(int blockId, bool data) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);

    if (data) fileBitMap.set(blockId);
    else fileBitMap.reset(blockId);
}


/**
 * \name AllocDisk         申请新的磁盘文件区空闲块
 * \param lastIdxBlockNum  申请进程中对应的最后一个索引块号
 * \param curFileLen       要存放的目标文件的长度
 * \param applyBlockNum    申请的空闲块个数
 * \return 
 */
std::vector<short> Disk::AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum) {
    std::vector<short> newIdxBlocksId;
    auto dataBlocksId = this->GetFreeBlocks(applyBlockNum);
    if (dataBlocksId.empty()) {
        std::cout << "磁盘盘块申请失败！" << std::endl;
        return std::vector<short>();
    }
    // 如果新分配的磁盘块数大于最后一个索引块（最多20个）中剩余位置，需要再分配一个新的索引块
    // 如何计算最后一个索引块中剩余位置：(20 - 文件长度 % 20)
    int availablePosNum = IDX_BLOCK_IDX_NUM - curFileLen % IDX_BLOCK_IDX_NUM;
    if (applyBlockNum > availablePosNum) {
        // 需要再分配applyIdxBlockNum个块用于索引
        int applyIdxBlockNum = (applyBlockNum - availablePosNum) / IDX_BLOCK_IDX_NUM + 1;
        newIdxBlocksId = this->GetFreeBlocks(applyIdxBlockNum);
    }
    else {
        // 不需要分配新的索引块
    }
    size_t curWriteIdx = 0;
    // 程序执行到这，索引块是够的，现在需要把新分配的数据块号写入索引块
    if (availablePosNum >= applyBlockNum) {  // 最后一个索引块中足够此次分配数据块号写入
        // 在最后一个索引块中写入applyBlockNum个索引
        int beginOffset = (curFileLen % IDX_BLOCK_IDX_NUM) * IDX_SIZE;
        this->UpdateIdxBlock(lastIdxBlockNum, beginOffset, dataBlocksId);
    }
    else {
        // 最后一个索引块容量不够，先在最后一个索引块中写入剩余availablePosNum个索引
        std::vector<short> subDataBlocksId(dataBlocksId.begin(), dataBlocksId.begin() + availablePosNum);
        int beginOffset = (curFileLen % IDX_BLOCK_IDX_NUM) * IDX_SIZE;
        this->UpdateIdxBlock(lastIdxBlockNum, beginOffset, subDataBlocksId);

        // 在新的索引块中写入剩余的索引
        std::vector<short> remainDataBlocksId(dataBlocksId.begin() + availablePosNum, dataBlocksId.end());
        for (size_t ii = 0; ii < newIdxBlocksId.size(); ii++) {
            std::vector<short> subDataBlocksId = {};
            if (remainDataBlocksId.size() <= IDX_BLOCK_IDX_NUM) {  // 不够一次写满索引块，不需要截取
                subDataBlocksId.insert(subDataBlocksId.begin(), remainDataBlocksId.begin(), dataBlocksId.end());
            }
            else {  // 截取前20个索引作为此次要写入的索引，同时更新剩余待写入的索引
                subDataBlocksId.insert(subDataBlocksId.begin(), remainDataBlocksId.begin(), dataBlocksId.begin() + IDX_BLOCK_IDX_NUM);
                remainDataBlocksId.erase(remainDataBlocksId.begin(), remainDataBlocksId.begin() + IDX_BLOCK_IDX_NUM);
            }

            int idxBlockId = newIdxBlocksId[ii];
            int beginOffset = ii * IDX_SIZE;  // 从块内0地址开始
            this->UpdateIdxBlock(idxBlockId, beginOffset, subDataBlocksId);
        }
    }
    return newIdxBlocksId;
}


/**
 * \name AllocFileBlock   创建进程时为其分配8个文件块和1个索引块
 * \return                返回为其分配的索引块号
 */
std::vector<short> Disk::AllocFileBlock() {
    // 分配一个索引块和8个数据块
    auto newIdxBlocksId = this->GetFreeBlocks(1);
    auto dataBlocksId = this->GetFreeBlocks(8);

    // 使用QtConcurrent::run运行函数，并传递参数
//    QtConcurrent::run([=]() {
//        // 在新线程中运行的代码
//        // 将8个数据块的块号写入索引块中

//    });

    for (size_t ii = 0; ii < dataBlocksId.size(); ii++) {
        int offset = IDX_SIZE * ii;
        int effectiveAddr = newIdxBlocksId.back() * BLOCK_SIZE + offset;
        this->WriteSingleBlockToDisk(effectiveAddr, IDX_SIZE, std::to_string(dataBlocksId[ii]).c_str());
//        std::vector<short> addrToWrite = { (short)effectiveAddr };

//        ProcessCommunication::ShakeSend("DISK", "WriteBlockId", addrToWrite, std::to_string(dataBlocksId[ii]).c_str());
//        std::string msgReceiveFromDisk = ProcessCommunication::ShakeReceive("DISK");
    }

    // 返回索引块号
    return newIdxBlocksId;
}


/**
 * \name FreeDisk            删除文件时释放其所占有的文件块和索引块
 * \param toFreeIdxBlocksId  要清除的文件所在索引块号
 */
void Disk::FreeDisk(const std::vector<short>& toFreeIdxBlocksId) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    auto dataBlocksId = this->GetDataBlocksId(toFreeIdxBlocksId);
    // 清空文件块数据
    for (const auto& blockId : dataBlocksId) {
        fileBitMap.reset(blockId);
        int effectiveAddr = blockId * BLOCK_SIZE;
        this->WriteSingleBlockToDisk(effectiveAddr, BLOCK_SIZE, "");
    }
    // 清空索引块数据
    for (auto blockId : toFreeIdxBlocksId) {
        fileBitMap.reset(blockId);
        this->WriteSingleBlockToDisk(blockId * BLOCK_SIZE, BLOCK_SIZE, "");
    }
    this->FlushBuffer();
}


/**
* \param addr：    要读的起始块
* \param blockNum：要读的块个数
*/
std::vector<std::string> Disk::ReadFile(const std::vector<short>& idxBlocksNum, const int& blockNum) {
    std::vector<std::string> blockDatas;
    std::vector<short> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    for (auto blockId : dataBlocksId) {
        char* data = this->ReadSingleBlockFromDisk(blockId);
        if (data != nullptr)
            blockDatas.push_back(data);
    }
    return blockDatas;
}


/**
 * \name WriteFile      写磁盘文件区
 * \param idxBlocksNum  要写入文件块所在的索引块号
 * \param dataToWrite   要写入的数据
 */
void Disk::WriteFile(const std::vector<short>& idxBlocksNum,
                     const std::string& dataToWrite) {
    std::vector<short> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    std::vector<std::string> blocksData;

    int dataSize = dataToWrite.size();
    int start = 0;

    while (start < dataSize) {
        int length = std::min(40, dataSize - start); // 每段的最大长度为 segmentSize
        std::string segment = dataToWrite.substr(start, length); // 获取子字符串
        if (length < 40) {
            segment.append(40 - length, '\0'); // 使用空字符填充
        }
        blocksData.push_back(segment); // 将子字符串添加到结果中
        start += length; // 更新下一个片段的起始位置
    }

    if (blocksData.size() < 8) {
        int remain_block_num = 8 - blocksData.size();
        for (size_t ii = 0; ii < remain_block_num; ii++) {
            std::string segment;
            segment.resize(40);
            blocksData.push_back(segment);
        }
    }

    for (size_t ii = 0; ii < blocksData.size(); ii++) {
        int effectiveAddr = dataBlocksId[ii] * BLOCK_SIZE;
        this->WriteSingleBlockToDisk(effectiveAddr, BLOCK_SIZE, blocksData[ii].c_str());
    }
    this->FlushBuffer();
}


void Disk::CreateDisk() {
    int diskSize = BLOCK_NUM * BLOCK_SIZE;

    if (rwCursor.is_open()) {
        // 将文件指针移动到指定大小
        rwCursor.seekp(diskSize - 1, std::ios::beg);

        // 写入一个字节，以确保文件占据指定大小
        rwCursor.write("", 1);
        std::cout << "磁盘创建成功！" << std::endl;
    }
    else {
        std::cerr << "无法打开磁盘：" <<  std::endl;
    }
}

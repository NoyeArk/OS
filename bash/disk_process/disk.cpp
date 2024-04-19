#include "disk.h"


Disk::Disk() {
    std::string diskPath = R"(C:\Users\dell\Desktop\disk_process\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);
    CreateDisk();
}


Disk::~Disk() {
    // 关闭文件
    rwCursor.close();
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
}


void Disk::FlushBuffer() {
    this->WriteSingleBlockToDisk(0, 0, "");
}


/**
 * \name WriteSwap           对换区进行写操作
 * \param toWriteBlocksData  要写入的数据集合
 */
void Disk::WriteSwap(const std::vector<char*>& toWriteBlocksData) {
    int swapBlockId = 900;
    for (auto blockData : toWriteBlocksData) {
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
* \param addr：    要读的起始块
* \param blockNum：要读的块个数
*/
std::vector<std::string> Disk::ReadFile(const std::vector<short>& idxBlocksNum) {
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
void Disk::WriteFile(const std::vector<short>& idxBlocksNum, const std::string& dataToWrite) {
    std::vector<short> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    std::vector<std::string> blocksData;

    int dataSize = dataToWrite.size();
    int start = 0;

    while (start < dataSize) {
        int length = std::min(40, dataSize - start); // 每段的最大长度为 segmentSize
        std::string segment = dataToWrite.substr(start, length); // 获取子字符串
        blocksData.push_back(segment); // 将子字符串添加到结果中

        start += length; // 更新下一个片段的起始位置
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
        std::cerr << "无法打开磁盘：" << std::endl;
    }
}
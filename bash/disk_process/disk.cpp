#include "disk.h"


Disk::Disk() {
    std::string diskPath = R"(C:\Users\dell\Desktop\disk_process\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);
    CreateDisk();
}


Disk::~Disk() {
    // �ر��ļ�
    rwCursor.close();
}


/**
 * \name GetDataBlocksId     �õ���������������Ӧ���ļ����
 * \type std::vector<short>  �ļ���ż���
 * \param idxBlocksNum       �����������
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
            if (ii % 2 == 1) {  // ����λ��
                if (byteInAscii[ii - 1] < 0 && byteInAscii[ii] < 0) {  // ��Ч����
                    // ����
                    blockId = byteInAscii[ii - 1];
                }
                else if (byteInAscii[ii - 1] > 0 && byteInAscii[ii] < 0) {  // ��λ��
                    blockId = byteInAscii[ii - 1];
                }
                else if (byteInAscii[ii - 1] > 0 && byteInAscii[ii] > 0) {  // ��λ��
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
 * \name UpdateIdxBlock   ����������������
 * \param idxBlockId      Ҫ���µ��������
 * \param beginOffset     Ҫд�����ʼ���ڵ�ַ ������ƫ��
 * \param dataBlocksId    Ҫд���������
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
 * \name ReadSingleBlockFromDisk  �Ӵ����ж���һ���������
 * \type char *                   ����������
 * \param blockIdToRead           Ҫ���Ŀ��
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
 * \name WriteSingleBlockToDisk  �Դ�����һ��������ݽ��и�д
 * \param effectiveAddr          Ҫд�����Ч��ַ
 * \param writeLen               Ҫд������ݳ��ȣ����ֽ�Ϊ��λ
 * \param dataToWrite            Ҫд�������
 * \brief                        ϸ�µ��Ըÿ�ĵڼ����ֽڿ�ʼд��,offset���ֽ�Ϊ��λ
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
 * \name WriteSwap           �Ի�������д����
 * \param toWriteBlocksData  Ҫд������ݼ���
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
 * \name ReadSwap            �Ի������ж�����
 * \type std::vector<char*>  ���ض��������ݼ���
 * \param blockNum           Ҫ���Ŀ����
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
* \param addr��    Ҫ������ʼ��
* \param blockNum��Ҫ���Ŀ����
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
 * \name WriteFile      д�����ļ���
 * \param idxBlocksNum  Ҫд���ļ������ڵ��������
 * \param dataToWrite   Ҫд�������
 */
void Disk::WriteFile(const std::vector<short>& idxBlocksNum, const std::string& dataToWrite) {
    std::vector<short> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    std::vector<std::string> blocksData;

    int dataSize = dataToWrite.size();
    int start = 0;

    while (start < dataSize) {
        int length = std::min(40, dataSize - start); // ÿ�ε���󳤶�Ϊ segmentSize
        std::string segment = dataToWrite.substr(start, length); // ��ȡ���ַ���
        blocksData.push_back(segment); // �����ַ�����ӵ������

        start += length; // ������һ��Ƭ�ε���ʼλ��
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
        // ���ļ�ָ���ƶ���ָ����С
        rwCursor.seekp(diskSize - 1, std::ios::beg);

        // д��һ���ֽڣ���ȷ���ļ�ռ��ָ����С
        rwCursor.write("", 1);
        std::cout << "���̴����ɹ���" << std::endl;
    }
    else {
        std::cerr << "�޷��򿪴��̣�" << std::endl;
    }
}
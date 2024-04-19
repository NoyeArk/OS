#include "disk.h"


Disk::Disk() {
    std::string diskPath = R"(D:\\qtonline\\run\\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);
    CreateDisk();

    // ��λʾͼ����λ����1
    fileBitMap.set();
    swapBitMap.set();

    // ReadSingleBlockFromDisk(0);
}


Disk::~Disk() {
    // �ر��ļ�
    rwCursor.close();
}

std::vector<short> Disk::GetFreeBlockId()
{
    std::vector<short> freeLoc;
    if (fileBitMap.none()) {
        std::cout << "�޿��п�" << std::endl;
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
 * \name GetFreeBlocks       �Ӵ����ļ���������applyBlockNum�����п�
 * \type std::vector<short>  ����õ��Ŀ����ļ���ż���
 * \param applyBlockNum      ����Ŀ��п����
 */
std::vector<short> Disk::GetFreeBlocks(int applyBlockNum) {
    std::vector<short> freeLoc;
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

    DebugCout("�˴�д�������Ϊ");
}



void Disk::FlushBuffer() {
    this->WriteSingleBlockToDisk(0, 0, "");
}


void Disk::DebugCout(std::string info) {
    if (!Debug) return;
    std::cout << "Disk::" << info << std::endl;
}


/**
 * \name WriteSwap           �Ի�������д����
 * \param toWriteBlocksData  Ҫд������ݼ���
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
 * \name QueryFreeFileBlock  ��ѯ��ǰ�����ļ����Ŀ��п��
 * \return                   ���ص�ǰ���п�ż���
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
 * \name SetBitMap  ��λʾͼ����д����
 * \param blockId   д��λʾͼ�ж�Ӧ�Ŀ��
 * \param data      Ҫд�����Ϣ true-1 false-0
 */
void Disk::SetBitMap(int blockId, bool data) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);

    if (data) fileBitMap.set(blockId);
    else fileBitMap.reset(blockId);
}


/**
 * \name AllocDisk         �����µĴ����ļ������п�
 * \param lastIdxBlockNum  ��������ж�Ӧ�����һ���������
 * \param curFileLen       Ҫ��ŵ�Ŀ���ļ��ĳ���
 * \param applyBlockNum    ����Ŀ��п����
 * \return 
 */
std::vector<short> Disk::AllocDisk(const int& lastIdxBlockNum, int curFileLen, int applyBlockNum) {
    std::vector<short> newIdxBlocksId;
    auto dataBlocksId = this->GetFreeBlocks(applyBlockNum);
    if (dataBlocksId.empty()) {
        std::cout << "�����̿�����ʧ�ܣ�" << std::endl;
        return std::vector<short>();
    }
    // ����·���Ĵ��̿����������һ�������飨���20������ʣ��λ�ã���Ҫ�ٷ���һ���µ�������
    // ��μ������һ����������ʣ��λ�ã�(20 - �ļ����� % 20)
    int availablePosNum = IDX_BLOCK_IDX_NUM - curFileLen % IDX_BLOCK_IDX_NUM;
    if (applyBlockNum > availablePosNum) {
        // ��Ҫ�ٷ���applyIdxBlockNum������������
        int applyIdxBlockNum = (applyBlockNum - availablePosNum) / IDX_BLOCK_IDX_NUM + 1;
        newIdxBlocksId = this->GetFreeBlocks(applyIdxBlockNum);
    }
    else {
        // ����Ҫ�����µ�������
    }
    size_t curWriteIdx = 0;
    // ����ִ�е��⣬�������ǹ��ģ�������Ҫ���·�������ݿ��д��������
    if (availablePosNum >= applyBlockNum) {  // ���һ�����������㹻�˴η������ݿ��д��
        // �����һ����������д��applyBlockNum������
        int beginOffset = (curFileLen % IDX_BLOCK_IDX_NUM) * IDX_SIZE;
        this->UpdateIdxBlock(lastIdxBlockNum, beginOffset, dataBlocksId);
    }
    else {
        // ���һ�������������������������һ����������д��ʣ��availablePosNum������
        std::vector<short> subDataBlocksId(dataBlocksId.begin(), dataBlocksId.begin() + availablePosNum);
        int beginOffset = (curFileLen % IDX_BLOCK_IDX_NUM) * IDX_SIZE;
        this->UpdateIdxBlock(lastIdxBlockNum, beginOffset, subDataBlocksId);

        // ���µ���������д��ʣ�������
        std::vector<short> remainDataBlocksId(dataBlocksId.begin() + availablePosNum, dataBlocksId.end());
        for (size_t ii = 0; ii < newIdxBlocksId.size(); ii++) {
            std::vector<short> subDataBlocksId = {};
            if (remainDataBlocksId.size() <= IDX_BLOCK_IDX_NUM) {  // ����һ��д�������飬����Ҫ��ȡ
                subDataBlocksId.insert(subDataBlocksId.begin(), remainDataBlocksId.begin(), dataBlocksId.end());
            }
            else {  // ��ȡǰ20��������Ϊ�˴�Ҫд���������ͬʱ����ʣ���д�������
                subDataBlocksId.insert(subDataBlocksId.begin(), remainDataBlocksId.begin(), dataBlocksId.begin() + IDX_BLOCK_IDX_NUM);
                remainDataBlocksId.erase(remainDataBlocksId.begin(), remainDataBlocksId.begin() + IDX_BLOCK_IDX_NUM);
            }

            int idxBlockId = newIdxBlocksId[ii];
            int beginOffset = ii * IDX_SIZE;  // �ӿ���0��ַ��ʼ
            this->UpdateIdxBlock(idxBlockId, beginOffset, subDataBlocksId);
        }
    }
    return newIdxBlocksId;
}


/**
 * \name AllocFileBlock   ��������ʱΪ�����8���ļ����1��������
 * \return                ����Ϊ�������������
 */
std::vector<short> Disk::AllocFileBlock() {
    // ����һ���������8�����ݿ�
    auto newIdxBlocksId = this->GetFreeBlocks(1);
    auto dataBlocksId = this->GetFreeBlocks(8);

    // ʹ��QtConcurrent::run���к����������ݲ���
//    QtConcurrent::run([=]() {
//        // �����߳������еĴ���
//        // ��8�����ݿ�Ŀ��д����������

//    });

    for (size_t ii = 0; ii < dataBlocksId.size(); ii++) {
        int offset = IDX_SIZE * ii;
        int effectiveAddr = newIdxBlocksId.back() * BLOCK_SIZE + offset;
        this->WriteSingleBlockToDisk(effectiveAddr, IDX_SIZE, std::to_string(dataBlocksId[ii]).c_str());
//        std::vector<short> addrToWrite = { (short)effectiveAddr };

//        ProcessCommunication::ShakeSend("DISK", "WriteBlockId", addrToWrite, std::to_string(dataBlocksId[ii]).c_str());
//        std::string msgReceiveFromDisk = ProcessCommunication::ShakeReceive("DISK");
    }

    // �����������
    return newIdxBlocksId;
}


/**
 * \name FreeDisk            ɾ���ļ�ʱ�ͷ�����ռ�е��ļ����������
 * \param toFreeIdxBlocksId  Ҫ������ļ������������
 */
void Disk::FreeDisk(const std::vector<short>& toFreeIdxBlocksId) {
    std::lock_guard<std::mutex> lock(fileBitMapMutex);
    auto dataBlocksId = this->GetDataBlocksId(toFreeIdxBlocksId);
    // ����ļ�������
    for (const auto& blockId : dataBlocksId) {
        fileBitMap.reset(blockId);
        int effectiveAddr = blockId * BLOCK_SIZE;
        this->WriteSingleBlockToDisk(effectiveAddr, BLOCK_SIZE, "");
    }
    // �������������
    for (auto blockId : toFreeIdxBlocksId) {
        fileBitMap.reset(blockId);
        this->WriteSingleBlockToDisk(blockId * BLOCK_SIZE, BLOCK_SIZE, "");
    }
    this->FlushBuffer();
}


/**
* \param addr��    Ҫ������ʼ��
* \param blockNum��Ҫ���Ŀ����
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
 * \name WriteFile      д�����ļ���
 * \param idxBlocksNum  Ҫд���ļ������ڵ��������
 * \param dataToWrite   Ҫд�������
 */
void Disk::WriteFile(const std::vector<short>& idxBlocksNum,
                     const std::string& dataToWrite) {
    std::vector<short> dataBlocksId = this->GetDataBlocksId(idxBlocksNum);
    std::vector<std::string> blocksData;

    int dataSize = dataToWrite.size();
    int start = 0;

    while (start < dataSize) {
        int length = std::min(40, dataSize - start); // ÿ�ε���󳤶�Ϊ segmentSize
        std::string segment = dataToWrite.substr(start, length); // ��ȡ���ַ���
        if (length < 40) {
            segment.append(40 - length, '\0'); // ʹ�ÿ��ַ����
        }
        blocksData.push_back(segment); // �����ַ�����ӵ������
        start += length; // ������һ��Ƭ�ε���ʼλ��
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

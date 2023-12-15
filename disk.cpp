#include "disk.h"

void Disk::GetFreeLoc() {
    std::vector<int> freeLoc;
    // 900~1023������ģ��һ����������������
    for (size_t ii = 0; ii < AVAILABLE_NUM; ii++) {
        if (bitmap[ii] == 1) 
            freeLoc.push_back(ii);
    }
}


void Disk::Alloc() {

}


Disk::Disk() {
    std::string diskPath = R"(F:\\code\\os\\disk.data)";
    rwCursor.open(diskPath, std::ios::in | std::ios::out | std::ios::binary);

    // ��ʼ��λʾͼ
    for (size_t ii = 0; ii < bitmap.size(); ii++) {
        bitmap[ii] = 1;
    }
}

Disk::~Disk() {
    // �ر��ļ�
    rwCursor.close();
}


/**
* \param addr��    Ҫ������ʼ��
* \param blockNum��Ҫ���Ŀ����
*/
char* Disk::Read(const int& addr, const int& blockNum) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    // ���ö�ȡλ��
    rwCursor.seekg(addr * BLOCK_SIZE);

    // ��ȡblockNum * 40b������
    char* buff = new char[blockNum * BLOCK_SIZE];
    rwCursor.read(buff, blockNum * BLOCK_SIZE);

    //std::cout << "Read " << rwCursor.gcount() << " bytes from the file." << std::endl;
    std::cout.write(buff, rwCursor.gcount()) << std::endl;

    return buff;
}


void Disk::Write(const int& addr, const std::string& dataToWrite) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    rwCursor.seekg(addr * BLOCK_SIZE);
    rwCursor.write(dataToWrite.c_str(), dataToWrite.size());

    std::cout << "Write Successful!!!" << std::endl;
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
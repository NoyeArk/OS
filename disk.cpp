#include "disk.h"

void Disk::GetFreeLoc() {
    std::vector<int> freeLoc;
    // 900~1023块用来模拟兑换分区，不参与计算
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

    // 初始化位示图
    for (size_t ii = 0; ii < bitmap.size(); ii++) {
        bitmap[ii] = 1;
    }
}

Disk::~Disk() {
    // 关闭文件
    rwCursor.close();
}


/**
* \param addr：    要读的起始块
* \param blockNum：要读的块个数
*/
char* Disk::Read(const int& addr, const int& blockNum) {
    if (!rwCursor.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        exit(-1);
    }
    // 设置读取位置
    rwCursor.seekg(addr * BLOCK_SIZE);

    // 读取blockNum * 40b的内容
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
#include "disk.h"

void Disk::Read() {

}


void Disk::Write() {
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // 创建文件输出流对象，打开文件，如果文件不存在，则创建它。
    std::ofstream fout;

    fout.open(filename, std::ios::app | std::ios::binary);

    if (fout.is_open() == false) {
        std::cout << "打开文件" << filename << "失败。\n";  
        exit(-1);
    }

    // 向文件中写入数据。
    struct Data {              // 结构体。
        char name[31];         // 姓名。
        int    no;             // 编号。  
    } data;

    data = { "zq",3 };
    fout.write((const char*)&data, sizeof(Data));   // 写入第一块数据。

    data = { "qq",8 };
    fout.write((const char*)&data, sizeof(Data));     // 写入第二块数据。

    fout.close();        // 关闭文件，fout对象失效前会自动调用close()。

    std::cout << "操作文件完成。\n";
}


void Disk::Test() {
    // 指定文件名
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // 指定文件大小（40KB）
    std::size_t file_size = 40 * 1024;  // 1 KB = 1024 bytes

    // 创建文件流对象
    std::ofstream outfile(filename, std::ios::binary | std::ios::out);

    if (outfile.is_open()) {
        // 将文件指针移动到指定大小
        outfile.seekp(file_size - 1, std::ios::beg);

        // 写入一个字节，以确保文件占据指定大小
        outfile.write("", 1);

        // 关闭文件流
        outfile.close();

        std::cout << "文件创建成功：" << filename << std::endl;
    }
    else {
        std::cerr << "无法打开文件：" << filename << std::endl;
    }
}
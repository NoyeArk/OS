#include "disk.h"

void Disk::Read() {

}


void Disk::Write() {
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // 创建文件输出流对象，打开文件，如果文件不存在，则创建它。
    //ofstream fout(filename, ios::binary);
    //ofstream fout(filename, ios::out | ios::binary);
    //ofstream fout(filename, ios::trunc | ios::binary);
    //ofstream fout(filename, ios::app | ios::binary);
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
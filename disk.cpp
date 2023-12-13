#include "disk.h"

void Disk::Read() {

}


void Disk::Write() {
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // �����ļ���������󣬴��ļ�������ļ������ڣ��򴴽�����
    std::ofstream fout;

    fout.open(filename, std::ios::app | std::ios::binary);

    if (fout.is_open() == false) {
        std::cout << "���ļ�" << filename << "ʧ�ܡ�\n";  
        exit(-1);
    }

    // ���ļ���д�����ݡ�
    struct Data {              // �ṹ�塣
        char name[31];         // ������
        int    no;             // ��š�  
    } data;

    data = { "zq",3 };
    fout.write((const char*)&data, sizeof(Data));   // д���һ�����ݡ�

    data = { "qq",8 };
    fout.write((const char*)&data, sizeof(Data));     // д��ڶ������ݡ�

    fout.close();        // �ر��ļ���fout����ʧЧǰ���Զ�����close()��

    std::cout << "�����ļ���ɡ�\n";
}


void Disk::Test() {
    // ָ���ļ���
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // ָ���ļ���С��40KB��
    std::size_t file_size = 40 * 1024;  // 1 KB = 1024 bytes

    // �����ļ�������
    std::ofstream outfile(filename, std::ios::binary | std::ios::out);

    if (outfile.is_open()) {
        // ���ļ�ָ���ƶ���ָ����С
        outfile.seekp(file_size - 1, std::ios::beg);

        // д��һ���ֽڣ���ȷ���ļ�ռ��ָ����С
        outfile.write("", 1);

        // �ر��ļ���
        outfile.close();

        std::cout << "�ļ������ɹ���" << filename << std::endl;
    }
    else {
        std::cerr << "�޷����ļ���" << filename << std::endl;
    }
}
#include "disk.h"

void Disk::Read() {

}


void Disk::Write() {
    std::string filename = R"(D:\horiki\code\os\disk.data)";

    // �����ļ���������󣬴��ļ�������ļ������ڣ��򴴽�����
    //ofstream fout(filename, ios::binary);
    //ofstream fout(filename, ios::out | ios::binary);
    //ofstream fout(filename, ios::trunc | ios::binary);
    //ofstream fout(filename, ios::app | ios::binary);
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
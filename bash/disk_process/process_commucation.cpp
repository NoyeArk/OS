#include "process_commucation.h"


ProcessCommunication::ProcessCommunication() {

}


std::vector<std::string> ProcessCommunication::AnalysisMessage(std::string message) {
    // ʹ�ÿո���Ϊ�ָ��������ַ����ָ�ɶ�����ַ���
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;

    // ͨ��istringstreamѭ����ȡÿ�����ʣ�ʹ�ÿո�ָ
    while (iss >> token) {
        if (tokens.size() <= 2) {
            tokens.push_back(token);
        }
        else {

        }
    }

    return tokens;
}


std::vector<std::string> ProcessCommunication::Receive() {
    TCHAR szName[] = TEXT("main_to_disk_communication");
    HANDLE hMapFile;

    //������
    HANDLE main_to_disk_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_over_lock"));
    HANDLE main_to_disk_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_create_lock"));

    //�ȴ�д�����������׼��
    WaitForSingleObject(main_to_disk_create_lock, INFINITE);

    //do
    //{
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,                 // do not inherit the name
        szName);               // name of mapping object
    //}
    //while (hMapFile == NULL);
    //Sleep(500);

    //ӳ���ļ��ں˶��󵽽��̿ռ�
    char* pBuf = (char*)MapViewOfFile(hMapFile, // handle to map object
        FILE_MAP_ALL_ACCESS,  // read/write permission
        0,
        0,
        0);
    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(hMapFile);
    }


    std::string convertedMessage(pBuf);
    message = AnalysisMessage(convertedMessage);
    std::cout << "���յ�����Ϊ��" << message[1] << std::endl;

    //֪ͨ����������ݶ�ȡ
    HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_disk_event"));
    if (NULL != hNotifyHandle)
    {
        ::SetEvent(hNotifyHandle);
    }
    CloseHandle(hNotifyHandle);

    //Sleep(500);

    //��ʹ�����̽���ӳ�䣬�˽��̲ſ��Խ���ӳ��
    WaitForSingleObject(main_to_disk_over_lock, INFINITE);
    CloseHandle(main_to_disk_over_lock);

    //�ӽ��̿ռ����ͷű�ӳ����ļ�
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return message;
}

std::vector<std::string> ProcessCommunication::ShakeReceive()
{
    std::string data;
    // �ļ�·��
    std::string filePath = "C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt";

    // ���ļ�
    std::ifstream inputFile(filePath);

    if (inputFile.is_open()) {
        while (std::getline(inputFile, data)) {
            continue;
        }
        // �ر��ļ�
        inputFile.close();

        message = AnalysisMessage(data);
        std::cout << "���յ�����Ϊ��" << message[1] << std::endl;
    }
    else {
        std::cerr << "�޷����ļ�" << std::endl;
    }
    return std::vector<std::string>();
}


void ProcessCommunication::Send(const char* dataToSend) {
    //ӳ���������
    TCHAR szName[] = TEXT("disk_to_main_communication");

    //�����ļ�
    HANDLE hFile = CreateFile(
        _T("disk_to_main_communication.txt"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    //������
    HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_event"));


    //�����ļ�ӳ���ں˶���
    HANDLE hMapHandle = CreateFileMapping(
        hFile,    // ʹ�ô����ļ�
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        4096,                    // maximum object size (low-order DWORD)
        szName);                 // name of mapping object
    if (NULL == hMapHandle)
    {
        printf("create file map error...\n");
        return;
    }
    //ӳ�䵽���̿ռ䣬��Χ��0-�ļ�β��
    char* pBuf = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pBuf == NULL)
    {
        CloseHandle(hMapHandle);
        return;
    }

    //ӳ�䵽���̿ռ����ʼ��ַ
    printf("pBufVal = %x\n", pBuf);

    //��������
    const char* content = &dataToSend[0];

    //�����ڴ�ӳ���ļ�
    strncpy(pBuf, content, strlen(content) + 1);

    //���Ѷ�����
    HANDLE disk_to_main_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_create_lock"));
    if (NULL != disk_to_main_create_lock)
    {
        ::SetEvent(disk_to_main_create_lock);
    }
    CloseHandle(disk_to_main_create_lock);

    //�ȴ���һ�����̶�ȡ����
    WaitForSingleObject(hNotifyHandle, INFINITE);


    //�ͷű�ӳ��������ڴ�ռ�
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapHandle);

    //����̷��ͽ����ź�
    HANDLE disk_to_main_over_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_over_lock"));
    if (NULL != disk_to_main_over_lock)
    {
        ::SetEvent(disk_to_main_over_lock);
    }
    CloseHandle(disk_to_main_over_lock);

    return;
}

void ProcessCommunication::ShakeSend(const char* dataToSend)
{
    // ���ļ�
    std::ofstream outFile("C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt", std::ios::trunc);
    std::cout << "���͵����ݣ�" << dataToSend << std::endl;
    if (!outFile.is_open()) {
        std::cerr << "����ͨ��ʧ��" << std::endl;
    }
    outFile << dataToSend << std::endl;
    outFile.close();

    Sleep(1000);
}

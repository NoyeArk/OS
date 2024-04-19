#include "process_commucation.h"


ProcessCommunication::ProcessCommunication() {

}


std::vector<std::string> ProcessCommunication::AnalysisMessage(std::string message) {
    // 使用空格作为分隔符，将字符串分割成多个子字符串
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;

    // 通过istringstream循环读取每个单词（使用空格分割）
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

    //创建锁
    HANDLE main_to_disk_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_over_lock"));
    HANDLE main_to_disk_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_create_lock"));

    //等待写进程完成数据准备
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

    //映射文件内核对象到进程空间
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
    std::cout << "接收的数据为：" << message[1] << std::endl;

    //通知进程完成数据读取
    HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_disk_event"));
    if (NULL != hNotifyHandle)
    {
        ::SetEvent(hNotifyHandle);
    }
    CloseHandle(hNotifyHandle);

    //Sleep(500);

    //先使主进程结束映射，此进程才可以结束映射
    WaitForSingleObject(main_to_disk_over_lock, INFINITE);
    CloseHandle(main_to_disk_over_lock);

    //从进程空间中释放被映射的文件
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return message;
}

std::vector<std::string> ProcessCommunication::ShakeReceive()
{
    std::string data;
    // 文件路径
    std::string filePath = "C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt";

    // 打开文件
    std::ifstream inputFile(filePath);

    if (inputFile.is_open()) {
        while (std::getline(inputFile, data)) {
            continue;
        }
        // 关闭文件
        inputFile.close();

        message = AnalysisMessage(data);
        std::cout << "接收的数据为：" << message[1] << std::endl;
    }
    else {
        std::cerr << "无法打开文件" << std::endl;
    }
    return std::vector<std::string>();
}


void ProcessCommunication::Send(const char* dataToSend) {
    //映射对象名称
    TCHAR szName[] = TEXT("disk_to_main_communication");

    //创建文件
    HANDLE hFile = CreateFile(
        _T("disk_to_main_communication.txt"),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    //创建锁
    HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_event"));


    //创建文件映射内核对象
    HANDLE hMapHandle = CreateFileMapping(
        hFile,    // 使用磁盘文件
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
    //映射到进程空间，范围从0-文件尾巴
    char* pBuf = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pBuf == NULL)
    {
        CloseHandle(hMapHandle);
        return;
    }

    //映射到进程空间的起始地址
    printf("pBufVal = %x\n", pBuf);

    //传递内容
    const char* content = &dataToSend[0];

    //操作内存映射文件
    strncpy(pBuf, content, strlen(content) + 1);

    //唤醒读进程
    HANDLE disk_to_main_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_create_lock"));
    if (NULL != disk_to_main_create_lock)
    {
        ::SetEvent(disk_to_main_create_lock);
    }
    CloseHandle(disk_to_main_create_lock);

    //等待另一个进程读取数据
    WaitForSingleObject(hNotifyHandle, INFINITE);


    //释放被映射的虚拟内存空间
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapHandle);

    //向进程发送结束信号
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
    // 打开文件
    std::ofstream outFile("C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt", std::ios::trunc);
    std::cout << "发送的数据：" << dataToSend << std::endl;
    if (!outFile.is_open()) {
        std::cerr << "进程通信失败" << std::endl;
    }
    outFile << dataToSend << std::endl;
    outFile.close();

    Sleep(1000);
}

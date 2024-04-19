#include "my_thread.h"

PageTableItem::PageTableItem(const int& memBlockId, std::chrono::system_clock::time_point createTime) : memBlockId(memBlockId) {
    this->isInMem = true;
    this->isModified = false;

    // 将当前时间转换为整数
    auto currentTimeInt = std::chrono::duration_cast<std::chrono::seconds>(createTime.time_since_epoch()).count();
    this->createTime = static_cast<int>(currentTimeInt);
    this->lastUseTime = lastUseTime;
}


PCB::PCB() {
    this->pid = 0;
    this->openFiles = {};
    this->pageTable = {};
}


void PCB::AppendMmu(std::vector<int> allocMem) {
    if (allocMem.size()) {
        std::cout << "PCB::AppendMmu 内存不够" << std::endl;
        return;
    }
    for (size_t ii = 0; ii < allocMem.size(); ii++) {
        this->pageTable.push_back(PageTableItem(allocMem[ii], std::chrono::system_clock::now()));
    }
}


ProcessCommunication::ProcessCommunication() {

    this->CreateFileMap();
}


void ProcessCommunication::CreateFileMap() {

}


/**
 * \name MakeMessageToSend  将命令和数据打包成要发送的数据，封装起来
 * \type char *             要发送的数据包
 * \param command           命令
 * \param blocksNum         操作的块号
 * \param dataToWrite       要写入的数据
 * \return                  封装好的数据包
 */
std::string ProcessCommunication::MakeMessageToSend(const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite) {
    char* message = (char*)command.c_str();
    strcat(message, " ");
    for (const auto& blockId : blocksNum) {
        strcat(message, std::to_string(blockId).c_str());
        strcat(message, " ");
    }
    strcat(message, dataToWrite.c_str());
    return std::string(message);
}


std::vector<std::string> ProcessCommunication::AnalysisMessage(std::string message) {
    // 使用空格作为分隔符，将字符串分割成多个子字符串
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;

    // 通过istringstream循环读取每个单词（使用空格分割）
    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}


std::string ProcessCommunication::Receive(std::string type) {
    std::cout << "等待接受数据..." << std::endl;

    std::string dataReceive;
    char* pBuf;
    TCHAR szNameFromDisk[] = TEXT("disk_to_main_communication");
    TCHAR szNameFromMemory[] = TEXT("memory_to_main_communication");


    if (type == "DISK") {
        //创建锁
        HANDLE disk_to_main_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_over_lock"));
        HANDLE disk_to_main_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_create_lock"));
        //等待写进程创建并准备数据
        WaitForSingleObject(disk_to_main_create_lock, INFINITE);

        HANDLE hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szNameFromDisk);       // name of mapping object
        if (hMapFile == NULL) {
            _tprintf(TEXT("Could not open file mapping object (%d).\n"), ::GetLastError());
            exit(-1);
        }
        // 映射文件内核对象到进程空间
        pBuf = (char*)MapViewOfFile(
            hMapFile,             // handle to map object
            FILE_MAP_ALL_ACCESS,  // read/write permission
            0,
            0,
            0);
        CreateFileMap();
        if (pBuf == NULL) {
            _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
            CloseHandle(hMapFile);
            exit(-1);
        }

        //读取数据与处理数据
        std::cout << "收到的数据：" << pBuf << std::endl;
        dataReceive = std::string(pBuf);

        // 找到第一个空格的位置
        size_t spacePos = dataReceive.find(" ");

        // 如果找到了空格
        if (spacePos != std::string::npos) {
            dataReceive = dataReceive.substr(spacePos + 1);
        }
        else if (dataReceive != "Ready") {
            std::cout << "通信错误：收到的消息中没有Ready字段" << std::endl;
            dataReceive = "Error";
        }

        // 通知进程完成数据读取
        HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_event"));
        if (NULL != hNotifyHandle) {
            ::SetEvent(hNotifyHandle);
        }
        CloseHandle(hNotifyHandle);

        //先使主进程结束映射，此进程才可以结束映射
        WaitForSingleObject(disk_to_main_over_lock, INFINITE);


        // 从进程空间中释放被映射的文件
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }
    else if (type == "MEMORY") {
        //创建锁
        HANDLE memory_to_main_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("memory_to_main_over_lock"));
        HANDLE memory_to_main_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("memory_to_main_create_lock"));
        //等待写进程创建并准备数据
        WaitForSingleObject(memory_to_main_create_lock, INFINITE);
        HANDLE hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szNameFromMemory);     // name of mapping object
        if (hMapFile == NULL) {
            _tprintf(TEXT("Could not open file mapping object (%d).\n"), ::GetLastError());
            exit(-1);
        }
        // 映射文件内核对象到进程空间
        pBuf = (char*)MapViewOfFile(
            hMapFile,             // handle to map object
            FILE_MAP_ALL_ACCESS,  // read/write permission
            0,
            0,
            0);
        CreateFileMap();
        if (pBuf == NULL) {
            _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
            CloseHandle(hMapFile);
            exit(-1);
        }

        //读取数据与处理数据
        std::cout << "收到的数据：" << pBuf << std::endl;
        dataReceive = std::string(pBuf);

        // 找到第一个空格的位置
        size_t spacePos = dataReceive.find(" ");

        // 如果找到了空格
        if (spacePos != std::string::npos) {
            dataReceive = dataReceive.substr(spacePos + 1);
        }
        else if (dataReceive != "Ready") {
            std::cout << "通信错误：收到的消息中没有Ready字段" << std::endl;
            dataReceive = "Error";
        }

        HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("memory_to_main_event"));
        if (NULL != hNotifyHandle) {
            ::SetEvent(hNotifyHandle);
        }
        CloseHandle(hNotifyHandle);

        //先使主进程结束映射，此进程才可以结束映射
        WaitForSingleObject(memory_to_main_over_lock, INFINITE);
        CloseHandle(memory_to_main_over_lock);


        // 从进程空间中释放被映射的文件
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        CloseHandle(memory_to_main_create_lock);
    }

    return dataReceive;
}

std::string ProcessCommunication::ShakeReceive(std::string type)
{
    std::string dataReceive;
    // 文件路径
    std::string filePath = "C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt";

    // 打开文件
    std::ifstream inputFile(filePath);

    if (inputFile.is_open()) {
        while (std::getline(inputFile, dataReceive)) {
            continue;
        }
        // 关闭文件
        inputFile.close();

        // 找到第一个空格的位置
        size_t spacePos = dataReceive.find(" ");

        // 如果找到了空格
        if (spacePos != std::string::npos) {
            dataReceive = dataReceive.substr(spacePos + 1);
        }
        else if (dataReceive != "Ready") {
            std::cout << "通信错误：收到的消息中没有Ready字段" << std::endl;
            dataReceive = "Error";
        }
    }
    else {
        std::cerr << "无法打开文件" << std::endl;
    }
}


/**
 * \name Send          发送信息给其它进程
 * \param type         和外存进程通信还是和内存进程通信
 * \param command      Write or Read
 * \param blocksNum    读的块号
 * \param dataToWrite  若为Write，要写入的数据
 */
void ProcessCommunication::Send(
    std::string type,
    const std::string& command,
    const std::vector<short>& blocksNum,
    const std::string& dataToWrite)
{
    std::string messageToSend = MakeMessageToSend(command, blocksNum, dataToWrite);

    char* memMapFileBufAddr;
    TCHAR szNameToDisk[] = TEXT("main_to_disk_communication");
    TCHAR szNameToMemory[] = TEXT("main_to_memory_communication");

    if (type == "DISK") {
        HANDLE hFile = CreateFile(
            _T("main_to_disk_communication.txt"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // 创建文件映射内核对象
        HANDLE hMapHandle = CreateFileMapping(
            hFile,                   // 使用磁盘文件
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToDisk);           // name of mapping object

        //创建锁
        HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_event"));

        // 得到内存映射文件的起始地址
        memMapFileBufAddr = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

        // 操作内存映射文件，可以像操作内存一样
        strncpy(memMapFileBufAddr, messageToSend.c_str(), messageToSend.length() + 1);
        std::cout << "发送的数据：" << messageToSend << std::endl;

        //唤醒读线程
        HANDLE main_to_disk_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_disk_create_lock"));
        if (NULL != main_to_disk_create_lock)
        {
            ::SetEvent(main_to_disk_create_lock);
        }
        CloseHandle(main_to_disk_create_lock);

        // 等待另一个进程读取数据
        WaitForSingleObject(hNotifyHandle, INFINITE);

        // 释放被映射的虚拟内存空间
        UnmapViewOfFile(memMapFileBufAddr);
        CloseHandle(hMapHandle);

        //向进程发出结束信号
        HANDLE main_to_disk_over_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_disk_over_lock"));
        if (NULL != main_to_disk_over_lock)
        {
            ::SetEvent(main_to_disk_over_lock);
        }
        CloseHandle(main_to_disk_over_lock);
    }

    else if (type == "MEMORY")
    {
        HANDLE hFile = CreateFile(
            _T("main_to_memory_communication.txt"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // 创建文件映射内核对象
        HANDLE hMapHandle = CreateFileMapping(
            hFile,                   // 使用磁盘文件
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToMemory);         // name of mapping object

        //创建锁
        HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_memory_event"));

        // 得到内存映射文件的起始地址
        memMapFileBufAddr = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (memMapFileBufAddr == NULL) {
            CloseHandle(hMapHandle);
            exit(-1);
        }

        // 操作内存映射文件，可以像操作内存一样
        strncpy(memMapFileBufAddr, messageToSend.c_str(), messageToSend.length() + 1);
        std::cout << "发送的数据：" << messageToSend << std::endl;

            HANDLE main_to_memory_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_memory_create_lock"));
        if (NULL != main_to_memory_create_lock)
        {
            ::SetEvent(main_to_memory_create_lock);
        }
        CloseHandle(main_to_memory_create_lock);

        // 等待另一个进程读取数据
        WaitForSingleObject(hNotifyHandle, 0);
        CloseHandle(hNotifyHandle);
        // 释放被映射的虚拟内存空间
        UnmapViewOfFile(memMapFileBufAddr);
        CloseHandle(hMapHandle);


        HANDLE main_to_memory_over_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_memory_over_lock"));
        if (NULL != main_to_memory_over_lock)
        {
            ::SetEvent(main_to_memory_over_lock);
        }
        CloseHandle(main_to_memory_over_lock);
    }


    std::cout << "发送数据完成" << std::endl;
}

void ProcessCommunication::ShakeSend(std::string type, const std::string &command, const std::vector<short> &blocksNum, const std::string &dataToWrite)
{
    std::string messageToSend = MakeMessageToSend(command, blocksNum, dataToWrite);

    if (type == "DISK") {
        // 打开文件
        std::ofstream outFile("C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt", std::ios::trunc);
        std::cout << "发送的数据：" << messageToSend << std::endl;
        if (!outFile.is_open()) {
            std::cerr << "进程通信失败" << std::endl;
        }
        outFile << messageToSend << std::endl;
        outFile.close();
    }

    else if (type == "MEMORY")
    {
        // 打开文件
        std::ofstream outFile("D:\\qtonline\\run\\main_to_memory_communication.txt", std::ios::trunc);
        std::cout << "发送的数据：" << messageToSend << std::endl;
        if (!outFile.is_open()) {
            std::cerr << "进程通信失败" << std::endl;
        }
        outFile << messageToSend << std::endl;
        outFile.close();
    }

    std::cout << "发送数据完成" << std::endl;
    Sleep(1000);
}


template <typename T>
ThreadPool<T>::ThreadPool(int maxConns) : maxConnections(maxConns), usedConnections(0) {
    for (int i = 0; i < maxConns; i++) {
        T* conn;
        connectionQueue.push(conn);
    }
    //std::cout << "connectionPool init ok" << std::endl;
}


template <typename T>
ThreadPool<T>::~ThreadPool() {
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!connectionQueue.empty()) {
        T* connectionPtr = connectionQueue.front();
        connectionQueue.pop();
        delete connectionPtr;
    }
}


template <typename T>
T* ThreadPool<T>::AcquireConnection() {
    std::unique_lock<std::mutex> lock(queueMutex);
    while (connectionQueue.empty()) {
        cond.wait(lock);
    }

    T* conn = connectionQueue.front();
    connectionQueue.pop();
    usedConnections++;
    return conn;
}


template <typename T>
void ThreadPool<T>::ReleaseConnection(T* conn) {
    std::lock_guard<std::mutex> lock(queueMutex);
    connectionQueue.push(conn);
    usedConnections--;
    cond.notify_one();
}


template <typename T>
int ThreadPool<T>::GetUsedConnectionsCount() {
    std::lock_guard<std::mutex> lock(queueMutex);
    return usedConnections;
}


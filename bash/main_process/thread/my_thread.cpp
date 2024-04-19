#include "my_thread.h"

PageTableItem::PageTableItem(const int& memBlockId, std::chrono::system_clock::time_point createTime) : memBlockId(memBlockId) {
    this->isInMem = true;
    this->isModified = false;

    // ����ǰʱ��ת��Ϊ����
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
        std::cout << "PCB::AppendMmu �ڴ治��" << std::endl;
        return;
    }
    for (size_t ii = 0; ii < allocMem.size(); ii++) {
        this->pageTable.push_back(PageTableItem(allocMem[ii], std::chrono::system_clock::now()));
    }
}

// �����ⲿ���о�̬�����Ķ���ͳ�ʼ��
HANDLE ProcessCommunication::hFile = INVALID_HANDLE_VALUE;  // �ʵ��滻 INVALID_HANDLE_VALUE Ϊʵ��ֵ
HANDLE ProcessCommunication::hMapHandle = NULL;             // �ʵ��滻 NULL Ϊʵ��ֵ
HANDLE ProcessCommunication::hNotifyHandle = NULL;          // �ʵ��滻 NULL Ϊʵ��ֵ

ProcessCommunication::ProcessCommunication() {

    this->CreateFileMap();
}


void ProcessCommunication::CreateFileMap() {

}


/**
 * \name MakeMessageToSend  ����������ݴ����Ҫ���͵����ݣ���װ����
 * \type char *             Ҫ���͵����ݰ�
 * \param command           ����
 * \param blocksNum         �����Ŀ��
 * \param dataToWrite       Ҫд�������
 * \return                  ��װ�õ����ݰ�
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
    // ʹ�ÿո���Ϊ�ָ��������ַ����ָ�ɶ�����ַ���
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;

    // ͨ��istringstreamѭ����ȡÿ�����ʣ�ʹ�ÿո�ָ
    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}


std::string ProcessCommunication::Receive(std::string type) {
    std::cout << "�ȴ���������..." << std::endl;

    std::string dataReceive;
    char* pBuf;
    HANDLE hMapFile{};
    TCHAR szNameFromDisk[] = TEXT("disk_to_main_communication");
    TCHAR szNameFromMemory[] = TEXT("memory_to_main_communication");

    if (type == "DISK") {
        do {
            hMapFile = OpenFileMapping(
                FILE_MAP_ALL_ACCESS,   // read/write access
                FALSE,                 // do not inherit the name
                szNameFromDisk);       // name of mapping object
        } while (hMapFile == NULL);
        Sleep(500);
    }
    else if (type == "MEMORY") {
        do {
            hMapFile = OpenFileMapping(
                FILE_MAP_ALL_ACCESS,   // read/write access
                FALSE,                 // do not inherit the name
                szNameFromMemory);     // name of mapping object
        } while (hMapFile == NULL);
        Sleep(500);
    }

    if (hMapFile == NULL) {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"), ::GetLastError());
        exit(-1);
    }

    do {
        // ӳ���ļ��ں˶��󵽽��̿ռ�
        pBuf = (char*)MapViewOfFile(
            hMapFile,             // handle to map object
            FILE_MAP_ALL_ACCESS,  // read/write permission
            0,
            0,
            0);
    } while (pBuf == NULL);
    CreateFileMap();

    if (pBuf == NULL) {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(hMapFile);
        exit(-1);
    }

    // ֪ͨ����������ݶ�ȡ
    if (type == "DISK")
        hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_event"));
    else
        hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("memory_to_main_event"));
    Sleep(500);
    if (NULL != hNotifyHandle) {
        ::SetEvent(hNotifyHandle);
    }
    else {
        std::cout << "Error" << std::endl;
    }

    std::cout << "�յ������ݣ�" << pBuf << std::endl;
    dataReceive = std::string(pBuf);

    // �ҵ���һ���ո��λ��
    size_t spacePos = dataReceive.find(" ");

    // ����ҵ��˿ո�
    if (spacePos != std::string::npos) {
        dataReceive = dataReceive.substr(spacePos + 1);
    }
    else if (dataReceive != "Ready") {
        std::cout << "ͨ�Ŵ����յ�����Ϣ��û��Ready�ֶ�" << std::endl;
        dataReceive = "Error";
    }
    // �ӽ��̿ռ����ͷű�ӳ����ļ�
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return dataReceive;
}


/**
 * \name Send          ������Ϣ����������
 * \param type         ��������ͨ�Ż��Ǻ��ڴ����ͨ��
 * \param command      Write or Read
 * \param blocksNum    ���Ŀ��
 * \param dataToWrite  ��ΪWrite��Ҫд�������
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
        hFile = CreateFile(
            _T("main_to_disk_communication.txt"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // �����ļ�ӳ���ں˶���
        hMapHandle = CreateFileMapping(
            hFile,                   // ʹ�ô����ļ�
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToDisk);           // name of mapping object
    }

    else if (type == "MEMORY") {
        hFile = CreateFile(
            _T("main_to_memory_communication.txt"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // �����ļ�ӳ���ں˶���
        hMapHandle = CreateFileMapping(
            hFile,                   // ʹ�ô����ļ�
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToMemory);         // name of mapping object
    }

    if (NULL == hMapHandle) {
        printf("create file map error...\n");
        exit(-1);
    }

    // �õ��ڴ�ӳ���ļ�����ʼ��ַ
    memMapFileBufAddr = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (memMapFileBufAddr == NULL) {
        CloseHandle(hMapHandle);
        exit(-1);
    }

    if (type == "DISK") {
        hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_event"));
    }
    else if (type == "MEMORY") {
        hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_memory_event"));
    }

    // �����ڴ�ӳ���ļ�������������ڴ�һ��
    strncpy(memMapFileBufAddr, messageToSend.c_str(), messageToSend.length() + 1);

    std::cout << "���͵����ݣ�" << messageToSend << std::endl;

    // �ȴ���һ�����̶�ȡ����
    WaitForSingleObject(hNotifyHandle, INFINITE);

    // �ͷű�ӳ��������ڴ�ռ�
    UnmapViewOfFile(memMapFileBufAddr);
    CloseHandle(hMapHandle);

    std::cout << "�����������" << std::endl;
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


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
    TCHAR szNameFromDisk[] = TEXT("disk_to_main_communication");
    TCHAR szNameFromMemory[] = TEXT("memory_to_main_communication");


    if (type == "DISK") {
        //������
        HANDLE disk_to_main_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_over_lock"));
        HANDLE disk_to_main_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("disk_to_main_create_lock"));
        //�ȴ�д���̴�����׼������
        WaitForSingleObject(disk_to_main_create_lock, INFINITE);

        HANDLE hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szNameFromDisk);       // name of mapping object
        if (hMapFile == NULL) {
            _tprintf(TEXT("Could not open file mapping object (%d).\n"), ::GetLastError());
            exit(-1);
        }
        // ӳ���ļ��ں˶��󵽽��̿ռ�
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

        //��ȡ�����봦������
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

        // ֪ͨ����������ݶ�ȡ
        HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_event"));
        if (NULL != hNotifyHandle) {
            ::SetEvent(hNotifyHandle);
        }
        CloseHandle(hNotifyHandle);

        //��ʹ�����̽���ӳ�䣬�˽��̲ſ��Խ���ӳ��
        WaitForSingleObject(disk_to_main_over_lock, INFINITE);


        // �ӽ��̿ռ����ͷű�ӳ����ļ�
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }
    else if (type == "MEMORY") {
        //������
        HANDLE memory_to_main_over_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("memory_to_main_over_lock"));
        HANDLE memory_to_main_create_lock = ::CreateEvent(NULL, TRUE, FALSE, _T("memory_to_main_create_lock"));
        //�ȴ�д���̴�����׼������
        WaitForSingleObject(memory_to_main_create_lock, INFINITE);
        HANDLE hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szNameFromMemory);     // name of mapping object
        if (hMapFile == NULL) {
            _tprintf(TEXT("Could not open file mapping object (%d).\n"), ::GetLastError());
            exit(-1);
        }
        // ӳ���ļ��ں˶��󵽽��̿ռ�
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

        //��ȡ�����봦������
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

        HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("memory_to_main_event"));
        if (NULL != hNotifyHandle) {
            ::SetEvent(hNotifyHandle);
        }
        CloseHandle(hNotifyHandle);

        //��ʹ�����̽���ӳ�䣬�˽��̲ſ��Խ���ӳ��
        WaitForSingleObject(memory_to_main_over_lock, INFINITE);
        CloseHandle(memory_to_main_over_lock);


        // �ӽ��̿ռ����ͷű�ӳ����ļ�
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        CloseHandle(memory_to_main_create_lock);
    }

    return dataReceive;
}

std::string ProcessCommunication::ShakeReceive(std::string type)
{
    std::string dataReceive;
    // �ļ�·��
    std::string filePath = "C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt";

    // ���ļ�
    std::ifstream inputFile(filePath);

    if (inputFile.is_open()) {
        while (std::getline(inputFile, dataReceive)) {
            continue;
        }
        // �ر��ļ�
        inputFile.close();

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
    }
    else {
        std::cerr << "�޷����ļ�" << std::endl;
    }
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
        HANDLE hFile = CreateFile(
            _T("main_to_disk_communication.txt"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        // �����ļ�ӳ���ں˶���
        HANDLE hMapHandle = CreateFileMapping(
            hFile,                   // ʹ�ô����ļ�
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToDisk);           // name of mapping object

        //������
        HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_disk_event"));

        // �õ��ڴ�ӳ���ļ�����ʼ��ַ
        memMapFileBufAddr = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

        // �����ڴ�ӳ���ļ�������������ڴ�һ��
        strncpy(memMapFileBufAddr, messageToSend.c_str(), messageToSend.length() + 1);
        std::cout << "���͵����ݣ�" << messageToSend << std::endl;

        //���Ѷ��߳�
        HANDLE main_to_disk_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_disk_create_lock"));
        if (NULL != main_to_disk_create_lock)
        {
            ::SetEvent(main_to_disk_create_lock);
        }
        CloseHandle(main_to_disk_create_lock);

        // �ȴ���һ�����̶�ȡ����
        WaitForSingleObject(hNotifyHandle, INFINITE);

        // �ͷű�ӳ��������ڴ�ռ�
        UnmapViewOfFile(memMapFileBufAddr);
        CloseHandle(hMapHandle);

        //����̷��������ź�
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

        // �����ļ�ӳ���ں˶���
        HANDLE hMapHandle = CreateFileMapping(
            hFile,                   // ʹ�ô����ļ�
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            1048576,                    // maximum object size (low-order DWORD)
            szNameToMemory);         // name of mapping object

        //������
        HANDLE hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_memory_event"));

        // �õ��ڴ�ӳ���ļ�����ʼ��ַ
        memMapFileBufAddr = (char*)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (memMapFileBufAddr == NULL) {
            CloseHandle(hMapHandle);
            exit(-1);
        }

        // �����ڴ�ӳ���ļ�������������ڴ�һ��
        strncpy(memMapFileBufAddr, messageToSend.c_str(), messageToSend.length() + 1);
        std::cout << "���͵����ݣ�" << messageToSend << std::endl;

            HANDLE main_to_memory_create_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_memory_create_lock"));
        if (NULL != main_to_memory_create_lock)
        {
            ::SetEvent(main_to_memory_create_lock);
        }
        CloseHandle(main_to_memory_create_lock);

        // �ȴ���һ�����̶�ȡ����
        WaitForSingleObject(hNotifyHandle, 0);
        CloseHandle(hNotifyHandle);
        // �ͷű�ӳ��������ڴ�ռ�
        UnmapViewOfFile(memMapFileBufAddr);
        CloseHandle(hMapHandle);


        HANDLE main_to_memory_over_lock = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("main_to_memory_over_lock"));
        if (NULL != main_to_memory_over_lock)
        {
            ::SetEvent(main_to_memory_over_lock);
        }
        CloseHandle(main_to_memory_over_lock);
    }


    std::cout << "�����������" << std::endl;
}

void ProcessCommunication::ShakeSend(std::string type, const std::string &command, const std::vector<short> &blocksNum, const std::string &dataToWrite)
{
    std::string messageToSend = MakeMessageToSend(command, blocksNum, dataToWrite);

    if (type == "DISK") {
        // ���ļ�
        std::ofstream outFile("C:\\Users\\dell\\Desktop\\disk_process\\main_to_disk.txt", std::ios::trunc);
        std::cout << "���͵����ݣ�" << messageToSend << std::endl;
        if (!outFile.is_open()) {
            std::cerr << "����ͨ��ʧ��" << std::endl;
        }
        outFile << messageToSend << std::endl;
        outFile.close();
    }

    else if (type == "MEMORY")
    {
        // ���ļ�
        std::ofstream outFile("D:\\qtonline\\run\\main_to_memory_communication.txt", std::ios::trunc);
        std::cout << "���͵����ݣ�" << messageToSend << std::endl;
        if (!outFile.is_open()) {
            std::cerr << "����ͨ��ʧ��" << std::endl;
        }
        outFile << messageToSend << std::endl;
        outFile.close();
    }

    std::cout << "�����������" << std::endl;
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


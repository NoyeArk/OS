#include "my_thread.h"

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
		size_t virtualPageNumber = this->pageTable.size();
		this->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
	}
}


ProcessCommucation::ProcessCommucation() {
    this->CreateFileMap();
}


void ProcessCommucation::CreateFileMap() {
    
}


/**
 * \name MakeMessageToSend  ����������ݴ����Ҫ���͵����ݣ���װ����
 * \type char *             Ҫ���͵����ݰ�
 * \param command           ����
 * \param blocksNum         �����Ŀ��
 * \param dataToWrite       Ҫд�������
 * \return                  ��װ�õ����ݰ�
 */
char* ProcessCommucation::MakeMessageToSend(const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite) {
    char* message = (char*)command.c_str();
    strcat(message, " ");
    for (const auto& blockId : blocksNum) {
        strcat(message, std::to_string(blockId).c_str());
        strcat(message, " ");
    }
    strcat(message, dataToWrite.c_str());
    return message;
}


std::vector<std::string> ProcessCommucation::AnalysisMessage(std::string message) {
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


void ProcessCommucation::Receive(std::string& data) {
    std::cout << "�ȴ���������..." << std::endl;

    char* pBuf;
    HANDLE hMapFile;
    TCHAR szName[] = TEXT("disk_to_main_commucation");

    do {
        hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szName);               // name of mapping object
    } while (hMapFile == NULL);

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
    
    this->CreateFileMap();

    if (pBuf == NULL) {
        _tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
        CloseHandle(hMapFile);
        exit(-1);
    }

    // ֪ͨ����������ݶ�ȡ
    HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_event"));
    if (NULL != hNotifyHandle) {
        ::SetEvent(hNotifyHandle);
    }
    else {
        std::cout << "Error" << std::endl;
    }
    std::cout << "�յ������ݣ�" << pBuf << std::endl;
    data = std::string(pBuf);

    // �ҵ���һ���ո��λ��
    size_t spacePos = data.find(" ");

    // ����ҵ��˿ո�
    if (spacePos != std::string::npos) {
        data = data.substr(spacePos + 1);
    }
    else {
        // ���û���ҵ��ո���������ַ���
        std::cout << "ͨ�Ŵ����յ�����Ϣ��û��Ready�ֶ�" << std::endl;
        data = "Error";
    }

    // �ӽ��̿ռ����ͷű�ӳ����ļ�
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}


/**
 * \name Send          ������Ϣ����������
 * \param type         ��������ͨ�Ż��Ǻ��ڴ����ͨ��
 * \param command      Write or Read
 * \param blocksNum    ���Ŀ��
 * \param dataToWrite  ��ΪWrite��Ҫд�������
 */
void ProcessCommucation::Send(
    std::string type,
    const std::string& command,
    const std::vector<short>& blocksNum,
    const std::string& dataToWrite) 
{
    char* messageToSend = this->MakeMessageToSend(command, blocksNum, dataToWrite);

    TCHAR szNameToDisk[] = TEXT("main_to_disk_commucation");
    TCHAR szNameToMemory[] = TEXT("main_to_memory_commucation");

    if (type == "DISK") {
        hFile = CreateFile(
            _T("main_to_disk_commucation.txt"),
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
            1024,                    // maximum object size (low-order DWORD)
            szNameToDisk);           // name of mapping object
    }

    else if (type == "MEMORY") {
        hFile = CreateFile(
            _T("main_to_memory_commucation.txt"),
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
            1024,                    // maximum object size (low-order DWORD)
            szNameToMemory);           // name of mapping object
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
        hNotifyHandle = ::CreateEvent(NULL, TRUE, FALSE, _T("main_to_momery_event"));
    }

    // �����ڴ�ӳ���ļ�������������ڴ�һ��
    strncpy(memMapFileBufAddr, messageToSend, strlen(messageToSend) + 1);

    std::cout << "���͵����ݣ�" << messageToSend << std::endl;

    // �ȴ���һ�����̶�ȡ����
    WaitForSingleObject(hNotifyHandle, INFINITE);

    // �ͷű�ӳ��������ڴ�ռ�
    UnmapViewOfFile(memMapFileBufAddr);
    CloseHandle(hMapHandle);
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
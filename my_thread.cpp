#include "my_thread.h"

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
 * \name MakeMessageToSend  将命令和数据打包成要发送的数据，封装起来
 * \type char *             要发送的数据包
 * \param command           命令
 * \param blocksNum         操作的块号
 * \param dataToWrite       要写入的数据
 * \return                  封装好的数据包
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


void ProcessCommucation::Receive(std::string& data) {
    std::cout << "等待接受数据..." << std::endl;

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
        // 映射文件内核对象到进程空间
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

    // 通知进程完成数据读取
    HANDLE hNotifyHandle = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("disk_to_main_event"));
    if (NULL != hNotifyHandle) {
        ::SetEvent(hNotifyHandle);
    }
    else {
        std::cout << "Error" << std::endl;
    }
    std::cout << "收到的数据：" << pBuf << std::endl;
    data = std::string(pBuf);

    // 找到第一个空格的位置
    size_t spacePos = data.find(" ");

    // 如果找到了空格
    if (spacePos != std::string::npos) {
        data = data.substr(spacePos + 1);
    }
    else {
        // 如果没有找到空格，输出整个字符串
        std::cout << "通信错误：收到的消息中没有Ready字段" << std::endl;
        data = "Error";
    }

    // 从进程空间中释放被映射的文件
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}


/**
 * \name Send          发送信息给其它进程
 * \param type         和外存进程通信还是和内存进程通信
 * \param command      Write or Read
 * \param blocksNum    读的块号
 * \param dataToWrite  若为Write，要写入的数据
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

        // 创建文件映射内核对象
        hMapHandle = CreateFileMapping(
            hFile,                   // 使用磁盘文件
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

        // 创建文件映射内核对象
        hMapHandle = CreateFileMapping(
            hFile,                   // 使用磁盘文件
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

    // 得到内存映射文件的起始地址
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

    // 操作内存映射文件，可以像操作内存一样
    strncpy(memMapFileBufAddr, messageToSend, strlen(messageToSend) + 1);

    std::cout << "发送的数据：" << messageToSend << std::endl;

    // 等待另一个进程读取数据
    WaitForSingleObject(hNotifyHandle, INFINITE);

    // 释放被映射的虚拟内存空间
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
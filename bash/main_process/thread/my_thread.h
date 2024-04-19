#ifndef MY_THREAD_H
#define MY_THREAD_H

#define _CRT_SECURE_NO_WARNINGS

/*********************************************************************
 * \file   my_thread.h
 * \brief  创建内核线程池
 *
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <chrono>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <tchar.h>
#include <sstream>
#include <windows.h>

#include "../directory/directory.h"

struct PageTableItem {
	bool isInMem;     // 状态位：该页是否在内存中，可能被换到对换区
	int memBlockId;   // 对应内存块号
	bool isModified;  // 修改位：是否被修改
	int createTime;   // 数据块进入内存时间
	int lastUseTime;  // 最后一次使用时间，供LRU算法使用

	PageTableItem(const int& memBlockId, std::chrono::system_clock::time_point createTime);
};


struct PCB {
	int pid;					  // 进程标识符
	std::vector<FCB*> openFiles;  // 当前进程打开的文件

	// 进程页表 结构：用户页-实际内存块
	std::vector<PageTableItem> pageTable;

	PCB();

	// 添加新的内存块
	void AppendMmu(std::vector<int> allocMem);
};


class ProcessCommunication
{
private:
	static HANDLE hFile;
	static HANDLE hMapHandle;
	static HANDLE hNotifyHandle;
	std::vector<std::string> message;

	static void CreateFileMap();
	static std::string MakeMessageToSend(const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite = "");
	std::vector<std::string> AnalysisMessage(std::string rawMessage);

public:
	ProcessCommunication();

	static std::string Receive(std::string type);
	static void Send(std::string type, const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite = "");
};


template <typename T>
class ThreadPool
{
private:
	std::mutex queueMutex;
	std::condition_variable cond;
	std::queue<T*> connectionQueue;
	int maxConnections;
	int usedConnections;

public:
	ThreadPool(int maxConns = 10);
	~ThreadPool();

	T* AcquireConnection();
	void ReleaseConnection(T* conn);
	int GetUsedConnectionsCount();
};

#endif
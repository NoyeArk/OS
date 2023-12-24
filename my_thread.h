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

#include "directory.h"


struct PCB {
	int pid;					  // 进程标识符
	std::vector<FCB*> openFiles;  // 当前进程打开的文件
	// 进程页表 结构：用户页-实际内存块
	std::vector<std::pair<int, int>> pageTable;  

	PCB();
	// 添加新的内存块
	void AppendMmu(std::vector<int> allocMem);
};


class ProcessCommucation
{
private:
	HANDLE hFile;
	HANDLE hMapHandle;
	HANDLE hNotifyHandle;
	char* memMapFileBufAddr;
	std::vector<std::string> message;

	void CreateFileMap();
	char* MakeMessageToSend(const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite = "");
	std::vector<std::string> AnalysisMessage(std::string rawMessage);

public:
	ProcessCommucation();

	void Receive(std::string& data);
	void Send(std::string type, const std::string& command, const std::vector<short>& blocksNum, const std::string& dataToWrite = "");
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
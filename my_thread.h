#ifndef MY_THREAD_H
#define MY_THREAD_H

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

#include "directory.h"

struct PCB {
	int pid;					         // 进程标识符
	std::vector<std::string> openFiles;  // 当前进程打开的文件
	// 进程页表 结构：用户页-实际内存块
	std::vector<std::pair<int, int>> pageTable;  

	PCB() {
		this->pid = 0;
		this->openFiles = {};
		this->pageTable = {};
	}

	// 添加新的内存块
	void AppendMmu(std::vector<int> allocMem) {
		if (allocMem.size()) {
			std::cout << "PCB::AppendMmu 内存不够" << std::endl;
			return;
		}
		for (size_t ii = 0; ii < allocMem.size(); ii++) {
			int virtualPageNumber = this->pageTable.size();
			this->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
		}
	}
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

	T* acquireConnection();
	void releaseConnection(T* conn);
	int getUsedConnectionsCount();
};

#endif
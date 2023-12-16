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
	int pid;					 // 进程标识符
	std::vector<FCB> openFiles;  // 当前进程打开的文件
	std::vector<std::pair<int, int>> pageTable;  // 进程页表 结构：用户页-实际内存块

	PCB() {
		this->pid = 0;
		this->openFiles = {};
		this->pageTable = {};
	}

	// 添加新的内存块
	void AppendMmu(int beginPhysicalBlockNumber, int blockNum) {
		for (size_t ii = 0; ii < blockNum; ii++) {
			int virtualPageNumber = this->pageTable.size();
			this->pageTable.push_back(std::make_pair(virtualPageNumber, beginPhysicalBlockNumber + ii));
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
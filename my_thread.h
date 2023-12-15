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

#include "directory.h"

struct PCB {
	int pid;					 // 进程标识符
	std::vector<int> mmu;		 // 内存管理单元
	std::vector<FCB> openFiles;  // 当前进程打开的文件
	// ...

	PCB() {
		this->pid = 0;
		this->mmu = {};
		this->openFiles = {};
	}

	// 添加新的内存块
	void appendMmu(int beginAddr, int blockNum) {
		for (size_t ii = 0; ii < blockNum; ii++) {
			this->mmu.push_back(beginAddr + ii);
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
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

struct PCB {
	int pid;               // 进程标识符
	std::vector<int> mmu;  // 内存管理单元
	// ...
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
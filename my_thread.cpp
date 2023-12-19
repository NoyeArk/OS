#include "my_thread.h"

PCB::PCB() {
	this->pid = 0;
	this->openFiles = {};
	this->pageTable = {};
}

void PCB::AppendMmu(std::vector<int> allocMem) {
	if (allocMem.size()) {
		std::cout << "PCB::AppendMmu ÄÚ´æ²»¹»" << std::endl;
		return;
	}
	for (size_t ii = 0; ii < allocMem.size(); ii++) {
		int virtualPageNumber = this->pageTable.size();
		this->pageTable.push_back(std::make_pair(virtualPageNumber, allocMem[ii]));
	}
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
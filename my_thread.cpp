#include "my_thread.h"

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
T* ThreadPool<T>::acquireConnection() {
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
void ThreadPool<T>::releaseConnection(T* conn) {
	std::lock_guard<std::mutex> lock(queueMutex);
	connectionQueue.push(conn);
	usedConnections--;
	cond.notify_one();
}


template <typename T>
int ThreadPool<T>::getUsedConnectionsCount() {
	std::lock_guard<std::mutex> lock(queueMutex);
	return usedConnections;
}
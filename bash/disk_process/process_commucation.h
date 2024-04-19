#ifndef PROCESS_COMMUCATION_H
#define PROCESS_COMMUCATION_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <tchar.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>

class ProcessCommunication
{
private:
	std::vector<std::string> message;

	std::vector<std::string> AnalysisMessage(std::string rawMessage);

public:
	ProcessCommunication();

	std::vector<std::string> Receive();
	std::vector<std::string> ShakeReceive();
	void Send(const char* dataToSend);
	void ShakeSend(const char* dataToSend);
};

#endif
#ifndef CMD_H
#define CMD_H

/*********************************************************************
 * \file   cmd.h
 * \brief  ���û������ָ����н������������ϵͳ����
 * 
 * \author horiki
 * \date   December 2023
 *********************************************************************/

#include <iostream>
#include <unordered_map>
#include <string>

#include "kernel.h"
#include "my_thread.h"


class Cmd
{
private:
	bool isExit;
	COMMAND command;		// ��ǰָ��
	//ThreadPool<Kernel> kernelThread;
	Kernel kernel;
	std::string userInput;  

private:
	inline void GetUserInput();
	void AnalysisCommand();
	void ExecuteCommand();
	inline void Exit();

	void Help();
public:
	void Run();
};

#endif
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
	Kernel kernel;
	COMMAND command;		// ��ǰָ��
	std::string userInput;  

private:
	inline void Exit();
	void ExecuteCommand();
	void AnalysisCommand();
	inline void GetUserInput();

	void Help();

public:
	Cmd();
	void Run();
};

#endif
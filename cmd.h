#ifndef CMD_H
#define CMD_H

/*********************************************************************
 * \file   cmd.h
 * \brief  对用户输入的指令进行解析，进行相关系统调用
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
	COMMAND command;		// 当前指令
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
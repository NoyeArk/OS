#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <unordered_map>
#include "kernel.h"

/*****************************************************************//**
 * \file   cmd.h
 * \brief  对用户输入的指令进行解析，进行相关系统调用
 * 
 * \author horiki
 * \date   December 2023
 *********************************************************************/



class Cmd
{
private:
	bool isExit;
	COMMAND command;		// 当前指令
	Kernel kernel;
	std::string userInput;  

	inline void GetUserInput();
	void AnalysisCommand();
	void ExecuteCommand();
	inline void Exit();

	void Help();
public:
	void Run();
};

#endif
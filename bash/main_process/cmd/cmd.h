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

#include "../kernel/kernel.h"
#include "../thread/my_thread.h"
#include "../compiler/include/compiler.h"


class Cmd
{
private:
	bool isExit;
	bool isCommandSucess;
	std::string userInput;  

	Kernel kernel;
	Command command;		// 当前指令
	Compiler compiler;

private:
	inline void Exit();
	void AnalysisCommand();
	void ExecuteCommand();
	void Feedback();

	inline void GetUserInput();

	void Help();

public:
	Cmd();
	void Run();
};

#endif
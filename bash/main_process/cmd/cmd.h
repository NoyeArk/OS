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
	Command command;		// ��ǰָ��
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
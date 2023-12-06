#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <unordered_map>
#include "syscall.h"

/*****************************************************************//**
 * \file   cmd.h
 * \brief  ���û������ָ����н���
 * 
 * \author horiki
 * \date   December 2023
 *********************************************************************/



class Cmd
{
private:
	bool isExit;
	COMMAND command;		// ��ǰָ��
	Syscall kernel;
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
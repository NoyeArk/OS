#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <unordered_map>
#include "kernel.h"

/*****************************************************************//**
 * \file   cmd.h
 * \brief  ���û������ָ����н������������ϵͳ����
 * 
 * \author horiki
 * \date   December 2023
 *********************************************************************/



class Cmd
{
private:
	bool isExit;
	COMMAND command;		// ��ǰָ��
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
#include "cmd.h"

std::unordered_map<std::string, COMMAND> commandMap = {
		{"format",	 FILE_FORMAT},
		{"makdir",	 FILE_MKDIR},
		{"rmdir",	 FILE_RMDIR},
		{"ls",	     FILE_LS},
		{"cd",       FILE_CD},
		{"create",   FILE_CREATE},
		{"open",	 FILE_OPEN},
		{"close",	 FILE_CLOSE},
		{"write",	 FILE_WRITE},
		{"read",	 FILE_READ},
		{"rm",	     FILE_RM},
		{"back",     FILE_BACK},
				     
		{"cls",      CMD_CLS},
		{"help",     CMD_HELP},
		{"exit",     CMD_EXIT}
};

inline void Cmd::GetUserInput() {
	std::cout << kernel.getCurPath() << ">";
	std::getline(std::cin, userInput);
	//std::cin >> userInput;
}

void Cmd::AnalysisCommand() {
	std::string op = userInput.substr(0, userInput.find(" "));

	auto it = commandMap.find(op);
	if (it != commandMap.end()) {
		command = it->second;
	}
	else {  // 非法输入
		std::cout << "‘" << op << "’不是内部或外部命令，也不是可运行的程序或批处理文件。" << std::endl << std::endl;
	}
}

void Cmd::ExecuteCommand() {
	switch (command)
	{
	// Cmd指令
	case CMD_CLS: 
		std::system("cls");
		break;
	case CMD_HELP: Help();
		break;
	case CMD_EXIT: Exit();
		break;
	default:
		kernel.SysCall(command, userInput);
		break;
	}
}

inline void Cmd::Exit() {
	isExit = true;
}

void Cmd::Help() {
	std::cout << "文件系统操作相关指令" << std::endl;
	std::cout << "	· format" << std::endl;
	std::cout << "	· mkdir" << std::endl;
	std::cout << "	· rmdir" << std::endl;
	std::cout << "	· ls" << std::endl;
	std::cout << "	· cd" << std::endl;
	std::cout << "	· create" << std::endl;
	std::cout << "	· open" << std::endl;
	std::cout << "	· close" << std::endl;
	std::cout << "	· write" << std::endl;
	std::cout << "	· read" << std::endl;
	std::cout << "	· rm" << std::endl;
	std::cout << "	· exit" << std::endl;
}

void Cmd::Run() {
	isExit = false;
	while (!isExit) {
		GetUserInput();
		AnalysisCommand();
		ExecuteCommand();
	}
}
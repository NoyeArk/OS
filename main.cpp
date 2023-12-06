﻿#include "cmd.h"

enum Command {
    LS,
    CD,
    CREATE,
    HELP
    // 添加其他命令枚举值
};

void test() {
    std::unordered_map<std::string, Command> commandMap = {
        {"ls", LS},
        {"cd", CD},
        {"create", CREATE},
        {"help", HELP}
        // 添加其他命令映射
    };

    std::string userInput;
    std::cout << "Enter command: ";
    std::cin >> userInput;

    auto it = commandMap.find(userInput);
    if (it != commandMap.end()) {
        Command command = it->second;
        // 执行相应的操作，例如设置命令或调用相应的函数
        std::cout << "Command: " << command << std::endl;
    }
    else {
        std::cout << "Invalid command." << std::endl;
    }
}

int main()
{
	Cmd cmd;
	cmd.Run();
    //test();
	return 0;
}
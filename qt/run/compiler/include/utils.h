#pragma once
/*
* 该文件用来实现一些词法分析程序中的工具函数
*/

#include "define.h"

extern const std::string blankTofind;
extern const std::vector<std::string> characterToFind;
extern const char numberToFind[];

// 检查当前字符串token中是否含有如下的特殊字符
bool checkSpecialChar(const std::string& token);
int isNumber(const std::string& str);
std::string removeBlank(const std::string& input);
std::string transName(std::string& token, bool strcon = false);
bool checkBeginNum(std::string str);
void printResult();


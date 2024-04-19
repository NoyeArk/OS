#pragma once
/*
* ���ļ�����ʵ��һЩ�ʷ����������еĹ��ߺ���
*/

#include "define.h"

extern const std::string blankTofind;
extern const std::vector<std::string> characterToFind;
extern const char numberToFind[];

// ��鵱ǰ�ַ���token���Ƿ������µ������ַ�
bool checkSpecialChar(const std::string& token);
int isNumber(const std::string& str);
std::string removeBlank(const std::string& input);
std::string transName(std::string& token, bool strcon = false);
bool checkBeginNum(std::string str);
void printResult();


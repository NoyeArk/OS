#pragma once

#define _CRT_SECURE_NO_WARNINGS


#define FIRST 1
#define SECOND 2

#define FORMAT 0    // ��""֮�������

#define DIVDIV 1    // ע��\/\/
#define LEDIVXING 2 // \/*
#define RIDIVXING 3 // *\/
#define NONEDIV 4   // ���в���ע������

#define LDIVDIV 5  // ���� \/\/��\/*
#define RDIVDIV 6  // ���� \/\/��*\/
#define DIVXING 7  // ���� \/*��*\/
#define DIVDIVXING 8  // ���� \/\/��\/*��*\/

#define TEXT 3      // ��������

#include <algorithm>
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <cassert>
#include <cerrno>
#include <string.h>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>
#include <memory.h>

static bool LexOut = false;
static bool ParserOut = true;
static bool ErrorOut = false;
static const bool CodeOut = false;

static const std::string blankTofind = " ";
static const std::vector<std::string> characterToFind = {
"[", "{", "(", ")", "}", "]", ",", ";", "+", "-", "*", "/", "%", "!=", "==", ">=", "<=", "!", "=" };
static const char numberToFind[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

static std::ofstream outputFile;

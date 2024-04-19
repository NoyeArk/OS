#pragma once

#include "define.h"
#include "utils.h"


inline int maxNumIdx(const std::string& str);
std::vector<std::string> stepSlice(const std::vector<std::string>& str);
bool splitQuotedText(const std::string& lineInput, std::string& preFormatLineInput, std::string& FormatLineInput, std::string& afterFormatLineInput);
std::vector<std::string> splitEveryToken(const std::string& str);
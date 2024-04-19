#include "../include/ast.h"

void flushCodeBuffer(int _true, int _false, int ifNum) {
	if (!CodeOut) return;
	if (CodeOutFile.is_open()) {
		// 当前栈长度为几，就给几赋值
		int ifCount = 0;
		for (size_t ii = 0; ii < CodeQueue.size(); ii++) {
			size_t found = CodeQueue[ii].find("if");
			if (found != std::string::npos) {
				if (++ifCount == ifNum) {  // 进行赋值
					CodeQueue[ii] += std::to_string(_true);
					CodeQueue[ii + 1] += std::to_string(_false);
				}
			}
		}
		if (ifNum == 1) {
			while (!CodeQueue.empty()) {
				CodeOutFile << CodeQueue.front() << std::endl;
				CodeQueue.erase(CodeQueue.begin());
			}
		}
	}
}

// 根据传入的类型来判断输出三地址码的格式
void gen(const bool& isOut,
	const int& codeType, 
	const int& op1, 
	const std::string& opType, 
	const int& op2, 
	const std::string& tar) {

	if (!CodeOut || !isOut) return;
	if (CodeOutFile.is_open()) {
		switch (codeType)
		{
		case EXP: {
			if (CodeQueue.empty())
				CodeOutFile << ++LineNum << "=" << op1 << opType << op2 << std::endl;
			else {  // 还有未输出的内容
				std::string code = std::to_string(++LineNum) + ":t" + std::to_string(++TempNum) + "=" + std::to_string(op1) + opType + std::to_string(op2);
				CodeQueue.push_back(code);
			}
			break;
		}
		case ASSIGN: {
			if (CodeQueue.empty())
				CodeOutFile << ++LineNum << ":" << opType << "=" << op1 << std::endl; 
			else {
				std::string code = std::to_string(++LineNum) + ":" + opType + "=" + std::to_string(op1);
				CodeQueue.push_back(code);
			}
			break;
		}
		case GOTO: {
			if(CodeQueue.empty()) 
				CodeOutFile << ++LineNum << ":goto " << op1 << std::endl; 
			else {  // 还有未输出的内容
				std::string code = std::to_string(++LineNum) + ":goto " + std::to_string(codeType);
				CodeQueue.push_back(code);
			}
			break;
		}
		case BUFFER: {  // 一次性输出全部的内容
			// 当前栈长度为几，就给几赋值
			int ifCount = 0;
			for (size_t ii = 0; ii < CodeQueue.size(); ii++) {
				size_t found = CodeQueue[ii].find("if");
				if (found != std::string::npos) {
					if (++ifCount == IfNum) {  // 进行赋值
						CodeQueue[ii] += std::to_string(op1);
						CodeQueue[ii+1] += std::to_string(op2);
					}
				}
			}
			if (IfNum == 1) {
				while (!CodeQueue.empty()) {
					CodeOutFile << CodeQueue.front() << std::endl;
					CodeQueue.erase(CodeQueue.begin());
				}
			}
			break;
		}
		default:
			break;
		}
	}
	else {
		std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
	}
}

// 根据传入的类型来判断输出三地址码的格式
void gen(const bool& isOut,
	const int& codeType, const std::string& op1, 
	const std::string& opType, const std::string& op2,
	const std::string& tar) {
	if (!CodeOut || !isOut) return;
	if (CodeOutFile.is_open()) {
		switch (codeType)
		{
		case IF: { 
			std::string code = std::to_string(++LineNum) + ":if " + op1  + opType + op2 + " goto ";
			CodeQueue.push_back(code);
			code = std::to_string(++LineNum) + ":goto ";
	 		CodeQueue.push_back(code);
			break;
		}
		case EXP: {
			if (CodeQueue.empty())
				if (op1 != "UnaryOp")
					CodeOutFile << ++LineNum << ":" << tar << "=" << op1 << opType << op2 << std::endl;
				else  // t1 = -a
					CodeOutFile << ++LineNum << ":" << tar << "=" << opType << op2 << std::endl;
			else {  // 还有未输出的内容
				std::string code = std::to_string(++LineNum) + ":t" + std::to_string(++TempNum) + "=" + op1 + opType + op2;
				CodeQueue.push_back(code);
			}
			break;
		}
		case ASSIGN: {
			if (CodeQueue.empty())
				CodeOutFile << ++LineNum << ":" << opType << "=" << op1 << std::endl;
			else {
				std::string code = std::to_string(++LineNum) + ":" + opType + "=" + op1;
				CodeQueue.push_back(code);
			}
			break;
		}
		default:
			break;
		}
	}
	else {
		std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
	}
}

int label() {
	return LineNum+1;
}

void setLineNum(const int& num) {
	LineNum = num;
}

// 每次只给最近的一个goto赋值
void rrGoto(const int& line1, const int& line2) {
	if (line2 == -1) {  // fill goto
		for (size_t ii = CodeQueue.size() - 1; ii >= 0; ii--) {
			size_t found = CodeQueue[ii].find("goto ");
			if (found != std::string::npos && CodeQueue[ii].size() == 7) {
				CodeQueue[ii] += std::to_string(line1);
				break;
			}
		}
	}
	else {  // fill if goto
		for (size_t ii = CodeQueue.size() - 1; ii >= 0; ii--) {
			if (CodeQueue[ii].find("if") != std::string::npos) {
				CodeQueue[ii] += std::to_string(line1);
				CodeQueue[ii+1] += std::to_string(line2);
				break;
			}
		}
	}
}


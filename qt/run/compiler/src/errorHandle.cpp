
#include "../include/errorHandle.h"


inline void outFileErrorResult(const std::string& type) {
	if (ErrorOutFile.is_open() && ErrorOut) {
		ErrorOutFile << globalLineNumber << " " << type << std::endl;
	}
	else {
		std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
	}
}

void errorHandle(const emErrorType& errorType, const int& lineNum) {
	globalLineNumber = lineNum;
	switch (errorType)
	{
	case A: missSemicn_A(); break;
	case B: missSemicn_B(); break;
	case C: missSemicn_C(); break;
	case D: missSemicn_D(); break;
	case E: missSemicn_E(); break;
	case F: missSemicn_F(); break;
	case G: missSemicn_G(); break;
	case H: missSemicn_H(); break;
	case I: missSemicn_I(); break;
	case J: missSemicn_J(); break;
	case K: missSemicn_K(); break;
	case L: missSemicn_L(); break;
	case M: missSemicn_M(); break;
	default: break;
	}
}

// a-格式字符串中出现非法字符
void missSemicn_A() {
	outFileErrorResult("a");
}
// b-函数名或变量名在当前作用域下重复定义
void missSemicn_B() {
	outFileErrorResult("b");
}
// c-使用未定义的标识符
void missSemicn_C() {
	outFileErrorResult("c");
}
// d-函数参数个数不匹配
void missSemicn_D() {
	outFileErrorResult("d");
}
// e-函数参数类型不匹配
void missSemicn_E() {
	outFileErrorResult("e");
}
// f-无返回值的函数存在return语句
void missSemicn_F() {
	outFileErrorResult("f");
}
// g-有返回值的函数缺少return语句
void missSemicn_G() {
	outFileErrorResult("g");
}
// h-不能改变常量的值
void missSemicn_H() {
	outFileErrorResult("h");
}
// i-缺少分号
void missSemicn_I() {
	outFileErrorResult("i");
}
// j-缺少)
void missSemicn_J() {
	outFileErrorResult("j");
}
// k-缺少]
void missSemicn_K() {
	outFileErrorResult("k");
}
// l-printf中格式字符与表达式个数不匹配
void missSemicn_L() {
	outFileErrorResult("l");
}
// m-在非循环块中使用break和continue
void missSemicn_M() {
	outFileErrorResult("m");
}
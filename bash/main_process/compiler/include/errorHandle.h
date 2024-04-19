#pragma once
#include "define.h"

static std::ofstream ErrorOutFile("error.txt", std::ios::out);
static int globalLineNumber;

typedef enum {
	Null, A, B, C, D, E, F, G, H, I, J, K, L, M
}emErrorType;

void errorHandle(const emErrorType& errorType, const int& lineNum);

// a-格式字符串中出现非法字符
void missSemicn_A();
// b-函数名或变量名在当前作用域下重复定义
void missSemicn_B();
// c-使用未定义的标识符
void missSemicn_C();
// d-函数参数个数不匹配
void missSemicn_D();
// e-函数参数类型不匹配
void missSemicn_E();
// f-无返回值的函数存在return语句
void missSemicn_F();
// g-有返回值的函数缺少return语句
void missSemicn_G();
// h-不能改变常量的值
void missSemicn_H();
// i-缺少分号
void missSemicn_I();
// j-缺少)
void missSemicn_J();
// k-缺少]
void missSemicn_K();
// l-printf中格式字符与表达式个数不匹配
void missSemicn_L();
// m-在非循环块中使用break和continue
void missSemicn_M();
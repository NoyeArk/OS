#pragma once
#include "define.h"


namespace TableSpace {
	typedef enum {
		constant,
		variable,
		null,
	}Kind;

	struct TableItem {
		std::string name; // 标识符名字
		Kind kind; // 标识符的类别信息
		std::string type;  // int | ffloat | array
		std::string value; // 常量标识符代表的常数值
		//int level; // 标识符所在的层(常量标识符不用)
		//int addr; // 变量标识符的偏移地址
		//int size; // 过程活动记录的初始数据区大小

		TableItem() {
			this->kind = null;
		}

		TableItem(std::string name, Kind kind, std::string value, std::string type)
			: name(name), kind(kind), value(value), type(type)
		{}
	};

	typedef enum {
		Int,
		Void
	}ReturnType;

	struct FuncItem {
		std::string name; // 函数名字
		ReturnType retType; // 函数返回值
		int paramNum; // 函数参数个数
		std::vector<std::pair<std::string, std::string>> typeNameVector;  // 第一个字段是类型，第二个是名字
		// 该字段用来存储函数体
		std::vector<std::string> funcBody;
		std::vector<std::string> funcBodName;

		FuncItem() {}

		FuncItem(std::string name, ReturnType retType,
			int paramNum, std::vector<std::pair<std::string, std::string>> typeNameVector)
			: name(name), retType(retType), paramNum(paramNum), typeNameVector(typeNameVector) {

		}
	};

	static std::stack<std::vector<TableItem>> globalTableStack;
	static std::vector<TableItem> globalCurrentTable; // 当前符号表
	static std::vector<FuncItem> globalFuncTable; // 函数符号表

	bool isConstIdent(const std::string&);

	// 函数符号表操作
	FuncItem getLastestFunc(); // 得到最新函数的相关定义
	void funcInsert(std::string name, ReturnType retType, int paramNum, std::vector<std::pair<std::string, std::string>> typeNameVector);
	FuncItem FuncQuery(const std::string&);
	void funcTableRemove();
	bool isfuncParamExist(const std::string& identName);
	void insertFuncBody(std::vector<std::string> body, std::vector<std::string> bodyName);

	// 变量符号表操作
	void insert(const std::string&, const Kind&, const std::string&, const std::string&);
	TableItem query(const std::string&, const bool& isDecl = false);
	void newScope();
	void remove();
	void update(const std::string& identName, const std::string& val);
}

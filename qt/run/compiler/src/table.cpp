
#include "../include/table.h"
using namespace TableSpace;

void TableSpace::funcInsert(
	std::string name,  // 函数名
	ReturnType retType,  // 返回值类型
	int paramNum, // 函数参数个数
	std::vector<std::pair<std::string, std::string>> typeNameVector // 参数类型
) {
	globalFuncTable.push_back(FuncItem(name, retType, paramNum, typeNameVector));
}


void TableSpace::insert(const std::string& name, const Kind& kind, const std::string& value, const std::string& type) {
	assert(kind == constant || kind == variable);
	globalCurrentTable.push_back(TableItem(name, kind, value, type));
}


TableItem TableSpace::query(const std::string& identName, const bool& isDecl) {
	// 先从当前表中查询，如果没有找到则从栈中其他表中查询
	for (const TableItem item : globalCurrentTable) {
		if (item.name == identName) {
			return item;
		}
	}
	if (!isDecl) { // 不是变量声明
		std::stack<std::vector<TableItem>> copyStack(globalTableStack);
		while (!copyStack.empty()) {
			std::vector<TableItem> topTable = copyStack.top(); // 获取栈顶元素
			for (const TableItem item : topTable) {
				if (item.name == identName) {
					return item;
				}
			}
			copyStack.pop(); // 弹出栈顶元素
		}
	}
	return TableItem();
}

// 查询函数名
FuncItem TableSpace::FuncQuery(const std::string& identName) {
	for (const FuncItem item : globalFuncTable) {
		if (item.name == identName) {
			return item;
		}
	}
	return FuncItem();
}
bool TableSpace::isfuncParamExist(const std::string& identName) {
	FuncItem item = globalFuncTable.back();
	for (const std::pair<std::string, std::string> typeName : item.typeNameVector) {
		if (typeName.second == identName)
			return true;
	}
	return false;
}


void TableSpace::remove() {
	if (globalTableStack.empty()) {
		globalCurrentTable.clear();
	}
	else {
		globalCurrentTable = globalTableStack.top();
		globalTableStack.pop();
	}
}
void TableSpace::funcTableRemove() {
	globalFuncTable.clear();
}


bool TableSpace::isConstIdent(const std::string& tokenName) {
	TableItem identInfo = TableSpace::query(tokenName);
	if (identInfo.kind == TableSpace::constant) {
		return true;
	}
	return false;
}


FuncItem TableSpace::getLastestFunc() {
	return globalFuncTable.back();
}

void TableSpace::newScope() {
	globalTableStack.push(globalCurrentTable);
	globalCurrentTable.clear();
}

void TableSpace::update(const std::string& identName, const std::string& val) {
	// 查询符号表，进行值的更新
	// 先从当前表中查询，如果没有找到则从栈中其他表中查询
	for (size_t ii = 0; ii < globalCurrentTable.size(); ii++) {
		if (globalCurrentTable[ii].name == identName) {
			globalCurrentTable[ii].value = val;
			return;
		}
	}
	std::stack<std::vector<TableItem>> copyStack(globalTableStack);
	// 清空栈中的内容
	while (!globalTableStack.empty()) {
		globalTableStack.pop();
	}
	while (!copyStack.empty()) {
		std::vector<TableItem> topTable = copyStack.top(); // 获取栈顶元素
		for (size_t ii = 0; ii < topTable.size(); ii++) {
			if (topTable[ii].name == identName) {
				topTable[ii].value = val;
				break;
			}
		}
		globalTableStack.push(topTable);
		copyStack.pop(); // 弹出栈顶元素
	}
}


void TableSpace::insertFuncBody(std::vector<std::string> body, std::vector<std::string> bodyName) {
	globalFuncTable.back().funcBody = body;
	globalFuncTable.back().funcBodName = bodyName;
}

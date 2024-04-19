
#include "../include/table.h"
using namespace TableSpace;

void TableSpace::funcInsert(
	std::string name,  // ������
	ReturnType retType,  // ����ֵ����
	int paramNum, // ������������
	std::vector<std::pair<std::string, std::string>> typeNameVector // ��������
) {
	globalFuncTable.push_back(FuncItem(name, retType, paramNum, typeNameVector));
}


void TableSpace::insert(const std::string& name, const Kind& kind, const std::string& value, const std::string& type) {
	assert(kind == constant || kind == variable);
	globalCurrentTable.push_back(TableItem(name, kind, value, type));
}


TableItem TableSpace::query(const std::string& identName, const bool& isDecl) {
	// �ȴӵ�ǰ���в�ѯ�����û���ҵ����ջ���������в�ѯ
	for (const TableItem item : globalCurrentTable) {
		if (item.name == identName) {
			return item;
		}
	}
	if (!isDecl) { // ���Ǳ�������
		std::stack<std::vector<TableItem>> copyStack(globalTableStack);
		while (!copyStack.empty()) {
			std::vector<TableItem> topTable = copyStack.top(); // ��ȡջ��Ԫ��
			for (const TableItem item : topTable) {
				if (item.name == identName) {
					return item;
				}
			}
			copyStack.pop(); // ����ջ��Ԫ��
		}
	}
	return TableItem();
}

// ��ѯ������
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
	// ��ѯ���ű�����ֵ�ĸ���
	// �ȴӵ�ǰ���в�ѯ�����û���ҵ����ջ���������в�ѯ
	for (size_t ii = 0; ii < globalCurrentTable.size(); ii++) {
		if (globalCurrentTable[ii].name == identName) {
			globalCurrentTable[ii].value = val;
			return;
		}
	}
	std::stack<std::vector<TableItem>> copyStack(globalTableStack);
	// ���ջ�е�����
	while (!globalTableStack.empty()) {
		globalTableStack.pop();
	}
	while (!copyStack.empty()) {
		std::vector<TableItem> topTable = copyStack.top(); // ��ȡջ��Ԫ��
		for (size_t ii = 0; ii < topTable.size(); ii++) {
			if (topTable[ii].name == identName) {
				topTable[ii].value = val;
				break;
			}
		}
		globalTableStack.push(topTable);
		copyStack.pop(); // ����ջ��Ԫ��
	}
}


void TableSpace::insertFuncBody(std::vector<std::string> body, std::vector<std::string> bodyName) {
	globalFuncTable.back().funcBody = body;
	globalFuncTable.back().funcBodName = bodyName;
}

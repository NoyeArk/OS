#pragma once
#include "define.h"


namespace TableSpace {
	typedef enum {
		constant,
		variable,
		null,
	}Kind;

	struct TableItem {
		std::string name; // ��ʶ������
		Kind kind; // ��ʶ���������Ϣ
		std::string type;  // int | ffloat | array
		std::string value; // ������ʶ������ĳ���ֵ
		//int level; // ��ʶ�����ڵĲ�(������ʶ������)
		//int addr; // ������ʶ����ƫ�Ƶ�ַ
		//int size; // ���̻��¼�ĳ�ʼ��������С

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
		std::string name; // ��������
		ReturnType retType; // ��������ֵ
		int paramNum; // ������������
		std::vector<std::pair<std::string, std::string>> typeNameVector;  // ��һ���ֶ������ͣ��ڶ���������
		// ���ֶ������洢������
		std::vector<std::string> funcBody;
		std::vector<std::string> funcBodName;

		FuncItem() {}

		FuncItem(std::string name, ReturnType retType,
			int paramNum, std::vector<std::pair<std::string, std::string>> typeNameVector)
			: name(name), retType(retType), paramNum(paramNum), typeNameVector(typeNameVector) {

		}
	};

	static std::stack<std::vector<TableItem>> globalTableStack;
	static std::vector<TableItem> globalCurrentTable; // ��ǰ���ű�
	static std::vector<FuncItem> globalFuncTable; // �������ű�

	bool isConstIdent(const std::string&);

	// �������ű����
	FuncItem getLastestFunc(); // �õ����º�������ض���
	void funcInsert(std::string name, ReturnType retType, int paramNum, std::vector<std::pair<std::string, std::string>> typeNameVector);
	FuncItem FuncQuery(const std::string&);
	void funcTableRemove();
	bool isfuncParamExist(const std::string& identName);
	void insertFuncBody(std::vector<std::string> body, std::vector<std::string> bodyName);

	// �������ű����
	void insert(const std::string&, const Kind&, const std::string&, const std::string&);
	TableItem query(const std::string&, const bool& isDecl = false);
	void newScope();
	void remove();
	void update(const std::string& identName, const std::string& val);
}

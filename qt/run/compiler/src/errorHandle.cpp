
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

// a-��ʽ�ַ����г��ַǷ��ַ�
void missSemicn_A() {
	outFileErrorResult("a");
}
// b-��������������ڵ�ǰ���������ظ�����
void missSemicn_B() {
	outFileErrorResult("b");
}
// c-ʹ��δ����ı�ʶ��
void missSemicn_C() {
	outFileErrorResult("c");
}
// d-��������������ƥ��
void missSemicn_D() {
	outFileErrorResult("d");
}
// e-�����������Ͳ�ƥ��
void missSemicn_E() {
	outFileErrorResult("e");
}
// f-�޷���ֵ�ĺ�������return���
void missSemicn_F() {
	outFileErrorResult("f");
}
// g-�з���ֵ�ĺ���ȱ��return���
void missSemicn_G() {
	outFileErrorResult("g");
}
// h-���ܸı䳣����ֵ
void missSemicn_H() {
	outFileErrorResult("h");
}
// i-ȱ�ٷֺ�
void missSemicn_I() {
	outFileErrorResult("i");
}
// j-ȱ��)
void missSemicn_J() {
	outFileErrorResult("j");
}
// k-ȱ��]
void missSemicn_K() {
	outFileErrorResult("k");
}
// l-printf�и�ʽ�ַ�����ʽ������ƥ��
void missSemicn_L() {
	outFileErrorResult("l");
}
// m-�ڷ�ѭ������ʹ��break��continue
void missSemicn_M() {
	outFileErrorResult("m");
}
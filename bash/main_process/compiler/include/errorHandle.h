#pragma once
#include "define.h"

static std::ofstream ErrorOutFile("error.txt", std::ios::out);
static int globalLineNumber;

typedef enum {
	Null, A, B, C, D, E, F, G, H, I, J, K, L, M
}emErrorType;

void errorHandle(const emErrorType& errorType, const int& lineNum);

// a-��ʽ�ַ����г��ַǷ��ַ�
void missSemicn_A();
// b-��������������ڵ�ǰ���������ظ�����
void missSemicn_B();
// c-ʹ��δ����ı�ʶ��
void missSemicn_C();
// d-��������������ƥ��
void missSemicn_D();
// e-�����������Ͳ�ƥ��
void missSemicn_E();
// f-�޷���ֵ�ĺ�������return���
void missSemicn_F();
// g-�з���ֵ�ĺ���ȱ��return���
void missSemicn_G();
// h-���ܸı䳣����ֵ
void missSemicn_H();
// i-ȱ�ٷֺ�
void missSemicn_I();
// j-ȱ��)
void missSemicn_J();
// k-ȱ��]
void missSemicn_K();
// l-printf�и�ʽ�ַ�����ʽ������ƥ��
void missSemicn_L();
// m-�ڷ�ѭ������ʹ��break��continue
void missSemicn_M();
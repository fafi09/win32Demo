// DllFuncCall.cpp : �������̨Ӧ�ó������ڵ㡣
//

//������ڸ�������ʱexe�����·�� ../bin/DllFuncCall.exe
//���µ������ʱ�����֡��޷����������Ҳ���ָ���ļ���
//�������,��ô��������·��Ҳ��Ҫͬ����Ϊ../bin�Ϳ�����

#include "conio.h"
#include "stdafx.h"
#pragma comment(lib, "../lib/dllfunc.lib")

int DLL_Add(int n1, int n2);
extern "C" int DLL_Sub(int n1, int n2);
int DLL_Mul(int n1, int n2);

int _tmain(int argc, _TCHAR* argv[])
{
	int nAdd = DLL_Add(100, 50);
	int nSub = DLL_Sub(100, 50);
	int nMul = DLL_Mul(100, 50);

	printf("nAdd=%d, nSub=%d, nMul=%d\n",nAdd, nSub, nMul);

	getch();
	return 0;
}


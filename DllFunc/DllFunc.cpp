// DllFunc.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

//c++������ʽ
_declspec(dllexport) int DLL_Add(int n1, int n2)
{
	return n1+n2;
}

//c������ʽ
extern "C" _declspec(dllexport) int DLL_Sub(int n1, int n2)
{
	return n1 - n2;
}

//def������ʽ
int DLL_Mul(int n1, int n2)
{
	return n1 * n2;
}
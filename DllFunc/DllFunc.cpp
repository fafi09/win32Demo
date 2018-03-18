// DllFunc.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//c++导出方式
_declspec(dllexport) int DLL_Add(int n1, int n2)
{
	return n1+n2;
}

//c导出方式
extern "C" _declspec(dllexport) int DLL_Sub(int n1, int n2)
{
	return n1 - n2;
}

//def导出方式
int DLL_Mul(int n1, int n2)
{
	return n1 * n2;
}
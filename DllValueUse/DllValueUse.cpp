// DllValueUse.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "conio.h"
#pragma comment(lib, "../lib/dllvalue.lib")

extern _declspec(dllimport) int g_nValue1;
extern _declspec(dllimport) int g_nValue2;

int _tmain(int argc, _TCHAR* argv[])
{
	printf("g_nValue1=%d\n",g_nValue1);
	printf("g_nValue2=%d\n",g_nValue2);
	getch();
	return 0;
}


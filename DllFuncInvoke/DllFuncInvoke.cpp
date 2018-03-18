// DllFuncInvoke.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "conio.h"

typedef int (* DLL_Add)(int, int);
typedef int (* DLL_Sub)(int, int);
typedef int (* DLL_Mul)(int, int);

/**
_declspec�����ĺ����������������仯������ֱ��ʹ��GetProcAddress
extern "C" �������ޱ仯������ʹ��
defҲ����ʹ��
*/
int _tmain(int argc, _TCHAR* argv[])
{
	HMODULE hModule 
		= LoadLibrary(TEXT("dllfunc.dll"));

	if(hModule == NULL)
	{
		printf("error");
	}
	/*DLL_Add Add 
		= (DLL_Add)GetProcAddress(hModule, "DLL_Add");*/
	DLL_Sub Sub 
		= (DLL_Sub)GetProcAddress(hModule, "DLL_Sub");
	DLL_Mul Mul 
		= (DLL_Mul)GetProcAddress(hModule, "DLL_Mul");

	/*printf("add=%d,sub=%d,mul=%d\n",
		Add(100,50), 
		Sub(100,50), 
		Mul(100,50));*/
	printf("sub=%d\n", Sub(100,50));
	printf("mul=%d\n", Mul(100,50));
	
	FreeLibrary(hModule);
	getch();
	return 0;
}


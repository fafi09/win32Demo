// DllFuncCall.cpp : 定义控制台应用程序的入口点。
//

//如果由于改了连接时exe的输出路径 ../bin/DllFuncCall.exe
//导致点击运行时，出现【无法启动程序，找不到指定文件】
//这个错误,那么常规的输出路径也需要同步改为../bin就可以了

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


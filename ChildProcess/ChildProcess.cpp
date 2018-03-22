// ChildProcess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	printf("ChlidProc: Hello World!\n");
	printf("ChlidProc: argc=%d\n", argc );
	wprintf(TEXT("ChlidProc: argv=%s\n"), argv[0] );

	getch( );
	return 0;
}


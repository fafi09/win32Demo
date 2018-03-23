// winthread.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

DWORD WINAPI ThreadFunc( LPVOID lpParam )
{
	DWORD nValue = (DWORD)lpParam;
	for(int nIndex = 0; nIndex < 10; nIndex++)
	{
		printf("Thread proc1 -------------%d\n",nValue);
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI ThreadFunc2( LPVOID lpParam )
{
	while(1)
	{
		printf("-------------Thread proc2\n");
		Sleep(1000);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread = NULL; 
	DWORD dwThreadId = 0;
	DWORD dwThrdParam = 2; 
	char szMsg[80] = {0};

	//创建线程
    hThread = CreateThread( 
        NULL,                        // no security attributes 
        0,                           // use default stack size  
        ThreadFunc,                  // thread function 
		(LPVOID)dwThrdParam,         // argument to thread function 
        CREATE_SUSPENDED,            // use default creation flags 
        &dwThreadId);                // returns the thread identifier 

	printf("tid=%d\n",dwThreadId);
	printf("thread handle=%p\n",hThread);

	getch();
	//开始线程
	ResumeThread(hThread);

	//等待线程1执行完
	WaitForSingleObject(hThread, INFINITE);
	//关闭线程1
	CloseHandle(hThread);

	//创建线程2
	hThread = CreateThread( 
        NULL,                        // no security attributes 
        0,                           // use default stack size  
        ThreadFunc2,                  // thread function 
		NULL,         // argument to thread function 
        0,            // use default creation flags 
        &dwThreadId);                // returns the thread identifier 

	printf("tid=%d\n",dwThreadId);
	printf("thread handle=%p\n",hThread);

	//挂起线程
	SuspendThread(hThread);
	//等待线程2执行完
	WaitForSingleObject(hThread,INFINITE);
	//关闭线程2
	CloseHandle(hThread);

	return 0;
}


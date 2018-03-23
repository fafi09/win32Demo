// winthread.cpp : �������̨Ӧ�ó������ڵ㡣
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

	//�����߳�
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
	//��ʼ�߳�
	ResumeThread(hThread);

	//�ȴ��߳�1ִ����
	WaitForSingleObject(hThread, INFINITE);
	//�ر��߳�1
	CloseHandle(hThread);

	//�����߳�2
	hThread = CreateThread( 
        NULL,                        // no security attributes 
        0,                           // use default stack size  
        ThreadFunc2,                  // thread function 
		NULL,         // argument to thread function 
        0,            // use default creation flags 
        &dwThreadId);                // returns the thread identifier 

	printf("tid=%d\n",dwThreadId);
	printf("thread handle=%p\n",hThread);

	//�����߳�
	SuspendThread(hThread);
	//�ȴ��߳�2ִ����
	WaitForSingleObject(hThread,INFINITE);
	//�ر��߳�2
	CloseHandle(hThread);

	return 0;
}


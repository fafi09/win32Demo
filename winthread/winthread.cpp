// winthread.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "atomlock.h"
#include "criticalsection.h"
#include "event.h"
#include "mymutex.h"
#include "mysemphore.h"
#include "mytimer.h"

CHAR* g_pszText1 = NULL;
//ʹ�ùؼ��ֶ���tls����
__declspec(thread) CHAR* g_pszText2 = NULL;

//ʹ�ú���ʵ��TLS
CHAR* g_pszTlsText = NULL;
DWORD g_nTlsIndex = 0;

void PrintTls()
{
	printf("g_pszTlsText=%s\n",g_pszTlsText);
	//��TLS��ȡֵ
	CHAR* pszText 
		= (CHAR*)TlsGetValue(g_nTlsIndex);

	printf("pszText=%s\n",pszText);
}

DWORD WINAPI PrintTLSFunc( LPVOID lpParam )
{
	CHAR* szText = (CHAR*)lpParam;
	g_pszTlsText = (CHAR*)malloc(100);
	memset(g_pszTlsText, 0, 100);
	strcpy(g_pszTlsText, szText);

	//��ֵ���浽TLS��
	TlsSetValue(g_nTlsIndex, szText);

	while(1)
	{
		PrintTls();
		Sleep(1000);
	}
	return 0;
}

void ThreadTLS2()
{
	HANDLE hThread = NULL;
	DWORD dwThreadId = 0;
	//����Tls index
	g_nTlsIndex = TlsAlloc();

	//�����߳�
	CHAR szText1[] = "Thread1=============";
	hThread 
		= CreateThread(NULL,0, PrintTLSFunc, szText1, 0, &dwThreadId);

	CHAR szText2[] = "=============Thread2";
	hThread 
		= CreateThread(NULL,0, PrintTLSFunc, szText2, 0, &dwThreadId);

	WaitForSingleObject(hThread, INFINITE);

	//�ͷ�����
	TlsFree(g_nTlsIndex);
}

void Print()
{
	printf("szText1=%s\n",g_pszText1);
	printf("szText2=%s\n",g_pszText2);
}

DWORD WINAPI PrintFunc( LPVOID lpParam )
{
	CHAR* szText = (CHAR*)lpParam;
	g_pszText1 = (CHAR*)malloc(100);
	memset(g_pszText1, 0, 100);
	strcpy(g_pszText1, szText);

	g_pszText2 = (CHAR*)malloc(100);
	memset(g_pszText2, 0, 100);
	strcpy(g_pszText2, szText);

	while(1)
	{
		Print();
		Sleep(1000);
	}
	return 0;
}


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


void BaseThread()
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
}

void ThreadTLS1()
{
	DWORD dwThreadId = 0;
	HANDLE hThread = NULL;

	CHAR szText1[] = "Thread1========";
	hThread 
		= CreateThread(
			NULL,
			0,
			PrintFunc, 
			szText1,
			0, 
			&dwThreadId);

	CHAR szText2[] = "===Thread2=====";
	hThread 
		= CreateThread(
			NULL,
			0,
			PrintFunc, 
			szText2,
			0, 
			&dwThreadId);

	CHAR szText3[] = "========Thread3";
	hThread 
		= CreateThread(
			NULL,
			0,
			PrintFunc, 
			szText3,
			0, 
			&dwThreadId);

	getch();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//BaseThread();
	//ThreadTLS1();
	//ThreadTLS2();
	//AtomLock();
	//CriticalSection();

	//EventOperation();

	//MutexOperation();

	//SemphoreOperation();

	//TimerOperation();

	APCTimer();
	return 0;
}


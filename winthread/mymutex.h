
HANDLE g_hMutex = NULL;

DWORD WINAPI MutexFunc1( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		Sleep(500);
		printf("Thread1==========\n");
		ReleaseMutex(g_hMutex);
	}
	return 0;
}

DWORD WINAPI MutexFunc2( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		Sleep(500);
		printf("=====Thread2=====\n");
		ReleaseMutex(g_hMutex);
	}
	return 0;
}

DWORD WINAPI MutexFunc3( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hMutex, INFINITE);
		printf("==========Thread3\n");
		Sleep(500);
		ReleaseMutex(g_hMutex);
	}
	return 0;
}


void CreateMutexThread()
{
	DWORD dwThreadId = 0;
	HANDLE hThread[3] = {0};

	hThread[0] = CreateThread(NULL, 0, MutexFunc1, NULL, 0, &dwThreadId);
	hThread[1] = CreateThread(NULL, 0, MutexFunc2, NULL, 0, &dwThreadId);
	hThread[2] = CreateThread(NULL, 0, MutexFunc3, NULL, 0, &dwThreadId);
	
	getch();
}

void MutexOperation()
{
	//创建互斥量
	g_hMutex = CreateMutex(NULL, FALSE, NULL);

	//创建线程
	CreateMutexThread();

	//关闭互斥量
	CloseHandle(g_hMutex);
}

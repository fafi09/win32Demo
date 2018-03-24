HANDLE g_hSemphore = NULL;

DWORD WINAPI SemphoreSend( LPVOID lpParam )
{
	while(1)
	{
		CHAR ch = getch();
		switch(ch)
		{
		case '1':
			ReleaseSemaphore(g_hSemphore, 1, NULL);
			break;
		case '5':
			ReleaseSemaphore(g_hSemphore, 5, NULL);
			break;
		}
	}
	return 0;
}

DWORD WINAPI SemphoreReceive( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hSemphore, INFINITE);
		printf("Hello Semphore\n");
		Sleep(500);
	}
	return 0;
}


void CreateSemThread()
{
	DWORD dwThreadId = 0;
	HANDLE hThread[2] = {0};

	hThread[0] = CreateThread(NULL, 0, SemphoreSend, NULL, 0, &dwThreadId);
	hThread[1] = CreateThread(NULL, 0, SemphoreReceive, NULL, 0, &dwThreadId);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
}

void SemphoreOperation()
{
	//创建信号量
	g_hSemphore 
		= CreateSemaphore(NULL, 5, 10, NULL);

	//创建线程
	CreateSemThread();

	//关闭信号量
	CloseHandle(g_hSemphore);
}
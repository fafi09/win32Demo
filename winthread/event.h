
HANDLE g_hEvent1 = NULL;
HANDLE g_hEvent2 = NULL;

DWORD WINAPI EventSend( LPVOID lpParam )
{
	while(1)
	{
		SetEvent(g_hEvent1);
		Sleep(1000);
		SetEvent(g_hEvent2);
		Sleep(2000);
	}
	return 0;
}

DWORD WINAPI EventReceive1( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hEvent1, INFINITE);
		printf("hello Event1:%p\n", g_hEvent1);
	}
	return 0;
}

DWORD WINAPI EventReceive2( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hEvent2, INFINITE);
		printf("hello Event2:%p\n", g_hEvent2);
		//�ֶ�����״̬
		ResetEvent(g_hEvent2);
	}
	return 0;
}

void CreateEventThread()
{
	DWORD dwThreadId = 0;
	HANDLE hThread[3] = {0};

	hThread[0] = CreateThread(NULL,0, EventSend, NULL, 0, &dwThreadId);
	hThread[1] = CreateThread(NULL,0, EventReceive1, NULL, 0, &dwThreadId);
	hThread[2] = CreateThread(NULL,0, EventReceive2, NULL, 0, &dwThreadId);

	getch();
}

void EventOperation()
{
	//�Զ�����״̬
	g_hEvent1 
		= CreateEvent(NULL, FALSE, FALSE, NULL);

	//�ֶ�����״̬
	g_hEvent2 
		= CreateEvent(NULL, TRUE, FALSE, NULL);


	//�����߳�
	CreateEventThread();


	CloseHandle(g_hEvent1);
	CloseHandle(g_hEvent2);
}
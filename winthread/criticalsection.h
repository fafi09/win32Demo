
CRITICAL_SECTION g_cs = {0};
LONG g_critialValue = 0;

void PrintCritical()
{
	EnterCriticalSection(&g_cs);

	printf("g_critialValue = %d\n", g_critialValue);
	g_critialValue++;

	LeaveCriticalSection(&g_cs);
}

DWORD WINAPI CriticalFunc( LPVOID lpParam )
{
	while(1)
	{
		PrintCritical();
		Sleep(100);
	}
	return 0;
}

void CreateCritialThread()
{
	HANDLE hThread[2] = {0};
	DWORD dwThreadId = 0;

	hThread[0] = CreateThread(NULL, 0, CriticalFunc, NULL, 0, &dwThreadId);
	hThread[1] = CreateThread(NULL, 0, CriticalFunc, NULL, 0, &dwThreadId);

	getch();
}

void CriticalSection()
{
	//��ʼ���ٽ���
	InitializeCriticalSection(&g_cs);

	//�����߳�
	CreateCritialThread();

	//ɾ���ٽ���
	DeleteCriticalSection(&g_cs);
}
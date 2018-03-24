
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
	//初始化临界区
	InitializeCriticalSection(&g_cs);

	//创建线程
	CreateCritialThread();

	//删除临界区
	DeleteCriticalSection(&g_cs);
}
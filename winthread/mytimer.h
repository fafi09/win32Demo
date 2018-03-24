#define _WIN32_WINNT 0x0400

HANDLE g_hTimer = NULL;

DWORD WINAPI TimerFunc( LPVOID lpParam )
{
	while(1)
	{
		WaitForSingleObject(g_hTimer, INFINITE);
		printf("hello timer\n");
	}
}

void TimerOperation()
{
	//创建定时器
	g_hTimer 
		= CreateWaitableTimer(NULL, FALSE, NULL);

	//设置定时器
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart=-100000000;
	SetWaitableTimer(g_hTimer, &liDueTime, 1000, NULL, NULL, FALSE);
	
	//创建线程
	DWORD dwThreadId = 0;
	HANDLE hThread 
		= CreateThread(NULL, 0, TimerFunc, NULL, 0, &dwThreadId);

	WaitForSingleObject(hThread, INFINITE);

	//关闭定时器
	CloseHandle(g_hTimer);
}

VOID CALLBACK TimerAPCProc(  
  LPVOID lpArgToCompletionRoutine,   // data value
  DWORD dwTimerLowValue,            // timer low value
  DWORD dwTimerHighValue            // timer high value
)
{
	printf("=======APC timer======\n");
}

void APCTimer()
{
	HANDLE hTimer 
		= CreateWaitableTimer(NULL, FALSE, NULL);

	//设置定时器
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart=-100000000;
	SetWaitableTimer(hTimer, &liDueTime, 1000, TimerAPCProc, NULL, FALSE);

	while(1)
	{
		SleepEx(-1, TRUE);
	}


	CloseHandle(hTimer);
}
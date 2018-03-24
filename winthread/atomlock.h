#pragma once
LONG g_nAtomValue1 = 0;
LONG g_nAtomValue2 = 0;
DWORD WINAPI InterLock1( LPVOID lpParam )
{
	for(int nIdx = 0; nIdx < 1000000; nIdx++)
	{
		g_nAtomValue1++;
	}
	return 0;
}

DWORD WINAPI InterLock2( LPVOID lpParam )
{
	for(int nIdx = 0; nIdx < 1000000; nIdx++)
	{
		InterlockedIncrement(&g_nAtomValue2);
	}
	return 0;
}

void AtomLock()
{
	HANDLE hThread[4] = {0};
	DWORD dwThreadId = 0;

	hThread[0] 
		= CreateThread(NULL, 0, InterLock1, NULL, 0, &dwThreadId);

	hThread[1] 
		= CreateThread(NULL, 0, InterLock1, NULL, 0, &dwThreadId);

	hThread[2] 
		= CreateThread(NULL, 0, InterLock2, NULL, 0, &dwThreadId);

	hThread[3] 
		= CreateThread(NULL, 0, InterLock2, NULL, 0, &dwThreadId);

	WaitForMultipleObjects(4, hThread, TRUE, INFINITE);

	printf("g_nAtomValue1=%d ,g_nAtomValue2=%d\n", 
		g_nAtomValue1,g_nAtomValue2);

	getch();
}
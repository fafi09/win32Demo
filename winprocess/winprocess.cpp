// winprocess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../psapi/PSAPI.H"
#pragma comment(lib,"../psapi/Psapi.Lib")

void EnvStrings()
{
	LPVOID sEnv = GetEnvironmentStrings();
	LPTSTR pszEnv = (LPTSTR)sEnv;
	while(pszEnv[0] != 0)
	{
		wprintf(TEXT("%s\n"),pszEnv);
		pszEnv = pszEnv + wcslen(pszEnv) + 1;
	}
	FreeEnvironmentStrings((LPTSTR)sEnv);
}

void EnvVariable(LPTSTR pszVar)
{
	SetEnvironmentVariable(pszVar, TEXT("C:\\"));

	TCHAR szValue[1024] = {0};

	GetEnvironmentVariable(pszVar, szValue, 1024);

	wprintf(TEXT("%s=%s\n"),pszVar, szValue);
}


void ProcInfo()
{
	DWORD nPid = GetCurrentProcessId();
	//���ص���α���
	//α�����һ������ĳ�������������Ϊ��ǰ���̾����
	//���ù��̿���ʹ�ô˾������Ҫ�������ʱָ���Լ��Ľ��̡�α��������ӽ��̼̳С� 
	//�˾�����жԽ��̶���������ܷ���Ȩ�ޡ�
	//����֧�ְ�ȫ��������ϵͳ�����ǰ�ȫ������Ϊ���ù�����������������Ȩ�ޡ�
	//������Щ��֧�ְ�ȫ��������ϵͳ������process_all_access��

	HANDLE hProcess = GetCurrentProcess();
	printf("pid=%d\n",nPid);
	printf("hProcess Handle=%p\n",hProcess);

	//���ݽ���id��ȡ���̾��
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, nPid);
	printf("hProcess Handle=%p\n",hProcess);


}

void ProcessModule()
{

	HMODULE hModules[256] = {0};
	DWORD nNeed = 0;
	HANDLE hProc = GetCurrentProcess();
	EnumProcessModules(hProc, hModules, 256, &nNeed);

	DWORD nCount = nNeed / sizeof(HMODULE);
	for(int i = 0; i < nCount; i++)
	{
		TCHAR szPath[MAX_PATH] = {0};
		GetModuleFileNameEx(
			hProc, 
			hModules[i],
			szPath,
			MAX_PATH);
		wprintf(TEXT("%d-%p-%s\n"),i, hModules[i], szPath);
	}

}

void Create()
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESIZE;
	si.dwXSize = 200;
	si.dwYSize = 100;
	CreateProcess(
		TEXT("../debug/ChildProcess.exe"),
		TEXT("\"hello process\""),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	printf("pid=%d\n",pi.dwProcessId);
	printf("process handle=%p\n",pi.hProcess);
	printf("tid=%d\n",pi.dwThreadId);
	printf("thread handle=%p\n",pi.hThread);
}

void Terminate(DWORD dwProcId)
{
	//�򿪽��̾��
	HANDLE hProc 
		= OpenProcess( 
			PROCESS_ALL_ACCESS, 
			FALSE, 
			dwProcId);

	//��������
	TerminateProcess(hProc, 0);
}

void Wait()
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	EnvStrings();
	EnvVariable(TEXT("testenv"));
	ProcInfo();
	ProcessModule();

	Create();
	//Terminate(66620);

	getch();
	return 0;
}

// winprocess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../psapi/PSAPI.H"
#include <TlHelp32.h>
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
	//���ù��̿���ʹ�ô˾������Ҫ������ʱָ���Լ��Ľ��̡�α��������ӽ��̼̳С� 
	//�˾�����жԽ��̶���������ܷ���Ȩ�ޡ�
	//����֧�ְ�ȫ��������ϵͳ�����ǰ�ȫ������Ϊ���ù����������������Ȩ�ޡ�
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
	si.cb = sizeof(si);

	CreateProcess(TEXT("C:\\Windows\\System32\\mspaint.exe"),
		NULL,NULL,NULL,FALSE, NULL,NULL,NULL,&si,&pi);

	printf("mspaint is running\n");
	//�ȴ����̽���
	WaitForSingleObject(pi.hProcess, INFINITE);
	printf("mspaint is stop\n");
}

HANDLE Create(LPTSTR szPath)
{
	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = {0};

	CreateProcess(szPath,NULL,NULL,NULL,
		FALSE,NULL,NULL,NULL,&si, &pi);

	return pi.hProcess;
}

void Job()
{
	//������ҵ
	HANDLE hJob 
		= CreateJobObject(NULL, TEXT("myJob"));

	//������ҵȨ��
	JOBOBJECT_BASIC_UI_RESTRICTIONS ui = {0};
	
	ui.UIRestrictionsClass 
		= JOB_OBJECT_UILIMIT_READCLIPBOARD |
		  JOB_OBJECT_UILIMIT_WRITECLIPBOARD;

	SetInformationJobObject(
		hJob,
		JobObjectBasicUIRestrictions,
		&ui,
		sizeof(ui));

	//�����̼��뵽��ҵ
	HANDLE hProc 
		= Create(TEXT("C:\\Windows\\System32\\calc.exe"));

	AssignProcessToJobObject(hJob, hProc);

	hProc = Create(TEXT("C:\\Windows\\System32\\calc.exe"));
	AssignProcessToJobObject(hJob, hProc);

	getch();

	//������ҵ
	TerminateJobObject(hJob, 0);

	//�ر�Job
	CloseHandle(hJob);

}

HANDLE GetProcessHandle(LPCWSTR lpName)
{
     DWORD dwPid = 0;
     HANDLE hProcess = NULL;
     HANDLE hProcessSnap;
     PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
     hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
     if (hProcessSnap == INVALID_HANDLE_VALUE)
     {
         //printf("Error: CreateToolhelp32Snapshot (of processes)\r\n");
         return NULL;
     }

    // Set the size of the structure before using it.
     pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
     // and exit if unsuccessful
     if (!Process32First(hProcessSnap, &pe32))
     {
         //printf("Error: Process32First\r\n"); // show cause of failure
         CloseHandle(hProcessSnap);          // clean the snapshot object
         return NULL;
     }

    // Now walk the snapshot of processes, and
     // display information about each process in turn
     int namelen = 200;
     char name[201] = { 0 };
     do
     {
         if (!wcscmp(pe32.szExeFile, lpName)) {
             dwPid = pe32.th32ProcessID;
             hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
             break;
         }

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
     return hProcess;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//EnvStrings();
	//EnvVariable(TEXT("testenv"));
	//ProcInfo();
	//ProcessModule();

	//Create();
	//Terminate(66620);

	//Wait();

	//Job();
	//getch();

	HANDLE hProcess = GetProcessHandle(TEXT("QQ.exe"));
	TerminateProcess(hProcess, 0);
	return 0;
}


// winmem.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

void Status()
{
	MEMORYSTATUS mms = {0};
	mms.dwLength = sizeof(MEMORYSTATUS);

	GlobalMemoryStatus(&mms);

	printf("TotalPhys=%u\n",mms.dwTotalPhys);
	printf("AvailPhys=%u\n",mms.dwAvailPhys);
	printf("TotalPageFile=%u\n",mms.dwTotalPageFile);
	printf("AvailPageFile=%u\n",mms.dwAvailPageFile);
	printf("TotalVirtual=%u\n",mms.dwTotalVirtual);
	printf("AvailVirtual=%u\n",mms.dwAvailVirtual);
	printf("MemoryLoad=%u\n",mms.dwMemoryLoad);


}

void VirtualAndSoOn()
{
	Status();

	CHAR* pszBuf
		= (CHAR*)VirtualAlloc(
					NULL,
					1024*1024*1024,
					MEM_RESERVE,
					PAGE_READWRITE);

	printf("===========MEM_RESERVE:%p===========\n",pszBuf);
	Status();

	getch();

	pszBuf 
		= (CHAR*)VirtualAlloc(
					pszBuf,
					1024*1024*1024, 
					MEM_COMMIT, 
					PAGE_READWRITE);

	printf("===========MEM_COMMIT:%p===========\n",pszBuf);

	Status();

	getch();

	strcpy(pszBuf, "hello virtual");
	printf("pszBuf=%s\n",pszBuf);
	Status();
	getch();

	BOOL bRet 
		= VirtualFree(pszBuf, 0, MEM_RELEASE);
	//MEM_RELEASE���ѡ���ʱ�򣬵ڶ�����������Ϊ0
	/*BOOL bRet 
		= VirtualFree(pszBuf, 1024*1024*1024, );*/
	if(bRet == FALSE)
	{
		printf("error=%d\n",GetLastError());
	}
	getch();
}

void HeapInfo()
{
	//Ĭ�ϵĶѵľ��
	HANDLE hHeap 
		= GetProcessHeap();
	printf("Default heap:%p\n",hHeap);

	//���жѵľ��
	HANDLE hHeaps[256] = {0};
	DWORD nCount 
		= GetProcessHeaps(256, hHeaps);

	printf("ALL HEAP:%d\n",nCount);
	for(int i = 0; i < nCount; i++)
	{
		printf("%d:%p\n", i, hHeaps[i]);
	}

}

void Heap()
{
	HeapInfo();

	//������
	HANDLE hHeap 
		= HeapCreate(HEAP_GENERATE_EXCEPTIONS, 1024*1024, 0);

	printf("hHeap=%p\n",hHeap);

	HeapInfo();

	CHAR* szBuf 
		= (CHAR*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 64);

	printf("szBuf=%p\n",szBuf);

	strcpy(szBuf, "hello heap");
	printf("szBuf=%s\n",szBuf);


	HeapFree(hHeap, 0, szBuf);

	HeapDestroy(hHeap);
}


void Map()
{
	//�����ļ�
	HANDLE hFile
		= CreateFile(TEXT("D:\\tst1.txt"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

	//�����ļ�ӳ��
	HANDLE hMap
		= CreateFileMapping(
			hFile, 
			NULL,
			PAGE_READWRITE,
			0, 
			1024*1024,
			NULL);

	//ӳ���ַ
	CHAR* pszText 
		= (CHAR*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS,0,0,0);

	//ʹ���ڴ�
	strcpy(pszText,"Hello File Mapping");
	printf("pszText=%s\n",pszText);
	//ж�ص�ַ
	UnmapViewOfFile(pszText);

	//�ر��ļ�ӳ��
	CloseHandle(hMap);
	//�ر��ļ�
	CloseHandle(hFile);
}

void ShowSysInfo()
{
	SYSTEM_INFO si = {0};
	GetSystemInfo(&si);

	printf("�ڴ�Ҳ��С:%d, ������С��ַ:%p, ��������ַ:%p, ����ķ�������:%d\n",
		si.dwPageSize, 
		si.lpMaximumApplicationAddress,
		si.lpMinimumApplicationAddress,
		si.dwAllocationGranularity);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//VirtualAndSoOn();
	//Heap();
	//Map();
	ShowSysInfo();
	getch();
	return 0;
}


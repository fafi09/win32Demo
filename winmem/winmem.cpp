// winmem.cpp : 定义控制台应用程序的入口点。
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
	//MEM_RELEASE这个选项的时候，第二个参数必须为0
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
	//默认的堆的句柄
	HANDLE hHeap 
		= GetProcessHeap();
	printf("Default heap:%p\n",hHeap);

	//所有堆的句柄
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

	//创建堆
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
	//创建文件
	HANDLE hFile
		= CreateFile(TEXT("D:\\tst1.txt"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

	//创建文件映射
	HANDLE hMap
		= CreateFileMapping(
			hFile, 
			NULL,
			PAGE_READWRITE,
			0, 
			1024*1024,
			NULL);

	//映射地址
	CHAR* pszText 
		= (CHAR*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS,0,0,0);

	//使用内存
	strcpy(pszText,"Hello File Mapping");
	printf("pszText=%s\n",pszText);
	//卸载地址
	UnmapViewOfFile(pszText);

	//关闭文件映射
	CloseHandle(hMap);
	//关闭文件
	CloseHandle(hFile);
}

void ShowSysInfo()
{
	SYSTEM_INFO si = {0};
	GetSystemInfo(&si);

	printf("内存也大小:%d, 可用最小地址:%p, 可用最大地址:%p, 区域的分配粒度:%d\n",
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


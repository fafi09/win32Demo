// winfs.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//卷的类型
void ShowType(LPWSTR DriveName)
{
	UINT type
		= GetDriveType(DriveName);
	switch(type)
	{
	case DRIVE_FIXED:
		printf(" type=%s ","fixed");
		break;
	case DRIVE_CDROM:
		printf(" type=%s ","cdrom");
		break;
	case DRIVE_REMOVABLE:
		printf(" type=%s ","removable");
		break;
	}
}

//卷的信息
void ShowVolumnInfo(LPWSTR DriveName)
{
	TCHAR szVolumnNameBuf[260] = {0};
	DWORD nVolumnSerialNumber = 0;
	DWORD nMaxFileNameLen = 0;
	DWORD nFileSysOp = 0;
	TCHAR szFileSysNameBuf[260] = {0};

	GetVolumeInformation(DriveName, 
						szVolumnNameBuf,
						260, 
						&nVolumnSerialNumber, 
						&nMaxFileNameLen,
						&nFileSysOp,
						szFileSysNameBuf,
						260);

	printf("volumnName=%s, VolumnSerialNum=%08X, MaxFileName=%d, FileSysOp=%08X, FileSysName=%s\n",
		szVolumnNameBuf, 
		nVolumnSerialNumber, 
		nMaxFileNameLen, 
		nFileSysOp, 
		szFileSysNameBuf);


	//FS_CASE_IS_PRESERVED


}

void ShowDiskFreeSpace(LPWSTR DriveName)
{
	//获取盘的大小
	DWORD SectorsPerCluster;
	DWORD BytesPerSector ;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;

	GetDiskFreeSpace(DriveName,
		&SectorsPerCluster,
		&BytesPerSector,
		&NumberOfFreeClusters,
		&TotalNumberOfClusters);

	printf("每个簇包含的扇区=%d, 每个扇区的字节数=%d, 空余簇的数量=%d, 总共的簇的数量=%d\n",
		SectorsPerCluster,
		BytesPerSector,
		NumberOfFreeClusters,
		TotalNumberOfClusters);

	//获取大小
	ULARGE_INTEGER FreeBytesAvailable;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;

	GetDiskFreeSpaceEx(DriveName, 
		&FreeBytesAvailable, 
		&TotalNumberOfBytes, 
		&TotalNumberOfFreeBytes);

	printf("可用的空余字节数=%I64d 全部的字节数=%I64d 空余的字节数=%I64d\n",
		FreeBytesAvailable,
		TotalNumberOfBytes,
		TotalNumberOfFreeBytes);
}


void Create()
{
	HANDLE hFile = CreateFile(TEXT("D:\\tst.txt"),
					GENERIC_READ | GENERIC_WRITE,
					NULL,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if(NULL == hFile)
	{
		DWORD error = GetLastError();
		printf("error=%d\n",error);
	}
	CloseHandle(hFile);
}


void Write()
{
	HANDLE hFile = CreateFile(TEXT("D:\\tst.txt"),
		GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	char szText[] = "Hello file";
	DWORD nWrite = 0;
	WriteFile(hFile, szText, strlen(szText), &nWrite, NULL); 
	printf("nWrite=%d\n",nWrite);
	CloseHandle(hFile);
}

void Read()
{
	HANDLE hFile = CreateFile(TEXT("D:\\tst.txt"),
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	char szText[64] = {0};
	DWORD nRead = 0;
	//设置文件的读写位置
	SetFilePointer(hFile, 1, 0, FILE_BEGIN);

	ReadFile(hFile,szText, 64, &nRead, NULL); 
	printf("nRead=%d, szText=%s\n",nRead, szText);
	CloseHandle(hFile);
}

void FileSize()
{
	HANDLE hFile = CreateFile(TEXT("D:\\Info.rar"),
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//LARGE_INTEGER li;

	DWORD nFileSize = 0;
	DWORD dwSize = GetFileSize(hFile, &nFileSize);
	if(dwSize == INVALID_FILE_SIZE)
	{
		DWORD dwError = GetLastError();
		printf("error=%d\n",dwError);
	}
	//li.LowPart = dwSize;
	//li.HighPart = nFileSize;

	printf("nFileSize=%ld\n",dwSize);


	//大文件超过0xffffffff
	LARGE_INTEGER lnFileSize;
	GetFileSizeEx(hFile, &lnFileSize);
	printf("lnFilesize=%ld\n",lnFileSize);

	CloseHandle(hFile);
}

void OperateFile()
{
	CopyFile(
		TEXT("D:\\tst.txt"),
		TEXT("D:\\tst1.txt"), 
		TRUE);

	DeleteFile(TEXT("D:\\tst.txt"));
}

void PrintFileTime(LPSTR name, LPFILETIME pFileTime)
{
	//将文件时间转化为本时区时间
	FileTimeToLocalFileTime(pFileTime, pFileTime);
	//将文件时间转化为系统时间
	SYSTEMTIME systime = {0};
	FileTimeToSystemTime(pFileTime, &systime);
	printf("%s: %d-%d-%d %d:%d:%d\n",name,
		systime.wYear,systime.wMonth,systime.wDay,
		systime.wHour,systime.wMinute,systime.wSecond);
}

void Attri()
{
	DWORD nAttr = GetFileAttributes(TEXT("D:\\tst1.txt"));
	printf("nAttr=%08X\n",nAttr);
	switch(nAttr)
	{
	case FILE_ATTRIBUTE_NORMAL:
		printf("FILE_ATTRIBUTE_NORMAL\n");
		break;
	case FILE_ATTRIBUTE_ARCHIVE:
		printf("FILE_ATTRIBUTE_ARCHIVE\n");
		break;
	}

	WIN32_FILE_ATTRIBUTE_DATA data= {0};
	GetFileAttributesEx(
		TEXT("D:\\tst1.txt"),
		GetFileExInfoStandard,
		&data);

	PrintFileTime("CreateTime", &data.ftCreationTime);
	PrintFileTime("AccessTime", &data.ftLastAccessTime);
	PrintFileTime("WriteTime", &data.ftLastWriteTime);
}

void Find(LPCWSTR szFileName, LPCWSTR szType)
{

	TCHAR szFind[MAX_PATH] = {0};
	wsprintf(szFind,TEXT("%s%s"), szFileName, szType);

	WIN32_FIND_DATAW data = {0};
	HANDLE hFind 
		= FindFirstFile(szFind, &data);

	BOOL bRet = TRUE;
	while(bRet == TRUE)
	{
		if(data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			
			if(data.cFileName[0] != '.')
			{
				wprintf(TEXT("+[%s]\n"),data.cFileName);
				TCHAR szNextFile[MAX_PATH] = {0};
				wsprintf(szNextFile,TEXT("%s%s\\"), szFileName, data.cFileName);
				Find(szNextFile, szType);
			}
		}
		else
		{
			wprintf(TEXT("-[%s]\n"),data.cFileName);
		}
		bRet = FindNextFile(hFind, &data);
	}

	FindClose(hFind);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//返回一个DWORD值,这个值
    //的每个位代表是否存在相应的卷
	//111100 FEDC
	DWORD nDrive = GetLogicalDrives();
	//转换成二进制
	char s[32] = {0};
	itoa(nDrive, s, 2);
	printf("nDrive=%s\n",s);

	//获取一个所卷的盘符的字符串
	TCHAR szDriveName[260] = {0};
	TCHAR* szTmp = NULL;
	DWORD nlen 
		= GetLogicalDriveStrings(260, szDriveName);

	printf("driveLen=%d\n",nlen);

	szTmp = szDriveName;
	while(szTmp[0] != NULL)
	{
		printf("driveName=%s ",szTmp);
		//卷的类型
		ShowType(szTmp);
		printf("\n");
		//卷的信息
		ShowVolumnInfo(szTmp);
		//卷的大小
		ShowDiskFreeSpace(szTmp);
		szTmp = wcslen(szTmp) + 1 + szTmp;
	}

	//修改卷的名称
    //  BOOL SetVolumeLabel(
    // LPCTSTR lpRootPathName,//盘符
    // LPCTSTR lpVolumeName);//新名称

	//获取windows目录
	TCHAR szWndDir[64] = {0};
	GetWindowsDirectory(szWndDir, 64);
	printf("windows dir:%s\n",szWndDir);

	//获取系统目录
	TCHAR szSysDir[MAX_PATH] = {0};
	GetSystemDirectory(szSysDir, MAX_PATH);
	printf("system dir:%s\n",szSysDir);

	//获取临时文件目录
	TCHAR szTmpDir[MAX_PATH] = {0};
	GetTempPath(MAX_PATH, szTmpDir);
	printf("temp dir:%s\n",szTmpDir);

	//获取当前目录，应用程序正在使用的目录
	TCHAR szCurrDir[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrDir);
	printf("current dir:%s\n",szCurrDir);

	//设置当前的目录
	SetCurrentDirectory(TEXT("C:\\study"));

	memset(szCurrDir, 0, MAX_PATH);
	
	GetCurrentDirectory(MAX_PATH, szCurrDir);
	printf("current dir:%s\n",szCurrDir);


	//创建目录
	//CreateDirectory(TEXT("D:\\1\\win32"),NULL);

	//删除目录
	//RemoveDirectory(TEXT("D:\\1\\2\\4\\8"));

	//修改目录
	//MoveFile(TEXT("D:\\1\\2\\4\\9"),TEXT("D:\\1\\2\\4\\11"));

	//创建文件
	Create();
	//写入文件
	Write();
	//读文件
	Read();

	//文件的大小
	FileSize();

	//文件操作
	OperateFile();

	//文件属性
	Attri();

	//搜索目录
	Find(TEXT("D:\\1\\"),TEXT("*.*"));
	getch();
	return 0;
}


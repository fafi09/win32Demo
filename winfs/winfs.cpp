// winfs.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//�������
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

//�����Ϣ
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
	//��ȡ�̵Ĵ�С
	DWORD SectorsPerCluster;
	DWORD BytesPerSector ;
	DWORD NumberOfFreeClusters;
	DWORD TotalNumberOfClusters;

	GetDiskFreeSpace(DriveName,
		&SectorsPerCluster,
		&BytesPerSector,
		&NumberOfFreeClusters,
		&TotalNumberOfClusters);

	printf("ÿ���ذ���������=%d, ÿ���������ֽ���=%d, ����ص�����=%d, �ܹ��Ĵص�����=%d\n",
		SectorsPerCluster,
		BytesPerSector,
		NumberOfFreeClusters,
		TotalNumberOfClusters);

	//��ȡ��С
	ULARGE_INTEGER FreeBytesAvailable;
	ULARGE_INTEGER TotalNumberOfBytes;
	ULARGE_INTEGER TotalNumberOfFreeBytes;

	GetDiskFreeSpaceEx(DriveName, 
		&FreeBytesAvailable, 
		&TotalNumberOfBytes, 
		&TotalNumberOfFreeBytes);

	printf("���õĿ����ֽ���=%I64d ȫ�����ֽ���=%I64d ������ֽ���=%I64d\n",
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
	//�����ļ��Ķ�дλ��
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


	//���ļ�����0xffffffff
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
	//���ļ�ʱ��ת��Ϊ��ʱ��ʱ��
	FileTimeToLocalFileTime(pFileTime, pFileTime);
	//���ļ�ʱ��ת��Ϊϵͳʱ��
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
	//����һ��DWORDֵ,���ֵ
    //��ÿ��λ�����Ƿ������Ӧ�ľ�
	//111100 FEDC
	DWORD nDrive = GetLogicalDrives();
	//ת���ɶ�����
	char s[32] = {0};
	itoa(nDrive, s, 2);
	printf("nDrive=%s\n",s);

	//��ȡһ��������̷����ַ���
	TCHAR szDriveName[260] = {0};
	TCHAR* szTmp = NULL;
	DWORD nlen 
		= GetLogicalDriveStrings(260, szDriveName);

	printf("driveLen=%d\n",nlen);

	szTmp = szDriveName;
	while(szTmp[0] != NULL)
	{
		printf("driveName=%s ",szTmp);
		//�������
		ShowType(szTmp);
		printf("\n");
		//�����Ϣ
		ShowVolumnInfo(szTmp);
		//��Ĵ�С
		ShowDiskFreeSpace(szTmp);
		szTmp = wcslen(szTmp) + 1 + szTmp;
	}

	//�޸ľ������
    //  BOOL SetVolumeLabel(
    // LPCTSTR lpRootPathName,//�̷�
    // LPCTSTR lpVolumeName);//������

	//��ȡwindowsĿ¼
	TCHAR szWndDir[64] = {0};
	GetWindowsDirectory(szWndDir, 64);
	printf("windows dir:%s\n",szWndDir);

	//��ȡϵͳĿ¼
	TCHAR szSysDir[MAX_PATH] = {0};
	GetSystemDirectory(szSysDir, MAX_PATH);
	printf("system dir:%s\n",szSysDir);

	//��ȡ��ʱ�ļ�Ŀ¼
	TCHAR szTmpDir[MAX_PATH] = {0};
	GetTempPath(MAX_PATH, szTmpDir);
	printf("temp dir:%s\n",szTmpDir);

	//��ȡ��ǰĿ¼��Ӧ�ó�������ʹ�õ�Ŀ¼
	TCHAR szCurrDir[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrDir);
	printf("current dir:%s\n",szCurrDir);

	//���õ�ǰ��Ŀ¼
	SetCurrentDirectory(TEXT("C:\\study"));

	memset(szCurrDir, 0, MAX_PATH);
	
	GetCurrentDirectory(MAX_PATH, szCurrDir);
	printf("current dir:%s\n",szCurrDir);


	//����Ŀ¼
	//CreateDirectory(TEXT("D:\\1\\win32"),NULL);

	//ɾ��Ŀ¼
	//RemoveDirectory(TEXT("D:\\1\\2\\4\\8"));

	//�޸�Ŀ¼
	//MoveFile(TEXT("D:\\1\\2\\4\\9"),TEXT("D:\\1\\2\\4\\11"));

	//�����ļ�
	Create();
	//д���ļ�
	Write();
	//���ļ�
	Read();

	//�ļ��Ĵ�С
	FileSize();

	//�ļ�����
	OperateFile();

	//�ļ�����
	Attri();

	//����Ŀ¼
	Find(TEXT("D:\\1\\"),TEXT("*.*"));
	getch();
	return 0;
}


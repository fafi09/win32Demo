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


	getch();
	return 0;
}


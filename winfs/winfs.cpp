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


	getch();
	return 0;
}


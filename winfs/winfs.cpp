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


	getch();
	return 0;
}


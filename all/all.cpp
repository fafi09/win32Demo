// all.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "all.h"
#include "stdio.h"

#define MAX_LOADSTRING 100
//ʹ��ʱ,Ҫ����TCHAR.Hͷ�ļ�֧��,ʹ��
//			  _UNICODE �꿪�ؽ��б���
			   
//			   CL window.c  /D_UNICODE
/*
int WINAPI MessageBoxA(
    HWND hWnd ,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uType);

int WINAPI MessageBoxW(
    HWND hWnd ,
    LPCWSTR lpText,
    LPCWSTR lpCaption,
    UINT uType);

#ifdef UNICODE
#define MessageBox  MessageBoxW
#else
#define MessageBox  MessageBoxA
#endif // !UNICODE
*/
// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HANDLE hOutStd = NULL;
BOOL isPaintRect;
UINT_PTR g_timerId = NULL;

int rectLeft = 600;
int rectTop = 600;
int rectRight = 700;
int rectBottom = 700;

VOID CALLBACK TimerProc1(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT_PTR idEvent,  // timer identifier
  DWORD dwTime       // current system time
)
{
	TCHAR szText[] = TEXT("TimerProc1....\n");
	WriteConsole(hOutStd,szText,wcslen(szText),NULL,NULL);
}

void DrawBmp(HDC hDC)
{
	//����λͼ��Դ
	HBITMAP hBitMap = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));

	//�õ���Դ��Ϣ
	BITMAP bmpInfo = {0};
	GetObject(hBitMap,sizeof(BITMAP),&bmpInfo);

	//����װλͼ��dc
	HDC hMapDC = CreateCompatibleDC(hDC);

	//��λͼװ����dc
	HBITMAP hBitMapOld = (HBITMAP)SelectObject(hMapDC, hBitMap);

	//����λͼ
	BitBlt(hDC,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,hMapDC ,0,0,SRCCOPY);
	//�������
	StretchBlt( hDC, 0, 0, bmpInfo.bmWidth+100, bmpInfo.bmHeight+100, 
		hMapDC, 0, 0, bmpInfo.bmWidth, 
		bmpInfo.bmHeight, SRCCOPY );

	//��ԭDC
	SelectObject(hMapDC, hBitMapOld);
	DeleteObject(hBitMap);
	DeleteDC(hMapDC);
}

void DrawRect(HDC hDC)
{
	HPEN hpen, hpenOld;
	HBRUSH hbrush, hbrushOld;

	hpen = CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
	hbrush = CreateSolidBrush(RGB(255, 255, 0));

	hpenOld = (HPEN)SelectObject(hDC, hpen);
	hbrushOld = (HBRUSH)SelectObject(hDC, hbrush);


	if(isPaintRect)
	{
		
		Rectangle(hDC,rectLeft,rectTop,rectRight,rectBottom);
		int nOldMap = SetMapMode( hDC, MM_ANISOTROPIC );
		//int nOldMap = SetMapMode( hDC, MM_HIMETRIC );
		//SetViewportOrgEx( hDC, 100, 100, NULL );
		SetViewportExtEx( hDC, 4, 4, NULL );
		SetWindowExtEx( hDC, -3, 3, NULL );
		Rectangle(hDC,rectLeft,rectTop,rectRight,rectBottom);
		SetMapMode( hDC, nOldMap );
	}
	else
	{
	}

	SelectObject(hDC, hbrushOld);
	SelectObject(hDC, hpenOld);

	DeleteObject(hbrushOld);

	DeleteObject(hpen);
}

void GetWndClassInfo(HWND hWnd, LPCTSTR lpszClass)
{
	WNDCLASSEX wcx;
	TCHAR result[64] = {0};
	TCHAR clsName[32] = {0};
	GetClassInfoEx(hInst,lpszClass,&wcx);
	GetClassName(hWnd,clsName,32);
	#ifdef UNICODE
	swprintf(result, TEXT("clsExtra:%d,wndExtra:%d,classname:%s"),wcx.cbClsExtra,wcx.cbWndExtra,clsName);
	#else
	sprintf(result, "clsExtra:%d,wndExtra:%d,classname:%s",wcx.cbClsExtra,wcx.cbWndExtra,clsName);
	#endif
	MessageBox(hWnd,result,TEXT("extra"),NULL);
}

void SetExtra(HWND hWnd)
{
	SetClassLong(hWnd,1,20);
	SetWindowLong(hWnd,1,30);
}

void GetExtra(HWND hWnd)
{
	TCHAR result[64] = {0};
	DWORD cl = GetClassLong(hWnd,1);
	DWORD wl = GetWindowLong(hWnd,1);
	#ifdef UNICODE
	swprintf(result,TEXT("clsExtra:%d,wndExtra:%d"),cl,wl);
	#else
	sprintf(result,"clsExtra:%d,wndExtra:%d",cl,wl);
	#endif
	MessageBox(hWnd,result,L"extra",NULL);
}
void Wide2Multi(HWND hWnd)
{
	char msg[64] = {0};
	WCHAR* pwszText = L"wide2Multi";
	int nlen = WideCharToMultiByte(CP_ACP,NULL,pwszText,wcslen(pwszText),NULL,0,NULL,NULL);
	char* szText = (char*)malloc(nlen+1);
	memset(szText,0,nlen+1);
	WideCharToMultiByte(CP_ACP,NULL,pwszText,wcslen(pwszText),szText,nlen,NULL,NULL);
	sprintf(msg,"length:%d",nlen);
	MessageBoxA(hWnd,szText,"len",MB_OK);
	free(szText);
}

void Multi2Wide(HWND hWnd)
{
	char* szText = "Multi2Wide";
	char msg[64] = {0};
	int nlen = MultiByteToWideChar(CP_ACP, NULL,szText,strlen(szText),NULL,0);
	WCHAR* pwszText = (WCHAR*)malloc((nlen+1)*sizeof(WCHAR));
	memset(pwszText,0,(nlen+1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, NULL,szText,strlen(szText),pwszText,nlen);
	//sprintf(msg,"length:%d",nlen);
	//MessageBoxA(hWnd,msg,"len",MB_OK);
	MessageBoxW(hWnd,pwszText,L"Wide",NULL);

	free(pwszText);
}

BOOL OnSetCursor(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//WM_NCHITTEST
	int nHitTest = LOWORD(lParam);
	if(nHitTest != HTCLIENT)
	{
		//���ڿͻ���������FALSE��
		//��DefWindowProc����
		return FALSE;
	}
	RECT rcClient = {0};
	GetClientRect(hWnd,&rcClient);

	POINT ptPos = {0};
	GetCursorPos(&ptPos);
	ScreenToClient(hWnd,&ptPos);

	//����λ�ü��ع��
	HCURSOR hCursor = NULL;
	if( ptPos.x < rcClient.right/2 )
	{
		if( ptPos.y < rcClient.bottom/2 )
		{
			hCursor = LoadCursor( NULL, IDC_SIZEALL );
		}
		else
		{
			hCursor = LoadCursor( NULL, IDC_CROSS );
		}
	}
	else
	{
		if( ptPos.y < rcClient.bottom/2 )
		{
			hCursor = LoadCursor( NULL, IDC_WAIT );
		}
		else
		{
			hCursor = LoadCursor( NULL, IDC_UPARROW );
		}
	}
	// ���ù��
	SetCursor( hCursor );
	return TRUE;
}

void OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nCmdId = LOWORD(wParam);
	TCHAR msg[64] = {0};
	switch(nCmdId)
	{
	case ID_32771:
		//MessageBox(hWnd,L"ID_32771",L"menu",NULL);
		Wide2Multi(hWnd);
		break;
	case ID_32772:
		//MessageBox(hWnd,L"ID_32772",L"menu",NULL);
		Multi2Wide(hWnd);
		break;
	case ID_32773:
		SetExtra(hWnd);
		break;
	case ID_32774:
		GetExtra(hWnd);
		break;
	case ID_32775:
		GetWndClassInfo(hWnd,TEXT("mywnd"));
		break;
	case IDM_ABOUT:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("����...\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case 3000:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("����1\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
		
	case 3001:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("����2\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case ID_32776:
		HMENU hMenu = GetMenu(hWnd);
		UINT state = GetMenuState(hMenu,nCmdId,MF_BYCOMMAND);
		if(!(state&MF_CHECKED))
		{
			isPaintRect = TRUE;
			CheckMenuItem(hMenu,nCmdId,MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			isPaintRect = FALSE;
			CheckMenuItem(hMenu,nCmdId,MF_BYCOMMAND | MF_UNCHECKED);
		}
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	}
}

void OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};

	switch(wParam)
	{
	case 1000:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("timer 1000\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case 1001:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("timer 1001\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case 1002:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("timer 1001\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	default:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("timer %d\n"),g_timerId);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;


	}
}

void OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//���ڴ�����Ϣ�����ڴ��ڴ�����
    //     ���ڴ������յ���һ����Ϣ��
    //     �����������Ϣ�������ݳ�ʼ��/�����Ӵ��ڵȡ�
	LPCREATESTRUCT pCreateStruct = LPCREATESTRUCT(lParam);
	WriteConsole(hOutStd,TEXT("OnCreate begin\n"),wcslen(TEXT("OnCreate begin\n")),NULL,NULL);
	WriteConsole(hOutStd,pCreateStruct->lpszClass,wcslen(pCreateStruct->lpszClass),NULL,NULL);
	WriteConsole(hOutStd,TEXT("OnCreate end\n"),wcslen(TEXT("OnCreate end\n")),NULL,NULL);

	//��ʼ������
	isPaintRect = TRUE;

	//ʹ�ô��ڴ���������ʼ��������ʱ��
	SetTimer(hWnd,1000,3*1000,NULL);
	SetTimer(hWnd,1001,5*1000,NULL);
	//ʹ�ô��ڴ���������ָ����ʱ��id����ʼ����ʱ��
	g_timerId = SetTimer(hWnd,NULL,7*1000,NULL);

	//ʹ�ö�ʱ������������ʼ����ʱ��
	SetTimer(NULL,1002,1*1000,TimerProc1);

	//�Ժ�����ʽ�����˵�
	/*
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu,MF_STRING | MF_CHECKED,1001,TEXT("�½�(&n)"));

	AppendMenu(hMenu,MF_STRING | MF_POPUP,(UINT)hFileMenu,TEXT("�ļ�(&F)"));

	SetMenu(hWnd,hMenu);
	*/

	//����ϵͳ�˵�
	HMENU hMenu = GetSystemMenu(hWnd,FALSE);

	InsertMenu(hMenu,3,MF_STRING | MF_BYPOSITION,2001,TEXT("�½�ϵͳ"));
}

void OnInitialMenuPopup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnInitialMenuPopup\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	HMENU hMenu = GetMenu(hWnd);
	HMENU hFileMenu = GetSubMenu(hMenu , 0);
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);

}

void OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SIZE_MAXHIDE:
		WriteConsole(hOutStd,TEXT("SIZE_MAXHIDE\n"),wcslen(TEXT("SIZE_MAXHIDE\n")),NULL,NULL);
		break;
	case SIZE_MAXIMIZED:
		WriteConsole(hOutStd,TEXT("SIZE_MAXIMIZED\n"),wcslen(TEXT("SIZE_MAXIMIZED\n")),NULL,NULL);
		break;
	case SIZE_MAXSHOW:
		WriteConsole(hOutStd,TEXT("SIZE_MAXSHOW\n"),wcslen(TEXT("SIZE_MAXSHOW\n")),NULL,NULL);
		break;
	case SIZE_MINIMIZED:
		WriteConsole(hOutStd,TEXT("SIZE_MINIMIZED\n"),wcslen(TEXT("SIZE_MINIMIZED\n")),NULL,NULL);
		break;
	case SIZE_RESTORED:
		WriteConsole(hOutStd,TEXT("SIZE_RESTORED\n"),wcslen(TEXT("SIZE_RESTORED\n")),NULL,NULL);
		break;
	}

	int width = LOWORD(lParam);
	int height = HIWORD(lParam);
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("width:%d,height:%d\n"),width,height);
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

}

BOOL OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case SC_CLOSE:
		WriteConsole(hOutStd,TEXT("SC_CLOSE\n"),wcslen(TEXT("SC_CLOSE\n")),NULL,NULL);
		if(IDOK == MessageBox(hWnd,TEXT("�Ƿ����"),TEXT("��ʾ"),MB_OKCANCEL|MB_ICONWARNING))
		{
			return TRUE;
		}
		return FALSE;
	case SC_MAXIMIZE:
		WriteConsole(hOutStd,TEXT("SC_MAXIMIZE\n"),wcslen(TEXT("SC_MAXIMIZE\n")),NULL,NULL);
		break;
	case SC_MINIMIZE:
		WriteConsole(hOutStd,TEXT("SC_MINIMIZE\n"),wcslen(TEXT("SC_MINIMIZE\n")),NULL,NULL);
		break;
		/*
	case SC_MOVE:
		WriteConsole(hOutStd,TEXT("SC_MOVE\n"),wcslen(TEXT("SC_MOVE\n")),NULL,NULL);
		break;
		*/
	case SC_RESTORE:
		WriteConsole(hOutStd,TEXT("SC_RESTORE\n"),wcslen(TEXT("SC_RESTORE\n")),NULL,NULL);
		break;
	case 2001:
		WriteConsole(hOutStd,TEXT("new system menu\n"),wcslen(TEXT("new system menu\n")),NULL,NULL);
		break;
	}

	int horizontal = LOWORD(lParam);
	int vertical = HIWORD(lParam);
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("horizontal :%d,vertical :%d\n"),horizontal,vertical);
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
	
	return TRUE;
}

void OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps = {0};
	HDC hDC = BeginPaint(hWnd,&ps);

	DrawRect(hDC);
	DrawBmp(hDC);

	EndPaint(hWnd,&ps);
}

void OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//���а�����Ϣʱ,���Ƚ���ϵͳ��Ϣ����,
    //  Ȼ���������Ϣѭ����ȡ.

	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("keyDown\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	switch(wParam)
	{
	case VK_LEFT:
		rectLeft = rectLeft-10;
		rectRight= rectRight-10;
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case VK_RIGHT:
		rectLeft = rectLeft+10;
		rectRight= rectRight+10;
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case VK_UP:
		rectTop = rectTop-10;
		rectBottom= rectBottom-10;
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case VK_DOWN:
		rectTop = rectTop+10;
		rectBottom= rectBottom+10;
		InvalidateRect(hWnd,NULL,TRUE);
		break;

	}
}

void OnKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("keyUp\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnChar\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnSysKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnSysKeyDown\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnSysKeyUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnSysKeyUp\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	//memset(msg,0,sizeof(msg));
	//swprintf(msg,TEXT("OnLButtonDown\n"));
	//WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	switch(wParam)
	{
	case MK_CONTROL:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_CONTROL x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_LBUTTON:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_LBUTTON x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_MBUTTON:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_MBUTTON x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_RBUTTON:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_RBUTTON x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_SHIFT:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_SHIFT x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_XBUTTON1:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_XBUTTON1 x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case MK_XBUTTON2:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("OnLButtonDown MK_XBUTTON2 x:%d,y:%d\n"),x,y);
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	}
}

void OnLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnLButtonUp\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnRButtonDown\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnRButtonUp\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	//�Ҽ��˵�
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu,MF_STRING,3000,TEXT("����1"));
	AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	AppendMenu(hMenu,MF_STRING,3001,TEXT("����2"));

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	POINT p = {0};
	p.x = x;
	p.y = y;
	//ScreenToClient(hWnd,&p);
	ClientToScreen(hWnd,&p);
	//��ʾ�˵�
	TrackPopupMenu(hMenu,TPM_LEFTALIGN,p.x,p.y,0,hWnd,NULL);
	//ɾ���˵�
	DestroyMenu(hMenu);
}

/**
2.5.3 WM_RBUTTONUP��WM_CONTEXTMENU�Ա�
        1) ����ϵ��ͬ, WM_RBUTTONUP�ͻ�������
           WM_CONTEXTMENU��Ļ����
        2) ����WM_RBUTTONUP��Ϣ,
           ����WM_CONTEXTMENU��Ϣ
*/
void OnContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnContextMenu\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	//�Ҽ��˵�
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu,MF_STRING,3002,TEXT("����3"));
	AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	AppendMenu(hMenu,MF_STRING,3003,TEXT("����4"));

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	POINT p = {0};
	p.x = x;
	p.y = y;
	//ScreenToClient(hWnd,&p);
	//ClientToScreen(hWnd,&p);
	//��ʾ�˵�
	TrackPopupMenu(hMenu,TPM_LEFTALIGN,p.x,p.y,0,hWnd,NULL);
	//ɾ���˵�
	DestroyMenu(hMenu);
}

void OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnMouseMove\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnLButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnLButtonDblClk\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnRButtonDblClk\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
}

void OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};

	short dalta = HIWORD(wParam);

	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnMouseWheel dalta:%d\n"),dalta);
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	rectRight = rectRight+dalta;
	rectBottom= rectBottom+dalta;
	InvalidateRect(hWnd,NULL,TRUE);
}

void CreateConsole()
{
	AllocConsole();
	hOutStd = GetStdHandle(STD_OUTPUT_HANDLE);
	//TCHAR szText[] = __T("debug message....\n");
	TCHAR szText[] = TEXT("debug message....\n");
	WriteConsole(hOutStd,szText,wcslen(szText),NULL,NULL);
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	//�����˳�-��ֹGetMessageѭ��
	case WM_DESTROY:
		KillTimer(hWnd,1000);
		KillTimer(hWnd,1001);
		KillTimer(hWnd,1002);
		KillTimer(hWnd,g_timerId);
		PostQuitMessage(0); //����������
		return 0;
	case WM_COMMAND:
		OnCommand(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CREATE:
		OnCreate(hWnd, uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		OnSize(hWnd, uMsg, wParam, lParam);
		break;
	case WM_PAINT:
		OnPaint(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CHAR:
		OnChar(hWnd, uMsg, wParam, lParam);
		break;
	case WM_SYSKEYDOWN:
		OnSysKeyDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_SYSKEYUP:
		OnSysKeyUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(hWnd, uMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hWnd, uMsg, wParam, lParam);
		break;
	case WM_LBUTTONDBLCLK:
		//����ע��Ҫ���� CS_DBLCLKS ����
		OnLButtonDblClk(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONDBLCLK:
		//����ע��Ҫ���� CS_DBLCLKS ����
		OnRButtonDblClk(hWnd, uMsg, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(hWnd, uMsg, wParam, lParam);
		break;
	case WM_TIMER:
		OnTimer(hWnd, uMsg, wParam, lParam);
		break;
	case WM_INITMENUPOPUP:
		//���û�����˵�����ʾ�����˵�֮ǰ��ϵͳ
	    //   ���򴰿ڷ���WM_INITMENUPOPUP��Ϣ��
		OnInitialMenuPopup(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CONTEXTMENU:
		//�Ҽ��˵�
		OnContextMenu(hWnd, uMsg, wParam, lParam);
		break;
	case WM_SETCURSOR:
		if(TRUE == OnSetCursor(hWnd, uMsg, wParam, lParam))
		{
			return 0;
		}
		break;
	case WM_SYSCOMMAND:
		if(FALSE == OnSysCommand(hWnd, uMsg, wParam, lParam))
		{
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MyChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	switch(uMsg)
	{
	//�����˳�-��ֹGetMessageѭ��
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		OnCommand(hWnd, uMsg, wParam, lParam);
		return 0;
	}
	*/
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL RegisterWindow(LPCWSTR lpszClassName,
					WNDPROC proc,
					int nBrush)
{
	//ע�ᴰ����
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	//2.1.2 Ӧ�ó����ȫ�ִ����ࡣ
	//wcx.style =CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = proc;
	//���������Ϣ���ڴ�Ĵ�С
	//wcx.cbClsExtra = NULL;
	wcx.cbClsExtra = 8;
	//�ڴ��ڵ�������Ϣ������Լ���Ϣ.
	//wcx.cbWndExtra = NULL;
	wcx.cbWndExtra = 8;
	wcx.hInstance = hInst;
	wcx.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	wcx.hCursor = LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND));
	wcx.hbrBackground = HBRUSH(nBrush);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = lpszClassName;
	wcx.hIconSm = NULL;

	ATOM nAtom = RegisterClassEx(&wcx);

	if(nAtom == 0)
	{
		return FALSE;
	}
	return TRUE;
}

HWND CreateMainWindow(LPCWSTR lpszClassName,HMENU hMenu)
{

	//�ַ�����Դ
	TCHAR szText[64] = {0};
	LoadString(hInst,IDS_TITLE,szText,64);
	/*
	2.1 ������ķ���
  	  2.1.1 ϵͳȫ�ֵĴ�����.���簴ť("BUTTON")��
  	    �ı��༭��("EDIT")�ȡ�
  	  2.1.2 Ӧ�ó����ȫ�ִ����ࡣ������һ��Ӧ��
  	    ������EXE��DLL������ģ����ʹ�õĴ����ࡣ
  	  2.1.3 �ֲ������ࡣֻ���ڱ�ģ����ʹ�õ�
  	    �����ࡣ
	*/
	//2.1.1 ϵͳȫ�ֵĴ�����.���簴ť("BUTTON")��
  	//    �ı��༭��("EDIT")�ȡ�
	/*
	HWND hWnd = CreateWindowEx(
			NULL,
			L"Button",
			L"all",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,NULL,hMenu,hInstance,NULL);
			*/
	//2.1.2 Ӧ�ó����ȫ�ִ����ࡣ������һ��Ӧ��
  	//    ������EXE��DLL������ģ����ʹ�õĴ����ࡣ
	/*
	Ӧ�ó����ȫ�ִ�����,��Ҫ�ô���ʵ��
  	    ע�ᣬ��ע��ʱ��Ҫ����CS_GLOBALCLASS���塣
  	    ʵ�ַ�ʽ��
  	       WNDCLASS wc = {0};
  	       wc.style = CS_GLOBALCLASS|CS_HREDRAW..;
  	       ....
  	       RegisterClass( &wc );
		   */
	//2.1.3 �ֲ������ࡣ
	HWND hWnd = CreateWindowEx(
			NULL,
			lpszClassName,
			szText,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,NULL,hMenu,hInst,NULL);

	return hWnd;
}


HWND CreateChildWindow(LPCWSTR lpszClassName, HWND parentWnd)
{
	HWND hWndChild = CreateWindowEx(
			NULL,
			lpszClassName,
			L"all",
			WS_SYSMENU  //��WS_MINIMIZEBOX��WS_MAXIMIZEBOXһ��ʹ�òų��������С��ť
			| WS_VISIBLE | WS_POPUP 
			| WS_CHILD | 
			//WS_MINIMIZEBOX | 
			//WS_MAXIMIZEBOX | 
			WS_BORDER | WS_THICKFRAME | WS_CAPTION,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,parentWnd,NULL,hInst,NULL);

	return hWndChild;
}

HWND CreateMDIClient(HWND parentWnd)
{
	CLIENTCREATESTRUCT cs = {0};
	cs.idFirstChild = 1000;
	HWND hWndChild = CreateWindowEx(
			NULL,
			TEXT("MDIClient"),
			L"all",
			WS_VISIBLE 
			| WS_CHILD,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,parentWnd,NULL,hInst,&cs);

	return hWndChild;
}

HWND CreateMDIChildWindow(LPCWSTR lpszClassName, HWND parentWnd)
{
	HWND hWndChild = CreateWindowEx(
			WS_EX_MDICHILD,
			lpszClassName,
			L"all",
			WS_SYSMENU  //��WS_MINIMIZEBOX��WS_MAXIMIZEBOXһ��ʹ�òų��������С��ť
			| WS_VISIBLE | WS_POPUP 
			| WS_CHILD | 
			//WS_MINIMIZEBOX | 
			//WS_MAXIMIZEBOX | 
			WS_BORDER | WS_THICKFRAME | WS_CAPTION,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,parentWnd,NULL,hInst,NULL);

	return hWndChild;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	hInst = hInstance;
	CreateConsole();
	//ע�ᴰ��
	RegisterWindow( TEXT("mywnd"),MyWndProc, COLOR_BTNFACE+1 );
	RegisterWindow( TEXT("child"),MyChildProc, COLOR_WINDOW);

	//���ز˵�
	HMENU hMenu = LoadMenu(hInstance,MAKEINTRESOURCE(IDC_ALL));

	//��������
	HWND hWnd = CreateMainWindow(TEXT("mywnd"),hMenu);

	//����mdi�ͻ���
	HWND hWndMDIClient = CreateMDIClient(hWnd);
	MoveWindow(hWndMDIClient, 100, 100, 500, 500, TRUE);
	//�����Ӵ���
	//HWND hWndChild = CreateChildWindow(TEXT("child"),hWnd);
	HWND hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	//MoveWindow(hWndChild, 500, 500, 200, 200, TRUE);

	//���ڵ���ʾ��ˢ��
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//��Ϣѭ��
	MSG msg = {0};
	BOOL bRet;

	//������ټ�
	HACCEL hAccel = LoadAccelerators(hInst,MAKEINTRESOURCE(IDC_ALL));
	//GetMessage��������
	//����ֵBOOL���ɹ���ȡ��Ϣ������TRUE������
	//			  ����ȡ��WM_QUIT��Ϣʱ������FALSE��
	//			����ʹ��PostQuitMessage�򴰿ڷ���WM_QUIT��Ϣ
	while(bRet = GetMessage(&msg, hWnd, NULL, NULL) != 0)
	{
		if(!TranslateAccelerator(hWnd,hAccel,&msg))
		{
			//���ǽ�������Ϣת�����ַ���Ϣ��
			//		1 ���ȼ���Ƿ��Ǽ��̰�����Ϣ    	
			//		2 ��������ǰ�����Ϣ�������ݰ���������
			//		  һ���ַ���Ϣ������һ��GetMessageִ��
			//		  ʱ�����յ������Ϣ��
			//	  3 ���δ���ְ�����Ϣ�������κδ���
			TranslateMessage(&msg);

			/*������Ϣ�����ڴ��ھ�����ҵ��������
					�Ĵ��ڴ�����, ���ô�������������Ϣ
					����
					���MSG�У�HWND���ھ��Ϊ�գ�
					DispatchMessage�����κδ���*/
			DispatchMessage(&msg);
		}
	}

	return 0;

}




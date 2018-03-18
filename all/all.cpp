// all.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "all.h"
#include "stdio.h"

#define MAX_LOADSTRING 100
//使用时,要增加TCHAR.H头文件支持,使用
//			  _UNICODE 宏开关进行编译
			   
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
// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HANDLE hOutStd = NULL;
BOOL isPaintRect;
UINT_PTR g_timerId = NULL;

int rectLeft = 600;
int rectTop = 600;
int rectRight = 700;
int rectBottom = 700;

HWND hDlg = NULL;
HWND hWndMDIClient = NULL;

BOOL CALLBACK GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    { 
        case WM_INITDIALOG: 
			MessageBox(hwndDlg, TEXT("WM_INITDIALOG"), TEXT("dialog"), NULL);
            return TRUE; 
 
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                    MessageBox(hwndDlg, TEXT("IDOK"), TEXT("dialog"), NULL);
                    return TRUE; 
 
                case IDCANCEL: 
                    DestroyWindow(hwndDlg); 
                    hDlg = NULL; 
                    return TRUE; 
            } 
    } 
    return FALSE; 
} 


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
	//导入位图资源
	HBITMAP hBitMap = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));

	//得到资源信息
	BITMAP bmpInfo = {0};
	GetObject(hBitMap,sizeof(BITMAP),&bmpInfo);

	//创建装位图的dc
	HDC hMapDC = CreateCompatibleDC(hDC);

	//将位图装入新dc
	HBITMAP hBitMapOld = (HBITMAP)SelectObject(hMapDC, hBitMap);

	//绘制位图
	BitBlt(hDC,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,hMapDC ,0,0,SRCCOPY);
	//拉伸绘制
	StretchBlt( hDC, 0, 0, bmpInfo.bmWidth+100, bmpInfo.bmHeight+100, 
		hMapDC, 0, 0, bmpInfo.bmWidth, 
		bmpInfo.bmHeight, SRCCOPY );

	//还原DC
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
		//不在客户区，返回FALSE，
		//让DefWindowProc处理
		return FALSE;
	}
	RECT rcClient = {0};
	GetClientRect(hWnd,&rcClient);

	POINT ptPos = {0};
	GetCursorPos(&ptPos);
	ScreenToClient(hWnd,&ptPos);

	//根据位置加载光标
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
	// 设置光标
	SetCursor( hCursor );
	return TRUE;
}

void OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nCmdId = LOWORD(wParam);
	TCHAR msg[64] = {0};
	switch(nCmdId)
	{
	case ID_32777:
		//MessageBox(hWnd, TEXT("ID_32777"), TEXT("command"), NULL);
		if (!IsWindow(hDlg))
		{
			hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndMDIClient, GoToProc);
			ShowWindow(hDlg, SW_SHOW);
		}
		break;
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
		swprintf(msg,TEXT("帮助...\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
	case 3000:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("测试1\n"));
		WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);
		break;
		
	case 3001:
		memset(msg,0,sizeof(msg));
		swprintf(msg,TEXT("测试2\n"));
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
	//窗口创建消息，是在窗口创建后，
    //     窗口处理函数收到第一条消息。
    //     可以在这个消息内做数据初始化/创建子窗口等。
	LPCREATESTRUCT pCreateStruct = LPCREATESTRUCT(lParam);
	WriteConsole(hOutStd,TEXT("OnCreate begin\n"),wcslen(TEXT("OnCreate begin\n")),NULL,NULL);
	WriteConsole(hOutStd,pCreateStruct->lpszClass,wcslen(pCreateStruct->lpszClass),NULL,NULL);
	WriteConsole(hOutStd,TEXT("OnCreate end\n"),wcslen(TEXT("OnCreate end\n")),NULL,NULL);

	//初始化数据
	isPaintRect = TRUE;

	//使用窗口处理函数，初始化两个定时器
	SetTimer(hWnd,1000,3*1000,NULL);
	SetTimer(hWnd,1001,5*1000,NULL);
	//使用窗口处理函数，不指定定时器id，初始化定时器
	g_timerId = SetTimer(hWnd,NULL,7*1000,NULL);

	//使用定时器处理函数，初始化定时器
	SetTimer(NULL,1002,1*1000,TimerProc1);

	//以函数方式创建菜单
	/*
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreatePopupMenu();
	AppendMenu(hFileMenu,MF_STRING | MF_CHECKED,1001,TEXT("新建(&n)"));

	AppendMenu(hMenu,MF_STRING | MF_POPUP,(UINT)hFileMenu,TEXT("文件(&F)"));

	SetMenu(hWnd,hMenu);
	*/

	//增加系统菜单
	HMENU hMenu = GetSystemMenu(hWnd,FALSE);

	InsertMenu(hMenu,3,MF_STRING | MF_BYPOSITION,2001,TEXT("新建系统"));
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
		if(IDOK == MessageBox(hWnd,TEXT("是否存盘"),TEXT("提示"),MB_OKCANCEL|MB_ICONWARNING))
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
	//当有按键消息时,首先进入系统消息队列,
    //  然后别程序的消息循环获取.

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

	//右键菜单
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu,MF_STRING,3000,TEXT("测试1"));
	AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	AppendMenu(hMenu,MF_STRING,3001,TEXT("测试2"));

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	POINT p = {0};
	p.x = x;
	p.y = y;
	//ScreenToClient(hWnd,&p);
	ClientToScreen(hWnd,&p);
	//显示菜单
	TrackPopupMenu(hMenu,TPM_LEFTALIGN,p.x,p.y,0,hWnd,NULL);
	//删除菜单
	DestroyMenu(hMenu);
}

/**
2.5.3 WM_RBUTTONUP和WM_CONTEXTMENU对比
        1) 坐标系不同, WM_RBUTTONUP客户区坐标
           WM_CONTEXTMENU屏幕坐标
        2) 先有WM_RBUTTONUP消息,
           后有WM_CONTEXTMENU消息
*/
void OnContextMenu(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR msg[64] = {0};
	memset(msg,0,sizeof(msg));
	swprintf(msg,TEXT("OnContextMenu\n"));
	WriteConsole(hOutStd,msg,wcslen(msg),NULL,NULL);

	//右键菜单
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu,MF_STRING,3002,TEXT("测试3"));
	AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
	AppendMenu(hMenu,MF_STRING,3003,TEXT("测试4"));

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	POINT p = {0};
	p.x = x;
	p.y = y;
	//ScreenToClient(hWnd,&p);
	//ClientToScreen(hWnd,&p);
	//显示菜单
	TrackPopupMenu(hMenu,TPM_LEFTALIGN,p.x,p.y,0,hWnd,NULL);
	//删除菜单
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
	//窗口退出-终止GetMessage循环
	case WM_DESTROY:
		KillTimer(hWnd,1000);
		KillTimer(hWnd,1001);
		KillTimer(hWnd,1002);
		KillTimer(hWnd,g_timerId);
		PostQuitMessage(0); //非阻塞函数
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
		//窗口注册要增加 CS_DBLCLKS 类型
		OnLButtonDblClk(hWnd, uMsg, wParam, lParam);
		break;
	case WM_RBUTTONDBLCLK:
		//窗口注册要增加 CS_DBLCLKS 类型
		OnRButtonDblClk(hWnd, uMsg, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(hWnd, uMsg, wParam, lParam);
		break;
	case WM_TIMER:
		OnTimer(hWnd, uMsg, wParam, lParam);
		break;
	case WM_INITMENUPOPUP:
		//当用户点击菜单，显示弹出菜单之前，系统
	    //   会向窗口发送WM_INITMENUPOPUP消息。
		OnInitialMenuPopup(hWnd, uMsg, wParam, lParam);
		break;
	case WM_CONTEXTMENU:
		//右键菜单
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
	//窗口退出-终止GetMessage循环
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
	//注册窗口类
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	//2.1.2 应用程序的全局窗口类。
	//wcx.style =CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = proc;
	//用于添加信息的内存的大小
	//wcx.cbClsExtra = NULL;
	wcx.cbClsExtra = 8;
	//在窗口的数据信息中添加自己信息.
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

	//字符串资源
	TCHAR szText[64] = {0};
	LoadString(hInst,IDS_TITLE,szText,64);
	/*
	2.1 窗口类的分类
  	  2.1.1 系统全局的窗口类.比如按钮("BUTTON")、
  	    文本编辑框("EDIT")等。
  	  2.1.2 应用程序的全局窗口类。可以在一个应用
  	    程序中EXE、DLL等所有模块中使用的窗口类。
  	  2.1.3 局部窗口类。只能在本模块中使用的
  	    窗口类。
	*/
	//2.1.1 系统全局的窗口类.比如按钮("BUTTON")、
  	//    文本编辑框("EDIT")等。
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
	//2.1.2 应用程序的全局窗口类。可以在一个应用
  	//    程序中EXE、DLL等所有模块中使用的窗口类。
	/*
	应用程序的全局窗口类,需要用代码实现
  	    注册，在注册时需要增加CS_GLOBALCLASS定义。
  	    实现方式：
  	       WNDCLASS wc = {0};
  	       wc.style = CS_GLOBALCLASS|CS_HREDRAW..;
  	       ....
  	       RegisterClass( &wc );
		   */
	//2.1.3 局部窗口类。
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
			WS_SYSMENU  //与WS_MINIMIZEBOX，WS_MAXIMIZEBOX一起使用才出现最大最小按钮
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
			WS_SYSMENU  //与WS_MINIMIZEBOX，WS_MAXIMIZEBOX一起使用才出现最大最小按钮
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
	//注册窗口
	RegisterWindow( TEXT("mywnd"),MyWndProc, COLOR_BTNFACE+1 );
	RegisterWindow( TEXT("child"),MyChildProc, COLOR_WINDOW);

	//加载菜单
	HMENU hMenu = LoadMenu(hInstance,MAKEINTRESOURCE(IDC_ALL));

	//建立窗口
	HWND hWnd = CreateMainWindow(TEXT("mywnd"),hMenu);

	//建立mdi客户区
	hWndMDIClient = CreateMDIClient(hWnd);
	MoveWindow(hWndMDIClient, 100, 100, 500, 500, TRUE);
	//建立子窗口
	//HWND hWndChild = CreateChildWindow(TEXT("child"),hWnd);
	HWND hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	hWndChild = CreateMDIChildWindow(TEXT("child"),hWndMDIClient);
	//MoveWindow(hWndChild, 500, 500, 200, 200, TRUE);

	//窗口的显示和刷新
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//消息循环
	MSG msg = {0};
	BOOL bRet;

	//处理加速键
	HACCEL hAccel = LoadAccelerators(hInst,MAKEINTRESOURCE(IDC_ALL));
	//GetMessage阻塞函数
	//返回值BOOL：成功获取消息，返回TRUE，但是
	//			  当获取到WM_QUIT消息时，返回FALSE。
	//			可以使用PostQuitMessage向窗口发送WM_QUIT消息
	while(bRet = GetMessage(&msg, hWnd, NULL, NULL) != 0)
	{
		if(!TranslateAccelerator(hWnd,hAccel,&msg))
		{
			if (!IsWindow(hDlg) || !IsDialogMessage(hDlg, &msg)) 
			{
			//就是将键盘消息转换成字符消息。
			//		1 首先检查是否是键盘按键消息    	
			//		2 如果发现是按键消息，将根据按键，产生
			//		  一个字符消息，在下一个GetMessage执行
			//		  时，会收到这个消息。
			//	  3 如果未发现按键消息，不做任何处理。
			TranslateMessage(&msg);

			/*根据消息数据内窗口句柄，找到这个窗口
					的窗口处理函数, 调用处理函数，进行消息
					处理。
					如果MSG中，HWND窗口句柄为空，
					DispatchMessage不做任何处理。*/
			DispatchMessage(&msg);
			}
		}
	}

	return 0;

}




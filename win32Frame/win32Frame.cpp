// win32Frame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "win32Frame.h"

//===========建立控制台窗口============
HANDLE hOut = NULL;
HINSTANCE hInst;
HWND hChild = NULL;
HWND hBitMapChild = NULL;
HWND hDlgWnd = NULL;

int nRectLeft = 10;
int nRectTop = 10;
int nRectRight = 50;
int nRectBottom = 50;
int nEach = 5;
TCHAR szText[128];
TCHAR szMouseWheelMsg[128];
TCHAR szSystemTime[64];
TCHAR szTimer1002[64];
int nX = 0;
int nY = 0;

BOOL CALLBACK MyDialogProc(  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message  
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		MessageBox(hwndDlg, TEXT("WM_INITDIALOG"), TEXT("Dialog"), NULL);
		break;
	case WM_CREATE:
		MessageBox(hwndDlg, TEXT("WM_CREATE"), TEXT("Dialog"), NULL);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwndDlg, wParam); 
            return TRUE;
		}
	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_CLOSE:
				EndDialog( hwndDlg, 0 );
				break;
			}
		}
		break;
	}
	return 0;
}

BOOL CALLBACK DeleteItemProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    { 
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                    EndDialog(hwndDlg, wParam); 
                    return TRUE; 
 
                    // Fall through. 
 
                case IDCANCEL: 
                    EndDialog(hwndDlg, wParam); 
                    return TRUE; 
            } 
    } 
    return FALSE; 

}

int CALLBACK EnumFontFamProc(
  ENUMLOGFONT *lpelf,    // logical-font data
  NEWTEXTMETRIC *lpntm,  // physical-font data
  DWORD FontType,        // type of font
  LPARAM lParam          // application-defined data
)
{
	//MessageBox(hChild, lpelf->elfLogFont.lfFaceName, TEXT("字体"), NULL);
	WriteConsole(hOut, lpelf->elfLogFont.lfFaceName, wcslen(lpelf->elfLogFont.lfFaceName), NULL,NULL);
	WriteConsole(hOut, TEXT("\n\r"), wcslen(TEXT("\n\r")), NULL,NULL);
	int far * aiFontCount = (int far *) lParam; 
 
    // Record the number of raster, TrueType, and vector 
    // fonts in the font-count array. 
 
    if (FontType & RASTER_FONTTYPE) 
        aiFontCount[0]++; 
    else if (FontType & TRUETYPE_FONTTYPE) 
        aiFontCount[2]++; 
    else 
        aiFontCount[1]++; 
 
    if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2]) 
        return TRUE; 
    else 
        return FALSE; 
 
    UNREFERENCED_PARAMETER( lpelf ); 
    UNREFERENCED_PARAMETER( lpntm ); 

	return 0;
}



void CreateConsole()
{
	AllocConsole();
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR msg[] = TEXT("debug message...\n");
	WriteConsole(hOut,msg,wcslen(msg),0,0);
}

void Multi2Wide(HWND hWnd)
{
	char* szText = "Multi2Wide";
	TCHAR msg[64] = {0};
	int nlen = MultiByteToWideChar(CP_ACP,NULL,szText,strlen(szText),NULL,0);

	WCHAR* pwszText = (WCHAR*)malloc((nlen+1) * sizeof(WCHAR));
	memset(pwszText,0,(nlen+1) * sizeof(WCHAR));

	MultiByteToWideChar(CP_ACP,NULL,szText,strlen(szText),pwszText,nlen);

	swprintf(msg,TEXT("len of char:%d\n"),nlen);
	WriteConsole(hOut,msg,wcslen(msg),0,0);
	WriteConsole(hOut,pwszText,wcslen(pwszText),0,0);
	MessageBoxW(hWnd, pwszText,L"Wide",0);
}


void Wide2Multi(HWND hWnd)
{
	WCHAR* pwszText = L"Wide2Multi";
	TCHAR msg[64] = {0};

	int nlen = WideCharToMultiByte(CP_ACP,NULL,pwszText,wcslen(pwszText),NULL,0,NULL,NULL);
	char* szText = (char*)malloc(nlen+1);
	memset(szText,0,nlen+1);
	WideCharToMultiByte(CP_ACP,NULL,pwszText,wcslen(pwszText),szText,nlen,NULL,NULL);

	swprintf(msg,TEXT("len of char:%d\n"),nlen);
	WriteConsole(hOut,msg,wcslen(msg),0,0);
	MessageBoxA(hWnd,szText,"wide",0);
}

/**
1.按键消息发送到主窗口
2.鼠标消息发送到子窗口
*/
LRESULT CALLBACK MyWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	switch(uMsg)
	{
	case WM_RBUTTONUP:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_POPUP, 3001, TEXT("主窗口测试1"));

			POINT point = {0};
			point.x = nX;
			point.y = nY;
			ClientToScreen(hwnd, &point); 

			TrackPopupMenu(hMenu, TPM_RIGHTALIGN, point.x, point.y, NULL, hwnd, NULL);

			break;
		}
	case WM_CREATE:
		{
		/*HMENU hDrawMenu = CreateMenu();
		HMENU hDrawCirclePopMenu = CreatePopupMenu();

		AppendMenu(hDrawCirclePopMenu, MF_STRING | MF_CHECKED, 1003, TEXT("画圆(&C)"));
		AppendMenu(hDrawCirclePopMenu, MF_STRING | MF_CHECKED, 1004, TEXT("画弧(&A)"));
		AppendMenu(hDrawMenu, MF_STRING | MF_POPUP, (UINT_PTR)hDrawCirclePopMenu, TEXT("画图(&D)"));

		SetMenu(hwnd, hDrawMenu);*/

		//获取系统菜单
		HMENU hMenu = GetSystemMenu(hwnd,FALSE);
		InsertMenu(hMenu, 3, MF_BYPOSITION | MF_STRING, 2001, TEXT("新建系统"));
		}
		break;
	case WM_INITMENUPOPUP:
		{
		HMENU hMenu = GetMenu(hwnd);
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
		break;
		}
	case WM_SYSCOMMAND:
		if(2001 == wParam)
		{
			MessageBox(hwnd,TEXT("新建系统"), TEXT("系统命令"), NULL);
		}
		//break;
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	case WM_COMMAND:
		if(ID_32776 == LOWORD(wParam))
		{
			//MessageBox(hwnd, TEXT("对话框"), TEXT("对话框"),NULL);
			hDlgWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, MyDialogProc);
			ShowWindow(hDlgWnd, SW_SHOW);
		}
		if(IDM_ABOUT == LOWORD(wParam))
		{
			if(DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUTBOX) ,hwnd, DeleteItemProc) == IDOK)
			{
				MessageBox(hwnd, TEXT("关于对话框OK"), TEXT("关于对话框"),NULL);
			}
		}
		if(ID_32775 == LOWORD(wParam))
		{
			//MessageBox(hwnd, TEXT("列举字体"), TEXT("字体"), NULL);
			PAINTSTRUCT ps = {0};
			HDC hDC = BeginPaint(hwnd, &ps);
			//EnumFontFamilies(hDC, NULL, (FONTENUMPROC)EnumFontFamProc, NULL);
			UINT uAlignPrev; 
			int aFontCount[] = { 0, 0, 0 }; 
			TCHAR szCount[8]; 
 
			EnumFontFamilies(hDC, (LPCTSTR) NULL, 
				(FONTENUMPROC) EnumFontFamProc, (LPARAM) aFontCount); 
 
			uAlignPrev = SetTextAlign(hDC, TA_UPDATECP); 
 
			MoveToEx(hDC, 10, 50, (LPPOINT)NULL); 
			TextOut(hDC, 0, 0, TEXT("Number of raster fonts: "), 24); 
			_itow(aFontCount[0], szCount, 10); 
			TextOut(hDC, 0, 0, szCount, wcslen(szCount)); 
 
			MoveToEx(hDC, 10, 75, (LPPOINT)NULL); 
			TextOut(hDC, 0, 0, TEXT("Number of vector fonts: "), 24); 
			_itow(aFontCount[1], szCount, 10); 
			TextOut(hDC, 0, 0, szCount, wcslen(szCount)); 
 
			MoveToEx(hDC, 10, 100, (LPPOINT)NULL); 
			TextOut(hDC, 0, 0, TEXT("Number of TrueType fonts: "), 26); 
			_itow(aFontCount[2], szCount, 10); 
			TextOut(hDC, 0, 0, szCount, wcslen(szCount)); 
 
			SetTextAlign(hDC, uAlignPrev); 


			EndPaint(hwnd, &ps);
		}
		if(1003 == LOWORD(wParam))
		{
			MessageBox(hwnd,TEXT("画圆"),TEXT("画图"), NULL);
			HMENU hMenu = GetMenu(hwnd);
			UINT state = GetMenuState(hMenu, 1003, MF_BYCOMMAND);
			if(!(state & MF_CHECKED))
			{
				CheckMenuItem(hMenu, 1003, MF_CHECKED | MF_BYCOMMAND);
			}
			else
			{
				CheckMenuItem(hMenu, 1003, MF_UNCHECKED | MF_BYCOMMAND);
			}
		}
		if(1004 == LOWORD(wParam))
		{
			MessageBox(hwnd,TEXT("画弧"),TEXT("画图"), NULL);
			HMENU hMenu = GetMenu(hwnd);
			HMENU hSubMenu = GetSubMenu(hMenu, 0);
			//按位置取菜单状态
			UINT state = GetMenuState(hSubMenu, 1, MF_BYPOSITION);
			if(!(state & MF_CHECKED))
			{
				CheckMenuItem(hSubMenu, 1, MF_CHECKED | MF_BYPOSITION);
			}
			else
			{
				CheckMenuItem(hSubMenu, 1, MF_UNCHECKED | MF_BYPOSITION);
			}
			/*UINT state = GetMenuState(hMenu, 1004, MF_BYCOMMAND);
			if(!(state & MF_CHECKED))
			{
				CheckMenuItem(hMenu, 1004, MF_CHECKED | MF_BYCOMMAND);
			}
			else
			{
				CheckMenuItem(hMenu, 1004, MF_UNCHECKED | MF_BYCOMMAND);
			}*/
		}
		if(3001 == LOWORD(wParam))
		{
			MessageBox(hwnd,szText,TEXT("主窗口测试1消息"),NULL);
		}
		if(ID_32771 == LOWORD(wParam))
		{
			MessageBox(hwnd,TEXT("画弧"),TEXT("主窗口消息"),NULL);
		}
		if(ID_32772 == LOWORD(wParam))
		{
			MessageBox(hwnd,TEXT("画圆"),TEXT("主窗口消息"),NULL);
		}
		break;
	case WM_DESTROY:
		KillTimer(hChild, 1001);
		KillTimer(hChild, 1002);
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if(VK_LEFT == wParam)
		{
			//MessageBox(hwnd,TEXT("VK_LEFT"),TEXT("keyup_message"),NULL);
			if(nRectLeft != 0)
			{
				nRectLeft = nRectLeft - nEach;
				nRectRight = nRectRight - nEach;
				InvalidateRect(hChild, NULL, TRUE);
			}
		}
		if(VK_RIGHT == wParam)
		{
			//MessageBox(hwnd,TEXT("VK_RIGHT"),TEXT("keyup_message"),NULL);
			nRectLeft = nRectLeft + nEach;
			nRectRight = nRectRight + nEach;
			InvalidateRect(hChild, NULL, TRUE);
		}
		if(VK_UP == wParam)
		{
			//MessageBox(hwnd,TEXT("VK_UP"),TEXT("keyup_message"),NULL);
			nRectTop = nRectTop - nEach;
			nRectBottom = nRectBottom - nEach;
			InvalidateRect(hChild, NULL, TRUE);
		}
		if(VK_DOWN == wParam)
		{
			//MessageBox(hwnd,TEXT("VK_DOWN"),TEXT("keyup_message"),NULL);
			nRectTop = nRectTop + nEach;
			nRectBottom = nRectBottom + nEach;
			InvalidateRect(hChild, NULL, TRUE);
		}
		break;
	case WM_CHAR:
		//按键d被按下
		if(wcscmp(TEXT("d"),(TCHAR*)&wParam) == 0)
		{
			//MessageBox(hwnd,TEXT("d"),TEXT("d"),NULL);
		}
		break;

	case WM_LBUTTONDOWN:
		//鼠标左键按下
		nX = LOWORD(lParam);
		nY = HIWORD(lParam);
		memset(szText,0,sizeof(szText));
		wsprintf(szText,TEXT("x:%d,y:%d"),nX, nY);
		MessageBox(hwnd,szText,TEXT("主窗口鼠标消息"),NULL);
		InvalidateRect(hChild, NULL, TRUE);
		break;
	case WM_MOUSEWHEEL:
		
		if(MK_CONTROL == LOWORD(wParam))
		{
			short dalta = HIWORD(wParam);
			dalta = dalta /	WHEEL_DELTA;
			memset(szMouseWheelMsg,0,sizeof(szMouseWheelMsg));
			wsprintf(szMouseWheelMsg,TEXT("主窗口MouseWheel:%d\n"),dalta);
			//WriteConsole(hOut, szMouseWheelMsg, wcslen(szMouseWheelMsg), NULL,NULL);
			nRectRight = nRectRight+dalta;
			nRectBottom= nRectBottom+dalta;
			InvalidateRect(hChild,NULL,TRUE);
		}
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}


LRESULT CALLBACK MyChildWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	switch(uMsg)
	{
	/*case WM_RBUTTONUP:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_POPUP, 3001, TEXT("子窗口测试1"));

			POINT point = {0};
			point.x = nX;
			point.y = nY;
			ClientToScreen(hwnd, &point); 

			TrackPopupMenu(hMenu, TPM_RIGHTALIGN, point.x, point.y, NULL, hwnd, NULL);

			break;
		}*/
	case WM_CONTEXTMENU:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING | MF_POPUP, 3001, TEXT("子窗口context测试1"));

			POINT point = {0};
			point.x = nX;
			point.y = nY;
			//ClientToScreen(hwnd, &point); 

			TrackPopupMenu(hMenu, TPM_RIGHTALIGN, point.x, point.y, NULL, hwnd, NULL);

			break;
		}
	case WM_COMMAND:
		if(3001 == LOWORD(wParam))
		{
			MessageBox(hwnd,TEXT("子窗口测试1"),TEXT("子窗口测试1消息"),NULL);
		}
		break;
	case WM_TIMER:
		memset(szTimer1002,0,sizeof(szTimer1002));
		wsprintf(szTimer1002,TEXT("TimerID:%d"),
			wParam);

		//WriteConsole(hOut,szSystemTime, wcslen(szSystemTime),NULL,NULL);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		{
		//memset(szText,0,sizeof(szText));

		PAINTSTRUCT PaintStruct = {0};
		HDC hDC = BeginPaint(hwnd, &PaintStruct);

		//HPEN hPen = CreatePen(PS_SOLID, 10,	RGB(255,0,0));
		HPEN hPen = CreatePen(PS_SOLID, 1,	RGB(255,0,0));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);


		//创建实心画刷
		HBRUSH hBrush = CreateSolidBrush(RGB(0,128,255));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);


		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(0,0,255));

		/*wcsncpy(szText, TEXT("Hello, Windows!"), 
			wcslen(TEXT("Hello, Windows!")));*/

		TextOut(hDC, 50,50, szText, wcslen(szText)); 
		TextOut(hDC, 100, 100, szSystemTime, wcslen(szSystemTime));
		TextOut(hDC, 200, 200, szTimer1002, wcslen(szTimer1002));

		//画矩形
		Rectangle(hDC, nRectLeft,nRectTop,nRectRight,nRectBottom);

		//画弧
		Rectangle(hDC, 100,100,300,300);
		SetArcDirection(hDC, AD_CLOCKWISE);
		Arc(hDC, 100,100, 300,300, 100,200,300,200);

		//画弧
		MoveToEx(hDC,400,100, NULL);
		AngleArc(hDC, 400,100,50,30,300);
		LineTo(hDC, 400, 100); 

		//折线
		POINT points[4];
		points[0].x = 560;
		points[0].y = 20;
		points[1].x = 580;
		points[1].y = 10;
		points[2].x = 600;
		points[2].y = 20;
		points[3].x = 620;
		points[3].y = 10;
		Polyline(hDC, points, 4);

		POINT ppoints[7];
		ppoints[0].x = 560;
		ppoints[0].y = 100;
		ppoints[1].x = 580;
		ppoints[1].y = 50;
		ppoints[2].x = 600;
		ppoints[2].y = 100;
		ppoints[3].x = 620;
		ppoints[3].y = 50;
		ppoints[4].x = 640;
		ppoints[4].y = 100;
		ppoints[5].x = 660;
		ppoints[5].y = 50;
		ppoints[6].x = 680;
		ppoints[6].y = 100;

		DWORD nGroup[] = {4,3};
		PolyPolyline(hDC, ppoints, nGroup, 2);

		POINT pBizerPoints[4];
		pBizerPoints[0].x = 560;
		pBizerPoints[0].y = 150;
		pBizerPoints[1].x = 580;
		pBizerPoints[1].y = 200;
		pBizerPoints[2].x = 600;
		pBizerPoints[2].y = 150;
		pBizerPoints[3].x = 620;
		pBizerPoints[3].y = 200;

		PolyBezier(hDC, pBizerPoints, 4);

		POINT pPolyPoints[4];
		pPolyPoints[0].x = 560;
		pPolyPoints[0].y = 300;
		pPolyPoints[1].x = 580;
		pPolyPoints[1].y = 350;
		pPolyPoints[2].x = 600;
		pPolyPoints[2].y = 300;
		pPolyPoints[3].x = 620;
		pPolyPoints[3].y = 350;

		MoveToEx(hDC,560,300,NULL);
		BYTE lpbTypes[4];
		lpbTypes[0] = PT_LINETO  ;
		lpbTypes[1] = PT_LINETO  ;
		lpbTypes[2] = PT_LINETO  ;
		lpbTypes[3] = PT_LINETO  ;
		PolyDraw(hDC, pPolyPoints, lpbTypes, 4);

		//圆角矩形
		RoundRect(hDC,200,10,250,60,30,30);

		//椭圆
		Ellipse(hDC, 30,350, 60,400);

		//饼
		Pie(hDC, 30, 410, 100, 450, 30, 410, 100, 450);

		//弦
		Chord(hDC, 150,410,250,450, 150,410, 250, 450);

		//多边形
		POINT pPolygonPoint[6]= {0};
		pPolygonPoint[0].x = 368;
		pPolygonPoint[0].y = 211;
		pPolygonPoint[1].x = 415;
		pPolygonPoint[1].y = 710;
		pPolygonPoint[2].x = 470;
		pPolygonPoint[2].y = 169;
		pPolygonPoint[3].x = 598;
		pPolygonPoint[3].y = 422;
		pPolygonPoint[4].x = 554;
		pPolygonPoint[4].y = 440;
		pPolygonPoint[5].x = 366;
		pPolygonPoint[5].y = 237;

		Polygon(hDC,pPolygonPoint,6);

		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

		DeleteObject(hBrush);
		DeleteObject(hPen);

		EndPaint(hwnd,&PaintStruct);
		//return 0;
		break;
		}
	case WM_KEYUP:
		//消息没有发到子窗口
		MessageBox(hwnd,TEXT("keyup"),TEXT("keyup_message"),NULL);
		break;
	case WM_LBUTTONDOWN:
		{
		int nIconWidth = GetSystemMetrics(SM_CXSMICON);
		int nIconHeight = GetSystemMetrics(SM_CYSMICON);
		//鼠标左键按下
		nX = LOWORD(lParam);
		nY = HIWORD(lParam);
		POINT point = {0};
		point.x = nX;
		point.y = nY;
		ClientToScreen(hwnd,&point);

		memset(szText,0,sizeof(szText));
		wsprintf(szText,TEXT("客户区坐标-[x:%d,y:%d],屏幕坐标-[x:%d,y:%d],图标-[w:%d,h:%d]"),nX, nY, point.x, point.y,nIconWidth,nIconHeight);
		//MessageBox(hwnd,szText,TEXT("子窗口鼠标消息"),NULL);
		InvalidateRect(hChild, NULL, TRUE);
		break;
		}
	case WM_MOUSEWHEEL:
		if(MK_CONTROL == LOWORD(wParam))
		{
			memset(szMouseWheelMsg,0,sizeof(szMouseWheelMsg));
			wsprintf(szMouseWheelMsg,TEXT("子窗口MouseWheel:%d\n"),HIWORD(wParam));
			WriteConsole(hOut, szMouseWheelMsg, wcslen(szMouseWheelMsg), NULL,NULL);
		}
		break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}


LRESULT CALLBACK BitMapWndProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
		int nIconWidth = GetSystemMetrics(SM_CXSMICON);
		int nIconHeight = GetSystemMetrics(SM_CYSMICON);
		//鼠标左键按下
		nX = LOWORD(lParam);
		nY = HIWORD(lParam);
		POINT point = {0};
		point.x = nX;
		point.y = nY;
		ClientToScreen(hwnd,&point);

		memset(szText,0,sizeof(szText));
		wsprintf(szText,TEXT("客户区坐标-[x:%d,y:%d],屏幕坐标-[x:%d,y:%d],图标-[w:%d,h:%d]"),nX, nY, point.x, point.y,nIconWidth,nIconHeight);
		//MessageBox(hwnd,szText,TEXT("子窗口鼠标消息"),NULL);
		InvalidateRect(hBitMapChild, NULL, TRUE);
		break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps = {0};
			HDC hDC = BeginPaint(hwnd, &ps);

			//SetWindowOrgEx(hDC, 100, 100, NULL);
			//加载位图
			HBITMAP hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));

			//获得位图信息
			BITMAP BitMap = {0};
			GetObject(hBitMap, sizeof(BITMAP), &BitMap);

			//得到画位图的设备
			HDC hBitMapDC = CreateCompatibleDC(hDC);

			//将位图绘制新的DC
			HBITMAP hOldBitMap = (HBITMAP)SelectObject(hBitMapDC, hBitMap);

			//切换视口坐标
			//SetViewportOrgEx(hDC, 100, 100, NULL);

			//切换窗口坐标
			//SetWindowOrgEx(hDC, 100, 100, NULL);

			//绘制到DC
			BitBlt(hDC, 100, 100, BitMap.bmWidth, BitMap.bmHeight, hBitMapDC, 0, 0, SRCCOPY);
			//StretchBlt(hDC, 100, 100, BitMap.bmWidth+100, BitMap.bmHeight+100, hBitMapDC, 0, 0, BitMap.bmWidth,BitMap.bmHeight,SRCCOPY);
			//StretchBlt(hDC, 0, 0, BitMap.bmWidth+100, BitMap.bmHeight+100, hBitMapDC, 0, 0, BitMap.bmWidth,BitMap.bmHeight,SRCCOPY);

			SelectObject(hBitMapDC, hOldBitMap);
			DeleteObject(hOldBitMap);
			DeleteDC(hBitMapDC);

			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(0,0,255));

			RECT nRect = {0};
			nRect.top = 50;
			nRect.left = 50;
			nRect.right = 200;
			nRect.bottom = 100;

			//DrawText(hDC, szText, wcslen(szText), &nRect,DT_LEFT | DT_SINGLELINE   ); 
			DrawText(hDC, szText, wcslen(szText), &nRect,DT_LEFT | DT_NOCLIP  ); 

			TCHAR szText2[12] = TEXT("E你xtTex好");
			INT  nText[10] = { 30, 0, 30, 30, 30, 
		        30, 30, 30, 0, 10 };
			ExtTextOut( hDC, 100, 300, ETO_OPAQUE,
		     NULL, szText2, wcslen(szText2), nText );

			HFONT hFont = CreateFont(100, 30, 100, 0,
									FW_BLACK, TRUE, TRUE, TRUE, DEFAULT_CHARSET,
									0, 0, 0, 0, TEXT("宋体") );
			HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

			TextOut(hDC, 100, 20, TEXT("宋体"), wcslen(TEXT("宋体")));
			
			SelectObject(hDC, hOldFont);
			DeleteObject(hFont);

			EndPaint(hwnd, &ps);
		}
		break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}


void SetExtra(HWND hWnd)
{
	SetClassLong(hWnd, 1, 20);
	SetWindowLong(hWnd, 1, 30);
	SetClassLong(hWnd, 2, 40);
	SetWindowLong(hWnd, 2, 50);
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


BOOL RegisterWindow(TCHAR* szClassName, WNDPROC WndProc, int nBrush)
{
	//定义窗口类
	WNDCLASSEX wcx = {0};
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	//wcx.lpfnWndProc = MyWndProc;
	wcx.lpfnWndProc = WndProc;
	wcx.cbClsExtra = 8;
	wcx.cbWndExtra = 8;
	wcx.hInstance = hInst;
	wcx.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
	wcx.hCursor = LoadCursor(hInst, IDC_WAIT);
	//wcx.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcx.hbrBackground = HBRUSH(nBrush);
	wcx.lpszMenuName = NULL;
	//wcx.lpszClassName = TEXT("win32Frame");
	wcx.lpszClassName = szClassName;
	wcx.hIconSm = NULL;
	//注册窗口类
	ATOM nAtom = RegisterClassEx(&wcx);

	if(nAtom == 0)
	{
		return FALSE;
	}
	return TRUE;
}


VOID CALLBACK MyTimerProc(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT_PTR idEvent,  // timer identifier
  DWORD dwTime       // current system time
)
{
	SYSTEMTIME SystemTime = {0};
	GetLocalTime(&SystemTime);

	memset(szSystemTime,0,sizeof(szSystemTime));
	wsprintf(szSystemTime,TEXT("%d-%d-%d %d:%d:%d"),
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);

	//WriteConsole(hOut,szSystemTime, wcslen(szSystemTime),NULL,NULL);
	InvalidateRect(hwnd, NULL, TRUE);
	
}

int WINAPI WinMain(  HINSTANCE hInstance,      // handle to current instance
  HINSTANCE hPrevInstance,  // handle to previous instance
  LPSTR lpCmdLine,          // command line
  int nCmdShow              // show state
) 
{
	BOOL bRet;
	MSG msg;
	hInst = hInstance;

	CreateConsole();

	//注册窗口
	RegisterWindow(TEXT("win32Frame"),MyWndProc, COLOR_BTNFACE+1 );
	RegisterWindow(TEXT("mywnd"),MyChildWndProc,  COLOR_WINDOW);
	RegisterWindow(TEXT("mybitmap"),BitMapWndProc,  COLOR_WINDOW);
	
	HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_WIN32FRAME));
	//建立窗口
	HWND hWnd = CreateWindowEx(NULL,
								TEXT("win32Frame"),
								//TEXT("Edit"),
								TEXT("win32sample"),
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								NULL,
								hMenu,
								hInstance,
								NULL);

	
	//建立多窗口客户区
	CLIENTCREATESTRUCT ccs;
	ccs.idFirstChild = 1000;

	HWND hMDIClient = 
		CreateWindowEx(
			0,
			TEXT("MDICLIENT"),
			TEXT("win32sample"),
			WS_CHILD | WS_VISIBLE , 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT,
			hWnd, 
			NULL, 
			hInstance,
			&ccs);

	RECT nCntRect = {0};
	//得到客户区大小，左上为0，右和下分别是宽和高
	GetClientRect(hWnd, &nCntRect);
	MoveWindow(hMDIClient, 
		nCntRect.left, 
		nCntRect.top, 
		nCntRect.right, 
		nCntRect.bottom, 
		TRUE); //最后一个参数为true，则向子窗口发送WM_PAINT消息

	hChild = 
		CreateWindowEx(
			WS_EX_MDICHILD,
			TEXT("mywnd"),
			TEXT("win32sample"),
			WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hMDIClient,
			NULL,
			hInst,
			NULL);


	hBitMapChild = 
		CreateWindowEx(
			WS_EX_MDICHILD,
			TEXT("mybitmap"),
			TEXT("bitmap"),
			WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hMDIClient,
			NULL,
			hInst,
			NULL);

	//MoveWindow(hChild, 0, 0, nCntRect.right, nCntRect.bottom, TRUE);
	//Multi2Wide(hWnd);
	//Wide2Multi(hWnd);
	//设置定时器
	SetTimer(hChild, 1001, 1000, MyTimerProc);
	SetTimer(hChild, 1002, 2000, NULL);
	//显示窗口
	ShowWindow(hWnd, TRUE);
	//更新窗口
	UpdateWindow(hWnd);
	//消息循环

	//快捷键
	HACCEL hAccel = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WIN32FRAME));

	while((bRet = GetMessage(&msg,hWnd,0,0)) != 0)
	{
		if(bRet == -1) 
		{

		}
		else
		{
			if(!TranslateAccelerator(hWnd, hAccel, &msg))
			{
				/*if (!IsWindow(hDlgWnd) || !IsDialogMessage(hDlgWnd, &msg)) 
				{*/
					//解析按键消息
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				/*}*/
			}
		}
	}

}
//#define MAX_LOADSTRING 100
//
//// 全局变量:
//HINSTANCE hInst;								// 当前实例
//TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
//TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
//
//// 此代码模块中包含的函数的前向声明:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//BOOL				InitInstance(HINSTANCE, int);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY _tWinMain(HINSTANCE hInstance,
//                     HINSTANCE hPrevInstance,
//                     LPTSTR    lpCmdLine,
//                     int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
// 	// TODO: 在此放置代码。
//	MSG msg;
//	HACCEL hAccelTable;
//
//	// 初始化全局字符串
//	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadString(hInstance, IDC_WIN32FRAME, szWindowClass, MAX_LOADSTRING);
//	MyRegisterClass(hInstance);
//
//	// 执行应用程序初始化:
//	if (!InitInstance (hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32FRAME));
//
//	// 主消息循环:
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	return (int) msg.wParam;
//}
//
//
//
////
////  函数: MyRegisterClass()
////
////  目的: 注册窗口类。
////
////  注释:
////
////    仅当希望
////    此代码与添加到 Windows 95 中的“RegisterClassEx”
////    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
////    这样应用程序就可以获得关联的
////    “格式正确的”小图标。
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32FRAME));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32FRAME);
//	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassEx(&wcex);
//}
//
////
////   函数: InitInstance(HINSTANCE, int)
////
////   目的: 保存实例句柄并创建主窗口
////
////   注释:
////
////        在此函数中，我们在全局变量中保存实例句柄并
////        创建和显示主程序窗口。
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   HWND hWnd;
//
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  目的: 处理主窗口的消息。
////
////  WM_COMMAND	- 处理应用程序菜单
////  WM_PAINT	- 绘制主窗口
////  WM_DESTROY	- 发送退出消息并返回
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (message)
//	{
//	case WM_COMMAND:
//		wmId    = LOWORD(wParam);
//		wmEvent = HIWORD(wParam);
//		// 分析菜单选择:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//		break;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		// TODO: 在此添加任意绘图代码...
//		EndPaint(hWnd, &ps);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}

// MyButton.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyButton.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYBUTTON, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYBUTTON));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYBUTTON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYBUTTON);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void OnDrawItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
	if(ODT_BUTTON == pdis->CtlType)
	{
		if(pdis->itemState & ODS_SELECTED)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(200,200,255));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(pdis->hDC, hBrush);

			RoundRect(pdis->hDC, 
					  pdis->rcItem.left, 
					  pdis->rcItem.top, 
					  pdis->rcItem.right, 
					  pdis->rcItem.bottom,
					  15,
					  15);

			SelectObject(pdis->hDC, hOldBrush);
			DeleteObject(hBrush);
		}
		else
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(100,100,255));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(pdis->hDC, hBrush);

			RoundRect(pdis->hDC, 
					  pdis->rcItem.left, 
					  pdis->rcItem.top, 
					  pdis->rcItem.right, 
					  pdis->rcItem.bottom,
					  15,
					  15);

			SelectObject(pdis->hDC, hOldBrush);
			DeleteObject(hBrush);
		}
		//��������
		TCHAR szText[260] = {0};
		GetWindowText(pdis->hwndItem, szText, 260);
		int nOldMode = SetBkMode(pdis->hDC, TRANSPARENT);

		DrawText(pdis->hDC, szText, wcslen(szText), &pdis->rcItem, 
			DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		SetBkMode(pdis->hDC, nOldMode);

	}
}

void OnMeasureItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPMEASUREITEMSTRUCT pdis = (LPMEASUREITEMSTRUCT)lParam;
	pdis->itemHeight = 20;
}
//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_MEASUREITEM:
		OnMeasureItem(hWnd, message, wParam, lParam);
		break;
	case WM_DRAWITEM:
		OnDrawItem(hWnd, message, wParam, lParam);
		break;
	case WM_CREATE:
		CreateWindow(
			TEXT("BUTTON"),
			TEXT("myself"), 
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
			60,50,100,50, hWnd, (HMENU)1001,hInst, NULL);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case 1001:
			if(wmEvent==BN_CLICKED)
			{
				MessageBox(hWnd, TEXT("click"), TEXT("mybutton"), NULL);
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

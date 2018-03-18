// modeless.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "modeless.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
TCHAR szMsg[64];

HWND hDlgWnd = NULL;
HWND ghEdit = NULL;
HWND ghSimpleCombo = NULL;
HWND ghDropdown = NULL;
HWND ghDropdownList = NULL;
HWND ghVscroll = NULL;
HWND ghHscroll = NULL;
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int nRadioCheck = 0;

BOOL CALLBACK GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    { 
		case WM_CREATE:
			//对话框没有WM_CREATE这个消息
			MessageBox(hwndDlg, TEXT("WM_CREATE"), TEXT("dialog"), NULL);
			break;
        case WM_INITDIALOG: 
			MessageBox(hwndDlg, TEXT("WM_INITDIALOG"), TEXT("dialog"), NULL);

			//静态文字框
			CreateWindow(
				TEXT("STATIC"),
				TEXT("hello static"), 
				WS_CHILD | WS_VISIBLE | 
				SS_SUNKEN | SS_SIMPLE | SS_NOTIFY, 
				50,50,100,100, hwndDlg, (HMENU)1001,hInst, NULL);

			//普通按钮
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("pushbutton"), 
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
				190,250,90,30, hwndDlg, (HMENU)1003,hInst, NULL);

			//三态
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("AUTO3STATE"), 
				WS_CHILD | WS_VISIBLE | BS_AUTO3STATE, 
				190,10,100,20, hwndDlg, (HMENU)2001,hInst, NULL);

			//分组按钮
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("分组1"), 
				WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
				180,30,150,80, hwndDlg, (HMENU)2001,hInst, NULL);

			//多选按钮
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("AUTOCHECKBOX"), 
				WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
				190,50,100,30, hwndDlg, (HMENU)3001,hInst, NULL);

			CreateWindow(
				TEXT("BUTTON"),
				TEXT("CHECKBOX"), 
				WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 
				190,70,100,30, hwndDlg, (HMENU)3002,hInst, NULL);

			//单选按钮
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("AUTORADIOBUTTON1"), 
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP , 
				190,120,200,30, hwndDlg, (HMENU)4001,hInst, NULL);

			CreateWindow(
				TEXT("BUTTON"),
				TEXT("AUTORADIOBUTTON2"), 
				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON , 
				190,140,200,30, hwndDlg, (HMENU)4002,hInst, NULL);

			CreateWindow(
				TEXT("BUTTON"),
				TEXT("RADIOBUTTON1"), 
				WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON  | WS_GROUP , 
				190,160,200,30, hwndDlg, (HMENU)5001,hInst, NULL);

			CreateWindow(
				TEXT("BUTTON"),
				TEXT("RADIOBUTTON2"), 
				WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON  , 
				190,180,200,30, hwndDlg, (HMENU)5002,hInst, NULL);
			CreateWindow(
				TEXT("BUTTON"),
				TEXT("RADIOBUTTON3"), 
				WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON  , 
				190,200,200,30, hwndDlg, (HMENU)5003,hInst, NULL);

			
            return TRUE; 
 
        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                    return TRUE; 
 
                case IDCANCEL: 
                    DestroyWindow(hwndDlg); 
                    hDlgWnd = NULL; 
                    return TRUE; 
				case 1001:
					if(STN_CLICKED == HIWORD(wParam))
					{
						MessageBox(hwndDlg, TEXT("click"), TEXT("static"), NULL);
					}
					return TRUE;
				case 1003:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						MessageBox(hwndDlg, TEXT("click"), TEXT("pushbutton"), NULL);
					}
					return TRUE;
				case 3001:
					
					if(BN_CLICKED  == HIWORD(wParam))
					{
						HWND hCheck = GetDlgItem(hwndDlg, 3001);
						LRESULT nCheck = 0;
						//MessageBox(hwndDlg, TEXT("click"), TEXT("pushbutton"), NULL);
						nCheck = SendMessage(hCheck, BM_GETCHECK, NULL, NULL);
						if(nCheck == BST_CHECKED)
						{
							MessageBox(hwndDlg, TEXT("checked"), TEXT("checkbox"), NULL);
						}
						if(nCheck == BST_UNCHECKED)
						{
							MessageBox(hwndDlg, TEXT("unchecked"), TEXT("checkbox"), NULL);
						}
					}
					return TRUE;
				case 3002:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						HWND hCheck = GetDlgItem(hwndDlg, 3002);
						LRESULT nCheck = 0;
						//MessageBox(hwndDlg, TEXT("click"), TEXT("pushbutton"), NULL);
						nCheck = SendMessage(hCheck, BM_GETCHECK, NULL, NULL);
						if(nCheck == BST_CHECKED)
						{
							SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, NULL);
						}
						if(nCheck == BST_UNCHECKED)
						{
							SendMessage(hCheck, BM_SETCHECK, BST_CHECKED, NULL);
						}
					}
					return TRUE;
				case 4001:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						UINT nCheck = IsDlgButtonChecked(hwndDlg, 4001);
						if(BST_CHECKED == nCheck)
						{
							MessageBox(hwndDlg, TEXT("4001"), TEXT("radio"), NULL);
						}
						
					}
					return TRUE;
				case 4002:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						UINT nCheck = IsDlgButtonChecked(hwndDlg, 4002);
						if(BST_CHECKED == nCheck)
						{
							MessageBox(hwndDlg, TEXT("4002"), TEXT("radio"), NULL);
						}
						
					}
					return TRUE;
				case 5001:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						CheckRadioButton(hwndDlg, 5001, 5003, 5001);
						
					}
					return TRUE;
				case 5002:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						CheckRadioButton(hwndDlg, 5001, 5003, 5002);
						
					}
					return TRUE;
				case 5003:
					if(BN_CLICKED  == HIWORD(wParam))
					{
						
						CheckRadioButton(hwndDlg, 5001, 5003, 5003);
						
					}
					return TRUE;
            } 
    } 
    return FALSE; 
} 

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MODELESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MODELESS));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MODELESS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MODELESS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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

void onSave(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR* pszBuff = NULL;
	char* pszDes = NULL;

	LRESULT nLength 
		= SendMessage(
			ghEdit, 
			WM_GETTEXTLENGTH,
			0,
			0);

	//swprintf(szMsg, TEXT("length:%ld"), nLength);
	//MessageBox(hWnd, szMsg, TEXT("length"), NULL);

	pszBuff = (TCHAR*)malloc((nLength + 1)*sizeof(TCHAR));
	memset(pszBuff, 0, (nLength + 1)*sizeof(TCHAR));

	SendMessage(ghEdit, WM_GETTEXT, (WPARAM)(nLength+1), (LPARAM)pszBuff);
	//MessageBox(hWnd, pszBuff, TEXT("contact"), NULL);

	int nlen = WideCharToMultiByte(
				CP_ACP,
				NULL,
				pszBuff,
				wcslen(pszBuff),
				NULL,NULL,NULL,NULL);

	pszDes = (char*)malloc(nlen + 1);
	memset(pszDes, 0, nlen+1);
	WideCharToMultiByte(
		CP_ACP,
		NULL, 
		pszBuff, 
		wcslen(pszBuff), 
		pszDes, 
		nlen, 
		NULL,NULL); 

	//保存文件
	FILE* pFile = fopen("d:\\1.txt","w+");
	
	fwrite(pszDes, sizeof(char), strlen(pszDes), pFile);

	fclose(pFile);
	free(pszBuff);
	free(pszDes);

}

void onOpen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char* pszDes = NULL;
	TCHAR* pszEdit = NULL;

	FILE* pFile = fopen("d:\\1.txt","r");
	//获取文件长度
	fseek(pFile,0,SEEK_END);
	long nLen = ftell(pFile);
	fseek(pFile,0,SEEK_SET);

	pszDes = (char*)malloc(nLen + 1);
	memset(pszDes, 0, nLen+1);
	fread(pszDes, nLen, 1, pFile);
	fclose(pFile);

	//MessageBoxA(hWnd, pszDes, "char", NULL);
	//多字节转换成宽字节
	int nLength = MultiByteToWideChar(CP_ACP,NULL, pszDes, strlen(pszDes), NULL, NULL);
	pszEdit = (TCHAR*)malloc( (nLength+1)*sizeof(TCHAR));
	memset(pszEdit, 0, (nLength+1)*sizeof(TCHAR));

	MultiByteToWideChar(CP_ACP, NULL, pszDes, strlen(pszDes), pszEdit, nLength);
	//MessageBox(hWnd, pszEdit, TEXT("multibyte"),NULL);
	SendMessage(ghEdit, WM_SETTEXT, NULL, (LPARAM)pszEdit);

	free(pszDes);
	free(pszEdit);

}

void onAdd(HWND hWnd)
{
	TCHAR szItem[] = TEXT("ComboBox 0");
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	for(int n = 0; n < 7; n++)
	{
		LRESULT nIndex = 
			SendMessage(
				hSimpleCombo, 
				CB_ADDSTRING, 
				NULL, 
				(LPARAM)szItem);

		//设定项的值
		SendMessage(hSimpleCombo,
			CB_SETITEMDATA,
			nIndex,
			100+n);

		SendMessage(hDropDown, CB_ADDSTRING, NULL, (LPARAM)szItem);
		SendMessage(hDropDownList, CB_ADDSTRING, NULL, (LPARAM)szItem);
		szItem[9]++;
	}
}

void onDel(HWND hWnd)
{
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	SendMessage(hSimpleCombo, CB_DELETESTRING, 3, NULL);
	SendMessage(hDropDown, CB_DELETESTRING, 3, NULL);
	SendMessage(hDropDownList, CB_DELETESTRING, 3, NULL);
}

void onReset(HWND hWnd)
{
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	SendMessage(hSimpleCombo, CB_RESETCONTENT, NULL, NULL);
	SendMessage(hDropDown, CB_RESETCONTENT, NULL, NULL);
	SendMessage(hDropDownList, CB_RESETCONTENT, NULL, NULL);
}

void onSel(HWND hWnd)
{
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	int nIndex 
		= SendMessage(hSimpleCombo, CB_GETCURSEL, NULL, NULL);

	int nValue 
		= SendMessage(hSimpleCombo, CB_GETITEMDATA, nIndex, NULL);

	swprintf(szMsg, TEXT("sel--index:%d, value:%d"), nIndex, nValue);
	MessageBox(hWnd, szMsg, TEXT("simplecombo"), NULL);

	//根据索引设置选择项
	SendMessage(hDropDown, CB_SETCURSEL, nIndex, NULL);
	//根据内容设置选择项
	TCHAR szSel[] = TEXT("ComboBox 4");
	SendMessage(hDropDownList, CB_SELECTSTRING, -1, (LPARAM)szSel);
	
}

void onFind(HWND hWnd)
{
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	//查找内容
	TCHAR szFind[] = TEXT("ComboBox 5");
	LRESULT nIndex 
		= SendMessage(hSimpleCombo, CB_FINDSTRING, -1, (LPARAM)szFind);

	SendMessage(hDropDown, CB_SETCURSEL, nIndex, NULL);
	SendMessage(hDropDownList, CB_SETCURSEL, nIndex, NULL);

}

void onPath(HWND hWnd)
{
	HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
	HWND hDropDown = GetDlgItem(hWnd, 7002);
	HWND hDropDownList = GetDlgItem(hWnd, 7003);

	TCHAR szPath[] = TEXT("D:\\1\\*.*");

	SendMessage(hSimpleCombo, CB_DIR, DDL_ARCHIVE | DDL_DIRECTORY, (LPARAM)szPath);
	SendMessage(hDropDown, CB_DIR, DDL_ARCHIVE | DDL_DIRECTORY, (LPARAM)szPath);
	SendMessage(hDropDownList, CB_DIR, DDL_ARCHIVE | DDL_DIRECTORY, (LPARAM)szPath);
}

void OnListAdd(HWND hWnd)
{
	HWND hSingleList = GetDlgItem(hWnd, 8001);
	HWND hMultiList = GetDlgItem(hWnd, 8002);

	TCHAR szItem[] = TEXT("ListBox 0");

	for(int i = 0; i < 7; i++)
	{
		SendMessage(hSingleList, LB_ADDSTRING, NULL, (LPARAM)szItem);
		SendMessage(hMultiList, LB_ADDSTRING, NULL, (LPARAM)szItem);
		szItem[8]++;
	}
}

void OnListSel(HWND hWnd)
{
	HWND hSingleList = GetDlgItem(hWnd, 8001);
	HWND hMultiList = GetDlgItem(hWnd, 8002);

	//设定指定选项
	SendMessage(hSingleList, LB_SETCURSEL, 2, NULL);
	for(int i = 1; i < 4; i++)
	{
		SendMessage(hMultiList, LB_SETSEL, TRUE, i);
	}

	//获取所有选择项的数量
	LRESULT nCount 
		= SendMessage(hMultiList, LB_GETSELCOUNT, NULL, NULL);
	
	//swprintf(szMsg, TEXT("count:%ld"), nCount);
	//MessageBox(hWnd, szMsg, TEXT("list"), NULL);

	INT selItems[7] = {0};
	SendMessage(hMultiList, LB_GETSELITEMS, 7, (LPARAM)selItems);

	swprintf(szMsg, TEXT("sel:%d,%d,%d,%d"), selItems[0],selItems[1],selItems[2],selItems[3]);
	MessageBox(hWnd, szMsg, TEXT("list"), NULL);
}

void OnHscroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nScrollCode = LOWORD(wParam);
	int nPos = HIWORD(wParam);

	switch(nScrollCode)
	{
	case SB_LINELEFT:
		nPos = SendMessage(ghHscroll, SBM_GETPOS, NULL, NULL);
		nPos = nPos - 10;
		SendMessage(ghHscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_LINERIGHT:
		nPos = SendMessage(ghHscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos + 10;
		SendMessage(ghHscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_PAGELEFT:
		nPos = SendMessage(ghHscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos - 50;
		SendMessage(ghHscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_PAGERIGHT:
		nPos = SendMessage(ghHscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos + 50;
		SendMessage(ghHscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_THUMBPOSITION:
		SendMessage(ghHscroll, SBM_SETPOS, nPos, TRUE);
		break;

	}

	InvalidateRect(hWnd, NULL, TRUE);
}

void OnVscroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nScrollCode = LOWORD(wParam);
	int nPos = HIWORD(wParam);

	switch(nScrollCode)
	{
	case SB_LINEUP:
		nPos = SendMessage(ghVscroll, SBM_GETPOS, NULL, NULL);
		nPos = nPos - 10;
		SendMessage(ghVscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_LINEDOWN:
		nPos = SendMessage(ghVscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos + 10;
		SendMessage(ghVscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_PAGEUP:
		nPos = SendMessage(ghVscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos - 50;
		SendMessage(ghVscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_PAGEDOWN:
		nPos = SendMessage(ghVscroll, SBM_GETPOS, NULL, NULL);
		nPos= nPos + 50;
		SendMessage(ghVscroll, SBM_SETPOS, nPos, TRUE);
		break;
	case SB_THUMBPOSITION:
		SendMessage(ghVscroll, SBM_SETPOS, nPos, TRUE);
		break;

	}

	InvalidateRect(hWnd, NULL, TRUE);
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_HSCROLL:
		OnHscroll(hWnd, message, wParam, lParam);
		break;
	case WM_VSCROLL:
		OnVscroll(hWnd, message, wParam, lParam);
		break;
	case WM_CREATE:
		CreateWindow(
				TEXT("STATIC"),
				TEXT("icon"), 
				WS_CHILD | WS_VISIBLE | 
				SS_SUNKEN | SS_ICON | SS_NOTIFY, 
				0,50,50,50, hWnd, (HMENU)1002,hInst, NULL);
		//简单组合框创建
		ghSimpleCombo = CreateWindow(
							TEXT("COMBOBOX"),
							TEXT("simple"), 
							WS_CHILD | WS_VISIBLE | CBS_SIMPLE , 
							60,50,100,150, hWnd, (HMENU)7001,hInst, NULL);

		//下拉式组合框
		ghDropdown = CreateWindow(
							TEXT("COMBOBOX"),
							TEXT("Dropdown"), 
							WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL , 
							170,50,150,100, hWnd, (HMENU)7002,hInst, NULL);

		//下拉列表组合框
		ghDropdownList = CreateWindow(
							TEXT("COMBOBOX"),
							TEXT("DropdownList"), 
							WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL , 
							330,50,150,100, hWnd, (HMENU)7003,hInst, NULL);
		//文本框
		ghEdit = CreateWindow(
					TEXT("EDIT"),
					TEXT("点dian滴滴"), 
					WS_CHILD | WS_VISIBLE 
					| ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL , 
					0,200,800,300, hWnd, (HMENU)6001,hInst, NULL);
		
		//单选列表框
		CreateWindow(
					TEXT("LISTBOX"),
					TEXT("点dian滴滴"), 
					WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL , 
					500,50,150,100, hWnd, (HMENU)8001,hInst, NULL);

		//多选列表框
		CreateWindow(
					TEXT("LISTBOX"),
					TEXT("点dian滴滴"), 
					WS_CHILD | WS_VISIBLE | LBS_NOTIFY | LBS_MULTIPLESEL | WS_VSCROLL, 
					670,50,150,100, hWnd, (HMENU)8002,hInst, NULL);
		
		RECT nRect;
		GetClientRect(hWnd, &nRect); 
		//创建滚动条
		ghHscroll = CreateWindow(
					TEXT("SCROLLBAR"),
					TEXT("点dian滴滴"), 
					WS_CHILD | WS_VISIBLE | SBS_HORZ, 
					830,nRect.top + 100,150,20, hWnd, (HMENU)9001,hInst, NULL);
		ghVscroll = CreateWindow(
					TEXT("SCROLLBAR"),
					TEXT("点dian滴滴"), 
					WS_CHILD | WS_VISIBLE | SBS_VERT , 
					nRect.right-20,0,20,100, hWnd, (HMENU)9002,hInst, NULL);

		//设置滚动范围
		SendMessage(ghHscroll, SBM_SETRANGE, 0, 300);
		SendMessage(ghVscroll, SBM_SETRANGE, 0, 300);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case ID_COMBO_DIR:
			onPath(hWnd);
			break;
		case ID_COMBO_FIND:
			onFind(hWnd);
			break;
		case ID_COMBO_RESET:
			onReset(hWnd);
			break;
		case ID_COMBO_ADD:
			//MessageBox(hWnd, TEXT("add"), TEXT("combo"), NULL);
			onAdd(hWnd);
			break;
		case ID_COMBO_DEL:
			onDel(hWnd);
			break;
		case ID_COMBO_SEL:
			onSel(hWnd);
			break;
		case ID_LIST_ADD:
			OnListAdd(hWnd);
			break;
		case ID_LIST_SEL:
			OnListSel(hWnd);
			break;
		case 8001:
			if(wmEvent == LBN_SELCHANGE)
			{
				//MessageBox(hWnd, TEXT("change"), TEXT("list"), NULL);
			}
			break;
		case 7001:
			if(wmEvent == CBN_SELCHANGE)
			{
				HWND hSimpleCombo = GetDlgItem(hWnd, 7001);
				HWND hDropDown = GetDlgItem(hWnd, 7002);
				HWND hDropDownList = GetDlgItem(hWnd, 7003);

				LRESULT nIndex 
					= SendMessage(hSimpleCombo, CB_GETCURSEL, NULL, NULL);

				SendMessage(hDropDown, CB_SETCURSEL, nIndex, NULL);
				SendMessage(hDropDownList, CB_SETCURSEL, nIndex, NULL);
			}
			break;
		case 6001:
			if(wmEvent == EN_CHANGE)
			{
				//MessageBox(hWnd, TEXT("change"), TEXT("edit"), NULL);
			}
			break;
		case ID_SAVE:
			onSave(hWnd, message, wParam, lParam);
			break;
		case ID_OPEN:
			onOpen(hWnd, message, wParam, lParam);
			break;
		case ID_SELALL:
			SendMessage(ghEdit, EM_SETSEL, 0, -1);
			break;
		case ID_COPY:
			SendMessage(ghEdit, WM_COPY, NULL, NULL);
			break;
		case ID_PASTE:
			SendMessage(ghEdit, WM_PASTE, NULL, NULL);
			break;
		case ID_32772:
			//MessageBox(hWnd, TEXT("icon"), TEXT("static"), NULL);
			{
			HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
			HWND hStatic = GetDlgItem(hWnd, 1002);
			SendMessage(hStatic, STM_SETICON,(WPARAM)hIcon, NULL); 
			}
			break;
		case ID_32771:
			//MessageBox(hWnd, TEXT("ID_32771"), TEXT("dialog"), NULL);
			hDlgWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1),hWnd, GoToProc);
			ShowWindow(hDlgWnd, SW_SHOW);
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
		{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		int nVPos = SendMessage(ghVscroll, SBM_GETPOS, NULL, NULL);
		int nHPos = SendMessage(ghHscroll, SBM_GETPOS, NULL, NULL);
		TCHAR szText[] = TEXT("scroll used");
		TextOut(hdc, 850 - nHPos, 80 - nVPos, szText, wcslen(szText));
		EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
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

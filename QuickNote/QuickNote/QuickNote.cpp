// QuickNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
NoteBook* myNoteBook;
HWND g_mainWindow;

NOTIFYICONDATA nidApp;
HMENU hMenu;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKNOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKNOTE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKNOTE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND g_mainWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_mainWindow)
   {
      return FALSE;
   }

   nidApp.cbSize = sizeof(NOTIFYICONDATA);
   nidApp.hWnd = (HWND)g_mainWindow;
   nidApp.uID = IDI_SYSTRAY;          
   nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; 
   nidApp.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTRAYICON));
   nidApp.uCallbackMessage = WM_SYSTRAY;
   LoadString(hInstance, IDS_SYSTRAYTOOLTIP, nidApp.szTip, 128);
   LoadString(hInstance, IDS_SYSTRAYTOOLTIP, nidApp.szInfo, 128);
   Shell_NotifyIcon(NIM_ADD, &nidApp);

   ShowWindow(g_mainWindow, SW_HIDE);
   UpdateWindow(g_mainWindow);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT lpClickPoint;
    switch (message)
    {
	case WM_CREATE:
		myNoteBook = new NoteBook;
		InstallHook(hWnd);
		break;
	case WM_SYSTRAY:
	{
		switch (LOWORD(lParam))
		{
		case WM_RBUTTONDOWN:
		{
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			GetCursorPos(&lpClickPoint);
			hMenu = CreatePopupMenu();
			InsertMenu(hMenu, -1, uFlag, IDM_ADDNOTE, L"Add new note...");
			InsertMenu(hMenu, -1, uFlag, IDM_VIEWNOTE, L"View note list...");
			InsertMenu(hMenu, -1, uFlag, IDM_STATISTICS, L"Statistics...");
			InsertMenu(hMenu, -1, uFlag, IDM_EXIT, L"Exit");
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN,
				lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
		}
			break;
		case WM_LBUTTONDOWN:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDNOTE), hWnd, AddNote_Dialog);
			break;
		}
	}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_VIEWNOTE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_VIEWNOTE), hWnd, ViewNote_Dialog);
				break;
			case IDM_ADDNOTE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDNOTE), hWnd, AddNote_Dialog);
				break;
			case IDM_STATISTICS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_STATISTICS), hWnd, Statistics_Dialog);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_CLOSE:
		if (myNoteBook != NULL)
			delete myNoteBook;
		UninstallHook(hWnd);
		Shell_NotifyIcon(NIM_DELETE, &nidApp);
		PostQuitMessage(0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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


INT_PTR CALLBACK	ViewNote_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_VIEW_EXIT:
			EndDialog(hDlg, true);
			break;
		default:
			break;
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hDlg, true);
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK	AddNote_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_ADD_CANCEL:
			EndDialog(hDlg, true);
			break;
		default:
			break;
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hDlg, true);
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK	Statistics_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_STAT_EXIT:
			EndDialog(hDlg, true);
			break;
		default:
			break;
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hDlg, true);
		break;
	}

	return (INT_PTR)FALSE;
}

void InstallHook(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"QuickNoteHook.dll");
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_installHook");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}

void UninstallHook(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"QuickNoteHook.dll");
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_removeHook");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}
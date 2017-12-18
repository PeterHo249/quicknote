// QuickNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
NoteBook* myNoteBook;
HWND g_mainWindow;

NOTIFYICONDATA nidApp;
HMENU hMenu;
int currentNotePos = -1;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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

	GdiplusShutdown(gdiplusToken);

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSTRAYICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKNOTE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SYSTRAYICON));

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
		INITCOMMONCONTROLSEX icex;
		// Ensure that the common control DLL is loaded. 
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
		InitCommonControlsEx(&icex);
		srand(time(NULL));
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
			InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, NULL, NULL);
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
				SendMessage(hWnd, WM_CLOSE, 0, 0);
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

INT_PTR CALLBACK ViewNote_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND tagTreeView = GetDlgItem(hDlg, IDC_VIEW_TAGLISTTREE);
	HWND noteTreeView = GetDlgItem(hDlg, IDC_VIEW_NOTELISTTREE);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		RefreshView(hDlg);
		return (INT_PTR)TRUE;
	}

	case WM_NOTIFY:
	{
		// Catch message of tree view

		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		// Message of tag tree view
		case IDC_VIEW_TAGLISTTREE:
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
			NMHDR info = pnmtv->hdr;
			
			if (TVN_SELCHANGED == info.code)
			{
				// Reload note tree view
				TreeView_DeleteAllItems(noteTreeView);
				SetDlgItemText(hDlg, IDC_VIEW_DATETXT, L"");
				SetDlgItemText(hDlg, IDC_VIEW_CONTENTTXT, L"");
				SetDlgItemText(hDlg, IDC_VIEW_TAGTXT, L"");

				TVITEM tvItem = pnmtv->itemNew;

				if ((int)tvItem.lParam != -1)
				{
					vector<int> noteIndexList = 
						myNoteBook->GetTag((int)tvItem.lParam)->GetNoteList();

					for (unsigned int i = 0; i < noteIndexList.size(); i++)
					{
						Note* pNote = myNoteBook->GetNoteAt(noteIndexList[i]);
						if (pNote != NULL)
						{
							// Add new item to tree view
							TVINSERTSTRUCT tvItem;
							tvItem.hParent = NULL;
							tvItem.hInsertAfter = TVI_LAST;
							tvItem.item.mask = TVIF_TEXT | TVIF_PARAM;
							tvItem.item.pszText = pNote->GetPreview();
							tvItem.item.lParam = (LPARAM)noteIndexList[i];
							TreeView_InsertItem(noteTreeView, &tvItem);
						}
					}
				}
				else
				{
					vector<int> noteIndexList = myNoteBook->GetNoteOrder();

					for (unsigned int i = 0; i < noteIndexList.size(); i++)
					{
						Note* pNote = myNoteBook->GetNoteAt(noteIndexList[i]);
						if (pNote != NULL)
						{
							// Add new item to tree view
							TVINSERTSTRUCT tvItem;
							tvItem.hParent = NULL;
							tvItem.hInsertAfter = TVI_LAST;
							tvItem.item.mask = TVIF_TEXT | TVIF_PARAM;
							tvItem.item.pszText = pNote->GetPreview();
							tvItem.item.lParam = (LPARAM)noteIndexList[i];
							TreeView_InsertItem(noteTreeView, &tvItem);
						}
					}
				}
			}
		}
		break;

		// Message of note tree view
		case IDC_VIEW_NOTELISTTREE:
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
			NMHDR info = pnmtv->hdr;

			if (TVN_SELCHANGED == info.code)
			{
				// Reload content of note
				TVITEM tvItem = pnmtv->itemNew;

				currentNotePos = (int)tvItem.lParam;
				Note* pNote = myNoteBook->GetNoteAt(currentNotePos);

				if (pNote != NULL)
				{
					WCHAR* temp = pNote->GetDate();
					SetDlgItemText(hDlg, IDC_VIEW_DATETXT, temp);
					temp = pNote->GetContent();
					SetDlgItemText(hDlg, IDC_VIEW_CONTENTTXT, temp);
					temp = pNote->GetTagString();
					SetDlgItemText(hDlg, IDC_VIEW_TAGTXT, temp);
				}
			}
		}
			break;
		}
	}
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_VIEW_DELETE:
			myNoteBook->DeleteNoteAt(currentNotePos);
			RefreshView(hDlg);
			break;

		case IDC_VIEW_MODIFYNOTE:
			if (currentNotePos == -1)
				break;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MODIFY), hDlg, Modify_Dialog);
			RefreshView(hDlg);
			break;

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

INT_PTR CALLBACK AddNote_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_ADD_SAVE:
		{
			WCHAR* contentString = new WCHAR[MAX_LOADSTRING + 1];
			WCHAR* tagString = new WCHAR[MAX_LOADSTRING + 1];
			ZeroMemory(contentString, (MAX_LOADSTRING + 1) * sizeof(WCHAR));
			ZeroMemory(tagString, (MAX_LOADSTRING + 1) * sizeof(WCHAR));

			GetDlgItemText(hDlg, IDC_ADD_CONTENTTXT, contentString, MAX_LOADSTRING);
			GetDlgItemText(hDlg, IDC_ADD_TAGTXT, tagString, MAX_LOADSTRING);

			if (wcslen(contentString) == 0 || wcslen(tagString) == 0)
			{
				// Show something to notify error
				if (wcslen(contentString) == 0)
					ShowWindow(GetDlgItem(hDlg, IDC_ADD_ERROR_CONTENT), SW_SHOW);
				else
					ShowWindow(GetDlgItem(hDlg, IDC_ADD_ERROR_CONTENT), SW_HIDE);

				if (wcslen(tagString) == 0)
					ShowWindow(GetDlgItem(hDlg, IDC_ADD_ERROR_TAG), SW_SHOW);
				else
					ShowWindow(GetDlgItem(hDlg, IDC_ADD_ERROR_TAG), SW_HIDE);

				break;
			}
			else
			{
				myNoteBook->AddNewNote(contentString, tagString);
				EndDialog(hDlg, TRUE);
			}
		}
			break;
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

INT_PTR CALLBACK Statistics_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	DrawingInfo* drawingInfo = NULL;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		WCHAR tempStr[100];
		wsprintf(tempStr, L"Number of tag: %d", myNoteBook->GetTagCount());
		SetDlgItemText(hDlg, IDC_STAT_TAGCOUNT, tempStr);
		wsprintf(tempStr, L"Number of note: %d", myNoteBook->GetNoteCount());
		SetDlgItemText(hDlg, IDC_STAT_NOTECOUNT, tempStr);
		InvalidateRect(hDlg, NULL, TRUE);
		return (INT_PTR)TRUE;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		// TODO: Add any drawing code that uses hdc here...
		RECT rect;
		GetWindowRect(hDlg, &rect);
		drawingInfo = new DrawingInfo(hdc, myNoteBook);
		drawingInfo->Draw();
		if (drawingInfo != NULL)
		{
			delete drawingInfo;
			drawingInfo = NULL;
		}

		EndPaint(hDlg, &ps);
	}
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_STAT_EXIT:
			if (drawingInfo != NULL)
			{
				delete drawingInfo;
				drawingInfo = NULL;
			}
			EndDialog(hDlg, true);
			break;
		default:
			break;
		}
	}
	break;

	case WM_CLOSE:
		if (drawingInfo != NULL)
		{
			delete drawingInfo;
			drawingInfo = NULL;
		}
		EndDialog(hDlg, true);
		break;
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Modify_Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		Note* noteTemp = myNoteBook->GetNoteAt(currentNotePos);
		SetDlgItemText(hDlg, IDC_MODIFY_CONTENTTXT, noteTemp->GetContent());
		SetDlgItemText(hDlg, IDC_MODIFY_TAGTXT, noteTemp->GetTagString());
		return (INT_PTR)TRUE;
	}
		break;

	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDC_MODIFY_SAVE:
		{
			WCHAR* contentString = new WCHAR[MAX_LOADSTRING + 1];
			WCHAR* tagString = new WCHAR[MAX_LOADSTRING + 1];
			ZeroMemory(contentString, (MAX_LOADSTRING + 1) * sizeof(WCHAR));
			ZeroMemory(tagString, (MAX_LOADSTRING + 1) * sizeof(WCHAR));

			GetDlgItemText(hDlg, IDC_MODIFY_CONTENTTXT, contentString, MAX_LOADSTRING);
			GetDlgItemText(hDlg, IDC_MODIFY_TAGTXT, tagString, MAX_LOADSTRING);

			if (wcslen(contentString) == 0 || wcslen(tagString) == 0)
			{
				// Show something to notify error
				if (wcslen(contentString) == 0)
					ShowWindow(GetDlgItem(hDlg, IDC_MODIFY_ERROR_CONTENT), SW_SHOW);
				else
					ShowWindow(GetDlgItem(hDlg, IDC_MODIFY_ERROR_CONTENT), SW_HIDE);

				if (wcslen(tagString) == 0)
					ShowWindow(GetDlgItem(hDlg, IDC_MODIFY_ERROR_TAG), SW_SHOW);
				else
					ShowWindow(GetDlgItem(hDlg, IDC_MODIFY_ERROR_TAG), SW_HIDE);

				break;
			}
			else
			{
				myNoteBook->ModifyNote(currentNotePos, contentString, tagString);
				EndDialog(hDlg, TRUE);
			}
		}
			break;
		case IDC_MODIFY_CANCEL:
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

void RefreshView(HWND hDlg)
{
	currentNotePos = -1;

	HWND tagTreeView = GetDlgItem(hDlg, IDC_VIEW_TAGLISTTREE);
	HWND noteTreeView = GetDlgItem(hDlg, IDC_VIEW_NOTELISTTREE);

	TreeView_DeleteAllItems(tagTreeView);
	TreeView_DeleteAllItems(noteTreeView);

	SetDlgItemText(hDlg, IDC_VIEW_DATETXT, L"");
	SetDlgItemText(hDlg, IDC_VIEW_CONTENTTXT, L"");
	SetDlgItemText(hDlg, IDC_VIEW_TAGTXT, L"");

	vector<vector<Tag*>> tagList = myNoteBook->GetTagList();

	WCHAR* tempStr = new WCHAR[100];
	ZeroMemory(tempStr, 100 * sizeof(WCHAR));

	TVINSERTSTRUCT allnoteTvItem;
	allnoteTvItem.hParent = NULL;
	allnoteTvItem.hInsertAfter = TVI_LAST;
	allnoteTvItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	wsprintf(tempStr, L"All Note (%d)", myNoteBook->GetNoteCount());
	allnoteTvItem.item.pszText = tempStr;
	allnoteTvItem.item.lParam = (LPARAM)-1;
	TreeView_InsertItem(tagTreeView, &allnoteTvItem);

	int index = 0;
	for (unsigned int i = 0; i < tagList.size(); i++)
	{
		for (unsigned int j = 0; j < tagList[i].size(); j++)
		{
			ZeroMemory(tempStr, 100 * sizeof(WCHAR));
			TVINSERTSTRUCT tvItem;
			tvItem.hParent = NULL;
			tvItem.hInsertAfter = TVI_LAST;
			tvItem.item.mask = TVIF_TEXT | TVIF_PARAM;
			wsprintf(tempStr, L"%s (%d)",
				tagList[i][j]->GetName(), tagList[i][j]->GetNoteList().size());
			tvItem.item.pszText = tempStr;
			tvItem.item.lParam = (LPARAM)index;
			TreeView_InsertItem(tagTreeView, &tvItem);
			index++;
		}
	}
}
#pragma once
#include "stdafx.h"

#include "resource.h"

#define MAX_LOADSTRING 255

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ViewNote_Dialog(HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam);
INT_PTR CALLBACK	AddNote_Dialog(HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam);
INT_PTR CALLBACK	Statistics_Dialog(HWND hDlg, UINT message, WPARAM wparam, LPARAM lparam);
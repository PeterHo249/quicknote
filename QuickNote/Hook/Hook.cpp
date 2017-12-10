// Hook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Hook.h"



HHOOK hHook = NULL;
HINSTANCE hinstLib;
HWND hMainWnd = NULL;

LRESULT CALLBACK LLKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT* kbHookStruct = (KBDLLHOOKSTRUCT*)lParam;
		if (kbHookStruct->vkCode == VK_SPACE && GetAsyncKeyState(VK_CONTROL) < 0)
		{
			SendMessage(hMainWnd, WM_COMMAND, MAKEWPARAM(IDM_ADDNOTE, 0), 0);
			return 1;
		}
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void _installHook(HWND hWnd)
{
	if (hHook != NULL)
		return;

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LLKeyboardHookProc, hinstLib, 0);

	hMainWnd = hWnd;
}

void _removeHook(HWND hWnd)
{
	if (hHook == NULL)
		return;

	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}
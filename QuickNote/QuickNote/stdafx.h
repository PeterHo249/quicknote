// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <vector>
#include <CommCtrl.h>
#include <windowsx.h>
#include <WinUser.h>
#include <WinBase.h>
#include <CommCtrl.h>
#include <vector>
#include <fstream>
#include <Shlwapi.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <ctime>
#include <shellapi.h>
#include <Gdiplusgraphics.h>

#include "Note.h"
#include "Tag.h"
#include "NoteBook.h"
#include "QuickNote.h"
#include "resource.h"
#include "DrawingInfo.h"
#include "TagDrawingInfo.h"

using namespace std;
using namespace Gdiplus;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment (lib,"Gdiplus.lib")

// BOM of UTF-16 file
const unsigned short BOM = 0xFEFF;

// Define some contant
#define MAX_FILE_PATH 1024
#define MAX_LOADSTRING 512
#define TOP_OFFSET 50

enum Direction
{
	topMove,
	rightMove,
	bottomMove,
	leftMove
};
#pragma once
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

class TagDrawingInfo
{
private:
	// Private members
	WCHAR* name;
	int rank;
	int height;
	int width;
	RectF* boundRect;
	Color* color;
	Font* font;
	HDC hdc;

	//==========================================================================

	// Private methods

	// Set suitable font for tag drawing
	Font* SetFont();
	// Set value for height and width member base on name string
	void SetSize();
	// Create a random color for tag drawing
	Color* GetRandomColor();

public:
	TagDrawingInfo();
	TagDrawingInfo(HDC hdc, WCHAR* name, int rank);
	~TagDrawingInfo();

	//==========================================================================
	
	// Set bound rectangle for tag drawing
	void SetRect(int left, int top);
	// Draw tag to screen
	void Draw();

	//==========================================================================

	// Getter
	int GetRank();
	int GetWidth();
	RectF* GetRect();
	int GetHeight();
};


#pragma once
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

class TagDrawingInfo
{
private:
	WCHAR* name;
	int rank;
	int height;
	int width;
	RectF* boundRect;
	Color* color;
	Font* font;
	HDC hdc;
public:
	TagDrawingInfo();
	TagDrawingInfo(HDC hdc, WCHAR* name, int rank);
	~TagDrawingInfo();

	void SetSize();
	Color* GetRandomColor();
	void SetRect(int top, int left);
	Font* SetFont();
	void Draw();
	int GetRank();
	int GetWidth();
};


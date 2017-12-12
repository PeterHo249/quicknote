#include "stdafx.h"
#include "TagDrawingInfo.h"


TagDrawingInfo::TagDrawingInfo()
{
	name = NULL;
	color = NULL;
	boundRect = NULL;
	font = NULL;
	rank = -1;
	height = -1;
	width = -1;
}


TagDrawingInfo::~TagDrawingInfo()
{
	if (this->color != NULL)
	{
		delete color;
		color = NULL;
	}

	if (this->boundRect != NULL)
	{
		delete boundRect;
		boundRect = NULL;
	}

	if (this->name != NULL)
	{
		delete[] name;
		name = NULL;
	}

	if (font != NULL)
	{
		delete font;
		font = NULL;
	}
}

TagDrawingInfo::TagDrawingInfo(HDC hdc, WCHAR* name, int rank)
{
	this->hdc = hdc;
	this->name = name;
	this->rank = rank;
	color = GetRandomColor();
	font = SetFont();
	SetSize();
	boundRect = NULL;
}

void TagDrawingInfo::SetSize()
{
	Graphics graphics(hdc);

	PointF origin(0.0f, 0.0f);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentCenter);
	RectF rect;

	graphics.MeasureString(this->name, wcslen(this->name) + 1, this->font, origin, &rect);
	
	SizeF size;
	rect.GetSize(&size);
	height = size.Height;
	width = size.Width + 5;
}

Color* TagDrawingInfo::GetRandomColor()
{
	int red = rand() % 255;
	int blue = rand() % 255;
	int green = rand() % 255;
	return new Color(red, green, blue);
}

void TagDrawingInfo::SetRect(int left, int top)
{
	this->boundRect = new RectF(left, top + TOP_OFFSET, this->width, this->height);
}

Font* TagDrawingInfo::SetFont()
{
	FontFamily* fontFamily = new FontFamily(L"Arial");
	Font* font = NULL;
	switch (this->rank)
	{
	case 1:
		font = new Font(fontFamily, 12, FontStyleRegular, UnitPixel);
		break;
	case 2:
		font = new Font(fontFamily, 16, FontStyleRegular, UnitPixel);
		break;
	case 3:
		font = new Font(fontFamily, 20, FontStyleRegular, UnitPixel);
		break;
	case 4:
		font = new Font(fontFamily, 24, FontStyleRegular, UnitPixel);
		break;
	}
	return font;
}

void TagDrawingInfo::Draw()
{
	Graphics graphics(this->hdc);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentCenter);
	SolidBrush brush(*color);
	graphics.DrawString(this->name, wcslen(this->name) + 1, font, *boundRect, &stringFormat, &brush);
}

int TagDrawingInfo::GetRank()
{
	return this->rank;
}

int TagDrawingInfo::GetWidth()
{
	return this->width;
}
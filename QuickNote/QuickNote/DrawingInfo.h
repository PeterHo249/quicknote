#pragma once
#include "vector"
#include "TagDrawingInfo.h"

using namespace std;

#define TOP_OFFSET 50

class DrawingInfo
{
private:
	HDC hdc;
	int lineCount;
	int lineLength;
	vector<TagDrawingInfo*> tagInfoList;
	int windowHeight;
	int windowWidth;
public:
	DrawingInfo();
	DrawingInfo(HDC hdc, NoteBook* noteBook, int windowsHeight, int windowWidth);
	~DrawingInfo();

	void Draw();
	void GetData(NoteBook* noteBook);
	void ComputeLineParameter();
	void AssignPosition();
	void SortTagInfoList();
};


#pragma once
#include "vector"
#include "TagDrawingInfo.h"
using namespace std;

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

	// Draw all tag to dialog window
	void Draw();

	// Get tag information from tag list in notebook
	void GetData(NoteBook* noteBook);

	// Compute line count and length base on tag info
	void ComputeLineParameter();

	// Assign position on dialog for tag string
	void AssignPosition();

	// Sort tag info list base on rank of tag
	void SortTagInfoList();
};


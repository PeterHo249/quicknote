#pragma once
#include "vector"
#include "TagDrawingInfo.h"
using namespace std;

class DrawingInfo
{
private:
	// Private member
	HDC hdc;
	vector<TagDrawingInfo*> tagInfoList;

	//==========================================================================

	// Private methods

	// Get tag information from tag list in notebook
	void GetData(NoteBook* noteBook);
	// Assign position on dialog for tag string
	void AssignPosition();
	// Sort tag info list base on rank of tag
	void SortTagInfoList();

public:
	DrawingInfo();
	DrawingInfo(HDC hdc, NoteBook* noteBook);
	~DrawingInfo();

	//==========================================================================

	// Draw all tag to dialog window
	void Draw();
};


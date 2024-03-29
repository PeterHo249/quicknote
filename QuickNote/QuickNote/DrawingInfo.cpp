#include "stdafx.h"
#include "DrawingInfo.h"


DrawingInfo::DrawingInfo()
{
}
//******************************************************************************
DrawingInfo::~DrawingInfo()
{
	for (unsigned int i = 0; i < tagInfoList.size(); i++)
	{
		if (tagInfoList[i] != NULL)
		{
			delete tagInfoList[i];
			tagInfoList[i] = NULL;
		}
	}
}
//******************************************************************************
DrawingInfo::DrawingInfo(HDC hdc, NoteBook* noteBook)
{
	this->hdc = hdc;
	GetData(noteBook);
	AssignPosition();
}

//==============================================================================

void DrawingInfo::Draw()
{
	for (unsigned int i = 0; i < tagInfoList.size(); i++)
	{
		tagInfoList[i]->Draw();
	}
}

//==============================================================================

void DrawingInfo::GetData(NoteBook* noteBook)
{
	vector<vector<Tag*>> data = noteBook->GetTagList();

	for (unsigned int i = 0; i < data.size(); i++)
	{
		for (unsigned int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j]->GetNoteList().size() == 0)
				continue;

			WCHAR* temp = data[i][j]->GetName();
			int length = wcslen(temp);
			WCHAR* inputName = new WCHAR[length + 1];
			ZeroMemory(inputName, (length + 1) * sizeof(WCHAR));
			wcscpy_s(inputName, length + 1, temp);

			float ratio = (float)data[i][j]->GetNoteList().size() / 
				noteBook->GetTagCount();
			
			tagInfoList.push_back(
				new TagDrawingInfo(this->hdc, inputName, (int)(ratio * 100)));
		}
	}

	SortTagInfoList();

	// Trim list into size 50
	while (tagInfoList.size() > 50)
	{
		if (tagInfoList[tagInfoList.size() - 1] != NULL)
			delete tagInfoList[tagInfoList.size() - 1];

		tagInfoList.pop_back();
	}
}
//******************************************************************************
void DrawingInfo::SortTagInfoList()
{
	int size = this->tagInfoList.size();

	for (int i = 0; i < size - 1; i++)
	{
		if (tagInfoList[i]->GetRank() == 4)
			continue;

		for (int j = i + 1; j < size; j++)
		{
			if (tagInfoList[i]->GetRank() < tagInfoList[j]->GetRank())
			{
				TagDrawingInfo* temp = tagInfoList[i];
				tagInfoList[i] = tagInfoList[j];
				tagInfoList[j] = temp;
			}
		}
	}
}
//******************************************************************************
void DrawingInfo::AssignPosition()
{
	if (this->tagInfoList.size() == 0)
		return;

	int tempLeft, tempTop, tempRight, tempBottom;
	int currentDirection;

	RectF* lastRect = NULL;
	Graphics graphics(this->hdc);

	// Assign for item 0
	tempLeft = 350;
	tempTop = 250;
	tempRight = tempLeft + this->tagInfoList[0]->GetWidth();
	tempBottom = tempTop + this->tagInfoList[0]->GetHeight();
	currentDirection = topMove;
	this->tagInfoList[0]->SetRect(tempLeft, tempTop);
	lastRect = this->tagInfoList[0]->GetRect();

	for (unsigned int i = 1; i < this->tagInfoList.size(); i++)
	{
		bool changeDirection = false;

		int top = 0;
		int left = 0;

		switch (currentDirection)
		{
		case topMove:
			left = lastRect->GetRight() - this->tagInfoList[i]->GetWidth();
			top = lastRect->GetTop() - this->tagInfoList[i]->GetHeight() - 3;
			if (top + this->tagInfoList[i]->GetHeight() < tempTop)
			{
				changeDirection = true;
				tempTop = top;
			}
			break;
		case rightMove:
			left = lastRect->GetRight() + 3;
			top = lastRect->GetTop();
			if (left > tempRight)
			{
				changeDirection = true;
				tempRight = left + this->tagInfoList[i]->GetWidth();
			}
			else
			{
				if (left + this->tagInfoList[i]->GetWidth() > tempRight)
				{
					tempRight = left + this->tagInfoList[i]->GetWidth();
				}
			}
			break;
		case bottomMove:
			left = lastRect->GetLeft();
			top = lastRect->GetBottom() + 3;
			if (top > tempBottom)
			{
				changeDirection = true;
				tempBottom = top + this->tagInfoList[i]->GetHeight();
			}
			break;
		case leftMove:
			left = lastRect->GetLeft() - this->tagInfoList[i]->GetWidth() - 6;
			top = lastRect->GetTop();
			if (left + this->tagInfoList[i]->GetWidth() < tempLeft)
			{
				changeDirection = true;
				tempLeft = left;
			}
			else
			{
				if (left < tempLeft)
				{
					tempLeft = left;
				}
			}
			break;
		}

		this->tagInfoList[i]->SetRect(left, top);
		lastRect = this->tagInfoList[i]->GetRect();

		if (changeDirection)
			currentDirection = (currentDirection + 1) % 4;
	}
}
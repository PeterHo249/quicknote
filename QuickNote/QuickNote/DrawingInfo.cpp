#include "stdafx.h"
#include "DrawingInfo.h"


DrawingInfo::DrawingInfo()
{
}


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

//HDC hdc;
//int lineCount;
//int lineLength;
//vector<TagDrawingInfo*> tagInfoList;
//int windowHeight;
//int windowWidth;

DrawingInfo::DrawingInfo(HDC hdc, NoteBook* noteBook, int windowsHeight, int windowWidth)
{
	this->windowHeight = windowsHeight;
	this->windowWidth = windowWidth;
	this->hdc = hdc;
	GetData(noteBook);
	ComputeLineParameter();
	AssignPosition();
}

void DrawingInfo::Draw()
{
	for (unsigned int i = 0; i < tagInfoList.size(); i++)
	{
		tagInfoList[i]->Draw();
	}
}

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

			int rank = 0;
			float ratio = (float)data[i][j]->GetNoteList().size() / noteBook->GetTagCount();

			if (ratio >= 0.75f)
				rank = 4;
			else
			{
				if (ratio >= 0.5f)
					rank = 3;
				else
				{
					if (ratio >= 0.25f)
						rank = 2;
					else
						rank = 1;
				}
			}

			tagInfoList.push_back(new TagDrawingInfo(this->hdc, inputName, rank));
		}
	}

	SortTagInfoList();

	// Trim list into size 100
	while (tagInfoList.size() > 100)
	{
		if (tagInfoList[tagInfoList.size() - 1] != NULL)
			delete tagInfoList[tagInfoList.size() - 1];

		tagInfoList.pop_back();
	}
}

void DrawingInfo::ComputeLineParameter()
{
	int totalLength = 0;

	for (unsigned int i = 0; i < tagInfoList.size(); i++)
		totalLength += tagInfoList[i]->GetWidth();

	lineLength = windowWidth - 40;
	lineCount = totalLength / lineLength;
	
	if (lineCount < 10)
	{
		lineCount = 10;
		lineLength = totalLength / lineCount;
	}
}

void DrawingInfo::AssignPosition()
{
	int centerHeight = this->windowHeight / 2;
	int centerWidth = this->windowWidth / 2;

	int centerLine = lineCount / 2;

	// Initial value
	vector<vector<int>> offsetOnLine;
	for (int i = 0; i < 2; i++)
	{
		vector<int> temp;
		offsetOnLine.push_back(temp);
		for (int j = 0; j < lineCount; j++)
		{
			offsetOnLine[i].push_back(centerWidth);
		}
	}

	int lineNumberUp = centerLine;
	int lineNumberDown = centerLine + 1;

	for (unsigned int i = 0; i < this->tagInfoList.size(); i++)
	{
		int peekLine;
		int peekTop;
		int peekLeft;

		if (i % 2 == 0)
		{
			peekLine = lineNumberUp;
			peekTop = (peekLine + 1) * 30;
			lineNumberUp--;
			if (lineNumberUp < 0)
				lineNumberUp = centerLine;
		}
		else
		{
			peekLine = lineNumberDown;
			peekTop = (peekLine + 1) * 30;
			lineNumberDown++;
			if (lineNumberDown >= lineCount)
				lineNumberDown = centerLine + 1;
		}

		// 0 is left, 1 is right
		if (centerWidth - offsetOnLine[0][peekLine] < offsetOnLine[1][peekLine] - centerWidth)
		{
			peekLeft = offsetOnLine[0][peekLine] - tagInfoList[i]->GetWidth();
			offsetOnLine[0][peekLine] = peekLeft;
		}
		else
		{
			peekLeft = offsetOnLine[1][peekLine];
			offsetOnLine[1][peekLine] = peekLeft + tagInfoList[i]->GetWidth();
		}

		tagInfoList[i]->SetRect(peekLeft, peekTop);
	}
}

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
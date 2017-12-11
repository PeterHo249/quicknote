#pragma once
#include "vector"
#include "TagDrawingInfo.h"

using namespace std;

class DrawingInfo
{
private:
	int lineCount;
	int lineLength;
	vector<TagDrawingInfo*> tagInfoList;
public:
	DrawingInfo();
	~DrawingInfo();
};


#include "stdafx.h"

Tag::Tag()
{
	this->name = NULL;
	this->hashCode = 0;
	this->noteIndexList.clear();
}
//******************************************************************************
Tag::Tag(WCHAR* name)
{
	this->name = name;
	this->hashCode = ComputeHashCode();
	this->noteIndexList.clear();
}
//******************************************************************************
Tag::Tag(WCHAR* name, WCHAR* notelist, int hashcode)
{
	this->name = name;
	this->hashCode = hashcode;

	this->noteIndexList.clear();
	WCHAR numString[5];
	ZeroMemory(numString, 5 * sizeof(WCHAR));
	int tempIndex = 0;
	int numIndex = 0;
	for (; notelist[tempIndex] != 0; tempIndex++)
	{
		if (notelist[tempIndex] == L' ')
		{
			this->noteIndexList.push_back(_wtoi(numString));
			ZeroMemory(numString, 5 * sizeof(WCHAR));
			numIndex = 0;
		}
		else
		{
			numString[numIndex] = notelist[tempIndex];
			numIndex++;
		}
	}
	this->noteIndexList.push_back(_wtoi(numString));
}
//******************************************************************************
Tag::~Tag()
{
	if (this->name != NULL)
		delete[] this->name;

	this->noteIndexList.clear();
}

//==============================================================================

void Tag::AddNoteIndex(int noteIndex)
{
	this->noteIndexList.push_back(noteIndex);
}
//******************************************************************************
void Tag::DeleteNoteIndex(int noteIndex)
{
	for (unsigned int i = 0; i < this->noteIndexList.size(); i++)
	{
		if (this->noteIndexList[i] == noteIndex)
			this->noteIndexList.erase(this->noteIndexList.begin() + i);
	}
}
//******************************************************************************
int Tag::ComputeHashCode()
{
	int code = 0;
	for (int i = 0; this->name[i] != 0; i++)
	{
		code += this->name[i];
	}
	return code % 131;
}
//******************************************************************************
WCHAR* Tag::ConvertIndexListToString()
{
	int length = this->noteIndexList.size();
	WCHAR* buffer = new WCHAR[length * 5 + 1];
	ZeroMemory(buffer, (length * 5 + 1) * sizeof(WCHAR));

	if (length == 0)
		return buffer;

	for (int i = 0; i < length; i++)
	{
		if (i == 0)
		{
			wsprintf(buffer, L"%d", this->noteIndexList[i]);
		}
		else
		{
			wsprintf(buffer, L"%s %d", buffer, this->noteIndexList[i]);
		}
	}

	return buffer;
}

//==============================================================================

WCHAR* Tag::GetName()
{
	return this->name;
}
//******************************************************************************
WCHAR* Tag::GetNoteIndexString()
{
	return ConvertIndexListToString();
}
//******************************************************************************
vector<int> Tag::GetNoteList()
{
	return this->noteIndexList;
}
//******************************************************************************
Tag* Tag::GetAddress()
{
	return this;
}
//******************************************************************************
int Tag::GetHashCode()
{
	return hashCode;
}

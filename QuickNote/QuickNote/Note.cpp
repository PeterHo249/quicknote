#include "stdafx.h"

Note::Note()
{
	this->date = NULL;
	this->content = NULL;
	this->previewNote = NULL;
	this->tagString = NULL;
}
//******************************************************************************
Note::Note(WCHAR* content, WCHAR* tagString)
{
	this->date = GetCurrentDate();
	this->content = content;
	this->tagString = tagString;
	StandardizeTagString();
	StandardizeContent();
	this->previewNote = GeneratePreviewNote();
}
//******************************************************************************
Note::Note(WCHAR* date, WCHAR* content, WCHAR* tagstring)
{
	this->date = date;
	this->content = content;
	this->tagString = tagstring;
	StandardizeTagString();
	StandardizeContent();
	this->previewNote = GeneratePreviewNote();
}
//******************************************************************************
Note::~Note()
{
	if (date != NULL)
		delete[] date;

	if (content != NULL)
		delete[] content;

	if (previewNote != NULL)
		delete[] previewNote;

	if (tagString != NULL)
		delete[] tagString;
}

//==============================================================================

WCHAR* Note::GeneratePreviewNote()
{
	WCHAR* buffer = new WCHAR[21];
	ZeroMemory(buffer, 21 * sizeof(WCHAR));

	for (int i = 0; i < 17; i++)
	{
		if (this->content[i] == 0)
			break;

		buffer[i] = this->content[i];
	}

	wsprintf(buffer, L"%s...", buffer);

	return buffer;
}

//==============================================================================

WCHAR* Note::GetCurrentDate()
{
	time_t current_time = time(NULL);
	tm time_ptr;

	localtime_s(&time_ptr, &current_time);

	WCHAR* buffer = new WCHAR[15];
	ZeroMemory(buffer, 15 * sizeof(WCHAR));
	wsprintf(buffer, L"%d/%d/%d", 
		time_ptr.tm_mday, time_ptr.tm_mon + 1, time_ptr.tm_year + 1900);

	return buffer;
}
//******************************************************************************
WCHAR* Note::GetDate()
{
	return this->date;
}
//******************************************************************************
WCHAR* Note::GetContent()
{
	return this->content;
}
//******************************************************************************
WCHAR* Note::GetTagString()
{
	return this->tagString;
}
//******************************************************************************
WCHAR* Note::GetPreview()
{
	return this->previewNote;
}

//==============================================================================

void Note::StandardizeTagString()
{
	int length = wcslen(this->tagString);
	WCHAR* newtagString = new WCHAR[length + 1];
	ZeroMemory(newtagString, (length + 1) * sizeof(WCHAR));

	int newtagIndex = 0;
	bool validFlag = false;
	for (int i = 0; i < length; i++)
	{
		if (this->tagString[i] == L' ' || this->tagString[i] == L'\r' 
			|| this->tagString[i] == L'\n')
			continue;

		if (this->tagString[i] == L',' && i != (length - 1))
		{
			if (validFlag)
			{
				newtagString[newtagIndex] = L',';
				newtagIndex++;
				newtagString[newtagIndex] = L' ';
				newtagIndex++;
				validFlag = false;
				continue;
			}
			else
			{
				continue;
			}
		}

		newtagString[newtagIndex] = this->tagString[i];
		newtagIndex++;
		validFlag = true;
	}

	delete[] this->tagString;
	this->tagString = newtagString;
}

//==============================================================================
void Note::StandardizeContent()
{
	int length = wcslen(this->content);
	for (int i = 0; i < length; i++)
	{
		if (this->content[i] == L'\r' || this->content[i] == L'\n')
			this->content[i] = L' ';
	}
}
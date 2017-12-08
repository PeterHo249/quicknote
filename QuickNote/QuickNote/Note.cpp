#include "stdafx.h"

Note::Note()
{
	this->date = NULL;
	this->content = NULL;
	this->previewNote = NULL;
	this->tagString = NULL;
}

Note::Note(WCHAR* content, WCHAR* tagString)
{
	this->date = GetCurrentDate();
	this->content = content;
	this->tagString = tagString;
	StandardizeTagString();
	this->previewNote = GeneratePreviewNote();
}

Note::Note(WCHAR* date, WCHAR* content, WCHAR* tagstring)
{
	this->date = date;
	this->content = content;
	this->tagString = tagstring;
	StandardizeTagString();
	this->previewNote = GeneratePreviewNote();
}

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

// Get current date (format dd/MM/yyyy)
WCHAR* Note::GetCurrentDate()
{
	time_t current_time = time(NULL);
	tm time_ptr;

	localtime_s(&time_ptr, &current_time);

	WCHAR* buffer = new WCHAR[15];
	ZeroMemory(buffer, 15 * sizeof(WCHAR));
	wsprintf(buffer, L"%d/%d/%d", time_ptr.tm_mday, time_ptr.tm_mon + 1, time_ptr.tm_year + 1900);

	return buffer;
}

// Generate preview from content
WCHAR* Note::GeneratePreviewNote()
{
	WCHAR* buffer = new WCHAR[21];
	ZeroMemory(buffer, 21 * sizeof(WCHAR));

	for (int i = 0; i < 20; i++)
	{
		buffer[i] = this->content[i];
	}

	return buffer;
}

// Standardize tag string
void Note::StandardizeTagString()
{
	int length = wcslen(this->tagString);
	WCHAR* newtagString = new WCHAR[length + 1];
	ZeroMemory(newtagString, (length + 1 )* sizeof(WCHAR));
	
	int newtagIndex = 0;
	bool validFlag = false;
	for (int i = 0; i < length; i++)
	{
		if (this->tagString[i] == L' ')
			continue;

		if (this->tagString[i] == L',' && i != (length - 1))
		{
			newtagString[newtagIndex] = L',';
			newtagIndex++;
			newtagString[newtagIndex] = L' ';
			newtagIndex++;
			continue;
		}

		newtagString[newtagIndex] = this->tagString[i];
		newtagIndex++;
	}

	delete[] this->tagString;
	this->tagString = newtagString;
}

// Get
WCHAR* Note::GetDate()
{
	return this->date;
}

WCHAR* Note::GetContent()
{
	return this->content;
}

WCHAR* Note::GetTagString()
{
	return this->tagString;
}
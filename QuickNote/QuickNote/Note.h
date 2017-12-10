#pragma once

class Note
{
	WCHAR* date;
	WCHAR* previewNote;
	WCHAR* content;
	WCHAR* tagString;
public:
	Note();
	Note(WCHAR* content, WCHAR* tagString);
	Note(WCHAR* date, WCHAR* content, WCHAR* tagstring);
	~Note();

	// Get current date (format dd/MM/yyyy)
	WCHAR* GetCurrentDate();

	// Generate preview from content
	WCHAR* GeneratePreviewNote();

	// Standardize tag string
	void StandardizeTagString();

	// Get
	WCHAR* GetDate();
	WCHAR* GetContent();
	WCHAR* GetTagString();
	WCHAR* GetPreview();
};


#pragma once

class Note
{
	// Private members
	WCHAR* date;
	WCHAR* previewNote;
	WCHAR* content;
	WCHAR* tagString;

	//==========================================================================

	// Private methods

	// Generate preview from content
	WCHAR* GeneratePreviewNote();	
public:
	Note();
	Note(WCHAR* content, WCHAR* tagString);
	Note(WCHAR* date, WCHAR* content, WCHAR* tagstring);
	~Note();

	//==========================================================================

	// Getter
	WCHAR* GetDate();
	WCHAR* GetContent();
	WCHAR* GetTagString();
	WCHAR* GetPreview();
	// Get current date (format dd/MM/yyyy)
	WCHAR* GetCurrentDate();

	//==========================================================================

	// Standardize tag string
	void StandardizeTagString();
};


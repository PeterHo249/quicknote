#include "stdafx.h"

NoteBook::NoteBook()
{
	tagCount = 0;
	noteCount = 0;

	vector<Tag*> temp;
	for (int i = 0; i < 131; i++)
	{
		tagHashTable.push_back(temp);
	}

	dataPath = GetDataFilePath();
	if (!IsCreateNewDataFile())
		ReadDataFromFile();
}
//******************************************************************************
NoteBook::~NoteBook()
{
	CleanUpTagList();

	if (this->tagCount != 0 || this->noteCount != 0)
		WriteDataToFile();

	// Clear note list
	for (unsigned int i = 0; i < noteList.size(); i++)
	{
		if (noteList[i] != NULL)
		{
			delete noteList[i];
			noteList[i] = NULL;
		}
	}
	noteList.clear();

	for (int i = 0; i < 131; i++)
	{
		// Clear tag list
		if (tagHashTable[i].size() != 0)
		{
			for (unsigned int j = 0; j < tagHashTable[i].size(); j++)
			{
				if (tagHashTable[i][j] != NULL)
				{
					delete tagHashTable[i][j];
					tagHashTable[i][j] = NULL;
				}
			}
		}
	}
	tagHashTable.clear();

	// Clear blank position list
	blankPos.clear();

	// Clear data path
	if (dataPath != NULL)
	{
		delete dataPath;
		dataPath = NULL;
	}
}

//==============================================================================

WCHAR* NoteBook::GetDataFilePath()
{
	WCHAR* filePath = new WCHAR[MAX_FILE_PATH];
	GetCurrentDirectory(MAX_FILE_PATH, filePath);
	StrCat(filePath, L"\\data.ini");
	return filePath;
}
//******************************************************************************
bool NoteBook::InitDataFile()
{
	DWORD byteWrittenCount;
	if (this->dataPath != NULL)
	{
		if (!PathFileExists(this->dataPath))
		{
			HANDLE fileHandle = CreateFile(this->dataPath, 
				GENERIC_READ | GENERIC_WRITE,
				0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(fileHandle, &BOM, sizeof(BOM), &byteWrittenCount, NULL);
			CloseHandle(fileHandle);
			return true;
		}
	}
	return false;
}
//******************************************************************************
// Data file format
/*

[notebook]
tagcount=
notecount=
totalnote=
blankpos=

[tag1]
name=
notelist=
hashcode=

[note1]
date=
content=
tagstring=

*/

bool NoteBook::WriteDataToFile()
{
	WCHAR tempString[MAX_LOADSTRING];

	// Write global settings
	ZeroMemory(tempString, MAX_LOADSTRING * sizeof(WCHAR));
	wsprintf(tempString, L"%d", tagCount);
	WritePrivateProfileString(L"notebook", L"tagcount", tempString, dataPath);

	ZeroMemory(tempString, MAX_LOADSTRING * sizeof(WCHAR));
	wsprintf(tempString, L"%d", noteCount);
	WritePrivateProfileString(L"notebook", L"notecount", tempString, dataPath);

	ZeroMemory(tempString, MAX_LOADSTRING * sizeof(WCHAR));
	wsprintf(tempString, L"%d", noteList.size());
	WritePrivateProfileString(L"notebook", L"totalnote", tempString, dataPath);

	int blankPosSize = blankPos.size();
	WCHAR* blankPosString = new WCHAR[blankPosSize * 4];
	ZeroMemory(blankPosString, blankPosSize * 4 * sizeof(WCHAR));
	for (int i = 0; i < blankPosSize; i++)
	{
		if (i == 0)
		{
			wsprintf(blankPosString, L"%d", blankPos[i]);
		}
		else
		{
			wsprintf(blankPosString, L"%s %d", blankPosString, blankPos[i]);
		}
	}
	if (blankPosSize == 0)
		WritePrivateProfileString(L"notebook", L"blankpos", 
			L"", dataPath);
	else
		WritePrivateProfileString(L"notebook", L"blankpos", 
			blankPosString, dataPath);
	if (blankPosString != NULL)
	{
		delete[] blankPosString;
		blankPosString = NULL;
	}

	// Write tag
	int tagNum = 0;
	WCHAR* tagSection = new WCHAR[MAX_LOADSTRING];
	for (unsigned int i = 0; i < tagHashTable.size(); i++)
	{
		if (tagHashTable[i].size() != 0)
		{
			for (unsigned int j = 0; j < tagHashTable[i].size(); j++)
			{
				ZeroMemory(tagSection, MAX_LOADSTRING * sizeof(WCHAR));
				wsprintf(tagSection, L"tag%d", tagNum);

				WritePrivateProfileString(tagSection, L"name", 
					tagHashTable[i][j]->GetName(), dataPath);
				WritePrivateProfileString(tagSection, L"notelist", 
					tagHashTable[i][j]->GetNoteIndexString(), dataPath);
				ZeroMemory(tempString, MAX_LOADSTRING * sizeof(WCHAR));
				wsprintf(tempString, L"%d", tagHashTable[i][j]->GetHashCode());
				WritePrivateProfileString(tagSection, L"hashcode", 
					tempString, dataPath);

				tagNum++;
			}
		}
	}
	if (tagSection != NULL)
	{
		delete[] tagSection;
		tagSection = NULL;
	}

	// Write note
	int noteNum = 0;
	WCHAR* noteSection = new WCHAR[MAX_LOADSTRING];
	for (unsigned int i = 0; i < noteList.size(); i++)
	{
		ZeroMemory(noteSection, MAX_LOADSTRING * sizeof(WCHAR));
		wsprintf(noteSection, L"note%d", noteNum);

		if (noteList[i] != NULL)
		{
			WritePrivateProfileString(noteSection, L"date", 
				noteList[i]->GetDate(), dataPath);
			WritePrivateProfileString(noteSection, L"content", 
				noteList[i]->GetContent(), dataPath);
			WritePrivateProfileString(noteSection, L"tagstring", 
				noteList[i]->GetTagString(), dataPath);
		}
		else
		{
			WritePrivateProfileString(noteSection, NULL, NULL, dataPath);
		}

		noteNum++;
	}
	if (noteSection != NULL)
	{
		delete[] noteSection;
		noteSection = NULL;
	}

	return true;
}
//******************************************************************************
bool NoteBook::ReadDataFromFile()
{
	WCHAR* tempString = NULL;

	// Read global setting
	tempString = new WCHAR[5];
	ZeroMemory(tempString, 5 * sizeof(WCHAR));
	// Tag count
	GetPrivateProfileString(L"notebook", L"tagcount", 
		L"", tempString, 5, dataPath);
	if (wcslen(tempString) != 0)
		this->tagCount = _wtoi(tempString);
	else
		return false;

	// Note count
	ZeroMemory(tempString, 5 * sizeof(WCHAR));
	GetPrivateProfileString(L"notebook", L"notecount", 
		L"", tempString, 5, dataPath);
	if (wcslen(tempString) != 0)
		this->noteCount = _wtoi(tempString);

	// note total
	int totalnote = 0;
	ZeroMemory(tempString, 5 * sizeof(WCHAR));
	GetPrivateProfileString(L"notebook", L"totalnote", 
		L"", tempString, 5, dataPath);
	if (wcslen(tempString) != 0)
		totalnote = _wtoi(tempString);

	if (tempString != NULL)
	{
		delete[] tempString;
		tempString = NULL;
	}

	// Blank pos
	tempString = new WCHAR[MAX_LOADSTRING];
	ZeroMemory(tempString, MAX_LOADSTRING * sizeof(WCHAR));
	GetPrivateProfileString(L"notebook", L"blankpos", 
		L"", tempString, MAX_LOADSTRING, dataPath);
	if (wcslen(tempString) != 0)
	{
		this->blankPos.clear();
		WCHAR numString[5];
		ZeroMemory(numString, 5 * sizeof(WCHAR));
		int tempIndex = 0;
		int numIndex = 0;
		for (; tempString[tempIndex] != 0; tempIndex++)
		{
			if (tempString[tempIndex] == L' ')
			{
				this->blankPos.push_back(_wtoi(numString));
				ZeroMemory(numString, 5 * sizeof(WCHAR));
				numIndex = 0;
			}
			else
			{
				numString[numIndex] = tempString[tempIndex];
				numIndex++;
			}
		}
		this->blankPos.push_back(_wtoi(numString));
	}
	if (tempString != NULL)
	{
		delete[] tempString;
		tempString = NULL;
	}

	// Read tag
	WCHAR tagSection[15];
	for (int i = 0; i < tagCount; i++)
	{
		ZeroMemory(tagSection, 15 * sizeof(WCHAR));
		wsprintf(tagSection, L"tag%d", i);

		// Name
		WCHAR* nameString = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(nameString, MAX_LOADSTRING * sizeof(WCHAR));
		GetPrivateProfileString(tagSection, L"name", 
			L"", nameString, MAX_LOADSTRING, dataPath);
		if (wcslen(nameString) == 0)
		{
			if (nameString != NULL)
			{
				delete[] nameString;
				nameString = NULL;
			}
			continue;
		}

		//note list
		WCHAR* noteString = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(noteString, MAX_LOADSTRING * sizeof(WCHAR));
		GetPrivateProfileString(tagSection, L"notelist", 
			L"", noteString, MAX_LOADSTRING, dataPath);
		if (wcslen(noteString) == 0)
		{
			if (noteString != NULL)
			{
				delete[] noteString;
				noteString = NULL;
			}
			continue;
		}

		// hash code
		WCHAR* codeString = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(codeString, MAX_LOADSTRING * sizeof(WCHAR));
		GetPrivateProfileString(tagSection, L"hashcode", 
			L"", codeString, MAX_LOADSTRING, dataPath);
		if (wcslen(codeString) == 0)
		{
			if (codeString != NULL)
			{
				delete[] codeString;
				codeString = NULL;
			}
			continue;
		}
		int hashcode = _wtoi(codeString);

		Tag* tagTemp = new Tag(nameString, noteString, hashcode);
		tagHashTable[hashcode].push_back(tagTemp);
	}

	// Read note
	noteList.clear();
	WCHAR noteSection[15];
	for (int i = 0; i < totalnote; i++)
	{
		ZeroMemory(noteSection, 15 * sizeof(WCHAR));
		wsprintf(noteSection, L"note%d", i);

		// date
		WCHAR* dateString = new WCHAR[20];
		ZeroMemory(dateString, 20 * sizeof(WCHAR));
		GetPrivateProfileString(noteSection, L"date", 
			L"", dateString, 20, dataPath);

		// content
		WCHAR* contentString = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(contentString, MAX_LOADSTRING * sizeof(WCHAR));
		GetPrivateProfileString(noteSection, L"content", 
			L"", contentString, MAX_LOADSTRING, dataPath);

		// tag string
		WCHAR* tagString = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(tagString, MAX_LOADSTRING * sizeof(WCHAR));
		GetPrivateProfileString(noteSection, L"tagstring", 
			L"", tagString, MAX_LOADSTRING, dataPath);

		if (wcslen(dateString) == 0 || wcslen(contentString) == 0 || 
			wcslen(tagString) == 0)
		{
			if (dateString != NULL)
			{
				delete[] dateString;
				dateString = NULL;
			}
			if (contentString != NULL)
			{
				delete[] contentString;
				contentString = NULL;
			}
			if (tagString != NULL)
			{
				delete[] tagString;
				tagString = NULL;
			}
			this->noteList.push_back(NULL);
		}
		else
		{
			Note* noteTemp = new Note(dateString, contentString, tagString);
			noteList.push_back(noteTemp);
		}
	}

	return true;
}
//******************************************************************************
bool NoteBook::IsCreateNewDataFile()
{
	return InitDataFile();
}

//==============================================================================

void NoteBook::AddNewTagFromFile(Tag* value)
{
	this->tagHashTable[value->GetHashCode()].push_back(value);
}
//******************************************************************************
void NoteBook::AddNewNoteFromFile(Note* value)
{
	this->noteList.push_back(value);
}
//******************************************************************************
void NoteBook::AddNewNote(Note* value)
{
	WCHAR* tagToken = new WCHAR[MAX_LOADSTRING];
	ZeroMemory(tagToken, MAX_LOADSTRING * sizeof(WCHAR));
	WCHAR* tagString = value->GetTagString();

	// Add note to note list, pos is note index
	int pos = 0;
	if (this->blankPos.size() == 0)
	{
		this->noteList.push_back(value);
		pos = this->noteList.size() - 1;
	}
	else
	{
		pos = this->blankPos.back();
		this->noteList[pos] = value;
		this->blankPos.pop_back();
	}
	noteCount++;

	// Add note index to tag
	int tokenIndex = 0;
	for (int i = 0; tagString[i] != 0; i++)
	{
		if (tagString[i] == L',')
		{
			AddNoteIndex(pos, tagToken);

			// Reset token string
			i++;
			tokenIndex = 0;
			ZeroMemory(tagToken, MAX_LOADSTRING * sizeof(WCHAR));
			continue;
		}

		tagToken[tokenIndex] = tagString[i];
		tokenIndex++;
	}
	// The last token
	AddNoteIndex(pos, tagToken);

	// Free token string
	if (tagToken != NULL)
	{
		delete[] tagToken;
		tagToken = NULL;
	}
}
//******************************************************************************
void NoteBook::AddNewTag(Tag* value)
{
	this->tagHashTable[value->GetHashCode()].push_back(value);
	tagCount++;
}
//******************************************************************************
void NoteBook::DeteleNote(Note* value)
{
	int pos = 0;
	// Get note position
	for (unsigned int i = 0; i < this->noteList.size(); i++)
	{
		if (noteList[i] == value)
		{
			pos = i;
			break;
		}
	}

	// Delete note index in tag
	Tag* tagTemp = NULL;
	WCHAR* tagToken = new WCHAR[MAX_LOADSTRING];
	ZeroMemory(tagToken, MAX_LOADSTRING * sizeof(WCHAR));
	int tokenIndex = 0;
	for (int i = 0; value->GetTagString()[i] != 0; i++)
	{
		if (value->GetTagString()[i] == L',')
		{
			tagTemp = SearchTag(tagToken);

			if (tagTemp != NULL)
				tagTemp->DeleteNoteIndex(pos);

			// Reset token string
			i++;
			tokenIndex = 0;
			ZeroMemory(tagToken, MAX_LOADSTRING * sizeof(WCHAR));
			continue;
		}

		tagToken[tokenIndex] = value->GetTagString()[i];
		tokenIndex++;
	}
	// Final token
	tagTemp = SearchTag(tagToken);

	if (tagTemp != NULL)
		tagTemp->DeleteNoteIndex(pos);

	// Delete note
	if (noteList[pos] != NULL)
	{
		delete noteList[pos];
		noteList[pos] = NULL;
		blankPos.push_back(pos);
		noteCount--;
	}

	// Free token string
	if (tagToken != NULL)
	{
		delete[] tagToken;
		tagToken = NULL;
	}
}
//******************************************************************************
void NoteBook::ModifyNote(int pos, WCHAR* content, WCHAR* tagString)
{
	Note* noteTemp = this->GetNoteAt(pos);
	this->DeteleNote(noteTemp);
	noteTemp = new Note(content, tagString);
	this->AddNewNote(noteTemp);
}
//******************************************************************************
void NoteBook::AddNoteIndex(int pos, WCHAR* tagToken)
{
	// Search tag
	Tag* tagTemp = NULL;
	int code = GetHashCode(tagToken);
	if (this->tagHashTable[code].size() == 0)
	{
		WCHAR* newTagToken = new WCHAR[MAX_LOADSTRING];
		ZeroMemory(newTagToken, MAX_LOADSTRING * sizeof(WCHAR));
		wcscpy_s(newTagToken, MAX_LOADSTRING, tagToken);
		tagTemp = new Tag(newTagToken);
		AddNewTag(tagTemp);
	}
	else
	{
		bool found = false;

		for (unsigned int i = 0; i < this->tagHashTable[code].size(); i++)
		{
			WCHAR* tagName = this->tagHashTable[code][i]->GetName();
			if (wcscmp(tagToken, tagName) == 0)
			{
				tagTemp = this->tagHashTable[code][i];
				found = true;
				break;
			}
		}

		if (!found)
		{
			WCHAR* newTagToken = new WCHAR[MAX_LOADSTRING];
			ZeroMemory(newTagToken, MAX_LOADSTRING * sizeof(WCHAR));
			wcscpy_s(newTagToken, MAX_LOADSTRING, tagToken);
			tagTemp = new Tag(newTagToken);
			AddNewTag(tagTemp);
		}
	}

	// Add note index to tag
	tagTemp->AddNoteIndex(pos);
}
//******************************************************************************
Tag* NoteBook::SearchTag(WCHAR* name)
{
	int code = GetHashCode(name);

	// Not found
	if (this->tagHashTable[code].size() == 0)
	{
		return NULL;
	}
	else
	{
		for (unsigned int i = 0; i < this->tagHashTable[code].size(); i++)
		{
			WCHAR* tagName = this->tagHashTable[code][i]->GetName();
			if (wcscmp(name, tagName) == 0)
			{
				return this->tagHashTable[code][i];
			}
		}
	}

	return NULL;
}
//******************************************************************************
void NoteBook::AddNewNote(WCHAR* content, WCHAR* tagString)
{
	Note* noteTemp = new Note(content, tagString);
	this->AddNewNote(noteTemp);
}
//******************************************************************************
void NoteBook::DeleteNoteAt(int pos)
{
	Note* noteTemp = GetNoteAt(pos);
	this->DeteleNote(noteTemp);
}

//==============================================================================

int NoteBook::GetTagCount()
{
	return this->tagCount;
}
//******************************************************************************
int NoteBook::GetNoteCount()
{
	return this->noteCount;
}
//******************************************************************************
vector<Note*> NoteBook::GetNoteList()
{
	return this->noteList;
}
//******************************************************************************
int GetHashCode(WCHAR* string)
{
	int code = 0;
	for (int i = 0; string[i] != 0; i++)
	{
		code += string[i];
	}
	return code % 131;
}
//******************************************************************************
vector<vector<Tag*>> NoteBook::GetTagList()
{
	return this->tagHashTable;
}
//******************************************************************************
Note* NoteBook::GetNoteAt(int pos)
{
	if (pos >= 0 && pos < this->noteList.size())
		return this->noteList[pos];
	else
		return NULL;
}
//******************************************************************************
Tag* NoteBook::GetTag(int pos)
{
	int index = 0;

	for (unsigned int i = 0; i < this->tagHashTable.size(); i++)
	{
		for (unsigned int j = 0; j < this->tagHashTable[i].size(); j++)
		{
			if (index == pos)
				return this->tagHashTable[i][j];

			index++;
		}
	}

	return NULL;
}

//==============================================================================

void NoteBook::CleanUpTagList()
{
	for (unsigned int i = 0; i < this->tagHashTable.size(); i++)
	{
		for (unsigned int j = 0; j < this->tagHashTable[i].size(); j++)
		{
			if (this->tagHashTable[i][j]->GetNoteList().size() == 0)
			{
				delete this->tagHashTable[i][j];
				tagHashTable[i].erase(tagHashTable[i].begin() + j);
				j--;
				tagCount--;
			}
		}
	}
}
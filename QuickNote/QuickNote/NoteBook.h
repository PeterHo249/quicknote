#pragma once
#include <vector>
using namespace std;
class NoteBook
{
private:
	// Private members
	int tagCount;
	int noteCount;
	vector< vector<Tag*> > tagHashTable;
	vector<Note*> noteList;
	vector<int> blankPos;
	WCHAR* dataPath;

	//==========================================================================

	// Data file business function

	// Get path of data file
	WCHAR* GetDataFilePath();
	// Initial data file
	bool InitDataFile();
	// Write data to file
	bool WriteDataToFile();
	// Read data from file
	bool ReadDataFromFile();
	// Check the data file is new
	bool IsCreateNewDataFile();

	// Clean up tag list
	void CleanUpTagList();

public:
	NoteBook();
	~NoteBook();

	//==========================================================================

	// Note and Tag business function
	
	// Add tag read from file
	void AddNewTagFromFile(Tag* value);
	// Add note read from file
	void AddNewNoteFromFile(Note* value);
	// Add new note
	void AddNewNote(Note* value);
	// Add new note (override)
	void AddNewNote(WCHAR* content, WCHAR* tagString);
	// Add a note index to tag
	void NoteBook::AddNoteIndex(int pos, WCHAR* tagToken);

	// Delete a note
	void DeteleNote(Note* value);
	// Delete note in notebook base on index
	void DeleteNoteAt(int pos);

	// Modify a note
	void ModifyNote(int pos, WCHAR* content, WCHAR* tagString);

	// Add new tag
	void AddNewTag(Tag* value);
	// Search a tag base on its name
	Tag* SearchTag(WCHAR* name);	

	//==========================================================================

	// Getter
	int GetTagCount();
	int GetNoteCount();
	vector<Note*> GetNoteList();
	vector<vector<Tag*>> GetTagList();
	Note* GetNoteAt(int pos);
	Tag* GetTag(int pos);
};

int GetHashCode(WCHAR* string);
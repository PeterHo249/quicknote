#pragma once
#include <vector>
using namespace std;
class NoteBook
{
private:
	int tagCount;
	int noteCount;
	vector< vector<Tag*> > tagHashTable;
	vector<Note*> noteList;
	vector<int> blankPos;
	WCHAR* dataPath;
public:
	NoteBook();
	~NoteBook();

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

	// Add tag read from file
	void AddNewTagFromFile(Tag* value);

	// Add note read from file
	void AddNewNoteFromFile(Note* value);

	// Add new note
	void AddNewNote(Note* value);

	// Add new note (override)
	void AddNewNote(WCHAR* content, WCHAR* tagString);

	// Add new tag
	void AddNewTag(Tag* value);

	// Delete a note
	void DeteleNote(Note* value);

	// Modify a note
	void ModifyNote(int pos, WCHAR* content, WCHAR* tagString);

	// Add a note index to tag
	void NoteBook::AddNoteIndex(int pos, WCHAR* tagToken);

	// Search a tag base on its name
	Tag* SearchTag(WCHAR* name);

	vector<vector<Tag*>> GetTagList();

	Note* GetNoteAt(int pos);

	Tag* GetTag(int pos);

	void DeleteNoteAt(int pos);

	int GetTagCount();

	int GetNoteCount();

	vector<Note*> GetNoteList();
};

int GetHashCode(WCHAR* string);


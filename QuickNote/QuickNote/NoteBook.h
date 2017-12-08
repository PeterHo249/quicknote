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

	// Add tag
	void AddNewTag(Tag* value);

	// Add note
	void AddNewNote(Note* value);
};


#pragma once
#include <vector>
using namespace std;
class Tag
{
	WCHAR* name;
	int hashCode;
	vector<int> noteIndexList;
public:
	Tag();
	Tag(WCHAR* name);
	Tag(WCHAR* name, WCHAR* notelist, int hashcode);
	~Tag();

	// Add note index
	void AddNoteIndex(int noteIndex);

	// Delete note index
	void DeleteNoteIndex(int noteIndex);

	// Compute hash code
	int ComputeHashCode();

	// Get hash code
	int GetHashCode();

	// Convert noteIndexList to string
	WCHAR* ConvertIndexListToString();

	// Get
	WCHAR* GetName();
	WCHAR* GetNoteIndexString();
	vector<int> GetNoteList();
	Tag* GetAddress();
};


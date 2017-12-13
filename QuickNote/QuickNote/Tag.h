#pragma once
#include <vector>
using namespace std;
class Tag
{
	// Private members
	WCHAR* name;
	int hashCode;
	vector<int> noteIndexList;

	//==========================================================================

	// Private methods

	// Compute hash code
	int ComputeHashCode();
public:
	Tag();
	Tag(WCHAR* name);
	Tag(WCHAR* name, WCHAR* notelist, int hashcode);
	~Tag();

	//==========================================================================

	// Add note index
	void AddNoteIndex(int noteIndex);
	// Delete note index
	void DeleteNoteIndex(int noteIndex);
	// Convert noteIndexList to string
	WCHAR* ConvertIndexListToString();

	//==========================================================================

	// Getter
	WCHAR* GetName();
	WCHAR* GetNoteIndexString();
	vector<int> GetNoteList();
	Tag* GetAddress();
	int GetHashCode();
};


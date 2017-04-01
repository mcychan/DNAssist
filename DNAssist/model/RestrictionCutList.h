#pragma once
#include "BubbleSort.h"
#include "RestrictionEnzymeCut.h"

#include <algorithm>
#include <vector>
using namespace std;

class CRestrictionCutList
{
public:
	CRestrictionCutList();
	~CRestrictionCutList();
	CRestrictionCutList(const CRestrictionCutList& passedobject);
	void AddElement(const string& enzymename, const string& recogsite, const string& siteformatted, vector<int> cleavagepos, int attr, char seq_form, int seq_length);
	void AddEntry(const CRestrictionEnzymeCut& array);
	string GetName(int index);
	size_t GetNameLength(int index);
	string GetSite(int index);
	int GetNumberOfCutPositions(int index);
	int* GetArray(int index);
	int GetArrayLength(int index);
	int GetArrayEntry(int index, int offset);
	int GetTotalCutPos();
	void MakeSequentialArray();
	string GetSequentialName(int index);
	int GetSequentialCutPos(int index);
	bool GetSequentialData(int index);
	int GetSequentialPointer(int index);

	size_t GetSelectedEnzymeArray(vector<int>& selectedenzymes);
	int* SortEnzymesByCutFrequency(vector<int>& cuts);
	inline vector<string> GetSeqNames() const {
		return sequentialnames;
	}
	inline vector<int> GetSeqCutPos() const {
		return sequentialcutpos;
	}
	inline vector<int> GetSeqSortPoint() const {
		return sortpointers;
	}
	inline CRestrictionEnzymeCut* GetPointer(int index) {
		return &array[index];
	}
	inline size_t size() const {
		return array.size();
	}

protected:
	CBubbleSort sortobject;
	vector<CRestrictionEnzymeCut> array;
	vector<bool> sequentialdata;
	vector<int> sequentialcutpos, sortpointers;
	vector<string> sequentialnames;	

};

#include "RestrictionCutList.h"

CRestrictionCutList::CRestrictionCutList()
{
}

CRestrictionCutList::CRestrictionCutList(const CRestrictionCutList& passedobject)
{
	array = passedobject.array;
	sequentialnames = passedobject.sequentialnames;
	sequentialcutpos = passedobject.sequentialcutpos;
	sequentialdata = passedobject.sequentialdata;
	sortpointers = passedobject.sortpointers;
}

CRestrictionCutList::~CRestrictionCutList()
{
	array.clear();
	sequentialnames.clear();
}

void CRestrictionCutList::AddElement(const string& enzymename, const string& recogsite, const string& siteformatted,
															  vector<int> cleavagepos, int attr,
                                               char seq_form, int seq_length)
{
	AddEntry(CRestrictionEnzymeCut(enzymename, recogsite,
											siteformatted, attr, cleavagepos,
							  				seq_form, seq_length));
}

void CRestrictionCutList::AddEntry(const CRestrictionEnzymeCut& restrictioncutarray)
{
   array.emplace_back(restrictioncutarray);
}

string CRestrictionCutList::GetName(int index)
{
	return GetPointer(index)->GetName();
}

int CRestrictionCutList::GetNumberOfCutPositions(int index)
{
	return GetPointer(index)->GetNumberCuts();
}

size_t CRestrictionCutList::GetNameLength(int index)
{
	return GetPointer(index)->GetName().length();
}

string CRestrictionCutList::GetSite(int index)
{
	return GetPointer(index)->GetSite();
}

int* CRestrictionCutList::GetArray(int index)
{
	return GetPointer(index)->GetCutPositionArray();
}

int CRestrictionCutList::GetArrayLength(int index)
{
	return GetPointer(index)->GetNumberCuts();
}

int CRestrictionCutList::GetArrayEntry(int index, int offset)
{
	return GetPointer(index)->GetCutPosition(offset);
}

int CRestrictionCutList::GetTotalCutPos()
{
	int cuts = 0;
	if (array.empty())
		return cuts;

	for(auto const& restrictioncutarray : array)
		cuts += restrictioncutarray.GetNumberCuts();
	return cuts;
}

void CRestrictionCutList::MakeSequentialArray()
{
	int entries = GetTotalCutPos();
	sequentialnames.clear();
	sequentialcutpos.clear();
	sortpointers.clear();
	sequentialdata.clear();
	sortobject.SetSortDirection(CBubbleSort::ascending);
	int counter = 0;
	for(int x = 0; x < size(); x++) {
		for(int y = 0; y < GetArrayLength(x); y ++) {
			sequentialnames.emplace_back(GetName(x));
			sequentialcutpos.emplace_back(GetArrayEntry(x, y));
			//	data == true is unique cutter.  Everything else == flase;
			bool data = (GetNumberOfCutPositions(x) == 1);
			sequentialdata.emplace_back(data);
			sortpointers.emplace_back(counter++);
		}
	}
	if(entries) // No need to sort if totalcutpositions = 0, i.e., none of the enzymes cuts
		sortobject.SortIndirect(&sequentialcutpos[0], entries, &sortpointers[0]);
}

string CRestrictionCutList::GetSequentialName(int index)
{
	if(index < sequentialnames.size())
		return sequentialnames[index];
	return "";
}

int CRestrictionCutList::GetSequentialCutPos(int index)
{
	if(index < sequentialcutpos.size())
		return sequentialcutpos[index];
	return 0;
}

bool CRestrictionCutList::GetSequentialData(int index)
{
	if(index < sequentialdata.size())
		return sequentialdata[index];
	return false;
}

int CRestrictionCutList::GetSequentialPointer(int index)
{
	if(index < sortpointers.size())
		return sortpointers[index];
	return 0;
}

size_t CRestrictionCutList::GetSelectedEnzymeArray(vector<int>& selectedenzymes)
{
	selectedenzymes.clear();
	int index = 0;
	selectedenzymes.reserve(array.size());
	for(auto const& restrictioncutarray : array) {
		if(restrictioncutarray.IsSelected())
			selectedenzymes.push_back(index);
		index++;
	}
	return selectedenzymes.size();
}

int* CRestrictionCutList::SortEnzymesByCutFrequency(vector<int>& cuts)
{
	size_t count = size();
	if(count == 0)
		return &cuts[0];
	cuts.clear();	
	cuts.resize(count);
	for(int x = 0; x < count; x++)
		cuts[x] = x;
	vector<int> cutfrequency;
	cutfrequency.resize(count);
	int i = 0;
	for (auto const& restrictioncutarray : array)
		cutfrequency[i++] = restrictioncutarray.GetNumberCuts();

	sortobject.SetSortDirection(CBubbleSort::ascending);
	sortobject.SortIndirect(&cutfrequency[0], size(), &cuts[0], 0, size() - 1);
	return &cuts[0];
}

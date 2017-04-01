#pragma once

#include <string>
#include <vector>
using namespace std;

class CRestrictionEnzymeCut
{
public:
	CRestrictionEnzymeCut(const string& enzymename, const string& enzymesite, const string& siteformatted, int attr, vector<int>& integerarray,
							  char seq_form, int seq_length, bool selected = false);
	CRestrictionEnzymeCut(const CRestrictionEnzymeCut& passedobject);
	~CRestrictionEnzymeCut();

	string GetNumberCuts(string& cuts);
	string GetSequenceLength(string& length);
	string GetCutPosition(int index, string& position);
	string GetFragmentLength(int index, string& length);

	//  Inlines
	inline string GetName() const
	{
		return name;
	}
	inline string GetSite() const
	{
		return site;
	}
	inline string GetSiteFormatted() const
	{
		return formattedsite;
	}
	inline size_t GetNumberCuts() const
	{
		return numberofcuts;
	}
	inline int GetCutPosition(int index) const
	{
		return index < cutposition.size() ? cutposition[index] : 0;
	}
	inline int CRestrictionEnzymeCut::GetFragmentLength(int index) const
	{
		return index < fragmentlength.size() ? fragmentlength[index] : 0;
	}
	inline int* GetCutPositionArray()
	{
		return &cutposition[0];
	}
	inline int GetSequenceLength() const
	{
		return sequencelength;
	}
	inline char GetSequenceForm() const
	{
		return sequenceform;
	}
	inline bool IsSelected() const
	{
		return select;
	}

	inline void SetSelected(bool value)
	{
		select = value;
	}

protected:
	char sequenceform;
	int sequencelength;
	string name;
	string site;
	string formattedsite;
	size_t numberofcuts;
	int attribute;
	bool select;
	vector<int> cutposition;
	vector<int> fragmentlength;

};

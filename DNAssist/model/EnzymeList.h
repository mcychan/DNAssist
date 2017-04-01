#pragma once
#include "FormatTransfer.h"
#include "SeqConvertor.h"

#include <algorithm>
#include <memory>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

class CEnzymeList
{
protected:
	struct Enzyme
	{
		string Name, Site, Upper, Lower, Attr;
		bool Selected = false;
		Enzyme(const string& name, const string& site, const string& upper, const string& lower, const string& attr) :
			Name(name), Site(site), Upper(upper), Lower(lower), Attr(attr)
		{
		}
	};

public:
	CEnzymeList(const wstring& enzymefile);
	CEnzymeList();
	~CEnzymeList();

	int AddEnzyme(const string& name, const string& site, const string& upper, const string& lower, const string& attr);
	int AssignAttribute(const string& site, const string& upper, const string& lower);
	int AssignAttribute(const string&, int upper, int lower);
	int GetAttribute(int index);
	string GetAttrString(int index, string& data);
	char GetComplement(char chr);//returns the base-paired base of chr
	string GetEnzyme(int index);
	int GetLowerCut(int index);
	string GetLowerCut(int index, string& lowercut);
	string GetName(int index);
	string GetName(int index, string& format);
	string GetName(const string& site);
	inline size_t GetNumberOfEnzymes(){return enzymearray.size();};
	string GetSite(const string& name);
	string GetSite(int index, string& format);
	string GetSiteComplement(int index, string& format);
	string GetSiteFormatted(int index, string& format);
	int GetSiteLength(int index);
	int GetIndex(const string&);
	int GetUpperCut(int index);
	string GetUpperCut(int index, string& uppercut);
	bool IsSelected(int index);
	bool IsSymmetric(const string& site);//returns TRUE is site is symmetrical
	void LoadStriderFile();
	void RetainSelected();
	void RemoveEnzymeBySortList(int index);
	void SetSelected(int index, bool value);
	void SelectAll();
	wstring GetFullPath();
	void SortList(int code = EL_SORTNAMEASCENDING);

protected:
	wstring filename;
	vector<Enzyme> enzymearray;
	CSeqConvertor m_convertsequence;
};

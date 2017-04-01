#include "EnzymeList.h"
#include "SeqValidator.h"

#include <cctype>
#include <iostream>     // std::ios, std::istream, std::cout
#include <fstream>      // std::filebuf

CEnzymeList::CEnzymeList()
{
}

CEnzymeList::CEnzymeList(const wstring& enzymefile)
{
	filename = enzymefile;
	LoadStriderFile();
}

CEnzymeList::~CEnzymeList()
{
}

//***************************************

int CEnzymeList::AddEnzyme(const string& name, const string& site, const string& upper, const string& lower, const string& attr)
{
	enzymearray.emplace_back(Enzyme{ name, site, upper, lower, attr });
	return enzymearray.size();
}

//**********************************************


//RemoveEnzyme deletes the all the enzyme entries associated
//with the enzyme XXX at the index 'index' on the SORTED
//ENZYME LIST, not the index of the enzyme in the string array(!!!)

void CEnzymeList::RemoveEnzymeBySortList(int index)
{
	enzymearray.erase(enzymearray.begin() + index);
}

void CEnzymeList::SortList(int code)
{
	if(enzymearray.empty())
		return;
	if(code == EL_SORTNAMEASCENDING)
		sort(enzymearray.begin(), enzymearray.end(),
		          [](const Enzyme& lhs, const Enzyme & rhs)
		{
		  return lhs.Name.compare(rhs.Name) < 0;
		}
		);
	else if(code == EL_SORTNAMEDESCENDING)
		sort(enzymearray.begin(), enzymearray.end(),
		          [](const Enzyme& lhs, const Enzyme & rhs)
		{
		  return lhs.Name.compare(rhs.Name) > 0;
		}
		);
	else if(code == EL_SORTSITEASCENDING)
		sort(enzymearray.begin(), enzymearray.end(),
		          [](const Enzyme& lhs, const Enzyme & rhs)
		{
		  return lhs.Site.compare(rhs.Site) < 0;
		}
		);
	else if(code == EL_SORTSITEDESCENDING)
		sort(enzymearray.begin(), enzymearray.end(),
		          [](const Enzyme& lhs, const Enzyme & rhs)
		{
		  return lhs.Site.compare(rhs.Site) > 0;
		}
		);
}

//**************************************

string CEnzymeList::GetSiteFormatted(int index, string& format)
{
	if(index >= enzymearray.size())
		return "";

	string unformat;
	GetSite(index, unformat);
	int pads;
	int attribute = GetAttribute(index);
	if((attribute & EL_CUTOUTSIDE) == EL_CUTOUTSIDE)
		pads = enzymearray[index].Upper.length() + enzymearray[index].Lower.length() + 2;//add uppercut + lowercut + two periods
	else if(((attribute & EL_NOSITE) == EL_NOSITE) || ((attribute & EL_NOCUT) == EL_NOCUT))
		pads = 0;
	else
		pads = 1;

	if(pads == 1) {
		format = unformat;
		format.insert(GetUpperCut(index), "/");
	}
	else if(pads == 0)
		format = unformat;
	else {
		format = unformat + ",";
		format += enzymearray[index].Upper + ",";
		format += enzymearray[index].Lower;
	}
	return format;
}


string CEnzymeList::GetSite(int index, string& format)
{
	if(index < enzymearray.size())
		format = enzymearray[index].Site;
	return format;
}

string CEnzymeList::GetSiteComplement(int index, string& format)
{
	string sitecomplement, sitereverse;
	string site;
	GetSite(index, site);
	m_convertsequence.SequenceComplement(site, sitecomplement, 'D');
	m_convertsequence.SequenceReverse(sitecomplement, sitereverse);

	format = sitereverse;
	return format;
}

int CEnzymeList::GetSiteLength(int index)
{
	return index < enzymearray.size() ? enzymearray[index].Site.length() : 0;
}

string CEnzymeList::GetName(int index, string& name)
{
	if(index < enzymearray.size())
		name = enzymearray[index].Name;
	return name;
}

bool CEnzymeList::IsSymmetric(const string& site)
{
	string sitecomplement, sitereverse;
	m_convertsequence.SequenceComplement(site, sitecomplement, 'D');
	m_convertsequence.SequenceReverse(sitecomplement, sitereverse);
	return site == sitereverse;
}

char CEnzymeList::GetComplement(char chr)
{
	char complement = 'X';
	switch(toupper(chr)) {
		case 'A': complement = 'T'; break;
		case 'T': complement = 'A'; break;
		case 'C': complement = 'G'; break;
		case 'G': complement = 'C'; break;
		case 'R': complement = 'Y'; break;
		case 'Y': complement = 'R'; break;
		case 'M': complement = 'K'; break;
		case 'K': complement = 'M'; break;
		case 'S': complement = 'W'; break;
		case 'W': complement = 'S'; break;
		case 'B': complement = 'V'; break;
		case 'V': complement = 'B'; break;
		case 'D': complement = 'H'; break;
		case 'H': complement = 'D'; break;
		case 'N': complement = 'N';
	}

	return complement == 'X' ? '0' : complement;
}

string CEnzymeList::GetAttrString(int index, string& data)
{
	if(index < enzymearray.size())
		data = enzymearray[index].Attr;
	return data;
}

int CEnzymeList::GetUpperCut(int index)
{
	return index < enzymearray.size() ? stoi(enzymearray[index].Upper) : 0;
}

int CEnzymeList::GetLowerCut(int index)
{
	return index < enzymearray.size() ? stoi(enzymearray[index].Lower) : 0;
}

int CEnzymeList::GetAttribute(int index)
{
	return index < enzymearray.size() ? stoi(enzymearray[index].Attr) : 0;
}

string CEnzymeList::GetEnzyme(int index)
{
	return enzymearray[index].Name;
}

int CEnzymeList::GetIndex(const string& name)
{
	auto pred = [name](const Enzyme& e) {
	    return e.Name == name;
	};

	auto it = find_if(enzymearray.begin(), enzymearray.end(), pred);
	return it != enzymearray.end() ? it - enzymearray.begin() : -1;
}

string CEnzymeList::GetSite(const string& name)
{
	int i = GetIndex(name);
	return i > -1 ? enzymearray[i].Site : "";
}

string CEnzymeList::GetName(const string& site)
{
	auto pred = [site](const Enzyme& e) {
		return e.Site == site;
	};

	auto it = find_if(enzymearray.begin(), enzymearray.end(), pred);
	return it != enzymearray.end() ? it->Name : "";
}

int CEnzymeList::AssignAttribute(const string& site, int upperint, int lowerint)
{
	int attribute = 0;
	if(site == "")
		return EL_NOSITE;
	if(IsSymmetric(site))
		attribute |= EL_PALINDROME;
	size_t sitelength = site.length();
	if(sitelength == 8)
		attribute |= EL_8CUTTER;
	else if(sitelength == 6)
		attribute |= EL_6CUTTER;
	else if(sitelength == 4)
		attribute |= EL_4CUTTER;
	if(lowerint < 0 || lowerint > sitelength || upperint < 0 || upperint > sitelength)
		attribute |= EL_CUTOUTSIDE;
	if(upperint < lowerint)
		attribute |= EL_5PRIME;
	else if (upperint > lowerint)
		attribute |= EL_3PRIME;
	else if(upperint == lowerint)
		attribute |= EL_BLUNT;
	return attribute;
}

int CEnzymeList::AssignAttribute(const string& site, const string& upper, const string& lower)
{
	return AssignAttribute(site, stoi(upper), stoi(lower));
}

void CEnzymeList::RetainSelected()
{
	enzymearray.erase(
	    std::remove_if(enzymearray.begin(), enzymearray.end(),
	    		[](const Enzyme& enzyme) { return !enzyme.Selected; }),
	enzymearray.end());
}

//
void CEnzymeList::LoadStriderFile()
{
	string raw, name, tempsite, site, upper, lower, attr;
	CSeqValidator valid;
	filebuf fb;
	if (fb.open(filename, ios::in)) {
		istream is(&fb);
		while(!is.eof()) {
			//read data from file
			getline(is, raw);
			if(raw.length() > 0 && raw[raw.length()-1] == ',') {
				if(raw.find(';') != string::npos) {
					//	Enzymes preceded by ";" have no known cut site, so ignore
					continue;
				}

				raw.erase(remove(raw.begin(), raw.end(), '\t'), raw.end());				
				size_t found = raw.find('#');
				if(found != string::npos)
					raw.erase(0, found + 1);

				size_t firstcomma = raw.find(',');

				name = raw.substr(0, firstcomma);
				transform(raw.begin(), raw.end(), raw.begin(), ::toupper);

				if(firstcomma != string::npos) {
					size_t secondcomma = raw.find(',', firstcomma + 1);
					if(secondcomma != string::npos) {
						tempsite = raw.substr(firstcomma + 1, secondcomma - firstcomma - 1);
						size_t slash = tempsite.find('/');
						bool noSlash = (slash == string::npos);

						if(noSlash) {
							site = tempsite;
							size_t thirdcomma = raw.find(',', secondcomma + 1);
							if(thirdcomma != string::npos) {
								upper = raw.substr(secondcomma + 1, thirdcomma - secondcomma - 1);
								size_t fourthcomma = raw.find(',', thirdcomma + 1);
								if(fourthcomma != string::npos) {
									lower = raw.substr(thirdcomma + 1, fourthcomma - thirdcomma - 1);
									attr = to_string(AssignAttribute(site, upper, lower));
								}
							}
						}
						else {
							site = tempsite.substr(0, slash);
							site += tempsite.substr(slash + 1);
							upper = to_string(slash);
							lower = to_string(tempsite.length() - stoi(upper) - 1);
							attr = to_string(AssignAttribute(site, upper, lower));
						}
					}
				}

				if(name.length() > 0 && site.length() > 0 && valid.IsValid('D', site, 0))
					AddEnzyme(name, site, upper, lower, attr);
			}

		}
	    fb.close();
	}
}

//
//	The following functions returns the entry corresponding to the supplied
// index in the sorted list.
//

string CEnzymeList::GetName(int index)
{
	return index < enzymearray.size() ? enzymearray[index].Name : "";
}

bool CEnzymeList::IsSelected(int index)
{
	return index < enzymearray.size() ? enzymearray[index].Selected : false;
}

void CEnzymeList::SetSelected(int index, bool value)
{
	if(index < enzymearray.size())
		enzymearray[index].Selected = value;
}

string CEnzymeList::GetUpperCut(int index, string& uppercut)
{
	if(index < enzymearray.size())
		uppercut = enzymearray[index].Upper;
	return uppercut;
}

string CEnzymeList::GetLowerCut(int index, string& lowercut)
{
	if(index < enzymearray.size())
		lowercut = enzymearray[index].Lower;
	return lowercut;
}

void CEnzymeList::SelectAll()
{
	for(int x = 0; x < enzymearray.size(); x++)
		SetSelected(x, true);
}

wstring CEnzymeList::GetFullPath()
{
	return filename;
}

#include "RestrictionEnzymeCut.h"

CRestrictionEnzymeCut::CRestrictionEnzymeCut(const string& enzymename, const string& enzymesite, const string& siteformatted,
															  int attr, vector<int>& integerarray,
                                               char seq_form, int seq_length, bool selected)
{
	name = enzymename;
	site = enzymesite;
	formattedsite = siteformatted;
	cutposition = integerarray;
	fragmentlength.clear();
	attribute = attr;
	sequenceform = seq_form;
	sequencelength = seq_length;
	select = selected;

	// Remove duplicate cutposition entries which can slip through
	int x = 0;
	while(cutposition.size() >= 2 && x < cutposition.size()-1) {
		if(cutposition[x] == cutposition[x+1])
			cutposition.erase(cutposition.begin() + x);
		else
			x++;
	}

	numberofcuts = cutposition.size(); // set numbercuts here, after duplicates have been removed

	// Calculate the fragment lengths
	if (sequenceform == 'C' && numberofcuts > 0) {
			fragmentlength.clear();
			fragmentlength.resize(numberofcuts + 1);
			int temp = sequencelength - cutposition[numberofcuts - 1] - 1;
			fragmentlength[0] = cutposition[0] + temp;
			for(int x = 1; x < numberofcuts; x++)
				fragmentlength[x] = cutposition[x] - cutposition[x - 1];
	}

	if (sequenceform == 'L') {
		fragmentlength.clear();
		if(numberofcuts > 0) {
			fragmentlength.resize(numberofcuts + 1);
			fragmentlength[0] = cutposition[0];
			for(int y = 1; y < numberofcuts; y++)
				fragmentlength[y] = cutposition[y] - cutposition[y - 1];
			fragmentlength.push_back(sequencelength - cutposition[numberofcuts-1]);
		}
		else
			fragmentlength.push_back(sequencelength);
	}
}

CRestrictionEnzymeCut::CRestrictionEnzymeCut(const CRestrictionEnzymeCut& passedobject)
{
	name = passedobject.name;
	site = passedobject.site;
	formattedsite = passedobject.formattedsite;
	cutposition = passedobject.cutposition;
	fragmentlength.clear();
	numberofcuts = cutposition.size();
	attribute = passedobject.attribute;
	sequenceform = passedobject.sequenceform;
	sequencelength = passedobject.sequencelength;
	select = passedobject.select;

	if (sequenceform == 'C' && numberofcuts > 0) {
		int temp = sequencelength - cutposition[numberofcuts - 1] - 1;
		fragmentlength.resize(numberofcuts + 1);
		fragmentlength[0] = cutposition[0] + temp;
		for(int x = 1; x < numberofcuts; x++)
			fragmentlength[x] = cutposition[x] - cutposition[x - 1];
	}

	if (sequenceform == 'L') {
		if(numberofcuts > 0) {
			fragmentlength.resize(numberofcuts + 1);
			fragmentlength[0] = cutposition[0];
			for(int y = 1; y < numberofcuts; y++)
				fragmentlength[y] = cutposition[y] - cutposition[y - 1];
			fragmentlength.push_back(sequencelength - cutposition[numberofcuts - 1]);
		}
		else
			fragmentlength.push_back(sequencelength);
	}
}

CRestrictionEnzymeCut::~CRestrictionEnzymeCut()
{
}

string CRestrictionEnzymeCut::GetNumberCuts(string& cuts)
{
	if(!numberofcuts)
		cuts = "0";
	else {
		int length = (int)(log10(numberofcuts + 0.0f) + 1);
		cuts = to_string(numberofcuts);
	}
	return cuts;
}

string CRestrictionEnzymeCut::GetSequenceLength(string& length)
{
	if(sequencelength) {
		int len = (int)(log10(sequencelength + 0.0f) + 1);
		length = to_string(sequencelength);
	}
	else
		length = "0";
	return length;
}

string CRestrictionEnzymeCut::GetCutPosition(int index, string& position)
{
	if(numberofcuts && index < cutposition.size()) {
		int value = cutposition[index];
		int len = (int)(log10(value + 0.0f) + 1);
		position = to_string(value);
	}
	else
		position = "0";
	return position;
}

string CRestrictionEnzymeCut::GetFragmentLength(int index, string& length)
{
	if(index < fragmentlength.size()) {
		int value = fragmentlength[index];
		int len = (int)(log10(value + 0.0f) + 1);
		length = to_string(value);
	}
	else
		length = "0";
	return length;
}

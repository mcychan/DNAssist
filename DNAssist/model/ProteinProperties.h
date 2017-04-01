#pragma once
#include "SeqConvertor.h"

#include <memory>
#include <sstream>
#include <string>
using namespace std;

class CProteinProperties
{
public:
	CProteinProperties(const string& sequence);
	~CProteinProperties();
	size_t* GetNumberOfAminoAcidResidues();
	size_t* GetMolePercentageAminoAcidResidues();
	string GetMolarAbsoptivityString(string& data);
	string GetOnePercentSolutionAbsorptivityString(string& data);
	string GetMolecularWeightString(string& data);
	string GetpIString(string& data);
	string GetChargeAtpHString(string& data);
	string GetNumberOfResiduesString(int index, string& data);
	string GetMolePercentOfResiduesString(int index, string& data);
	int GetAminoAcidIndex(char aa);
	string GetListOfCommonAminoAcids();
	int GetNumberOfCommonAminoAcids();

protected:
	int GetMolarAbsorptivity();
	int GetOnePercentSolutionAbsorptivity();
	float GetMolecularWeight();
	float GetChargeAtpH();
	float GetpI();
	int GetNumberOfSpecificResidues(char residue);

	size_t numberofcommonaminoacids;
	string aminoacidlist, listofaminoacids, proteinsequence;
	unique_ptr<size_t[]> molepercentageaminoacidresidues, numberofaminoacidresidues;
	unique_ptr<float[]> molecularweight;

	CSeqConvertor convertobject;
};

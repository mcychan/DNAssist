#pragma once
#include <string>

using namespace std;

class CSeqValidator
{
public:
	CSeqValidator();
	bool IsValid(char type, const string& data, int strict = 1);//If strict == 1,
											   //the strict limited memberlist is used
	bool IsValid(char type, char base, int strict = 1);
	bool IsValidForPaste(char type, const string& data, int strict = 1);
	bool TestValid(const string& data, const string& members);
	bool IsAminoAcid(char aa);
	bool IsRNAbase(char rna);
	bool IsDNAbase(char dna);
	bool IsDegenerateDNAbase(char dna);
	bool IsFloat(const string& data);
	bool IsInt(const string& data);
	bool IsProtein(const string& data);
	size_t StripForPaste(char type, string& data, int strict = 1);
	size_t CopyValidCharacters(string& data, const string& validchars);

protected:
	string DNAstrict;
	string DNAfuzzy;
	string RNAstrict;
	string RNAfuzzy;
	string Proteinstrict;
	string Proteinfuzzy;
	string DNAstrictPaste;
	string DNAfuzzyPaste;
	string RNAstrictPaste;
	string RNAfuzzyPaste;
	string ProteinstrictPaste;
	string ProteinfuzzyPaste;

};

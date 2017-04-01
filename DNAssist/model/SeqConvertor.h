#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>
using namespace std;

class CCodonTable;

class CSeqConvertor
{
public:
	size_t ConvertSequence(const string& inputseq, string& outputseq, char intype, char outtype, CCodonTable* pCodontable = NULL, bool truncate = true);
	string SequenceComplement(const string& inputseq, string& outputseq, char seqtype);
	string SequenceReverse(const string& inputseq, string& outputseq);
	string SequenceReverseComplement(const string& inputseq, string& outputseq, char seqtype);
	string ConvertAminoAcidOneToThree(const char letter, string& three);
	char ConvertAminoAcidThreeToOne(const string& three);
	bool IsAminoAcid(const char letter);
	bool IsAminoAcid(const string& letter);
	bool IsCodon(string& codon);
	bool IsPalindromic(const string& seq);
	string GetListOfAminoAcids(string& array);
	string ConvertDNAToRNA(string& sequence);
	string ConvertRNAToDNA(string& sequence);
	string ReverseTranslate(const string& protein, string& degenerateDNA, CCodonTable* pCodontable);
	string RandomizeSequence(const string& inputseq, string& outputseq);

protected:
	char GetBaseComplement(char input, char seqtype);
	char GetBaseConverted(char input, char intype, char outtype);
	size_t ConvertNucleicAcidToProtein(const string& inputseq, string& outputseq, CCodonTable* pCodontable, bool truncateatstop = true);
	char GetDegenerateDNABase(const string& mixofbases);
};

#include "SeqValidator.h"
#include <iostream>

CSeqValidator::CSeqValidator()
{
	DNAstrict = "GATCgatc\0";
	DNAfuzzy = "GATCRYMKSWBDHVNgatcrymkswbdhvn\0";
	RNAstrict = "GAUCgauc\0";
	RNAfuzzy = "GAUCRYMKSWBDHVNgaucrymkswbdhvn\0";
	Proteinstrict = "ACDEFGHIKLMNPQRSTVWYacdefghiklmnpqrstvwy\0";
	Proteinfuzzy = "ABCDEFGHIKLMNPQRSTVWXYZabcdefghiklmnpqrstvwxyz\0";
	DNAstrictPaste = "GATCgatc\r\n 0123456789\0";
	DNAfuzzyPaste = "GATCRYMKSWBDHVNgatcrymkswbdhvn\r\n 0123456789\0";
	RNAstrictPaste = "GAUCgauc\r\n 0123456789\0";
	RNAfuzzyPaste = "GAUCRYMKSWBDHVNgaucrymkswbdhvn\r\n 0123456789\0";
	ProteinstrictPaste = "ACDEFGHIKLMNPQRSTVWYacdefghiklmnpqrstvwy\r\n 0123456789\0";
	ProteinfuzzyPaste = "ABCDEFGHIKLMNPQRSTVWXYZabcdefghiklmnpqrstvwxyz\r\n 0123456789\0";
}

bool CSeqValidator::TestValid(const string& data, const string& members)
{
	return data.find_first_not_of(members) == string::npos;
}

bool CSeqValidator::IsValid(char type, const string& data, int strict)
{
	if(strict) {
		if(type == 'D')
			return TestValid(data, DNAstrict);
		if(type == 'G')
			return TestValid(data, DNAfuzzy);
		if(type == 'R')
			return TestValid(data, RNAstrict);
		if(type == 'P')
			return TestValid(data, Proteinstrict);
   }

	if(type == 'D')
		return TestValid(data, DNAfuzzy);
	if(type == 'G')
		return TestValid(data, DNAfuzzy);
	if(type == 'R')
		return TestValid(data, RNAfuzzy);
	if(type == 'P')
		return TestValid(data, Proteinfuzzy);

	return false;
}

bool CSeqValidator::IsValidForPaste(char type, const string& data, int strict)
{
	if(strict) {
		if(type == 'D')
			return TestValid(data, DNAstrictPaste);
		if(type == 'G')
			return TestValid(data, DNAfuzzyPaste);
		if(type == 'R')
			return TestValid(data, RNAstrictPaste);
		if(type == 'P')
			return TestValid(data, ProteinstrictPaste);
	}

	if(type == 'D')
		return TestValid(data, DNAfuzzyPaste);
	if(type == 'G')
		return TestValid(data, DNAfuzzyPaste);
	if(type == 'R')
		return TestValid(data, RNAfuzzyPaste);
	if(type == 'P')
		return TestValid(data, ProteinfuzzyPaste);

	return false;
}

size_t CSeqValidator::CopyValidCharacters(string& data, const string& validchars)
{
	size_t length = data.length();
	if(length <= 0)
		return 0;

	int copyposition = 0;
	for(int readposition = 0; readposition < length; readposition++) {
		if(validchars.find(data[readposition]) != string::npos)
			data[copyposition++] = data[readposition];
	}
	data.resize(copyposition);
	return data.length();
}

size_t CSeqValidator::StripForPaste(char type, string& data, int strict)
{
	if(strict) {
		if(type == 'D')
			return CopyValidCharacters(data, DNAstrict);
		if(type == 'G')
			return CopyValidCharacters(data, DNAfuzzy);
		if(type == 'R')
			return CopyValidCharacters(data, RNAstrict);
		if(type == 'P')
			return CopyValidCharacters(data, Proteinstrict);
	}

	if(type == 'D')
		return CopyValidCharacters(data, DNAfuzzy);
	if(type == 'G')
		return CopyValidCharacters(data, DNAfuzzy);
	if(type == 'R')
		return CopyValidCharacters(data, RNAfuzzy);
	if(type == 'P')
		return CopyValidCharacters(data, Proteinfuzzy);

	return 0;
}

bool CSeqValidator::IsAminoAcid(char aa)
{
	//	NOTE: this function expects an uppercase aa
	return Proteinstrict.find(aa) != string::npos;
}

bool CSeqValidator::IsRNAbase(char rna)
{
	//	NOTE: this function expects an uppercase rna
	return RNAstrict.find(rna) != string::npos;
}

bool CSeqValidator::IsDNAbase(char dna)
{
	//	NOTE: this function expects an uppercase dna
	return DNAstrict.find(dna) != string::npos;
}

bool CSeqValidator::IsDegenerateDNAbase(char dna)
{
	//	NOTE: this function expects an uppercase dna
	return DNAfuzzy.find(dna) != string::npos;
}

bool CSeqValidator::IsValid(char type, char base, int strict)
{
	switch(type)
	{
		case 'D' : return IsDNAbase(base);
		case 'G' : return IsDegenerateDNAbase(base);
		case 'R' : return IsRNAbase(base);
		case 'P' : return IsAminoAcid(base);
	}
	return false;
}

bool CSeqValidator::IsFloat(const string& data)
{
	size_t length = data.length();
	if(length <= 0)
		return false;

	auto it = data.cbegin();

	while(it != data.cend()) {
		switch(*it) {
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
			case '-' :
			case 'E' :
			case 'e' :
			case '.' : break;
			default : return false;
		}
		++it;
	}
	return true;
}

bool CSeqValidator::IsInt(const string& data)
{
	size_t length = data.length();
	if(length <= 0)
		return false;

	auto it = data.cbegin();

	while(it != data.cend()) {
		switch(*it) {
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' : break;
			default : return false;
      	}
      	++it;
	}
	return true;
}

bool CSeqValidator::IsProtein(const string& data)
{
	size_t length = data.length();
	if(length <= 0)
		return false;

	auto it = data.cbegin();

	while(it != data.cend()) {
		if(!IsAminoAcid(*it))
			break;
		++it;
	}

	return it == data.cend();
}

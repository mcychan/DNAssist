#include "FileTypeDetector.h"

char CFileTypeDetector::GetMostLikelySequenceType(const char* sequence)
{
	string seq(sequence);
	size_t length = seq.length();
	if(length <= 0)
		return '\0';

	int dna = 0, rna = 0, protein = 0, degenerate = 0;
	string::const_iterator it = seq.begin();

	while(it != seq.end()) {
		if(m_seqValidator.IsValid('D', *it, 1))
			dna++;
		if(m_seqValidator.IsValid('R', *it, 1))
			rna++;
		if(m_seqValidator.IsValid('P', *it, 1))
			protein++;
		if(m_seqValidator.IsValid('G', *it, 0))
			degenerate++;
		++it;
   	}

	if(dna > 0 && dna >= rna && dna >= protein && dna >= degenerate)
		return 'D';
	if(rna > 0 && rna >= dna && rna >= protein && rna >= degenerate)
		return 'R';
	if(protein > 0 && protein >= rna && protein >= dna && protein >= degenerate)
		return 'P';
	if(degenerate > 0 && degenerate >= rna && degenerate >= protein && degenerate >= dna)
		return 'G';
	return '\0';
}

size_t CFileTypeDetector::RemoveBadCharacters(string& sequence, char sequencetype, int strict)
{
	string seq(sequence);
	size_t length = seq.length();
	if(length <= 0)
		return 0;

	string tempsequence(seq);
	string::const_iterator it = seq.begin();

	int copyposition = 0;
	while(it != seq.end()) {
		if(m_seqValidator.IsValid(sequencetype, *it, strict))
			tempsequence[copyposition++] = *it;
		++it;
   	}
	tempsequence.resize(copyposition);

	seq = tempsequence;
	return seq.length();
}

char CFileTypeDetector::GetMostLikelySequenceType(string& sequence_to_pass_back, const string& temp_sequence)
{
	char type = 0;
	size_t length = temp_sequence.length();
	if(length <= 0)
		return type;

	string temp_dna = "", temp_rna = "", temp_protein = "", temp_degenerate_dna = "";
	string::const_iterator it = temp_sequence.begin();

	while(it != temp_sequence.end()) {
		if(m_seqValidator.IsDNAbase(*it))
			temp_dna += *it;
		if(m_seqValidator.IsRNAbase(*it))
			temp_rna += *it;
		if(m_seqValidator.IsAminoAcid(*it))
			temp_protein += *it;
		if(m_seqValidator.IsDegenerateDNAbase(*it))
			temp_degenerate_dna += *it;

		++it;
	}

	if(temp_dna.length() > 0)
		type += 2;
	if(temp_rna.length() > 0)
		type += 4;
	if(temp_protein.length() > 0)
		type += 8;
	if(temp_degenerate_dna.length() > 0)
		type += 1;


	if(type == 'D')
		sequence_to_pass_back = temp_dna;
	else if(type == 'P')
		sequence_to_pass_back = temp_protein;
	else if(type == 'G')
		sequence_to_pass_back = temp_degenerate_dna;
	else if(type == 'R')
		sequence_to_pass_back = temp_rna;

	return type < 16 ? 0 : type;
}

bool CFileTypeDetector::IsTextFile()
{
	return true;
}

bool CFileTypeDetector::IsStriderFile(const char* sequence)
{
	string seq(sequence);
	if (seq.length() < 32)
		return false;
	char start[] = {(char) 255, '\0'};
	string begin = start;
	return seq.find(start) != string::npos;
}

bool CFileTypeDetector::IsFastaFile(const char* sequence)
{
	string seq(sequence);
	string begin = ">";
	return seq.find(begin) != string::npos;
}

bool CFileTypeDetector::IsDNAtoolsFile(const char* sequence, const string& applicationname)
{
	string seq(sequence);
	if(seq.find(applicationname) == string::npos)
		return seq.find("DNAtools") != string::npos;
	return true;
}

bool CFileTypeDetector::IsGCGFile(const char* sequence)
{
	string seq(sequence);
	string begin = "..";
	return seq.find(begin) != string::npos;
}

bool CFileTypeDetector::IsGenBankFile(const char* sequence)
{
	string seq(sequence);
	string begin = "ORIGIN";
	return seq.find(begin) != string::npos;
}

char CFileTypeDetector::GetDNABase(int base)
{
	switch(base)
	{
		case 0 : return 'G';
		case 1 : return 'A';
		case 2 : return 'T';
		case 3 : return 'C';
		default : return 'X';
	}
}

char CFileTypeDetector::GetRNABase(int base)
{
	switch(base)
	{
		case 0 : return 'G';
		case 1 : return 'A';
		case 2 : return 'U';
		case 3 : return 'C';
		default : return 'X';
	}
}

int CFileTypeDetector::GetNumberOfBasesInByte(char base)
{
	int bit6 = 64;
	int bit5 = 32;
	if(!(base & bit6))
		return 3;
	if(!(base & bit5))
		return 2;
	return 1;
}

const wstring CFileTypeDetector::DetermineMostLikelyFileType(const char* sequence, const string& applicationname)
{
	if(IsDNAtoolsFile(sequence, applicationname))
		return L"seq";
	if(IsGenBankFile(sequence))
		return L"gbk";
	if(IsGCGFile(sequence))
		return L"gcg";
	if(IsFastaFile(sequence))
		return L"fas";
	if(IsStriderFile(sequence))
		return L"str";
	return L"txt";
}

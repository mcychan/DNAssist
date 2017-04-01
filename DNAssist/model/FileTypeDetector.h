#pragma once
#include "SeqValidator.h"

#include <string>
using namespace std;

class CFileTypeDetector
{
public:
	char GetMostLikelySequenceType(const char* sequence);
	size_t RemoveBadCharacters(string& sequence, char sequencetype = 'D', int strict = 1);
	char GetMostLikelySequenceType(string& sequence_to_pass_back, const string& temp_sequence);
	bool IsTextFile();
	bool IsStriderFile(const char* sequence);
	bool IsFastaFile(const char* sequence);
	bool IsDNAtoolsFile(const char* sequence, const string& applicationname);
	bool IsGCGFile(const char* sequence);
	bool IsGenBankFile(const char* sequence);
	int GetNumberOfBasesInByte(char base);
	char GetDNABase(int base);
	char GetRNABase(int base);
	const wstring DetermineMostLikelyFileType(const char* sequence, const string& applicationname);

protected:
	CSeqValidator m_seqValidator;
};

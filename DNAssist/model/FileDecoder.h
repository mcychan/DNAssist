#pragma once
#include "FileTypeDetector.h"
#include "SeqValidator.h"

#include <algorithm>
#include <string>
using namespace std;

class CFileDecoder
{
public:
	CFileDecoder(const wstring& fullfilepath);
	char DecodeTextFile(string& sequence_to_pass_back);
	char DecodeStriderFile(string& sequence_to_pass_back);
	char DecodeFastaFile(string& sequence_to_pass_back);
	char DecodeDNAtoolsFile(string& sequence_to_pass_back, char& form, string& comment, const string& applicationname);
	char DecodeGCGFile(string& sequence_to_pass_back);
	char DecodeGenBankFile(string& sequence_to_pass_back);

	bool ExtractCharactersFromByte(char byte, string& characters, char type);
	bool DecodeDNAtoolsFile(string& sequence_to_pass_back, string temp_seq, char type);
	char GetSequence(string& sequence_to_pass_back, char& form, string& comment, const string& applicationname);
	inline wstring GetExtension() const {
		return m_extension;
	}

protected:
	string m_content;
	wstring m_extension;
	CSeqValidator m_seqValidator;
	CFileTypeDetector m_fileTypeDetector;
};


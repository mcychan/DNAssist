#pragma once

#include <algorithm>
#include <string>
using namespace std;

class CFileEncoder
{
public:
	CFileEncoder(const wstring& fullfilepath);
	bool WriteFile(const string& sequence, char type, char form, const string& comments, const string& applicationName, const string& version);
	inline wstring GetExtension() const {
		return m_extension;
	}

protected:
	wstring m_pathname;
	wstring m_extension;
	bool EncodeTextFile(const string& tempsequence);
	bool EncodeFastaFile(const string& tempsequence);
	bool EncodeGenBankFile(const string& tempsequence);
	bool EncodeGCGFile(const string& tempsequence);
	bool EncodeDNAtoolsFile(const string& tempsequence, char type, char form, const string& comments, const string& applicationName, const string& version);
	unsigned char CompressCharactersIntoByte(const string& threebases);
	int EncodeDNAOrRNABase(char base);

};

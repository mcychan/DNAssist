#include "FileDecoder.h"
#include <fstream>      // std::ifstream
#include <iterator>     // std::istream_iterator

CFileDecoder::CFileDecoder(const wstring& fullfilepath) {
	size_t found = fullfilepath.find_last_of(L'.');
	if(found != string::npos) {
		m_extension = fullfilepath.substr(fullfilepath.find_last_of('.') + 1);
		transform(m_extension.begin(), m_extension.end(), m_extension.begin(), ::tolower);
	}
	else
		m_extension = L"";

	ifstream ifs(fullfilepath);
	if (ifs) {
		m_isValid = ifs.good();
		m_content = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
		ifs.close();
	}
	else
		m_isValid = false;
}

char CFileDecoder::DecodeTextFile(string& sequence_to_pass_back)
{
	sequence_to_pass_back = m_content;
	transform(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), sequence_to_pass_back.begin(), ::toupper);
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());
	return m_fileTypeDetector.GetMostLikelySequenceType(sequence_to_pass_back.c_str());
}


char CFileDecoder::DecodeStriderFile(string& sequence_to_pass_back)
{
	if(m_content.length() < 32)
		return 0;

	//	In Strider, the sequence length is given as three bytes at offset 29, 30 and 31 from the file start
	const char topbyte = m_content[29];
	const char middlebyte = m_content[30];
	const char lowbyte = m_content[31];
	int sequencelength = (int)topbyte*256*256 + (int)middlebyte*256 + (int)lowbyte;

/* All Strider files have a binary header followed by the sequence in text.
The value 0xFF appears towards the end of the header, followed by lots of 0x00
before the sequence starts.  Find this 0xFF and copy the entire file from there.
*/
	string temp_sequence = m_content;
	size_t found = temp_sequence.find_first_of((char) 255);
	if(found == string::npos)
		return 0;

	temp_sequence.erase(0, found + 1);
	char type = m_fileTypeDetector.GetMostLikelySequenceType(temp_sequence.c_str());
	m_fileTypeDetector.RemoveBadCharacters(temp_sequence, type, 1);
	if(temp_sequence.length() > sequencelength)
		sequence_to_pass_back = temp_sequence.substr(0, sequencelength);
	else
		sequence_to_pass_back = temp_sequence;
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());
	return type;
}

char CFileDecoder::DecodeFastaFile(string& sequence_to_pass_back)
{
	string temp_sequence = m_content;
	size_t found = temp_sequence.find_first_of('>');
	if(found == string::npos)
		return 0;

	temp_sequence.erase(0, found + 1);
	size_t ln = temp_sequence.find_first_of('\n');
	if(ln == string::npos)
		return 0;

	sequence_to_pass_back = temp_sequence.substr(0, ln);
	transform(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), sequence_to_pass_back.begin(), ::toupper);
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());	
	return m_fileTypeDetector.GetMostLikelySequenceType(sequence_to_pass_back.c_str());
}

char CFileDecoder::DecodeDNAtoolsFile(string& sequence_to_pass_back, char& form, string& comment, const string& applicationname)
{
/* The DNAssist file header looks like:
	DNAssist;Versionxxx;D;L;commentxxx;sequence
   if the sequence is DNA or RNA every two bits
   in a byte is a character
*/
	string temp_sequence = m_content;
	size_t foundApp = temp_sequence.find(applicationname);
	size_t foundTools = temp_sequence.find("DNAtools");
	if(foundApp == string::npos && foundTools == string::npos)
		return 0;

	if (foundApp != string::npos)
		temp_sequence.erase(0, foundApp + applicationname.length() + 1);
	else
		temp_sequence.erase(0, foundTools + strlen("DNAtools") + 1);
	// Get end of DNAssist
	
	char separator = ';';
	size_t found = temp_sequence.find_first_of(separator);
	string version = temp_sequence.substr(0, found);
	transform(version.begin(), version.end(), version.begin(), ::tolower);
	//	Get end of version info
	temp_sequence.erase(0, found + 1);

	//	Get type info
	char type = temp_sequence[0];	
	found = temp_sequence.find_first_of(separator);
	//	Get end of type info
	temp_sequence.erase(0, found + 1);

	//	Get form info
	form = temp_sequence[0];
	if (form != 'C' && form != 'L')
		form = 'L';
	found = temp_sequence.find_first_of(separator);
	//Get end of form info
	temp_sequence.erase(0, found + 1);

	//	Get end of comment info
	found = temp_sequence.find_first_of(separator);
	comment = temp_sequence.substr(0, found);

	//	set begin to start of sequence
	temp_sequence.erase(0, found + 1);

	if((type == 'D' || type == 'R') && version.find("version beta 2") == 0)
		DecodeDNAtoolsFile(sequence_to_pass_back, temp_sequence, type);
	else if(temp_sequence.length() > 0) {
		sequence_to_pass_back = "";
		auto it = temp_sequence.cbegin();

		while(it != temp_sequence.cend()) {
			if(m_seqValidator.IsAminoAcid(*it))
				sequence_to_pass_back += *it;
			++it;
		}
	}
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());
	if (type != 'D' && type != 'R' && type != 'P' && type != 'G')
		type = m_fileTypeDetector.GetMostLikelySequenceType(sequence_to_pass_back.c_str());
	return type;
}

char CFileDecoder::DecodeGCGFile(string& sequence_to_pass_back)
{
	string start = "..";
	string temp_sequence = m_content;
	size_t found = temp_sequence.find(start);
	if(found == string::npos)
		return '\0';

	temp_sequence.erase(0, found + start.length());
	sequence_to_pass_back = temp_sequence;
	transform(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), sequence_to_pass_back.begin(), ::toupper);
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());
	return m_fileTypeDetector.GetMostLikelySequenceType(sequence_to_pass_back.c_str());
}

char CFileDecoder::DecodeGenBankFile(string& sequence_to_pass_back)
{
	string start = "ORIGIN";
	string temp_sequence = m_content;
	size_t found = temp_sequence.find(start);
	if(found == string::npos)
		return '\0';

	temp_sequence.erase(0, found + start.length());
	found = temp_sequence.find("//");
	if(found == string::npos)
		sequence_to_pass_back = temp_sequence;
	else
		sequence_to_pass_back = temp_sequence.substr(0, found);

	transform(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), sequence_to_pass_back.begin(), ::toupper);
	sequence_to_pass_back.erase(remove_if(sequence_to_pass_back.begin(), sequence_to_pass_back.end(), ::isspace), sequence_to_pass_back.end());
	return m_fileTypeDetector.GetMostLikelySequenceType(sequence_to_pass_back.c_str());
}


bool CFileDecoder::ExtractCharactersFromByte(char byte, string& characters, char type)
{
	//	All DNAssist bytes coding bases have the 7th bit set.
	//	Thus, test this.  If it is not set, return 0;
	if(!(byte & 128))
		return false;

	int number_of_bases_in_byte  = m_fileTypeDetector.GetNumberOfBasesInByte(byte);
	int code;
	int bottom_bit = 3;
	int combined_bits;
	characters.clear();
	characters.resize(number_of_bases_in_byte);
	char base;
	
	if(type == 'D') {
		for(int x = 0; x < number_of_bases_in_byte; x++) {
			// Generate mask with shifted 11s (00000011, 00001100 etc.) in successive rounds
			//	Use this to mask out the relevant bits.  Shift the two 'lifted-out' bits
			//	back down to bit-place 1 and 0.
			combined_bits = bottom_bit<<(x*2);
			code = (byte & combined_bits)>>(x*2);
			base = m_fileTypeDetector.GetDNABase(code);

			if(m_seqValidator.IsDNAbase(base))
				characters[number_of_bases_in_byte-x-1] = base;
			else
				return false;
		}
	}
	else if(type == 'R') {
		for(int x = 0; x < number_of_bases_in_byte; x++) {
			// Same bit operation as for DNA above
			combined_bits = bottom_bit<<(x*2);
			code = (byte & combined_bits)>>(x*2);
			base = m_fileTypeDetector.GetRNABase(code);

			if(m_seqValidator.IsRNAbase(base))
				characters[number_of_bases_in_byte-x-1] = base;
			else
				return false;
		}
	}
	else
		return false;
	return characters.length() > 0;
}

bool CFileDecoder::DecodeDNAtoolsFile(string& sequence_to_pass_back, const string& temp_seq, char type)
{
/*
Byte is 128|64|32|16|8|4|2|1 -- value
          7  6  5  4 3 2 1 0 -- bit number
The DNAssist file is encoded with bits 7 set.  This ensures that the
byte value is at least 128 and outside of the value that is assigned as a delimiter
in the ReadFile function above that is stripped by ifstream reading a text file.
Maximally, bits 5+4, 3+2 and 1+1 each code for one base. Bit 6 is not set.  If bit 6
is set, that means that less than 3 bases are encoded.  In this case if bit 5 is not set
two bases are present (bits 3+2 and 1+0).  If bit 5 is set, only one base is present (bits 1+0).

The 'coding' bits in the byte are used for one of the DNA (G, A, T and C) or RNA
(G, A, U and C) bases -- 2 bits per base. The two bits are used as
G: 00, A: 01, T or U: 10 and C: 11.  Thus, the three bases GAC will to assigned
to bits 5+4, 3+2 and 1+0 respectively: (0*32+0*16) + (0*8+1*4) + (1*2+1*1) = 7.  Add
to this bit 7 (128), and the byte value for GAC is 135.  The two bases GT will be:
128(bit 7 set) +64 (bit 6 set) +0 (bit 5 not set) +0 (G: bits 3+2 not set)+2 (T: bit 1 set).
The one base C will be:
128(bit 7 set) +64 (bit 6 set) +32 (bit 5 set; one base present) +0 (bits 4+3+2 unset)
+3 (C: bits 1+0 set).
This scheme allow up to three DNA or RNA bases to be stored per byte.
*/
	string threebases = "";
	sequence_to_pass_back = "";
	auto it = m_content.cbegin();

	while(it != m_content.cend()) {
   //	If it is a garbage character (bit 7 not set), ExtractCharactersFromByte will
   //return 0.  If this happens, don't insert it.  This usually happens with
   //	the last byte in the file fetched by ifstream ('\n').
		if(ExtractCharactersFromByte(*it, threebases, type))
			sequence_to_pass_back += threebases;
		++it;
	}

	return sequence_to_pass_back.length() > 0;
}

char CFileDecoder::GetSequence(string& sequence_to_pass_back, char& form, string& comment, const string& applicationname)
{
	if(m_extension == L"")
		m_extension = m_fileTypeDetector.DetermineMostLikelyFileType(m_content.c_str(), applicationname);
	//	Read the file depending on the file extension
	if(m_extension == L"txt")
		return DecodeTextFile(sequence_to_pass_back);
	if(m_extension == L"str")
		return DecodeStriderFile(sequence_to_pass_back);
	if(m_extension == L"fas")
		return DecodeFastaFile(sequence_to_pass_back);
	if(m_extension == L"seq")
		return DecodeDNAtoolsFile(sequence_to_pass_back, form, comment, applicationname);
	if(m_extension == L"gcg")
		return DecodeGCGFile(sequence_to_pass_back);
	if(m_extension == L"gbk")
		return DecodeGenBankFile(sequence_to_pass_back);

	return 0;
}

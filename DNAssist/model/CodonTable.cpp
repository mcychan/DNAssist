#include "CodonTable.h"
#include "SeqConvertor.h"

#include <ctype.h>
#include <fstream>      // std::ifstream
#include <iterator>     // std::istream_iterator
#include <map>

static map<char, short> baseIndexMap;

CCodonTable::CCodonTable(const wstring& fullfilepath)
{
	tablename = "";
	seqConvertor = make_unique<CSeqConvertor>();
	filename = fullfilepath;

	// Initialize the codontable
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			for(int z = 0; z < 4; z++) {
				table[x][y][z] = '0';
			}
		}
	}

	if(baseIndexMap.empty()) {
		baseIndexMap['G'] = 0;
		baseIndexMap['g'] = 0;
		baseIndexMap['A'] = 1;
		baseIndexMap['a'] = 1;
		baseIndexMap['T'] = 2;
		baseIndexMap['t'] = 2;
		baseIndexMap['U'] = 2;
		baseIndexMap['u'] = 2;
		baseIndexMap['C'] = 3;
		baseIndexMap['c'] = 3;
	}

	LoadCodonTableFromFile();
}

CCodonTable::~CCodonTable()
{
}

void CCodonTable::TrimStart()
{
	string start = " ..";
	size_t found = data.find(start);
	if(found != string::npos)
		data.erase(0, found + start.length());
}

string CCodonTable::ReadThreeLetters(size_t index)
{
	if(index + 2 > data.length())
		return "";

	if(isalpha(data[index]) && isalpha(data[index+1]) && isalpha(data[index+2]))
		return data.substr(index, 3);
	return "";
}

short CCodonTable::GetBaseIndex(char base)
{
	auto result = baseIndexMap.find(base);
	return result == baseIndexMap.end() ? -1 : result->second;
}

CodonIndex CCodonTable::GetCodonIndex(string codon)
{
	if(codon.length() == 3)
		return CodonIndex(GetBaseIndex(codon[0]), GetBaseIndex(codon[1]), GetBaseIndex(codon[2]));
	return CodonIndex(-1, -1, -1);
}

bool CCodonTable::LoadCodonTable(string codon, string aminoacid)
{
	CodonIndex index = GetCodonIndex(codon);
	if(index.one < 0 || index.two < 0 || index.three < 0)
		return false;

	if (aminoacid.length() != 1)
		table[index.one][index.two][index.three] = GetSeqConvertor()->ConvertAminoAcidThreeToOne(aminoacid);
	else
		table[index.one][index.two][index.three] = aminoacid[0];
	return true;  // Not testing for anything yet
}

bool CCodonTable::GetCodonsFromFileString()
{
	TrimStart();
	if(data.length() == 0)
		return false;	//MessageBox

	int x = 0;
	while(x < data.length()) {
		while(x < data.length() - 2 && !isalpha(data[x]))
			x++;

		string aa = ReadThreeLetters(x);
		if(GetSeqConvertor()->IsAminoAcid(aa)) {
			x += 3;

			while(x < data.length() - 2 && !isalpha(data[x]))
				x++;

			string cd = ReadThreeLetters(x);
			GetSeqConvertor()->ConvertRNAToDNA(cd);
			if(GetSeqConvertor()->IsCodon(cd)) {
				LoadCodonTable(cd, aa);
				x += 3;
			}
			else
				return false; // Not valid file
		}
		else
			return false;	// not valid file
   	}
	return true;
}

bool CCodonTable::LoadCodonTableFromFile()
{
	ifstream ifs(filename);
	if(ifs) {
		data = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
		ifs.close();
	}
	else
		data = "";
	return GetCodonsFromFileString();
}

char CCodonTable::GetBaseIndex(int index, char type)
{
	switch(index) {
		case 0 : return 'G';
		case 1 : return 'A';
		case 2 :
		{
			if(type == 'D')
				return 'T';
			if(type == 'R')
				return 'U';
			return 'X';
		}
		case 3 : return 'C';
		default : return 'X';
	}
}

char CCodonTable::GetAminoAcid(string codon)
{
	CodonIndex index = GetCodonIndex(codon);
	if(index.one != -1 && index.two != -1 && index.three != -1)
		return table[index.one][index.two][index.three];
	return 'X';
}

string CCodonTable::GetTableName()
{
	return "";
}

bool CCodonTable::IsCodonTableValid()
{
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			for(int z = 0; z < 4; z++) {
				if(!GetSeqConvertor()->IsAminoAcid(table[x][y][z]))
					return false;
			}
		}
	}
	return true;
}

string CCodonTable::GetStartCodon(string& start, char type)
{
	if(type == 'D')
		start = "ATG";
	else if(type == 'R')
		start = "AUG";
	else
		start = "";
	return start;
}

string CCodonTable::GetCodon(string& codon, const CodonIndex& index, char type)
{
	codon.resize(3);
	codon[0] = GetBaseIndex(index.one, type);
	codon[1] = GetBaseIndex(index.two, type);
	codon[2] = GetBaseIndex(index.three, type);
	return codon;
}

void CCodonTable::GetStopCodons(vector<string>& result, char type)
{
	result.reserve(4 * 4 * 4);
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			for(int z = 0; z < 4; z++) {
				char zop = table[x][y][z];
				if(zop == '*') {
					string codon;
					GetCodon(codon, CodonIndex(x, y, z), type);
					result.emplace_back(codon);
				}
			}
		}
	}
}

void CCodonTable::GetCodonFromAminoAcid(vector<string>& result, char aminoacid)
{
	result.reserve(4 * 4 * 4);
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			for(int z = 0; z < 4; z++) {
				if(table[x][y][z] == aminoacid) {
					string codon;
					GetCodon(codon, CodonIndex(x, y, z), 'D');
					result.emplace_back(codon);
				}
			}
		}
	}
}

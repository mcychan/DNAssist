#pragma once

#include <memory>
#include <string>
#include <vector>
using namespace std;

class CSeqConvertor;

struct CodonIndex
{
	short one;
	short two;
	short three;

	CodonIndex(short x, short y, short z) : one(x), two(y), three(z) {
	}
};

class CCodonTable
{
public:
	CCodonTable(const wstring& fullfilepath);
	~CCodonTable();
	bool LoadCodonTableFromFile();
	bool IsCodonTableValid();
	char GetAminoAcid(string codon);
	void GetStopCodons(vector<string>& result, char type = 'D');
	string GetStartCodon(string& start, char type = 'D');
	void GetCodonFromAminoAcid(vector<string>& result, char aminoacid);

private:
	char table[4][4][4]; // This contains the 1 letter aa for a x,y,z codon
	string tablename;
	wstring filename;
	string data;
	unique_ptr<CSeqConvertor> seqConvertor;

protected:
	CodonIndex GetCodonIndex(string codon);
	short GetBaseIndex(char base);
	bool GetCodonsFromFileString();
	string GetTableName();
	void TrimStart();
	string ReadThreeLetters(size_t index);
	bool LoadCodonTable(string codon, string aminoacid);
	string GetCodon(string& codon, const CodonIndex& index, char type = 'D');
	char GetBaseIndex(int index, char type = 'D');
	inline CSeqConvertor* GetSeqConvertor() {
		return seqConvertor.get();
	}

};

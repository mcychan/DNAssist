#include "SeqConvertor.h"
#include "CodonTable.h"

#include <random>

static map<char, char> dgMap, rMap;

char CSeqConvertor::GetBaseComplement(char input, char intype)
{
	if(intype == 'D' || intype == 'G') {
		if(dgMap.empty()) {
			dgMap['G'] = 'C';
			dgMap['A'] = 'T';
			dgMap['T'] = 'A';
			dgMap['C'] = 'G';
			dgMap['R'] = 'Y';
			dgMap['Y'] = 'R';
			dgMap['M'] = 'K';
			dgMap['K'] = 'M';
			dgMap['S'] = 'S';
			dgMap['W'] = 'W';
			dgMap['B'] = 'V';
			dgMap['V'] = 'B';
			dgMap['D'] = 'H';
			dgMap['H'] = 'D';
			dgMap['N'] = 'N';
			dgMap['g'] = 'c';
			dgMap['a'] = 't';
			dgMap['t'] = 'a';
			dgMap['c'] = 'g';
			dgMap['r'] = 'y';
			dgMap['y'] = 'r';
			dgMap['m'] = 'k';
			dgMap['k'] = 'm';
			dgMap['s'] = 's';
			dgMap['w'] = 'w';
			dgMap['b'] = 'v';
			dgMap['v'] = 'b';
			dgMap['d'] = 'h';
			dgMap['h'] = 'd';
			dgMap['n'] = 'n';
		}

		auto it = dgMap.find(input);
		if (it != dgMap.end())
			return it->second;
	}
	else if (intype == 'R') {
		if(rMap.empty()) {
			rMap['G'] = 'C';
			rMap['A'] = 'U';
			rMap['U'] = 'A';
			rMap['C'] = 'G';
			rMap['g'] = 'c';
			rMap['a'] = 'u';
			rMap['u'] = 'a';
			rMap['c'] = 'g';
		}

		auto it = rMap.find(input);
		if (it != rMap.end())
			return it->second;
	}
	return '\0';
}

string CSeqConvertor::SequenceComplement(const string& inputseq, string& outputseq, char seqtype)
{
	outputseq = "";
	string::const_iterator it = inputseq.begin();

	while(it != inputseq.end()) {
		outputseq += GetBaseComplement(*it, seqtype);
		++it;
	}
	return outputseq;
}

string CSeqConvertor::SequenceReverse(const string& inputseq, string& outputseq)
{
	outputseq = string(inputseq.rbegin(), inputseq.rend());
	return outputseq;
}

string CSeqConvertor::SequenceReverseComplement(const string& inputseq, string& outputseq, char seqtype)
{
	string complement = "";
	SequenceComplement(inputseq, complement, seqtype);
	SequenceReverse(complement, outputseq);
	return outputseq;
}

char CSeqConvertor::GetBaseConverted(char input, char intype, char outtype)
{
	char output = '\0';
	if(intype == 'D' && outtype == 'R') {
		switch(input)
		{
			case 'G': output = 'G'; break;
			case 'A': output = 'A'; break;
			case 'T': output = 'U'; break;
			case 'C': output = 'C'; break;
			case 'g': output = 'g'; break;
			case 'a': output = 'a'; break;
			case 't': output = 'u'; break;
			case 'c': output = 'c'; break;
		}
	}
	else if (intype == 'R' && outtype == 'D') {
		switch(input)
		{
			case 'G': output = 'G'; break;
			case 'A': output = 'A'; break;
			case 'U': output = 'T'; break;
			case 'C': output = 'C'; break;
			case 'g': output = 'g'; break;
			case 'a': output = 'a'; break;
			case 'u': output = 't'; break;
			case 'c': output = 'c'; break;
		}
	}
	return output;
}

string CSeqConvertor::GetListOfAminoAcids(string& array)
{
	array = "GAVLIPFYWSTDENQCMHKR";
	return array;
}

string CSeqConvertor::ConvertAminoAcidOneToThree(const char letter, string& three)
{
	if(letter == 'A' || letter == 'a') {
		three = "ala";
		return three;
	}
	if(letter == 'V' || letter == 'v') {
      three = "val";
      return three;
	}
	if(letter == 'L' || letter == 'l') {
      three = "leu";
      return three;
	}
	if(letter == 'I' || letter == 'i') {
      three = "ile";
      return three;
	}
	if(letter == 'P' || letter == 'p') {
		three = "pro";
		return three;
	}
	if(letter == 'F' || letter == 'f') {
		three = "phe";
		return three;
	}
	if(letter == 'Y' || letter == 'y') {
		three = "tyr";
		return three;
	}
	if(letter == 'W' || letter == 'w') {
		three = "trp";
		return three;
	}
	if(letter == 'S' || letter == 's') {
		three = "ser";
		return three;
	}
	if(letter == 'T' || letter == 't') {
		three = "thr";
		return three;
	}
	if(letter == 'D' || letter == 'd') {
		three = "asp";
      return three;
	}
	if(letter == 'E' || letter == 'e') {
      three = "glu";
      return three;
	}
	if(letter == 'N' || letter == 'n') {
		three = "asn";
		return three;
	}
	if(letter == 'Q' || letter == 'q') {
		three = "gln";
		return three;
	}
	if(letter == 'C' || letter == 'c') {
		three = "cys";
		return three;
	}
	if(letter == 'M' || letter == 'm') {
		three = "met";
		return three;
	}
	if(letter == 'H' || letter == 'h') {
		three = "his";
		return three;
	}
	if(letter == 'K' || letter == 'k') {
		three = "lys";
		return three;
	}
	if(letter == 'R' || letter == 'r') {
		three = "arg";
		return three;
	}
	if(letter == 'G' || letter == 'g') {
		three = "gly";
		return three;
	}
	if(letter == '*') {
		three = "end";
		return three;
	}

	return three;
}

char CSeqConvertor::ConvertAminoAcidThreeToOne(const string& aa)
{
	if(aa.length() == 0)
		return 0;

	string three = aa;
	transform(three.begin(), three.end(), three.begin(), ::tolower);
	if(three == "gly")
		return 'G';
	if(three == "arg")
		return 'R';
	if(three == "lys")
		return 'K';
	if(three == "his")
		return 'H';
	if(three == "met")
		return 'M';
	if(three == "cys")
		return 'C';
	if(three == "gln")
		return 'Q';
	if(three == "asn")
		return 'N';
	if(three == "glu")
		return 'E';
	if(three == "asp")
		return 'D';
	if(three == "thr")
		return 'T';
	if(three == "ser")
		return 'S';
	if(three == "trp")
		return 'W';
	if(three == "tyr")
		return 'Y';
	if(three == "phe")
		return 'F';
	if(three == "pro")
		return 'P';
	if(three == "ile")
		return 'I';
	if(three == "leu")
		return 'L';
	if(three == "val")
		return 'V';
	if(three == "ala")
		return 'A';
	if(three == "end")
		return '*';

	return 0;
}

bool CSeqConvertor::IsAminoAcid(const char letter)
{
	string temp = "";
	return ConvertAminoAcidOneToThree(letter, temp) != "";
}

bool CSeqConvertor::IsAminoAcid(const string& letter)
{
	string temp = "";
	if(letter.length() == 1) {
		if(ConvertAminoAcidOneToThree(letter[0], temp) != "")
			return true;
	}
	else if(letter.length() == 3) {
		if(ConvertAminoAcidThreeToOne(letter))
			return true;
	}
	return false;
}

bool CSeqConvertor::IsCodon(string& codon)
{
	if(codon.length() != 3)
		return false;

	transform(codon.begin(), codon.end(), codon.begin(), ::toupper);
	if(codon == "GGG")
		return true;
	if(codon == "GGA")
		return true;
	if(codon == "GGT")
		return true;
	if(codon == "GGC")
		return true;
	if(codon == "GAG")
		return true;
	if(codon == "GAA")
		return true;
	if(codon == "GAT")
		return true;
	if(codon == "GAC")
		return true;
	if(codon == "GTG")
		return true;
	if(codon == "GTA")
		return true;
	if(codon == "GTT")
		return true;
	if(codon == "GTC")
		return true;
	if(codon == "GCG")
		return true;
	if(codon == "GCA")
		return true;
	if(codon == "GCT")
		return true;
	if(codon == "GCC")
		return true;

	if(codon == "AGG")
		return true;
	if(codon == "AGA")
		return true;
	if(codon == "AGT")
		return true;
	if(codon == "AGC")
		return true;
	if(codon == "AAG")
		return true;
	if(codon == "AAA")
		return true;
	if(codon == "AAT")
		return true;
	if(codon == "AAC")
		return true;
	if(codon == "ATG")
		return true;
	if(codon == "ATA")
		return true;
	if(codon == "ATT")
		return true;
	if(codon == "ATC")
		return true;
	if(codon == "ACG")
		return true;
	if(codon == "ACA")
		return true;
	if(codon == "ACT")
		return true;
	if(codon == "ACC")
		return true;


	if(codon == "TGG")
		return true;
	if(codon == "TGA")
		return true;
	if(codon == "TGT")
		return true;
	if(codon == "TGC")
		return true;
	if(codon == "TAG")
		return true;
	if(codon == "TAA")
		return true;
	if(codon == "TAT")
		return true;
	if(codon == "TAC")
		return true;
	if(codon == "TTG")
		return true;
	if(codon == "TTA")
		return true;
	if(codon == "TTT")
		return true;
	if(codon == "TTC")
		return true;
	if(codon == "TCG")
		return true;
	if(codon == "TCA")
		return true;
	if(codon == "TCT")
		return true;
	if(codon == "TCC")
		return true;

	if(codon == "CGG")
		return true;
	if(codon == "CGA")
		return true;
	if(codon == "CGT")
		return true;
	if(codon == "CGC")
		return true;
	if(codon == "CAG")
		return true;
	if(codon == "CAA")
		return true;
	if(codon == "CAT")
		return true;
	if(codon == "CAC")
		return true;
	if(codon == "CTG")
		return true;
	if(codon == "CTA")
		return true;
	if(codon == "CTT")
		return true;
	if(codon == "CTC")
		return true;
	if(codon == "CCG")
		return true;
	if(codon == "CCA")
		return true;
	if(codon == "CCT")
		return true;
	if(codon == "CCC")
		return true;

	return false;
}

bool CSeqConvertor::IsPalindromic(const string& seq)
{
	string sitecomplement = "";
	string sitereverse = "";
	SequenceComplement(seq, sitecomplement, 'D');
	SequenceReverse(sitecomplement, sitereverse);
	return seq == sitereverse;
}

string CSeqConvertor::ConvertDNAToRNA(string& sequence)
{
	replace(sequence.begin(), sequence.end(), 'T', 'U');
	replace(sequence.begin(), sequence.end(), 't', 'u');
	return sequence;
}

string CSeqConvertor::ConvertRNAToDNA(string& sequence)
{
	replace(sequence.begin(), sequence.end(), 'U', 'T');
	replace(sequence.begin(), sequence.end(), 'u', 't');
	return sequence;
}

string CSeqConvertor::ReverseTranslate(const string& protein, string& degenerateDNA, CCodonTable* pCodontable)
{
	if(protein.length() == 0 || !pCodontable)
		return 0;

	vector<string> result;
	string::const_iterator it = protein.begin();

	while(it != protein.end()) {		
		pCodontable->GetCodonFromAminoAcid(result, *it);
		size_t numberofcodons = result.size();
		if(numberofcodons > 0) {			
			for(int z = 0; z < 3; z++) {
				string temp = "";
      			for(auto const& codon : result)
         			temp += codon[z];

				char base = GetDegenerateDNABase(temp);
				degenerateDNA += base;
			}
		}
		result.clear();
		it++;
	}
	return degenerateDNA;
}

size_t CSeqConvertor::ConvertNucleicAcidToProtein(const string& inputseq, string& outputseq, CCodonTable* pCodontable, bool truncateatstop)
{
	size_t length = inputseq.length() / 3;

//	If the in sequence < 3 bp or the codontable in invalid, return 0
	if(!length || !pCodontable)
		return 0;

	outputseq = "";
	for (int x = 0; x < length; x++) {
		string temp = inputseq.substr(3 * x, 3);
		outputseq += pCodontable->GetAminoAcid(temp);
	}

	if(truncateatstop) {
		size_t found = outputseq.find('*');
		if (found != string::npos) {
			outputseq.resize(found);
			return found;
		}
	}
		
	return 0;
}

size_t CSeqConvertor::ConvertSequence(const string& inputseq, string& outputseq,
												char intype, char outtype,
                                    CCodonTable* pCodontable, bool truncate)
{
	outputseq = "";
	if(outtype == 'D' || outtype == 'R') {
		string::const_iterator it = inputseq.begin();

		while(it != inputseq.end()) {
			outputseq += GetBaseConverted(*it, intype, outtype);
			++it;
		}
	}
	else if(outtype == 'P' && (intype == 'D' || intype == 'R'))
		return ConvertNucleicAcidToProtein(inputseq, outputseq, pCodontable, truncate);
	return 0;
}

char CSeqConvertor::GetDegenerateDNABase(const string& mixofbases)
{
	int binarycode = 0;
	if(mixofbases.find('G') != string::npos)
		binarycode += 8;
	if(mixofbases.find('A') != string::npos)
		binarycode += 4;
	if(mixofbases.find('T') != string::npos)
		binarycode += 2;
	if(mixofbases.find('C') != string::npos)
		binarycode += 1;

	switch(binarycode) {
		case 8 : return 'G';
		case 4 : return 'A';
		case 2 : return 'T';
		case 1 : return 'C';
		case 5 : return 'M';
		case 12 : return 'R';
		case 6 : return 'W';
		case 9 : return 'S';
		case 3 : return 'Y';
		case 10 : return 'K';
		case 13 : return 'V';
		case 7 : return 'H';
		case 14 : return 'D';
		case 11 : return 'B';
		case 15 : return 'N';
		default : return '\0';
	}
}

string CSeqConvertor::RandomizeSequence(const string& inputseq, string& outputseq)
{
	outputseq = inputseq;
	shuffle(outputseq.begin(), outputseq.end(), default_random_engine(random_device()()));
	return outputseq;
}


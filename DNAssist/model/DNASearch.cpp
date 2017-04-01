#include "DNASearch.h"

static map<char, int> dnaMap, rnaMap, proteinMap;

CDNASearch::CDNASearch(const string& sequence_to_be_searched, size_t length_of_string, char type_of_sequence, char form_of_sequence)
{
	sequence_type = type_of_sequence;
	data_length = length_of_string;
	sequence_form = form_of_sequence;
	if(sequence_type == 'D' || sequence_type == 'R') {
		skip = make_unique<int[]>(5);
		alphabet = 4;
	}
	else if(sequence_type == 'P') {
		skip = make_unique<int[]>(21);
		alphabet = 20;
	}
	data = sequence_to_be_searched;
	transform(data.begin(), data.end(), data.begin(), ::toupper);

	if (dnaMap.empty()) {
		dnaMap['G'] = 1;
		dnaMap['A'] = 2;
		dnaMap['T'] = 3;
		dnaMap['C'] = 4;
	}

	if (rnaMap.empty()) {
		rnaMap['G'] = 1;
		rnaMap['A'] = 2;
		rnaMap['U'] = 3;
		rnaMap['C'] = 4;
	}

	if (proteinMap.empty()) {
		proteinMap['A'] = 1;
		proteinMap['C'] = 2;
		proteinMap['D'] = 3;
		proteinMap['E'] = 4;
		proteinMap['F'] = 5;
		proteinMap['G'] = 6;
		proteinMap['H'] = 7;
		proteinMap['I'] = 8;
		proteinMap['K'] = 9;
		proteinMap['L'] = 10;
		proteinMap['M'] = 11;
		proteinMap['N'] = 12;
		proteinMap['P'] = 13;
		proteinMap['Q'] = 14;
		proteinMap['R'] = 15;
		proteinMap['S'] = 16;
		proteinMap['T'] = 17;
		proteinMap['V'] = 18;
		proteinMap['W'] = 19;
		proteinMap['Y'] = 20;
	}
}

CDNASearch::~CDNASearch()
{
}

int CDNASearch::index(char base)
{
	if(sequence_type == 'D') {
		auto result = dnaMap.find(base);
		return result == dnaMap.end() ? 0 : result->second;
	}
	if(sequence_type == 'R') {
		auto result = rnaMap.find(base);
		return result == rnaMap.end() ? 0 : result->second;
	}

	auto result = proteinMap.find(base);
	return result == proteinMap.end() ? 0 : result->second;
}

void CDNASearch::make_skip_table(const string& pattern)
{
	for(int i = 1; i <= alphabet; i++)
		skip[i] = pattern_length;
	for(int j = 0; j < pattern_length; j++)
		assign_degenerate_skip(pattern[j], j);
}

void CDNASearch::assign_degenerate_skip(char base, int j)
{
	if(sequence_type == 'D' || sequence_type == 'G') {
		switch(base)
		{
			case 'G': skip[index('G')] = pattern_length - j - 1; break;
			case 'A': skip[index('A')] = pattern_length - j - 1; break;
			case 'T': skip[index('T')] = pattern_length - j - 1; break;
			case 'C': skip[index('C')] = pattern_length - j - 1; break;
			case 'R':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('A')] = pattern_length - j - 1;
			}
			break;
			case 'Y':
			{
				skip[index('C')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'M':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
			break;
			case 'K':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'S':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
			break;
			case 'W':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'B':
			{
				skip[index('C')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'D':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'H':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
			}
			break;
			case 'V':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
			}
			break;
			case 'N':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('A')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
		}
	}
	else if(sequence_type == 'R') {
		switch(base)
		{
			case 'G': skip[index('G')] = pattern_length - j - 1; break;
			case 'A': skip[index('A')] = pattern_length - j - 1; break;
			case 'U': skip[index('U')] = pattern_length - j - 1; break;
			case 'C': skip[index('C')] = pattern_length - j - 1; break;
			case 'R':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('A')] = pattern_length - j - 1;
			}
			break;
			case 'Y':
			{
				skip[index('C')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'M':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
			break;
			case 'K':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'S':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
			break;
			case 'W':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'B':
			{
				skip[index('C')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'D':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'H':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
			}
			break;
			case 'V':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
			}
			break;
			case 'N':
			{
				skip[index('G')] = pattern_length - j - 1;
				skip[index('A')] = pattern_length - j - 1;
				skip[index('U')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
			}
		}
	}
	else {
		switch(base)
		{
			case 'A': skip[index('A')] = pattern_length - j - 1; break;
			case 'C': skip[index('C')] = pattern_length - j - 1; break;
			case 'D': skip[index('D')] = pattern_length - j - 1; break;
			case 'E': skip[index('E')] = pattern_length - j - 1; break;
			case 'F': skip[index('F')] = pattern_length - j - 1; break;
			case 'G': skip[index('G')] = pattern_length - j - 1; break;
			case 'H': skip[index('H')] = pattern_length - j - 1; break;
			case 'I': skip[index('I')] = pattern_length - j - 1; break;
			case 'K': skip[index('K')] = pattern_length - j - 1; break;
			case 'L': skip[index('L')] = pattern_length - j - 1; break;
			case 'M': skip[index('M')] = pattern_length - j - 1; break;
			case 'N': skip[index('N')] = pattern_length - j - 1; break;
			case 'P': skip[index('P')] = pattern_length - j - 1; break;
			case 'Q': skip[index('Q')] = pattern_length - j - 1; break;
			case 'R': skip[index('R')] = pattern_length - j - 1; break;
			case 'S': skip[index('S')] = pattern_length - j - 1; break;
			case 'T': skip[index('T')] = pattern_length - j - 1; break;
			case 'V': skip[index('V')] = pattern_length - j - 1; break;
			case 'W': skip[index('W')] = pattern_length - j - 1; break;
			case 'Y': skip[index('Y')] = pattern_length - j - 1; break;
			case 'B':
			{
				skip[index('N')] = pattern_length - j - 1;
				skip[index('D')] = pattern_length - j - 1;
			}
			break;
			case 'Z':
			{
				skip[index('E')] = pattern_length - j - 1;
				skip[index('Q')] = pattern_length - j - 1;
			}
			break;
			case 'X':
			{
				skip[index('A')] = pattern_length - j - 1;
				skip[index('C')] = pattern_length - j - 1;
				skip[index('D')] = pattern_length - j - 1;
				skip[index('E')] = pattern_length - j - 1;
				skip[index('F')] = pattern_length - j - 1;
				skip[index('G')] = pattern_length - j - 1;
				skip[index('H')] = pattern_length - j - 1;
				skip[index('I')] = pattern_length - j - 1;
				skip[index('K')] = pattern_length - j - 1;
				skip[index('L')] = pattern_length - j - 1;
				skip[index('M')] = pattern_length - j - 1;
				skip[index('N')] = pattern_length - j - 1;
				skip[index('P')] = pattern_length - j - 1;
				skip[index('Q')] = pattern_length - j - 1;
				skip[index('R')] = pattern_length - j - 1;
				skip[index('S')] = pattern_length - j - 1;
				skip[index('T')] = pattern_length - j - 1;
				skip[index('V')] = pattern_length - j - 1;
				skip[index('W')] = pattern_length - j - 1;
				skip[index('Y')] = pattern_length - j - 1;
			}
		}
	}
}

int CDNASearch::FuzzyCircular(const string& pattern, int search_start, int miss)
{
	int mismatch = 0;
	if(search_start < data_length) {
		for(int j = pattern_length-1; j>=0 && search_start < data_length + pattern_length - 1; j--, search_start--) {
			while(!(compare_degenerate(data, search_start % data_length, pattern, j))) {
				mismatch++;
				if(mismatch <= miss)
					break;
				mismatch = 0;
				int t = skip[index(data[search_start % data_length])];
				search_start += ((pattern_length-j)>t) ? pattern_length-j : t;
				j = pattern_length - 1;
				if(search_start>=data_length+pattern_length)
					return data_length;
			}
		}
		return search_start + 1;
	}
	return data_length;
}

int CDNASearch::FuzzyLinear(const string& pattern, int search_start, int miss)
{
	int mismatch = 0;
	if(search_start < data_length) {
		for(int j = pattern_length-1; j>=0 && search_start < data_length; j--,search_start--) {
			while(!(compare_degenerate(data, search_start, pattern, j))) {
				mismatch++;
				if(mismatch <= miss)
					break;
				mismatch = 0;
				int t = skip[index(data[search_start])];
				search_start += ((pattern_length-j) > t) ? pattern_length-j : t;
				j = pattern_length - 1;
				if(search_start>=data_length)
					return data_length;
			}
		}
		return search_start + 1;
	}
	return data_length;
}

int CDNASearch::FuzzySearch(const string& pattern_string, size_t length, int search_start, int miss)
{
	int result;
	string pattern = pattern_string;
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
	pattern_length = min(length, pattern.length());
	search_start = search_start + length - 1;
	make_skip_table(pattern);
	if(sequence_form == 'C')
		result = FuzzyCircular(pattern, search_start, miss);
	else
		result = FuzzyLinear(pattern, search_start, miss);
	return result;
}

int CDNASearch::FuzzySearchMultiple(const string& pattern_to_match, size_t length_of_pattern, vector<int>& resultarray, int search_start, int miss)
{
	string pattern = pattern_to_match;
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
	pattern_length = min(length_of_pattern, pattern.length());
	size_t result = search_start+length_of_pattern-1;
	make_skip_table(pattern);
	resultarray.reserve(data_length + length_of_pattern);
	if(sequence_form == 'C') {
		do {
			result = FuzzyCircular(pattern, result, miss);
			resultarray.push_back(result);
			result += length_of_pattern;
		}
		while(result <= data_length + length_of_pattern - 1);
		return resultarray.size();
	}

	do {
		result = FuzzyLinear(pattern, result, miss);
		resultarray.push_back(result);
		result += length_of_pattern;
	}
	while(result <= data_length);
	return resultarray.size();
}

int CDNASearch::SearchCircular(const string& pattern, int search_start)
{
	if(search_start < data_length) {
		for(int j = pattern_length-1; j>=0 && search_start < data_length + pattern_length - 1; j--,search_start--) {
			while(!(Compare(data, search_start % data_length, pattern, j))) {
				int t = skip[index(data[search_start % data_length])];
				search_start += ((pattern_length - j) > t) ? pattern_length - j : t;
				j = pattern_length - 1;
				if(search_start >= data_length + pattern_length)
					return data_length;
			}
		}
		return search_start+1;
	}
	return data_length;
}

int CDNASearch::SearchLinear(const string& pattern, int search_start)
{
	if(search_start<data_length) {
		for(int j = pattern_length-1; j>=0 && search_start < data_length; j--, search_start--) {
			while(!(Compare(data, search_start, pattern, j))) {
				int t = skip[index(data[search_start])];
				search_start += ((pattern_length - j) > t) ? pattern_length - j : t;
				j = pattern_length-1;
				if(search_start >= data_length)
					return data_length;
			}
		}
		return search_start + 1;
	}
	return data_length;
}

int CDNASearch::Search(const string& pattern_string, size_t length, int search_start)
{
	int result;
	string pattern = pattern_string;
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
	pattern_length = min(length, pattern.length());
	search_start = search_start + length - 1;
	make_skip_table(pattern);
	if(sequence_form == 'C')
		result = SearchCircular(pattern, search_start);
	else
		result = SearchLinear(pattern, search_start);
	return result;
}

int CDNASearch::SearchMultiple(const string& pattern_to_match, size_t length_of_pattern, vector<int>& resultarray, int search_start)
{
	string pattern = pattern_to_match;
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::toupper);
	pattern_length = min(length_of_pattern, pattern.length());
	int result = search_start + length_of_pattern - 1;
	make_skip_table(pattern);
	resultarray.reserve(data_length + length_of_pattern);
	if(sequence_form == 'C') {
		do {
			result = SearchCircular(pattern, result);
			resultarray.push_back(result);
			result += length_of_pattern;
		}
		while(result <= data_length + length_of_pattern - 1);
		return resultarray.size();
	}

	do {
		result = SearchLinear(pattern, result);
		resultarray.push_back(result);
		result += length_of_pattern;
	}
	while(result <= data_length);
	return resultarray.size();
}

bool CDNASearch::compare_degenerate(const string& data, int i, string pattern, int j)
{
	if(sequence_type == 'D') {
		switch(pattern[j]) {
			case 'G' : return (data[i] == 'G');
			case 'A' : return (data[i] == 'A');
			case 'T' : return (data[i] == 'T');
			case 'C' : return (data[i] == 'C');
			case 'R' : return (data[i] == 'G' || data[i] == 'A');
			case 'Y' : return (data[i] == 'C' || data[i] == 'T');
			case 'M' : return (data[i] == 'A' || data[i] == 'C');
			case 'K' : return (data[i] == 'G' || data[i] == 'T');
			case 'S' : return (data[i] == 'G' || data[i] == 'C');
			case 'W' : return (data[i] == 'A' || data[i] == 'T');
			case 'B' : return (data[i] == 'G' || data[i] == 'C' || data[i] == 'T');
			case 'D' : return (data[i] == 'A' || data[i] == 'G' || data[i] == 'T');
			case 'H' : return (data[i] == 'A' || data[i] == 'C' || data[i] == 'T');
			case 'V' : return (data[i] == 'A' || data[i] == 'C' || data[i] == 'G');
			case 'N' : return (data[i] == 'G' || data[i] == 'A' || data[i] == 'T' || data[i] == 'C');
			default : return false;
		}
	}
	if(sequence_type == 'R') {
		switch(pattern[j]) {
			case 'G' : return (data[i] == 'G');
			case 'A' : return (data[i] == 'A');
			case 'U' : return (data[i] == 'U');
			case 'C' : return (data[i] == 'C');
			case 'R' : return (data[i] == 'G' || data[i] == 'A');
			case 'Y' : return (data[i] == 'C' || data[i] == 'T');
			case 'M' : return (data[i] == 'A' || data[i] == 'C');
			case 'K' : return (data[i] == 'G' || data[i] == 'T');
			case 'S' : return (data[i] == 'G' || data[i] == 'C');
			case 'W' : return (data[i] == 'A' || data[i] == 'T');
			case 'B' : return (data[i] == 'G' || data[i] == 'C' || data[i] == 'T');
			case 'D' : return (data[i] == 'A' || data[i] == 'G' || data[i] == 'T');
			case 'H' : return (data[i] == 'A' || data[i] == 'C' || data[i] == 'T');
			case 'V' : return (data[i] == 'A' || data[i] == 'C' || data[i] == 'G');
			case 'N' : return (data[i] == 'G' || data[i] == 'A' || data[i] == 'T' || data[i] == 'C');
			default : return false;
		}
	}

	switch(pattern[j]) {
		case 'A' : return (data[i] == 'A');
		case 'B' : return (data[i] == 'D' || data[i] == 'N');
		case 'C' : return (data[i] == 'C');
		case 'D' : return (data[i] == 'D');
		case 'E' : return (data[i] == 'E');
		case 'F' : return (data[i] == 'F');
		case 'G' : return (data[i] == 'G');
		case 'H' : return (data[i] == 'H');
		case 'I' : return (data[i] == 'I');
		case 'K' : return (data[i] == 'K');
		case 'L' : return (data[i] == 'L');
		case 'M' : return (data[i] == 'M');
		case 'N' : return (data[i] == 'N');
		case 'P' : return (data[i] == 'P');
		case 'Q' : return (data[i] == 'Q');
		case 'R' : return (data[i] == 'R');
		case 'S' : return (data[i] == 'S');
		case 'T' : return (data[i] == 'T');
		case 'V' : return (data[i] == 'V');
		case 'W' : return (data[i] == 'W');
		case 'X' : return (data[i] == 'A' || data[i] == 'C' ||
				data[i] == 'D' || data[i] == 'E' ||
				data[i] == 'F' || data[i] == 'G' ||
				data[i] == 'H' || data[i] == 'I' ||
				data[i] == 'K' || data[i] == 'L' ||
				data[i] == 'M' || data[i] == 'N' ||
				data[i] == 'P' || data[i] == 'Q' ||
				data[i] == 'R' || data[i] == 'S' ||
				data[i] == 'T' || data[i] == 'V' ||
				data[i] == 'W' || data[i] == 'Y');
		case 'Y' : return (data[i] == 'Y');
		case 'Z' : return (data[i] == 'Q' || data[i] == 'E');
		default : return false;
	}
}

bool CDNASearch::Compare(const string& data, int i, string pattern, int j)
{
	return pattern[j] == data[i];
}

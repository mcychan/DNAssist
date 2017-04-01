#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class CDNASearch
{

public:
	CDNASearch(const string& sequence_to_be_searched, size_t length_of_string, char type_of_sequence = 'D', char form_of_sequence = 'L');
	~CDNASearch();

	char sequence_type; //DNA (D), RNA (R) or protein (P) sequence to be searched
	char sequence_form; //C = circular, L = linear
	unique_ptr<int[]> skip;
	string data; // pointer to sequence to be searched
	size_t pattern_length; // length of pattern
	size_t data_length; //length of sequence
	int alphabet; // number of different characters in sequence_type sequence
	int FuzzySearch(const string& pattern_to_match, size_t length_of_pattern, int search_start = 0, int miss = 0);
	int FuzzySearchMultiple(const string& pattern_to_match, size_t length_of_pattern, vector<int>& resultarray, int search_start = 0, int miss = 0);
	int Search(const string& pattern_to_match, size_t length_of_pattern, int search_start = 0);
	int SearchMultiple(const string& pattern_to_match, size_t length_of_pattern, vector<int>& resultarray, int search_start = 0);

protected:
	int index(char base);
	void make_skip_table(const string& pattern);
	void assign_degenerate_skip(char, int);
	bool compare_degenerate(const string& data, int i, string pattern, int j);
	bool Compare(const string& data, int i, string pattern, int j);
	int FuzzyLinear(const string& pattern, int start, int misses);
	int FuzzyCircular(const string& pattern, int start, int misses);
	int SearchLinear(const string& pattern, int start);
	int SearchCircular(const string& pattern, int start);
};

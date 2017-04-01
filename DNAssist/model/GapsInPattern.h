#pragma once

#include <memory>
#include <string>
#include <vector>
using namespace std;

class CGapsInPattern {
protected:
	int combinations, gaps, maxgapsize;
public:
	CGapsInPattern(int number_of_gaps, int maximum_gap_size);
	~CGapsInPattern();

	int MakeGaps(vector<vector<int> >& result);
	int HoleCombinations(int sequencelength, vector<vector<int> >& result);
	int PermuteSequence(const char* pattern, int length, vector<vector<int> >& result);
	void SortResult(int entries, int width, vector<vector<int> >& result);
	int CompareEntries(int width, const int* test, const int* reference);
	int RemoveDuplicates(int entries, int width, vector<vector<int> >& result);
	void FormatString(string& data, char* pad_character, const int* format);
	int FormatStringArray(string& data, int numberofcombinations,
														 vector<string>& stringarray, char* pad_character,
														 vector<vector<int> > result);

};

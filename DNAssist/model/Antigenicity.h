#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class CAntigenicity
{
public:
	CAntigenicity(const string& seq, size_t windowsize);
	~CAntigenicity();
	bool CalculateAntigenicity(vector<float>& array);

protected:
	string sequence;	
	int start;
	int end;
	size_t m_nWindowsize;
	short m_nPercent = 0;

	void Initialise();
	int GetIndex(char residue);
	float GetAntigenicityValue(int index);
	float CalculateAntigenicityAtWindow(int windowstart);
};

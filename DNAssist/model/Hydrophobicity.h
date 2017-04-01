#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class CHydrophobicity
{
public:
	CHydrophobicity(const string& seq, size_t windowsize);
	~CHydrophobicity();
	bool CalculateHydrophobicity(vector<float>& array);

protected:
	string sequence;
	int start;
	int end;
	size_t m_nWindowsize;
	short m_nPercent = 0;

	void Initialise();
	int GetIndex(char residue);
	float GetHydrophobicityValue(int index);
	float CalculateHydrophobicityAtWindow(int windowstart);
};

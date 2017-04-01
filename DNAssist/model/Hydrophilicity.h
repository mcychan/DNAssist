#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class CHydrophilicity
{
public:
	CHydrophilicity(const string& seq, size_t windowsize);
	~CHydrophilicity();
	bool CalculateHydrophilicity(vector<float>& array);

protected:
	string sequence;
	int start;
	int end;
	size_t m_nWindowsize;
	short m_nPercent = 0;

	void Initialise();
	int GetIndex(char residue);
	float GetHydrophilicityValue(int index);
	float CalculateHydrophilicityAtWindow(int windowstart);
};

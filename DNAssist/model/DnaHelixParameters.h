#pragma once

#include <algorithm>
#include <map>
#include <string>
using namespace std;

class CDnaHelixParameters
{

public:
	CDnaHelixParameters(const string& sequence, char type);
	~CDnaHelixParameters();

	float GetDeltaH();
	float GetDeltaS();
	int GetAnnealingTemp();
	int GetMeltingTemp();
	int GetNumberOfBases(char base);
	int GetNumberOfA();
	int GetNumberOfG();
	int GetNumberOfT();
	int GetNumberOfC();
	int GetNumberOfU();
	int GetGplusCPercentage();
	int GetAplusTPercentage();
	int GetMolecularWeightSingle();
	int GetMolecularWeightDouble();
	float GetAbsorbanceMilligramDouble();
	float GetAbsorbanceMolarDouble();
	float GetAbsorbanceMilligramSingle();
	float GetAbsorbanceMolarSingle();
	float GetOligoAbsorbanceMolar();
	inline int GetLength() {
		return length;
	}

protected:
	string sequence;
	int length;
	char type;
	map<char, int> baseCountMap;

	int GetBaseIndex(const string& basepair);	
};

#include "DnaHelixParameters.h"

static float enthalpy[16];
static float entropy[16];
static float helixinitiationS;
static float helixinitiationH;
static float primerconcentration;
static float potassiumconcentration;
static float universalgasconstant;
static map<string, int> baseDoublesMap;

CDnaHelixParameters::CDnaHelixParameters(const string& Sequence, char sequencetype = 'D')
{
	sequence = Sequence;
	length = sequence.length();
	type = sequencetype;
	if (baseDoublesMap.empty()) {
		baseDoublesMap["AA"] = 0;
		baseDoublesMap["TT"] = 1;
		baseDoublesMap["AT"] = 2;
		baseDoublesMap["TA"] = 3;
		baseDoublesMap["CA"] = 4;
		baseDoublesMap["TG"] = 5;
		baseDoublesMap["GT"] = 6;
		baseDoublesMap["AC"] = 7;
		baseDoublesMap["CT"] = 8;
		baseDoublesMap["AG"] = 9;
		baseDoublesMap["GA"] = 10;
		baseDoublesMap["TC"] = 11;
		baseDoublesMap["CG"] = 12;
		baseDoublesMap["GC"] = 13;
		baseDoublesMap["GG"] = 14;
		baseDoublesMap["CC"] = 15;

		enthalpy[0] = 9.1f;
		enthalpy[1] = 9.1f;
		enthalpy[2] = 8.6f;
		enthalpy[3] = 6.0f;
		enthalpy[4] = 5.8f;
		enthalpy[5] = 5.8f;
		enthalpy[6] = 6.5f;
		enthalpy[7] = 6.5f;
		enthalpy[8] = 7.8f;
		enthalpy[9] = 7.8f;
		enthalpy[10] = 5.6f;
		enthalpy[11] = 5.6f;
		enthalpy[12] = 11.9f;
		enthalpy[13] = 11.1f;
		enthalpy[14] = 11.0f;
		enthalpy[15] = 11.0f;

		entropy[0] = 0.024f;
		entropy[1] = 0.024f;
		entropy[2] = 0.0239f;
		entropy[3] = 0.0169f;
		entropy[4] = 0.0129f;
		entropy[5] = 0.0129f;
		entropy[6] = 0.0173f;
		entropy[7] = 0.0173f;
		entropy[8] = 0.0208f;
		entropy[9] = 0.0208f;
		entropy[10] = 0.0135f;
		entropy[11] = 0.0135f;
		entropy[12] = 0.0278f;
		entropy[13] = 0.0267f;
		entropy[14] = 0.0266f;
		entropy[15] = 0.0266f;
		helixinitiationS = 0.0168f;
		helixinitiationH = 0.0f;
		primerconcentration = pow(10.0f, -6);
		potassiumconcentration = 0.05f;
		universalgasconstant = 0.001987f;
	}
}

CDnaHelixParameters::~CDnaHelixParameters()
{
}

float CDnaHelixParameters::GetDeltaH()
{
	float enthalpyParam = 0.0f;
	for(int x = 0; x < length-2; x++) {
		string basepair = sequence.substr(x, 2);
		int index = GetBaseIndex(basepair);
		enthalpyParam += enthalpy[index];
	}
	return enthalpyParam + helixinitiationH;
}

float CDnaHelixParameters::GetDeltaS()
{
	float entropyParam = 0.0f;
	for(int x = 0; x < length-2; x++) {
		string basepair = sequence.substr(x, 2);
		int index = GetBaseIndex(basepair);
		entropyParam += entropy[index];
	}
	return entropyParam + helixinitiationS;
}

int CDnaHelixParameters::GetMeltingTemp()
{
	if(type != 'D')
		return 0;
	float deltaS = GetDeltaS();
	float deltaH = GetDeltaH();
	int tm = (int)(0.5f + (7.209f * log(potassiumconcentration))+ (deltaH/(deltaS - universalgasconstant*log(primerconcentration/4.0f))) - 273.15f);
	return tm;
}

int CDnaHelixParameters::GetAnnealingTemp()
{
	if(type != 'D')
		return 0;
	return GetMeltingTemp() - 15;
}

int CDnaHelixParameters::GetBaseIndex(const string& basepair)
{
	string temp = basepair;
	if (basepair.length() > 2)
		temp = basepair.substr(0, 2);
	transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	return baseDoublesMap[temp];
}

int CDnaHelixParameters::GetNumberOfBases(char base)
{
	if (baseCountMap.empty()) {
		for (int x = 0; x < length; x++) {
			char tmpBase = sequence[x];
			auto got = baseCountMap.find(tmpBase);
			if (got == baseCountMap.end())
				baseCountMap[tmpBase] = 1;
			else
				baseCountMap[tmpBase] = got->second + 1;
		}
	}
	return baseCountMap[base];
}

int CDnaHelixParameters::GetNumberOfA()
{
	return GetNumberOfBases('A');
}

int CDnaHelixParameters::GetNumberOfG()
{
	return GetNumberOfBases('G');
}

int CDnaHelixParameters::GetNumberOfT()
{
	return GetNumberOfBases('T');
}

int CDnaHelixParameters::GetNumberOfC()
{
	return GetNumberOfBases('C');
}

int CDnaHelixParameters::GetNumberOfU()
{
	return GetNumberOfBases('U');
}

int CDnaHelixParameters::GetGplusCPercentage()
{
	int G = GetNumberOfG();
	int C = GetNumberOfC();
	int value = 100*(C+G)/length;
	return value;
}

int CDnaHelixParameters::GetAplusTPercentage()
{
	int value = GetGplusCPercentage();
	return 100 - value;
}

int CDnaHelixParameters::GetMolecularWeightSingle()
{
	return GetMolecularWeightDouble() / 2;
}

int CDnaHelixParameters::GetMolecularWeightDouble()
{
	return 650 * length;
}

float CDnaHelixParameters::GetAbsorbanceMilligramDouble()
{
	if(type != 'D')
		return 0;
	if(length > 100)
		return 20;
	return 20.0f / 27.0f * (float) GetOligoAbsorbanceMolar() / GetMolecularWeightSingle();
}

float CDnaHelixParameters::GetAbsorbanceMolarDouble()
{
	if(type != 'D')
		return 0;
	if(length > 100)
		return 1000000 * 13.2f * length / 1000.0f;
	return GetOligoAbsorbanceMolar() * 20.0f / 27.0f;
}

float CDnaHelixParameters::GetAbsorbanceMilligramSingle()
{
	if(length > 100)
		return (type == 'D') ? 27.0f : 25.0f;
	return GetOligoAbsorbanceMolar() / GetMolecularWeightSingle();
}

float CDnaHelixParameters::GetAbsorbanceMolarSingle()
{
	if(length > 100)
		return 1000000.0f * 10 * length / 1000.0f;
	return GetOligoAbsorbanceMolar();
}

float CDnaHelixParameters::GetOligoAbsorbanceMolar()
{
	int T = (type == 'D') ? GetNumberOfT() : GetNumberOfU();
	int G = GetNumberOfG();
	int A = GetNumberOfA();
	int C = GetNumberOfC();
	float factor = 1.2f * G + 1.5f * A + 0.84f * T + 0.71f * C;
	return 1000000.0f * factor / 100.0f;
}

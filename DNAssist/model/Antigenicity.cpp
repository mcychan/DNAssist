#include "Antigenicity.h"

static const float antigenicitymatrix[21] {
	0.0f, -0.18f, -0.03f, -0.01f, 0.01f, 0.12f, -0.05f, 0.21f, -0.11f, -0.01f, 0.65f,
	0.31f, -0.12f, 0.75f, -0.07f, 0.06f, -0.38f, -2.92f, -0.77f, -0.14f, -0.05f
};
static map<char, int> residueMap;

CAntigenicity::CAntigenicity(const string& seq, size_t windowsize)
{
	sequence = seq;
	m_nWindowsize = windowsize;
	Initialise();
}

CAntigenicity::~CAntigenicity()
{

}

void CAntigenicity::Initialise()
{
	start = 0;
	end = sequence.length() - m_nWindowsize;

	if (residueMap.empty()) {
		residueMap['G'] = 1;
		residueMap['Q'] = 2;
		residueMap['S'] = 3;
		residueMap['Y'] = 4;
		residueMap['A'] = 5;
		residueMap['K'] = 6;
		residueMap['T'] = 7;
		residueMap['W'] = 8;
		residueMap['V'] = 9;
		residueMap['H'] = 10;
		residueMap['D'] = 11;
		residueMap['C'] = 12;
		residueMap['L'] = 13;
		residueMap['R'] = 14;
		residueMap['E'] = 15;
		residueMap['M'] = 16;
		residueMap['I'] = 17;
		residueMap['F'] = 18;
		residueMap['N'] = 19;
		residueMap['P'] = 20;
	}
}

int CAntigenicity::GetIndex(char residue)
{
	auto result = residueMap.find(residue);
	return result == residueMap.end() ? 0 : result->second;	
}

float CAntigenicity::GetAntigenicityValue(int index)
{
	return antigenicitymatrix[index];
}

float CAntigenicity::CalculateAntigenicityAtWindow(int windowstart)
{
	float value = 0.0f;
	for(int x = windowstart; x < windowstart + m_nWindowsize; x++)
		value += GetAntigenicityValue(GetIndex(sequence[x]));
	return value;
}

bool CAntigenicity::CalculateAntigenicity(vector<float>& array)
{
	array.clear();
	int count = end - start + 1;
	array.resize(count);
#pragma omp parallel for
	for(int x = start; x <= end; x++) {
		int i = x - start;
		m_nPercent = 100 * i / count;
		array[i] = CalculateAntigenicityAtWindow(x);
	}
	return true;
}

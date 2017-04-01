#include "Hydrophilicity.h"

static const float hydrophilicitymatrix[21] = {
		0.0f, 0.0f, 0.2f, 0.3f, -2.3f, -0.5f, 3.0f, -0.4f, -3.4f, -1.5f, -0.5f,
		2.5f, -1.0f, -1.8f, 3.0f, 2.5f, -1.3f, -1.8f, -2.5f, 0.2f, -1.2f
};
static map<char, short> residueIndexMap;

CHydrophilicity::CHydrophilicity(const string& seq, size_t windowsize)
{
	sequence = seq;
	m_nWindowsize = windowsize;
	Initialise();
}

CHydrophilicity::~CHydrophilicity()
{
}

void CHydrophilicity::Initialise()
{
	start = 0;
	end = sequence.length() - m_nWindowsize;
	if(residueIndexMap.empty()) {
		residueIndexMap['G'] = 1;
		residueIndexMap['Q'] = 2;
		residueIndexMap['S'] = 3;
		residueIndexMap['Y'] = 4;
		residueIndexMap['A'] = 5;
		residueIndexMap['K'] = 6;
		residueIndexMap['T'] = 7;
		residueIndexMap['W'] = 8;
		residueIndexMap['V'] = 9;
		residueIndexMap['H'] = 10;
		residueIndexMap['D'] = 11;
		residueIndexMap['C'] = 12;
		residueIndexMap['L'] = 13;
		residueIndexMap['R'] = 14;
		residueIndexMap['E'] = 15;
		residueIndexMap['M'] = 16;
		residueIndexMap['I'] = 17;
		residueIndexMap['F'] = 18;
		residueIndexMap['N'] = 19;
		residueIndexMap['P'] = 20;
	}
}

int CHydrophilicity::GetIndex(char residue)
{
	auto got = residueIndexMap.find(residue);
	return (got == residueIndexMap.end()) ? 0 : got->second;
}

float CHydrophilicity::GetHydrophilicityValue(int index)
{
	return hydrophilicitymatrix[index];
}

float CHydrophilicity::CalculateHydrophilicityAtWindow(int windowstart)
{
	float value = 0.0f;
	for(int x = windowstart; x < windowstart + m_nWindowsize; x++)
		value += GetHydrophilicityValue(GetIndex(sequence[x]));
	return value;
}

bool CHydrophilicity::CalculateHydrophilicity(vector<float>& array)
{
	array.clear();
	int count = end - start + 1;
	array.resize(count);
#pragma omp parallel for
	for(int x = start; x <= end; x ++) {
		int i = x - start;
		m_nPercent = 100 * i / count;
		array[i] = CalculateHydrophilicityAtWindow(x);
	}
	return true;
}

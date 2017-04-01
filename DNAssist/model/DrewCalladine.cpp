#include "DrewCalladine.h"

#include <algorithm>

static const short testwindowsize = 125; //size of test window in base-pairs
static const short testwindowBound = testwindowsize - 1;
static const float unitchange = 360 / 10.2f;
static const float phasearray[12][13] = {
	{ 1.48f,1.21f,0.96f,1.03f,1.05f,0.68f,0.65f,0.68f,1.05f,1.03f,0.96f,1.21f,1.48f },
	{ 1.23f,1.06f,0.95f,1.07f,1.02f,0.84f,0.88f,0.84f,1.02f,1.07f,0.95f,1.06f,1.23f },
	{ 0.84f,0.89f,0.83f,0.96f,1.14f,1.23f,1.05f,1.23f,1.14f,0.96f,0.83f,0.89f,0.84f },
	{ 1.32f,1.16f,0.98f,1.05f,0.91f,0.83f,0.84f,0.83f,0.91f,1.05f,0.98f,1.16f,1.32f },
	{ 1.04f,1.07f,1.15f,0.94f,0.92f,0.96f,0.88f,0.96f,0.92f,0.94f,1.15f,1.07f,1.04f },
	{ 1.04f,1.02f,1.01f,0.98f,0.99f,1.02f,0.91f,1.02f,0.99f,0.98f,1.01f,1.02f,1.04f },
	{ 0.66f,0.69f,0.86f,1.03f,1.15f,1.29f,1.30f,1.29f,1.15f,1.03f,0.86f,0.69f,0.66f },
	{ 0.95f,0.93f,0.92f,0.96f,0.97f,1.09f,1.34f,1.09f,0.97f,0.96f,0.92f,0.93f,0.95f },
	{ 0.91f,1.10f,1.14f,0.96f,0.96f,0.90f,0.98f,0.90f,0.96f,0.96f,1.14f,1.10f,0.91f },
	{ 1.07f,1.07f,1.22f,0.75f,0.96f,1.08f,0.76f,1.08f,0.96f,0.75f,1.22f,1.07f,1.07f },
	{ 0.96f,1.33f,1.16f,1.02f,0.81f,0.82f,0.77f,0.82f,0.81f,1.02f,1.16f,1.33f,0.96f },
	{ 1.28f,1.48f,1.28f,1.07f,0.66f,0.50f,0.73f,0.50f,0.66f,1.07f,1.28f,1.48f,1.28f }
};
static short angleIndex[testwindowBound] = { 0 };
static map<string, short> baseStepIndexMap;

CDrewCalladine::CDrewCalladine(const string& inputsequence, vector<float>* resultArray, int* pPercentCompleted, int origin)
{
	m_pPercentCompleted = pPercentCompleted;
	startbase = origin;
	sequence = inputsequence;
	length = inputsequence.length();
	positioningsignal = resultArray;

	if(baseStepIndexMap.empty()) {		
		baseStepIndexMap["CG"] = 0;
		baseStepIndexMap["TG"] = 1;
		baseStepIndexMap["CA"] = 1;
		baseStepIndexMap["TA"] = 2;
		baseStepIndexMap["GG"] = 3;
		baseStepIndexMap["CC"] = 3;
		baseStepIndexMap["AG"] = 4;
		baseStepIndexMap["CT"] = 4;
		baseStepIndexMap["GA"] = 5;
		baseStepIndexMap["TC"] = 5;
		baseStepIndexMap["AA"] = 6;
		baseStepIndexMap["TT"] = 6;
		baseStepIndexMap["AT"] = 7;
		baseStepIndexMap["GT"] = 8;
		baseStepIndexMap["AC"] = 8;
		baseStepIndexMap["GC"] = 9;
		baseStepIndexMap["TGC"] = 9;
		baseStepIndexMap["CGC"] = 9;
		baseStepIndexMap["AGC"] = 10;
		baseStepIndexMap["GGC"] = 11;

		CalculateWindowAngles();
	}
}

short CDrewCalladine::GetBaseStepIndex(const string& data)
{
	auto got = baseStepIndexMap.find(data);
	return (got == baseStepIndexMap.end()) ? -1 : got->second;
}

short CDrewCalladine::AngleToIndex(short angle)
{
	if(angle < 0 || angle > 360)
		return -1;

	short index = (angle - 15) / 30;
	return (index < 0 || index > 11) ? 0 : index;
}

void CDrewCalladine::CalculateWindowAngles()
{
#pragma omp parallel for
	for(int x = 0; x < testwindowBound; x++) {
		int y = (x >= 62) ? x : testwindowBound - 1 - x;
		short angle = ((int)((unitchange / 2) + (y - 62) * unitchange)) % 360;

		if(x >= 52 && x <= 61)
			angle = (angle + 145) % 360;
		else if(x >= 62 && x <= 71)
			angle = abs(angle - 145) % 360;

		angleIndex[x] = AngleToIndex(angle);
	}
}

bool CDrewCalladine::CalculatePositioningSignal()
{
	int max = (int) length - testwindowsize;
	positioningsignal->resize(max + 1);
#pragma omp parallel for
	for(int x = 0; x <= max; x++) {
		*m_pPercentCompleted = 100 * x / max;
		float result = 0.0f;
		for(int y = 0; y < testwindowBound; y++) {
			string basestep = sequence.substr(x + y, 2);

			if(basestep == "GC") {
				if(y != 0)
					basestep = sequence.substr(x + y - 1, 3);
			}

			short angleindex = angleIndex[y];
			int baseindex = GetBaseStepIndex(basestep);
			float rawvalue = phasearray[baseindex][angleindex];
			result += (0.616f * log(rawvalue));
		}
		positioningsignal->at(x) = result;
	}
	*m_pPercentCompleted = 100;
	return true;
}

float CDrewCalladine::GetMaximum()
{
	return *max_element(positioningsignal->begin(), positioningsignal->end());
}

float CDrewCalladine::GetMinimum()
{
	return *min_element(positioningsignal->begin(), positioningsignal->end());
}

float CDrewCalladine::GetRange()
{
	return GetMaximum() - GetMinimum();
}

float CDrewCalladine::GetPositioningValue(int index)
{
	if(index < positioningsignal->size())
		return positioningsignal->at(index);
	return 0.0f;
}

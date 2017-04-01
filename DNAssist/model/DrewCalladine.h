#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

class CDrewCalladine
{
public:
	CDrewCalladine(const string& testsequence, vector<float>* resultArray, int* pPercentCompleted, int firstbasenumber = 0);
	short GetBaseStepIndex(const string& data);
	short AngleToIndex(short angle);
	void CalculateWindowAngles();
	bool CalculatePositioningSignal();

	float GetMaximum();
	float GetMinimum();
	float GetRange();
	float GetPositioningValue(int index);

	inline size_t GetSequenceLength() const {
		return length;
	};
	inline int GetSequenceStart() const {
		return startbase;
	};
	inline int GetDyadStart() const {
		return startbase + 63;
	};// start location of dyad
	inline size_t GetDyadEnd() const {
		return length - 62;
	};//end location of dyad
	inline int GetNumberOfValues() const {
		return (int)length - 124;
	};

protected:
	int startbase, endbase;
	int* m_pPercentCompleted;
	size_t length;
	string sequence;
	vector<float>* positioningsignal;

};

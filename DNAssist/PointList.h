#pragma once
#include "model/IntList.h"

#include <iterator>
#include <vector>

using namespace std;

class CPointList
{
public:
	CPointList();
	~CPointList();

	size_t GetXValue(int index);
	size_t Get_YValue(int index);//strange, compiles does not discriminate between GetXValue and GetYValue, hence _YValue
	void AddPoint(int x, int y);
	void AddPoint(POINT* point);
	void AddPoint(POINT point);
	inline size_t getMembers() const
	{
		return x.GetArrayLength();
	}
	POINT GetPoint(int index);
	int GetCounter();
	void IncrementCounter();
	void OverWritePoint(POINT point, int index);
	void AddConstant(int number);

protected:
	int counter = 0;
	CIntList x;
	CIntList y;

private:	

};

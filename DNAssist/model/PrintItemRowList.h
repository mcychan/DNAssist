#pragma once
#include "PrintItemRow.h"

#include <vector>
using namespace std;

class CPrintItemRowList
{
public:
	CPrintItemRowList();
	~CPrintItemRowList();

	CPrintItemRow* GetPrintItemRow(int index);
	void AddPrintItemRow(const CPrintItemRow& row);
	void AddArray(CPrintItemRowList* pList);
	int GetStringLengthThatWillHoldAllItems();
	inline size_t size() const {
		return array.size();
	}
	inline void clear() {
		array.clear();
	}

protected:
	vector<CPrintItemRow> array;
};

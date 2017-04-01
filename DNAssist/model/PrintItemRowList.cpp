#include "PrintItemRowList.h"

CPrintItemRowList::CPrintItemRowList()
{
}

CPrintItemRowList::~CPrintItemRowList()
{
}

void CPrintItemRowList::AddPrintItemRow(const CPrintItemRow& row)
{
	array.emplace_back(row);
}

CPrintItemRow* CPrintItemRowList::GetPrintItemRow(int index)
{
	if(index < array.size())
		return &array[index];
	return nullptr;
}

int CPrintItemRowList::GetStringLengthThatWillHoldAllItems()
{
	int requiredLength = 0;
	for (int x = 0; x < array.size(); x++) {
		auto printItemRow = GetPrintItemRow(x);
		int y = printItemRow->size() - 1;
		int length = printItemRow->GetCoordinate(y) + printItemRow->GetStringLength(y);
		requiredLength = max(length, requiredLength);
	}
	return requiredLength;
}

void CPrintItemRowList::AddArray(CPrintItemRowList* pList)
{
	array.insert(array.end(), pList->array.cbegin(), pList->array.cend());
}


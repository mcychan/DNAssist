#include "PrintItemRow.h"

CPrintItemRow::CPrintItemRow()
{
}

CPrintItemRow::CPrintItemRow(const string& data, int coord, int itemdata)
{
	AddItem(data, coord, itemdata);
}

CPrintItemRow::CPrintItemRow(const CPrintItemRow& passedobject)
{
	items = passedobject.items;
}

CPrintItemRow::~CPrintItemRow()
{
}

void CPrintItemRow::AddItem(const string& data, int coord, bool itemdata)
{
	items.emplace_back(PrintItem{data, coord, itemdata});
}

void CPrintItemRow::AddItem(const string& data, int stringlength, int coord, bool itemdata)
{
	if(stringlength < data.length())
		AddItem(data.substr(0, stringlength), coord, itemdata);
	else
		AddItem(data, coord, itemdata);
}

string CPrintItemRow::GetString(int index) const
{
	if(index < items.size())
		return items[index].Data;
	return "";
}

int CPrintItemRow::GetCoordinate(int index) const
{
	if(index < items.size())
		return items[index].Coord;
	return -1;
}

int CPrintItemRow::GetItemData(int index) const
{
	if(index < items.size())
			return items[index].Itemdata;
	return -1;
}

size_t CPrintItemRow::GetStringLength(int index) const
{
	return GetString(index).length();
}

//	This function tests whether a string will fit at coordinate 'place' (numbered relative to
//	the beginning of the entire sequence analysed [may be a selection]) with the current row
//	being tested
bool CPrintItemRow::WillFit(int place)
{
	if(items.empty())
		return true;
	const int LAST = items.size() - 1;
	return GetCoordinate(LAST) + GetStringLength(LAST)+1 <= place;
}

void CPrintItemRow::SortAndRemoveDuplicates()
{
	sort(items.begin(), items.end(),
	          [](const PrintItem& lhs, const PrintItem & rhs)
	{
	  return lhs.Coord < rhs.Coord;
	}
	);
	items.erase(unique(items.begin(), items.end()), items.end());
}

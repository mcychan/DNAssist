#pragma once

#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class CPrintItemRow
{
public:
	CPrintItemRow();
	CPrintItemRow(const string& data, int coord, int itemdata);
	CPrintItemRow(const CPrintItemRow&);
	~CPrintItemRow();
	void AddItem(const string& data, int coord, bool itemdata);
	void AddItem(const string& data, int stringlength, int coord, bool itemdata);
	string GetString(int index) const;
	size_t GetStringLength(int index) const;
	int GetCoordinate(int index) const;
	int GetItemData(int index) const;
	bool WillFit(int place);
	void SortAndRemoveDuplicates();
	inline size_t size() const {
		return items.size();
	}

protected:
	struct PrintItem {
		string Data;
		int Coord;
		bool Itemdata;

		PrintItem(string data, int coord, bool itemdata) : Data(data), Coord(coord), Itemdata(itemdata) {
		}

		bool PrintItem::operator==(const PrintItem& rhs) const
		{
			if (Coord != rhs.Coord)
				return false;
			return true;
		}
	};
	vector<PrintItem> items;
};

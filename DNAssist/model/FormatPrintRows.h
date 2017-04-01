#pragma once
#include "PrintItemRow.h"
#include "PrintItemRowList.h"

#include <string>
#include <vector>
using namespace std;

struct blockstructure
{
	int endblock = 0, fullblock = 0, gapsize = 0, startblock = 0;
	vector<int> array;
	inline bool empty() {
		return array.empty();
	}
	inline size_t size() {
		return array.size();
	}
};

class CFormatPrintRows
{
public:
	CFormatPrintRows(const blockstructure& block, int length_of_print_row);
	~CFormatPrintRows();
	void AddSite(const string& name, int position, int start, bool data);
	string GetNameRow(int row_number, string& data);
	string GetPositionRow(int row_number, string& data);
	CPrintItemRow* GetNameRow(int row_number);
	CPrintItemRow* GetPositionRow(int row_number);
	string GetNameTickRow(string& data);
	CPrintItemRow* GetNameTickRow(CPrintItemRow& itemRow);
	string GetPositionTickRow(string& data);
	CPrintItemRow* GetPositionTickRow(CPrintItemRow& itemRow);
	CPrintItemRowList* GetNumbersRows(CPrintItemRowList& rowarray, int start, int length);
	inline size_t GetNumberNameRows() const {
		return arrayofnames.size();
	};
	inline size_t GetNumberPositionRows() const {
		return arrayofpositions.size();
	};

protected:
	int rowlength;
	int spacebetweenwords;
	CPrintItemRowList arrayofnames;
	CPrintItemRowList arrayofpositions;
	blockstructure blockspacing;

	int InsertGaps(int pos, int start);
	int AdjustPositionIfOverhangs(int position, int stringlength, int blocklength);
};

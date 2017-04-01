#include "FormatPrintRows.h"

CFormatPrintRows::CFormatPrintRows(const blockstructure& blocks, int length_of_print_row)
{
	spacebetweenwords = 1;//Leave 1 space between words when trying to fit
	blockspacing = blocks;
	rowlength = length_of_print_row;//This includes additional spaces to the right, if the sequencenumber goes there
}

CFormatPrintRows::~CFormatPrintRows()
{
}

void CFormatPrintRows::AddSite(const string& name, int position, int start, bool data = false)
{	
	if(arrayofnames.size() == 0) {
		CPrintItemRow itemRow;
		itemRow.AddItem(name, InsertGaps(position, start), data);
		arrayofnames.AddPrintItemRow(move(itemRow));
	}
	else {
		int iteration = 0;
		while(iteration < arrayofnames.size() && !arrayofnames.GetPrintItemRow(iteration)->WillFit(InsertGaps(position, start)))
			iteration++;
		if(iteration == arrayofnames.size()) {
			CPrintItemRow itemRow;
			itemRow.AddItem(name, InsertGaps(position, start), data);
			arrayofnames.AddPrintItemRow(move(itemRow));
		}
		else
			arrayofnames.GetPrintItemRow(iteration)->AddItem(name, InsertGaps(position, start), data);
	}

	int length_of_position_string = 1; // if position = 0 log10 is undefined; take care of it here
	if (position)
		length_of_position_string = (int)(log10(position + 0.0f) + 1);
	string posname = to_string(position);

	if(arrayofpositions.size() == 0) {
		CPrintItemRow itemRow;
		itemRow.AddItem(posname, InsertGaps(position, start), data);
		arrayofpositions.AddPrintItemRow(itemRow);
	}
	else {
		int iteration = 0;
		while(iteration < arrayofpositions.size() && !arrayofpositions.GetPrintItemRow(iteration)->WillFit(InsertGaps(position, start)))
			iteration++;
		if(iteration == arrayofpositions.size()) {
			CPrintItemRow itemRow;
			itemRow.AddItem(posname, InsertGaps(position, start), data);
			arrayofpositions.AddPrintItemRow(itemRow);
		}
		else
			arrayofpositions.GetPrintItemRow(iteration)->AddItem(posname, InsertGaps(position, start), 0);
	}

}

string CFormatPrintRows::GetNameRow(int row_number, string& data)
{
	auto pir = arrayofnames.GetPrintItemRow(row_number);
	int index = pir->size() - 1;
	int stringlength = pir->GetStringLength(index) + pir->GetCoordinate(index);
	data.clear();
	data.resize(stringlength);
	for(int x = 0; x <= index; x ++) {		
		int stringlength = pir->GetStringLength(x);
		int stringposition = pir->GetCoordinate(x);
		string namelabel = pir->GetString(x);
		for(int y = 0; y < stringlength; y++)
			data[stringposition + y] = namelabel[y];
	}
	return data;
}

CPrintItemRow* CFormatPrintRows::GetNameRow(int row_number)
{
	if (row_number < arrayofnames.size())
		return arrayofnames.GetPrintItemRow(row_number);
	return 0;
}

string CFormatPrintRows::GetPositionRow(int row_number, string& data)
{
	auto pir = arrayofpositions.GetPrintItemRow(row_number);
	int index = pir->size() - 1;
	int stringlength = pir->GetStringLength(index) + pir->GetCoordinate(index);
	data.clear();
	data.resize(stringlength);
	for(int x = 0; x <= index; x ++) {
		int stringlength = pir->GetStringLength(x);
		int stringposition = pir->GetCoordinate(x);
		string namelabel = pir->GetString(x);
		for(int y = 0; y < stringlength; y++)
			data[stringposition+y] = namelabel[y];
	}
	return data;
}

CPrintItemRow* CFormatPrintRows::GetPositionRow(int row_number)
{
	if (row_number < arrayofpositions.size())
		return arrayofpositions.GetPrintItemRow(row_number);
	return 0;
}

CPrintItemRow* CFormatPrintRows::GetNameTickRow(CPrintItemRow& itemRow)
{
	for(int a = 0; a < arrayofnames.size(); a++) {
		for(int x = 0; x < arrayofnames.GetPrintItemRow(a)->size(); x ++)
			itemRow.AddItem("|", arrayofnames.GetPrintItemRow(a)->GetCoordinate(x), 0);
	}
	if(itemRow.size() > 0)
		itemRow.SortAndRemoveDuplicates();
	return &itemRow;
}

string CFormatPrintRows::GetNameTickRow(string& data)
{
	size_t stringlength = arrayofnames.GetStringLengthThatWillHoldAllItems();
	data.clear();
	data.resize(stringlength);
	for(int a = 0; a < arrayofnames.size(); a++) {
		auto pir = arrayofpositions.GetPrintItemRow(a);
		for(int x = 0; x < pir->size(); x ++) {
			int tickposition = pir->GetCoordinate(x);
			data[tickposition] = '|';
		}
	}
	return data;
}

CPrintItemRow* CFormatPrintRows::GetPositionTickRow(CPrintItemRow& itemRow)
{
	for(int a = 0; a < arrayofpositions.size(); a++) {
		auto pir = arrayofpositions.GetPrintItemRow(a);
		for(int x = 0; x < pir->size(); x++)
			itemRow.AddItem("|", pir->GetCoordinate(x), 0);
	}
	if(itemRow.size() > 0)
		itemRow.SortAndRemoveDuplicates();
	return &itemRow;
}

string CFormatPrintRows::GetPositionTickRow(string& data)
{
	size_t stringlength = arrayofpositions.GetStringLengthThatWillHoldAllItems();
	data.resize(rowlength);
	for(size_t a = 0; a < arrayofpositions.size(); a++) {
		auto pir = arrayofpositions.GetPrintItemRow(a);
		for(size_t x = 0; x < pir->size(); x++) {
			int tickposition = pir->GetCoordinate(x);
			data[tickposition] = '|';
		}
	}
	return data;
}

//	pos is the sequenceposition relative to the start of the sequence (which may be
//	outside a selection, if any), start is the true sequencenumber
// of the first nucleotide in the selection (if any) or sequence.

int CFormatPrintRows::InsertGaps(int pos, int start)
{
	if(blockspacing.empty())
		return pos;

	int x = 0;
	while(x < blockspacing.size() && (pos - start) > blockspacing.array[x])
		x++;
	return (pos - start) + x * blockspacing.gapsize;
}

CPrintItemRowList* CFormatPrintRows::GetNumbersRows(CPrintItemRowList& rowarray, int start, int length)
{
	bool lastnumber = false;
	int coordinate = 0;
	string numberstring;
	rowarray.clear();

	//	Do the startblock
	int number = blockspacing.array[0] + start;
	numberstring = to_string(number);
	if(number < length)
		coordinate = InsertGaps(number, start) - (numberstring.length() - 1);
	else if(number == length) {
		lastnumber = true;
		coordinate = InsertGaps(number, start) - (numberstring.length() - 1);
	}
	else if(number > length && !lastnumber) {
		lastnumber = true;
		number = length;
		coordinate = InsertGaps(length, start) - (numberstring.length() - 1);
	}

	CPrintItemRow itemRow;
	itemRow.AddItem(numberstring, coordinate, 0);
	rowarray.AddPrintItemRow(move(itemRow));

	//	Do the rest of the blocks, if any
	for(int x = 1; x < blockspacing.size() && ((blockspacing.array[x] + start) <= length); x++) {
		number = blockspacing.array[x] + start;
		numberstring = to_string(number);
		coordinate = InsertGaps(number, start) - (numberstring.length() - 1);
		int iteration = 0;
		while(iteration < rowarray.size() && !rowarray.GetPrintItemRow(iteration)->WillFit(coordinate))
			iteration++;
		if(iteration == rowarray.size()) {
			CPrintItemRow itemRow;
			itemRow.AddItem(numberstring, coordinate, 0);
			rowarray.AddPrintItemRow(move(itemRow));
		}
		else
			rowarray.GetPrintItemRow(iteration)->AddItem(numberstring, coordinate, 0);
	}

	// Do the last number in each row
	int temp = (start+rowlength-1 > length ? length : start+rowlength-1);
	if(number != temp) {
		number = temp;
		numberstring = to_string(number);
		coordinate = InsertGaps(number, start) - (numberstring.length() - 1);
		int iteration = 0;
		while(iteration < rowarray.size() && !rowarray.GetPrintItemRow(iteration)->WillFit(coordinate))
			iteration++;
		if(iteration == rowarray.size()) {
			CPrintItemRow itemRow;
			itemRow.AddItem(numberstring, coordinate, 0);
			rowarray.AddPrintItemRow(move(itemRow));
		}
		else
			rowarray.GetPrintItemRow(iteration)->AddItem(numberstring, coordinate, 0);
	}
	return &rowarray;
}

int CFormatPrintRows::AdjustPositionIfOverhangs(int position, int stringlength, int blocklength)
{
	position -= (stringlength - 1);
	if(stringlength > blocklength)
		return position+ (stringlength - blocklength);
	return position;
}


#include "AnalyzeDNAList.h"

CAnalyzeDNAList::CAnalyzeDNAList(const string& pointer_to_sequence,
											  int seq_start,
                                   int seq_length,
								   CRestrictionCutList* result,
								PrintFormatTransferBuffer* printFormatTransferBuffer,
								   int font_cx, int font_cy,
                                   int numberofcharactersperhorizontalrow,
                                   int LinesPerPage,
                                   vector<int> PageBreaks,
                                   char seq_type,
                                   CCodonTable* codon
                                   )
{
	sequence = pointer_to_sequence; //start of the sequence to list
	sequence_length = seq_length; //length of the sequence to list
	sequence_start = seq_start;
	row_length = 0;
	number_rows = 0; //number of rows in the row 2D array
	resultarray = result;
	printrow = 0;
	rowcell_cx = font_cx;
	rowcell_cy = font_cy;
	numcharshorz = numberofcharactersperhorizontalrow;
	codontable = codon;
	sequence_type = seq_type;
	linesperpage = LinesPerPage;
	pagebreaks = PageBreaks;

	//	Get all the format options selected by the user
	transferbuffer = printFormatTransferBuffer;

	//	Get all the format options selected by the user

	if (transferbuffer->numbersabovesequence)
		sequencenumberposition = NUMBER_ABOVE;
	else if (transferbuffer->numbersbelowsequence)
		sequencenumberposition = NUMBER_BELOW;
	else if (transferbuffer->numbersrightsequence)
		sequencenumberposition = NUMBER_RIGHT;
	if (transferbuffer->displaywatsonandcrick)
		bothstrands = true;
	else
		bothstrands = false;
	if (transferbuffer->displaysequenceblocks)
		sequenceinblocks = true;
	else
		sequenceinblocks = false;
}

CAnalyzeDNAList::~CAnalyzeDNAList()
{

}

CPrintItemRowList* CAnalyzeDNAList::GetSitesInList(CPrintItemRowList& rowarray)
{
	SetBlockDimensions();
	row_length = RowLength();
	number_rows = NumberOfSequenceRows();

	//	Fill the rows
	int remain = sequence_length;
	int loop = 0;
	blockstructure block;
	MakeBlockGapStructure(block);

	//	The number of rows in a sequence block (restriction enzymes plus sequence plus
	//	cut sites) are calculated.  If, when added to the current number of rows on
	//	the page, it spills over the linesperpage, the row before the current
	//	sequence block is set as the last line on the page, and the sequence block is
	//	started on the next page.  This is only relevant to the printing.
	int rows_on_this_page = 2;
	int block_end;
	int number_of_lines_on_previous_page;

	for(int x = 0; x < number_rows; x++) {
		int block_start = rowarray.size();

		// Determine where gaps in the sequence will be, and fill the blockstructure

		printrow = make_unique<CFormatPrintRows>(block, row_length);
		if(resultarray) {	//	Test to see if this list has any enzymes or is just straight list
			if(resultarray->GetTotalCutPos()) {
				while(resultarray->GetSequentialCutPos(resultarray->GetSequentialPointer(loop)) <= (x+1) * row_length && loop < resultarray->GetTotalCutPos()) {
					//	Get the sequential name, position and give it to a CPrintItemRowList via
					//	the CFormatPrintRows object.  The latter will take care of shifting strings up
					//	a line to fit nicely.  The position that is passed is adjusted for the start of
					// the sequence in case of a selection being analysed.  The sequence start is
					// numbered from one for the window, therefore subtract one.  The start of each row
					//	is also incremented with sequence start, so that the names are properly aligned
					//	with the sequence in the printout.

					int sortpointer = resultarray->GetSequentialPointer(loop++);
					printrow->AddSite(resultarray->GetSequentialName(sortpointer),
									resultarray->GetSequentialCutPos(sortpointer) + sequence_start - 1,
									x * row_length + sequence_start,
      	                  resultarray->GetSequentialData(sortpointer));
				}
			}
		}

		for(int a = printrow->GetNumberNameRows()-1; a >= 0;  a--) {
			CPrintItemRow* printItemRow = printrow->GetNameRow(a);
			rowarray.AddPrintItemRow(*printItemRow);
		}

		//	Insert the name ticks
		CPrintItemRow printitems;
		printrow->GetNameTickRow(printitems);
		if(printitems.size() > 0)
			rowarray.AddPrintItemRow(printitems);

		// If the numbers go above the sequence, do it here
		if(sequencenumberposition == NUMBER_ABOVE) {
			CPrintItemRowList temparray;
			printrow->GetNumbersRows(temparray, x * row_length + sequence_start, sequence_length + sequence_start-1);
			for(int d = temparray.size()-1; d >= 0; d--) {
				CPrintItemRow* printItemRow = temparray.GetPrintItemRow(d);
				rowarray.AddPrintItemRow(*printItemRow);
			}
		}

		//	Make a copy of the sequence 'string' to be printed, insert gaps, add as
		//	CPrintItemRow to CPrintItemRowList passed as rowarray

		int seqlengthleft = min(row_length, remain);
		string tempstring = sequence.substr(x * row_length, seqlengthleft);
		InsertGapSpaces(&rowarray, tempstring, sequence_start + (x * row_length), (bool)(sequencenumberposition == NUMBER_RIGHT));

		//	Do we need to show both Watson and Crick strands? If yes, do it.
		//	Make a copy of the complementary sequence 'string' to be printed, insert
		//gaps, add as CPrintItemRow to CPrintItemRowList passed as rowarray

		if(bothstrands && (sequence_type == 'D' || sequence_type == 'G')) {
			string convertstring;//temp string for ConvertSequence
			convertobject.SequenceComplement(sequence.substr(x * row_length, min(remain, row_length)), convertstring, sequence_type);
			InsertGapSpaces(&rowarray, convertstring, sequence_start + (x * row_length), false);
		}

		// If the numbers go below the sequence, do it here
		if(sequencenumberposition == NUMBER_BELOW) {
			CPrintItemRowList temparray;
			printrow->GetNumbersRows(temparray, x * row_length + sequence_start, sequence_length + sequence_start - 1);
			for(int d = 0; d < temparray.size(); d++) {
				CPrintItemRow* printItemRow = temparray.GetPrintItemRow(d);
				rowarray.AddPrintItemRow(*printItemRow);
			}
		}

		// Add positiontickrows
		CPrintItemRow printitems2;
		printrow->GetPositionTickRow(printitems2);
		if(printitems2.size() > 0)
			rowarray.AddPrintItemRow(printitems2);

		// Add position strings
		for(int b = 0; b < printrow->GetNumberPositionRows(); b++) {
			CPrintItemRow* printItemRow = printrow->GetPositionRow(b);
			rowarray.AddPrintItemRow(*printItemRow);
		}
		remain = remain - row_length;

		// Add blank line to make outlay tidier
		CPrintItemRow blank("", 0, 0);
		rowarray.AddPrintItemRow(blank);

		//	Do the pagination stuff for printing
		block_end = rowarray.size();
		if(rows_on_this_page + block_end - block_start < linesperpage)
			rows_on_this_page += block_end - block_start;
		else {
			number_of_lines_on_previous_page = 0;
			if(pagebreaks.size() > 0)
				number_of_lines_on_previous_page = pagebreaks[pagebreaks.size() - 1];
			pagebreaks.push_back(number_of_lines_on_previous_page + rows_on_this_page);
			rows_on_this_page = block_end - block_start;
		}
	}

	//	Write out the remainder of this section, if any, on the current page
	number_of_lines_on_previous_page = 0;
	if(pagebreaks.size() > 0)
		number_of_lines_on_previous_page = pagebreaks[pagebreaks.size() - 1];
	if(rows_on_this_page)
		pagebreaks.push_back(number_of_lines_on_previous_page + rows_on_this_page);

	return &rowarray;
}

CPrintItemRowList* CAnalyzeDNAList::TranslateNucleicAcid(CPrintItemRowList& rowarray, int phase)
{
	if(!codontable) //	If there is not a valid codon table, return 0
		return 0;

	SetBlockDimensions();
	row_length = RowLength();
	number_rows = NumberOfSequenceRows();

	//	Fill the rows
	int remain = sequence_length;
	blockstructure block;
	MakeBlockGapStructure(block);
	auto proteinphase = make_unique<string[]>(phase);
	auto leftofphase = make_unique<int[]>(phase);
	auto proteinleft = make_unique<int[]>(phase);
	string reversecomplementsequence;

	if(phase > 3) //	If we are going to do the reverse, make the string here
		convertobject.SequenceReverseComplement(sequence, reversecomplementsequence, sequence_type);

	for(int a = 0; a < phase; a++) {
		if(a < 3) {
			proteinphase[a] = "";
			convertobject.ConvertSequence(sequence.substr(a), proteinphase[a], sequence_type, 'P', codontable, false);
			InsertSpacesInProteinSequence(proteinphase[a], sequence_length, a, 2);
			leftofphase[a] = proteinphase[a].length();
		}
		else { //	Do the reverse string
			proteinphase[a] = "";
			convertobject.ConvertSequence(reversecomplementsequence.substr(a-3), proteinphase[a], sequence_type, 'P', codontable, false);
			InsertSpacesInProteinSequence(proteinphase[a], sequence_length, a, 2);
			string tmpString;
			proteinphase[a] = convertobject.SequenceReverse(proteinphase[a], tmpString);
			leftofphase[a] = proteinphase[a].length();
		}
	}

	//	Set up the parameters used to do pagination for printing
	int rows_on_this_page = 0;
	int block_end;
	int number_of_lines_on_previous_page;

	for(int x = 0; x < number_rows; x++) {
		int block_start = rowarray.size();
		// Write one or three phases of the protein sequence
		for(int b = (phase >= 3 ? 2 : 0); b >= 0; b--) {
			proteinleft[b] = (leftofphase[b] > row_length ? row_length : leftofphase[b]);
			if(proteinleft[b] > 0) {
				string tempstring = proteinphase[b].substr(x * row_length, proteinleft[b]);
				InsertGapSpaces(&rowarray, tempstring, sequence_start + (x * row_length), false);
			}
		}

		// Determine where gaps in the sequence will be, and fill the blockstructure
		printrow = make_unique<CFormatPrintRows>(block, row_length);

		// If the numbers go above the sequence, do it here
		if(sequencenumberposition == NUMBER_ABOVE) {
			CPrintItemRowList temparray;
			printrow->GetNumbersRows(temparray, x * row_length  +sequence_start, sequence_length + sequence_start - 1);
			for(int d = temparray.size()-1; d >= 0; d--) {
				auto printItemRow = temparray.GetPrintItemRow(d);
				rowarray.AddPrintItemRow(*printItemRow);
			}
		}

		// Make a copy of the sequence 'string' to be printed, insert gaps, add as
		// CPrintItemRow to CPrintItemRowList passed as rowarray
		int seqlengthleft = (remain > row_length ? row_length : remain);
		string tempstring = sequence.substr(x * row_length, seqlengthleft);
		InsertGapSpaces(&rowarray, tempstring, sequence_start + (x * row_length), (bool)(sequencenumberposition == NUMBER_RIGHT));

		//	Do we need to show both Watson and Crick strands? If yes, do it.
		//	Make a copy of the complementary sequence 'string' to be printed, insert
		//gaps, add as CPrintItemRow to CPrintItemRowList passed as rowarray

		if(bothstrands) {
			string convertstring;//temp string for ConvertSequence
			convertobject.SequenceComplement(sequence.substr(x * row_length, min(remain, row_length)), convertstring, 'D');   //*************change 'D'
			InsertGapSpaces(&rowarray, convertstring, sequence_start+(x*row_length), false);
		}

		// If the numbers go below the sequence, do it here
		if(sequencenumberposition == NUMBER_BELOW) {
			CPrintItemRowList temparray;
			printrow->GetNumbersRows(temparray, x*row_length+sequence_start, sequence_length + sequence_start-1);
			for(int d = 0; d < temparray.size(); d++) {
				CPrintItemRow* printItemRow = temparray.GetPrintItemRow(d);
				rowarray.AddPrintItemRow(*printItemRow);
			}
		}

		// Write protein phases 4 to 6 (0 to 5 in array)
		for(int c = 3; c < phase; c++) {
			proteinleft[c] = (leftofphase[c] > row_length ? row_length : leftofphase[c]);
			if(proteinleft[c] > 0) {
				string tempstring = proteinphase[c].substr(x * row_length, proteinleft[c]);
				InsertGapSpaces(&rowarray, tempstring, sequence_start + (x * row_length), false);
			}
		}

		CPrintItemRow blank("", 0, 0); // Add a blank line
		rowarray.AddPrintItemRow(blank);
		remain = remain - row_length;
		for(int d = 0; d < phase; d++)
			leftofphase[d] -= row_length;

		// Do the pagination stuff for printing
		block_end = rowarray.size();
		if(rows_on_this_page + block_end - block_start < linesperpage)
			rows_on_this_page += block_end - block_start;
		else {
			number_of_lines_on_previous_page = 0;
			if(pagebreaks.size() > 0)
				number_of_lines_on_previous_page = pagebreaks[pagebreaks.size() - 1];
			pagebreaks.push_back(number_of_lines_on_previous_page + rows_on_this_page);
			rows_on_this_page = block_end - block_start;
		}
	}

	//	Write out the remainder of this section, if any, on the current page
	number_of_lines_on_previous_page = 0;
	if(pagebreaks.size() > 0)
		number_of_lines_on_previous_page = pagebreaks[pagebreaks.size() - 1];
	if(rows_on_this_page)
		pagebreaks.push_back(number_of_lines_on_previous_page + rows_on_this_page);

	return &rowarray;
}


//returns rowlength as number of character cells
int CAnalyzeDNAList::RowLength()
{
	if(sequencenumberposition == NUMBER_RIGHT) {
		int availablecharacterspaces = numcharshorz - GetLargestSequenceNumberLabel() - 2;

		// If the block will not fit in one line, change format to continuous
		if(block_gap_length+block_length > availablecharacterspaces)
			sequenceinblocks = false;
		if(sequenceinblocks)
			return GetNumberOfBlocks(availablecharacterspaces) * block_length;
		return(GetTidyRowLength(availablecharacterspaces));
	}
	else {//	Numbers above or below
		int availablecharacterspaces = numcharshorz;
		if(sequenceinblocks)
			return GetNumberOfBlocks(availablecharacterspaces) * block_length;
		return GetTidyRowLength(availablecharacterspaces);
	}
}

int CAnalyzeDNAList::NumberOfSequenceRows()
{
	return (sequence_length + row_length - 1) / row_length;
}

int CAnalyzeDNAList::GetLargestSequenceNumberLabel()
{
	int largest = sequence_length + sequence_start - 1;
	if(!largest)
		return 1;
	return log10(largest + 0.0f) + 1;
}

string CAnalyzeDNAList::GetSequenceNumberLabel(int position, string& data)
{
	int size;
	if(position < 1)
		size = 1;
	else
		size = log10(position + 0.0f) + 1;
	return to_string(position);
}

void CAnalyzeDNAList::InsertGapSpaces(CPrintItemRowList* array, string& data, int position, bool numbertoright)
{
	CPrintItemRow printrow;
	int stringlength = data.length();
	size_t startblocklength = GetStartBlockLength();
	int numberofgaps = GetNumberOfGaps();
	int x = 0;
	int remain = stringlength - startblocklength;
	printrow.AddItem(data, startblocklength, 0, 0);
	while(remain > 0) {
		printrow.AddItem(data.substr(startblocklength + block_length * x), min(block_length, remain), block_gap_length * (x+1) + startblocklength + block_length * x, 0);
		x++;
		remain -= block_length;
	}
	if(numbertoright) {
		string temp;
		printrow.AddItem(GetSequenceNumberLabel(stringlength+position-1, temp), stringlength+numberofgaps * block_gap_length + 2, 0);
	}
	array->AddPrintItemRow(printrow);
}

// Number of sequence blocks + block gaps that will fit in the passed linelength
int CAnalyzeDNAList::GetNumberOfBlocks(int linelength)
{
	blocks_per_row = linelength / (block_length + block_gap_length);
	return blocks_per_row;
}

// Get a rowlength from passed linelength that is a multiple of 10
int CAnalyzeDNAList::GetTidyRowLength(int linelength)
{
	if(linelength / 10 == 0)  // In case font is very big, and 10 chars will not fit on page
		return 1;
	return 10 * (linelength / 10);
}


void CAnalyzeDNAList::SetBlockDimensions()
{
	if(sequenceinblocks) {
		block_length = transferbuffer->blocksize;
		block_gap_length = transferbuffer->blockgapsize;
	}
	else {
		block_length = 10;
		block_gap_length = 0;
	}
	blocks_per_row = 5;
}

blockstructure* CAnalyzeDNAList::MakeBlockGapStructure(blockstructure& blocks)
{
	MakeGapArray(blocks.array);
	if(!sequenceinblocks)
		blocks.gapsize = 0;
	else
		blocks.gapsize = block_gap_length;
	blocks.startblock = GetStartBlockLength();
	blocks.endblock = GetEndBlockLength();
	blocks.fullblock = GetFullBlockLength();
	return &blocks;
}

int CAnalyzeDNAList::GetStartBlockLength()
{
	int startblock = (block_length - (sequence_start % block_length)) + 1;
	int stringlength = RowLength();
	if(startblock > stringlength)
		return stringlength;
	return startblock;
}

int CAnalyzeDNAList::GetEndBlockLength()
{
	return (row_length - GetStartBlockLength()) % block_length;
}

int CAnalyzeDNAList::GetFullBlockLength()
{
	return block_length;
}

int CAnalyzeDNAList::GetNumberOfGaps()
{
	int startblocklength = GetStartBlockLength();
	int stringlength = RowLength();
	if(startblocklength >= stringlength)
		return 0;
	return 1 + (stringlength - startblocklength) / block_length;
}

int CAnalyzeDNAList::MakeGapArray(vector<int>& array)
{
	int startblocklength = GetStartBlockLength();
	int stringlength = RowLength();
	array.clear();
	int gaps = GetNumberOfGaps();
	if(!gaps) {
		array.push_back(startblocklength - 1);
		return 1;
	}

	array.resize(gaps);
	array[0] = startblocklength-1;
	int remain = stringlength-startblocklength;
	int x = 1;
	while(x < gaps && remain < stringlength) {
		array[x] = array[x-1] + block_length;
		remain -= block_length;
		x++;
	}
	return gaps;
}

void CAnalyzeDNAList::InsertSpacesInProteinSequence(string& seq, int lengthofnucleicacid, int phase, int numberofspaces)
{
	if(seq.empty())
		return;
	size_t proteinlength = seq.length();
	size_t proteinsequencewithspaceslength;
	if(phase < 3)
		proteinsequencewithspaceslength = (numberofspaces + 1) * proteinlength;
	else
		proteinsequencewithspaceslength = lengthofnucleicacid;
	string temp = seq;
	seq.clear();
	seq.assign(proteinsequencewithspaceslength, ' ');
	for(int x = 0; x < proteinlength; x++) {
		if(phase < 3)
			seq[(x * (numberofspaces+1)) + phase] = temp[x];
		else
			seq[(x * (numberofspaces+1)) + phase - 3] = temp[x];
	}
}

#pragma once
#include "CodonTable.h"
#include "FormatPrintRows.h"
#include "FormatTransfer.h"
#include "PrintItemRowList.h"
#include "RestrictionCutList.h"
#include "SeqConvertor.h"

#include <memory>
#include <string>
#include <vector>
using namespace std;

#define NUMBER_ABOVE 0
#define NUMBER_BELOW 1
#define NUMBER_RIGHT 2

class CAnalyzeDNAList
{
public:
	CAnalyzeDNAList(const string& pointer_to_sequence, int seq_start, int seq_length,
					  CRestrictionCutList* result,
		PrintFormatTransferBuffer* printFormatTransferBuffer,
		int font_cx, int font_cy,
                 int numberofcharactersperhorizontalrow,
                 int linesperpage,
                 vector<int> pagebreaks,
                 char seq_type = 'D',
                 CCodonTable* codon = 0);

	~CAnalyzeDNAList();

	CPrintItemRowList* GetSitesInList(CPrintItemRowList& rowarray);
	CPrintItemRowList* TranslateNucleicAcid(CPrintItemRowList& rowarray, int phase);
	vector<int> pagebreaks;

protected:
	bool bothstrands, sequenceinblocks;
	bool numbertoright;
	char sequence_type;
	int linesperpage;
	int numcharshorz;
	int sequence_start;
	int sequence_length; //length of the sequence to list
	int sequencenumberposition;

	int block_length; //length of one sequence block
	int block_gap_length; //length of the gaps between sequence blocks
	int row_length;
	int character_cx, character_cy; //width and height of a character
	int rowcell_cx, rowcell_cy; // width and height of character in row cell
	int row_height;
	int blocks_per_row;
	int number_rows; //number of rows in the row 2D array

	string sequence; //start of the sequence to list
	CSeqConvertor convertobject;
	unique_ptr<CFormatPrintRows> printrow;
	CRestrictionCutList* resultarray;
	CCodonTable* codontable;
	PrintFormatTransferBuffer* transferbuffer;

	void InsertSpacesInProteinSequence(string& seq, int lengthofnucleicacid, int phase = 1, int numberofspaces = 2);

	int RowLength();
	int NumberOfSequenceRows();
	int GetLargestSequenceNumberLabel();
	string GetSequenceNumberLabel(int position, string& data);
	void InsertGapSpaces(CPrintItemRowList* array, string& data, int position, bool numbertoright);
	int GetNumberOfBlocks(int linelength);
	int GetTidyRowLength(int linelength);
	void SetBlockDimensions();
	blockstructure* MakeBlockGapStructure(blockstructure& blocks);
	int GetStartBlockLength();
	int GetEndBlockLength();
	int GetFullBlockLength();
	int GetNumberOfGaps();
	int MakeGapArray(vector<int>& array);
};

#pragma once
#include <string>

using namespace std;

struct AddEnzymeTransferBuffer
{
	string nametext;
	string sitetext;
	string uppertext;
	string lowertext;
};

struct AlignOptionsBuffer
{
	int gapopeningpenalty;
	int delaydivergentsequences;
	int proteinweightmatrix;
	int dnaweightmatrix;
	int residuespecificpenalty;
	int hydrophilicpenalty;
	int endgapseparation;
	int gapseparationdistance;
	int percentageconserved;
	int percentageidentical;
	float dnatransitionweight;
	float gapextensionpenalty;
	unsigned long identicalforegroundColor;
	unsigned long identicalbackgroundColor;
	unsigned long conservedforegroundColor;
	unsigned long conservedbackgroundColor;
};

enum ConversionOutputTransferBuffer
{
	replace, newwindow
};

enum DisplayFormat
{
	DF_LIST=2, DF_GRAPHIC=4, DF_TABLE=8, DF_LINE=16, DF_ALL=30, DF_TRANSLATE=32
};

enum EnzymeFormat
{
	EL_SORTNAMEASCENDING=0, EL_SORTNAMEDESCENDING=1, EL_SORTSITEASCENDING=2, EL_SORTSITEDESCENDING=3,
	EL_5PRIME=2, EL_3PRIME=4, EL_BLUNT=8, EL_NOSITE=16,
	EL_NONPALINDROME=32, EL_CUTOUTSIDE=64, EL_8CUTTER=128, EL_NOCUT=256, EL_4CUTTER=512,
	EL_6CUTTER=1024, EL_PALINDROME=2048, EL_ALL=4096,
	EL_SELECTION=8192, EL_GRAPHICUNIQUE=16384, EL_GRAPHICUNIQUEANDSEL=32768, EL_GRAPHICSELECTION=65536
};

struct EnzymeOptionsTransferBuffer
{
	bool all;
	bool selection;
	bool wholeset;
	bool fiveprime;
	bool threeprime;
	bool blunt;
	bool fourcutter;
	bool sixcutter;
	bool eightcutter;
	bool palindrome;
	bool outside;
	int graphicunique;
	int graphicuniqueandsel;
	int graphicselection;
};

struct FindORFTransferBuffer
{
	int showineditor;
	int showinresultwindow;
	int global = 0;
	int fromcursor;
};

struct FormatTransferBuffer
{
	bool all;
	bool sequence;
	bool graphic;
	bool line;
	bool tables;
};

struct PrintFontTransferBuffer
{
	int facestyle;
	int facesize;
};

struct PrintFormatTransferBuffer
{
	int displaywatsononly = 0;
	int displaywatsonandcrick = 0;
	int displaycontinuoussequence = 0;
	int displaysequenceblocks = 0;
	int numbersabovesequence = 0;
	int numbersbelowsequence = 0;
	int numbersrightsequence = 0;

	unsigned short blocksize;
	unsigned short blockgapsize;
};

struct TransFacFormatTransferBuffer
{
	bool showall;
	bool list;
	bool map;
	bool table;
};

enum TransFacOptionsTransferBuffer
{
	graphicunique, graphicuniqueandsel, graphicselection
};

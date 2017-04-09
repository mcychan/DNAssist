
// DNAssistView.cpp : implementation of the CDNAssistView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "ResultDoc.h"
#include "resource.h"
#include "ResultView.h"
#include "RichTextFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDNAssistView

IMPLEMENT_DYNCREATE(CResultView, CScrollView)

BEGIN_MESSAGE_MAP(CResultView, CScrollView)
ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
END_MESSAGE_MAP()

// CDNAssistView construction/destruction

CResultView::CResultView()
{
	height = 10;
	width = 8;
	leftmargin = 1;
	rightmargin =1;
	topmargin = 1;
	bottommargin = 1;
	mapmode = MM_TEXT;
	lines_per_page = 0;
	//	Show if text/graphics in window is currently selected
	selection = false;
	m_colorSeqDataBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_colorSeqDataText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_colorAddrBkgnd = ::GetSysColor(COLOR_BTNFACE);
}

CResultView::~CResultView()
{
}

BOOL CResultView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void CResultView::OnInitialUpdate()
{	
	CScrollView::OnInitialUpdate();	

	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);	
}

void CResultView::UpdateCtrlStatus()
{
	const CString BLANK = _T("");
	for (int i = 1; i<5; ++i)
		theApp.m_pStatusBar->SetPaneText(i, BLANK);
}

void CResultView::OnSetFocus(CWnd* pOldWnd)
{
	// take care of caret
	CScrollView::OnSetFocus(pOldWnd);
	UpdateCtrlStatus();
}

BOOL CResultView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		/// ScrollWindow(-(x-xOrig), -(y-yOrig)); // <-- removed
		Invalidate(); // <-- added // Invalidates whole client area
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}

	return TRUE;
}

// We will handle this ourselves
BOOL CResultView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// Get number of characters in one block
int CResultView::GetBlockWidth()
{
	//allow 1 space between name and number and 2 spaces between adjacent columns = +3
	return GetLongestNameString() + GetLongestNumberCutsString() + 3;
}

size_t CResultView::GetLongestNameString()
{
	size_t longest = 0;
	auto result = GetDocument()->GetData();	
	for (int i = 0; i < result->size(); ++i) {
		auto rc = result->GetPointer(i);
		size_t value = rc->GetName().length();
		longest = max(value, longest);
	}
	return longest;
}

size_t CResultView::GetLongestSiteString()
{
	size_t longest = 0;
	auto result = GetDocument()->GetData();
	for (int i = 0; i < result->size(); ++i) {
		auto rc = result->GetPointer(i);
		size_t value = rc->GetSiteFormatted().length();
		longest = max(value, longest);
	}
	return longest;
}

size_t CResultView::GetLongestNumberCutsString()
{
	size_t longest = 0;
	auto result = GetDocument()->GetData();
	for (int i = 0; i < result->size(); ++i) {
		auto rc = result->GetPointer(i);
		string cuts;
		int value = rc->GetNumberCuts(cuts).length();
		longest = max(value, longest);
	}
	return longest;
}

size_t CResultView::GetLongestCutPositionString()
{
	//  Assume here that the longest cutposition is the sequencelength
	auto result = GetDocument()->GetData();
	if(result->size() == 0)
		return 0;

	string seq;
	return result->GetPointer(0)->GetSequenceLength(seq).length();
}

size_t CResultView::GetLongestFragmentLengthString()
{
	//  Assume here that the longest fragment is the sequencelength
	return GetLongestCutPositionString();
}

size_t CResultView::GetLongestCutPositionAndFragmentLengthString()
{
	//  Assume here that the longest fragment is the sequencelength, and
	//  that the largest cutposition is the sequencelength, so just return
	//  two times the sequencelength or this enzyme result set
	auto result = GetDocument()->GetData();
	if(result->size() == 0)
		return 0;

	string seq;
	int value = result->GetPointer(0)->GetSequenceLength(seq).length();
	return 2 * value + 5; // 5 allows for space_data_space_bracket_data_bracket_space
}


//  This function returns the largest length of the first column of the enzyme
//  cut location and fragment length section.  The first column will contain
//  the enzyme name, site and number of cuts
size_t CResultView::GetLongestNameSiteNumber()
{
	return GetLongestNameString() + GetLongestSiteString() + GetLongestNumberCutsString() + 6;//Allow 2 spaces between each of the 3 columns, and two trailing spaces
}

int CResultView::GetNoCutBlockWidth()
{
	return GetLongestSiteString() + GetLongestNameString() + 4; //4 = data_2 spaces_data_2 spaces
}

int CResultView::GetLargeTickValue(int range)
{
	if(range)
		return (int) pow(10.0f, (int)(log10(range + 0.0f)));
	return 0;
}

CPoint CResultView::RotatePoint(CPoint oldcoords, float angle, bool clockwise)
{
	int xcos = oldcoords.x * cos(angle);
	int ysin = oldcoords.y * sin(angle);
	int xsin = oldcoords.x * sin(angle);
	int ycos = oldcoords.y * cos(angle);
	if(!clockwise)
		return CPoint(xcos - ysin, xsin + ycos);
	return CPoint(xcos + ysin, -xsin + ycos);
}

CPoint CResultView::MovePointToZero(CPoint value, CPoint offset)
{
	return value - offset;
}

CPoint CResultView::MovePointFromZero(CPoint value, CPoint offset)
{
	return value + offset;
}

size_t CResultView::GetSequenceLength()
{
	return GetDocument()->GetSeqLength();
}

BOOL CResultView::Touches(const CRect& rect1, const CRect& rect2)
{
	return (rect1.left <= rect2.right && rect1.right >= rect2.left &&
			rect1.top <= rect2.bottom && rect1.bottom >= rect2.top);
}

size_t CResultView::FormatGraphicLabelTag()
{
	if(graphiclabelposition.empty())
		return 0;

	CResultDoc* pDoc = GetDocument();
	int a = 1;
	int x;
	int number = graphiclabeltag.size();
	int loopstart;
	size_t sequencelength = pDoc->GetSeqLength();	

	size_t maxRight = 0;
	if(pDoc->GetSeqForm() == 'C') {
		if(number == 1) { // Special case
			if(graphiclabelposition[0] > 0.5 * sequencelength)
				MoveTagLeft(0);
			return 0;
		}
		if(graphiclabelposition[0] > 0.5 * sequencelength)
			MoveTagLeft(0); //	If case the first one is past 6 o' clock
		while(a < number && graphiclabelposition[a] <= 0.25 * sequencelength) {
			x = a;
			while(a < number && x > 0 && Touches(GetTagRectangle(x-1), GetTagRectangle(x))) {
				MoveTagUp(x-1, x);
				x--;
			}
			a++;
		}
		loopstart = a-1;
		while(a < number && graphiclabelposition[a] <= 0.5 * sequencelength) {
			x = loopstart;
			while(x < a && a < number) {
				if(Touches(GetTagRectangle(x), GetTagRectangle(a)))
					MoveTagDown(a, x);
				x++;
			}
			a++;
		}
		while(a < number && graphiclabelposition[a] <= 0.75 * sequencelength) {
			MoveTagLeft(a);
			x = a;
			while(a < number && x > 0 && Touches(GetTagRectangle(x-1), GetTagRectangle(x))) {
				MoveTagDown(x-1, x);
				x--;
			}
			a++;
		}
		loopstart = a-1;
		while(a < number && graphiclabelposition[a] <= sequencelength) {
			MoveTagLeft(a);
			x = loopstart;
			while(x < a && a < number) {
				if(Touches(GetTagRectangle(x), GetTagRectangle(a)))
					MoveTagUp(a, x);
				x++;
			}
			a++;
		}
	}
	else if(pDoc->GetSeqForm() == 'L') {		
		loopstart = a-1;		
		while(a < number && graphiclabelposition[a] <= sequencelength) {
			x = loopstart;
			while(x < a && a < number) {
				if(Touches(GetTagRectangle(x), GetTagRectangle(a))) {
					MoveTagUp(a, x);
					x = loopstart-1; //start checking again from 0
				}
				x++;
			}
			a++;
			maxRight = max(GetTagRectangle(x).right, maxRight);
		}
	}
	return maxRight;
}

CRect CResultView::GetTagRectangle(int index)
{
	if(index >= 0 && index < labelcoords.size())
		return CRect(labelcoords[index], CPoint(labelcoords[index].x + chardim.x * graphiclabeltag[index].length(),
   			  	 labelcoords[index].y + chardim.y));
	return CRect(0, 0, 0, 0);
}

void CResultView::MoveTagUp(int tomoveindex, int staythesame)
{
	CPoint staysamepoint = labelcoords[staythesame];
	int oldPointY = labelcoords[tomoveindex].y;
	labelcoords[tomoveindex].y = staysamepoint.y - chardim.y;
	if(oldPointY == labelcoords[tomoveindex].y)
		labelcoords[tomoveindex].y--;
}

void CResultView::MoveTagLeft(int index)
{
	labelcoords[index].x -= graphiclabeltag[index].length() * chardim.x;
}

void CResultView::MoveTagDown(int tomoveindex, int staythesame)
{
	CPoint staysamepoint = labelcoords[staythesame];
	int oldPointY = labelcoords[tomoveindex].y;
	labelcoords[tomoveindex].y = staysamepoint.y + chardim.y;
	if (oldPointY == labelcoords[tomoveindex].y)
		labelcoords[tomoveindex].y++;
}

void CResultView::MakeSiteUsageColumns()
{
	auto result = GetDocument()->GetData();
	int entries = result->size();
	if (entries <= 0)
		return;

	// Set up the site usage columns
	maxcolumns = pageprintwidth / (chardim.x * GetBlockWidth());	
	if(maxcolumns) { //Make sure maxcolumns != 0
		if(entries % maxcolumns)
			numberofrows = 1 + entries / maxcolumns;
		else
			numberofrows = entries / maxcolumns;
		if(entries % numberofrows)
			numberofcolumns = 1 + entries / numberofrows;
		else
			numberofcolumns = entries / numberofrows;
	}
	else {//If maxcolumns == 0, set it to one and print as many rows as members
		numberofcolumns = 1;
		numberofrows = entries;
	}

	//	Set up parameters used to calculate pagination for printing
	//	Get the line of the last pagebreak
	int start_of_this_section;
	if(!pagebreaks.empty())
		start_of_this_section = pagebreaks[pagebreaks.size()-1];
	else
		start_of_this_section = 0;

	//	Get the number of the entry in the rowarry where the current section will start
	int first_row;
	if(arrayofprintrows.size() > 0)
		first_row = arrayofprintrows.size();
	else
		first_row = 0;

	// Section Header
	CPrintItemRow header, freeline, freeline2;
	freeline2.AddItem("", 0, 0);
	header.AddItem("Number of recognition sites", 0, 0);
	freeline.AddItem("", 0, 0);
	arrayofprintrows.AddPrintItemRow(freeline2);
	arrayofprintrows.AddPrintItemRow(header);
	arrayofprintrows.AddPrintItemRow(freeline);

	//
	// Fill the rows strings to be printed
	//
	int blockwidth = GetBlockWidth();
	for(int row = 0; row < numberofrows; row++) {
		CPrintItemRow items;
		for(int column = 0; column < numberofcolumns && row + numberofrows * column < entries; column++) {
			auto rec = result->GetPointer(row + numberofrows * column);
			string cuts;
			rec->GetNumberCuts(cuts);
			items.AddItem(rec->GetName(), column * blockwidth, 0);
			items.AddItem(cuts, (column + 1) * blockwidth - cuts.length() - 2, 0);
		}
		arrayofprintrows.AddPrintItemRow(items);
   }

	//	Set up the pagebreaks
	int last_row = arrayofprintrows.size();
	int length_of_section = last_row - first_row;
	int count = 1;
	pagebreaks.reserve((int) ceil(length_of_section / lines_per_page));
	while(count * lines_per_page <= length_of_section) {
		pagebreaks.push_back(start_of_this_section + count * lines_per_page);
		count++;
	}
	if(pagebreaks.size() && (pagebreaks[pagebreaks.size() - 1] < start_of_this_section + length_of_section))
		pagebreaks.push_back(start_of_this_section + length_of_section);
}

void CResultView::MakeCutPositionColumns()
{
	CResultDoc* pDoc = GetDocument();
	int maxPage = max(pageprintwidth / (chardim.x * GetLongestCutPositionAndFragmentLengthString()), 1);
	int datablockwidth = GetLongestCutPositionAndFragmentLengthString();

	int positionwidth = GetLongestCutPositionString();

	//	Get the line of the last pagebreak
	int start_of_this_section;
	if(!pagebreaks.empty())
		start_of_this_section = pagebreaks[pagebreaks.size() - 1];
	else
		start_of_this_section = 0;

	//	Get the number of the entry in the rowarry where the current section will start
	int first_row;
	if(arrayofprintrows.size() > 0)
		first_row = arrayofprintrows.size();
	else
		first_row = 0;

	// Write the section header
	CPrintItemRow header, freeline, freeline2;
	freeline2.AddItem("", 0, 0);
	header.AddItem("Cleavage positions (fragment length)", 0, 0);
	freeline.AddItem("", 0, 0);
	arrayofprintrows.AddPrintItemRow(freeline2);
	arrayofprintrows.AddPrintItemRow(header);
	arrayofprintrows.AddPrintItemRow(freeline);

	auto result = pDoc->GetData();
	for(int z = 0; z < result->size(); z++) {
		auto rec = pDoc->GetRow(z);
		if(rec->GetNumberCuts() > 0) { //See if it cuts
			CPrintItemRow items;			
			items.AddItem(rec->GetName(), 0, 0);
			items.AddItem(rec->GetSiteFormatted(), rec->GetName().length() + 2, 0);
			string cuts;
			items.AddItem(rec->GetNumberCuts(cuts), rec->GetName().length() + rec->GetSiteFormatted().length() + 4, 0);
			arrayofprintrows.AddPrintItemRow(items);
			int a = 0;
			if(pDoc->GetSeqForm() == 'C') {
				while(a * maxPage < rec->GetNumberCuts()) {
					CPrintItemRow items;
					for(int b = 0; (b < maxPage) && (a*maxPage+b < rec->GetNumberCuts()); b++) {
						string cuts;
						rec->GetCutPosition(a*maxPage+b, cuts);
						size_t cutlength = cuts.length();
						string frag;
						rec->GetFragmentLength(a*maxPage+b, frag);
						size_t fraglength = frag.length();
						items.AddItem(cuts, b * datablockwidth + positionwidth - cutlength + 1, 0);
						items.AddItem("(", b * datablockwidth + (datablockwidth - 1) - 3 - fraglength, 0);
						items.AddItem(frag, b * datablockwidth + (datablockwidth - 1) - 2 - fraglength, 0);
						items.AddItem(")", b * datablockwidth + (datablockwidth - 1) - 2, 0);
					}
					arrayofprintrows.AddPrintItemRow(items);
					a++;
				}
			}
			else if(pDoc->GetSeqForm() == 'L') {
				while(a * maxPage <= rec->GetNumberCuts()) {
					CPrintItemRow items;
					for(int b = 0; (b < maxPage) && (a * maxPage + b <= rec->GetNumberCuts()); b++) {
						string cuts;
						if(a * maxPage + b == rec->GetNumberCuts())
							pDoc->GetRow(0)->GetSequenceLength(cuts);
						else
							rec->GetCutPosition(a * maxPage + b, cuts);
						size_t cutlength = cuts.length();
						string frag;
						rec->GetFragmentLength(a * maxPage + b, frag);
						size_t fraglength = frag.length();
						items.AddItem(cuts, b * datablockwidth + positionwidth - cutlength + 1, 0);
						items.AddItem("(", b * datablockwidth + (datablockwidth-1) - 3 - fraglength, 0);
						items.AddItem(frag, b * datablockwidth + (datablockwidth-1) - 2 - fraglength, 0);
						items.AddItem(")", b * datablockwidth + (datablockwidth-1) - 2, 0);
					}
					arrayofprintrows.AddPrintItemRow(items);
					a++;
				}
			}
		}
	}

	//	Set up the pagebreaks
	size_t last_row = arrayofprintrows.size();
	int length_of_section = last_row - first_row;
	int count = 1;
	while(count * lines_per_page <= length_of_section) {
		pagebreaks.push_back(start_of_this_section + count * lines_per_page);
		count++;
	}
	if(!pagebreaks.empty() && (pagebreaks[pagebreaks.size() - 1] < start_of_this_section + length_of_section))
		pagebreaks.push_back(start_of_this_section + length_of_section);
}

void CResultView::MakeNoCutterColumns()
{
	//	Get the line of the last pagebreak
	int start_of_this_section;
	if(!pagebreaks.empty())
		start_of_this_section = pagebreaks[pagebreaks.size()-1];
	else
		start_of_this_section = 0;

	//	Get the number of the entry in the rowarry where the current section will start
	int first_row = arrayofprintrows.size();

	//
	// Set up columns for enzymes that do not cut
	//
	CPrintItemRow header, freeline, freeline2;
	freeline2.AddItem("", 0, 0);
	header.AddItem("Absent sites", 0, 0);
	freeline.AddItem("", 0, 0);
	arrayofprintrows.AddPrintItemRow(freeline2);
	arrayofprintrows.AddPrintItemRow(header);
	arrayofprintrows.AddPrintItemRow(freeline);

	maxcolumns = pageprintwidth / (chardim.x * GetNoCutBlockWidth());
	CResultDoc* pDoc = GetDocument();
	int entries = pDoc->GetNumberNoCutters();
	if(maxcolumns) { //Make sure maxcolumns != 0
		if(entries % maxcolumns)
			numberofrows = 1 + entries / maxcolumns;
		else
			numberofrows = entries / maxcolumns;
		if(numberofrows) {
			if(entries % numberofrows)
				numberofcolumns = 1 + entries/numberofrows;
			else
				numberofcolumns = entries / numberofrows;
		}
		else
			numberofrows = entries;
	}
	else { //If maxcolumns == 0, set it to one and print as many rows as members
		numberofcolumns = 1;
		numberofrows = entries;
	}

	//
	// Fill the rows strings to be printed
	//
	int blockwidth = GetNoCutBlockWidth();	
	auto result = pDoc->GetData();
	for(int row = 0; row < numberofrows; row++) {
		CPrintItemRow items;
		for(int column = 0; column < numberofcolumns && row + numberofrows * column < entries; column++) {
			auto rec = pDoc->GetRow(row + numberofrows * column);
			items.AddItem(rec->GetName(), column * blockwidth, 0);
			items.AddItem(rec->GetSiteFormatted(), column * blockwidth + rec->GetName().length() + 2, 0);
		}
		arrayofprintrows.AddPrintItemRow(items);
	}

	//	Set up the pagebreaks
	int last_row = arrayofprintrows.size();
	int length_of_section = last_row - first_row;
	int count = 1;
	while(count * lines_per_page <= length_of_section) {
		pagebreaks.push_back(start_of_this_section + count * lines_per_page);
		count++;
	}
	if(!pagebreaks.empty() && (pagebreaks[pagebreaks.size() - 1] < start_of_this_section + length_of_section))
		pagebreaks.push_back(start_of_this_section + length_of_section);
}

void CResultView::MakeGraphicLabelTag()
{
	graphiclabeltag.clear();
	graphiclabelposition.clear();

	CResultDoc* pDoc = GetDocument();
	size_t start = pDoc->GetFirstUniqueCutter();
	int len = pDoc->GetNumberUniqueCutters();
	auto result = pDoc->GetData();

	if ((pDoc->analysisflags & EL_GRAPHICUNIQUE) || (pDoc->analysisflags & EL_GRAPHICUNIQUEANDSEL)) {
		graphiclabeltag.resize(start + len);
		graphiclabelposition.resize(start + len);
		for (size_t x = start; x < start + len; x++) {
			auto rec = pDoc->GetRow(x);
			string number;
			rec->GetCutPosition(0, number);
			string data = number;
			data += " ";
			data += rec->GetName();
			data += " ";
			graphiclabeltag[x] = data;
			graphiclabelposition[x] = rec->GetCutPosition(0);
		}
	}
	if ((pDoc->analysisflags & EL_GRAPHICSELECTION) || (pDoc->analysisflags & EL_GRAPHICUNIQUEANDSEL)) {
		int numberofselectedenzymes = result->GetSelectedEnzymeArray(selectedenzymes);
		for (int selenz = 0; selenz < numberofselectedenzymes; selenz++) {
			auto rec = result->GetPointer(selectedenzymes[selenz]);
			for (int cuts = 0; cuts < rec->GetNumberCuts(); cuts++) {
				string number;
				rec->GetCutPosition(cuts, number);
				string data = "* ";
				data += number;
				data += rec->GetName();
				data += " ";
				graphiclabeltag.push_back(move(data));
				graphiclabelposition.emplace_back(rec->GetCutPosition(cuts));
			}
		}
	}
	// Sort the arrays -- it saves a lot of heartache later
	int loopstart = 0;
	len = graphiclabeltag.size();//	recalculate len; we may have added sites from selected enzymes above
	if (len <= 0)
		return;

	int* pos = &graphiclabelposition[0];
	for (int z = loopstart + 1; z < len; z++) {
		int innerloop = z;
		while (innerloop > loopstart && pos[innerloop] < pos[innerloop - 1]) {
			swap(pos[innerloop - 1], pos[innerloop]);
			swap(graphiclabeltag[innerloop - 1], graphiclabeltag[innerloop]);
			innerloop--;
		}
	}
}

void CResultView::MakeGraphicMap(int& nWidth, int nHeight)
{
	//
	// Calculate the graphic map
	//
	CResultDoc* pDoc = GetDocument();
	int percentageofouter = 2;
	innertick.clear();
	outertick.clear();
	double pi = 3.14159265358979;
	size_t sequencelength = pDoc->GetSeqLength();
	//	Get the best unit, i.e. 100 for sequence ~1000bp, 1000 for sequence 1000s of bp etc.
	int largetick = GetLargeTickValue(sequencelength);
	CPoint temppoint, unitpoint, adjustedpoint;
	labeltickstart.clear();
	labeltickend.clear();
	labelcoords.clear();

	//
	// If the sequence is circular
	//
	if(pDoc->GetSeqForm() == 'C') {
		if(nWidth < nHeight) {
			center = CPoint(nWidth / 2 + leftoffset, nHeight / 2);
			outercircle = CRect(center.x - nWidth / 4, center.y - nWidth / 4,
      								center.x - nWidth / 4 + nWidth / 2, center.y - nWidth / 4+nWidth / 2);
			innercircle = CRect(center.x - nWidth / 4 + (nWidth / 2) * 2 * percentageofouter / 100,
      								center.y - nWidth / 4 + (nWidth / 2) * 2 *percentageofouter / 100,
                              center.x - nWidth / 4 + nWidth / 2 - (nWidth / 2) * 2 * percentageofouter / 100,
                              center.y - nWidth / 4 + nWidth / 2 - (nWidth / 2) * 2 * percentageofouter / 100);

			outerstart = CPoint(center.x, center.y - nWidth / 4);
			outerend = CPoint(center.x, center.y - nWidth / 4);
			innerstart = CPoint(center.x, center.y - nWidth / 4 + (nWidth / 2) * 2 * percentageofouter / 100);
			innerend = CPoint(center.x, center.y - nWidth / 4 + (nWidth / 2) * 2 * percentageofouter / 100);
		}
		else {
			center = CPoint(nWidth / 2 + leftoffset, nHeight / 2);
			outercircle = CRect(center.x - nHeight / 4, center.y - nHeight / 4,
      								center.x - nHeight / 4 + nHeight / 2, center.y - nHeight / 4 + nHeight / 2);
			innercircle = CRect(center.x - nHeight / 4 + (nHeight / 2) * 2 * percentageofouter / 100,
      								center.y - nHeight / 4 + (nHeight / 2) * 2 * percentageofouter / 100,
                              center.x - nHeight / 4 + nHeight / 2 - (nHeight / 2) * 2 * percentageofouter / 100,
                              center.y - nHeight / 4 + nHeight / 2 -(nHeight / 2) * 2 * percentageofouter / 100);

			outerstart = CPoint(center.x, center.y - nHeight / 4);
			outerend = CPoint(center.x, center.y - nHeight / 4);
			innerstart = CPoint(center.x, center.y - nHeight / 4 + (nHeight / 2) * 2 * percentageofouter / 100);
			innerend = CPoint(center.x, center.y - nHeight / 4 + (nHeight / 2) * 2 * percentageofouter / 100);
		}
		CPoint labeltickendcoord(outerstart.x, outerstart.y - chardim.y);
		CPoint labelcoordscoords(outerstart.x, outerstart.y - 2 * chardim.y);
		int tickCount = sequencelength / largetick;
		innertick.resize(tickCount);
		outertick.resize(tickCount);
		for(int r = 0; r < tickCount; r++) {
			float angle =  (float)(2 * pi * r * largetick / sequencelength);
			innertick[r] = MovePointFromZero(RotatePoint(MovePointToZero(innerstart, center), angle, false), center);
			outertick[r] = MovePointFromZero(RotatePoint(MovePointToZero(outerstart, center), angle, false), center);
		}

		MakeGraphicLabelTag();

		size_t tagCount = graphiclabeltag.size();
		labeltickstart.resize(tagCount);
		labeltickend.resize(tagCount);
		labelcoords.resize(tagCount);
		for(int q = 0; q < tagCount; q++) {
			float angle = (float)(2 * pi * graphiclabelposition[q] / sequencelength);
			labeltickstart[q] = MovePointFromZero(RotatePoint(MovePointToZero(outerstart, center), angle, false), center);
			labeltickend[q] = MovePointFromZero(RotatePoint(MovePointToZero(labeltickendcoord, center), angle, false), center);
			CPoint labelcoord = MovePointFromZero(RotatePoint(MovePointToZero(labelcoordscoords, center), angle, false), center);
			labelcoords[q] = CPoint(labelcoord.x, (int) (labelcoord.y - 0.5 * chardim.y));
		}

		FormatGraphicLabelTag();
	}
	else if(pDoc->GetSeqForm() == 'L') {
		MakeGraphicLabelTag();
		center = CPoint(leftoffset + nWidth / 2, nHeight / 2);
		sequencerectangle = CRect(center.x - nHeight / 3, center.y - 0.5 * chardim.y,
			center.x - nHeight / 3 + nHeight * 2 / 3, center.y + 0.5 * chardim.y);

		int tickCount = largetick > 0 ? sequencelength / largetick : 0;
		innertick.resize(tickCount);
		outertick.resize(tickCount);
		for(int r = 0; r < tickCount; r++) {
			LONG tickWidth = sequencerectangle.left + (float)(r * largetick / sequencelength) * sequencerectangle.Width();
			innertick[r] = CPoint(tickWidth, sequencerectangle.bottom);
			outertick[r] = CPoint(tickWidth, sequencerectangle.top);
		}

		size_t tagCount = graphiclabeltag.size();
		labeltickstart.resize(tagCount);
		labeltickend.resize(tagCount);
		labelcoords.resize(tagCount);
		for(int q = 0; q < tagCount; q++) {
			float xpos = (float) graphiclabelposition[q] / (float) sequencelength;
			LONG tagWidth = sequencerectangle.left + xpos * sequencerectangle.Width();
			labeltickstart[q] = CPoint(tagWidth, sequencerectangle.top);
			labeltickend[q] = CPoint(tagWidth, sequencerectangle.top - chardim.y);
			labelcoords[q] = CPoint(tagWidth, sequencerectangle.top - 2 * chardim.y);
		}
		nWidth = max(FormatGraphicLabelTag(), nWidth);
	}
}

void CResultView::MakeCutPositionLines()
{
}

void CResultView::SetModifiedFlag(BOOL bModified)
{
}

void CResultView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CResultDoc* pDoc = GetDocument();
	auto data = pDoc->GetData();
	if (data && data->size() <= 0) {
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
		return;
	}

	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetViewportOrg(CPoint(0, 0)); // force viewport origin to zero	

	if (lines_per_page <= 0) {
		if (pDC->IsPrinting()) {
			pDC->SetMapMode(MM_LOENGLISH);
			pDC->SetMapMode(MM_ANISOTROPIC);
			CSize size;
			GetViewportExtEx(pDC->GetSafeHdc(), &size);
			SetViewportExtEx(pDC->GetSafeHdc(), size.cx, -size.cy, NULL);
		}
		else {
			pDC->SetMapMode(MM_TEXT);          // force map mode to MM_TEXT
											   //	"Generic" width X height for window painting purposes,
											   //	since there apparently is no printer
			width = 8;
			height = 11;
		}

		SetFontDimensions(pDC);
		characters_per_row = GetCharactersPerRow();
		lines_per_page = GetLinesPerPage();		

		//	If this is for a nucleic acid translation, do only that, then return
		if (pDoc->displayflags & DF_TRANSLATE) {
			analyzednalist = pDoc->GetAnalyzeDNAList(chardim, characters_per_row,
				lines_per_page, pagebreaks);
			analyzednalist->TranslateNucleicAcid(arrayofprintrows, pDoc->phase);

			int topoffset = vertpixels * topmargin / 2;
			pagewidth = arrayofprintrows.GetStringLengthThatWillHoldAllItems() * chardim.x + leftoffset * 2;
			pageheight = arrayofprintrows.size() * chardim.y + topoffset * 2;
			CSize sizeTotal(pagewidth, pageheight);
			SetScrollSizes(MM_TEXT, sizeTotal);
			return;
		}			

		if (pDoc->displayflags & DF_LIST) {
			analyzednalist = pDoc->GetAnalyzeDNAList(chardim, characters_per_row, lines_per_page, pagebreaks);
			analyzednalist->GetSitesInList(arrayofprintrows);
			int topoffset = vertpixels * topmargin / 2;
			pagewidth = arrayofprintrows.GetStringLengthThatWillHoldAllItems() * chardim.x + leftoffset;
			pageheight = arrayofprintrows.size() * chardim.y + topoffset * 2;
		}		

		if (pDoc->displayflags & DF_TABLE) {
			MakeSiteUsageColumns();
			MakeCutPositionColumns();
			MakeNoCutterColumns();

			int topoffset = vertpixels * topmargin / 2;
			pagewidth = arrayofprintrows.GetStringLengthThatWillHoldAllItems() * chardim.x + leftoffset;
			pageheight = arrayofprintrows.size() * chardim.y + topoffset * 2;
		}

		if (pDoc->displayflags & DF_GRAPHIC) {			
			pagewidth = width * horpixels;
			pageheight = height * vertpixels;
			if (pDC->IsPrinting())
				MakeGraphicMap(pageprintwidth, pageprintheight);
			else
				MakeGraphicMap(pagewidth, pageheight);
		}

		if (pDoc->displayflags & DF_LINE)
			MakeCutPositionLines();

		CSize sizeTotal(pagewidth, pageheight);
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
}

void CResultView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// get the current page number(for first page returns 1, for second - 2 and so on)
	size_t nCurPage = pInfo->m_nCurPage;
	int maxPg = GetTotalNumberOfPages();

	// Compute the number of pages to print.
	//
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(maxPg);
}

void CResultView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CPoint point(20, -18);
	pDC->TextOut(point.x, point.y, _T("DNAssist"));
}

void CResultView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
	int nPageWidth = ::MulDiv(pDC->GetDeviceCaps(PHYSICALWIDTH), 1440, pDC->GetDeviceCaps(LOGPIXELSX));
	int nPageHeight = ::MulDiv(pDC->GetDeviceCaps(PHYSICALHEIGHT), 1440, pDC->GetDeviceCaps(LOGPIXELSY));
	CPoint point(0, -nPageHeight + 638);

	//draw page number
	CString   sTemp;
	sTemp.Format(_T("%d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
	CSize size = pDC->GetTextExtent(sTemp);
	point.x += nPageWidth * 0.544f - size.cx; // Move 4.5 inches right
	pDC->TextOut(point.x, point.y, sTemp); // right-justified
}

void CResultView::PaintSeqData(CDC* pDC, CPrintInfo* pInfo)
{
	CResultDoc* pDoc = GetDocument();
	size_t number_of_line_at_top = 0;
	UINT nCurPage = 1;
	if(pInfo) {
		nCurPage = pInfo->m_nCurPage;
		if(pDoc->displayflags & DF_GRAPHIC)
			number_of_line_at_top = nCurPage > 2 ? pagebreaks[nCurPage - 3] : 0;
		else
			number_of_line_at_top = nCurPage > 1 ? pagebreaks[nCurPage - 2] : 0;
	}

	int topoffset = vertpixels * topmargin / 2;
	pDC->SelectObject(font.get());
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfUnderline = TRUE;
	CFont underlinefont;
	underlinefont.CreateFontIndirect(&logFont);

	logFont.lfWeight = FW_BOLD;
	//logFont.lfOutPrecision = OUT_TT_PRECIS;
	logFont.lfUnderline = FALSE;
	CFont boldfont;
	boldfont.CreateFontIndirect(&logFont);
	CPen thinpen(PS_SOLID | PS_COSMETIC, 1, ::GetSysColor(COLOR_WINDOWTEXT));
	CPen thickpen(PS_SOLID | PS_COSMETIC, 2, ::GetSysColor(COLOR_WINDOWTEXT));
	int space_for_figure = GetLinesPerPage() * chardim.y;

	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);

	CPoint point = GetScrollPosition();
	//	Draw graphic when selected and any part is visible
	if (pDoc->displayflags & DF_GRAPHIC && nCurPage == 1) {
		if (pDoc->GetSeqForm() == 'C') {
			pDC->SelectObject(&thickpen);
			pDC->Arc(outercircle, outerstart, outerend);
			pDC->Arc(innercircle, innerstart, innerend);
			pDC->SelectObject(&thinpen);
		}
		else {
			pDC->SelectObject(&thickpen);
			pDC->Rectangle(sequencerectangle);
			pDC->SelectObject(&thinpen);
		}

		for (int y = 0; y < innertick.size(); y++) {
			pDC->MoveTo(innertick[y].x, innertick[y].y);
			pDC->LineTo(outertick[y].x, outertick[y].y);
		}

		for (int z = 0; z < graphiclabelposition.size(); z++) {
			if (graphiclabeltag[z].empty())
				continue;
			pDC->MoveTo(labeltickstart[z].x, labeltickstart[z].y);
			pDC->LineTo(labeltickend[z].x, labeltickend[z].y);
			pDC->TextOut(labelcoords[z].x, labelcoords[z].y, CString(graphiclabeltag[z].c_str()));
		}

		//	Print the graphic legend (pBR322 4260bp etc.)
		CPoint namecoords = GetGraphicNameTagCoordinates(pDC);
		CString name;
		GetGraphicNameTag(name);
		pDC->SelectObject(&boldfont);
		pDC->TextOut(namecoords.x, namecoords.y, name);
		CPoint sizecoords = GetGraphicSizeTagCoordinates(pDC);
		CString sizetag;
		GetGraphicSizeTag(sizetag);
		pDC->TextOut(sizecoords.x, sizecoords.y, sizetag);
		pDC->SelectObject(font.get());
	}

	if(pDoc->displayflags & DF_LIST || pDoc->displayflags & DF_TABLE || pDoc->displayflags & DF_TRANSLATE) {
		if (pDC->IsPrinting()) {
			if(!(pDoc->displayflags & DF_GRAPHIC))
				nCurPage++; //	When printing translation, increment currentpage, since there is no graphic

			for(int x = number_of_line_at_top; x < pagebreaks[nCurPage - 2] && x < arrayofprintrows.size(); x++) {
				auto pir = arrayofprintrows.GetPrintItemRow(x);
				for(int y = 0; y < pir->size(); y++) {
					if(pir->GetItemData(y))
						pDC->SelectObject(&underlinefont);
					else
						pDC->SelectObject(font.get());
					CString str(pir->GetString(y).c_str());
					pDC->TextOut(pir->GetCoordinate(y) * chardim.x + leftoffset, (x - number_of_line_at_top) * chardim.y + topoffset, str);
				}
			}
		}
		else {
			int start, end;
			if(pDoc->displayflags & DF_GRAPHIC) {
				start = (rect.top - topoffset) / chardim.y - GetLinesPerPage();
				end = 1 + start;
			}
			else {
				space_for_figure = 0;
				start = (rect.top - topoffset) / chardim.y;
				end = 1 + start;
			}
			start = max(0, start);
			end = (end > arrayofprintrows.size()) ? arrayofprintrows.size() : end;
			for(int x = start; x < end ; x++) {
				auto pir = arrayofprintrows.GetPrintItemRow(x);
				for(int y = 0; y < pir->size(); y++) {
					if(pir->GetItemData(y))
						pDC->SelectObject(&underlinefont);
					else
						pDC->SelectObject(font.get());
					CString str(pir->GetString(y).c_str());
					pDC->TextOut(pir->GetCoordinate(y) * chardim.x + leftoffset, x * chardim.y + topoffset + space_for_figure, str);
				}
			}
		}
	}

	boldfont.DeleteObject();
}

void CResultView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	PrintHeader(pDC, pInfo);
	PaintSeqData(pDC, pInfo);
	PrintFooter(pDC, pInfo);
}

void CResultView::OnDraw(CDC* pDCView)
{
	ASSERT(MM_TEXT == pDCView->GetMapMode());
	ASSERT(CPoint(0, 0) == pDCView->GetViewportOrg());

	if (pagewidth <= 0 || pageheight <= 0) {
		CRect rcClip(0, 0, 0, 0);
		pDCView->GetClipBox(rcClip);
		pDCView->FillSolidRect(rcClip, m_colorAddrBkgnd);
		ReleaseDC(pDCView);
		return;
	}

	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);

	const LONG cx = rcClient.right;      // view client area width
	const LONG cy = rcClient.bottom;     // view client area height	

	if (m_pDC.get() == NULL) {
		m_pDC = make_unique<CMemoryDC>(pDCView, pagewidth, pageheight);
		//draw background		
		CRect rcDoc(0, 0, pagewidth, pageheight);
		m_pDC->FillSolidRect(rcDoc, m_colorAddrBkgnd);

		PaintSeqData(m_pDC.get(), NULL);

		ReleaseDC(m_pDC.get());
		CResultDoc* pDoc = GetDocument();
		if (pDoc->displayflags & DF_GRAPHIC) {
			if(pDoc->GetSeqForm() == 'L')
				ScrollToPosition(CPoint(0, pageheight / 8));
			else
				ScrollToPosition(CPoint(0, pageheight / 4));
		}
	}

	CPoint point = GetScrollPosition();
	if(character_dimension.cy <= 0)
		character_dimension.cy = textmetric.tmHeight + textmetric.tmExternalLeading;
	const size_t nScrolledLine = floor(point.y / character_dimension.cy);

	const LONG xPos = point.x;           // horizontal scroll position
	const LONG yPos = nScrolledLine * character_dimension.cy;           // vertical scroll position
													   // source and destination cordinates and sizes
	LONG nDstWidth, nDstHeight;

	if (pagewidth > cx)
		nDstWidth = pagewidth - xPos;
	else
		nDstWidth = pagewidth;

	if (pageheight > cy)
		nDstHeight = pageheight - yPos;
	else
		nDstHeight = pageheight;

	CRect rcClip(0, 0, 0, 0);
	pDCView->GetClipBox(rcClip);
	pDCView->BitBlt(rcClip.left, rcClip.top, nDstWidth, nDstHeight,
		m_pDC.get(),
		xPos, yPos,
		SRCCOPY);
	pDCView->ExcludeClipRect(rcClip.left, rcClip.top, nDstWidth, nDstHeight);
	pDCView->FillSolidRect(rcClip, m_colorAddrBkgnd);
	ReleaseDC(pDCView);
}

void CResultView::SetFontDimensions(CDC* pDC)
{
	CResultDoc* pDoc = GetDocument();
	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);

	horpixels = pDC->GetDeviceCaps(LOGPIXELSX);
	vertpixels = pDC->GetDeviceCaps(LOGPIXELSY);
	pageprintwidth = (width - leftmargin - rightmargin) * horpixels;
	pageprintheight = (height - topmargin - bottommargin) * vertpixels;
	//pageprintwidth = (rcClient.Width() - leftmargin - rightmargin);
	//pageprintheight = rcClient.Height() - topmargin - bottommargin;

	leftoffset = leftmargin * horpixels / 2;
	font = make_unique<CFont>();
	auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
	font->CreateFont(printFontTransferBuffer->facesize * theApp.GetSystemScaleFactor(), 0, 0, 0, printFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, theApp.printFontFace);

	pDC->SelectObject(font.get());

	//	Make this to a temporary underlined font to get the proper size, since we may
	//	underline some characters in Paint()
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfUnderline = TRUE;
	CFont tempfont;
	tempfont.CreateFontIndirect(&logFont);
	pDC->SelectObject(tempfont);

	pDC->GetTextMetrics(&textmetric);
	//dimension of character in device context
	chardim.x = (textmetric.tmAveCharWidth + textmetric.tmMaxCharWidth) / 2;
	chardim.y = textmetric.tmHeight + textmetric.tmExternalLeading;
}

int CResultView::GetCharactersPerRow()
{
	return pageprintwidth / chardim.x;
}

int CResultView::GetLinesPerPage()
{
	return pageprintheight / chardim.y;
}

CRect CResultView::GetClientPosition()
{
	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);
	int screenwidth = rcClient.Width();
	int screenheight = rcClient.Height();
	int wantedwindowheight = arrayofprintrows.size() * vertpixels > 0.8 * screenheight ? 0.8 * screenheight : arrayofprintrows.size() * vertpixels;
	return CRect((screenwidth - width * horpixels) / 2, (screenheight - wantedwindowheight) / 2,
			width * horpixels > 0.8 * screenwidth ? 0.8 * screenwidth : width * horpixels,
            height * vertpixels > 0.8 * screenheight ? 0.8 * screenheight : height * vertpixels);
}

vector<int> CResultView::GetPageBreaks()
{
	return pagebreaks;
}

int CResultView::GetTotalNumberOfPages()
{
	int pages = pagebreaks.size();
	if(GetDocument()->displayflags & DF_GRAPHIC)
		pages++;
	return max(pages, 1);
}

//	This function returns the string decsribing the graphic, such as pBR322, pUC18 etc.
CString CResultView::GetGraphicNameTag(CString& data)
{
	CResultDoc* pDoc = GetDocument();
	data = pDoc->GetTitle();
	if(data.GetLength() == 0)
		return data;
	unsigned int chars_that_fit_in_circle;
	if(pDoc->GetSeqForm() == 'C')
		chars_that_fit_in_circle = innercircle.right / chardim.x;
	else
		chars_that_fit_in_circle = data.GetLength() + 1;
	if(data.GetLength() > chars_that_fit_in_circle)
		data = data.Left(chars_that_fit_in_circle-1);
	return data.Left(data.Find(_T('.')));
}

//	This function returns the print coordinates of the grahpic name tag.
//	The returned coordinates are one character cell above the page center,
// and centered on this center
CPoint CResultView::GetGraphicNameTagCoordinates(CDC* pDC)
{
	CResultDoc* pDoc = GetDocument();
	CPoint coords(0, 0);
	CString name;
	GetGraphicNameTag(name);
	if(name.GetLength() == 0)
		return coords;

	// Get the character sizes of the "bold font"
	HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont * gf = CFont::FromHandle(hf);
	LOGFONT labellogfont;
	gf->GetLogFont(&labellogfont);
	labellogfont.lfWeight = FW_BOLD;
	//labellogfont.lfOutPrecision = OUT_TT_PRECIS;
	CFont boldfont;
	boldfont.CreateFontIndirect(&labellogfont);
	pDC->SelectObject(&boldfont);
	TEXTMETRIC localmetrics;
	CPoint word;
	pDC->GetTextMetrics(&localmetrics);
	word.x = pDC->GetTextExtent(name).cx;
	word.y = localmetrics.tmHeight + localmetrics.tmExternalLeading;

	coords.x = center.x - word.x / 2;
	if(pDoc->GetSeqForm() == 'C')
		coords.y = center.y - word.y;
	else
		coords.y = center.y + 2 * word.y;

	boldfont.DeleteObject();
	return coords;
}

//	This function returns the string decsribing the graphic size, such as 123bp, 3245bp etc.
CString CResultView::GetGraphicSizeTag(CString& data)
{
	size_t sequencelength = GetDocument()->GetSeqLength();
	int size;
	if(sequencelength < 1)
		size = 1;
	else
		size = log10(sequencelength + 0.0f) + 1;

	data.Format(_T("%dbp"), sequencelength);
	return data;
}

//	This function returns the print coordinates of the graphic size tag.
//	The returned coordinates are below the page center,
// and centered on this center
CPoint CResultView::GetGraphicSizeTagCoordinates(CDC* pDC)
{
	CPoint coords(0, 0);
	CString name;
	GetGraphicSizeTag(name);

	// Get the character sizes of the "bold font"
	HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont * gf = CFont::FromHandle(hf);
	LOGFONT labellogfont;
	gf->GetLogFont(&labellogfont);
	labellogfont.lfWeight = FW_BOLD;
	//labellogfont.lfOutPrecision = OUT_TT_PRECIS;
	CFont boldfont;
	boldfont.CreateFontIndirect(&labellogfont);
	pDC->SelectObject(&boldfont);
	TEXTMETRIC localmetrics;
	CPoint word;
	pDC->GetTextMetrics(&localmetrics);
	word.x = pDC->GetTextExtent(name).cx;
	word.y = localmetrics.tmHeight + localmetrics.tmExternalLeading;

	//	Calculate the coordinates
	coords.x = center.x - word.x / 2;
	if(GetDocument()->GetSeqForm() == 'C')
		coords.y = center.y + word.y;
	else
		coords.y = center.y + 3 * word.y;

	boldfont.DeleteObject();
	return coords;
}

BOOL CResultView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	cumulatedwheelrotation += zDelta;
	if(cumulatedwheelrotation >= WHEEL_DELTA) {
		WPARAM param = MAKEWPARAM(SB_PAGEUP, 0);
		PostMessage(WM_VSCROLL, param, 0);
		cumulatedwheelrotation = 0;
	}
	else if (-cumulatedwheelrotation >= WHEEL_DELTA) {
		WPARAM param = MAKEWPARAM(SB_PAGEDOWN, 0);
		PostMessage(WM_VSCROLL, param, 0);
		cumulatedwheelrotation = 0;
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CResultView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	if(selection) {
		selection = false;
		Invalidate();
	}
}

void CResultView::OnEditSelectAll()
{
	selection = true;
	Invalidate();
}

string CResultView::GetTextFormat()
{
	string txt;
	int outerloop = arrayofprintrows.size();
	for(int x = 0; x < outerloop; x++) {
		auto pir = arrayofprintrows.GetPrintItemRow(x);
		int innerloop = pir->size();
		for (int y = 0; y < innerloop; y++) {
			int numberofspaces = 0;
			//	Calculate the intervening spaces
			if (y)
				numberofspaces = pir->GetCoordinate(y) - pir->GetCoordinate(y - 1) - pir->GetString(y - 1).length();
			else
				numberofspaces = pir->GetCoordinate(y);

			string temp;
			temp.assign(numberofspaces, (char) 32);
			txt += temp;
			txt += pir->GetString(y);
		}

		txt += (char) 13;
		txt += (char) 10;
	}
	return txt;
}

string CResultView::GetRichTextFormat()
{
	string rtf;
	//	Do all this to get the LOGFONT structure of the resultwindow font
	CFont font;
	auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
	font.CreateFont(printFontTransferBuffer->facesize, 0, 0, 0, printFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, theApp.printFontFace);

	LOGFONT logFont;
	font.GetLogFont(&logFont);

	CRichTextFormat rtfobject(logFont);
	rtfobject.Initialise();

	int numberofspaces;
	int outerloop = arrayofprintrows.size();
	for(int x = 0; x < outerloop; x++) {
		auto pir = arrayofprintrows.GetPrintItemRow(x);
		int innerloop = pir->size();
		for(int y = 0; y < innerloop; y++) {
			//	Calculate the intervening spaces
			if(y)
				numberofspaces = pir->GetCoordinate(y) - pir->GetCoordinate(y - 1) - pir->GetString(y - 1).length();
			else
				numberofspaces = pir->GetCoordinate(y);

			string temp;
			temp.assign(numberofspaces, (char) 32);
			rtfobject.AddString(temp);

			// Set underline if required
			if(pir->GetItemData(y))
				rtfobject.SetUnderline();

			// Add the relevant text
			rtfobject.AddString(pir->GetString(y));

			//	Set the font back
			rtfobject.SetFont();
		}
		rtfobject.AddLineBreak();
	}
	rtfobject.Finalise();
	rtfobject.GetRichTextFormat(rtf);
	font.DeleteObject();
	return rtf;
}

void CResultView::SaveAsEnhancedMetaFile(LPCTSTR lpszFilePath)
{
	CMetaFileDC MFDC;
	CClientDC cdc(this);

	MFDC.CreateEnhanced(NULL, NULL, NULL, NULL);
	MFDC.SetBkMode(TRANSPARENT);

	MFDC.SetAttribDC(cdc.m_hAttribDC);

	MFDC.SetMapMode(MM_LOENGLISH);
	MFDC.SetMapMode(MM_ANISOTROPIC);
	CSize size;
	GetViewportExtEx(MFDC.GetSafeHdc(), &size);
	SetViewportExtEx(MFDC.GetSafeHdc(), size.cx * 1.2, -size.cy * 1.2, NULL);
	PaintSeqData(&MFDC, NULL);

	HENHMETAFILE hEnhMetaFile = MFDC.CloseEnhanced();
	HENHMETAFILE hEMF = CopyEnhMetaFile(hEnhMetaFile, lpszFilePath);
	DeleteEnhMetaFile(hEMF);
	DeleteEnhMetaFile(hEnhMetaFile);
}

void CResultView::OnEditCopy()
{
	CResultDoc* pDoc = GetDocument();
	if(pDoc->displayflags & DF_GRAPHIC) {
		CRect rect(0, 0, pagewidth, pageheight);
		CBitmap junk;
		CClientDC cdc(this);
		CDC dc;
		dc.CreateCompatibleDC(&cdc);
		junk.CreateCompatibleBitmap(&cdc, rect.Width(), rect.Height());
		dc.SelectObject(&junk);

		//draw background		
		CRect rcDoc(0, 0, pagewidth, pageheight);
		dc.FillSolidRect(rcDoc, m_colorAddrBkgnd);
		PaintSeqData(&dc, NULL);

		if (OpenClipboard()) {
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, junk.m_hObject);
			CloseClipboard();
		}
	}
	else if((pDoc->displayflags & DF_LIST) || (pDoc->displayflags & DF_TABLE) || (pDoc->displayflags & DF_TRANSLATE)) {		
		CLIPFORMAT cfRTF = RegisterClipboardFormat(_T("Rich Text Format"));
		if(cfRTF) {
			string rtf = GetRichTextFormat();
			const char* data = rtf.c_str();
			HGLOBAL glob = GlobalAlloc(GMEM_FIXED, rtf.length());
			memcpy(glob, data, rtf.length() * sizeof(char));
			if (OpenClipboard()) {
				EmptyClipboard();
				SetClipboardData(cfRTF, glob);
				CloseClipboard();
			}
		}
		else {
			string txt = GetTextFormat();
			const char* data = txt.c_str();
			HGLOBAL glob = GlobalAlloc(GMEM_FIXED, txt.length());
			memcpy(glob, data, txt.length() * sizeof(char));
			if (OpenClipboard()) {
				EmptyClipboard();
				SetClipboardData(CF_TEXT, glob);
				CloseClipboard();
			}
		}
	}
}

void CResultView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu *pContextMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(2);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);		
}

// CDNAssistView diagnostics

#ifdef _DEBUG
void CResultView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CResultView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CResultDoc* CResultView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResultDoc)));
	return (CResultDoc*) m_pDocument;
}
#endif //_DEBUG

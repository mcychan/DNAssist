
// AlignView.cpp : implementation of the CAlignView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "AlignDoc.h"
#include "resource.h"
#include "AlignView.h"
#include "RichTextFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAlignView

IMPLEMENT_DYNCREATE(CAlignView, CScrollView)

BEGIN_MESSAGE_MAP(CAlignView, CScrollView)
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

// CAlignView construction/destruction

CAlignView::CAlignView()
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	identicalcutoff = alignOptionsBuffer->percentageidentical;
	conservedcutoff = alignOptionsBuffer->percentageconserved;
	identicalcolor = alignOptionsBuffer->identicalbackgroundColor;
	conservedcolor = alignOptionsBuffer->conservedbackgroundColor;
	conservedpencolor = alignOptionsBuffer->conservedforegroundColor;
	identicalpencolor  = alignOptionsBuffer->identicalforegroundColor;
	left_offset = 0;
	top_offset = 0;
	titlestart = 0;
	firstpositionstart = 0;
	sequencestart = 0;
	secondpositionstart = 0;
	cumulatedwheelrotation = 0;
	m_colorAddrBkgnd = ::GetSysColor(COLOR_BTNFACE);
}

CAlignView::~CAlignView()
{
}

BOOL CAlignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void CAlignView::OnInitialUpdate()
{	
	CScrollView::OnInitialUpdate();	

	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);	
}

void CAlignView::UpdateCtrlStatus()
{
	const CString BLANK = _T("");
	for (int i = 1; i<5; ++i)
		theApp.m_pStatusBar->SetPaneText(i, BLANK);
}

void CAlignView::OnSetFocus(CWnd* pOldWnd)
{
	// take care of caret
	CScrollView::OnSetFocus(pOldWnd);
	SetScrollSizes(MM_TEXT, CSize(printsize_pixels.cx, printsize_pixels.cy));
	UpdateCtrlStatus();
}

BOOL CAlignView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_TRACKPOS;

	if (LOBYTE(nScrollCode) == SB_THUMBTRACK) {
		GetScrollInfo(SB_HORZ, &info);
		nPos = info.nTrackPos;
	}

	if (HIBYTE(nScrollCode) == SB_THUMBTRACK) {
		GetScrollInfo(SB_VERT, &info);
		nPos = info.nTrackPos;
	}

	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CAlignView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
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
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;

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
BOOL CAlignView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CAlignView::SetPageLayout()
{
	GetCurrentFont();
	sequencerowlength = GetLengthOfOneSequenceRow();
	TruncateTitles();
	numberofsinglesequencerows = GetNumberOfRows();
	AssignSequenceNumbers();
	GetStartOfPrintItems();
	Paginate();
}

CFont* CAlignView::GetCurrentFont()
{
	font = make_unique<CFont>();
	auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
	font->CreateFont(editorFontTransferBuffer->facesize * theApp.GetSystemScaleFactor(), 0, 0, 0, editorFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, theApp.printFontFace);
	return font.get();
}

CPoint CAlignView::GetCharacterDimension(CDC* pDC)
{
	pDC->SelectObject(font.get());
	TEXTMETRIC localmetrics;
	pDC->GetTextMetrics(&localmetrics);
	//dimension of character in device context
	characterdimension.cx = (localmetrics.tmAveCharWidth + localmetrics.tmMaxCharWidth) / 2;
	characterdimension.cy = localmetrics.tmHeight + localmetrics.tmExternalLeading;
	return characterdimension;
}

int CAlignView::GetLengthOfOneSequenceRow(CDC* pDC)
{
	CAlignDoc* pDoc = GetDocument();
	int length = GetRowDimension(pDC).x;
	int tagwords = pDoc ->GetLongestNameString() + 2 * pDoc->GetLongestPositionString(); // size one either side
	sequencerowlength = length - tagwords - 4;
	return sequencerowlength;
}

void CAlignView::GetStartOfPrintItems()
{
	CAlignDoc* pDoc = GetDocument();
	titlestart = 0;
	firstpositionstart = titlestart + pDoc->GetLongestNameString() + 2;
	sequencestart = firstpositionstart + pDoc->GetLongestPositionString() + 1;
	secondpositionstart = sequencestart + GetLengthOfOneSequenceRow() + 1;
}

void CAlignView::AssignSequenceNumbers()
{
	CAlignDoc* pDoc = GetDocument();
	int alignmentlength = pDoc->GetLengthOfSequence();
	int numberofsequences = pDoc->GetNumberOfSequences();
	sequencenumbers.resize(numberofsequences);
	for(int x = 0; x < numberofsequences; x++) {
		string* stringpointer = pDoc->GetAlignment(x);
		if(stringpointer == nullptr)
			return;

		int number = 0;
		vector<int> row(numberofsinglesequencerows);
		for(int y = 0; y < numberofsinglesequencerows; y++) {
			int offset =  alignmentlength - y * sequencerowlength < sequencerowlength ?
					alignmentlength - y * sequencerowlength : sequencerowlength;
			number = pDoc->GetSequenceNumber(stringpointer->substr(y * sequencerowlength), offset, number);
			row[y] = number;
		}
		sequencenumbers[x] = row;
	}
}

void CAlignView::TruncateTitles()
{
	CAlignDoc* pDoc = GetDocument();
	int numberofsequences = pDoc->GetNumberOfSequences();
	size_t maxtitle = rowdimension.cx / 2;
	int titlelength = pDoc->GetLongestNameString();
	if(titlelength > maxtitle) {
		for(int x = 0; x < numberofsequences; x++) {
			CString* name = pDoc->GetSeqName(x);
			if(name == nullptr)
				return;
			if(name->GetLength() > maxtitle) {
				*name = name->Left(maxtitle - 3);
				name->Append( _T("..."));
			}
		}
	}
}

CPoint CAlignView::GetPageSizeInInches(CDC* pDC)
{
	if(pDC->IsPrinting()) {
		POINT ptPaper;
		pDC->Escape(GETPHYSPAGESIZE, NULL, NULL, &ptPaper);
		pagesize_pixelsperinch.cx = pDC->GetDeviceCaps(LOGPIXELSX);
		pagesize_pixelsperinch.cy = pDC->GetDeviceCaps(LOGPIXELSY);
		pagesize_inches.cx = (ptPaper.x + (0.5 * pagesize_pixelsperinch.cx)) / pagesize_pixelsperinch.cx;
		pagesize_inches.cy = (ptPaper.y + (0.5 * pagesize_pixelsperinch.cy)) / pagesize_pixelsperinch.cy;
	}
	else {
		//	"Generic" page size if there is no printer
		pagesize_inches.cx = 8;
		pagesize_inches.cy = 11;
	}
	return pagesize_inches;
}

CRect CAlignView::GetPrintMargins(CDC* pDC)
{
	if(pDC->IsPrinting()) {
		POINT ptMargin, ptPaper;
		// Start by getting the dimensions of the unprintable part of the
		// page (in device units). GETPRINTINGOFFSET will tell us the left
		// and upper unprintable area.
		pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &ptMargin);
		pDC->Escape(GETPHYSPAGESIZE, NULL, NULL, &ptPaper);
		return CRect(ptMargin.x, ptMargin.y, ptPaper.x - ptMargin.x, ptPaper.y - ptMargin.y);
	}
	return CRect(1, 1, 1, 1);
}

CPoint CAlignView::GetPagePixelsPerInch(CDC* pDC)
{
	pDC->SetMapMode(MM_TEXT);
	pagesize_pixelsperinch.cx = pDC->GetDeviceCaps(LOGPIXELSX);
	pagesize_pixelsperinch.cy = pDC->GetDeviceCaps(LOGPIXELSY);
	return pagesize_pixelsperinch;
}

CPoint CAlignView::GetPrintSizeInPixels(CDC* pDC)
{
	GetPageSizeInInches(pDC);
	GetPrintMargins(pDC);
	GetPagePixelsPerInch(pDC);
	printsize_pixels.cx = (pagesize_inches.cx - margin_inches.left - margin_inches.right) * pagesize_pixelsperinch.cx;
	printsize_pixels.cy = (pagesize_inches.cy - margin_inches.top - margin_inches.bottom) * pagesize_pixelsperinch.cy;
	left_offset = margin_inches.left * pagesize_pixelsperinch.cx;
	top_offset = margin_inches.top * pagesize_pixelsperinch.cy;
	return printsize_pixels;
}

CPoint CAlignView::GetRowDimension(CDC* pDC)
{
	GetPrintSizeInPixels(pDC);
	GetCharacterDimension(pDC);
	rowdimension.cx = printsize_pixels.cx / characterdimension.cx;
	rowdimension.cy = printsize_pixels.cy / characterdimension.cy;
	return rowdimension;
}

int CAlignView::GetLengthOfOneSequenceRow()
{
	CAlignDoc* pDoc = GetDocument();
	int length = rowdimension.cx;
	int tagwords = pDoc->GetLongestNameString() + 2 * pDoc->GetLongestPositionString(); // size one either side
	sequencerowlength = length - tagwords - 4;
	return sequencerowlength;
}

//	Returns the number of rows ONE sequence string will fit into
int CAlignView::GetNumberOfRows()
{
	CAlignDoc* pDoc = GetDocument();
	int lengthOfOneSequenceRow = GetLengthOfOneSequenceRow();
	return (pDoc->GetLengthOfSequence() + lengthOfOneSequenceRow - 1) / lengthOfOneSequenceRow;
}

int CAlignView::GetTotalNumberOfRows()
{
	int number = GetNumberOfRows();
	return number * (GetDocument()->GetNumberOfSequences() + 1);
}

CSize CAlignView::ConvertPaintRectToRowNumbers(CRect& rect)
{
	CSize rownumbers;
	rownumbers.cx = (rect.top - characterdimension.cy + 1) / characterdimension.cy < 0 ?
				     0 : (rect.top - characterdimension.cy + 1) / characterdimension.cy;
	rownumbers.cy = (rect.bottom + characterdimension.cy - 1) / characterdimension.cy;
	return rownumbers;
}

CSize CAlignView::ConvertRowNumberToRowGroupNumber(CSize row)
{
	CAlignDoc* pDoc = GetDocument();
	int numberofsequences = pDoc->GetNumberOfSequences();
	CSize rowgroup;
	rowgroup.cx = row.cx / (numberofsequences + 1);
	int maxgroups = GetNumberOfRows() - 1;
	int bottomrow = (row.cy + numberofsequences) / (numberofsequences + 1);
	rowgroup.cy = max(bottomrow, maxgroups);
	return rowgroup;
}

int CAlignView::GetNumberOfRowsThatFitOnOnePage()
{
	return printsize_pixels.cy / characterdimension.cy;
}

void CAlignView::Paginate()
{
	int rowgroupsperpage = GetNumberOfRowGroupsPerPage();
	int totalnumberofgroups = GetNumberOfRows();
	int x = 1;
	while(rowgroupsperpage * x < totalnumberofgroups) {
		pagebreaks.push_back(x * rowgroupsperpage - 1);
		x++;
	}
	if(pagebreaks.size() && pagebreaks[pagebreaks.size() - 1] < totalnumberofgroups)
		pagebreaks.push_back(totalnumberofgroups - 1);
	else if(pagebreaks.empty())
		pagebreaks.push_back(totalnumberofgroups - 1);
}

int CAlignView::GetNumberOfRowGroupsPerPage()
{
	return GetNumberOfRowsThatFitOnOnePage() / (GetDocument()->GetNumberOfSequences() + 1);
}

void CAlignView::SetModifiedFlag(BOOL bModified)
{
}

void CAlignView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetViewportOrg(CPoint(0, 0)); // force viewport origin to zero

	CAlignDoc* pDoc = GetDocument();
	if(!pDoc->IsCalculating()) {
		if (pDC->IsPrinting()) {
			pDC->SetMapMode(MM_LOENGLISH);
			pDC->SetMapMode(MM_ANISOTROPIC);
			CSize size;
			GetViewportExtEx(pDC->GetSafeHdc(), &size);
			SetViewportExtEx(pDC->GetSafeHdc(), size.cx, -size.cy, NULL);
		}

		if (rowdimension.cx <= 0 || rowdimension.cy <= 0)
			GetRowDimension(pDC);

		GetPageSizeInInches(pDC);
		SetPageLayout();
	}
}

void CAlignView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// get the current page number(for first page returns 1, for second - 2 and so on)
	size_t nCurPage = pInfo->m_nCurPage;
	int maxPg = GetTotalNumberOfPages();

	// Compute the number of pages to print.
	//
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(maxPg);
}

void CAlignView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CPoint point(20, -18);
	pDC->TextOut(point.x, point.y, _T("DNAssist"));
}

void CAlignView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
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

void CAlignView::PaintSeqData(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcClient(0, 0, 0, 0);
	GetClientRect(&rcClient);

	if (characterdimension.cx <= 0 || characterdimension.cy <= 0)
		GetCharacterDimension(pDC);
	CSize rowgroups = ConvertRowNumberToRowGroupNumber(ConvertPaintRectToRowNumbers(rcClient));
	pDC->SelectObject(*font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	CAlignDoc* pDoc = GetDocument();
	int alignmentlength = pDoc->GetLengthOfSequence();
	int numberofsequences = pDoc->GetNumberOfSequences();

	for(int y = rowgroups.cx; y <= rowgroups.cy; y++) {
		for(int x = 0; x < numberofsequences; x++) {			
	   		int entry = pDoc->GetSortOrder(x) - 1;
	      	int y_position = characterdimension.cy * y * (numberofsequences + 1) + x * characterdimension.cy;
			CString* pName = pDoc->GetSeqName(entry);
			string* pAlignment = pDoc->GetAlignment(x);
			if (pName == nullptr || pAlignment == nullptr)
				continue;

	      	pDC->TextOut(titlestart * characterdimension.cx, y_position, *pName);
	      	if(!y)
	      		pDC->TextOut(firstpositionstart * characterdimension.cx, y_position, _T("1"));
	      	else {
	      		CString numbersequence;
	      		numbersequence.Format(_T("%d"), 1 + sequencenumbers[x][y - 1]);
	      		pDC->TextOut(firstpositionstart * characterdimension.cx, y_position, numbersequence);
	      	}
	      	int numberofcharacters = min(alignmentlength - y * sequencerowlength, sequencerowlength);
	      	for(int z = 0; z < numberofcharacters; z++) {
	   			if(pDoc->GetBgColor(x, y * sequencerowlength + z) == 1) {
	   				pDC->SetBkColor(identicalcolor);
	   				pDC->SetTextColor(identicalpencolor);
	   				pDC->SetBkMode(OPAQUE);
	   			}
	      		else if(pDoc->GetBgColor(x, y * sequencerowlength + z) == 2) {
	      			pDC->SetBkColor(conservedcolor);
	      			pDC->SetTextColor(conservedpencolor);
	      			pDC->SetBkMode(OPAQUE);
	      		}
	            else {	            	
					pDC->SetBkMode(TRANSPARENT);
	            	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	            }
	   			string temp = pAlignment->substr(y * sequencerowlength + z);
	         	pDC->TextOut((z + sequencestart) * characterdimension.cx, y_position, CString(temp.c_str()), 1);
	      	}
	      	pDC->SetBkMode(TRANSPARENT);
	      	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	      	CString numbersequence;
	      	numbersequence.Format(_T("%d"), 1 + sequencenumbers[x][y]);
	      	pDC->TextOut((sequencestart + numberofcharacters + 1) * characterdimension.cx, y_position, numbersequence);
		}
	}
}

void CAlignView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	PrintHeader(pDC, pInfo);

	int maxPg = GetTotalNumberOfPages();
	CAlignDoc* pDoc = GetDocument();
	int alignmentlength = pDoc->GetLengthOfSequence();
	int numberofsequences = pDoc->GetNumberOfSequences();

	// Compute the number of pages to print.
	//
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(maxPg);
	int currentPage = pInfo->m_nCurPage;
	int rowgroup_at_top = currentPage > 1 ? pagebreaks[currentPage - 2] + 1 : 0;
	for(int y = rowgroup_at_top; y <= pagebreaks[currentPage - 1]; y++) {
		for(int x = 0; x < numberofsequences; x++) {
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			int entry = pDoc->GetSortOrder(x) - 1;
			int y_position = characterdimension.cy * (y - rowgroup_at_top) * (numberofsequences + 1) + x * characterdimension.cy + top_offset;
			pDC->TextOut(titlestart * characterdimension.cx + left_offset, y_position, *(pDoc->GetSeqName(entry)));
			if(!y)
				pDC->TextOut(firstpositionstart * characterdimension.cx + left_offset, y_position, _T("1"));
			else {
				CString numbersequence;
				numbersequence.Format(_T("%d"), 1 + sequencenumbers[x][y - 1]);
				pDC->TextOut(firstpositionstart * characterdimension.cx, y_position, numbersequence);
			}
			int numberofcharacters = min(alignmentlength - y * sequencerowlength, sequencerowlength);
			for(int z = 0; z < numberofcharacters; z++) {
				if(pDoc->GetBgColor(x, y * sequencerowlength + z) == 1) {
					pDC->SetBkColor(identicalcolor);
					pDC->SetTextColor(identicalpencolor);
					pDC->SetBkMode(OPAQUE);
				}
				else if(pDoc->GetBgColor(x, y * sequencerowlength + z) == 2) {
					pDC->SetBkColor(conservedcolor);
					pDC->SetTextColor(conservedpencolor);
					pDC->SetBkMode(OPAQUE);
				}
				else {
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
				}
				string temp = pDoc->GetAlignment(x)->substr(y * sequencerowlength + z);
				pDC->TextOut((z + sequencestart) * characterdimension.cx, y_position, CString(temp.c_str()), 1);
			}
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			CString numbersequence;
			numbersequence.Format(_T("%d"), 1 + sequencenumbers[x][y]);
			pDC->TextOut((sequencestart + numberofcharacters + 1) * characterdimension.cx + left_offset, y_position, numbersequence);
		}
	}

	PrintFooter(pDC, pInfo);
}

void CAlignView::OnDraw(CDC* pDCView)
{
	ASSERT(MM_TEXT == pDCView->GetMapMode());
	ASSERT(CPoint(0, 0) == pDCView->GetViewportOrg());
	if (GetDocument()->IsCalculating()) {
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

	int pageHeight = ceil(GetTotalNumberOfRows() * characterdimension.cy);
	if (m_pDC.get() == NULL) {		
		m_pDC = make_unique<CMemoryDC>(pDCView, printsize_pixels.cx, pageHeight);
		SetScrollSizes(MM_TEXT, CSize(printsize_pixels.cx, pageHeight));
		
		//draw background		
		CRect rcDoc(0, 0, printsize_pixels.cx, pageHeight);
		m_pDC->FillSolidRect(rcDoc, m_colorAddrBkgnd);

		PaintSeqData(m_pDC.get(), NULL);

		ReleaseDC(m_pDC.get());		
	}

	CPoint point = GetScrollPosition();
	if(characterdimension.cy <= 0) {
		TEXTMETRIC localmetrics;
		pDCView->GetTextMetrics(&localmetrics);
		characterdimension.cy = localmetrics.tmHeight + localmetrics.tmExternalLeading;
	}
	const size_t nScrolledLine = floor(point.y / characterdimension.cy);

	const LONG xPos = point.x;           // horizontal scroll position
	const LONG yPos = nScrolledLine * characterdimension.cy;           // vertical scroll position
													   // source and destination cordinates and sizes
	LONG nDstWidth, nDstHeight;

	if (printsize_pixels.cx > cx)
		nDstWidth = printsize_pixels.cx - xPos;
	else
		nDstWidth = printsize_pixels.cx;

	if (pageHeight > cy)
		nDstHeight = pageHeight - yPos;
	else
		nDstHeight = pageHeight;

	pDCView->BitBlt(rcClient.left, rcClient.top, nDstWidth, nDstHeight,
		m_pDC.get(),
		xPos, yPos,
		SRCCOPY);
	pDCView->ExcludeClipRect(rcClient.left, rcClient.top, nDstWidth, nDstHeight);
	pDCView->FillSolidRect(rcClient, m_colorAddrBkgnd);
	ReleaseDC(pDCView);
}

CRect CAlignView::GetClientPosition()
{
	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);
	int screenwidth = rcClient.Width();
	int screenheight = rcClient.Height();
	int wantedwindowheight = GetTotalNumberOfRows() * pagesize_pixelsperinch.cy > 0.8 * screenheight ? 0.8 * screenheight : GetTotalNumberOfRows() * pagesize_pixelsperinch.cy;
	CRect rect((screenwidth-pagesize_inches.cx * pagesize_pixelsperinch.cx) / 2, (screenheight - wantedwindowheight) / 2,
			pagesize_inches.cx * pagesize_pixelsperinch.cx > 0.8 * screenwidth ? 0.8 * screenwidth : pagesize_inches.cx * pagesize_pixelsperinch.cx,
			pagesize_inches.cy * pagesize_pixelsperinch.cy > 0.8 * screenheight ? 0.8 * screenheight : pagesize_inches.cy * pagesize_pixelsperinch.cy);

	return rect;
}

BOOL CAlignView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void CAlignView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	if(selection) {
		selection = false;
		Invalidate();
	}
}

void CAlignView::OnEditSelectAll()
{
	selection = true;
	Invalidate();
}

string CAlignView::GetTextFormat()
{
	int lastrow = GetNumberOfRows();
	CAlignDoc* pDoc = GetDocument();
	int alignmentlength = pDoc->GetLengthOfSequence();
	int numberofsequences = pDoc->GetNumberOfSequences();
	string temparray;

	for(int y = 0; y < lastrow; y++) {
		for(int x = 0; x < numberofsequences; x++) {
			//	Write the title
			int entry = pDoc->GetSortOrder(x) - 1;
			CString* pData = pDoc->GetSeqName(entry);
			string data = (LPCSTR) CStringA(*pData);
			temparray += data;

			//	Pad with spaces
			string temp;
			int numberofspaces = firstpositionstart - pData->GetLength();
			temp.assign(numberofspaces, 32);
			temparray += temp;

			//	Write the sequence position
			if(!y) {
				temparray += "1";
				numberofspaces = sequencestart - firstpositionstart - 1;	//	number of spaces to pad
			}
			else {
				string temp2 = to_string(1 + sequencenumbers[x][y - 1]);
				temparray += temp2;
				numberofspaces = sequencestart - firstpositionstart - temp2.length(); //	number of spaces to pad
			}

			//	Pad with spaces
			temp.clear();
			temp.assign(numberofspaces, 32);
			temparray += temp;

			// Write the sequence
			// First write the new font and background colors if that have changed
			int numberofcharacters = min(alignmentlength - y * sequencerowlength, sequencerowlength);
			string temp2 = pDoc->GetAlignment(x)->substr(y * sequencerowlength, numberofcharacters);
			temparray += temp2 + " ";//	Add one space
			temp2 = to_string(sequencenumbers[x][y]);
			temparray += temp2 + (char) 13 + (char) 10;
		}
		temparray += (char) 13;
		temparray += (char) 10;
	}

	return temparray;
}

string CAlignView::GetRTFText()
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
	rtfobject.GetColorTable(identicalpencolor, conservedpencolor, identicalcolor, conservedcolor);
	int oldcolorindex = 0;
	int lastrow = GetNumberOfRows();

	CAlignDoc* pDoc = GetDocument();
	int alignmentlength = pDoc->GetLengthOfSequence();
	int numberofsequences = pDoc->GetNumberOfSequences();

	for(int y = 0; y < lastrow; y++) {
		for(int x = 0; x < numberofsequences; x++) {
			//	Write the title
			int entry = pDoc->GetSortOrder(x) - 1;
			CString* pData = pDoc->GetSeqName(entry);
			string data = (LPCSTR) CStringA(*pData);
			rtfobject.AddString(data);

			//	Pad with spaces
			int numberofspaces = firstpositionstart - data.length();
			string temp;
			temp.assign(numberofspaces, (char) 32);
			rtfobject.AddString(temp);

			//	Write the sequence position
			if(!y) {
				rtfobject.AddString("1");
				numberofspaces = sequencestart - firstpositionstart - 1;	//	number of spaces to pad
			}
			else {
				string temp2 = to_string(1 + sequencenumbers[x][y - 1]);
				rtfobject.AddString(temp2);
				numberofspaces = sequencestart - firstpositionstart - temp2.length(); //	number of spaces to pad
			}

			// Pad with spaces
			temp.clear();
			temp.assign(numberofspaces, (char) 32);
			rtfobject.AddString(temp);

			// Write the sequence
			// First write the new font and background colors if that have changed
			int numberofcharacters = min(alignmentlength - y * sequencerowlength, sequencerowlength);
			for(int z = 0; z < numberofcharacters; z++) {
				short currentcolorindex = pDoc->GetBgColor(x, y * sequencerowlength + z);
				if(currentcolorindex != oldcolorindex) {
					if(currentcolorindex == 0) {
						rtfobject.SetForeGroundColor(1);
						rtfobject.SetBackGroundColor(2);
					}
					else if(currentcolorindex == 1) {
						rtfobject.SetForeGroundColor(3);
						rtfobject.SetBackGroundColor(5);
					}
					else if(currentcolorindex == 2) {
						rtfobject.SetForeGroundColor(4);
						rtfobject.SetBackGroundColor(6);
					}
				}
				oldcolorindex = currentcolorindex;
				rtfobject.AddCharacter(pDoc->GetAlignment(x)->at(y * sequencerowlength + z));
			}
			//	Make sure the font is black on white
			oldcolorindex = 0;
			rtfobject.SetForeGroundColor(1);
			rtfobject.SetBackGroundColor(2);
			rtfobject.AddString(" ");//	Add one space
			string temp2 = to_string(sequencenumbers[x][y]);
			rtfobject.AddString(temp2);
			rtfobject.AddLineBreak();
		}
		rtfobject.AddLineBreak();
	}
	rtfobject.Finalise();
	rtfobject.GetRichTextFormat(rtf);
	font.DeleteObject();
	return rtf;
}

void CAlignView::OnEditCopy()
{
	
	CLIPFORMAT cfRTF = RegisterClipboardFormat(_T("Rich Text Format"));
	if(cfRTF) {		
		string rtf = GetRTFText();
		HGLOBAL glob = GlobalAlloc(GMEM_FIXED, rtf.length());
		const char* data = rtf.c_str();
		memcpy(glob, data, rtf.length() * sizeof(char));
		if (OpenClipboard()) {
			EmptyClipboard();
			SetClipboardData(cfRTF, glob);
			CloseClipboard();
		}
	}
	else {		
		string txt = GetTextFormat();
		HGLOBAL glob = GlobalAlloc(GMEM_FIXED, txt.length());
		const char* data = txt.c_str();
		memcpy(glob, data, txt.length() * sizeof(char));
		if (OpenClipboard()) {
			EmptyClipboard();
			SetClipboardData(CF_TEXT, glob);
			CloseClipboard();
		}
	}
}

void CAlignView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_CONTEXT_MENU));
	auto pContextMenu = menu.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

// CAlignView diagnostics

#ifdef _DEBUG
void CAlignView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAlignView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAlignDoc* CAlignView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAlignDoc)));
	return (CAlignDoc*) m_pDocument;
}
#endif //_DEBUG

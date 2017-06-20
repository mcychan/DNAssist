
// DNAssistView.cpp : implementation of the CDrewPlotView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "resource.h"
#include "DrewPlotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrewPlotView

IMPLEMENT_DYNCREATE(CDrewPlotView, CScrollView)

BEGIN_MESSAGE_MAP(CDrewPlotView, CScrollView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
END_MESSAGE_MAP()

// CDrewPlotView construction/destruction
CDrewPlotView::CDrewPlotView()
{
	mapmode = MM_ANISOTROPIC;
	dottypen = make_unique<CPen>(PS_DOT | PS_COSMETIC, 1, m_colorSeqDataText);
	framepen = make_unique<CPen>(PS_SOLID | PS_COSMETIC, 2, m_colorSeqDataText);
	plotPen = make_unique<CPen>(PS_SOLID | PS_COSMETIC, 1, m_colorSeqDataText);
	cumulatedwheelrotation = 0;
	horpixels = 0;
	vertpixels = 0;
	pageprintwidth = 0;
	pageprintheight = 0;
	width = 0;
	height = 0;
	blockheight = 0;
	blockwidth = 0;
	blocksperpage = 0;
	leftmargin = 1;
	rightmargin = 1;
	topmargin = 1;
	bottommargin = 1;
	leftoffset = 0;
	selection = false;
	sequence_length = 0;
	pagebreaks.clear();
	m_colorSeqDataBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_colorSeqDataText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_colorAddrBkgnd = ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION);
}

BOOL CDrewPlotView::LoadData()
{	
	CDrewPlotDoc* pDoc = GetDocument();	
	auto data = pDoc->GetData();
	if (data->empty())
		return FALSE;
	
	start = pDoc->m_nStartBase;
	sequence_length = pDoc->GetLength(); //length of the sequence to list
	plottype = pDoc->GetPlotType();

	minimum = (int)(*min_element(data->begin(), data->end()) - 0.999);
	if (minimum > 0)
		minimum = 0;
	maximum = (int)(*max_element(data->begin(), data->end()) + 0.999);
	if (maximum < 0)
		maximum = 0;

	Paginate();
	Yscale = (float)block_height / ((float)(maximum - minimum));
	return TRUE;
}

CDrewPlotView::~CDrewPlotView()
{
}

BOOL CDrewPlotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs	
	cs.dwExStyle |= WS_EX_COMPOSITED;

	return CScrollView::PreCreateWindow(cs);
}

void CDrewPlotView::OnInitialUpdate()
{	
	CScrollView::OnInitialUpdate();	

	HFONT hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	CFont * gf = CFont::FromHandle(hf);
	gf->GetLogFont(&labellogfont);
	labellogfont.lfWeight = FW_BOLD;
	labellogfont.lfOutPrecision = OUT_TT_PRECIS;
	labelfont = make_unique<CFont>();
	labelfont->CreateFontIndirect(&labellogfont);

	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDrewPlotView::UpdateCtrlStatus()
{
	const CString BLANK = _T("");
	for (int i = 1; i<5; ++i)
		theApp.m_pStatusBar->SetPaneText(i, BLANK);
}

void CDrewPlotView::OnSetFocus(CWnd* pOldWnd)
{
	// take care of caret
	CScrollView::OnSetFocus(pOldWnd);
	SetScrollSizes(MM_TEXT, CSize(width, height));
	UpdateCtrlStatus();
}

BOOL CDrewPlotView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
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

BOOL CDrewPlotView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
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
BOOL CDrewPlotView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// CDrewPlotView printing

BOOL CDrewPlotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrewPlotView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CDrewPlotDoc* pDoc = GetDocument();
	if (sequence_length == 0) {
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	CScrollView::OnPrepareDC(pDC, pInfo);
	pDC->SetViewportOrg(CPoint(0, 0)); // force viewport origin to zero
	if (pDC->IsPrinting()) {
		pDC->SetMapMode(MM_LOENGLISH);
		pDC->SetMapMode(MM_ANISOTROPIC);
		CSize size;
		GetViewportExtEx(pDC->GetSafeHdc(), &size);
		SetViewportExtEx(pDC->GetSafeHdc(), size.cx, -size.cy, NULL);
	}
	else
		pDC->SetMapMode(MM_TEXT);
	
	if (sequence_length == 0 && !pDoc->IsCalculating()) {
		pageprintwidth = ::MulDiv(pDC->GetDeviceCaps(PHYSICALWIDTH), 1440, pDC->GetDeviceCaps(LOGPIXELSX));
		pageprintheight = ::MulDiv(pDC->GetDeviceCaps(PHYSICALHEIGHT), 1440, pDC->GetDeviceCaps(LOGPIXELSY));
		SetFontDimensions(pDC);
		if (!LoadData())
			return;

		pDC->GetTextMetrics(&textmetric);
		//dimension of character in device context
		CSize char_dimension = pDC->GetTextExtent(_T("W"), 1);
		character_dimension.cx = char_dimension.cx;
		character_dimension.cy = textmetric.tmHeight;

		//dimension of row cell in device context
		rowcell_dimension.cx = char_dimension.cx;
		rowcell_dimension.cy = textmetric.tmHeight + textmetric.tmExternalLeading;

		CRect rcClient(0, 0, 0, 0);
		GetClientRect(rcClient);

		//width = max(rcClient.right, sequence_length * point_dimension + block_left + block_left_gap + block_right_gap);             // source document width
		//height = max(rcClient.bottom, pagebreaks.empty() ? 100 : pagebreaks[pagebreaks.size() - 1]);             // source document height

		width = rcClient.right - block_left_gap - block_right_gap; // source document width
		height = (block_height + block_gap) * number_blocks; // source document height
		if(number_blocks > 1)
			height += block_height;
		SetScrollSizes(MM_TEXT, CSize(width, height));

		SetGraphicBlockDimension();
	}	

	UINT nCurPage = 1;
	if(pInfo)
		nCurPage = pInfo->m_nCurPage;

	//dimension of row cell in device context
	rowcell_dimension.cx = textmetric.tmAveCharWidth;
	rowcell_dimension.cy = textmetric.tmHeight + textmetric.tmExternalLeading;

	if (pDC->IsPrinting()) {
		start_and_end_blocks.x = nCurPage > 1 ? pagebreaks[nCurPage-1] : 0;
		start_and_end_blocks.y = pagebreaks[nCurPage];
	}
	else {		
		start_and_end_blocks = GetStartAndEndBlocks();
	}
}

void CDrewPlotView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CPoint point(20, -18);
	pDC->TextOut(point.x, point.y, _T("DNAssist"));
}

void CDrewPlotView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{	
	CPoint point(0, -pageprintheight + 638);

	//draw page number
	CString   sTemp;
	sTemp.Format(_T("%d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
	CSize size = pDC->GetTextExtent(sTemp);
	point.x += pageprintwidth * 0.544f - size.cx; // Move 4.5 inches right
	pDC->TextOut(point.x, point.y, sTemp); // right-justified
}

void CDrewPlotView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// get the current page number(for first page returns 1, for second - 2 and so on)
	size_t nCurPage = pInfo->m_nCurPage;
	SetFontDimensions(pDC);
	SetGraphicBlockDimension();

	pDC->SetMapMode(mapmode);
	pDC->SelectObject(font.get());
	pDC->GetTextMetrics(&textmetric);

	//dimension of character in device context
	CSize char_dimension = pDC->GetTextExtent(_T("W"),1);
	character_dimension.cx = char_dimension.cx;
	character_dimension.cy = textmetric.tmHeight;
	Paginate();

	// Compute the number of pages to print.
	//
	short nMaxPage = max(1, GetTotalNumberOfPages());

	pInfo->SetMaxPage(nMaxPage);
}

void CDrewPlotView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CDrewPlotDoc* pDoc = GetDocument();
	auto data = pDoc->GetData();
	PrintHeader(pDC, pInfo);
	int remain = sequence_length;
	int horizontallabelinterval = GetHorizontalLabelInterval(pDC);
	int verticallabelinterval = GetVerticalLabelInterval();
	int right_margin;
	pDC->SelectObject(framepen.get());

	CFont bold;
	labellogfont.lfEscapement = 900;
	labellogfont.lfWeight = FW_BOLD;
	bold.CreateFontIndirect(&labellogfont);

	for(int x = start_and_end_blocks.x; x < start_and_end_blocks.y; x++) {
		pDC->SelectObject(font.get());
		POINT ptMargin;

		// Start by getting the dimensions of the unprintable part of the
		// page (in device units). GETPRINTINGOFFSET will tell us the left
		// and upper unprintable area.
		pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &ptMargin);

		int leftmargin = ptMargin.x < 0 ? 1000 : ptMargin.x;
		int topmargin = ptMargin.y < 0 ? 1000 : ptMargin.y;
		int leftoffset = ((float)((float)leftmargin/(float)1000)* pDC->GetDeviceCaps(LOGPIXELSX));
		int topoffset = ((float)((float)topmargin/(float)1000)* pDC->GetDeviceCaps(LOGPIXELSY));
		int bottom = (x - start_and_end_blocks.x + 1) * block_bottom;
		int top = bottom - block_height;

		int number_of_points_to_plot = min(remain - (x * points_per_block), points_per_block);
		right_margin = (number_of_points_to_plot-1) * point_dimension+block_left + block_left_gap + block_right_gap;
		pDC->SelectObject(framepen.get());
		pDC->MoveTo(block_left + leftoffset, bottom + topoffset);
		pDC->LineTo(block_left + leftoffset, top + topoffset);
		pDC->LineTo(right_margin + leftoffset, top + topoffset);
		pDC->LineTo(right_margin + leftoffset, bottom + topoffset);
		pDC->LineTo(block_left + leftoffset, bottom + topoffset);

		pDC->SelectObject(dottypen.get());
		if(0 < maximum && 0 > minimum) {
			pDC->MoveTo(block_left + leftoffset, (int)(minimum * Yscale) + bottom + topoffset);
			pDC->LineTo(right_margin + leftoffset, (int)(minimum * Yscale) + bottom + topoffset);
		}

		pDC->SelectObject(plotPen.get());
		for(int y = x * points_per_block; y < x * points_per_block + number_of_points_to_plot; y++) {
			float value1 = data->at(y);
			int value2 = (int)(-Yscale * (value1 - minimum)) + bottom;
			if(y == x * points_per_block)
				pDC->MoveTo(block_left + block_left_gap + (y - x * points_per_block) * point_dimension + leftoffset, value2 + topoffset);
			else
				pDC->LineTo(block_left + block_left_gap + (y - x * points_per_block) * point_dimension + leftoffset, value2 + topoffset);
		}
		for(int a = x * points_per_block; a < x * points_per_block + number_of_points_to_plot; a++) {
			int left = block_left + block_left_gap + (a - x * points_per_block) * point_dimension;
			int label = start + a;
			if (label % 100 == 0) {
				pDC->MoveTo(left + leftoffset,  bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom  - tick100 + topoffset);
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top + tick100 + topoffset);
			}
			else if (label % 10 == 0) {
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top + tick10 + topoffset);
				pDC->MoveTo(left + leftoffset, bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom - tick10 + topoffset);
			}
			else {
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top  + tick1 + topoffset);
				pDC->MoveTo(left + leftoffset, bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom - tick1 + topoffset);
			}
			if(label%horizontallabelinterval == 0) {
				int wordwidth = log10(label + 0.0f)+1;
				CString word;
				word.Format(_T("%d"), label);
				CSize ext = pDC->GetTextExtent(word);
				pDC->TextOut(left-0.5 * ext.cx + leftoffset, bottom + 0.25 * ext.cy + topoffset, word);
			}
		}

		for(int vertick = minimum; vertick <= maximum; vertick++) {
			pDC->MoveTo(block_left + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
			pDC->LineTo(block_left + tickside + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
			pDC->MoveTo(right_margin + leftoffset, -Yscale * (vertick-minimum) + bottom + topoffset);
			pDC->LineTo(right_margin - tickside + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
		}

		int verlabel = minimum;
		float labelwidth = abs(minimum + 0.0f) >= abs(maximum + 0.0f) ? abs(minimum + 0.0f) : abs(maximum + 0.0f);
		if(labelwidth <= 0)
			labelwidth = 1.0f;
		else
			labelwidth = log10(labelwidth);

		while(verlabel <= maximum) {
			CString ver;
			ver.Format(_T("%d"), verlabel);
			CSize vext = pDC->GetTextExtent(ver);
			pDC->TextOut(block_left - vext.cx - 5 + leftoffset, -Yscale * (verlabel - minimum) - 0.5 * vext.cy + bottom + topoffset , ver);
		    verlabel += verticallabelinterval;
		}

		//	Write the X-axis label
		pDC->SelectObject(font.get());

		CSize Xlabelsize = pDC->GetTextExtent(pDoc->Xaxistitle);
		// Make sure the Xaxistitle will not shift past the block's left margin
		int xposition = ((right_margin - block_left - Xlabelsize.cx) / 2) < 0 ? block_left : block_left + ((right_margin - block_left - Xlabelsize.cx)/2);
		pDC->TextOut(xposition + leftoffset, bottom + 1.5 * Xlabelsize.cy + topoffset , pDoc->Xaxistitle);

		//	Write the Y-axis label
		pDC->SelectObject(&bold);
		CSize axisext = pDC->GetTextExtent(pDoc->Yaxistitle);
		pDC->TextOut(block_left - 3 * axisext.cy + leftoffset, bottom - ((block_height - axisext.cx) / 2) + topoffset, pDoc->Yaxistitle);
	}

	bold.DeleteObject();
	PrintFooter(pDC, pInfo);
}

void CDrewPlotView::PaintSeqData(CDC* pDC)
{
	//draw background
	CRect rcDoc(0, 0, width, height);
	pDC->FillSolidRect(rcDoc, RGB(255, 255, 255));

	CDrewPlotDoc* pDoc = GetDocument();
	auto data = pDoc->GetData();	

	int remain = sequence_length;
	labellogfont.lfEscapement = 0;
	font = make_unique<CFont>();
	font->CreateFontIndirect(&labellogfont);
	int horizontallabelinterval = GetHorizontalLabelInterval(pDC);
	int verticallabelinterval = GetVerticalLabelInterval();

	pDC->SelectObject(framepen.get());

	CFont bold;
	labellogfont.lfEscapement = 900;
	labellogfont.lfWeight = FW_BOLD;
	bold.CreateFontIndirect(&labellogfont);

	for(int x = start_and_end_blocks.x; x < start_and_end_blocks.y; x++) {
		pDC->SelectObject(font.get());

		int bottom = (x + 1) * block_bottom - block_gap / 2;
		int top = bottom - block_height - tick10;
		int leftoffset = 0, topoffset = 0;

		int number_of_points_to_plot = min(remain - (x * points_per_block), points_per_block);
		int right_margin = (number_of_points_to_plot - 1) * point_dimension + block_left + block_left_gap + block_right_gap;
		pDC->SelectObject(framepen.get());
		pDC->MoveTo(block_left + leftoffset, bottom + topoffset);
		pDC->LineTo(block_left + leftoffset, top + topoffset);
		pDC->LineTo(right_margin + leftoffset, top + topoffset);
		pDC->LineTo(right_margin + leftoffset, bottom + topoffset);
		pDC->LineTo(block_left + leftoffset, bottom + topoffset);

		pDC->SelectObject(dottypen.get());
		if(0 < maximum && 0 > minimum) {
			pDC->MoveTo(block_left + leftoffset,(int)(minimum * Yscale)  +  bottom + topoffset);
			pDC->LineTo(right_margin + leftoffset, (int)(minimum * Yscale) + bottom + topoffset);
		}

		pDC->SelectObject(plotPen.get());
		for(int y = x * points_per_block; y < x * points_per_block + number_of_points_to_plot; y++) {
			float value1 = data->at(y);
			int value2 = (int)(-Yscale*(value1 - minimum)) + bottom;
			if(y == x * points_per_block)
				pDC->MoveTo(block_left + block_left_gap + (y - x * points_per_block) * point_dimension + leftoffset, value2 + topoffset);
			else
				pDC->LineTo(block_left + block_left_gap + (y - x * points_per_block) * point_dimension + leftoffset, value2 + topoffset);
		}

		for(int a = x * points_per_block; a < x * points_per_block + number_of_points_to_plot; a++) {
			int left = block_left + block_left_gap + (a - x * points_per_block) * point_dimension;
			int label = start+a;
			if (label % 100 == 0) {
				pDC->MoveTo(left + leftoffset, bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom - tick100 + topoffset);
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top + tick100 + topoffset);
			}
			else if (label%10 == 0) {
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top + tick10 + topoffset);
				pDC->MoveTo(left + leftoffset, bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom - tick10 + topoffset);
			}
			else {
				pDC->MoveTo(left + leftoffset, top + topoffset);
				pDC->LineTo(left + leftoffset, top + tick1 + topoffset);
				pDC->MoveTo(left + leftoffset, bottom + topoffset);
				pDC->LineTo(left + leftoffset, bottom - tick1 + topoffset);
			}
			if(label % horizontallabelinterval == 0) {
				int wordwidth = log10(label + 0.0f) + 1;
				CString word;
				word.Format(_T("%d"), label);
				CSize ext = pDC->GetTextExtent(word);
				pDC->TextOut(left - 0.5 * ext.cx + leftoffset, bottom + 0.25 * ext.cy + topoffset, word);
			}
		}

		for(int vertick = minimum; vertick <= maximum; vertick++) {
			pDC->MoveTo(block_left + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
			pDC->LineTo(block_left + tickside + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
			pDC->MoveTo(right_margin + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
			pDC->LineTo(right_margin - tickside + leftoffset, -Yscale * (vertick - minimum) + bottom + topoffset);
		}

		int verlabel = minimum;
		float labelwidth = abs(minimum + 0.0f) >= abs(maximum + 0.0f) ? abs(minimum + 0.0f) : abs(maximum + 0.0f);
		if(labelwidth <= 0)
			labelwidth = 1;
		else
			labelwidth = log10(labelwidth);

		while(verlabel <= maximum) {
			CString ver;
			ver.Format(_T("%d"), verlabel);
			CSize vext = pDC->GetTextExtent(ver);
			pDC->TextOut(block_left - vext.cx - 5 + leftoffset, -Yscale * (verlabel - minimum) - 0.5 * vext.cy + bottom + topoffset , ver);
			verlabel += verticallabelinterval;
		}

		//	Write the X-axis label
		pDC->SelectObject(font.get());
		CSize Xlabelsize = pDC->GetTextExtent(pDoc->Xaxistitle);
		//	Make  sure the Xaxistitle will not shift past the block's left margin
		int xposition = ((right_margin - block_left - Xlabelsize.cx) / 2) < 0 ? block_left : block_left + ((right_margin - block_left - Xlabelsize.cx) / 2);
		pDC->TextOut(xposition + leftoffset, bottom + 1.5 * Xlabelsize.cy + topoffset , pDoc->Xaxistitle);

		//	Write the Y-axis label
		pDC->SelectObject(&bold);
		CSize axisext = pDC->GetTextExtent(pDoc->Yaxistitle);
		pDC->TextOut(block_left - 3 * axisext.cy + leftoffset, bottom - ((block_height - axisext.cx) / 2) + topoffset, pDoc->Yaxistitle);
	}

	bold.DeleteObject();
}

void CDrewPlotView::OnDraw(CDC* pDCView)
{
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

	if (sequence_length > 0 && m_pDC.get() == NULL) {
		m_pDC = make_unique<CMemoryDC>(pDCView, width, height);

		PaintSeqData(m_pDC.get());
		ReleaseDC(m_pDC.get());
	}

	CPoint point = GetScrollPosition();
	if (character_dimension.cy <= 0)
		character_dimension.cy = textmetric.tmHeight + textmetric.tmExternalLeading;
	const size_t nScrolledLine = floor(point.y / character_dimension.cy);

	const LONG xPos = point.x;           // horizontal scroll position
	const LONG yPos = nScrolledLine * character_dimension.cy;           // vertical scroll position
												   // source and destination cordinates and sizes
	LONG nDstWidth, nDstHeight;

	if (width > cx)
		nDstWidth = width - xPos;
	else
		nDstWidth = width;

	if (height > cy)
		nDstHeight = height - yPos;
	else
		nDstHeight = height;

	pDCView->BitBlt(rcClient.left, rcClient.top, nDstWidth, nDstHeight,
		m_pDC.get(),
		xPos, yPos,
		SRCCOPY);
	pDCView->ExcludeClipRect(rcClient.left, rcClient.top, nDstWidth, nDstHeight);
	pDCView->FillSolidRect(rcClient, m_colorAddrBkgnd);
	ReleaseDC(pDCView);
}

//returns rowlength as number of character cells
int CDrewPlotView::BlockWidth()
{
	return 200;
}

int CDrewPlotView::BlockHeight()
{
	return 50;
}

int CDrewPlotView::DeviceCoordToRow(int coord)
{
	return coord / rowcell_dimension.cy;
}

int CDrewPlotView::GetPageWidth()
{
	if (pageprintwidth <= 0)
		pageprintwidth = 500;
	return pageprintwidth;
}

int CDrewPlotView::GetPageHeight()
{
	return pageprintheight;
}

int CDrewPlotView::GetLinesPerPage()
{
	return 40;//temp solution
}

void CDrewPlotView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDrewPlotView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDrewPlotView::SetFontDimensions(CDC* pDC)
{
	pDC->SetMapMode(mapmode);
	horpixels = pDC->GetDeviceCaps(LOGPIXELSX);
	vertpixels = pDC->GetDeviceCaps(LOGPIXELSY);
	pageprintwidth = (width-leftmargin-rightmargin)*horpixels;
	pageprintheight = (height-topmargin-bottommargin)*vertpixels;
	leftoffset = leftmargin * horpixels;

	HFONT hf = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	CFont * gf = CFont::FromHandle(hf);
	gf->GetLogFont(&labellogfont);
	labellogfont.lfWeight = FW_BOLD;
	labellogfont.lfHeight = -12 * theApp.GetSystemScaleFactor();
	labellogfont.lfOutPrecision = OUT_TT_PRECIS;
	labelfont = make_unique<CFont>();
	labelfont->CreateFontIndirect(&labellogfont);
}

void CDrewPlotView::SetGraphicBlockDimension()
{	
	point_dimension = 5;
	block_width = 0.8f * width;
	block_height = 0.2f * block_width;
	block_gap = 80;
	block_left = 80;
	block_left_gap = 10;
	block_right_gap = 10;
	block_right = block_width+block_left+block_left_gap+block_right_gap;
	block_top = 0;
	block_bottom = block_height+block_gap;
	points_per_block = block_width/point_dimension;
	number_blocks = (sequence_length + points_per_block - 1) / points_per_block;
	blocks_per_page = GetPageHeight()/(block_height+block_gap);
	tickside = 2;
	tick1 = 2;
	tick10 = 5;
	tick100 = 10;
}

BOOL CDrewPlotView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

int CDrewPlotView::GetVerticalLabelInterval()
{
	int labelinterval = 1;
	float verticaltickdistance = Yscale;
	int range  = maximum - minimum;
	int maximumincrements;
	int power;
	if(range <= 0)
		power = 1;
	else
		power = log10(range + 0.0f);
	maximumincrements = pow(10.0f, power);
	while((labelinterval * verticaltickdistance < rowcell_dimension.cy) && (labelinterval <= maximumincrements))
		labelinterval++;
	return labelinterval;
}

int CDrewPlotView::GetHorizontalLabelInterval(CDC* pDC)
{
	CString temp;
	temp.Format(_T("%d"), start + sequence_length);

	CSize texCSize = pDC->GetTextExtent(temp);
	int interval = 0;
	int horizontaltickdistance = point_dimension;
	float power, range = start + sequence_length;
	if(range <= 0)
		power = 1;
	else
		power = log10(range);
	int maximumincrements = static_cast<int>(pow(10.0f, power));
	int labelinterval = static_cast<int>(pow(10.0f, interval));
	while((labelinterval * horizontaltickdistance < texCSize.cx) && (labelinterval <= maximumincrements)) {
		interval++;
		labelinterval = pow(10.0f, interval);
	}
	if(labelinterval > maximumincrements)
		labelinterval = pow(10.0f, (interval-1));

	return labelinterval;
}

CPoint CDrewPlotView::GetStartAndEndBlocks()
{
	CPoint coords;
	coords.x = 0;
	coords.y = (int)(1 + ((float) height / block_bottom));
	coords.y = (coords.y > number_blocks) ? number_blocks : coords.y;
	if(height == 0 && width == 0) {
		coords.x = 0;
		coords.y = number_blocks;
	}
	return coords;
}

vector<int> CDrewPlotView::GetPageBreaks()
{
	return pagebreaks;
}

int CDrewPlotView::GetTotalNumberOfPages()
{
	return pagebreaks.size() - 1;
}

int CDrewPlotView::GetNumberOfBlocksPerPage()
{
	return GetPageHeight() / (block_height+block_gap);
}

int CDrewPlotView::GetTotalNumberOfBlocks()
{
	return (sequence_length+points_per_block-1) / points_per_block;
}

void CDrewPlotView::Paginate()
{
	if (number_blocks <= 0) {
		CRect rcClient(0, 0, 0, 0);
		GetClientRect(rcClient);
		width = rcClient.right;
		height = rcClient.bottom;
	}

	SetGraphicBlockDimension();
	int numberofblocks = GetTotalNumberOfBlocks();
	int numberofblocksperpage = GetNumberOfBlocksPerPage();
	pagebreaks.clear();
	pagebreaks.push_back(0);
	if (numberofblocksperpage <= 0)
		return;

	int count = 1;
	int remain = numberofblocks;
	do {
		int page_break = count * numberofblocksperpage < numberofblocks ? count * numberofblocksperpage : numberofblocks;
		pagebreaks.push_back(page_break);
		remain -= numberofblocksperpage;
		count++;
   } while(remain > 0);
}

void CDrewPlotView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	if(selection) {
		selection = false;
		Invalidate();
	}
}

void CDrewPlotView::OnEditSelectAll()
{
	selection = true;
	Invalidate();
}

void CDrewPlotView::OnEditCopy()
{
	CMetaFileDC MFDC;
	CClientDC cdc(this);

	MFDC.CreateEnhanced(NULL, NULL, NULL, NULL);
	MFDC.SetAttribDC(cdc.m_hAttribDC);

	MFDC.SetMapMode(MM_LOENGLISH);
	MFDC.SetMapMode(MM_ANISOTROPIC);
	CSize size;
	GetViewportExtEx(MFDC.GetSafeHdc(), &size);
	SetViewportExtEx(MFDC.GetSafeHdc(), size.cx * 1.5, -size.cy * 1.5, NULL);
	if (MFDC.CreateEnhanced(NULL, NULL, NULL, NULL)) {
		PaintSeqData(&MFDC);
		HENHMETAFILE hmf;
		if ((hmf = MFDC.CloseEnhanced())) {
			if (OpenClipboard()) {
				EmptyClipboard();
				SetClipboardData(CF_ENHMETAFILE, hmf);
				CloseClipboard();
			}
			else {
				/*
				* The metafile is deleted only
				* when it has not been set in
				* the clipboard.
				*/
				::DeleteEnhMetaFile(hmf);
			}
		}
	}
}

void CDrewPlotView::SaveAsEnhancedMetaFile(LPCTSTR lpszFilePath)
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
	SetViewportExtEx(MFDC.GetSafeHdc(), size.cx * 1.5, -size.cy * 1.5, NULL);
	PaintSeqData(&MFDC);

	HENHMETAFILE hEnhMetaFile = MFDC.CloseEnhanced();
	HENHMETAFILE hEMF = CopyEnhMetaFile(hEnhMetaFile, lpszFilePath);
	DeleteEnhMetaFile(hEMF);
	DeleteEnhMetaFile(hEnhMetaFile);
}

void CDrewPlotView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_CONTEXT_MENU));
	auto pContextMenu = menu.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

// CDrewPlotView diagnostics

#ifdef _DEBUG
void CDrewPlotView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrewPlotView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDrewPlotDoc* CDrewPlotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrewPlotDoc)));
	return (CDrewPlotDoc*)m_pDocument;
}
#endif //_DEBUG

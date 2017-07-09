// DNAssistView.cpp : implementation of the CSeqView class
//

#include "stdafx.h"
#include <afxpriv.h>
#include "MemDC.h"

#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "DNAssistDoc.h"
#include "resource.h"
#include "DNAssistView.h"
#include "ResultView.h"
#include "HelixParamsDialog.h"
#include "ProteinPropertiesDialog.h"
#include "RestrictEnzymesPropertySheet.h"
#include "TransFacPropertySheet.h"
#include "UPDialog.h"

#include "model/QuickSort.h"
#include "model/RestrictionCutList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeqView

IMPLEMENT_DYNCREATE(CDNAssistView, CScrollView)

BEGIN_MESSAGE_MAP(CDNAssistView, CScrollView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_CONVERT_REVERSE, OnConvertComplement)
	ON_COMMAND(ID_CONVERT_REVERSETRANSLATE, OnConvertProToDna)
	ON_COMMAND(ID_CONVERT_DNA2RNA, OnConvertDnaToRna)
	ON_COMMAND(ID_CONVERT_RNA2DNA, OnConvertRnaToDna)
	ON_COMMAND(ID_CONVERT_DNA2PRO, OnConvertDnaToPro)
	ON_COMMAND(ID_CONVERT_RNA2PRO, OnConvertRnatoPro)
	ON_COMMAND(ID_ANALYZE_FINDORFS, OnAnalyzeFindorfs)
	ON_COMMAND(ID_ANALYZE_FINDPATTERN, OnAnalyzeFindpattern)
	ON_COMMAND(ID_ANALYZE_RANDOMIZE, OnAnalyzeRandomize)
	ON_COMMAND(ID_PROPERTIES_PHYSICOCHEMICAL, OnPropertiesPhysicochemical)
	ON_COMMAND(ID_ANALYZE_TRANSCRIPTIONFACTORS, OnAnalyzeTranscriptionfactors)
	ON_COMMAND(ID_ANALYZE_NUCLEOSOMEPOSITIONS, OnAnalyzeNucleosomepositions)
	ON_COMMAND(ID_ANALYZE_RESTRICTIONMAPPING, OnAnalyzeRestrictionMapping)
	ON_COMMAND(ID_ANALYZE_LIST, OnAnalyzeList)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateIndicatorOvr)
	ON_MESSAGE(WM_CARET_POS_CHANGED, OnCaretPosChanged)
	ON_MESSAGE(WM_FIND_DIALOG, OnFindText)
	ON_MESSAGE(WM_FIND_ORF_DIALOG, OnFindORF)
	ON_MESSAGE(WM_FIND_PATTERN_DIALOG, OnFindPattern)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	
END_MESSAGE_MAP()

// CSeqView construction/destruction

CDNAssistView::CDNAssistView()
{
	// We have to do some initialisation here
	m_bInsert = TRUE;
	m_colorAddrBkgnd = ::GetSysColor(COLOR_BTNFACE);
	m_colorAddrText = ::GetSysColor(COLOR_GRAYTEXT);
	m_colorSeqDataBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_colorSeqDataText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_colorSeqDataSelectedBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);
	m_colorSeqDataSelectedText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_nBytesPerGroup = 10;
	m_nByteSpacing = 0;
	m_nGroupSpacing = 1;
	m_nAddrCharLen = 1;
	m_nTotalLines = 0;
	m_nCurPos[0][0] = 0;
	m_nCurPos[0][1] = 1;
	m_nBytesRead = 0;
	m_nSelStart = 0;
	m_nSelEnd = 0;

	m_bShiftKeyDown = FALSE;

	// TEMPORARY
	for (int i = 0; i<20; ++i)
		m_mapSpecialChars[i] = RGB(0, 0, 255);

	LoadDefaultFont();
}

CDNAssistView::~CDNAssistView()
{
}

BOOL CDNAssistView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

int CDNAssistView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDocVarManager = make_unique<CDocVarManager>(theApp.undoLevels);
	SetData(GetDocument()->GetData());
	// Calculate initial value based on the size of the control
	ComputeCharCellSize();
	ComputeAddressRect();
	ComputeSeqDataRect();

	// init caret position
	m_ptCaretPosition.x = m_rectDataSeq.left;
	m_ptCaretPosition.y = m_rectDataSeq.top;

	// Set the cursor to be I-beam
	m_hCursorIBean = theApp.LoadStandardCursor(IDC_IBEAM);

	return 0;
}

void CDNAssistView::LoadDefaultFont()
{
	// we need to clean up the object before use
	m_Font.DeleteObject();

	auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
	m_Font.CreateFont(editorFontTransferBuffer->facesize, 0, 0, 0, editorFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, theApp.editorFontFace);
}

void CDNAssistView::ComputeCharCellSize()
{
	// determine the size of the Character Cell
	CPaintDC dc(this);
	dc.SelectObject(&m_Font);
	m_sizeCharCell = dc.GetTextExtent(_T("A"));
}

void CDNAssistView::ComputeTotalLines()
{
	// get number of bytes in the file
	size_t dwFileSize = m_pDataStr->length();
	// Calcuate how many bytes per line are there
	size_t dwBytesPerLine = m_nBytesPerGroup * theApp.numberOfGroups;
	// calculate how many lines total
	m_nTotalLines = dwFileSize / dwBytesPerLine;
	// there is always at least one line
	++m_nTotalLines;
}

void CDNAssistView::ComputeAddressRect()
{
	// top left is 0,0
	m_rectAddr.top = 0;
	m_rectAddr.left = 0;
	m_rectAddr.right = m_sizeCharCell.cx * (m_nAddrCharLen + 1L); // +1 because of the ":"
	m_rectAddr.bottom = m_nTotalLines * m_sizeCharCell.cy;
}

void CDNAssistView::ComputeSeqDataRect()
{
	m_rectDataSeq.top = 0;
	m_rectDataSeq.left = m_sizeCharCell.cx * (m_nAddrCharLen + 3L);
	m_rectDataSeq.bottom = (m_nTotalLines + 1) * m_sizeCharCell.cy;

	size_t dwTotalChars = theApp.numberOfGroups * m_nBytesPerGroup;
	dwTotalChars += (theApp.numberOfGroups - 1) * m_nGroupSpacing;
	dwTotalChars += theApp.numberOfGroups * (m_nBytesPerGroup - 1) * m_nByteSpacing;
	m_rectDataSeq.right = m_rectDataSeq.left + m_sizeCharCell.cx * dwTotalChars;

	CSize sizeTotal;
	// TODO: calculate the total size of this view

	sizeTotal.cx = m_rectDataSeq.right;
	sizeTotal.cy = m_rectDataSeq.bottom;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CDNAssistView::ComputeCharOffset()
{
	// first clear the vector
	m_vecCharOffset.clear();

	// first pos
	size_t nPos = 0;

	// go through the groups
	for (int i = 0; i<theApp.numberOfGroups; ++i)
	{
		// go through each byte in the group
		for (int j = 0; j<m_nBytesPerGroup; ++j)
		{
			// save the position
			m_vecCharOffset.push_back(nPos);
			// increase the postion for spacing 
			// between bytes in the group +1
			nPos += (m_nByteSpacing + 1);
		}
		// here increase the postion for 
		// spacing between groups -m_nSpaceByte because 
		// we spaced it already for m_nSpaceByte in prev. loop
		nPos += (m_nGroupSpacing - m_nByteSpacing);
	}
}

void CDNAssistView::ComputeAddressCharLen()
{
	// get the size of the file
	size_t dwFileSize = m_pDataStr->length();
	// construct Seq number
	CString strFileLen;
	strFileLen.Format(_T("%d:"), dwFileSize);
	// assign the length
	m_nAddrCharLen = strFileLen.GetLength();
}

BOOL CDNAssistView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (MK_CONTROL & nFlags) {
		if (zDelta > 0)
			ChangeFontSize(TRUE);
		else if (zDelta < 0)
			ChangeFontSize(FALSE);
		return FALSE;
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDNAssistView::CreateEditCaret()
{
	CreateSolidCaret(m_sizeCharCell.cx, m_sizeCharCell.cy);
	SetCaretPos(m_ptCaretPosition);
	ShowCaret();
}

void CDNAssistView::DestroyEditCaret()
{
	HideCaret();
	DestroyCaret();
}

void CDNAssistView::PaintAddrRect(CDC* pDC)
{
	// load solid pen
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_colorAddrBkgnd);
	pDC->SelectObject(&pen);
		
	int nBottom = m_rectAddr.bottom;
	int nRight = m_rectAddr.right;
	m_rectAddr.bottom = m_rectDataSeq.bottom;
	m_rectAddr.right = m_rectDataSeq.left;
	pDC->FillSolidRect(m_rectAddr, m_colorAddrBkgnd);
	m_rectAddr.bottom = nBottom;
	m_rectAddr.right = nRight;
}

void CDNAssistView::PaintAddresses(CDC* pDC)
{
	// some temporary data
	CString strText;

	// put together the string to draw
	size_t dwBytesPerLine = theApp.numberOfGroups * m_nBytesPerGroup;
	for (size_t i = 0; i<m_nTotalLines; ++i)
	{
		size_t nAddr = i * dwBytesPerLine + 1;
		CString strTemp; strTemp.Format(_T("%.*d:"), m_nAddrCharLen, nAddr);
		strText += strTemp + _T(" ");
	}

	// set bk color and draw it
	pDC->SetBkColor(m_colorAddrBkgnd);
	pDC->SetTextColor(m_colorAddrText);
	pDC->DrawText(strText, m_rectAddr,
		DT_NOPREFIX | DT_NOCLIP | DT_TOP | DT_LEFT | DT_WORDBREAK);
}

void CDNAssistView::PaintSeqDataRect(CDC* pDC)
{
	// load pen
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_colorSeqDataBkgnd);

	pDC->SelectObject(&pen);
	pDC->FillSolidRect(m_rectDataSeq, m_colorSeqDataBkgnd);
}

void CDNAssistView::PaintSeqData(CDC* pDC)
{
	// get the data from the file
	size_t dwBytesPerLine = theApp.numberOfGroups * m_nBytesPerGroup;

	m_nBytesRead = m_pDataStr->length();
	// assamble page to display
	string strPage;
	FormatBytesSeq(strPage, *m_pDataStr, m_nBytesRead);

	// set bk color and draw it
	pDC->SetBkColor(m_colorSeqDataBkgnd);
	pDC->SetTextColor(m_colorSeqDataText);
	CString page(strPage.c_str());
	pDC->DrawText(page, m_rectDataSeq,
		DT_NOPREFIX | DT_NOCLIP | DT_TOP | DT_LEFT | DT_WORDBREAK);
}

void CDNAssistView::PaintSeqSelection(CDC* pDC)
{
	// Check is anything selected
	if ((m_nSelStart == m_nSelEnd) || (m_nBytesRead == 0)) {
		CString strSel = _T("");
		if(m_prevStatus != strSel) {
			theApp.m_pStatusBar->SetPaneText(1, strSel);
			m_prevStatus = strSel;
		}
		return;
	}

	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end 
	if (m_nSelStart > m_nSelEnd)
	{
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	// Check do we need to paint the selection at all
	// First calculate page starting offset from the file begin
	size_t nBytesPerLine = theApp.numberOfGroups * m_nBytesPerGroup;

	// first determine in which line starts the drawing
	size_t dwStartLine = nSelStart / nBytesPerLine;

	// another check is the selection visible
	if (nSelEnd > m_pDataStr->length())
		return;

	size_t dwStartOffset = nSelStart % nBytesPerLine;

	vector<string> vecLines;
	string dataSegment = m_pDataStr->substr(nSelStart, nSelEnd - nSelStart);
	FormatSelLineSeq(vecLines, dwStartOffset, dataSegment, nSelEnd - nSelStart);
	// draw first line
	if (vecLines.size() < 1)
		return;

	// now we have formated lines so lets figure out where to draw

	// so calculate starting y position
	size_t nYPos = dwStartLine * m_sizeCharCell.cy;
	size_t nXPos = (m_nGroupSpacing * (dwStartOffset / m_nBytesPerGroup) + m_nByteSpacing * (dwStartOffset - 1)) * m_sizeCharCell.cx;
	nXPos += m_rectDataSeq.left + dwStartOffset * m_sizeCharCell.cx;

	pDC->SetBkColor(m_colorSeqDataSelectedBkgnd);
	pDC->SetTextColor(m_colorSeqDataSelectedText);

	CString vecLine(vecLines[0].c_str());
	pDC->TextOut(nXPos, nYPos, vecLine);
	for (int i = 1; i < vecLines.size(); ++i) {
		vecLine = vecLines[i].c_str();
		pDC->TextOut(m_rectDataSeq.left, nYPos + i*m_sizeCharCell.cy, vecLine);
	}

	CString strSel;
	int length = nSelEnd - nSelStart;
	if (length > 0)
		strSel.Format(_T("Selection: %d - %d; Length: %d"), nSelStart, nSelEnd, length);

	if(m_prevStatus != strSel) {
		theApp.m_pStatusBar->SetPaneText(1, strSel);
		m_prevStatus = strSel;
	}
}

void CDNAssistView::UpdateCtrlStatus()
{
	// destroy caret
	DestroyEditCaret();

	// recalculate caret position because it might be that
	// we changed the font; first check which line are we in
	size_t dwCurLine = m_ptCaretPosition.y / m_sizeCharCell.cy;
	size_t dwCurCol = m_ptCaretPosition.x / m_sizeCharCell.cx - (m_nAddrCharLen + 3);

	// Calculate initial valuse based on the size of the control
	ComputeAddressCharLen();
	ComputeCharCellSize();
	ComputeAddressRect();
	ComputeSeqDataRect();
	ComputeCharOffset();
	ComputeTotalLines();

	// put it into the correct line
	m_ptCaretPosition.y = dwCurLine * m_sizeCharCell.cy;
	m_ptCaretPosition.x = (dwCurCol + (m_nAddrCharLen + 3L)) * m_sizeCharCell.cx;
	// recreate caret
	CreateEditCaret();

	// we probably need to repaint
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

int CDNAssistView::GetNextCaretPos(BOOL bJumpByte)
{
	// get the caret offset relative to the file begin
	size_t dwOffset = GetCaretPosFileOffset();

	// if we are at the end of the file + 1, then caret should not be moved
	if ((dwOffset>m_pDataStr->length() - 1) || (m_pDataStr->length() == 0))
		return m_vecCharOffset[m_nCurPos[0][0]];

	// check do we need to jump complete byte
	if (bJumpByte)
	{
		// check are we at the end of the line 
		// we must check this way because this is special case
		// we are moving bytewise so we have to check are we at the last byte
		// doesn't matter which nibble
		if (m_nCurPos[0][0] == (m_nBytesPerGroup*theApp.numberOfGroups - 1))
		{
			// If we are at the end of the line we need to go
			// to the next line
			if (MoveCaretDown())
			{
				m_nCurPos[0][0] = 0;
				m_nCurPos[0][1] = 1;
				return m_vecCharOffset[m_nCurPos[0][0]];
			}
			return m_vecCharOffset[m_nCurPos[0][0]];
		}
		m_nCurPos[0][0] += 1;
		m_nCurPos[0][1] = 1;
		return m_vecCharOffset[m_nCurPos[0][0]];
	}

	// check are we at the first nibble of the byte
	if (m_nCurPos[0][1] == 1)
	{
		m_nCurPos[0][1] = 2;
		return m_vecCharOffset[m_nCurPos[0][0]] + 1;
	}

	// check are we at the end of the line
	if (IsLineEnd())
	{
		// If we are at the end of the line we need to go
		// to the next line
		if (MoveCaretDown())
		{
			m_nCurPos[0][0] = 0;
			m_nCurPos[0][1] = 1;
			return m_vecCharOffset[m_nCurPos[0][0]];
		}
		return m_vecCharOffset[m_nCurPos[0][0]] + 1;
	}

	// if we are on the second nibble then go to next byte
	m_nCurPos[0][0] += 1;
	m_nCurPos[0][1] = 1;
	return m_vecCharOffset[m_nCurPos[0][0]];
}

int CDNAssistView::GetPrevCaretPos(BOOL bJumpByte)
{
	// check do we need to jump complete byte
	if (bJumpByte)
	{
		// check are we at the end of the line
		// we only check are we at the end bytewise doesn't matter which nibble
		if (m_nCurPos[0][0] == 0)
		{
			// If we are at the end of the line we need to go
			// to the next line
			if (MoveCaretUp())
			{
				m_nCurPos[0][0] = (theApp.numberOfGroups*m_nBytesPerGroup - 1);
				m_nCurPos[0][1] = 1;
				return m_vecCharOffset[m_nCurPos[0][0]];
			}
			return m_vecCharOffset[m_nCurPos[0][0]];
		}
		m_nCurPos[0][0] -= 1;
		m_nCurPos[0][1] = 1;
		return m_vecCharOffset[m_nCurPos[0][0]];
	}

	// check are we at the second nibble of the byte
	if (m_nCurPos[0][1] == 2)
	{
		m_nCurPos[0][1] = 1;
		return m_vecCharOffset[m_nCurPos[0][0]];
	}

	// check are we at the begin of the line
	if (IsLineBegin())
	{
		// If we reached line bgin then we need to move caret up
		if (MoveCaretUp())
		{
			m_nCurPos[0][0] = (theApp.numberOfGroups*m_nBytesPerGroup - 1);
			m_nCurPos[0][1] = 2;
			return (m_vecCharOffset[m_nCurPos[0][0]] + 1);
		}
		return m_vecCharOffset[m_nCurPos[0][0]];
	}

	// if we are on the first nibble then go to next byte second nibble
	m_nCurPos[0][0] -= 1;
	m_nCurPos[0][1] = 2;
	return m_vecCharOffset[m_nCurPos[0][0]] + 1;
}

void CDNAssistView::AlignCaret(CPoint &point)
{
	// make copies so we don't damage these rectangles
	CRect m_SeqRect = m_rectDataSeq;

	// make sure top > bottom and left > right
	m_SeqRect.NormalizeRect();

	// check is the click in the in the Seq area
	if (m_SeqRect.PtInRect(point)) {
		AlignCaretSeq(point);
		return;
	}

}

void CDNAssistView::AlignCaretSeq(CPoint &point)
{
	// calculate in which line is the caret
	size_t nLine = (point.y / m_sizeCharCell.cy);
	// do not move the caret past the top line
	if (point.y <= 0)
		nLine = 0;
	// do not move the caret past bottom line or
	// if the desired position is past the bottom line
	// then leave the cursor in the current line
	if ((nLine > (m_nTotalLines - 1)) || (m_nTotalLines == 0))
		nLine = m_nTotalLines - 1;

	// calculate column
	// this column must be aligned agains the data display 
	int nCol = (point.x / m_sizeCharCell.cx) - (m_nAddrCharLen + 3);

	// now we need to align the column regarding the byte grouping
	AlignColumnAndSetPosSeq(nCol);

	// Check are we in the last line and if yes then we need to make sure
	// that we do not pass the last byte in the last line. Ok we can pass it
	// but only to the next position where we could add next byte.
	size_t dwFileSize = m_pDataStr->length();
	size_t dwBytesPerLine = m_nBytesPerGroup*theApp.numberOfGroups;
	size_t dwBytesInLastLine = dwFileSize%dwBytesPerLine;
	// if bytes in the last line is 0 that could mean that 
	// there are m_nBytesPerGroup*theApp.m_nNumberOfGroups bytes in the last line or no bytes
	// so we need to check for that
	if (nLine == (m_nTotalLines - 1)) {
		// we are in the last line
		// now check are we pass the allowed position
		// if the length of the file is 0 then do not allow
		// caret to go to the next pos because we are already at that pos
		if (dwFileSize > 0) {
			if (nCol>m_vecCharOffset[dwBytesInLastLine])
				nCol = m_vecCharOffset[dwBytesInLastLine];
		}
		else nCol = m_vecCharOffset[0];
	}

	// AlignColumAndSetPosSeq sets the active caret position
	AlignColumnAndSetPosSeq(nCol);
	m_ptCaretPosition.x = (nCol + m_nAddrCharLen + 3) * m_sizeCharCell.cx;
	m_ptCaretPosition.y = nLine * m_sizeCharCell.cy;
	SetCaretPos(m_ptCaretPosition);
}


void CDNAssistView::AlignColumnAndSetPosSeq(int &nCol)
{
	// go through all valid positions and find out which one is closest
	int pos = abs(m_vecCharOffset[0] - nCol);
	int i = 1;
	for (; i<m_vecCharOffset.size(); ++i)
	{
		// find minimum distance & return it
		if (abs(m_vecCharOffset[i] - nCol) >= pos)
		{
			m_nCurPos[0][0] = i - 1;
			m_nCurPos[0][1] = 1;
			nCol = m_vecCharOffset[i - 1];
			return;
		}

		pos = abs(m_vecCharOffset[i] - nCol);
	}

	// if not found then it must be last one
	m_nCurPos[0][0] = i - 1;
	m_nCurPos[0][1] = 1;
	nCol = m_vecCharOffset[i - 1];
}

void CDNAssistView::FormatBytesSeq(string& strRes, string& dataSegment, size_t nCount)
{
	// construct the spacing strings
	string strByteSpacing(m_nByteSpacing, ' ');
	string strGroupSpacing(m_nGroupSpacing, ' ');
	int j = 0; // counts bytes in the group
	int k = 0; // group counter
	for (size_t i = 0; i<nCount; ++i) // go through all bytes
	{
		++j; // increase number of bytes
		strRes += dataSegment[i];
		// are we done with the group?
		if (j == m_nBytesPerGroup)
		{
			k++; // increase group number
			j = 0; // reset byte counter for new group

			strRes += strGroupSpacing;
		}
		strRes += strByteSpacing;
	}
}

void CDNAssistView::FormatSelLineSeq(std::vector<string> &vecRes, size_t dwStart, string& dataSegment, size_t nCount)
{
	// construct the spacing strings
	string strByteSpacing(m_nByteSpacing, ' ');
	string strGroupSpacing(m_nGroupSpacing, ' ');
	string strLine = "";
	// clear array
	vecRes.clear();

	int j = dwStart;
	// counts groups or this is 1-based group index
	// we have to activate the group where dwStart is
	int k = dwStart / m_nBytesPerGroup;
	if (nCount > dataSegment.length())
		nCount = 0;

	vecRes.reserve(nCount);
	for (size_t i = 0; i<nCount; ++i) // go through all bytes
	{
		++j; // increase number of bytes
		strLine += dataSegment[i];
		// are we done with the group?
		if (j % m_nBytesPerGroup == 0)
		{
			k++; // increase group number
			j = 0; // reset byte counter for new group
			strLine += strGroupSpacing;
			// are we done with all groups in the line
			// and if not then space groups correctly
			if (k == theApp.numberOfGroups)
			{
				// reset group counter because we are in the next line
				k = 0;
				// add this line to the array
				vecRes.push_back(strLine);
				// prepare line for next line
				strLine = "";
				continue;
			}
		}
		strLine += strByteSpacing;
	}

	// don't forget last line
	const auto strEnd = strLine.find_last_not_of(' ');
	strLine.substr(0, strEnd);
	vecRes.push_back(move(strLine));
}

BOOL CDNAssistView::MoveCaretUp()
{
	// figure out in which line is the caret
	// this value is relative to the top line
	UINT nCaretCurLine = m_ptCaretPosition.y / m_sizeCharCell.cy;
	// if the caret is in the first line and there are no other lines so do nothing
	if (nCaretCurLine == 0)
	{
		CPoint pt = GetScrollPosition();
		size_t nLinesScrolled = floor(pt.y / m_sizeCharCell.cy);
		// if the caret is in the first line and there are some lines to scroll then scroll
		if (nLinesScrolled != 0)
			SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
		else return FALSE;
	}

	// otherwise move caret in usual way
	m_ptCaretPosition.y -= m_sizeCharCell.cy;
	SetCaretPos(m_ptCaretPosition);
	return TRUE;
}

BOOL CDNAssistView::MoveCaretDown()
{
	// figure out in which line is the caret
	// this value is relative to the top line
	UINT nCaretCurLine = m_ptCaretPosition.y / m_sizeCharCell.cy;
	// check does the moving need to do anything
	if (nCaretCurLine == (m_nTotalLines - 1))
		return FALSE;

	// check is the caret in the before last line and if it is make sure it 
	// jumps back to the last character in the last line
	if (nCaretCurLine == (m_nTotalLines - 2))
	{
		// get the number of chars in the last line
		size_t dwFileSize = m_pDataStr->length();
		size_t dwBytesPerLine = m_nBytesPerGroup*theApp.numberOfGroups;
		size_t dwBytesInLastLine = dwFileSize%dwBytesPerLine;

		// ok we are in the Seq area
		if (m_vecCharOffset[m_nCurPos[0][0]]>m_vecCharOffset[dwBytesInLastLine])
		{
			m_nCurPos[0][0] = dwBytesInLastLine;
			m_nCurPos[0][1] = 1;
			// recalculate x coordinates of the caret
			m_ptCaretPosition.x = m_rectDataSeq.left + m_vecCharOffset[m_nCurPos[0][0]] * m_sizeCharCell.cx;
		}

	}

	// otherwise move caret in usual way
	m_ptCaretPosition.y += m_sizeCharCell.cy;
	SetCaretPos(m_ptCaretPosition);
	return TRUE;
}

void CDNAssistView::MoveCaretLeft(BOOL bJumpByte)
{
	// determine if the user is running in the ascii editor
	// the reason is because we need to determine the caret offset
	size_t dwCaretOffset = m_rectDataSeq.left;

	// get the coordinates
	m_ptCaretPosition.x = dwCaretOffset + GetPrevCaretPos(bJumpByte)*m_sizeCharCell.cx;
	SetCaretPos(m_ptCaretPosition);
}

void CDNAssistView::MoveCaretRight(BOOL bJumpByte)
{
	// determine if the user is running in the ascii editor
	// the reason is because we need to determine the caret offset
	size_t dwCaretOffset = m_rectDataSeq.left;

	// get the coordinates and move it
	m_ptCaretPosition.x = dwCaretOffset + GetNextCaretPos(bJumpByte)*m_sizeCharCell.cx;
	SetCaretPos(m_ptCaretPosition);
}

BOOL CDNAssistView::ChangeFontSize(BOOL bIncrease)
{
	auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
	if (bIncrease)
	{
		++editorFontTransferBuffer->facesize;
		if (editorFontTransferBuffer->facesize > 20)
		{
			editorFontTransferBuffer->facesize = 20;
			LoadDefaultFont();
			UpdateCtrlStatus();
			return FALSE;
		}
		LoadDefaultFont();
		UpdateCtrlStatus();
		return TRUE;
	}

	--editorFontTransferBuffer->facesize;
	if (editorFontTransferBuffer->facesize < 12)
	{
		editorFontTransferBuffer->facesize = 12;
		LoadDefaultFont();
		UpdateCtrlStatus();
		return FALSE;
	}
	LoadDefaultFont();
	UpdateCtrlStatus();
	return TRUE;
}

size_t CDNAssistView::GetCaretPosFileOffset()
{
	// Get file size
	size_t dwFileSize = m_pDataStr->length();
	size_t dwBytesPerLine = theApp.numberOfGroups * m_nBytesPerGroup;

	CPoint pt = GetScrollPosition();
	// determine in which line is the caret relative to the top
	// this value is zero based which means that if caret is in the 
	// first line from the top this value will be zero
	size_t dwCurLine = (pt.y + m_ptCaretPosition.y) / m_sizeCharCell.cy;
	// Calcualte all lines before this one
	size_t dwRes = (dwCurLine * dwBytesPerLine) + m_nCurPos[0][0];
	return dwRes;
}

void CDNAssistView::MoveCaretToFilePos(size_t dwPos)
{
	// get the file size
	size_t dwFileSize = m_pDataStr->length();
	// check is the position invlid
	if ((dwPos<0) || (dwPos>dwFileSize))
		return;

	// get current page offsets
	size_t dwBytesPerLine = theApp.numberOfGroups * m_nBytesPerGroup;
	size_t dwPageStartOffset = 0;
	size_t dwPageEndOffset = m_nTotalLines * dwBytesPerLine;

	// check is the desired position visible on this page
	if ((dwPos >= dwPageStartOffset) && (dwPos <= dwPageEndOffset))
	{
		// convert the global offset to one that is relative to this page
		int dwPageOffset = (int)dwPos - dwPageStartOffset;
		// get which line is that
		size_t dwLine = dwPageOffset / dwBytesPerLine;
		size_t dwLineOffset = dwPageOffset%dwBytesPerLine;

		m_ptCaretPosition.x = m_rectDataSeq.left + m_vecCharOffset[dwLineOffset] * m_sizeCharCell.cx;
		m_ptCaretPosition.y = dwLine*m_sizeCharCell.cy;

		// set that as current position
		m_nCurPos[0][0] = dwLineOffset;
		m_nCurPos[0][1] = 1;

		// now move caret there 
		SetCaretPos(m_ptCaretPosition);
		return;
	}
}

BOOL CDNAssistView::EditData(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// check is the charater printable
	if ((nChar < 0x20) || (nChar > 0x7E))
	{
		Beep(2000, 100);
		return FALSE;
	}

	const char seqType = GetDocument()->GetSeqType();
	// check is the charater printable
	if (!m_seqValidator.IsValid(seqType, (char)nChar)) {
		Beep(2000, 100);
		return FALSE;
	}

	// figure out at which position are we
	size_t dwPos = GetCaretPosFileOffset();

	// check are we overwriting or inserting
	if (m_bInsert) {
		m_pDataStr->insert(dwPos, 1, nChar);
	}
	else {
		// overwrite means set low and high nibble
		(*m_pDataStr)[dwPos] = nChar;
	}
	
	// set modified flag
	SetModifiedFlag(TRUE);

	UpdateCtrlStatus();
	return TRUE;
}

void CDNAssistView::DeleteData(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// lets check do we need to delete a bunch of selected bytes or just one byte
	if (m_nSelStart == m_nSelEnd) {
		size_t nStart = GetCaretPosFileOffset();
		if (nStart < m_pDataStr->length()) {
			string dataSegment = "" + m_pDataStr->at(nStart);
			m_pDocVarManager->AddToUndoList(1, dataSegment, GetCaretPosFileOffset(), 0, 0);
			m_pDataStr->erase(nStart, 1);
		}
	}
	else {
		string dataSegment;
		GetSelectedSeq(dataSegment);
		int nStart = min(m_nSelStart, m_nSelEnd);
		int nSelectionExtent = abs((int) m_nSelStart - (int) m_nSelEnd);
		m_pDocVarManager->AddToUndoList(1, dataSegment, GetCaretPosFileOffset(), nStart, nSelectionExtent);
		if (nStart < m_pDataStr->length()) {
			m_pDataStr->erase(nStart, nSelectionExtent);
			// now we have to move caret to the position of the selection start
			MoveCaretToFilePos(nStart);
		}
	}

	m_nSelEnd = m_nSelStart = GetCaretPosFileOffset();

	// set modified flag
	SetModifiedFlag(TRUE);

	UpdateCtrlStatus();
}

// CSeqView printing

BOOL CDNAssistView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDNAssistView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);	
	pDC->SetViewportOrg(CPoint(0, 0)); // force viewport origin to zero
	if (pDC->IsPrinting())
		pDC->SetMapMode(MM_LOENGLISH);
	else
		pDC->SetMapMode(MM_TEXT);          // force map mode to MM_TEXT
}

void CDNAssistView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
	CPoint point(20, -18);
	pDC->TextOut(point.x, point.y, _T("DNAssist"));
}

void CDNAssistView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
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

void CDNAssistView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	int nPageWidth = ::MulDiv(pDC->GetDeviceCaps(PHYSICALWIDTH), 1440, pDC->GetDeviceCaps(LOGPIXELSX)) - 2690;
	int nPageHeight = ::MulDiv(pDC->GetDeviceCaps(PHYSICALHEIGHT), 1440, pDC->GetDeviceCaps(LOGPIXELSY)) - 2438;
	// assamble page to display
	FormatBytesSeq(m_printBuffer, *m_pDataStr, m_nBytesRead);
	nCharsPerLineToPrint = nPageWidth / 144;
	nLinesPerPageToPrint = nPageHeight / 288;
	short nTotalLines = ceil((float) m_printBuffer.length() / nCharsPerLineToPrint);
	short nMaxPage = max(1, ceil((float) nTotalLines / nLinesPerPageToPrint));
	
	pInfo->SetMaxPage(nMaxPage);
}

void CDNAssistView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	PrintHeader(pDC, pInfo);
	CPoint     point(720, -1440);
	CFont      font;
	LOGFONT lf;
	m_Font.GetLogFont(&lf);
	// 14-point fixed-pitch font
	font.CreateFont(-280, 0, 0, 0, 400, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, lf.lfFaceName);
	// Courier New is a TrueType font
	CFont* pOldFont = (CFont*)(pDC->SelectObject(&font));

	pDC->SetMapMode(MM_TWIPS);
	
	// get the current page number(for first page returns 1, for second - 2 and so on)
	size_t nCurPage = pInfo->m_nCurPage;
	// get the index of the first CString in the array that has to be printed in this page
	size_t nStartPos = (nCurPage - 1) * nCharsPerLineToPrint * nLinesPerPageToPrint;
	size_t nEndPos = nCurPage * nCharsPerLineToPrint * nLinesPerPageToPrint;
	// fill a TEXTMETRIC struct with various information about the selected font
	CRect rect(720, -720, m_nPageWidth, m_nPageHeight);
	if (nCurPage < pInfo->GetMaxPage()) {
		CString page(m_printBuffer.substr(nStartPos, nEndPos - nStartPos).c_str());
		pDC->DrawText(page, rect,
			DT_NOPREFIX | DT_NOCLIP | DT_TOP | DT_LEFT | DT_WORDBREAK);		
	}
	else {
		// will stop printing if all strings are printed
		pInfo->m_bContinuePrinting = FALSE;
		CString page(m_printBuffer.substr(nStartPos).c_str());
		pDC->DrawText(page, rect,
			DT_NOPREFIX | DT_NOCLIP | DT_TOP | DT_LEFT | DT_WORDBREAK);
	}

	PrintFooter(pDC, pInfo);
	pDC->SelectObject(pOldFont);
}

void CDNAssistView::OnDraw(CDC* pDCView)
{
	ASSERT(MM_TEXT == pDCView->GetMapMode());
	ASSERT(CPoint(0, 0) == pDCView->GetViewportOrg());

	SetData(GetDocument()->GetData());
	ComputeSeqDataRect();

	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);

	const LONG cx = rcClient.right;      // view client area width
	const LONG cy = rcClient.bottom;     // view client area height	
	const LONG bx = m_rectDataSeq.right;             // source document width
	const LONG by = m_rectDataSeq.bottom;             // source document height

	CMemoryDC dc(pDCView, bx, by);

	// select font desired
	dc.SelectObject(&m_Font);

	// paint rectangles
	PaintAddrRect(&dc);
	PaintSeqDataRect(&dc);

	// paint characters
	PaintAddresses(&dc);
	PaintSeqData(&dc);
	PaintSeqSelection(&dc);

	ReleaseDC(&dc);

	CPoint point = GetScrollPosition();
	const size_t nScrolledLine = floor(point.y / m_sizeCharCell.cy);

	const LONG xPos = point.x;           // horizontal scroll position
	const LONG yPos = nScrolledLine * m_sizeCharCell.cy;           // vertical scroll position

																   // source and destination cordinates and sizes
	LONG nDstWidth, nDstHeight;

	if (bx > cx)
		nDstWidth = bx - xPos;
	else
		nDstWidth = bx;

	if (by > cy)
		nDstHeight = by - yPos;
	else
		nDstHeight = by;

	pDCView->BitBlt(rcClient.left, rcClient.top, nDstWidth, nDstHeight,
		&dc,
		xPos, yPos,
		SRCCOPY);
	m_rectAddr.bottom = m_rectDataSeq.bottom;
	pDCView->ExcludeClipRect(m_rectAddr);
	pDCView->ExcludeClipRect(m_rectDataSeq);
	pDCView->FillSolidRect(rcClient, m_colorAddrBkgnd);
	ReleaseDC(pDCView);	
}

BOOL CDNAssistView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
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

BOOL CDNAssistView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
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

	// update position on the status bar
	PostMessage(WM_CARET_POS_CHANGED, 0, 0);
	return TRUE;
}

// We will handle this ourselves
BOOL CDNAssistView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CDNAssistView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(GetHCursor());
	return TRUE;
}

BOOL CDNAssistView::VScrollToCaretPos(BOOL bDoScroll)
{
	// check first is the caret on the screen visible 
	// and if not then scroll the view up to that line
	CPoint pt = GetScrollPosition();
	if (m_ptCaretPosition.y < pt.y) {
		// move caret
		if (m_ptCaretPosition.y < 0)
			m_ptCaretPosition.y = 0;

		SetCaretPos(m_ptCaretPosition);
		if(bDoScroll) {
			// move scrollbar
			ScrollToPosition(m_ptCaretPosition);
		}
		// ... and redraw
		UpdateCtrlStatus();
		return TRUE;
	}

	CRect rcClient(0, 0, 0, 0);
	GetClientRect(rcClient);
	if(m_ptCaretPosition.y >= pt.y + rcClient.Height())
		ScrollToPosition(m_ptCaretPosition);

	return FALSE;
}

void CDNAssistView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();

		// If shift key is down then we are selecting
		if (m_bShiftKeyDown)
		{
			// move caret one byte forwards
			MoveCaretLeft(TRUE);
			// set this position to be new selection end
			m_nSelEnd = GetCaretPosFileOffset();
			Invalidate();
		}
		else
		{
			MoveCaretLeft(TRUE);
			// reset all selections
			m_nSelStart = m_nSelEnd = GetCaretPosFileOffset();
			Invalidate();
		}
	}
	break;
	case VK_RIGHT:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();

		// If shift key is down then we are selecting
		if (m_bShiftKeyDown)
		{
			// move caret one byte backwards
			MoveCaretRight(TRUE);
			// set this position to be new selection end
			m_nSelEnd = GetCaretPosFileOffset();
			Invalidate();
		}
		else
		{
			MoveCaretRight(TRUE);
			// reset all selections
			m_nSelStart = m_nSelEnd = GetCaretPosFileOffset();
			Invalidate();
		}
	}
	break;
	case VK_UP:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();
		BOOL needScroll = m_ptCaretPosition.y <= 0;
		// move caret one byte backwards
		if(!needScroll)
			MoveCaretUp();

		SendMessage(WM_VSCROLL, SB_LINEUP);
		needScroll = m_ptCaretPosition.y < 0;
		// If shift key is down then we are selecting
		if (m_bShiftKeyDown) {			
			// set this position to be new selection end
			m_nSelEnd = GetCaretPosFileOffset();
			if (needScroll) {
				SendMessage(WM_VSCROLL, SB_LINEUP);
				MoveCaretDown();
			}			
		}
		else {
			// reset all selections
			m_nSelStart = m_nSelEnd = GetCaretPosFileOffset();			
			if (needScroll) {
				SendMessage(WM_VSCROLL, SB_LINEUP);
				MoveCaretDown();
			}
		}
		Invalidate();
	}
	break;
	case VK_DOWN:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();
		CRect rcClient(0, 0, 0, 0);
		GetClientRect(rcClient);
		BOOL needScroll = m_ptCaretPosition.y > rcClient.bottom - m_sizeCharCell.cy;
		// move caret one byte backwards
		if(!needScroll)
			MoveCaretDown();

		needScroll = m_ptCaretPosition.y > rcClient.bottom;
		// If shift key is down then we are selecting
		if (m_bShiftKeyDown) {			
			// set this position to be new selection end
			m_nSelEnd = GetCaretPosFileOffset();			
			if (needScroll)
				SendMessage(WM_VSCROLL, SB_LINEDOWN);				
		}
		else {
			// reset all selections
			m_nSelStart = m_nSelEnd = GetCaretPosFileOffset();
			if (needScroll)
				SendMessage(WM_VSCROLL, SB_LINEDOWN);							
		}
		MoveCaretUp();
		SendMessage(WM_VSCROLL, SB_LINEDOWN);
		Invalidate();
	}
	break;
	case VK_INSERT:
		m_bInsert = !m_bInsert;
		break;
	case VK_DELETE:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();

		// Here we need to check first what is selected
		// if nothing is selected then delete only active byte
		DeleteData(nChar, nRepCnt, nFlags);
	}break;
	case VK_BACK:
		break;
	case VK_SHIFT:
		m_bShiftKeyDown = TRUE;
		break;
	case VK_CONTROL:
		break;
	default:
	{
		// first we need to scroll to the caret position
		VScrollToCaretPos();
	}
	}

	// update position on the status bar
	PostMessage(WM_CARET_POS_CHANGED, 0, 0);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDNAssistView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// Recalcuate Values
	ComputeAddressRect();

	Invalidate();
}

void CDNAssistView::OnSetFocus(CWnd* pOldWnd)
{
	// take care of caret
	CScrollView::OnSetFocus(pOldWnd);
	m_prevStatus = _T(" ");
	UpdateCtrlStatus();

	// update position on the status bar
	PostMessage(WM_CARET_POS_CHANGED, 0, 0);

	CString strSel;
	int length = m_nSelEnd - m_nSelStart;
	if (length > 0)
		strSel.Format(_T("Selection: %d - %d; Length: %d"), m_nSelStart, m_nSelEnd, length);
	else
		strSel.Format(_T("Selection: 0"));
	theApp.m_pStatusBar->SetPaneText(1, strSel);

	CDNAssistDoc* pDoc = GetDocument();
	theApp.m_pStatusBar->SetPaneText(3, pDoc->GetSeqTypeDescr());
	theApp.m_pStatusBar->SetPaneText(4, pDoc->GetFormDescr());
}

void CDNAssistView::OnKillFocus(CWnd* pNewWnd)
{
	// take care of caret
	DestroyEditCaret();
	CScrollView::OnKillFocus(pNewWnd);
	Invalidate();
}

void CDNAssistView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// st the focus on the window
	SetFocus();

	// we have to allign carret in that area 
	// if the user clicked on any area
	AlignCaret(point);

	// take over the mouse capture 
	SetCapture();

	// deselect any selection
	m_nSelStart = m_nSelEnd = GetCaretPosFileOffset();

	// update position on the status bar
	PostMessage(WM_CARET_POS_CHANGED, 0, 0);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CDNAssistView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetCapture() == this)
		ReleaseCapture();

	CScrollView::OnLButtonUp(nFlags, point);
}

void CDNAssistView::OnMouseMove(UINT nFlags, CPoint point)
{
	// if the left mouse button in pushed then 
	// follow the mouse with caret 
	if (nFlags & MK_LBUTTON)
	{
		CRect rcClient(0, 0, 0, 0);
		GetClientRect(rcClient);
		if (m_ptCaretPosition.y > rcClient.bottom - m_sizeCharCell.cy)
			SendMessage(WM_VSCROLL, SB_LINEDOWN);
		else if (m_ptCaretPosition.y <= 0)
			SendMessage(WM_VSCROLL, SB_LINEUP);

		// allign the caret
		AlignCaret(point);
		// set this position to be new selection end
		m_nSelEnd = GetCaretPosFileOffset();

		if(!VScrollToCaretPos())
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}
}

void CDNAssistView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_SHIFT:
		m_bShiftKeyDown = FALSE;
		break;
	}

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDNAssistView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!EditData(nChar, nRepCnt, nFlags))
		return;

	m_currentPatternIndex = -1;
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
	if (m_bInsert)
		OnKeyDown(VK_RIGHT, nRepCnt, nFlags);
}

void CDNAssistView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// update control data
	UpdateCtrlStatus();
}

void CDNAssistView::OnUpdateIndicatorOvr(CCmdUI* pCmdUI)
{
	// pCmdUI->Enable(!m_wndHexCtrl.m_bInsert);
	pCmdUI->Enable(!m_bInsert);
}

BOOL CDNAssistView::GetCaretPosByte(char& chData)
{
	// if no bytes are read yet then we cannot return anything
	if (m_nBytesRead == 0)
		return FALSE;

	// get how many bytes per line are there
	size_t dwBytesPerLine = theApp.numberOfGroups*m_nBytesPerGroup;

	// get the offset from the begin of the page
	// first get current line
	size_t dwCurLine = m_ptCaretPosition.y / m_sizeCharCell.cy;

	// now lets calculate the offset from the page begin
	size_t dwPageOffset = dwCurLine*dwBytesPerLine + m_nCurPos[0][0];
	if (dwPageOffset<m_nBytesRead)
	{
		chData = (*m_pDataStr)[dwPageOffset];
		return TRUE;
	}
	return FALSE;
}

LRESULT CDNAssistView::OnCaretPosChanged(WPARAM wParam, LPARAM lParam)
{
	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end 
	if (m_nSelStart > m_nSelEnd)
	{
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	// Calcuate how many bytes per line are there
	size_t dwBytesPerLine = m_nBytesPerGroup * theApp.numberOfGroups;
	// display the offset from the file begin
	size_t dwStartLine = nSelStart / dwBytesPerLine;
	// so calculate starting y position
	CString strPos;
	size_t dwPos = GetCaretPosFileOffset();
	strPos.Format(_T("Ln: %d  Pos: %d"), dwStartLine + 1, dwPos + 1);
	theApp.m_pStatusBar->SetPaneText(2, strPos);
	return 0;
}

void CDNAssistView::GetSelectedSeq(string& dataSegment)
{
	// Check is anything seleted
	if ((m_nSelStart == m_nSelEnd) || (m_nBytesRead == 0))
		return;

	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end 
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}
	dataSegment = m_pDataStr->substr(nSelStart, nSelEnd - nSelStart);
}

void CDNAssistView::SetSelectedSeq(const string& dataSegment)
{
	// Check is anything seleted
	if ((m_nSelStart == m_nSelEnd) || (m_nBytesRead == 0))
		return;

	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}
	m_pDataStr->replace(nSelStart, nSelEnd - nSelStart, dataSegment);
	GetDocument()->AddToHistory();
}

void CDNAssistView::SetModifiedFlag(BOOL bModified)
{
	GetDocument()->SetModifiedFlag(bModified);
}

LRESULT CDNAssistView::OnFindText(WPARAM wParam, LPARAM lParam)
{
	BOOL isFindNext = wParam == IDC_FINDAGAIN;
	BOOL isGlobal = m_findDlg.IsGlobal();
	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end 
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	CDNAssistDoc* pDoc = GetDocument();
	int nCurrPos = isFindNext ? nSelStart : -1;
	CPoint pt;
	if (isGlobal)
		pt = m_findDlg.Search(*m_pDataStr, pDoc->GetSeqType(), pDoc->GetSeqForm(), nCurrPos);
	else
		pt = m_findDlg.Search(m_pDataStr->substr(nSelStart), pDoc->GetSeqType(), pDoc->GetSeqForm(), nCurrPos);
	if (pt.x > -1 && pt.x < m_pDataStr->length()) {
		m_nSelStart = pt.x;
		m_nSelEnd = m_nSelStart + pt.y;
		MoveCaretToFilePos(m_nSelStart);
		if (!VScrollToCaretPos(TRUE))
			Invalidate();
	}
	else {
		if(isFindNext)
			AfxMessageBox(_T("There are no further occurrences of the search string."));
		else
			AfxMessageBox(_T("Search string does not occur."));
	}
	return 0;
}

LRESULT CDNAssistView::OnFindORF(WPARAM wParam, LPARAM lParam)
{
	BOOL isFindNext = wParam == IDC_FINDNEXTORF;
	BOOL isGlobal = m_pFindORFDlg.IsGlobal();
	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end 
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	vector<CPoint> points;
	CDNAssistDoc* pDoc = GetDocument();
	auto pCodontable = pDoc->GetCodonTable();

	if (isFindNext) {		
		if (m_nORFIndex < points.size()) {
			m_nSelStart = points[m_nORFIndex].x;
			m_nSelEnd = m_nSelStart + points[m_nORFIndex].y;
			MoveCaretToFilePos(m_nSelStart);
			if (!VScrollToCaretPos(TRUE))
				Invalidate();
			m_nORFIndex++;
			return 0;
		}
		else
			AfxMessageBox(_T("There are no further ORFs in the sequence"));
	}
	else {
		m_nORFIndex = 0;
		if (isGlobal)
			m_pFindORFDlg.SearchForORFs(pCodontable, *m_pDataStr, points, pDoc->GetSeqType(), pDoc->GetSeqForm());
		else
			m_pFindORFDlg.SearchForORFs(pCodontable, m_pDataStr->substr(nSelStart), points, pDoc->GetSeqType(), pDoc->GetSeqForm());
	}

	if (points.empty())
		AfxMessageBox(_T("No ORFs were found"));
	else {
		m_nSelStart = points[0].x;
		m_nSelEnd = m_nSelStart + points[0].y;
		MoveCaretToFilePos(m_nSelStart);
		if (!VScrollToCaretPos(TRUE))
			Invalidate();
	}
	return 0;
}

LRESULT CDNAssistView::OnFindPattern(WPARAM wParam, LPARAM lParam)
{
	CDNAssistDoc* pDoc = GetDocument();
	char sequence_type = pDoc->GetSeqType();	
	if(wParam == IDC_FINDPATTERN)		
		m_findPatternDlg.AdjustListBoxes(sequence_type);
	else if (wParam == IDC_FINDPATTERN_FIND || m_currentPatternIndex < 0) {
		m_currentPatternIndex = 0;
		m_patternLocation.clear(), m_patternLength.clear();

		m_findPatternDlg.FindCallback(*m_pDataStr, sequence_type, pDoc->GetSeqForm(), m_patternLocation, m_patternLength);
		if (m_patternLocation.empty()) {
			AfxMessageBox(_T("Pattern does not occur"));
			return 0;
		}

		size_t numberOfPatternHits = m_patternLocation.size();	
		CQuickSort sortobject;
		m_sortedPatternIndexes = make_unique<int[]>(numberOfPatternHits);
		for (int a = 0; a < numberOfPatternHits; a++)
			m_sortedPatternIndexes[a] = a;
		sortobject.IndirectQuickSort(m_sortedPatternIndexes.get(), &m_patternLocation[0], 0, numberOfPatternHits - 1);

		//::SetCursor(old_cursor);
		m_nSelStart = m_patternLocation[m_sortedPatternIndexes[m_currentPatternIndex]];
		m_nSelEnd = m_nSelStart + m_patternLength[m_sortedPatternIndexes[m_currentPatternIndex]];
		MoveCaretToFilePos(m_nSelStart);
		if (!VScrollToCaretPos(TRUE))
			Invalidate();
	}
	else if (wParam == IDC_FINDPATTERN_FINDNEXT) {
		m_currentPatternIndex++;
		size_t numberOfPatternHits = m_patternLocation.size();
		if (m_currentPatternIndex < numberOfPatternHits) {
			m_nSelStart = m_patternLocation[m_sortedPatternIndexes[m_currentPatternIndex]];
			m_nSelEnd = m_nSelStart + m_patternLength[m_sortedPatternIndexes[m_currentPatternIndex]];
			MoveCaretToFilePos(m_nSelStart);
			if (!VScrollToCaretPos(TRUE))
				Invalidate();
		}
		else
			AfxMessageBox(_T("No further occurrences of the pattern"));
	}
	return 0;
}

void CDNAssistView::OnEditCopy()
{
	string dataSegment;
	GetSelectedSeq(dataSegment);

	HGLOBAL glob = GlobalAlloc(GMEM_FIXED, 32);
	const char* data = dataSegment.c_str();
	memcpy(glob, data, sizeof(data));
	if (OpenClipboard()) {
		EmptyClipboard();
		SetClipboardData(CF_TEXT, glob);
		CloseClipboard();
	}
}

void CDNAssistView::OnEditPaste()
{
	OpenClipboard();
	if (IsClipboardFormatAvailable(CF_HDROP)) {
		HGLOBAL hGlobal = (HGLOBAL)GetClipboardData(CF_HDROP);
		HDROP hDrop = (HDROP)GlobalLock(hGlobal);
		if (hDrop) {
			TCHAR lpszFileName[MAX_PATH];
			DragQueryFile(hDrop, 0, lpszFileName, MAX_PATH);
			CDNAssistDoc* pDoc = GetDocument();
			if (pDoc->IsModified())
				theApp.OpenDocumentFile(lpszFileName, true);
			else
				pDoc->OnOpenDocument(lpszFileName);
			GlobalUnlock(hGlobal);
		}
	}
	else {
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);
		if (hClipboardData) {
			char* pchData = (char*)GlobalLock(hClipboardData);
			if (pchData) {
				DeleteData(VK_DELETE, 0, 0);
				size_t dwPos = GetCaretPosFileOffset();
				string pasteData(pchData);
				m_seqValidator.StripForPaste(GetDocument()->GetSeqType(), pasteData);
				m_pDataStr->insert(dwPos, pasteData);
				GlobalUnlock(hClipboardData);
				if(IsSelection()) {
					int nSelectionExtent = abs((int) m_nSelStart - (int) m_nSelEnd);
					m_pDocVarManager->AddToUndoList(0, pasteData, dwPos, m_nSelStart, nSelectionExtent);
				}
				else
					m_pDocVarManager->AddToUndoList(0, pasteData, dwPos, 0, 0);
			}
		}
	}
	SetModifiedFlag(TRUE);
	CloseClipboard();
}

void CDNAssistView::OnEditSelectAll()
{
	m_nSelStart = 0;
	m_nSelEnd = m_pDataStr->length();
	Invalidate();
}

void CDNAssistView::OnEditCut()
{
	OnEditCopy();
	DeleteData(VK_DELETE, 0, 0);
}

void CDNAssistView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	bool bEnable = IsSelection() && m_nBytesRead > 0;
	pCmdUI->Enable(bEnable);
}


void CDNAssistView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	bool bEnable = IsSelection() && m_nBytesRead > 0;
	pCmdUI->Enable(bEnable);
}


void CDNAssistView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	bool bEnable = IsClipboardFormatAvailable(CF_TEXT) || IsClipboardFormatAvailable(CF_UNICODETEXT);
	pCmdUI->Enable(bEnable);
}


void CDNAssistView::OnUpdateEditSelectAll(CCmdUI *pCmdUI)
{
	bool bEnable = m_nBytesRead > 0;
	pCmdUI->Enable(bEnable);
}

void CDNAssistView::OnUpdateEditFind(CCmdUI *pCmdUI)
{
	bool bEnable = m_nBytesRead > 0 && GetDocument()->GetSeqType() != 'G';
	pCmdUI->Enable(bEnable);
}

void CDNAssistView::CreateNewFrame()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CDNAssistView::ConvertSequence(char outType)
{
	CDNAssistDoc* pDoc = GetDocument();
	string outputseq;
	if (outType != 'P') {
		if (IsSelection()) { // open new window
			string dataSegment;
			GetSelectedSeq(dataSegment);
			CreateNewFrame();

			m_seqConvertor.ConvertSequence(dataSegment, outputseq, pDoc->GetSeqType(), outType);
			CDNAssistDoc* pNewDoc = dynamic_cast<CDNAssistDoc*>(theApp.GetActiveDocument());
			pNewDoc->SetData(outputseq, outType);
		}
		else {
			m_seqConvertor.ConvertSequence(*m_pDataStr, outputseq, pDoc->GetSeqType(), outType);
			pDoc->SetData(outputseq, outType);
			m_pDocVarManager->Clear();
		}
		return;
	}

	CCodonTable* pCodonTable = pDoc->GetCodonTable();
	if (!pDoc->IsCodonTableValid()) {
		AfxMessageBox(_T("The current codon table in not valid"));
		return;
	}

	bool truncateatstop = false;
	if (IsSelection()) { // open new window
		string dataSegment;
		GetSelectedSeq(dataSegment);
		CreateNewFrame();

		truncateatstop = m_seqConvertor.ConvertSequence(dataSegment, outputseq, pDoc->GetSeqType(), outType, pCodonTable) > 0;
		CDNAssistDoc* pNewDoc = dynamic_cast<CDNAssistDoc*>(theApp.GetActiveDocument());
		if (truncateatstop)
			AfxMessageBox(_T("The converted sequence will be truncated \nat an in-frame STOP codon."));
		pNewDoc->SetData(outputseq, outType);
	}
	else {
		truncateatstop = m_seqConvertor.ConvertSequence(*m_pDataStr, outputseq, pDoc->GetSeqType(), outType, pCodonTable) > 0;
		if (truncateatstop)
			AfxMessageBox(_T("The converted sequence will be truncated \nat an in-frame STOP codon."));
		pDoc->SetData(outputseq, outType);
		m_pDocVarManager->Clear();
	}
}

void CDNAssistView::OnConvertComplement()
{
	CDNAssistDoc* pDoc = GetDocument();
	char outType = pDoc->GetSeqType();
	string outputseq;
	if (IsSelection()) { // open new window
		string dataSegment;
		GetSelectedSeq(dataSegment);

		m_seqConvertor.SequenceReverseComplement(dataSegment, outputseq, outType);
		if(theApp.ConversionOutputTransferBuffer == newwindow) {
			CreateNewFrame();
			CDNAssistDoc* pNewDoc = dynamic_cast<CDNAssistDoc*>(theApp.GetActiveDocument());
			pNewDoc->SetData(outputseq, outType);
		}
		else {
			SetSelectedSeq(outputseq);
			int nSelectionExtent = abs((int) m_nSelStart - (int) m_nSelEnd);
			m_pDocVarManager->AddToUndoList(0, outputseq, GetCaretPosFileOffset(), m_nSelStart, nSelectionExtent);
		}
	}
	else {
		m_seqConvertor.SequenceReverseComplement(*m_pDataStr, outputseq, outType);
		pDoc->SetData(outputseq, outType);
		m_pDocVarManager->AddToUndoList(0, outputseq, GetCaretPosFileOffset(), 0, 0);
	}
}

void CDNAssistView::OnConvertProToDna()
{
	CDNAssistDoc* pDoc = GetDocument();
	CCodonTable* pCodonTable = pDoc->GetCodonTable();
	if (!pDoc->IsCodonTableValid()) {
		AfxMessageBox(_T("The current codon table in not valid"));
		return;
	}

	string outputseq;
	if (IsSelection()) { // open new window
		string dataSegment;
		GetSelectedSeq(dataSegment);
		CreateNewFrame();

		m_seqConvertor.ReverseTranslate(dataSegment, outputseq, pCodonTable);
		CDNAssistDoc* pNewDoc = dynamic_cast<CDNAssistDoc*>(theApp.GetActiveDocument());
		pNewDoc->SetData(outputseq, 'G');
	}
	else {
		m_seqConvertor.ReverseTranslate(*m_pDataStr, outputseq, pCodonTable);
		pDoc->SetData(outputseq, 'G');
		m_pDocVarManager->Clear();
	}
}

void CDNAssistView::OnConvertDnaToRna()
{
	ConvertSequence('R');
}

void CDNAssistView::OnConvertRnaToDna()
{
	ConvertSequence('D');
}

void CDNAssistView::OnConvertDnaToPro()
{
	ConvertSequence('P');
}

void CDNAssistView::OnConvertRnatoPro()
{
	ConvertSequence('P');
}

void CDNAssistView::OnAnalyzeRandomize()
{
	CDNAssistDoc* pDoc = GetDocument();
	char outType = pDoc->GetSeqType();
	string outputseq;
	if (IsSelection()) { // open new window
		string dataSegment;
		GetSelectedSeq(dataSegment);

		m_seqConvertor.RandomizeSequence(dataSegment, outputseq);
		if(theApp.ConversionOutputTransferBuffer == newwindow) {
			CreateNewFrame();
			CDNAssistDoc* pNewDoc = dynamic_cast<CDNAssistDoc*>(theApp.GetActiveDocument());
			pNewDoc->SetData(outputseq, outType);
		}
		else {
			SetSelectedSeq(outputseq);
			int nSelectionExtent = abs((int) m_nSelStart - (int) m_nSelEnd);
			m_pDocVarManager->AddToUndoList(0, outputseq, GetCaretPosFileOffset(), m_nSelStart, nSelectionExtent);
		}
	}
	else {
		m_seqConvertor.RandomizeSequence(*m_pDataStr, outputseq);
		pDoc->SetData(outputseq, outType);
		m_pDocVarManager->AddToUndoList(0, outputseq, GetCaretPosFileOffset(), 0, 0);
	}
}

void CDNAssistView::ShowDialog(CDialog* pDialog)
{
	pDialog->SetWindowPos(&this->wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	pDialog->CenterWindow();
	pDialog->ShowWindow(SW_SHOW);
}

void CDNAssistView::OnEditFind()
{
	if (m_pFindORFDlg.m_hWnd)
		m_pFindORFDlg.ShowWindow(SW_HIDE);
	if (m_findPatternDlg.m_hWnd)
		m_findPatternDlg.ShowWindow(SW_HIDE);
	if (NULL == m_findDlg.m_hWnd)
		m_findDlg.Create(CFindDialog::IDD);
	m_findDlg.MakeTranspanency();
	ShowDialog(&m_findDlg);
}

void CDNAssistView::OnAnalyzeFindorfs()
{
	if (m_findDlg.m_hWnd)
		m_findDlg.ShowWindow(SW_HIDE);
	if (m_findPatternDlg.m_hWnd)
		m_findPatternDlg.ShowWindow(SW_HIDE);
	if (NULL == m_pFindORFDlg.m_hWnd)
		m_pFindORFDlg.Create(CFindORFDialog::IDD);
	m_pFindORFDlg.MakeTranspanency();
	ShowDialog(&m_pFindORFDlg);
}

void CDNAssistView::OnAnalyzeFindpattern()
{
	if (m_findDlg.m_hWnd)
		m_findDlg.ShowWindow(SW_HIDE);
	if (m_pFindORFDlg.m_hWnd)
		m_pFindORFDlg.ShowWindow(SW_HIDE);
	if (NULL == m_findPatternDlg.m_hWnd)
		m_findPatternDlg.Create(CFindPatternDialog::IDD);
	m_currentPatternIndex = 0;
	m_findPatternDlg.MakeTranspanency();
	m_findPatternDlg.AdjustListBoxes(GetDocument()->GetSeqType());
	ShowDialog(&m_findPatternDlg);
}

void CDNAssistView::OnPropertiesPhysicochemical()
{
	string selectedSeq;
	if (IsSelection())
		GetSelectedSeq(selectedSeq);
	else
		selectedSeq = *m_pDataStr;

	char seqType = GetDocument()->GetSeqType();
	if (seqType == 'D' || seqType == 'R') {
		if (selectedSeq.length() > 2) {
			CHelixParamsDialog helixParamsDialog(selectedSeq, seqType);
			helixParamsDialog.DoModal();
		}
		else {
			CString msg;
			msg.LoadString(IDS_REQUIREMORETHANTWO);
			AfxMessageBox(msg);
		}
	}
	else if (seqType == 'P' && selectedSeq.length() > 0) {
		CProteinPropertiesDialog proteinPropertiesDialog(selectedSeq);
		proteinPropertiesDialog.DoModal();
	}
}

void CDNAssistView::OnAnalyzeTranscriptionfactors()
{
	m_pLof = make_unique<CEnzymeList>();
	CTransFacPropertySheet transFacPropertySheet(_T("Transcription Factors"), m_pLof.get(), this, 0);
	transFacPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if(transFacPropertySheet.DoModal() != IDOK)
		return;

	if(m_pLof->GetNumberOfEnzymes() == 0)
		return;

	m_pLof->SortList();

	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	int displayformat = theApp.GetTransFacFormatOptions();
	int analysisoptions = theApp.GetTransFacOptions();
	CDNAssistDoc* pDoc = GetDocument();

	CString childtitle = pDoc->GetTitle();
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	BeginWaitCursor();	
	int transfacId = theApp.GetTransfacId();
	m_pRcl = make_unique<CRestrictionCutList>();
	bool bAnalysted = false;
	if (IsSelection()) {
		string dataSegment;
		GetSelectedSeq(dataSegment);

		if (displayformat & DF_GRAPHIC) {
			CResultDoc* pResultDoc = theApp.OnNewResultDoc();
			if (transfacId > 0)
				tempTitle.Format(_T("TransFac Graphic Map - %s [%d]"), childtitle, transfacId);
			else
				tempTitle.Format(_T("TransFac Graphic Map - %s"), childtitle);
			bAnalysted = pResultDoc->TranscriptionFactors(dataSegment, pDoc->GetSeqType(), pDoc->GetSeqForm(), m_pLof.get(), m_pRcl.get());
			pResultDoc->SetData(dataSegment, GetSelStart(), dataSegment.length(),
				DF_GRAPHIC, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc->SetTitle(tempTitle);
			displayformat ^= DF_GRAPHIC;
		}

		if (displayformat) {
			CResultDoc* pResultDoc2 = theApp.OnNewResultDoc();
			if (transfacId > 0)
				tempTitle.Format(_T("TransFac Analysis - %s [%d]"), childtitle, transfacId);
			else
				tempTitle.Format(_T("TransFac Analysis - %s"), childtitle);
			if(!bAnalysted)
				bAnalysted = pResultDoc2->TranscriptionFactors(dataSegment, pDoc->GetSeqType(), pDoc->GetSeqForm(), m_pLof.get(), m_pRcl.get());
			pResultDoc2->SetData(dataSegment, GetSelStart(), dataSegment.length(),
				displayformat, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc2->SetTitle(tempTitle);
		}
	}
	else {

		if (displayformat & DF_GRAPHIC) {
			CResultDoc* pResultDoc = theApp.OnNewResultDoc();
			if (transfacId > 0)
				tempTitle.Format(_T("TransFac Graphic Map - %s [%d]"), childtitle, transfacId);
			else
				tempTitle.Format(_T("TransFac Graphic Map - %s"), childtitle);
			bAnalysted = pResultDoc->TranscriptionFactors(*m_pDataStr, pDoc->GetSeqType(), pDoc->GetSeqForm(), m_pLof.get(), m_pRcl.get());
			pResultDoc->SetData(*m_pDataStr, 1, m_pDataStr->length(),
				DF_GRAPHIC, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc->SetTitle(tempTitle);
			displayformat ^= DF_GRAPHIC;
		}
		
		//	See if the user want anything else
		if (displayformat) {
			CResultDoc* pResultDoc2 = theApp.OnNewResultDoc();
			if (transfacId > 0)
				tempTitle.Format(_T("TransFac Analysis - %s [%d]"), childtitle, transfacId);
			else
				tempTitle.Format(_T("TransFac Analysis - %s"), childtitle);
			if (!bAnalysted)
				bAnalysted = pResultDoc2->TranscriptionFactors(*m_pDataStr, pDoc->GetSeqType(), pDoc->GetSeqForm(), m_pLof.get(), m_pRcl.get());
			pResultDoc2->SetData(*m_pDataStr, 1, m_pDataStr->length(),
				displayformat, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc2->SetTitle(tempTitle);
		}
	}
	EndWaitCursor();
}

void CDNAssistView::OnAnalyzeRestrictionMapping()
{
	CRestrictEnzymesPropertySheet restrictEnzymesPropertySheet(_T("Restriction Enzymes"), this, 0);
	restrictEnzymesPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if(restrictEnzymesPropertySheet.DoModal() != IDOK)
		return;

	CEnzymeList* enzymeList = restrictEnzymesPropertySheet.GetEnzymeList();
	if(!enzymeList->GetNumberOfEnzymes()) {
		AfxMessageBox(_T("You did not select any enzymes."));
		return;
	}

	if(theApp.GetEnzymeOptionsTransferBuffer()->selection) {
		enzymeList->RetainSelected();
		if(!enzymeList->GetNumberOfEnzymes()) {
			AfxMessageBox(_T("You chose to use only the selected enzymes,\nbut did not select any enzymes."));
			return;
		}
	}

	enzymeList->SortList();

	size_t nSelStart = m_nSelStart;
	size_t nSelEnd = m_nSelEnd;

	// Check is start after the end
	if (m_nSelStart > m_nSelEnd) {
		nSelStart = m_nSelEnd;
		nSelEnd = m_nSelStart;
	}

	int displayformat = theApp.GetTransFacFormatOptions();
	int analysisoptions = theApp.GetTransFacOptions();
	CDNAssistDoc* pDoc = GetDocument();

	CString childtitle = pDoc->GetTitle();
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	BeginWaitCursor();	
	int restrictionId = theApp.GetRestrictionId();
	m_pRcl = make_unique<CRestrictionCutList>();
	bool bAnalysted = false;
	if (IsSelection()) {
		string dataSegment;
		GetSelectedSeq(dataSegment);

		if (displayformat & DF_GRAPHIC) {
			CResultDoc* pResultDoc = theApp.OnNewResultDoc();
			if (restrictionId > 0)
				tempTitle.Format(_T("Graphic Map - %s [%d]"), childtitle, restrictionId);
			else
				tempTitle.Format(_T("Graphic Map - %s"), childtitle);
			bAnalysted = pResultDoc->RestrictionMapping(dataSegment, pDoc->GetSeqType(), pDoc->GetSeqForm(), enzymeList, m_pRcl.get());
			pResultDoc->SetData(dataSegment, GetSelStart(), dataSegment.length(),
				DF_GRAPHIC, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc->SetTitle(tempTitle);
			displayformat ^= DF_GRAPHIC;
		}

		if (displayformat) {
			CResultDoc* pResultDoc2 = theApp.OnNewResultDoc();
			if (restrictionId > 0)
				tempTitle.Format(_T("Restriction Map - %s [%d]"), childtitle, restrictionId);
			else
				tempTitle.Format(_T("Restriction Map - %s"), childtitle);
			if (!bAnalysted)
				bAnalysted = pResultDoc2->RestrictionMapping(dataSegment, pDoc->GetSeqType(), pDoc->GetSeqForm(), enzymeList, m_pRcl.get());
			pResultDoc2->SetData(dataSegment, GetSelStart(), dataSegment.length(),
				displayformat, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc2->SetTitle(tempTitle);
		}
	}
	else {

		if (displayformat & DF_GRAPHIC) {
			CResultDoc* pResultDoc = theApp.OnNewResultDoc();
			if (restrictionId > 0)
				tempTitle.Format(_T("Graphic Map - %s [%d]"), childtitle, restrictionId);
			else
				tempTitle.Format(_T("Graphic Map - %s"), childtitle);
			bAnalysted = pResultDoc->RestrictionMapping(*m_pDataStr, pDoc->GetSeqType(), pDoc->GetSeqForm(), enzymeList, m_pRcl.get());
			pResultDoc->SetData(*m_pDataStr, 1, m_pDataStr->length(),
				DF_GRAPHIC, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc->SetTitle(tempTitle);
			displayformat ^= DF_GRAPHIC;
		}
		
		//	See if the user want anything else
		if (displayformat) {
			CResultDoc* pResultDoc2 = theApp.OnNewResultDoc();
			if (restrictionId > 0)
				tempTitle.Format(_T("Restriction Map - %s [%d]"), childtitle, restrictionId);
			else
				tempTitle.Format(_T("Restriction Map - %s"), childtitle);
			if (!bAnalysted)
				bAnalysted = pResultDoc2->RestrictionMapping(*m_pDataStr, pDoc->GetSeqType(), pDoc->GetSeqForm(), enzymeList, m_pRcl.get());
			pResultDoc2->SetData(*m_pDataStr, 1, m_pDataStr->length(),
				displayformat, analysisoptions, m_pRcl.get(), pDoc->GetSeqType(), pDoc->GetSeqForm());
			pResultDoc2->SetTitle(tempTitle);
		}
		
	}
	EndWaitCursor();
}

void CDNAssistView::OnAnalyzeList()
{
	CDNAssistDoc* pDoc = GetDocument();
	CString childtitle = _T("Sequence List - ") + pDoc->GetTitle();
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	BeginWaitCursor();
	CResultDoc* pResultDoc = theApp.OnNewResultDoc();
	int seqListId = theApp.GetSeqListId();
	if (seqListId > 0)
		tempTitle.Format(_T("%s [%d]"), childtitle, seqListId);
	else
		tempTitle.Format(_T("%s"), childtitle);
	if (IsSelection()) {
		string dataSegment;
		GetSelectedSeq(dataSegment);
		pResultDoc->SetData(dataSegment, GetSelStart(), dataSegment.length(),
			DF_LIST, EL_GRAPHICUNIQUE, nullptr, pDoc->GetSeqType(), pDoc->GetSeqForm());
	}
	else {
		pResultDoc->SetData(*m_pDataStr, 1, m_pDataStr->length(),
			DF_LIST, EL_GRAPHICUNIQUE, nullptr, pDoc->GetSeqType(), pDoc->GetSeqForm());
	}
	pResultDoc->SetTitle(tempTitle);
	EndWaitCursor();
}

bool CalculateNucleosomeProc(const CUPDUPDATA* pCUPDUPData)
{	
    //Retrieve the App Supplied Data
    auto pnCount= (int*) pCUPDUPData->GetAppData();

    while(pCUPDUPData->ShouldTerminate() == false && *pnCount != 100) {
        pCUPDUPData->SetProgress(*pnCount); //Update Progress Bar
        Sleep(250);
    }

    pCUPDUPData->SetProgress(_T("Done !!"), 100);    
    return true;
}

void CDNAssistView::CalculateNucleosome(const CString& title, const string& dataStr, int selStart)
{
	m_pDrewPlotDoc = theApp.OnNewDrewPlot();
	m_pDrewPlotDoc->Xaxistitle = _T("Dyad Position");
	m_pDrewPlotDoc->Yaxistitle = _T("Probability");

	CString childtitle = _T("Nucleosome Positions - ") + title;
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;
	int nucleosomeId = theApp.GetNucleosomeId();
	if (nucleosomeId > 0)
		tempTitle.Format(_T("%s [%d]"), childtitle, nucleosomeId);
	else
		tempTitle.Format(_T("%s"), childtitle);
	m_pDrewPlotDoc->SetTitle(tempTitle);

	int nData = 0;
	m_caption = L"Calculating..";
	CUPDialog waitDlg(AfxGetMainWnd()->GetSafeHwnd(), CalculateNucleosomeProc, &nData, &m_caption, false);
	m_pDrewPlotDoc->CalculateNucleosome(dataStr, &nData, selStart);
	waitDlg.DoModal();
}

void CDNAssistView::OnAnalyzeNucleosomepositions()
{
	CDNAssistDoc* pDoc = GetDocument();
	if(IsSelection()) {
		string dataSegment;
		GetSelectedSeq(dataSegment);
		if(dataSegment.length() >= 125)
			CalculateNucleosome(pDoc->GetTitle(), dataSegment, GetSelStart());
	}
	else if(m_pDataStr->length() >= 125)
		CalculateNucleosome(pDoc->GetTitle(), *m_pDataStr);
	else
		AfxMessageBox(_T("Length of sequence or\nselection must be larger\nthan 124bp."));
}

void CDNAssistView::OnEditUndo()
{
	auto undostruct = m_pDocVarManager->TakeFromUndoList();
	if(!undostruct)
		return;

	size_t extent = undostruct->data.length();
	if(undostruct->type == 1) {//This must be inserted; Do not use this HPEditWindow's InsertString()
									//because that will just put the operation back in the undolist
		SetModifiedFlag(TRUE);
		m_pDataStr->insert(undostruct->writePosition, undostruct->data);
	}
	else {
	   int start = undostruct->writePosition;
	   SetModifiedFlag(TRUE);
		m_pDataStr->erase(start, extent);
	}

	if(undostruct->selectionExtent != m_pDataStr->length() + extent) {
		m_nSelStart = undostruct->selectionStart;
		m_nSelEnd = m_nSelStart + undostruct->selectionExtent;
	}
	else
		m_nSelStart =  m_nSelEnd = 0;

	MoveCaretToFilePos(undostruct->writePosition);
	if (!VScrollToCaretPos(TRUE))
		Invalidate();
}


void CDNAssistView::OnEditRedo()
{
	auto undostruct = m_pDocVarManager->GetRedoOperation();
	if(!undostruct)
		return;

	size_t extent = undostruct->data.length();
	if(undostruct->type == 0) {//This must be inserted; Do not use this HPEditWindow's InsertString()
									//because that will just put the operation back in the undolist
		m_pDataStr->insert(undostruct->writePosition, undostruct->data);
	}
	else {
		int start = undostruct->writePosition;
		m_pDataStr->erase(start, extent);
	}

	m_nSelStart = undostruct->selectionStart;
	m_nSelEnd = m_nSelStart + undostruct->selectionExtent;

	MoveCaretToFilePos(undostruct->writePosition);
	if (!VScrollToCaretPos(TRUE))
		Invalidate();
}

void CDNAssistView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	bool bEnable = m_pDocVarManager->CanUndo();
	pCmdUI->Enable(bEnable);
}

void CDNAssistView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	bool bEnable = m_pDocVarManager->CanRedo();
	pCmdUI->Enable(bEnable);
}

void CDNAssistView::OnDestroy()
{
	const CString BLANK = _T("");
	for(int i=1; i<5; ++i)
		theApp.m_pStatusBar->SetPaneText(i, BLANK);
	CScrollView::OnDestroy();
}

// CDNAssistView diagnostics

#ifdef _DEBUG
void CDNAssistView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDNAssistView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDNAssistDoc* CDNAssistView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDNAssistDoc)));
	return (CDNAssistDoc*)m_pDocument;
}
#endif //_DEBUG

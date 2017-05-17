// DNAssistView.h : interface of the CDNAssistView class
//

#pragma once
#include <map>
#include <vector>

#include "model/DocVarManager.h"
#include "model/EnzymeList.h"
#include "model/SeqConvertor.h"
#include "model/SeqValidator.h"

#include "DNAssistDoc.h"
#include "DrewPlotDoc.h"
#include "FindDialog.h"
#include "FindORFDialog.h"
#include "FindPatternDialog.h"

class CDNAssistCntrItem;

// message sent to the parent window which signals that the user changed the caret position
#define WM_CARET_POS_CHANGED	(WM_USER+0x1)

using namespace std;

class CDNAssistView : public CScrollView
{
private:
	vector<int> m_patternLocation, m_patternLength;
	unique_ptr<int[]> m_sortedPatternIndexes;
	int m_currentPatternIndex;
	CString m_prevStatus;
	wstring m_caption;

	unique_ptr<CDocVarManager> m_pDocVarManager;
	unique_ptr<CEnzymeList> m_pLof;
	unique_ptr<CRestrictionCutList> m_pRcl;
	CSeqConvertor m_seqConvertor;
	CSeqValidator m_seqValidator;

	CFindDialog m_findDlg;
	CFindORFDialog m_pFindORFDlg;
	CFindPatternDialog m_findPatternDlg;
	
	CDrewPlotDoc* m_pDrewPlotDoc;

	void CreateNewFrame();
	void ConvertSequence(char outType);

protected:
	HCURSOR m_hCursorIBean;

	//! Array of all bytes that are painted with special color like CR, LF, FF ...
	std::map<BYTE, COLORREF> m_mapSpecialChars;

	//! TRUE if the Shift key is down for selection
	BOOL m_bShiftKeyDown;

	//! Number of bytes lastly read from the file
	size_t m_nBytesRead;

	size_t m_nORFIndex;

	//! Contains the map from char to ASCII character for ascii display
	std::map<unsigned char, TCHAR> m_mapASCII;

	/*! Contains the offset in characters from the data display begin
	where the byte is represented. This is used for caret movement
	when the user edits Seq data. */
	std::vector<int> m_vecCharOffset;

	/*! Contains current byte (not char) position of the caret.
	The second value is denoting are we on the firs or second nibble.
	This is relevant only if we are in Seq edit area -> ascii is different.*/
	int m_nCurPos[1][2];

	//! Contains the font used for data display
	CFont m_Font;

	//! Is inser or overwite activated
	BOOL m_bInsert;

	//! Contians the size of the character cell
	CSize m_sizeCharCell;

	//! Contains the caret position with respect to current screen
	CPoint m_ptCaretPosition;

	//! Starting position of the selection relative to the file begin
	size_t m_nSelStart;

	//! End selection byte relative to the file begin
	size_t m_nSelEnd;

	size_t m_nPrevPageStartOffset;

	//! Contains the coordinates of the area where the data is presented in Seq form
	CRect m_rectDataSeq;

	//! Contains the coordinates of the area where the addresses are drawn
	CRect m_rectAddr;

	string* m_pDataStr;
	
	//For printing
	string m_printBuffer;
	size_t nLinesPerPageToPrint;
	size_t nCharsPerLineToPrint;

	//! Contains number of lines of the data file
	size_t m_nTotalLines;

	//! Contains the column grouping or number of columns per group
	size_t m_nBytesPerGroup;

	//! Contains the number of chars the groups are spaced away from each other
	size_t m_nGroupSpacing;

	//! Contains the number of chars each column is spaced from each other
	size_t m_nByteSpacing;

	//! Contains how many chars is the address display long
	size_t m_nAddrCharLen;

	const short m_nPageWidth = 11520; // 8 inches
	const short m_nPageHeight = -15120; // 10.5 inches

	//! Address background color
	COLORREF m_colorAddrBkgnd;

	//! Address text color
	COLORREF m_colorAddrText;

	//! Seq Data Background color
	COLORREF m_colorSeqDataBkgnd;

	//! Seq Data Text color (not seleted)
	COLORREF m_colorSeqDataText;

	//! Seq Data Background color of the selected text
	COLORREF m_colorSeqDataSelectedBkgnd;

	//! Seq Data text color of the selected text
	COLORREF m_colorSeqDataSelectedText;

	//! Loads default font for display
	void LoadDefaultFont();

	//! Determines the size of the char cell
	void ComputeCharCellSize();

	//! Calcuates the number of lines this file has based on the grouping
	void ComputeTotalLines();

	//! Calcualtes the size of the rectangle where the addresses will be drawn
	virtual void ComputeAddressRect();

	//! Calcualtes the size of the rectangle where Seq data is drawn
	virtual void ComputeSeqDataRect();

	//! Computes character offsets of each byte in the line according to layout
	void ComputeCharOffset();

	//! Computes address display width in chars
	void ComputeAddressCharLen();	

	//! Creates edit caret in shape of the box with area of character size
	virtual void CreateEditCaret();

	//! Destroys the edit caret
	virtual void DestroyEditCaret();


	// Painting
	//! Paints the rectangle where the addresses are displayed
	void PaintAddrRect(CDC* pDC);

	//! Paints the addresses
	void PaintAddresses(CDC* pDC);

	//! Paints the rectangle where the Seq data is displayed
	void PaintSeqDataRect(CDC* pDC);

	//! Paints Seq data to the Seq data area
	virtual void PaintSeqData(CDC* pDC);

	//! Paints selection if there is any
	void PaintSeqSelection(CDC* pDC);

	// Caret movement
	//! Returns next caret position (no matter is it Seq or ascii)
	// This value is multiplied with char widht for coordinate calculation
	int GetNextCaretPos(BOOL bJumpByte);

	//! Returns previous caret position (no matter is it Seq or ascii)
	// This value is multiplied with char widht for coordinate calculation
	int GetPrevCaretPos(BOOL bJumpByte);

	//! Determines are we at the line end
	BOOL IsLineEnd()
	{
		return((m_nCurPos[0][0] == (m_nBytesPerGroup * theApp.numberOfGroups - 1)) && (m_nCurPos[0][1] == 2));
	};

	//! Determines are we at the line begin
	BOOL IsLineBegin()
	{
		return((m_nCurPos[0][0] == 0) && (m_nCurPos[0][1] == 1));
	};


	//! Alligns carret to the closest byte position
	void AlignCaret(CPoint &point);

	//! Aligns caret in the Seq area to the nearest byte
	void AlignCaretSeq(CPoint &point);

	//! Aligns specified column to the closest valid column
	void AlignColumnAndSetPosSeq(int &nCol);

	//! Formats the bytes and converts them to Seq with proper spacing 
	virtual void FormatBytesSeq(string& strRes, string& dataSegment, size_t nCount);

	//! Formats a bunch of bytes and prepares them for selection
	/*! It returns a vector of strings which will be painted in different color.
	The parameter dwStart is zero based starting point of the first line.*/
	void FormatSelLineSeq(std::vector<string> &vecRes, size_t dwStart, string& dataSegment, size_t nCount);

	//! Moves caret up and scrolles if necessary; Returns true if caret moved up
	BOOL MoveCaretUp();

	//! Moves caret down and scrolles the screen if necessary; returns true if caret moved down
	BOOL MoveCaretDown();

	//! Moves caret left 
	void MoveCaretLeft(BOOL bJumpByte);

	//! Moves caret right
	void MoveCaretRight(BOOL bJumpByte);

	//! Moves caret to the position in the file specified
	void MoveCaretToFilePos(size_t dwPos);

	//! Handels char editing
	virtual BOOL EditData(UINT nChar, UINT nRepCnt, UINT nFlags);

	//! Deletes a byte (usually when user pushes delete)
	virtual void DeleteData(UINT nChar, UINT nRepCnt, UINT nFlags);	

	void SetModifiedFlag(BOOL bModified = 1);

	void ShowDialog(CDialog* pDialog);

	CDNAssistView();
	DECLARE_DYNCREATE(CDNAssistView)

	// Operations
public:
	CDNAssistDoc* GetDocument() const;

	void GetSelectedSeq(string& dataSegment);
	void SetSelectedSeq(const string& dataSegment);

	//! Updates the status of the control
	void UpdateCtrlStatus();

	inline bool IsSelection() const
	{
		return (m_nSelEnd - m_nSelStart) > 0;
	}
	inline size_t GetSelStart() const
	{
		return m_nSelStart + 1;
	}

	//! Data string
	inline void SetData(string* pDataStr) {
		m_pDataStr = pDataStr;
	}	

	//! Returns the file offset from caret position where the caret currently is
	size_t GetCaretPosFileOffset();

	//! Returns the value of the active byte
	BOOL GetCaretPosByte(char& chData);

	//! Increases/Decreases the font size
	/*! Increases/Decreases the font size. If the limit is reached the
	function returnes FALSE otherwise it returns true. */
	BOOL ChangeFontSize(BOOL bIncrease);

	inline HCURSOR GetHCursor()
	{
		return m_hCursorIBean;
	}

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDCView);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	void CalculateNucleosome(const CString& title, const string& dataStr, int selStart = 0);

	// Implementation
public:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual ~CDNAssistView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL VScrollToCaretPos(BOOL bDoScroll = FALSE);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateIndicatorOvr(CCmdUI* pCmdUI);
	afx_msg LRESULT OnCaretPosChanged(WPARAM wParam, LPARAM lParam);	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnFindText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindORF(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindPattern(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnConvertComplement();
	afx_msg void OnConvertDnaToRna();
	afx_msg void OnConvertRnaToDna();
	afx_msg void OnAnalyzeRandomize();
	afx_msg void OnAnalyzeFindorfs();
	afx_msg void OnAnalyzeFindpattern();
	afx_msg void OnPropertiesPhysicochemical();
	afx_msg void OnDestroy();
	afx_msg void OnConvertProToDna();
	afx_msg void OnConvertDnaToPro();
	afx_msg void OnConvertRnatoPro();
	afx_msg void OnAnalyzeTranscriptionfactors();
	afx_msg void OnAnalyzeRestrictionMapping();
	afx_msg void OnAnalyzeList();
	afx_msg void OnAnalyzeNucleosomepositions();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);	
};

#ifndef _DEBUG  // debug version in SeqView.cpp
inline CDNAssistDoc* CDNAssistView::GetDocument() const
{
	return reinterpret_cast<CDNAssistDoc*>(m_pDocument);
}
#endif


// AlignView.h : interface of the CAlignView class
//

#pragma once
#include "MemDC.h"
#include "AlignDoc.h"

#include <memory>
#include <string>
#include <vector>

#include "model/CodonTable.h"
#include "model/PrintItemRowList.h"

class CDNAssistCntrItem;

using namespace std;

class CAlignView : public CScrollView
{
private:
	//! Address background color
	COLORREF m_colorAddrBkgnd;
	vector<vector<int> > sequencenumbers;

protected: // create from serialization only
	CAlignView();
	DECLARE_DYNCREATE(CAlignView)

protected:	
	BOOL selection;
	int identicalcutoff;
	int conservedcutoff;
	int left_offset;
	int top_offset;
	int titlestart;
	int firstpositionstart;
	int cumulatedwheelrotation;
	int sequencestart;
	int secondpositionstart;
	int numberofsinglesequencerows;
	int sequencerowlength;	
	COLORREF identicalcolor, conservedcolor, conservedpencolor, identicalpencolor;
	CRect margin_inches;
	CSize characterdimension, printsize_pixels, pagesize_inches, pagesize_pixelsperinch, rowdimension;
	vector<int> pagebreaks;
	unique_ptr<CFont> font;
	unique_ptr<CMemoryDC> m_pDC;

	void SetPageLayout();
	CFont* GetCurrentFont();
	CPoint GetCharacterDimension(CDC* pDC);
	int GetLengthOfOneSequenceRow(CDC* pDC);
	void GetStartOfPrintItems();
	int GetNumberOfRows();
	void AssignSequenceNumbers();
	void TruncateTitles();
	int GetTotalNumberOfRows();
	CSize ConvertPaintRectToRowNumbers(CRect& rect);
	CSize ConvertRowNumberToRowGroupNumber(CSize row);
	CPoint GetPageSizeInInches(CDC* pDC);
	CRect GetPrintMargins(CDC* pDC);
	CPoint GetPagePixelsPerInch(CDC* pDC);
	CPoint GetPrintSizeInPixels(CDC* pDC);
	CPoint GetRowDimension(CDC* pDC);
	int GetLengthOfOneSequenceRow();
	int GetNumberOfRowsThatFitOnOnePage();
	void Paginate();
	int GetNumberOfRowGroupsPerPage();
	CRect GetClientPosition();
	//! Updates the status of the control
	void UpdateCtrlStatus();
	inline vector<int>* GetPageBreaks() {
		return &pagebreaks;
	}
	inline size_t GetTotalNumberOfPages() {
		return pagebreaks.size();
	}

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Overrides
protected:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDCView);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//! Paints Seq data to the Seq data area
	virtual void PaintSeqData(CDC* pDC, CPrintInfo* pInfo);
	virtual void SetModifiedFlag(BOOL bModified = 1);

// Implementation
public:
	CAlignDoc* GetDocument() const;
	string GetTextFormat();
	string GetRTFText();
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual ~CAlignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	
	DECLARE_MESSAGE_MAP()	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditCopy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
};

#ifndef _DEBUG  // debug version in SeqView.cpp
inline CAlignDoc* CAlignView::GetDocument() const
{
	return reinterpret_cast<CAlignDoc*>(m_pDocument);
}
#endif

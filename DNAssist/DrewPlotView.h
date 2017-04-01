
// DNAssistView.h : interface of the CDrewPlotView class
//

#pragma once
#include "DrewPlotDoc.h"
#include "MemDC.h"

#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class CDNAssistCntrItem;

class CDrewPlotView : public CScrollView
{
private:
	//! Address background color
	COLORREF m_colorAddrBkgnd;
	COLORREF m_colorSeqDataBkgnd, m_colorSeqDataText;

protected: // create from serialization only
	CDrewPlotView();	
	DECLARE_DYNCREATE(CDrewPlotView)

// Operations
public:
	CDrewPlotDoc* GetDocument() const;

	CSize character_dimension; //width and height of a character
	CSize rowcell_dimension; // width and height of character in row cell
	int GetPageHeight();
	int GetPageWidth();
	int GetLinesPerPage();
	void SetFontDimensions(CDC* pDC);
	void SetGraphicBlockDimension();
	int GetTotalNumberOfPages();
	vector<int> GetPageBreaks();
	int GetNumberOfBlocksPerPage();
	int GetTotalNumberOfBlocks();
	void Paginate();
	void SaveAsEnhancedMetaFile(LPCTSTR lpszFilePath);

protected:
	BOOL selection;	
	int horpixels;
	int vertpixels;
	int pageprintwidth;
	int pageprintheight;	
	int width;
	int height;
	int leftmargin;
	int rightmargin;
	int topmargin;
	int bottommargin;
	int leftoffset;
	int blockheight;
	int blockwidth;
	int blocksperpage;
	char plottype;

	size_t sequence_length; //length of the sequence to list
	int start;
	int points_per_block;
	int block_width;
	int block_height;
	int block_left;
	int block_right;
	int block_bottom;
	int block_left_gap;
	int block_right_gap;
	int block_top;
	int block_gap; //length of the gaps between sequence blocks
	int number_blocks;
	int blocks_per_page;
	int point_dimension;
	int tickside;
	int tick1;
	int tick10;
	int tick100;
	int minimum;
	int maximum;
	int mapmode;
	int cumulatedwheelrotation;
	float Yscale;	
	CPoint start_and_end_blocks;
	LOGFONT logfont, labellogfont, titlelogfont;
	vector<int> pagebreaks;
	unique_ptr<CFont> font, labelfont, titlefont;
	unique_ptr<CPen> dottypen, framepen, plotPen;	
	unique_ptr<CMemoryDC> m_pDC;
	
	TEXTMETRIC textmetric;	

	BOOL LoadData();
	int DeviceCoordToRow(int coord);
	int BlockWidth();
	int BlockHeight();
	int GetVerticalLabelInterval();
	int GetHorizontalLabelInterval(CDC* pDC);
	CPoint GetStartAndEndBlocks();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CDrewPlotView();
protected:	
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
	virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDCView);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//! Paints Seq data to the Seq data area
	virtual void PaintSeqData(CDC* pDC);
	//! Updates the status of the control
	void UpdateCtrlStatus();

// Implementation
public:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditCopy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};

#ifndef _DEBUG  // debug version in SeqView.cpp
inline CDrewPlotDoc* CDrewPlotView::GetDocument() const
{
	return reinterpret_cast<CDrewPlotDoc*>(m_pDocument);
}
#endif


// DNAssistView.h : interface of the CDNAssistView class
//

#pragma once
#include "MemDC.h"
#include "ResultDoc.h"

#include <memory>

#include "model/PrintItemRowList.h"

class CDNAssistCntrItem;

using namespace std;

class CResultView : public CScrollView
{
private:
	//! Address background color
	COLORREF m_colorAddrBkgnd;
	COLORREF m_colorSeqDataBkgnd, m_colorSeqDataText;

protected: // create from serialization only	
	DECLARE_DYNCREATE(CResultView)

private:
	CRect GetClientPosition();
	int GetLinesPerPage();
	vector<int> GetPageBreaks();
	int GetTotalNumberOfPages();

protected:
	int pagewidth;
	int pageheight;
	int leftmargin;
	int leftoffset;
	int rightmargin;
	int topmargin;
	int bottommargin;
	int height;
	int width;
	int mapmode;
	int horpixels;
	int vertpixels;
	int pageprintwidth;
	int pageprintheight;
	int maxcolumns;
	int numberofcolumns;
	int numberofrows;
	int cumulatedwheelrotation;
	int characters_per_row;
	int lines_per_page;
	bool selection;
	vector<string> tablerows;	
	unique_ptr<CFont> font;
	unique_ptr<LOGFONT> logfont;
	TEXTMETRIC textmetric;	
	vector<CPoint> outertick;
	vector<CPoint> innertick;
	vector<int> graphiclabelposition;
	vector<string> graphiclabeltag;
	vector<CPoint> labeltickstart;
	vector<CPoint> labeltickend;
	vector<CPoint> labelcoords;
	vector<int> selectedenzymes;
	vector<int> pagebreaks;
	unique_ptr<CMemoryDC> m_pDC;
	CAnalyzeDNAList* analyzednalist;
	CPoint fontdimension;
	CPoint chardim;	
	CPoint outerstart, outerend, innerstart, innerend, center;
	CRect outercircle, innercircle, sequencerectangle;
	CSize character_dimension; //width and height of a character	
	CPrintItemRowList arrayofprintrows;

	int GetBlockWidth();
	int GetCharactersPerRow();
	size_t GetLongestNameString();
	size_t GetLongestNumberCutsString();
	size_t GetLongestSiteString();
	size_t GetLongestCutPositionAndFragmentLengthString(); // column width of data columns in name, site, cuts, positions and length section
	size_t GetLongestNameSiteNumber(); //column width of 1st column in name, site, cuts, positions and length section
	int GetNoCutBlockWidth();	
	size_t GetLongestCutPositionString();
	size_t GetLongestFragmentLengthString();
	size_t GetSequenceLength();
	CPoint MovePointFromZero(CPoint value, CPoint offset);
	CPoint MovePointToZero(CPoint value, CPoint offset);
	CPoint RotatePoint(CPoint oldcoords, float angle, bool clockwise = true);
	int GetLargeTickValue(int range);
	void MakeGraphicLabelTag();	
	BOOL Touches(const CRect& rect1, const CRect& rect2);
	size_t FormatGraphicLabelTag();
	CRect GetTagRectangle(int index);
	void MoveTagUp(int tomoveindex, int staysameindex);
	void MoveTagDown(int tomoveindex, int staysameindex);
	void MoveTagLeft(int index);
	void MakeSiteUsageColumns();
	void MakeCutPositionColumns();
	void MakeNoCutterColumns();
	void MakeGraphicMap(int& nWidth, int nHeight);
	void MakeCutPositionLines();
	void SetFontDimensions(CDC* pDC);
	//! Updates the status of the control
	void UpdateCtrlStatus();
	CString GetGraphicNameTag(CString& data);
	CPoint GetGraphicNameTagCoordinates(CDC* pDC);
	CString GetGraphicSizeTag(CString& data);
	CPoint GetGraphicSizeTagCoordinates(CDC* pDC);

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
	CResultView();
	CResultDoc* GetDocument() const;
	string GetTextFormat();
	string GetRichTextFormat();
	void SaveAsEnhancedMetaFile(LPCTSTR lpszFilePath);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual ~CResultView();
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
inline CResultDoc* CResultView::GetDocument() const
{
	return reinterpret_cast<CResultDoc*>(m_pDocument);
}
#endif

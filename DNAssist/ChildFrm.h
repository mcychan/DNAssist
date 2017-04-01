
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Operations
public:	

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	wstring m_caption;
	void TranslateNucleicAcidToProtein(short phase);
	DECLARE_MESSAGE_MAP()	
	afx_msg void OnOptionsTranscriptionfactors();	
	afx_msg void OnOptionsEditor();
	afx_msg void OnOptionsAnalysis();
	afx_msg void OnOptionsResultdisplay();
	afx_msg void OnOptionsAlignment();
	afx_msg void OnPropertiesHydrophobicity();
	afx_msg void OnPropertiesHydrophilicity();
	afx_msg void OnPropertiesAntigenicity();
public:
	afx_msg void OnTranslate1phase();
	afx_msg void OnTranslate3phase();
	afx_msg void OnTranslate6phase();
	afx_msg void OnAnalyzeAlign();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnUpdateAnalyzeAlign(CCmdUI *pCmdUI);
};

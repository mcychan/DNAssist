#pragma once
// FontPage.h : header file
//
 
 
/////////////////////////////////////////////////////////////////////////////
// CFontPage dialog
 
class CFontPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFontPage)
 
// Construction
public:
	void GetCurrentFont(LPLOGFONT lplf);
	CFontPage(BOOL isPrint = TRUE, LOGFONT* plogfont = NULL);
	~CFontPage();
 
// Dialog Data
	//{{AFX_DATA(CFontPage)
	enum { IDD = IDD_FONTPROPERTYPAGE };
	CStatic	m_staticSample;
	CComboBox	m_comboStyle;
	CComboBox	m_comboSize;
	CComboBox	m_comboFont;
	BOOL	m_bStrikeOut;
	BOOL	m_bUnderline;
	CString	m_sFont;
	CString	m_sSize;
	CString	m_sStyle;
	//}}AFX_DATA
 
 
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFontPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
 
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFontPage)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
 
private:
	BOOL m_isPrint;
	int m_cyPixelsPerInch;
	CFont m_fontSample;
	static int CALLBACK FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, 
		int FontType, CFontPage* pFontPage );
 
 
};

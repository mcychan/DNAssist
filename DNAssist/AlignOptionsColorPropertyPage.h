#pragma once
#include "ColorButton.h"

// CAlignOptionsColorPropertyPage

class CAlignOptionsColorPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignOptionsColorPropertyPage)

public:
	CAlignOptionsColorPropertyPage();
	virtual ~CAlignOptionsColorPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALIGN_COLOR };
#endif

protected:
	COLORREF m_conservedforegroundColor, m_conservedbackgroundColor, m_identicalforegroundColor, m_identicalbackgroundColor;
	CColorButton m_conservedforegroundBtn, m_conservedbackgroundBtn, m_identicalforegroundBtn, m_identicalbackgroundBtn;
	CString m_percentageconserved, m_percentageidentical;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAlignDefaultColor();
};

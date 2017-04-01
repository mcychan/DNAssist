#pragma once


// CAlignOptionsGeneralPropertyPage

class CAlignOptionsGeneralPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignOptionsGeneralPropertyPage)

public:
	CAlignOptionsGeneralPropertyPage();
	virtual ~CAlignOptionsGeneralPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALIGN_GENERAL };
#endif

protected:
	CString m_delaydivergentsequences, m_gapopeningpenalty, m_gapextensionpenalty;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedAlignDefaults();
};

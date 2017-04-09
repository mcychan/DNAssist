#pragma once


// CAlignOptionsDNAPropertyPage

class CAlignOptionsDNAPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignOptionsDNAPropertyPage)

public:
	CAlignOptionsDNAPropertyPage();
	virtual ~CAlignOptionsDNAPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALIGN_DNA };
#endif

protected:
	int m_dnaweightmatrix;
	CString m_dnatransitionweight;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedAlignDefaultDna();
};

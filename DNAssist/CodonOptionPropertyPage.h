#pragma once
#include "afxwin.h"


// CCodonOptionPropertyPage

class CCodonOptionPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCodonOptionPropertyPage)

public:
	CCodonOptionPropertyPage();
	virtual ~CCodonOptionPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CODON };
#endif

private:
	CString m_codonDir;
	CString m_codonFile;
	CComboBox m_cmbCodon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCodonpathbrowse();
	afx_msg void OnEnChangeCodonpath();

protected:
	CEdit m_txtCodonDir;
};

#pragma once


// CAlignOptionsProteinPropertyPage

class CAlignOptionsProteinPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAlignOptionsProteinPropertyPage)

public:
	CAlignOptionsProteinPropertyPage();
	virtual ~CAlignOptionsProteinPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALIGN_PROTEIN };
#endif

protected:
	BOOL m_residuespecificpenalty, m_hydrophilicpenalty, m_endgapseparation;
	int m_proteinweightmatrix;
	CString m_hydrophilicresidues, m_gapseparationdistance;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAlignDefaultsProtein();
};

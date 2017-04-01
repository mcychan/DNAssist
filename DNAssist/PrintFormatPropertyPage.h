#pragma once


// CPrintFormatPropertyPage 

class CPrintFormatPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPrintFormatPropertyPage)

public:
	CPrintFormatPropertyPage();
	virtual ~CPrintFormatPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTFORMAT };
#endif

protected:
	BOOL m_strand, m_display, m_position;
	CString m_blocksize, m_blockgapsize;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void EnableCombos();
	afx_msg void OnBnClickedDisplay();
};

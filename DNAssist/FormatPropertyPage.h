#pragma once


// CFormatPropertyPage 

class CFormatPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFormatPropertyPage)

public:
	CFormatPropertyPage();
	virtual ~CFormatPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWAS };
#endif

protected:
	BOOL m_all, m_sequence, m_graphic, m_line, m_tables;
	int GetNumberOfCheckedCheckBoxes();
	UINT GetSelectedCheckBox();
	afx_msg void EnableAll();
	afx_msg void DisableIfOnlyOneSelected();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void CFormatPropertyPage::OnBnClickedAll();
	afx_msg void CFormatPropertyPage::OnBnClickedSeq();
	afx_msg void CFormatPropertyPage::OnBnClickedLine();
	afx_msg void CFormatPropertyPage::OnBnClickedGraphic();
	afx_msg void CFormatPropertyPage::OnBnClickedTable();
};

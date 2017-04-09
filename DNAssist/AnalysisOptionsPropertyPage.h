#pragma once


// CAnalysisOptionsPropertyPage

class CAnalysisOptionsPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAnalysisOptionsPropertyPage)

public:
	CAnalysisOptionsPropertyPage();
	virtual ~CAnalysisOptionsPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALYSISOPTIONS };
#endif

protected:
	BOOL m_selection, m_wholeset, m_fiveprime, m_threeprime, m_blunt, m_fourcutter, m_sixcutter, m_eightcutter, m_palindrome, m_outside, m_graphic;
	int GetNumberOfCheckedCheckBoxes();
	UINT GetSelectedCheckBox();
	afx_msg void EnableAll();
	afx_msg void DisableIfOnlyOneSelected();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedWholeSet();
	afx_msg void OnBnClickedFivePrime();
	afx_msg void OnBnClickedThreePrime();
	afx_msg void OnBnClickedBlunt();
	afx_msg void OnBnClickedFourCutter();
	afx_msg void OnBnClickedSixCutter();
	afx_msg void OnBnClickedEightCutter();
	afx_msg void OnBnClickedPalindrome();
	afx_msg void OnBnClickedOutside();
};

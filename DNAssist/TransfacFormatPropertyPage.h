#pragma once


// TransfacFormatPropertyPage

class CTransfacFormatPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTransfacFormatPropertyPage)

public:
	CTransfacFormatPropertyPage();
	virtual ~CTransfacFormatPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFAC_DISPLAY };
#endif

protected:
	BOOL m_showall, m_list, m_map, m_table;
	int GetNumberOfCheckedCheckBoxes();
	UINT GetSelectedCheckBox();
	afx_msg void EnableAll();
	afx_msg void DisableIfOnlyOneSelected();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedAllformats();
	afx_msg void OnBnClickedSeqList();
	afx_msg void OnBnClickedGraphicMap();
	afx_msg void OnBnClickedTable();
};

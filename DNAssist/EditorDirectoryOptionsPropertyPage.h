#pragma once


// CEditorDirectoryOptionsPropertyPage 對話方塊

class CEditorDirectoryOptionsPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEditorDirectoryOptionsPropertyPage)

public:
	CEditorDirectoryOptionsPropertyPage();
	virtual ~CEditorDirectoryOptionsPropertyPage();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITOR_DIRECTORIES };
#endif

protected:
	CString m_sequenceDirectory;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedEditorSequencesBrowse();
	virtual void OnOK();
};

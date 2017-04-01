#pragma once


// CEditorLinelengthOptionsPropertyPage 對話方塊

class CEditorLinelengthOptionsPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEditorLinelengthOptionsPropertyPage)

public:
	CEditorLinelengthOptionsPropertyPage();
	virtual ~CEditorLinelengthOptionsPropertyPage();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITOR_LINELENGTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
};

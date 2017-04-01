#pragma once


// CProfilesPropertyPage 對話方塊

class CProfilesPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CProfilesPropertyPage)

public:
	CProfilesPropertyPage();
	virtual ~CProfilesPropertyPage();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
	CString m_hpb;
	CString m_hpl;
	CString m_a;
public:
	virtual void OnOK();
};

#pragma once


// CTransFacGMPropertyPage

class CTransFacOptionsPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTransFacOptionsPropertyPage)

public:
	CTransFacOptionsPropertyPage();
	virtual ~CTransFacOptionsPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFAC_OPTIONS };
#endif

protected:
	BOOL m_transFacOptions;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
};

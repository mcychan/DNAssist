#pragma once


// CConversionOutputPropertyPage

class CConversionOutputPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CConversionOutputPropertyPage)

public:
	CConversionOutputPropertyPage();
	virtual ~CConversionOutputPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT };
#endif

protected:
	BOOL m_replace;
	CString m_blocksize, m_blockgapsize;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

#pragma once


// CAddEnzymeDialog

class CAddEnzymeDialog : public CDialog
{
	DECLARE_DYNAMIC(CAddEnzymeDialog)

public:
	CAddEnzymeDialog(AddEnzymeTransferBuffer* pAddEnzymeTransferBuffer, CWnd* pParent = NULL);   
	virtual ~CAddEnzymeDialog();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDENZYME };
#endif

protected:
	CString m_maxname, m_maxsite, m_maxupper, m_maxlower;
	AddEnzymeTransferBuffer* m_pEnzymetransferbuffer;
	afx_msg BOOL OkIsValid();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

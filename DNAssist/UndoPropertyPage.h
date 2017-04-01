#pragma once


// UndoPropertyPage

class CUndoPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUndoPropertyPage)

public:
	CUndoPropertyPage();
	virtual ~CUndoPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNDO };
#endif

protected:
	CString m_undolevels;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

#pragma once
#include <string>

using namespace std;

// CFindDialog ��ܤ��

class CFindDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindDialog)

public:
	CFindDialog(CWnd* pParent = NULL);   // �зǫغc�禡
	void MakeTranspanency(double transparencyPercentage = 80.0);
	afx_msg CPoint Search(const string& seq, char sequence_type, char seqForm, int match);
	inline BOOL IsGlobal() {
		return !m_bScope;
	}
	inline CStringA GetFindString() {
		return CStringA(m_strFind);
	}
	virtual ~CFindDialog();

	enum { IDD = IDD_FINDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	LRESULT NotifyView(UINT id);

	DECLARE_MESSAGE_MAP()
	BOOL m_bScope;
	CString m_strFind;

	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedFindAgain();
};

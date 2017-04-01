#pragma once
#include "afxwin.h"


// CSequenceTypeDialog 對話方塊

class CSequenceTypeDialog : public CDialog
{
	DECLARE_DYNAMIC(CSequenceTypeDialog)

public:
	CSequenceTypeDialog(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CSequenceTypeDialog();
	inline char GetSeqType()
	{
		switch (m_currSel) {
			case 1: return 'G';
			case 2: return 'D';
			case 3: return 'R';
			case 4: return 'P';
			default: return 0;
		}
	}

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEQUENCETYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	int m_currSel;
};

// SequenceTypeDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "SequenceTypeDialog.h"



// CSequenceTypeDialog 對話方塊

IMPLEMENT_DYNAMIC(CSequenceTypeDialog, CDialog)

CSequenceTypeDialog::CSequenceTypeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SEQUENCETYPE, pParent)
	, m_currSel(1)
{
}

CSequenceTypeDialog::~CSequenceTypeDialog()
{
}

void CSequenceTypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_SEQUENCETYPE, m_currSel);
}

BOOL CSequenceTypeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();
	return TRUE;
}


BEGIN_MESSAGE_MAP(CSequenceTypeDialog, CDialog)
END_MESSAGE_MAP()


// CSequenceTypeDialog 訊息處理常式

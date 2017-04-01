// ProgressDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "ProgressDialog.h"



// CProgressDialog 對話方塊

IMPLEMENT_DYNAMIC(CProgressDialog, CDialog)

CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROGRESSDIALOG, pParent)
{

}

CProgressDialog::~CProgressDialog()
{
}

void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
END_MESSAGE_MAP()


// CProgressDialog 訊息處理常式

// SequenceListPropertyPage.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "SequenceListPropertyPage.h"



// CSequenceListPropertyPage 對話方塊

IMPLEMENT_DYNAMIC(CSequenceListPropertyPage, CPropertyPage)

CSequenceListPropertyPage::CSequenceListPropertyPage()
	: CPropertyPage(IDD_ANALYSISOPTIONS)
{

}

CSequenceListPropertyPage::~CSequenceListPropertyPage()
{
}

void CSequenceListPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSequenceListPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CSequenceListPropertyPage 訊息處理常式

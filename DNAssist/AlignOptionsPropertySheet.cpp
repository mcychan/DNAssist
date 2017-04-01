// AlignOptionsPropertySheet.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignOptionsPropertySheet.h"


// CAlignOptionsPropertySheet

IMPLEMENT_DYNAMIC(CAlignOptionsPropertySheet, CPropertySheet)

CAlignOptionsPropertySheet::CAlignOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CAlignOptionsPropertySheet::CAlignOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&alignOptionsGeneralPropertyPage);
	AddPage(&alignOptionsDNAPropertyPage);
	AddPage(&alignOptionsProteinPropertyPage);
	AddPage(&alignOptionsColorPropertyPage);
}

CAlignOptionsPropertySheet::~CAlignOptionsPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CAlignOptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CAlignOptionsPropertySheet 訊息處理常式

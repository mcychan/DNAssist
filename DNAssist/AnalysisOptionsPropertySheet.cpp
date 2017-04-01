// AnalysisOptionsPropertySheet.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AnalysisOptionsPropertySheet.h"


// CAnalysisOptionsPropertySheet

IMPLEMENT_DYNAMIC(CAnalysisOptionsPropertySheet, CPropertySheet)

CAnalysisOptionsPropertySheet::CAnalysisOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CAnalysisOptionsPropertySheet::CAnalysisOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&codonOptionPropertyPage);
	AddPage(&conversionOutputPropertyPage);
	AddPage(&profilesPropertyPage);
}

CAnalysisOptionsPropertySheet::~CAnalysisOptionsPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CAnalysisOptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CAnalysisOptionsPropertySheet 訊息處理常式

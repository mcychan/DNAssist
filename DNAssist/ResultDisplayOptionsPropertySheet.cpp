// ResultDisplayOptionsPropertySheet.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "ResultDisplayOptionsPropertySheet.h"


// CResultDisplayOptionsPropertySheet

IMPLEMENT_DYNAMIC(CResultDisplayOptionsPropertySheet, CPropertySheet)

CResultDisplayOptionsPropertySheet::CResultDisplayOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CResultDisplayOptionsPropertySheet::CResultDisplayOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&printFormatPropertyPage);
	AddPage(&fontPropertyPage);
}

CResultDisplayOptionsPropertySheet::~CResultDisplayOptionsPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CResultDisplayOptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()

// CResultDisplayOptionsPropertySheet

// RestrictEnzymesPropertySheet.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "RestrictEnzymesPropertySheet.h"


// CRestrictEnzymesPropertySheet

IMPLEMENT_DYNAMIC(CRestrictEnzymesPropertySheet, CPropertySheet)

CRestrictEnzymesPropertySheet::CRestrictEnzymesPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CRestrictEnzymesPropertySheet::CRestrictEnzymesPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&selectEnzymesPropertyPage);
	AddPage(&transfacDisplayFormatPropertyPage);
	AddPage(&analysisOptionsPropertyPage);
}

CRestrictEnzymesPropertySheet::~CRestrictEnzymesPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CRestrictEnzymesPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CRestrictEnzymesPropertySheet

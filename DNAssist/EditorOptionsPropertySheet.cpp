// EditorOptionsPropertySheet.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "EditorOptionsPropertySheet.h"


// CEditorOptionsPropertySheet

IMPLEMENT_DYNAMIC(CEditorOptionsPropertySheet, CPropertySheet)

CEditorOptionsPropertySheet::CEditorOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CEditorOptionsPropertySheet::CEditorOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	fontPropertyPage = make_unique<CFontPage>(FALSE);
	AddPage(fontPropertyPage.get());
	AddPage(&editorLinelengthOptionsPropertyPage);
	AddPage(&editorDirectoryOptionsPropertyPage);
	AddPage(&undoPropertyPage);
}

CEditorOptionsPropertySheet::~CEditorOptionsPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CEditorOptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()

// CEditorOptionsPropertySheet

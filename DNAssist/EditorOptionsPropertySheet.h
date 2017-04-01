#pragma once
#include "EditorDirectoryOptionsPropertyPage.h"
#include "EditorLinelengthOptionsPropertyPage.h"
#include "FontPage.h"
#include "UndoPropertyPage.h"

#include <memory>

// CEditorOptionsPropertySheet

class CEditorOptionsPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CEditorOptionsPropertySheet)

private:
	CEditorLinelengthOptionsPropertyPage editorLinelengthOptionsPropertyPage;
	CEditorDirectoryOptionsPropertyPage editorDirectoryOptionsPropertyPage;
	unique_ptr<CFontPage> fontPropertyPage;
	CUndoPropertyPage undoPropertyPage;

public:
	CEditorOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEditorOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CEditorOptionsPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
};



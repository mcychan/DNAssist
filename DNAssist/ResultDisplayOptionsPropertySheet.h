#pragma once
#include "FontPage.h"
#include "PrintFormatPropertyPage.h"

// CResultDisplayOptionsPropertySheet

class CResultDisplayOptionsPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CResultDisplayOptionsPropertySheet)

private:
	CFontPage fontPropertyPage;
	CPrintFormatPropertyPage printFormatPropertyPage;

public:
	CResultDisplayOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CResultDisplayOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CResultDisplayOptionsPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
};



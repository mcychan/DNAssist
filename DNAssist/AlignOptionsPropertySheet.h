#pragma once
#include "AlignOptionsGeneralPropertyPage.h"
#include "AlignOptionsDNAPropertyPage.h"
#include "AlignOptionsProteinPropertyPage.h"
#include "AlignOptionsColorPropertyPage.h"


// CAlignOptionsPropertySheet

class CAlignOptionsPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAlignOptionsPropertySheet)

private:
	CAlignOptionsGeneralPropertyPage alignOptionsGeneralPropertyPage;
	CAlignOptionsDNAPropertyPage alignOptionsDNAPropertyPage;
	CAlignOptionsProteinPropertyPage alignOptionsProteinPropertyPage;
	CAlignOptionsColorPropertyPage alignOptionsColorPropertyPage;

public:
	CAlignOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAlignOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAlignOptionsPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
};



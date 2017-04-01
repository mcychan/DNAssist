#pragma once
#include "CodonOptionPropertyPage.h"
#include "ConversionOutputPropertyPage.h"
#include "ProfilesPropertyPage.h"


// CAnalysisOptionsPropertySheet

class CAnalysisOptionsPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAnalysisOptionsPropertySheet)

private:
	CCodonOptionPropertyPage codonOptionPropertyPage;
	CConversionOutputPropertyPage conversionOutputPropertyPage;
	CProfilesPropertyPage profilesPropertyPage;

public:
	CAnalysisOptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAnalysisOptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CAnalysisOptionsPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
};



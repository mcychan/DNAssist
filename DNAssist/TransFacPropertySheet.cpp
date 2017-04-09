// TransFacPropertySheet.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "TransFacPropertySheet.h"

#include "model/EnzymeList.h"

// CTransFacPropertySheet

IMPLEMENT_DYNAMIC(CTransFacPropertySheet, CPropertySheet)

CTransFacPropertySheet::CTransFacPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CTransFacPropertySheet::CTransFacPropertySheet(LPCTSTR pszCaption, CEnzymeList* listoffactors, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	try {
		db.open(CT2A((LPCTSTR) theApp.transfacPath));
	}
	catch (CppSQLite3Exception& e)
	{
		CString errMsg;
		errMsg.Format(_T("Cannot open database : %s"), theApp.transfacPath);
		AfxMessageBox(errMsg);
	}

	kingdomFacPropertyPage = make_unique<CTransFacPropertyPage>("Kingdom");
	AddPage(kingdomFacPropertyPage.get());
	speciesFacPropertyPage = make_unique<CTransFacPropertyPage>("Species");
	AddPage(speciesFacPropertyPage.get());
	factorSequencePropertyPage = make_unique<CFactorSequencePropertyPage>(listoffactors);
	AddPage(factorSequencePropertyPage.get());
	AddPage(&transfacDisplayFormatPropertyPage);
	AddPage(&transFacGMPropertyPage);
}

void CTransFacPropertySheet::OnLbSelChange(const string& field)
{
	factorSequencePropertyPage->selectionchanged = TRUE;
}

BOOL CTransFacPropertySheet::FindSelections(const string& field, vector<string>& selections)
{
	if(field == "Species") {
		BOOL selectionchanged = kingdomFacPropertyPage->selectionchanged;
		if(selectionchanged)
			kingdomFacPropertyPage->GetListBoxSelectedStrings(selections);
		return selectionchanged;
	}
	if(field == "Factor") {
		BOOL selectionchanged = speciesFacPropertyPage->selectionchanged;
		if(selectionchanged)
			speciesFacPropertyPage->GetListBoxSelectedStrings(selections);
		return selectionchanged;
	}
	return FALSE;
}

void CTransFacPropertySheet::QueryCompleted(const string& field)
{
	if(field == "Species")
		kingdomFacPropertyPage->selectionchanged = FALSE;
	else if(field == "Factor")
		speciesFacPropertyPage->selectionchanged = FALSE;
}

CTransFacPropertySheet::~CTransFacPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CTransFacPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// CTransFacPropertySheet

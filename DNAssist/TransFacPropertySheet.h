#pragma once
#include "FactorSequencePropertyPage.h"
#include "TransfacFormatPropertyPage.h"
#include "TransFacOptionsPropertyPage.h"
#include "TransFacPropertyPage.h"
#include "CppSQLite3.h"

#include <memory>
#include <string>

using namespace std;

// CTransFacPropertySheet

class CTransFacPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTransFacPropertySheet)

public:
	CTransFacPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTransFacPropertySheet(LPCTSTR pszCaption, CEnzymeList* listoffactors, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	BOOL FindSelections(const string& field, vector<string>& selections);
	void OnLbSelChange(const string& field);
	void QueryCompleted(const string& field);
	virtual ~CTransFacPropertySheet();
	inline CppSQLite3DB* GetDatabase() {
		return &db;
	}

protected:
	CppSQLite3DB db;
	unique_ptr<CTransFacPropertyPage> kingdomFacPropertyPage;
	unique_ptr<CTransFacPropertyPage> speciesFacPropertyPage;
	unique_ptr<CFactorSequencePropertyPage> factorSequencePropertyPage;
	CTransfacFormatPropertyPage transfacDisplayFormatPropertyPage;
	CTransFacOptionsPropertyPage transFacGMPropertyPage;
	DECLARE_MESSAGE_MAP()
};



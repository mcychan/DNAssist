#pragma once
#include "SelectEnzymesPropertyPage.h"
#include "TransfacFormatPropertyPage.h"
#include "AnalysisOptionsPropertyPage.h"

#include <memory>

using namespace std;

// CRestrictEnzymesPropertySheet

class CRestrictEnzymesPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CRestrictEnzymesPropertySheet)

private:
	CSelectEnzymesPropertyPage selectEnzymesPropertyPage;
	CTransfacFormatPropertyPage transfacDisplayFormatPropertyPage;
	CAnalysisOptionsPropertyPage analysisOptionsPropertyPage;
	unique_ptr<CEnzymeList> m_pEnzymelist;

public:
	CRestrictEnzymesPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CRestrictEnzymesPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CRestrictEnzymesPropertySheet();
	inline CEnzymeList* GetEnzymeList() {
		return m_pEnzymelist.get();
	}
	inline void SetEnzymeList(CEnzymeList* pEnzymelist) {
		m_pEnzymelist = unique_ptr<CEnzymeList>(pEnzymelist);
	}

protected:
	DECLARE_MESSAGE_MAP()
};



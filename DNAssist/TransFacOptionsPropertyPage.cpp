// TransFacOptionsPropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "TransFacOptionsPropertyPage.h"


// CTransFacGMPropertyPage

IMPLEMENT_DYNAMIC(CTransFacOptionsPropertyPage, CPropertyPage)

CTransFacOptionsPropertyPage::CTransFacOptionsPropertyPage()
	: CPropertyPage(IDD_TRANSFAC_OPTIONS)
{
	m_transFacOptions = theApp.TransFacOptions;
}

CTransFacOptionsPropertyPage::~CTransFacOptionsPropertyPage()
{
}

void CTransFacOptionsPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TRANSFAC_UNIQUE, m_transFacOptions);
}


BEGIN_MESSAGE_MAP(CTransFacOptionsPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CTransFacGMPropertyPage
void CTransFacOptionsPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	theApp.TransFacOptions = m_transFacOptions;
}

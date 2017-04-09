// ProfilesPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "ProfilesPropertyPage.h"

// CProfilesPropertyPage

IMPLEMENT_DYNAMIC(CProfilesPropertyPage, CPropertyPage)

CProfilesPropertyPage::CProfilesPropertyPage()
	: CPropertyPage(IDD_PROPERTIES)
{
	m_hpb.Format(_T("%d"), theApp.hydrophobicityOptions);
	m_hpl.Format(_T("%d"), theApp.hydrophilicityOptions);
	m_a.Format(_T("%d"), theApp.antigenicityOptions);
}

CProfilesPropertyPage::~CProfilesPropertyPage()
{
}

void CProfilesPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_HYDROPHOBICITY, m_hpb);
	DDX_CBString(pDX, IDC_HYDROPHILICITY, m_hpl);
	DDX_CBString(pDX, IDC_ANTIGENICITY, m_a);
}


BEGIN_MESSAGE_MAP(CProfilesPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CProfilesPropertyPage

void CProfilesPropertyPage::OnOK()
{
	CPropertyPage::OnOK();
	theApp.hydrophobicityOptions = _ttoi(m_hpb);
	theApp.hydrophilicityOptions = _ttoi(m_hpl);
	theApp.antigenicityOptions = _ttoi(m_a);
}

// AlignOptionsGeneralPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignOptionsGeneralPropertyPage.h"



// CAlignOptionsGeneralPropertyPage

IMPLEMENT_DYNAMIC(CAlignOptionsGeneralPropertyPage, CPropertyPage)

CAlignOptionsGeneralPropertyPage::CAlignOptionsGeneralPropertyPage()
	: CPropertyPage(IDD_ALIGN_GENERAL)
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	m_delaydivergentsequences.Format(_T("%d"), alignOptionsBuffer->delaydivergentsequences);
	m_gapopeningpenalty.Format(_T("%d"), alignOptionsBuffer->gapopeningpenalty);
	m_gapextensionpenalty.Format(_T("%.2f"), alignOptionsBuffer->gapextensionpenalty);
}

CAlignOptionsGeneralPropertyPage::~CAlignOptionsGeneralPropertyPage()
{
}

void CAlignOptionsGeneralPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_DELAY_DIVERGENT_SEQUENCES, m_delaydivergentsequences);
	DDX_CBString(pDX, IDC_GAP_OPENING_PENALTY, m_gapopeningpenalty);
	DDX_Text(pDX, IDC_GAP_EXTENSION_PENALTY, m_gapextensionpenalty);
}

void CAlignOptionsGeneralPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	alignOptionsBuffer->delaydivergentsequences = _ttoi(m_delaydivergentsequences);
	alignOptionsBuffer->gapopeningpenalty = _ttoi(m_gapopeningpenalty);
	alignOptionsBuffer->gapextensionpenalty = _ttof(m_gapextensionpenalty);
}

BEGIN_MESSAGE_MAP(CAlignOptionsGeneralPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ALIGN_DEFAULTS, OnBnClickedAlignDefaults)
END_MESSAGE_MAP()


// CAlignOptionsGeneralPropertyPage
void CAlignOptionsGeneralPropertyPage::OnBnClickedAlignDefaults()
{
	UpdateData(FALSE);
}

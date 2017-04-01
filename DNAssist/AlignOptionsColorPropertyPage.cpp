// AlignOptionsColorPropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignOptionsColorPropertyPage.h"



// CAlignOptionsColorPropertyPage 

IMPLEMENT_DYNAMIC(CAlignOptionsColorPropertyPage, CPropertyPage)

CAlignOptionsColorPropertyPage::CAlignOptionsColorPropertyPage()
	: CPropertyPage(IDD_ALIGN_COLOR)
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	m_conservedforegroundColor = alignOptionsBuffer->conservedforegroundColor;
	m_conservedbackgroundColor = alignOptionsBuffer->conservedbackgroundColor;
	m_identicalforegroundColor = alignOptionsBuffer->identicalforegroundColor;
	m_identicalbackgroundColor = alignOptionsBuffer->identicalbackgroundColor;
	m_percentageconserved.Format(_T("%d"), alignOptionsBuffer->percentageconserved);
	m_percentageidentical.Format(_T("%d"), alignOptionsBuffer->percentageidentical);
}

CAlignOptionsColorPropertyPage::~CAlignOptionsColorPropertyPage()
{
}

void CAlignOptionsColorPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONSERVED_FOREGROUND, m_conservedforegroundBtn);
	DDX_Control(pDX, IDC_CONSERVED_BACKGROUND, m_conservedbackgroundBtn);
	DDX_Control(pDX, IDC_IDENTICAL_FOREGROUND, m_identicalforegroundBtn);
	DDX_Control(pDX, IDC_IDENTICAL_BACKGROUND, m_identicalbackgroundBtn);
	DDX_ColorButton(pDX, IDC_CONSERVED_FOREGROUND, m_conservedforegroundColor);
	DDX_ColorButton(pDX, IDC_CONSERVED_BACKGROUND, m_conservedbackgroundColor);
	DDX_ColorButton(pDX, IDC_IDENTICAL_FOREGROUND, m_identicalforegroundColor);
	DDX_ColorButton(pDX, IDC_IDENTICAL_BACKGROUND, m_identicalbackgroundColor);
	DDX_CBString(pDX, IDC_CONSERVED_PERCENTAGE, m_percentageconserved);
	DDX_CBString(pDX, IDC_IDENTICAL_PERCENTAGE, m_percentageidentical);
}


BEGIN_MESSAGE_MAP(CAlignOptionsColorPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ALIGN_DEFAULT_COLOR, OnBnClickedAlignDefaultColor)
END_MESSAGE_MAP()

void CAlignOptionsColorPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	alignOptionsBuffer->conservedforegroundColor = m_conservedforegroundColor;
	alignOptionsBuffer->conservedbackgroundColor = m_conservedbackgroundColor;
	alignOptionsBuffer->identicalforegroundColor = m_identicalforegroundColor;
	alignOptionsBuffer->identicalbackgroundColor = m_identicalbackgroundColor;
	alignOptionsBuffer->percentageconserved = _ttoi(m_percentageconserved);
	alignOptionsBuffer->percentageidentical = _ttoi(m_percentageidentical);
}

// CAlignOptionsColorPropertyPage
void CAlignOptionsColorPropertyPage::OnBnClickedAlignDefaultColor()
{
	UpdateData(FALSE);
}

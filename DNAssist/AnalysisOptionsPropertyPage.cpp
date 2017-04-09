// AnalysisOptionsPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AnalysisOptionsPropertyPage.h"


// CAnalysisOptionsPropertyPage

IMPLEMENT_DYNAMIC(CAnalysisOptionsPropertyPage, CPropertyPage)

CAnalysisOptionsPropertyPage::CAnalysisOptionsPropertyPage()
	: CPropertyPage(IDD_ANALYSISOPTIONS)
{
	auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer();
	if(enzymeOptionsTransferBuffer->all)
		m_selection = 0;
	else if(enzymeOptionsTransferBuffer->selection)
		m_selection = 1;
	else
		m_selection = -1;

	m_wholeset = enzymeOptionsTransferBuffer->wholeset;
	m_fiveprime = enzymeOptionsTransferBuffer->fiveprime;
	m_threeprime = enzymeOptionsTransferBuffer->threeprime;
	m_blunt = enzymeOptionsTransferBuffer->blunt;
	m_fourcutter = enzymeOptionsTransferBuffer->fourcutter;
	m_sixcutter = enzymeOptionsTransferBuffer->sixcutter;
	m_eightcutter = enzymeOptionsTransferBuffer->eightcutter;
	m_palindrome = enzymeOptionsTransferBuffer->palindrome;
	m_outside = enzymeOptionsTransferBuffer->outside;

	if (enzymeOptionsTransferBuffer->graphicunique)
		m_graphic = 0;
	else if (enzymeOptionsTransferBuffer->graphicuniqueandsel)
		m_graphic = 1;
	else if (enzymeOptionsTransferBuffer->graphicselection)
		m_graphic = 2;
	else
		m_graphic = -1;
}

CAnalysisOptionsPropertyPage::~CAnalysisOptionsPropertyPage()
{
}

void CAnalysisOptionsPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SLALL, m_selection);
	DDX_Check(pDX, IDC_SLWHOLESET, m_wholeset);
	DDX_Check(pDX, IDC_SLFIVEPRIME, m_fiveprime);
	DDX_Check(pDX, IDC_SLTHREEPRIME, m_threeprime);
	DDX_Check(pDX, IDC_SLBLUNT, m_blunt);
	DDX_Check(pDX, IDC_SLFOURCUTTER, m_fourcutter);
	DDX_Check(pDX, IDC_SLSIXCUTTER, m_sixcutter);
	DDX_Check(pDX, IDC_SLEIGHTCUTTER, m_eightcutter);
	DDX_Check(pDX, IDC_SLPALINDROME, m_palindrome);
	DDX_Check(pDX, IDC_SLOUTSIDE, m_outside);
	DDX_Radio(pDX, IDC_GPUNIQUE, m_graphic);
}

int CAnalysisOptionsPropertyPage::GetNumberOfCheckedCheckBoxes()
{
	int number = 0;
	if(m_fiveprime)
		number++;
	if(m_threeprime)
		number++;
	if(m_blunt)
		number++;
	if(m_fourcutter)
		number++;
	if(m_sixcutter)
		number++;
	if(m_eightcutter)
		number++;
	if(m_palindrome)
		number++;
	if(m_outside)
		number++;
	return number;
}

UINT CAnalysisOptionsPropertyPage::GetSelectedCheckBox()
{
	if(m_fiveprime)
		return IDC_SLFIVEPRIME;
	if(m_threeprime)
		return IDC_SLTHREEPRIME;
	if(m_blunt)
		return IDC_SLBLUNT;
	if(m_fourcutter)
		return IDC_SLFOURCUTTER;
	if(m_sixcutter)
		return IDC_SLSIXCUTTER;
	if(m_eightcutter)
		return IDC_SLEIGHTCUTTER;
	if(m_palindrome)
		return IDC_SLEIGHTCUTTER;
	if(m_outside)
		return IDC_SLOUTSIDE;
	return 0;
}

void CAnalysisOptionsPropertyPage::EnableAll()
{
	UINT nIDs[] = {IDC_SLWHOLESET, IDC_SLFIVEPRIME, IDC_SLTHREEPRIME, IDC_SLBLUNT,
			IDC_SLFOURCUTTER, IDC_SLSIXCUTTER, IDC_SLEIGHTCUTTER, IDC_SLPALINDROME, IDC_SLOUTSIDE
	};
	for(int i=0; i< sizeof(nIDs) / sizeof (UINT); ++i)
		GetDlgItem(nIDs[i])->EnableWindow(TRUE);
}

void CAnalysisOptionsPropertyPage::DisableIfOnlyOneSelected()
{
	if(GetNumberOfCheckedCheckBoxes() == 1) {
		UINT nID = GetSelectedCheckBox();
		if(nID > 0)
			GetDlgItem(nID)->EnableWindow(FALSE);
		return;
	}
	EnableAll();
}

BEGIN_MESSAGE_MAP(CAnalysisOptionsPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_SLWHOLESET, OnBnClickedWholeSet)
	ON_BN_CLICKED(IDC_SLFIVEPRIME, OnBnClickedFivePrime)
	ON_BN_CLICKED(IDC_SLTHREEPRIME, OnBnClickedThreePrime)
	ON_BN_CLICKED(IDC_SLBLUNT, OnBnClickedBlunt)
	ON_BN_CLICKED(IDC_SLFOURCUTTER, OnBnClickedFourCutter)
	ON_BN_CLICKED(IDC_SLSIXCUTTER, OnBnClickedSixCutter)
	ON_BN_CLICKED(IDC_SLEIGHTCUTTER, OnBnClickedEightCutter)
	ON_BN_CLICKED(IDC_SLPALINDROME, OnBnClickedPalindrome)
	ON_BN_CLICKED(IDC_SLOUTSIDE, OnBnClickedOutside)
END_MESSAGE_MAP()

BOOL CAnalysisOptionsPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	DisableIfOnlyOneSelected();

	return TRUE;
}

// CAnalysisOptionsPropertyPage
void CAnalysisOptionsPropertyPage::OnBnClickedWholeSet()
{
	m_wholeset = !m_wholeset;
	DisableIfOnlyOneSelected();
	if(m_wholeset) {
		m_fiveprime = m_threeprime = m_sixcutter = m_wholeset;
		m_eightcutter = m_palindrome = m_outside = m_wholeset;
	}
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedFivePrime()
{
	m_fiveprime = !m_fiveprime;
	DisableIfOnlyOneSelected();
	if(!m_fiveprime)
		m_wholeset = m_fiveprime;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedThreePrime()
{
	m_threeprime = !m_threeprime;
	DisableIfOnlyOneSelected();
	if(!m_threeprime)
		m_wholeset = m_threeprime;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedBlunt()
{
	m_blunt = !m_blunt;
	DisableIfOnlyOneSelected();
	if(!m_blunt)
		m_wholeset = m_blunt;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedFourCutter()
{
	m_fourcutter = !m_fourcutter;
	DisableIfOnlyOneSelected();
	if(!m_fourcutter)
		m_wholeset = m_fourcutter;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedSixCutter()
{
	m_sixcutter = !m_sixcutter;
	DisableIfOnlyOneSelected();
	if(!m_sixcutter)
		m_wholeset = m_sixcutter;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedEightCutter()
{
	m_eightcutter = !m_eightcutter;
	DisableIfOnlyOneSelected();
	if(!m_eightcutter)
		m_wholeset = m_eightcutter;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedPalindrome()
{
	m_palindrome = !m_palindrome;
	DisableIfOnlyOneSelected();
	if(!m_palindrome)
		m_wholeset = m_palindrome;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnBnClickedOutside()
{
	m_outside = !m_outside;
	DisableIfOnlyOneSelected();
	if(!m_outside)
		m_wholeset = m_outside;
	UpdateData(FALSE);
}

void CAnalysisOptionsPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer();
	enzymeOptionsTransferBuffer->all = (m_selection == 0);
	enzymeOptionsTransferBuffer->selection = (m_selection == 1);
	enzymeOptionsTransferBuffer->wholeset = m_wholeset;
	enzymeOptionsTransferBuffer->fiveprime = m_fiveprime;
	enzymeOptionsTransferBuffer->threeprime = m_threeprime;
	enzymeOptionsTransferBuffer->blunt = m_blunt;
	enzymeOptionsTransferBuffer->fourcutter = m_fourcutter;
	enzymeOptionsTransferBuffer->sixcutter = m_sixcutter;
	enzymeOptionsTransferBuffer->eightcutter = m_eightcutter;
	enzymeOptionsTransferBuffer->palindrome = m_palindrome;
	enzymeOptionsTransferBuffer->outside = m_outside;
	enzymeOptionsTransferBuffer->graphicunique = (m_graphic == 0);
	enzymeOptionsTransferBuffer->graphicuniqueandsel = (m_graphic == 1);
	enzymeOptionsTransferBuffer->graphicselection = (m_graphic == 2);
}

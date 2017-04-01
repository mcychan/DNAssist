// AlignOptionsDNAPropertyPage.cpp : ��@��
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignOptionsDNAPropertyPage.h"



// CAlignOptionsDNAPropertyPage ��ܤ��

IMPLEMENT_DYNAMIC(CAlignOptionsDNAPropertyPage, CPropertyPage)

CAlignOptionsDNAPropertyPage::CAlignOptionsDNAPropertyPage()
	: CPropertyPage(IDD_ALIGN_DNA)
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	m_dnaweightmatrix = alignOptionsBuffer->dnaweightmatrix;
	m_dnatransitionweight.Format(_T("%0.1f"), alignOptionsBuffer->dnatransitionweight);
}

CAlignOptionsDNAPropertyPage::~CAlignOptionsDNAPropertyPage()
{
}

void CAlignOptionsDNAPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_DNA_WEIGHT_MATRIX, m_dnaweightmatrix);
	DDX_CBString(pDX, IDC_DNA_TRANSITION_WEIGHT, m_dnatransitionweight);
}


BEGIN_MESSAGE_MAP(CAlignOptionsDNAPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ALIGN_DEFAULT_DNA, OnBnClickedAlignDefaultDna)
END_MESSAGE_MAP()

void CAlignOptionsDNAPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	alignOptionsBuffer->dnaweightmatrix = m_dnaweightmatrix;
	alignOptionsBuffer->dnatransitionweight = _ttof(m_dnatransitionweight);
}

// CAlignOptionsDNAPropertyPage
void CAlignOptionsDNAPropertyPage::OnBnClickedAlignDefaultDna()
{
	UpdateData(FALSE);
}

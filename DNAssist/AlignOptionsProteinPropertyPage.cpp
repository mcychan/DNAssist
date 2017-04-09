// AlignOptionsProteinPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignOptionsProteinPropertyPage.h"



// CAlignOptionsProteinPropertyPage

IMPLEMENT_DYNAMIC(CAlignOptionsProteinPropertyPage, CPropertyPage)

CAlignOptionsProteinPropertyPage::CAlignOptionsProteinPropertyPage()
	: CPropertyPage(IDD_ALIGN_PROTEIN)
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	m_proteinweightmatrix = alignOptionsBuffer->proteinweightmatrix;
	m_residuespecificpenalty = (alignOptionsBuffer->residuespecificpenalty == 1);
	m_hydrophilicpenalty = (alignOptionsBuffer->hydrophilicpenalty == 1);
	m_hydrophilicresidues = theApp.hydrophilicresidues;
	m_endgapseparation = (alignOptionsBuffer->endgapseparation == 1);
	m_gapseparationdistance.Format(_T("%d"), alignOptionsBuffer->gapseparationdistance);
}

CAlignOptionsProteinPropertyPage::~CAlignOptionsProteinPropertyPage()
{
}

void CAlignOptionsProteinPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_PROTEIN_WEIGHT_MATRIX, m_proteinweightmatrix);
	DDX_Check(pDX, IDC_RESIDUE_SPECIFIC_PENALTY, m_residuespecificpenalty);
	DDX_Check(pDX, IDC_HYDROPHYLIC_PENALTY, m_hydrophilicpenalty);
	DDX_Text(pDX, IDC_HYDROPHILIC_RESIDUES, m_hydrophilicresidues);
	DDX_Check(pDX, IDC_END_GAP_SEPARATION, m_endgapseparation);
	DDX_Text(pDX, IDC_GAP_SEPARATION_DISTANCE, m_gapseparationdistance);
}


BEGIN_MESSAGE_MAP(CAlignOptionsProteinPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_ALIGN_DEFAULTS_PROTEIN, OnBnClickedAlignDefaultsProtein)
END_MESSAGE_MAP()

void CAlignOptionsProteinPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	alignOptionsBuffer->proteinweightmatrix = m_proteinweightmatrix;
	alignOptionsBuffer->residuespecificpenalty = m_residuespecificpenalty ? 1 : 0;
	alignOptionsBuffer->hydrophilicpenalty = m_hydrophilicpenalty ? 1 : 0;
	theApp.hydrophilicresidues = (LPCSTR) CStringA(m_hydrophilicresidues);
	alignOptionsBuffer->endgapseparation = m_endgapseparation ? 1 : 0;
	alignOptionsBuffer->gapseparationdistance = _ttoi(m_gapseparationdistance);
}

// CAlignOptionsProteinPropertyPage
void CAlignOptionsProteinPropertyPage::OnBnClickedAlignDefaultsProtein()
{
	UpdateData(FALSE);
}

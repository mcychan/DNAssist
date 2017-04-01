// ProteinPropertiesDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "ProteinPropertiesDialog.h"



// CProteinPropertiesDialog 對話方塊

IMPLEMENT_DYNAMIC(CProteinPropertiesDialog, CDialog)

static UINT residue_static[] = { IDC_RES_G, IDC_RES_A, IDC_RES_V, IDC_RES_L, IDC_RES_I, IDC_RES_P, IDC_RES_F, IDC_RES_Y, IDC_RES_W, IDC_RES_S,
	IDC_RES_T, IDC_RES_D, IDC_RES_E, IDC_RES_N, IDC_RES_Q, IDC_RES_C, IDC_RES_M, IDC_RES_H, IDC_RES_K, IDC_RES_R };

static UINT molepercent_static[] = { IDC_MOLE_G, IDC_MOLE_A, IDC_MOLE_V, IDC_MOLE_L, IDC_MOLE_I, IDC_MOLE_P, IDC_MOLE_F, IDC_MOLE_Y, IDC_MOLE_W, IDC_MOLE_S,
	IDC_MOLE_T, IDC_MOLE_D, IDC_MOLE_E, IDC_MOLE_N, IDC_MOLE_Q, IDC_MOLE_C, IDC_MOLE_M, IDC_MOLE_H, IDC_MOLE_K, IDC_MOLE_R };

CProteinPropertiesDialog::CProteinPropertiesDialog(const string& sequence, CWnd* pParent /*=NULL*/)
	: proteinsequence(sequence), CDialog(IDD_PROTEINPROPERTIES, pParent)
{
	propertiesobject = make_unique<CProteinProperties>(proteinsequence);	
}

void CProteinPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROTEINLENGTH, m_Length);
	DDX_Text(pDX, IDC_MW, m_molecular_weight);
	DDX_Text(pDX, IDC_PI, m_pI);
	DDX_Text(pDX, IDC_A280, m_A280);
	DDX_Text(pDX, IDC_CHARGEATPH, m_ChargeAtpH);
	DDX_Text(pDX, IDC_E280, m_E280);
}

BOOL CProteinPropertiesDialog::OnInitDialog()
{
	BOOL bResult = CDialog::OnInitDialog();
	Calculate();
	UpdateData(FALSE);
	return bResult;
}

CProteinPropertiesDialog::~CProteinPropertiesDialog()
{	
}

void CProteinPropertiesDialog::Calculate()
{
	LoadNumberOfResiduesControls();
	LoadMolePercentageControls();
	LoadMolecularWeightControl();
	LoadpIControl();
	LoadChargeAtpHControl();
	LoadMolarAbsorptivityControl();
	LoadEOnePercentControl();

	size_t proteinlength = proteinsequence.length();
	if (proteinlength != 1)
		m_Length.Format(_T("%d%s"), proteinlength, _T(" amino acids"));
	else
		m_Length.Format(_T("%d%s"), proteinlength, _T(" amino acid"));
}

void CProteinPropertiesDialog::LoadNumberOfResiduesControls()
{
	propertiesobject->GetNumberOfAminoAcidResidues();
	int numberofcommonaminoacids = propertiesobject->GetNumberOfCommonAminoAcids();	
	for (int x = 0; x < numberofcommonaminoacids; x++) {
		string data;
		propertiesobject->GetNumberOfResiduesString(x, data);
		SetDlgItemText(residue_static[x], CString(data.c_str()));
	}
}

void CProteinPropertiesDialog::LoadMolePercentageControls()
{
	propertiesobject->GetMolePercentageAminoAcidResidues();	
	int numberofcommonaminoacids = propertiesobject->GetNumberOfCommonAminoAcids();
	for (int x = 0; x < numberofcommonaminoacids; x++) {
		string data;
		propertiesobject->GetMolePercentOfResiduesString(x, data);
		SetDlgItemText(molepercent_static[x], CString(data.c_str()));
	}
}

void CProteinPropertiesDialog::LoadMolecularWeightControl()
{
	string data;
	propertiesobject->GetMolecularWeightString(data);
	m_molecular_weight = data.c_str();
}

void CProteinPropertiesDialog::LoadpIControl()
{
	string data;
	propertiesobject->GetpIString(data);
	m_pI = data.c_str();
}

void CProteinPropertiesDialog::LoadChargeAtpHControl()
{
	string data;
	propertiesobject->GetChargeAtpHString(data);
	m_ChargeAtpH = data.c_str();
}


void CProteinPropertiesDialog::LoadMolarAbsorptivityControl()
{
	string data;
	propertiesobject->GetMolarAbsoptivityString(data);
	m_A280 = data.c_str();
}

void CProteinPropertiesDialog::LoadEOnePercentControl()
{
	string data;
	propertiesobject->GetOnePercentSolutionAbsorptivityString(data);
	m_E280 = data.c_str();
}

BEGIN_MESSAGE_MAP(CProteinPropertiesDialog, CDialog)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

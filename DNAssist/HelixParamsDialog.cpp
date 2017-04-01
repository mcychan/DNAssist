// HelixParamsDialog.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "HelixParamsDialog.h"

// CHelixParamsDialog 對話方塊

IMPLEMENT_DYNAMIC(CHelixParamsDialog, CDialog)

CHelixParamsDialog::CHelixParamsDialog(const string& sequence, const char seqType, CWnd* pParent /*=NULL*/)
	: type(seqType), CDialog(IDD_HELIXPARAMS, pParent)
{
	helixparameter = make_unique<CDnaHelixParameters>(sequence, seqType);
	degreescelsius.AppendChar(176);
	degreescelsius.AppendChar('C');
}

CHelixParamsDialog::~CHelixParamsDialog()
{
}

void CHelixParamsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_G, m_Gnucleootides);
	DDX_Text(pDX, IDC_A, m_Anucleootides);
	DDX_Text(pDX, IDC_T, m_Tnucleootides);
	DDX_Text(pDX, IDC_C, m_Cnucleootides);
	DDX_Text(pDX, IDC_GplusC, m_GPlusCPercentage);
	DDX_Text(pDX, IDC_AplusT, m_APlusTPercentage);
	DDX_Text(pDX, IDC_LENGTH, m_Length);
	DDX_Text(pDX, IDC_MELT, m_melt);
	DDX_Text(pDX, IDC_ANNEAL, m_anneal);
	DDX_Text(pDX, IDC_MOLWEIGHTSINGLE, m_MolecularWeightSingle);
	DDX_Text(pDX, IDC_MOLWEIGHTDOUBLE, m_MolecularWeightDouble);
	DDX_Text(pDX, IDC_A_MOLE_DOUBLE, m_AbsorbanceDoubleMolar);
	DDX_Text(pDX, IDC_A_MG_DOUBLE, m_AbsorbanceDoubleMilligram);
	DDX_Text(pDX, IDC_A_MOL_SINGLE, m_AbsorbanceSingleMolar);
	DDX_Text(pDX, IDC_A_MG_SINGLE, m_AbsorbanceSingleMilligram);
	DDX_Text(pDX, IDC_APLUSTLABEL, m_APlusTLabel);
	DDX_Text(pDX, IDC_TLABEL, m_Tlabel);
}

void CHelixParamsDialog::Calculate()
{
	if (type == 'D') {
		m_Tlabel = _T("T:");
		m_APlusTLabel = _T("A+T%:");
	}
	else {
		m_Tlabel = _T("U:");
		m_APlusTLabel = _T("A+U%:");
	}

	if (helixparameter->GetMolecularWeightSingle() > 1000) {
		float weight = (float)helixparameter->GetMolecularWeightSingle() / 1000.0f;
		m_MolecularWeightSingle.Format(_T("%.2f kDa"), weight);
	}
	else
		m_MolecularWeightSingle.Format(_T("%d Da"), helixparameter->GetMolecularWeightSingle());

	if (type != 'D')
		m_MolecularWeightDouble = _T("-");
	else if (helixparameter->GetMolecularWeightDouble() > 1000) {
		float weight = (float)helixparameter->GetMolecularWeightDouble() / 1000.0f;
		m_MolecularWeightDouble.Format(_T("%.2f kDa"), weight);
	}
	else
		m_MolecularWeightDouble.Format(_T("%d Da"), helixparameter->GetMolecularWeightDouble());

	seqlength = helixparameter->GetLength();
	if (seqlength != 1) {
		if (type == 'D')
			m_Length.Format(_T("%d%s"), seqlength, _T(" basepairs"));
		else
			m_Length.Format(_T("%d%s"), seqlength, _T(" nucleotides"));
	}
	else {
		if (type == 'D')
			m_Length.Format(_T("%d%s"), seqlength, _T(" basepair"));
		else
			m_Length.Format(_T("%d%s"), seqlength, _T(" nucleotide"));
	}

	if (helixparameter->GetMeltingTemp() > 0)
		m_melt = MakeCelsiusString(helixparameter->GetMeltingTemp());
	else
		m_melt = _T("-");

	if (helixparameter->GetAnnealingTemp() > 0)
		m_anneal = MakeCelsiusString(helixparameter->GetAnnealingTemp());
	else
		m_anneal = _T("-");

	m_Gnucleootides.Format(_T("%d"), helixparameter->GetNumberOfG());
	if (helixparameter->GetNumberOfG() == 1)
		m_Gnucleootides += _T(" nucleotide");
	else
		m_Gnucleootides += _T(" nucleotides");

	m_Anucleootides.Format(_T("%d"), helixparameter->GetNumberOfA());
	if (helixparameter->GetNumberOfA() == 1)
		m_Anucleootides += _T(" nucleotide");
	else
		m_Anucleootides += _T(" nucleotides");

	if (type == 'D') {
		m_Tnucleootides.Format(_T("%d"), helixparameter->GetNumberOfT());
		if (helixparameter->GetNumberOfT() == 1)
			m_Tnucleootides += _T(" nucleotide");
		else
			m_Tnucleootides += _T(" nucleotides");
	}
	else {
		m_Tnucleootides.Format(_T("%d"), helixparameter->GetNumberOfU());
		if (helixparameter->GetNumberOfU() == 1)
			m_Tnucleootides += _T(" nucleotide");
		else
			m_Tnucleootides += _T(" nucleotides");
	}

	m_Cnucleootides.Format(_T("%d"), helixparameter->GetNumberOfC());
	if (helixparameter->GetNumberOfC() == 1)
		m_Cnucleootides += _T(" nucleotide");
	else
		m_Cnucleootides += _T(" nucleotides");

	m_GPlusCPercentage.Format(_T("%d%%"), helixparameter->GetGplusCPercentage());

	m_APlusTPercentage.Format(_T("%d%%"), helixparameter->GetAplusTPercentage());

	float absorbance;
	if (type == 'D') {
		float absorbanceMilligramDouble = helixparameter->GetAbsorbanceMilligramDouble();
		if (absorbanceMilligramDouble < 10)
			m_AbsorbanceDoubleMilligram.Format(_T("%.2f"), absorbanceMilligramDouble);
		else
			m_AbsorbanceDoubleMilligram.Format(_T("%d"), (int)absorbanceMilligramDouble);
		m_AbsorbanceDoubleMilligram += _T(" AU.mg-1.ml-1.cm-1");

		absorbance = helixparameter->GetAbsorbanceMolarDouble();
		if (absorbance >= 1000.0f && absorbance < 1000000.0f) {
			m_AbsorbanceDoubleMolar.Format(_T("%.2f"), (float)(absorbance / 1000.0f));
			m_AbsorbanceDoubleMolar += _T(" AU.mM-1.cm-1");
		}
		else if (absorbance < 1000.0f) {
			m_AbsorbanceDoubleMolar.Format(_T("%.2f"), (float)absorbance);
			m_AbsorbanceDoubleMolar += _T(" AU.M-1.cm-1");
		}
		else {
			m_AbsorbanceDoubleMolar.Format(_T("%.2f"), (float)(absorbance / 1000000.0f));
			m_AbsorbanceDoubleMolar += _T(" AU.μM-1.cm-1");
		}
	}
	else {
		m_AbsorbanceDoubleMilligram = _T("-");
		m_AbsorbanceDoubleMolar = _T("-");
	}

	float absorbanceMilligramSingle = helixparameter->GetAbsorbanceMilligramSingle();
	if (absorbanceMilligramSingle < 10)
		m_AbsorbanceSingleMilligram.Format(_T("%.2f"), absorbanceMilligramSingle);
	else
		m_AbsorbanceSingleMilligram.Format(_T("%d"), (int)absorbanceMilligramSingle);
	m_AbsorbanceSingleMilligram += _T(" AU.mg-1.ml-1.cm-1");

	absorbance = helixparameter->GetAbsorbanceMolarSingle();
	if (absorbance < 1000.0f) {
		m_AbsorbanceSingleMolar.Format(_T("%.2f"), absorbance);
		m_AbsorbanceSingleMolar += _T(" AU.M-1.cm-1");
	}
	else if (absorbance >= 1000.0f && absorbance < 1000000.0f) {
		absorbance /= 1000.0f;
		m_AbsorbanceSingleMolar.Format(_T("%.2f"), absorbance);
		m_AbsorbanceSingleMolar += _T(" AU.mM-1.cm-1");
	}
	else {
		absorbance /= 1000000.0f;
		m_AbsorbanceSingleMolar.Format(_T("%.2f"), absorbance);
		m_AbsorbanceSingleMolar += _T(" AU.μM-1.cm-1");
	}
}

BOOL CHelixParamsDialog::OnInitDialog()
{
	Calculate();

	BOOL bResult = CDialog::OnInitDialog();
	UpdateData(FALSE);
	return bResult;
}

CString CHelixParamsDialog::MakeCelsiusString(int degrees)
{
	int size = (int)(log10(degrees + 0.0f) + 1);
	CString result;
	result.Format(_T("%d%s"), degrees, degreescelsius);
	return result;
}

float CHelixParamsDialog::ConvertCaloriesToJoules(float number)
{
	return number / 0.2388f; // 1J = 0.2388 cal
}

BEGIN_MESSAGE_MAP(CHelixParamsDialog, CDialog)
END_MESSAGE_MAP()

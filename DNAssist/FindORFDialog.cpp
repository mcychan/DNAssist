// FindORF.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "DNAssistDoc.h"
#include "FindORFDialog.h"

#include "model/DNASearch.h"

// CFindORF 

IMPLEMENT_DYNAMIC(CFindORFDialog, CDialog)

CFindORFDialog::CFindORFDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FINDORF, pParent)
	, m_global(FALSE)
{

}

CFindORFDialog::~CFindORFDialog()
{
}

void CFindORFDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ORFSCOPEGLOBAL, m_global);
}

void CFindORFDialog::MakeTranspanency(double transparencyPercentage)
{
	// Enable WS_EX_LAYERED window extended style.
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(),
		GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(),
		GWL_EXSTYLE,
		ExtendedStyle | WS_EX_LAYERED);

	// Select the transparency percentage.
	// The alpha will be calculated accordingly.

	// Set the alpha for transparency.
	// 0 is transparent and 255 is opaque.
	double fAlpha = transparencyPercentage * (255.0 / 100);
	BYTE byAlpha = static_cast<BYTE>(fAlpha);
	SetLayeredWindowAttributes(0, byAlpha, LWA_ALPHA);
}

BEGIN_MESSAGE_MAP(CFindORFDialog, CDialog)
	ON_BN_CLICKED(IDC_FINDORF, OnBnClickedFind)
	ON_BN_CLICKED(IDC_FINDNEXTORF, OnBnClickedFindAgain)
END_MESSAGE_MAP()


// CFindORF 
void CFindORFDialog::GeneratePointArrayOfORFs(const vector<int>& startpositions, const vector<int>& stoppositions, vector<CPoint>& points, int sequencelength)
{
	points.clear();
	size_t count = startpositions.size();
	points.reserve(count);
	for(int x = 0; x < count; x++) {
		int y = 0;
		while(y < stoppositions.size() && (stoppositions[y] - startpositions[x]) <= 0 || (((stoppositions[y] - startpositions[x]) % 3) != 0))
			y++;
		if(y < stoppositions.size())
			points.emplace_back(startpositions[x], stoppositions[y]);
		else
			points.emplace_back(startpositions[x], sequencelength);
	}
}

void CFindORFDialog::SearchForORFs(CCodonTable* pCodontable, const string& sequence, vector<CPoint>& points, char seqType, char seqForm)
{
	vector<int> startcodonarray, stopcodonarray;
	vector<string> stopcodons;
	string startcodon;
	pCodontable->GetStartCodon(startcodon, seqType);
	pCodontable->GetStopCodons(stopcodons, seqType);
	CDNASearch searchobject(sequence, sequence.length(), seqType, seqForm);

	//	First search for the start codons
	searchobject.SearchMultiple(startcodon, startcodon.length(), startcodonarray, 0);
	startcodonarray.pop_back();//	Delete last entry, which is sequencelength

	//	Now search for all the stop codons
	for(int x = 0; x < stopcodons.size(); x++) {
		vector<int> temparray;
		searchobject.SearchMultiple(stopcodons[x], 3, temparray, 0);
		temparray.pop_back();
		stopcodonarray.insert(stopcodonarray.end(), temparray.cbegin(), temparray.cend());
	}
	sort(stopcodonarray.begin(), stopcodonarray.end());
	GeneratePointArrayOfORFs(startcodonarray, stopcodonarray, points, sequence.length());
}

void CFindORFDialog::OnBnClickedFind()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDORF);
}

void CFindORFDialog::OnBnClickedFindAgain()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDNEXTORF);	
}

LRESULT CFindORFDialog::NotifyView(UINT id)
{
	// Make it work in MFC SDI and MDI apps, with or without doc-view
	CFrameWnd * pFrame = (CFrameWnd*) ::AfxGetMainWnd();
	pFrame = pFrame->GetActiveFrame();

	CWnd * pView = pFrame->GetActiveView();

	if (!pView) pView = pFrame->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pView);

	LRESULT r = 0;
	if (pView) r = pView->SendMessage(WM_FIND_ORF_DIALOG, (WPARAM)id);
	return r;
}

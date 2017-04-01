// FindDialog.cpp : ��@��
//

#include "stdafx.h"
#include "DNAssist.h"
#include "FindDialog.h"

#include "model/DNASearch.h"

// CFindDialog ��ܤ��

IMPLEMENT_DYNAMIC(CFindDialog, CDialog)

CFindDialog::CFindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FINDDLG, pParent)
	, m_bScope(FALSE)
	, m_strFind(_T(""))
{

}

CFindDialog::~CFindDialog()
{
}

void CFindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SCOPEGLOBAL, m_bScope);
	DDX_Text(pDX, IDC_FINDTEXT, m_strFind);
}

void CFindDialog::MakeTranspanency(double transparencyPercentage)
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

BEGIN_MESSAGE_MAP(CFindDialog, CDialog)
	ON_BN_CLICKED(IDC_FIND, OnBnClickedFind)
	ON_BN_CLICKED(IDC_FINDAGAIN, OnBnClickedFindAgain)
END_MESSAGE_MAP()

// CFindDialog �T���B�z�`��
void CFindDialog::OnBnClickedFind()
{
	UpdateData(TRUE);
	NotifyView(IDC_FIND);
}

void CFindDialog::OnBnClickedFindAgain()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDAGAIN);
}

CPoint CFindDialog::Search(const string& seq, char sequence_type, char seqForm, int match)
{
	CDNASearch searchobject(seq, seq.length(), sequence_type, seqForm);
	string texttofind(CStringA(m_strFind).MakeUpper());
	match = searchobject.Search(texttofind, texttofind.length(), match + 1);
	return CPoint(match, texttofind.length());
}

LRESULT CFindDialog::NotifyView(UINT id)
{
	// Make it work in MFC SDI and MDI apps, with or without doc-view
	CFrameWnd * pFrame = (CFrameWnd*) ::AfxGetMainWnd();
	pFrame = pFrame->GetActiveFrame();

	CWnd * pView = pFrame->GetActiveView();

	if (!pView) pView = pFrame->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pView);

	LRESULT r = 0;
	if (pView) r = pView->SendMessage(WM_FIND_DIALOG, (WPARAM)id);
	return r;
}

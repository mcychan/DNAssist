// CodonOptionPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "CodonOptionPropertyPage.h"



// CCodonOptionPropertyPage

IMPLEMENT_DYNAMIC(CCodonOptionPropertyPage, CPropertyPage)

CCodonOptionPropertyPage::CCodonOptionPropertyPage()
	: CPropertyPage(IDD_CODON)
{
	int iSlash = theApp.codonPath.ReverseFind(_T('\\'));
	m_codonDir = theApp.codonPath.Left(iSlash);
	m_codonFile = theApp.codonPath.Mid(iSlash + 1);	
}

CCodonOptionPropertyPage::~CCodonOptionPropertyPage()
{
}

void CCodonOptionPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CODONPATH, m_codonDir);
	DDX_Text(pDX, IDC_CODONFILES, m_codonFile);
	DDX_Control(pDX, IDC_CODONFILES, m_cmbCodon);
	DDX_Control(pDX, IDC_CODONPATH, m_txtCodonDir);
}

void CCodonOptionPropertyPage::OnEnChangeCodonpath()
{
	m_cmbCodon.SetRedraw(FALSE);
	m_cmbCodon.ResetContent();
	m_txtCodonDir.GetWindowText(m_codonDir);

	TCHAR lpszOldPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpszOldPath);
	SetCurrentDirectory(m_codonDir);
	m_cmbCodon.Dir(DDL_READWRITE, _T("*.*"));
	SetCurrentDirectory(lpszOldPath);

	m_cmbCodon.SelectString(0, m_codonFile);
	m_cmbCodon.SetRedraw(TRUE);
	m_cmbCodon.Invalidate();
}

BOOL CCodonOptionPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	OnEnChangeCodonpath();

	return TRUE;
}

void CCodonOptionPropertyPage::OnOK()
{
	CPropertyPage::OnOK();
	CString codonPath;
	codonPath.Format(_T("%s\\%s"), m_codonDir, m_codonFile);
	if(PathFileExists(codonPath))
		theApp.codonPath = codonPath;
}

BEGIN_MESSAGE_MAP(CCodonOptionPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CODONPATHBROWSE, OnBnClickedCodonpathbrowse)
	ON_EN_CHANGE(IDC_CODONPATH, OnEnChangeCodonpath)
END_MESSAGE_MAP()


// CCodonOptionPropertyPage

void CCodonOptionPropertyPage::OnBnClickedCodonpathbrowse()
{
	CFolderPickerDialog folderPickerDialog(m_codonDir, OFN_FILEMUSTEXIST | OFN_ENABLESIZING, this,
		sizeof(OPENFILENAME));
	if (folderPickerDialog.DoModal() == IDOK) {
		POSITION pos = folderPickerDialog.GetStartPosition();
		if (pos) {
			m_codonDir = folderPickerDialog.GetNextPathName(pos);
			UpdateData(FALSE);
			OnEnChangeCodonpath();			
		}
	}
}

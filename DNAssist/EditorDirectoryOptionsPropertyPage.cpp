// EditorDirectoryOptionsPropertyPage.cpp : 實作檔
//

#include "stdafx.h"
#include "DNAssist.h"
#include "EditorDirectoryOptionsPropertyPage.h"



// CEditorDirectoryOptionsPropertyPage 對話方塊

IMPLEMENT_DYNAMIC(CEditorDirectoryOptionsPropertyPage, CPropertyPage)

CEditorDirectoryOptionsPropertyPage::CEditorDirectoryOptionsPropertyPage()
	: CPropertyPage(IDD_EDITOR_DIRECTORIES)
{
	m_sequenceDirectory = theApp.sequenceDirectory;	
}

CEditorDirectoryOptionsPropertyPage::~CEditorDirectoryOptionsPropertyPage()
{
}

void CEditorDirectoryOptionsPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITOR_SEQUENCE, m_sequenceDirectory);
}

void CEditorDirectoryOptionsPropertyPage::OnOK()
{
	CPropertyPage::OnOK();
	theApp.sequenceDirectory = m_sequenceDirectory;
}

BEGIN_MESSAGE_MAP(CEditorDirectoryOptionsPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_EDITOR_SEQUENCES_BROWSE, OnBnClickedEditorSequencesBrowse)
END_MESSAGE_MAP()


// CEditorDirectoryOptionsPropertyPage 訊息處理常式

void CEditorDirectoryOptionsPropertyPage::OnBnClickedEditorSequencesBrowse()
{
	CFolderPickerDialog folderPickerDialog(m_sequenceDirectory, OFN_FILEMUSTEXIST | OFN_ENABLESIZING, this,
		sizeof(OPENFILENAME));
	if (folderPickerDialog.DoModal() == IDOK) {
		POSITION pos = folderPickerDialog.GetStartPosition();
		if (pos) {
			m_sequenceDirectory = folderPickerDialog.GetNextPathName(pos);
			UpdateData(FALSE);
		}
	}
}

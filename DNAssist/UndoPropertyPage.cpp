// UndoPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "UndoPropertyPage.h"


// UndoPropertyPage

IMPLEMENT_DYNAMIC(CUndoPropertyPage, CPropertyPage)

CUndoPropertyPage::CUndoPropertyPage()
	: CPropertyPage(IDD_UNDO)
{
	m_undolevels.Format(_T("%d"), theApp.undoLevels);
}

CUndoPropertyPage::~CUndoPropertyPage()
{
}

void CUndoPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_UNDOLEVELS, m_undolevels);
}

void CUndoPropertyPage::OnOK()
{
	CPropertyPage::OnOK();
	theApp.undoLevels = _ttoi(m_undolevels);
}

BEGIN_MESSAGE_MAP(CUndoPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// UndoPropertyPage

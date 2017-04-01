// AddEnzymeDialog.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AddEnzymeDialog.h"



// CAddEnzymeDialog 

IMPLEMENT_DYNAMIC(CAddEnzymeDialog, CDialog)

CAddEnzymeDialog::CAddEnzymeDialog(AddEnzymeTransferBuffer* pAddEnzymeTransferBuffer, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ADDENZYME, pParent)
{
	m_pEnzymetransferbuffer = pAddEnzymeTransferBuffer;
}

CAddEnzymeDialog::~CAddEnzymeDialog()
{
}

void CAddEnzymeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDENZYMENAME, m_maxname);
	DDX_Text(pDX, IDC_ADDENZYMESITE, m_maxsite);
	DDX_Text(pDX, IDC_ADDENZYMEUPPER, m_maxupper);
	DDX_Text(pDX, IDC_ADDENZYMELOWER, m_maxlower);
}

BOOL CAddEnzymeDialog::OkIsValid()
{
	UpdateData(TRUE);
	return m_maxname.GetLength() > 0 && m_maxsite.GetLength() > 0 && m_maxupper.GetLength() > 0 && m_maxlower.GetLength() > 0;
}


BEGIN_MESSAGE_MAP(CAddEnzymeDialog, CDialog)
END_MESSAGE_MAP()


// CAddEnzymeDialog �T���B�z�`��
void CAddEnzymeDialog::OnOK()
{
	if(!OkIsValid()) {
		OnCancel();
		return;
	}

	CDialog::OnOK();
	m_pEnzymetransferbuffer->nametext = (LPCSTR) CStringA(m_maxname);
	m_pEnzymetransferbuffer->sitetext = (LPCSTR) CStringA(m_maxsite);
	m_pEnzymetransferbuffer->uppertext = (LPCSTR) CStringA(m_maxupper);
	m_pEnzymetransferbuffer->lowertext = (LPCSTR) CStringA(m_maxlower);
}

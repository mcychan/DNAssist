// FormatPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "FormatPropertyPage.h"



// CFormatPropertyPage

IMPLEMENT_DYNAMIC(CFormatPropertyPage, CPropertyPage)

CFormatPropertyPage::CFormatPropertyPage()
	: CPropertyPage(IDD_SHOWAS)
{

}

CFormatPropertyPage::~CFormatPropertyPage()
{
}

void CFormatPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

int CFormatPropertyPage::GetNumberOfCheckedCheckBoxes()
{
	int number = 0;
	if(m_sequence)
		number++;
	if(m_graphic)
		number++;
	if(m_line)
		number++;
	if(m_tables)
		number++;
	return number;
}

UINT CFormatPropertyPage::GetSelectedCheckBox()
{
	if(m_sequence)
		return IDC_FORMATSEQUENCE;
	if(m_graphic)
		return IDC_FORMATGRAPHIC;
	if(m_line)
		return IDC_FORMATLINE;
	if(m_tables)
		return IDC_FORMATTABLE;
	return 0;
}

void CFormatPropertyPage::EnableAll()
{
	UINT nIDs[] = {IDC_FORMATSEQUENCE, IDC_FORMATGRAPHIC, IDC_FORMATLINE, IDC_FORMATTABLE
	};
	for(int i=0; i< sizeof(nIDs) / sizeof (UINT); ++i)
		GetDlgItem(nIDs[i])->EnableWindow(TRUE);
}

void CFormatPropertyPage::DisableIfOnlyOneSelected()
{
	if(GetNumberOfCheckedCheckBoxes() == 1) {
		UINT nID = GetSelectedCheckBox();
		if(nID > 0)
			GetDlgItem(nID)->EnableWindow(FALSE);
		return;
	}
	EnableAll();
}

BEGIN_MESSAGE_MAP(CFormatPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_FORMATALL, OnBnClickedAll)
	ON_BN_CLICKED(IDC_FORMATSEQUENCE, OnBnClickedSeq)
	ON_BN_CLICKED(IDC_FORMATLINE, OnBnClickedLine)
	ON_BN_CLICKED(IDC_FORMATGRAPHIC, OnBnClickedGraphic)
	ON_BN_CLICKED(IDC_FORMATTABLE, OnBnClickedTable)
END_MESSAGE_MAP()

BOOL CFormatPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	DisableIfOnlyOneSelected();

	return TRUE;
}

// CFormatPropertyPage
void CFormatPropertyPage::OnBnClickedAll()
{
	m_all = !m_all;
	if(m_all)
		m_sequence = m_graphic = m_line = m_tables = m_all;
	UpdateData(FALSE);
}

void CFormatPropertyPage::OnBnClickedSeq()
{
	m_sequence = !m_sequence;
	if(!m_sequence)
		m_all = m_sequence;
	UpdateData(FALSE);
}

void CFormatPropertyPage::OnBnClickedLine()
{
	m_line = !m_line;
	if(!m_line)
		m_all = m_line;
	UpdateData(FALSE);
}

void CFormatPropertyPage::OnBnClickedGraphic()
{
	m_graphic = !m_graphic;
	if(!m_graphic)
		m_all = m_graphic;
	UpdateData(FALSE);
}

void CFormatPropertyPage::OnBnClickedTable()
{
	m_tables = !m_tables;
	if(!m_tables)
		m_all = m_tables;
	UpdateData(FALSE);
}

void CFormatPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto formatTransferBuffer = theApp.GetFormatTransferBuffer();
	formatTransferBuffer->all = m_all;
	formatTransferBuffer->sequence = m_sequence;
	formatTransferBuffer->graphic = m_graphic;
	formatTransferBuffer->line = m_line;
	formatTransferBuffer->tables = m_tables;
}

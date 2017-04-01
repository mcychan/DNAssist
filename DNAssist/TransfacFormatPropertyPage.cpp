// TransfacFormatPropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "TransfacFormatPropertyPage.h"


// TransfacFormatPropertyPage 

IMPLEMENT_DYNAMIC(CTransfacFormatPropertyPage, CPropertyPage)

CTransfacFormatPropertyPage::CTransfacFormatPropertyPage()
	: CPropertyPage(IDD_TRANSFAC_DISPLAY)
{
	auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer();
	m_showall = transFacFormatTransferBuffer->showall;
	m_list = transFacFormatTransferBuffer->list;
	m_map = transFacFormatTransferBuffer->map;
	m_table = transFacFormatTransferBuffer->table;
}

CTransfacFormatPropertyPage::~CTransfacFormatPropertyPage()
{
}

void CTransfacFormatPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_TRANSFAC_ALLFORMATS, m_showall);
	DDX_Check(pDX, IDC_TRANSFAC_SEQUENCELIST, m_list);
	DDX_Check(pDX, IDC_TRANSFAC_GRAPHICMAP, m_map);
	DDX_Check(pDX, IDC_TRANSFAC_TABLE, m_table);
}

int CTransfacFormatPropertyPage::GetNumberOfCheckedCheckBoxes()
{
	int number = 0;
	if (m_showall)
		number++;
	if (m_list)
		number++;
	if (m_map)
		number++;
	if (m_table)
		number++;
	return number;
}

UINT CTransfacFormatPropertyPage::GetSelectedCheckBox()
{
	if (m_showall)
		return IDC_TRANSFAC_ALLFORMATS;
	if (m_list)
		return IDC_TRANSFAC_SEQUENCELIST;
	if (m_map)
		return IDC_TRANSFAC_GRAPHICMAP;
	if (m_table)
		return IDC_TRANSFAC_TABLE;
	return 0;
}

void CTransfacFormatPropertyPage::EnableAll()
{
	UINT nIDs[] = { IDC_TRANSFAC_ALLFORMATS, IDC_TRANSFAC_SEQUENCELIST, IDC_TRANSFAC_GRAPHICMAP, IDC_TRANSFAC_TABLE
	};
	for (int i = 0; i< sizeof(nIDs) / sizeof(UINT); ++i)
		GetDlgItem(nIDs[i])->EnableWindow(TRUE);
}

void CTransfacFormatPropertyPage::DisableIfOnlyOneSelected()
{
	if (GetNumberOfCheckedCheckBoxes() == 1) {
		UINT nID = GetSelectedCheckBox();
		if (nID > 0)
			GetDlgItem(nID)->EnableWindow(FALSE);
		return;
	}
	EnableAll();
}

BOOL CTransfacFormatPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	DisableIfOnlyOneSelected();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CTransfacFormatPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_TRANSFAC_ALLFORMATS, OnBnClickedAllformats)
	ON_BN_CLICKED(IDC_TRANSFAC_SEQUENCELIST, OnBnClickedSeqList)
	ON_BN_CLICKED(IDC_TRANSFAC_GRAPHICMAP, OnBnClickedGraphicMap)
	ON_BN_CLICKED(IDC_TRANSFAC_TABLE, OnBnClickedTable)
END_MESSAGE_MAP()

// TransfacDisplayFormatPropertyPage
void CTransfacFormatPropertyPage::OnBnClickedAllformats()
{
	m_showall = !m_showall;
	if(m_showall)
		m_list = m_map = m_table = m_showall;
	DisableIfOnlyOneSelected();
	UpdateData(FALSE);
}

void CTransfacFormatPropertyPage::OnBnClickedSeqList()
{
	m_list = !m_list;
	if(!m_list)
		m_showall = m_list;
	DisableIfOnlyOneSelected();
	UpdateData(FALSE);
}

void CTransfacFormatPropertyPage::OnBnClickedGraphicMap()
{
	m_map = !m_map;
	if(!m_map)
		m_showall = m_map;
	DisableIfOnlyOneSelected();
	UpdateData(FALSE);
}

void CTransfacFormatPropertyPage::OnBnClickedTable()
{
	m_table = !m_table;
	if(!m_table)
		m_showall = m_table;
	DisableIfOnlyOneSelected();
	UpdateData(FALSE);
}

void CTransfacFormatPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer();
	transFacFormatTransferBuffer->showall = m_showall;
	transFacFormatTransferBuffer->list = m_list;
	transFacFormatTransferBuffer->map = m_map;
	transFacFormatTransferBuffer->table = m_table;
}

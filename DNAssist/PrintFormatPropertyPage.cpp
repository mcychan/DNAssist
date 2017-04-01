// PrintFormatPropertyPage.cpp : ��@��
//

#include "stdafx.h"
#include "DNAssist.h"
#include "PrintFormatPropertyPage.h"



// CPrintFormatPropertyPage ��ܤ��

IMPLEMENT_DYNAMIC(CPrintFormatPropertyPage, CPropertyPage)

CPrintFormatPropertyPage::CPrintFormatPropertyPage()
	: CPropertyPage(IDD_PRINTFORMAT)
{
	auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer();
	if (printFormatTransferBuffer->displaywatsononly)
		m_strand = 0;
	else if (printFormatTransferBuffer->displaywatsonandcrick)
		m_strand = 1;
	else
		m_strand = -1;

	if (printFormatTransferBuffer->displaycontinuoussequence)
		m_display = 0;
	else if (printFormatTransferBuffer->displaysequenceblocks)
		m_display = 1;
	else
		m_display = -1;

	m_blocksize.Format(_T("%d"), printFormatTransferBuffer->blocksize);
	m_blockgapsize.Format(_T("%d"), printFormatTransferBuffer->blockgapsize);

	if (printFormatTransferBuffer->numbersabovesequence)
		m_position = 0;
	else if (printFormatTransferBuffer->numbersbelowsequence)
		m_position = 1;
	else if (printFormatTransferBuffer->numbersrightsequence)
		m_position = 2;
	else
		m_position = -1;
}

CPrintFormatPropertyPage::~CPrintFormatPropertyPage()
{
}

void CPrintFormatPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DISPLAYWATSONONLY, m_strand);
	DDX_Radio(pDX, IDC_DISPLAYCONTINOUSSEQUENCE, m_display);
	DDX_CBString(pDX, IDC_BLOCKSIZE, m_blocksize);
	DDX_CBString(pDX, IDC_BLOCKGAPSIZE, m_blockgapsize);
	DDX_Radio(pDX, IDC_NUMBERABOVESEQUENCE, m_position);
}

BOOL CPrintFormatPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	EnableCombos();

	return TRUE;
}

// CPrintFormatPropertyPage �T���B�z�`��
void CPrintFormatPropertyPage::OnOK()
{
	CPropertyPage::OnOK();

	auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer();
	printFormatTransferBuffer->displaywatsononly = (m_strand == 0);
	printFormatTransferBuffer->displaywatsonandcrick = (m_strand == 1);
	printFormatTransferBuffer->displaycontinuoussequence = (m_display == 0);
	printFormatTransferBuffer->displaysequenceblocks = (m_display == 1);

	printFormatTransferBuffer->blocksize = _ttoi(m_blocksize);
	printFormatTransferBuffer->blockgapsize = _ttoi(m_blockgapsize);

	printFormatTransferBuffer->numbersabovesequence = (m_position == 0);
	printFormatTransferBuffer->numbersbelowsequence = (m_position == 1);
	printFormatTransferBuffer->numbersrightsequence = (m_position == 2);
}

BEGIN_MESSAGE_MAP(CPrintFormatPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_DISPLAYCONTINOUSSEQUENCE, OnBnClickedDisplay)
	ON_BN_CLICKED(IDC_DISPLAYSEQUENCEBLOCKS, OnBnClickedDisplay)
END_MESSAGE_MAP()

void CPrintFormatPropertyPage::EnableCombos()
{
	GetDlgItem(IDC_BLOCKSIZE)->EnableWindow(m_display);
	GetDlgItem(IDC_BLOCKGAPSIZE)->EnableWindow(m_display);
}

void CPrintFormatPropertyPage::OnBnClickedDisplay()
{
	UpdateData(TRUE);
	EnableCombos();
}


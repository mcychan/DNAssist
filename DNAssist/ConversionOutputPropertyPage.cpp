// ConversionOutputPropertyPage.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "ConversionOutputPropertyPage.h"



// CConversionOutputPropertyPage

IMPLEMENT_DYNAMIC(CConversionOutputPropertyPage, CPropertyPage)

CConversionOutputPropertyPage::CConversionOutputPropertyPage()
	: CPropertyPage(IDD_OUTPUT)
{
	m_replace = theApp.ConversionOutputTransferBuffer;
}

CConversionOutputPropertyPage::~CConversionOutputPropertyPage()
{
}

void CConversionOutputPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_OUTPUTREPLACE, m_replace);
}


BEGIN_MESSAGE_MAP(CConversionOutputPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CConversionOutputPropertyPage
void CConversionOutputPropertyPage::OnOK()
{
	CPropertyPage::OnOK();
	theApp.ConversionOutputTransferBuffer = m_replace;	
}

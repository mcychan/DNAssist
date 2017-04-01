// FontPage.cpp : implementation file
//
 
#include "stdafx.h"
#include "DNAssist.h"
#include "FontPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CFontPage property page
 
IMPLEMENT_DYNCREATE(CFontPage, CPropertyPage)
 
CFontPage::CFontPage(BOOL isPrint, LOGFONT* plogfont /*= NULL*/) : CPropertyPage(CFontPage::IDD)
{
	//{{AFX_DATA_INIT(CFontPage)
	//}}AFX_DATA_INIT
	m_isPrint = isPrint;
	LOGFONT logfont;
	CWindowDC dc(GetDesktopWindow());
	m_cyPixelsPerInch = GetDeviceCaps(dc, LOGPIXELSY);	
	if(plogfont == NULL) {
		CFont font;
		if(m_isPrint) {
			auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
			font.CreateFont(printFontTransferBuffer->facesize, 0, 0, 0, printFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, theApp.printFontFace);
		}
		else {
			auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
			font.CreateFont(editorFontTransferBuffer->facesize, 0, 0, 0, editorFontTransferBuffer->facestyle, FALSE, FALSE, FALSE, 1, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH|FF_DONTCARE, theApp.editorFontFace);
		}
		font.GetLogFont(&logfont);
		plogfont = &logfont;
	}
	m_fontSample.CreateFontIndirect(plogfont);
 
	m_bStrikeOut = plogfont->lfStrikeOut;
	m_bUnderline = plogfont->lfUnderline;
	m_sFont = plogfont->lfFaceName;
	m_sSize.Format(_T("%d"), abs(plogfont->lfHeight));
	m_sStyle = _T("Regular");
	if(plogfont->lfWeight >= 700 && plogfont->lfItalic)
		m_sStyle = _T("Bold Italic");
	else if(plogfont->lfItalic )
		m_sStyle = _T("Italic");
	else if (plogfont->lfWeight >= 700)
		m_sStyle = _T("Bold");	
}
 
CFontPage::~CFontPage()
{
}
 
void CFontPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontPage)
	DDX_Control(pDX, IDC_SAMPLE, m_staticSample);
	DDX_Control(pDX, IDC_STYLE, m_comboStyle);
	DDX_Control(pDX, IDC_FONTSIZE, m_comboSize);
	DDX_Control(pDX, IDC_FONT, m_comboFont);
	DDX_CBString(pDX, IDC_FONT, m_sFont);
	DDX_CBString(pDX, IDC_FONTSIZE, m_sSize);
	DDV_MaxChars(pDX, m_sSize, LF_FACESIZE);
	DDX_CBString(pDX, IDC_STYLE, m_sStyle);
	//}}AFX_DATA_MAP
}
 
 
BEGIN_MESSAGE_MAP(CFontPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFontPage)
	ON_CBN_SELCHANGE(IDC_FONT, OnSelChange)
	ON_CBN_SELCHANGE(IDC_STYLE, OnSelChange)
	ON_CBN_SELCHANGE(IDC_FONTSIZE, OnSelChange)
	ON_BN_CLICKED(IDC_STRIKEOUT, OnSelChange)
	ON_BN_CLICKED(IDC_UNDERLINE, OnSelChange)
	ON_CBN_KILLFOCUS(IDC_FONT, OnSelChange)
	ON_CBN_KILLFOCUS(IDC_STYLE, OnSelChange)
	ON_CBN_KILLFOCUS(IDC_FONTSIZE, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////
// CFontPage message handlers
 
 
BOOL CFontPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	// Populate combo box controls and set their default values to be the
	// same as the current.	

	CWindowDC dc(this);
	LOGFONT logfont;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfFaceName[0] = '\0';
	logfont.lfPitchAndFamily = 0;
	EnumFontFamiliesEx( dc.m_hDC, &logfont, (FONTENUMPROC)FontEnumProc, 
			(LPARAM) this, 0 );	

	LOGFONT lf;
	GetCurrentFont(&lf);
	const int currentPointSize = MulDiv(lf.lfHeight, 72, m_cyPixelsPerInch);	

	// Fill Size combobox with "common" sizes

	TCHAR* Defaults[] = { _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("14"),
				_T("16"), _T("18"), _T("20"), _T("22"), _T("24"), _T("26"), 
				_T("28"), _T("36"), _T("48") };

	for (int i = 0; i < (sizeof(Defaults)/sizeof(Defaults[0])); i++)
	{
		int pos = m_comboSize.AddString(Defaults[i]);

		// Set the current selection
		if (currentPointSize == _ttoi(Defaults[i]))
			m_comboSize.SetCurSel(pos);
	}


	// Fill Style combobox with "common" styles
	m_comboStyle.AddString( _T("Regular") );
	m_comboStyle.AddString( _T("Bold") );
	m_comboStyle.AddString( _T("Italic") );
	m_comboStyle.AddString( _T("Bold Italic") );

	// Make sure current selections are correct

	int indexCS = m_comboStyle.FindStringExact(-1, m_sStyle);
	if (CB_ERR != indexCS)
		m_comboStyle.SetCurSel(indexCS);

	int indexFS = m_comboFont.FindStringExact(-1, lf.lfFaceName);
	if (CB_ERR != indexFS)
		m_comboFont.SetCurSel(indexFS);

	OnSelChange(); // ensure sample text is updated
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
 
int CALLBACK CFontPage::FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, 
		int FontType, CFontPage* pFontPage)
{
 
	if(pFontPage->m_comboFont.FindStringExact( 0, (LPCTSTR)lpelfe->elfFullName) == CB_ERR) {
		bool blsMonoSpaced = lpelfe->elfLogFont.lfPitchAndFamily & FIXED_PITCH;
		// Add to list
		if(pFontPage->m_isPrint || blsMonoSpaced)
			pFontPage->m_comboFont.AddString((LPCTSTR) lpelfe->elfFullName);
	}
 
 	return 1;
}
 
void CFontPage::OnSelChange() 
{
	// The selection hasn't changed yet, so change it
	if( IsChild( GetFocus() ) && 
			GetFocus()->GetParent()->IsKindOf( RUNTIME_CLASS( CComboBox ) ) )
	{
		CComboBox *cb = (CComboBox *)GetFocus()->GetParent();
		CString sText;
		if( cb->GetCurSel() != CB_ERR )
		{
			cb->GetLBText( cb->GetCurSel(), sText );
			cb->SetWindowText( sText );
		}
	}
 
	UpdateData(TRUE);
	
	LOGFONT logfont;
	m_fontSample.GetLogFont( &logfont );
	logfont.lfStrikeOut = m_bStrikeOut;
	logfont.lfUnderline = m_bUnderline;
	memcpy( logfont.lfFaceName, m_sFont, LF_FACESIZE );
 
	logfont.lfHeight = MulDiv(_ttoi(m_sSize), m_cyPixelsPerInch, 72);
	 
	logfont.lfWeight = 400;			//Regular
	logfont.lfItalic = FALSE;
	if( m_sStyle.Find( _T("Italic") ) != -1 )
		logfont.lfItalic = TRUE;
	if( m_sStyle.Find( _T("Bold") ) != -1 )
		logfont.lfWeight = 700;
 
	m_fontSample.DeleteObject();
	m_fontSample.CreateFontIndirect( &logfont );
 	m_staticSample.SetFont(&m_fontSample);	
}
 
void CFontPage::GetCurrentFont(LPLOGFONT lplf)
{
	m_fontSample.GetLogFont( lplf );
}

void CFontPage::OnOK()
{
	CPropertyPage::OnOK();
	LOGFONT logfont;
	GetCurrentFont(&logfont);

	if(m_isPrint) {
		auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
		theApp.printFontFace = logfont.lfFaceName;
		printFontTransferBuffer->facestyle = logfont.lfWeight;
		printFontTransferBuffer->facesize = logfont.lfHeight;
	}
	else {
		auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
		theApp.editorFontFace = logfont.lfFaceName;
		editorFontTransferBuffer->facestyle = logfont.lfWeight;
		editorFontTransferBuffer->facesize = logfont.lfHeight;
	}
}

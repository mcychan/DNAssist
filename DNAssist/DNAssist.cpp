
// DNAssist.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DNAssist.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DNAssistDoc.h"
#include "AlignView.h"
#include "DrewPlotView.h"
#include "ResultView.h"
#include "DNAssistView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDNAssistApp

BEGIN_MESSAGE_MAP(CDNAssistApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_NEW_DNA, OnNewDna)
	ON_COMMAND(ID_NEW_RNA, OnNewRna)
	ON_COMMAND(ID_NEW_PROTEIN, OnNewProtein)
	ON_COMMAND(ID_NEW_DEGENERATEDNA, OnNewDegeneratedna)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)	
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_COMMAND(ID_HELP_ENTREZ, OnHelpEntrez)
	ON_COMMAND(ID_HELP_BLAST, OnHelpBlast)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
END_MESSAGE_MAP()


// CDNAssistApp construction

CDNAssistApp::CDNAssistApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("DNAssist.3.10"));

	// TODO: add construction code here,
	m_pTaskbarList.Release();
	initedTaskBar = false;
}

// The one and only CDNAssistApp object

CDNAssistApp theApp;


// CDNAssistApp initialization

BOOL CDNAssistApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("DNAssist"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	m_pDocManager = new CMultDocManager(_T(".seq;.txt;.fas;.gcg;.gbk;.str"), _T("DNAssist files (*.seq);Text files (*.txt);Fasta files (*.fas);GCG files (*.gcg);GenBank files (*.gbk);Strider files (*.str)"));

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	// first file type no save
	CDocTemplate* pMainTemplate = new CMultiDocTemplate(IDR_strTYPE,
		RUNTIME_CLASS(CDNAssistDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDNAssistView));
	if (!pMainTemplate)
		return FALSE;
	pMainTemplate->SetContainerInfo(IDR_strTYPE_CNTR_IP);
	AddDocTemplate(pMainTemplate);

	m_pDrewPlotTemplate = new CMultiDocTemplate(IDR_plotTYPE,
		RUNTIME_CLASS(CDrewPlotDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDrewPlotView));
	m_pDrewPlotTemplate->SetContainerInfo(IDR_strTYPE_CNTR_IP);
	AddDocTemplate(m_pDrewPlotTemplate);

	m_pDNAViewTemplate = new CMultiDocTemplate(IDR_viewTYPE,
		RUNTIME_CLASS(CResultDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CResultView));
	m_pDNAViewTemplate->SetContainerInfo(IDR_strTYPE_CNTR_IP);
	AddDocTemplate(m_pDNAViewTemplate);

	m_pAlignViewTemplate = new CMultiDocTemplate(IDR_viewTYPE,
		RUNTIME_CLASS(CAlignDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAlignView));
	m_pAlignViewTemplate->SetContainerInfo(IDR_strTYPE_CNTR_IP);
	AddDocTemplate(m_pAlignViewTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}

// CDNAssistApp message handlers

int CDNAssistApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

#if (_MSC_VER > 1400)
	m_pTaskbarList.Release();
#endif

	return CWinApp::ExitInstance();
}

static bool IsWin7() {
	DWORD dwMajor = LOBYTE(LOWORD(GetVersion()));
	DWORD dwMinor = HIBYTE(LOWORD(GetVersion()));
	return (dwMajor > 6 || (dwMajor == 6 && dwMinor > 0));
}

void CDNAssistApp::SetProgressValue(short pos)
{
#if (_MSC_VER > 1400)
	if (IsWin7()) {
		if (!initedTaskBar) {
			m_pTaskbarList.CoCreateInstance(CLSID_TaskbarList);
			initedTaskBar = true;
		}
		if(m_pTaskbarList)
			m_pTaskbarList->SetProgressValue(m_pMainWnd->m_hWnd, pos, 100);
	}	
#endif
}

void CDNAssistApp::ProgressComplete()
{
	// Clear the taskbar progress bar.
#if (_MSC_VER > 1400)
	if (IsWin7() && m_pTaskbarList)
		m_pTaskbarList->SetProgressState(m_pMainWnd->m_hWnd, TBPF_NOPROGRESS);
#endif
}


// CDNAssistApp message handlers
CString CDNAssistApp::GetAppLoc()
{
	CString m_sAppPath;
	::GetModuleFileName(NULL, m_sAppPath.GetBuffer(MAX_PATH), MAX_PATH);
	m_sAppPath.ReleaseBuffer();

	int nIndex = m_sAppPath.ReverseFind('\\');
	if (nIndex != -1)
		m_sAppPath = m_sAppPath.Left(nIndex + 1);
	return m_sAppPath;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_ABOUT_COPYRIGHT, _T(VER_LEGALCOPYRIGHT_STR));
	CenterWindow();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CDNAssistApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CDocument* CDNAssistApp::GetActiveDocument()
{
	CDocument* pDoc = NULL;
	CWnd* pWndMain = AfxGetMainWnd();
	if (NULL != pWndMain)
	{
		if (pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			// MDI application, so first we have to get the active MDI child frame.
			CFrameWnd* pFrame = dynamic_cast<CMDIFrameWnd*>(pWndMain)->MDIGetActive();
			if (NULL != pFrame)
			{
				pDoc = pFrame->GetActiveDocument();
			}
		}
		else if (pWndMain->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{
			// SDI appllication so main window is the active frame. 
			pDoc = dynamic_cast<CFrameWnd*>(pWndMain)->GetActiveDocument();
		}
		else
		{
			ASSERT(FALSE); // Neither MDI nor SDI application.
		}
	}
	return pDoc;
}

double CDNAssistApp::GetSystemScaleFactor()				// returns 1.0 for no scaling
{
	if (m_scale > 0)
		return m_scale;
	// some simple caching to speed things up

	// get DPI for the system
	HDC		hdc;
	hdc = ::GetDC(NULL);
	UINT	dpiX = 96;		// default DPI

	dpiX = GetDeviceCaps(hdc, LOGPIXELSX);

	m_scale = (double) dpiX / 96.0;
	if (m_scale < 1.0)
		m_scale = 1.0;		// prevent funny business with super large monitors returning very low DPI values

	::ReleaseDC(NULL, hdc);
	return m_scale;
}

int CDNAssistApp::GetDisplayFormatOptions()
{
	auto formatTransferBuffer = theApp.GetFormatTransferBuffer();
	if(formatTransferBuffer->all)
		return DF_ALL;
	int value = 0;
	if(formatTransferBuffer->sequence)
		value |= DF_LIST;
	if(formatTransferBuffer->graphic)
		value |= DF_GRAPHIC;
	if(formatTransferBuffer->line)
		value |= DF_LINE;
	if(formatTransferBuffer->tables)
		value |= DF_TABLE;
	return value;
}

int CDNAssistApp::GetTransFacFormatOptions()
{
	auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer();
	if(transFacFormatTransferBuffer->showall)
		return DF_ALL;
	int value = 0;
	if(transFacFormatTransferBuffer->list)
		value |= DF_LIST;
	if(transFacFormatTransferBuffer->map)
		value |= DF_GRAPHIC;
	if(transFacFormatTransferBuffer->table)
		value |= DF_TABLE;
	return value;
}

int CDNAssistApp::GetEnzymeAttributeOptions()
{
	int value = 0;
	auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer();
	if(enzymeOptionsTransferBuffer->fiveprime)
		value |= EL_5PRIME;
	if(enzymeOptionsTransferBuffer->threeprime)
		value |= EL_3PRIME;
	if(enzymeOptionsTransferBuffer->blunt)
		value |= EL_BLUNT;
	if(enzymeOptionsTransferBuffer->fourcutter)
		value |= EL_4CUTTER;
	if(enzymeOptionsTransferBuffer->sixcutter)
		value |= EL_6CUTTER;
	if(enzymeOptionsTransferBuffer->eightcutter)
		value |= EL_8CUTTER;
	if(enzymeOptionsTransferBuffer->palindrome)
		value |= EL_PALINDROME;
	if(enzymeOptionsTransferBuffer->outside)
		value |= EL_CUTOUTSIDE;

	if(enzymeOptionsTransferBuffer->all)
		value |= EL_ALL;
	if(enzymeOptionsTransferBuffer->selection)
		value |= EL_SELECTION;
	if(enzymeOptionsTransferBuffer->graphicunique)
		value |= EL_GRAPHICUNIQUE;
	if(enzymeOptionsTransferBuffer->graphicuniqueandsel)
		value |= EL_GRAPHICUNIQUEANDSEL;
	if(enzymeOptionsTransferBuffer->graphicselection)
		value |= EL_GRAPHICSELECTION;

	return value;
}

int CDNAssistApp::GetTransFacOptions()
{
	int value = EL_ALL;
	if (theApp.TransFacOptions == graphicunique)
		value |= EL_GRAPHICUNIQUE;
	if (theApp.TransFacOptions == graphicuniqueandsel)
		value |= EL_GRAPHICUNIQUEANDSEL;
	if (theApp.TransFacOptions == graphicselection)
		value |= EL_GRAPHICSELECTION;
	return value;
}

CDrewPlotDoc* CDNAssistApp::OnNewDrewPlot()
{
	return dynamic_cast<CDrewPlotDoc*>(theApp.m_pDrewPlotTemplate->OpenDocumentFile(NULL));
}

CResultDoc* CDNAssistApp::OnNewResultDoc()
{
	return dynamic_cast<CResultDoc*>(theApp.m_pDNAViewTemplate->OpenDocumentFile(NULL));
}

CAlignDoc* CDNAssistApp::OnNewAlignDoc()
{
	return dynamic_cast<CAlignDoc*>(theApp.m_pAlignViewTemplate->OpenDocumentFile(NULL));
}

// CDNAssistApp message handlers
void CDNAssistApp::OnNewDna()
{
	OnFileNew();
	CDNAssistDoc* pDoc = dynamic_cast<CDNAssistDoc*>(GetActiveDocument());
	pDoc->SetSeqType('D');
}

void CDNAssistApp::OnNewRna()
{
	OnFileNew();
	CDNAssistDoc* pDoc = dynamic_cast<CDNAssistDoc*>(GetActiveDocument());
	pDoc->SetSeqType('R');
}

void CDNAssistApp::OnNewProtein()
{
	OnFileNew();
	CDNAssistDoc* pDoc = dynamic_cast<CDNAssistDoc*>(GetActiveDocument());
	pDoc->SetSeqType('P');
}

void CDNAssistApp::OnNewDegeneratedna()
{
	OnFileNew();
	CDNAssistDoc* pDoc = dynamic_cast<CDNAssistDoc*>(GetActiveDocument());
	pDoc->SetSeqType('G');
}

void CDNAssistApp::OnHelpEntrez()
{
	ShellExecute(NULL, _T("open"), _T("https://www.ncbi.nlm.nih.gov/pubmed"), NULL, NULL, SW_SHOWNORMAL);
}

void CDNAssistApp::OnHelpBlast()
{
	ShellExecute(NULL, _T("open"), _T("https://blast.ncbi.nlm.nih.gov/Blast.cgi"), NULL, NULL, SW_SHOWNORMAL);
}

void CDNAssistApp::OnHelpContents()
{
	const CString helpDir = GetAppLoc() + _T("Help");
	ShellExecute(AfxGetMainWnd()->m_hWnd, _T("open"), _T("dnassist.chm"), NULL, helpDir, SW_SHOWNORMAL);
}

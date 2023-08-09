
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DNAssist.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_USER_THREAD_COMPLETE, OnProgressComplete)
	ON_MESSAGE(WM_USER_THREAD_UPDATE_PROGRESS, OnUpdateProgress)
	ON_WM_MOVE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_SEL,
	ID_INDICATOR_POS,
	ID_INDICATOR_SEQTYPE,
	ID_INDICATOR_FORM
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	theApp.m_pStatusBar = &m_wndStatusBar;

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (theApp.IsWin7())
		CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void**)&m_pTaskbarList);

	// create mdi tabs 
	m_wndMDITabs.Create(this, MT_TOP);

	UINT nl;
	theApp.codonPath = theApp.GetProfileString(_T("Path"), _T("Codon"));
	if (theApp.codonPath == _T("")) {
		theApp.sequenceDirectory = theApp.GetAppLoc() + _T("sequences");
		theApp.codonPath = theApp.GetAppLoc() + _T("Codon\\default.txt");
		theApp.enzymePath = theApp.GetAppLoc() + _T("Enzymes\\default.txt");
		theApp.transfacPath = theApp.GetAppLoc() + _T("transfac.db");
		theApp.numberOfGroups = 8;

		auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer(TRUE);
		printFormatTransferBuffer->displaywatsonandcrick = 1;
		printFormatTransferBuffer->displaysequenceblocks = 1;
		printFormatTransferBuffer->numbersabovesequence = 1;
		printFormatTransferBuffer->blocksize = 10;
		printFormatTransferBuffer->blockgapsize = 2;

		theApp.printFontFace = _T("Courier New");
		auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer(TRUE);
		printFontTransferBuffer->facesize = -12;
		printFontTransferBuffer->facestyle = FW_NORMAL;

		theApp.editorFontFace = _T("Courier New");
		auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer(TRUE);
		editorFontTransferBuffer->facesize = 14;
		editorFontTransferBuffer->facestyle = FW_NORMAL;

		auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer(TRUE);
		transFacFormatTransferBuffer->showall = true;
		transFacFormatTransferBuffer->list = true;
		transFacFormatTransferBuffer->map = true;
		transFacFormatTransferBuffer->table = true;

		theApp.TransFacOptions = graphicunique;

		theApp.hydrophobicityOptions = 5;
		theApp.hydrophilicityOptions = 5;
		theApp.antigenicityOptions = 5;

		auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer(TRUE);
		alignOptionsBuffer->gapopeningpenalty = 10;
		alignOptionsBuffer->gapextensionpenalty = 0.1f;
		alignOptionsBuffer->delaydivergentsequences = 40;
		alignOptionsBuffer->dnatransitionweight = 0.5f;
		alignOptionsBuffer->proteinweightmatrix = 0;
		alignOptionsBuffer->dnaweightmatrix = 0;
		alignOptionsBuffer->residuespecificpenalty = 0;
		alignOptionsBuffer->hydrophilicpenalty = 0;
		alignOptionsBuffer->endgapseparation = 0;
		alignOptionsBuffer->gapseparationdistance = 8;
		theApp.hydrophilicresidues = _T("GPSNDQEKR");
		alignOptionsBuffer->identicalforegroundColor = RGB(0, 0, 0);
		alignOptionsBuffer->identicalbackgroundColor = RGB(255, 0, 255);
		alignOptionsBuffer->conservedforegroundColor = RGB(0, 0, 0);
		alignOptionsBuffer->conservedbackgroundColor = RGB(0, 128, 192);
		alignOptionsBuffer->percentageconserved = 100;
		alignOptionsBuffer->percentageidentical = 100;

		auto formatTransferBuffer = theApp.GetFormatTransferBuffer(TRUE);
		formatTransferBuffer->all = true;
		formatTransferBuffer->sequence = true;
		formatTransferBuffer->graphic = true;
		formatTransferBuffer->line = true;
		formatTransferBuffer->tables = true;

		auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer(TRUE);
		enzymeOptionsTransferBuffer->all = true;
		enzymeOptionsTransferBuffer->wholeset = true;
		enzymeOptionsTransferBuffer->fiveprime = true;
		enzymeOptionsTransferBuffer->threeprime = true;
		enzymeOptionsTransferBuffer->blunt = true;
		enzymeOptionsTransferBuffer->fourcutter = true;
		enzymeOptionsTransferBuffer->sixcutter = true;
		enzymeOptionsTransferBuffer->eightcutter = true;
		enzymeOptionsTransferBuffer->palindrome = true;
		enzymeOptionsTransferBuffer->outside = true;
		enzymeOptionsTransferBuffer->graphicunique = 1;

		theApp.undoLevels = 10;

		theApp.ConversionOutputTransferBuffer = true;
	}
	else {
		theApp.sequenceDirectory = theApp.GetProfileString(_T("Path"), _T("SequenceDir"), theApp.GetAppLoc() + _T("sequences"));
		theApp.codonPath = theApp.GetProfileString(_T("Path"), _T("Codon"), theApp.GetAppLoc() + _T("Codon\\default.txt"));
		theApp.enzymePath = theApp.GetProfileString(_T("Path"), _T("Enzyme"), theApp.GetAppLoc() + _T("Enzymes\\default.txt"));
		theApp.transfacPath = theApp.GetProfileString(_T("Path"), _T("Transfac"), theApp.GetAppLoc() + _T("transfac.db"));
		theApp.numberOfGroups = theApp.GetProfileInt(_T("Options"), _T("NumberOfGroups"), 8);

		auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer();
		theApp.GetProfileBinary(_T("Format"), _T("Print"), (LPBYTE*)&printFormatTransferBuffer, &nl);
		theApp.SetPrintFormatTransferBuffer(printFormatTransferBuffer);

		theApp.printFontFace = theApp.GetProfileString(_T("Font"), _T("PrintFace"), _T("Courier New"));
		auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
		theApp.GetProfileBinary(_T("Font"), _T("Print"), (LPBYTE*)&printFontTransferBuffer, &nl);
		theApp.SetPrintFontTransferBuffer(printFontTransferBuffer);

		theApp.editorFontFace = theApp.GetProfileString(_T("Font"), _T("EditorFace"), _T("Courier New"));
		auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
		theApp.GetProfileBinary(_T("Font"), _T("Editor"), (LPBYTE*)&editorFontTransferBuffer, &nl);
		theApp.SetEditorFontTransferBuffer(editorFontTransferBuffer);

		auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer();
		theApp.GetProfileBinary(_T("Format"), _T("TransFac"), (LPBYTE*)&transFacFormatTransferBuffer, &nl);
		theApp.SetTransFacFormatTransferBuffer(transFacFormatTransferBuffer);

		int transFacOptions = theApp.GetProfileInt(_T("Options"), _T("TransFac"), graphicunique);
		theApp.TransFacOptions = transFacOptions;

		theApp.hydrophobicityOptions = theApp.GetProfileInt(_T("Options"), _T("Hydrophobicity"), 5);
		theApp.hydrophilicityOptions = theApp.GetProfileInt(_T("Options"), _T("Hydrophilicity"), 5);
		theApp.antigenicityOptions = theApp.GetProfileInt(_T("Options"), _T("Antigenicity"), 5);

		theApp.hydrophilicresidues = theApp.GetProfileString(_T("Options"), _T("HydrophilicResidues"), _T("GPSNDQEKR"));
		auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
		theApp.GetProfileBinary(_T("Options"), _T("Align"), (LPBYTE*)&alignOptionsBuffer, &nl);
		theApp.SetAlignOptionsBuffer(alignOptionsBuffer);

		auto formatTransferBuffer = theApp.GetFormatTransferBuffer();
		theApp.GetProfileBinary(_T("Options"), _T("Format"), (LPBYTE*)&formatTransferBuffer, &nl);
		theApp.SetFormatTransferBuffer(formatTransferBuffer);

		auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer();
		theApp.GetProfileBinary(_T("Options"), _T("Enzyme"), (LPBYTE*)&enzymeOptionsTransferBuffer, &nl);
		theApp.SetEnzymeOptionsTransferBuffer(enzymeOptionsTransferBuffer);

		theApp.undoLevels = theApp.GetProfileInt(_T("Options"), _T("UndoLevels"), 10);
		theApp.ConversionOutputTransferBuffer = theApp.GetProfileInt(_T("Window"), _T("ConversionOutput"), 1);
	}

	WINDOWPLACEMENT* lwp;

	if (theApp.GetProfileBinary(_T("Window"), _T("Position"), (LPBYTE*)&lwp, &nl)) {
		if (lwp->rcNormalPosition.left > 0 && lwp->rcNormalPosition.top > 0)
			SetWindowPlacement(lwp);
		delete[] lwp;
	}
	else
		SendMessage(WM_SIZE);

	return 0;
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);

	// synchronise with views
	m_wndMDITabs.Update();
}

afx_msg LRESULT CMainFrame::OnSizeParent(WPARAM, LPARAM lParam)
{
	m_wndMDITabs.Update();
	return 0L;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if(!CMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

LRESULT CMainFrame::OnProgressComplete(WPARAM wParam, LPARAM lParam)
{
	// Clear the taskbar progress bar.
	if (m_pTaskbarList)
		m_pTaskbarList->SetProgressState(m_hWnd, TBPF_NOPROGRESS);

	return 0L;
}

LRESULT CMainFrame::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	int pos = (int)wParam;
	if (m_pTaskbarList)
		m_pTaskbarList->SetProgressValue(m_hWnd, pos, 100);

	return 0L;
}

BOOL CMainFrame::DestroyWindow()
{
	theApp.WriteProfileString(_T("Path"), _T("SequenceDir"), theApp.sequenceDirectory);
	theApp.WriteProfileString(_T("Path"), _T("Codon"), theApp.codonPath);
	theApp.WriteProfileString(_T("Path"), _T("Enzyme"), theApp.enzymePath);
	theApp.WriteProfileString(_T("Path"), _T("Transfac"), theApp.transfacPath);
	theApp.WriteProfileInt(_T("Options"), _T("NumberOfGroups"), theApp.numberOfGroups);

	auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer();
	theApp.WriteProfileBinary(_T("Format"), _T("Print"), (LPBYTE)printFormatTransferBuffer, sizeof(*printFormatTransferBuffer));

	theApp.WriteProfileString(_T("Font"), _T("PrintFace"), theApp.printFontFace);
	auto printFontTransferBuffer = theApp.GetPrintFontTransferBuffer();
	theApp.WriteProfileBinary(_T("Font"), _T("Print"), (LPBYTE)printFontTransferBuffer, sizeof(*printFontTransferBuffer));

	theApp.WriteProfileString(_T("Font"), _T("EditorFace"), theApp.editorFontFace);
	auto editorFontTransferBuffer = theApp.GetEditorFontTransferBuffer();
	theApp.WriteProfileBinary(_T("Font"), _T("Editor"), (LPBYTE)editorFontTransferBuffer, sizeof(*editorFontTransferBuffer));

	auto transFacFormatTransferBuffer = theApp.GetTransFacFormatTransferBuffer();
	theApp.WriteProfileBinary(_T("Format"), _T("TransFac"), (LPBYTE)transFacFormatTransferBuffer, sizeof(*transFacFormatTransferBuffer));

	theApp.WriteProfileInt(_T("Options"), _T("TransFac"), theApp.TransFacOptions);

	theApp.WriteProfileInt(_T("Options"), _T("Hydrophobicity"), theApp.hydrophobicityOptions);
	theApp.WriteProfileInt(_T("Options"), _T("Hydrophilicity"), theApp.hydrophilicityOptions);
	theApp.WriteProfileInt(_T("Options"), _T("Antigenicity"), theApp.antigenicityOptions);

	theApp.WriteProfileString(_T("Options"), _T("HydrophilicResidues"), theApp.hydrophilicresidues);
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	theApp.WriteProfileBinary(_T("Options"), _T("Align"), (LPBYTE)alignOptionsBuffer, sizeof(*alignOptionsBuffer));

	auto formatTransferBuffer = theApp.GetFormatTransferBuffer();
	theApp.WriteProfileBinary(_T("Options"), _T("Format"), (LPBYTE)formatTransferBuffer, sizeof(*formatTransferBuffer));

	auto enzymeOptionsTransferBuffer = theApp.GetEnzymeOptionsTransferBuffer();
	theApp.WriteProfileBinary(_T("Options"), _T("Enzyme"), (LPBYTE)enzymeOptionsTransferBuffer, sizeof(*enzymeOptionsTransferBuffer));

	theApp.WriteProfileInt(_T("Options"), _T("UndoLevels"), theApp.undoLevels);
	theApp.WriteProfileInt(_T("Window"), _T("ConversionOutput"), theApp.ConversionOutputTransferBuffer);

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	theApp.WriteProfileBinary(_T("Window"), _T("Position"), (LPBYTE)&wp, sizeof(wp));

	if (theApp.IsWin7())
		m_pTaskbarList.Release();
	return CMDIFrameWnd::DestroyWindow();
}

// CMainFrame message handlers

void CMainFrame::OnMove(int x, int y)
{
	CMDIFrameWnd::OnMove(x, y);

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

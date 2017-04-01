
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "DNAssist.h"
#include "DNAssistDoc.h"
#include "DrewPlotView.h"
#include "DNAssistView.h"

#include "ChildFrm.h"
#include "AlignDialog.h"
#include "EditorOptionsPropertySheet.h"
#include "AnalysisOptionsPropertySheet.h"
#include "ResultDisplayOptionsPropertySheet.h"
#include "AlignOptionsPropertySheet.h"
#include "UPDialog.h"

#include "model/AnalyzeDNAList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)	
	ON_COMMAND(ID_OPTIONS_TRANSCRIPTIONFACTORS, OnOptionsTranscriptionfactors)	
	ON_COMMAND(ID_OPTIONS_EDITOR, OnOptionsEditor)
	ON_COMMAND(ID_OPTIONS_ANALYSIS, OnOptionsAnalysis)
	ON_COMMAND(ID_OPTIONS_RESULTDISPLAY, OnOptionsResultdisplay)
	ON_COMMAND(ID_OPTIONS_ALIGNMENT, OnOptionsAlignment)
	ON_COMMAND(ID_PROPERTIES_HYDROPHOBICITY, OnPropertiesHydrophobicity)
	ON_COMMAND(ID_PROPERTIES_HYDROPHILICITY, OnPropertiesHydrophilicity)
	ON_COMMAND(ID_PROPERTIES_ANTIGENICITY, OnPropertiesAntigenicity)
	ON_COMMAND(ID_TRANSLATE_1PHASE, OnTranslate1phase)
	ON_COMMAND(ID_TRANSLATE_3PHASE, OnTranslate3phase)
	ON_COMMAND(ID_TRANSLATE_6PHASE, OnTranslate6phase)
	ON_COMMAND(ID_ANALYZE_ALIGN, OnAnalyzeAlign)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_ALIGN, OnUpdateAnalyzeAlign)
	ON_WM_MOVE()	
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style |= WS_MAXIMIZE | WS_VISIBLE;// maximize
	if(!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);

	// synchronise with views
	AfxGetMainWnd()->SendMessage(WM_SIZEPARENT);
}

void CChildFrame::OnOptionsTranscriptionfactors()
{
	ShellExecute(NULL, _T("open"), _T("http://sqlitebrowser.org"), NULL, NULL, SW_SHOWNORMAL);
}

void CChildFrame::TranslateNucleicAcidToProtein(short phase)
{
	CView* pView = GetActiveView();
	if (!pView->IsKindOf(RUNTIME_CLASS(CDNAssistView)))
		return;

	CDNAssistView* pSeqView = dynamic_cast<CDNAssistView*>(pView);
	CDNAssistDoc* pDoc = pSeqView->GetDocument();	

	CResultDoc* pResultDoc = theApp.OnNewResultDoc();

	// HCURSOR old_cursor = SetCursor(LoadCursor(0, IDC_WAIT));
	CString childtitle;
	childtitle.Format(_T("Translation - %s"), pDoc->GetTitle());

	CString tempTitle;
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	int translationId = theApp.GetTranslationId();
	if (translationId > 0)
		tempTitle.Format(_T("%s [%d)"), childtitle, translationId);
	else
		tempTitle.Format(_T("%s"), childtitle);

	string data = *pDoc->GetData();
	if (pSeqView->IsSelection()) {
		string dataSegment;
		pSeqView->GetSelectedSeq(dataSegment);
		pResultDoc->SetData(dataSegment, pSeqView->GetSelStart(), dataSegment.length(), DF_TRANSLATE, EL_GRAPHICUNIQUE, NULL,
			'D', 'L', pDoc->GetCodonTable(), phase);
	}
	else
		pResultDoc->SetData(data, 1, data.length(), DF_TRANSLATE, EL_GRAPHICUNIQUE, NULL,
			'D', 'L', pDoc->GetCodonTable(), phase);

	pResultDoc->SetTitle(tempTitle);
	// ::SetCursor(old_cursor);
}

void CChildFrame::OnTranslate1phase()
{
	TranslateNucleicAcidToProtein(1);
}

void CChildFrame::OnTranslate3phase()
{
	TranslateNucleicAcidToProtein(3);
}

void CChildFrame::OnTranslate6phase()
{
	TranslateNucleicAcidToProtein(6);
}

void CChildFrame::OnOptionsEditor()
{
	CEditorOptionsPropertySheet editorOptionsPropertySheet(_T("Editor Options"), this, 0);
	editorOptionsPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	editorOptionsPropertySheet.DoModal();
}

void CChildFrame::OnOptionsAnalysis()
{
	CAnalysisOptionsPropertySheet analysisOptionsPropertySheet(_T("Analysis Options"), this, 0);
	analysisOptionsPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	analysisOptionsPropertySheet.DoModal();
}

void CChildFrame::OnOptionsResultdisplay()
{
	CResultDisplayOptionsPropertySheet resultDisplayOptionsPropertySheet(_T("Result Display Options"), this, 0);
	resultDisplayOptionsPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	resultDisplayOptionsPropertySheet.DoModal();
}

void CChildFrame::OnOptionsAlignment()
{
	CAlignOptionsPropertySheet alignOptionsPropertySheet(_T("Alignment Options"), this, 0);
	alignOptionsPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	alignOptionsPropertySheet.DoModal();
}

// CChildFrame message handlers

void CChildFrame::OnPropertiesHydrophobicity()
{
	CView* pView = GetActiveView();
	if (!pView->IsKindOf(RUNTIME_CLASS(CDNAssistView)))
		return;

	CDNAssistView* pSeqView = dynamic_cast<CDNAssistView*>(pView);
	CDNAssistDoc* pDoc = pSeqView->GetDocument();
	string dataSegment = *pDoc->GetData();

	CString childtitle;
	childtitle.Format(_T("Hydrophobicity Profile - %s"), pDoc->GetTitle());
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	size_t start_base = pSeqView->GetSelStart() + (theApp.hydrophobicityOptions - 1) / 2;
	if(pSeqView->IsSelection()) {		
		pSeqView->GetSelectedSeq(dataSegment);
		tempTitle.Format(_T("%s [%d]"), childtitle, start_base);
	}
	else
		tempTitle = childtitle;	

	BeginWaitCursor();
	CDrewPlotDoc* pDrewPlotDoc = theApp.OnNewDrewPlot();
	pDrewPlotDoc->Xaxistitle = _T("Residue Number");
	pDrewPlotDoc->Yaxistitle = _T("Hydrophobicity");

	pDrewPlotDoc->SetTitle(tempTitle);

	pDrewPlotDoc->m_nStartBase = start_base;
	pDrewPlotDoc->CalculateHydrophobicity(dataSegment, 0);
	EndWaitCursor();
}

void CChildFrame::OnPropertiesHydrophilicity()
{
	CView* pView = GetActiveView();
	if (!pView->IsKindOf(RUNTIME_CLASS(CDNAssistView)))
		return;

	CDNAssistView* pSeqView = dynamic_cast<CDNAssistView*>(pView);
	CDNAssistDoc* pDoc = pSeqView->GetDocument();
	string dataSegment = *pDoc->GetData();
	CString childtitle;
	childtitle.Format(_T("Hydrophilicity Profile - %s"), pDoc->GetTitle());
	int iSlash = childtitle.ReverseFind(_T('.'));
	if (iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	size_t start_base = pSeqView->GetSelStart() + (theApp.hydrophobicityOptions - 1) / 2;
	if (pSeqView->IsSelection()) {
		pSeqView->GetSelectedSeq(dataSegment);
		tempTitle.Format(_T("%s [%d]"), childtitle, start_base);
	}
	else
		tempTitle = childtitle;

	BeginWaitCursor();
	CDrewPlotDoc* pDrewPlotDoc = theApp.OnNewDrewPlot();
	pDrewPlotDoc->Xaxistitle = _T("Residue Number");
	pDrewPlotDoc->Yaxistitle = _T("Hydrophilicity");

	pDrewPlotDoc->SetTitle(tempTitle);

	pDrewPlotDoc->m_nStartBase = start_base;
	pDrewPlotDoc->CalculateHydrophilicity(dataSegment, 0);
	EndWaitCursor();
}

void CChildFrame::OnPropertiesAntigenicity()
{
	CView* pView = GetActiveView();
	if (!pView->IsKindOf(RUNTIME_CLASS(CDNAssistView)))
		return;

	CDNAssistView* pSeqView = dynamic_cast<CDNAssistView*>(pView);
	CDNAssistDoc* pDoc = pSeqView->GetDocument();
	string dataSegment = *pDoc->GetData();

	CString childtitle;
	childtitle.Format(_T("Antigenicity Profile - %s"), pDoc->GetTitle());
	int iSlash = childtitle.ReverseFind(_T('.'));
	if(iSlash > -1)
		childtitle = childtitle.Left(iSlash);
	CString tempTitle;

	size_t start_base = pSeqView->GetSelStart() + (theApp.hydrophobicityOptions - 1) / 2;
	if (pSeqView->IsSelection()) {
		pSeqView->GetSelectedSeq(dataSegment);
		tempTitle.Format(_T("%s [%d]"), childtitle, start_base);
	}
	else
		tempTitle = childtitle;

	BeginWaitCursor();
	CDrewPlotDoc* pDrewPlotDoc = theApp.OnNewDrewPlot();
	pDrewPlotDoc->Xaxistitle = _T("Residue Number");
	pDrewPlotDoc->Yaxistitle = _T("Antigenicity");

	pDrewPlotDoc->SetTitle(tempTitle);

	pDrewPlotDoc->m_nStartBase = start_base;
	pDrewPlotDoc->CalculateAntigenicity(dataSegment, 0);
	EndWaitCursor();
}

bool CalculateAlignmentsProc(const CUPDUPDATA* pCUPDUPData)
{
	//Retrieve the App Supplied Data
	auto pnCount = (int*) pCUPDUPData->GetAppData();

	while (!pCUPDUPData->ShouldTerminate() && *pnCount <= 100) {
		pCUPDUPData->SetProgress(pCUPDUPData->GetStatus(), *pnCount); //Update Progress Bar
		Sleep(800);
	}

	pCUPDUPData->SetProgress(100);
	return true;
}

void CChildFrame::OnAnalyzeAlign()
{
	CAlignDialog alignDialog;
	if(alignDialog.DoModal() != IDOK)
		return;

	vector<string> seqList;
	alignDialog.GetSequenceList(seqList);
	if(seqList.empty())
		return;

	CAlignDoc* pAlignDoc = theApp.OnNewAlignDoc();
	CString tempTitle;
	int alignmentId = theApp.GetAlignmentId();
	if (alignmentId > 0)
		tempTitle.Format(_T("%s [%d]"), _T("Alignment"), alignmentId);
	else
		tempTitle = _T("Alignment");
	pAlignDoc->SetTitle(tempTitle);

	int nPerc = 0;
	m_caption = L"Calculating..";
	m_caption.resize(256, '\0');
	CUPDialog waitDlg(AfxGetMainWnd()->GetSafeHwnd(), CalculateAlignmentsProc, &nPerc, &m_caption, false);
	pAlignDoc->CalculateAlignments(alignDialog.GetSequenceType(), &nPerc, &m_caption, alignDialog.GetNameList(), seqList);
	waitDlg.DoModal();
}

void CChildFrame::OnUpdateAnalyzeAlign(CCmdUI *pCmdUI)
{
	int nCount = 0;
	// loop through application's document templates
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (NULL != posDocTemplate) {
		CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (NULL != posDoc) {
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
			if (pDoc->IsKindOf(RUNTIME_CLASS(CDNAssistDoc)))
				++nCount;
			if (nCount > 1) {
				pCmdUI->Enable(TRUE);
				return;
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CChildFrame::OnMove(int x, int y)
{
	CMDIChildWnd::OnMove(x, y);

	AfxGetMainWnd()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG




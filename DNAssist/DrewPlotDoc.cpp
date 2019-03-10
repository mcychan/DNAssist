
// HydroDoc.cpp : implementation of the CHydroDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "DrewPlotDoc.h"
#include "DrewPlotView.h"

#include "model/Antigenicity.h"
#include "model/Hydrophilicity.h"
#include "model/Hydrophobicity.h"

#include <algorithm>
#include <fstream>      // std::ofstream
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHydroDoc

IMPLEMENT_DYNCREATE(CDrewPlotDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrewPlotDoc, CDocument)
END_MESSAGE_MAP()


// CHydroDoc construction/destruction

CDrewPlotDoc::CDrewPlotDoc()
{
	m_pThread = NULL;
	DeleteContents();
}

CDrewPlotDoc::~CDrewPlotDoc()
{
}

void CDrewPlotDoc::DeleteContents()
{
	Xaxistitle = _T("X-axis");
	Yaxistitle = _T("Y-axis");
	m_nLength = -1;
	resultarray.clear();
	Stop();
}

BOOL CDrewPlotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CDrewPlotDoc::OnOpenDocument(LPCTSTR pszPathName)
{		
	return FALSE;
}

BOOL CDrewPlotDoc::DoFileSave()
{
	CString pathName = GetPathName();
	if (!PathFileExists(pathName)) {
		return DoSave(pathName, TRUE);
	}

	return OnSaveDocument(pathName);
}

BOOL CDrewPlotDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CFileDialog dlgFile(FALSE, _T("emf"), NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
	_tcsncpy_s(dlgFile.m_ofn.lpstrFile, MAX_PATH, GetTitle(), MAX_PATH);
	dlgFile.m_ofn.lpstrFilter = _T("Enhanced Metafile (*.emf)\0*.emf\0Text file (*.txt)\0*.txt\0\0");
	dlgFile.m_ofn.nFilterIndex = 0;

	if (IDOK == dlgFile.DoModal()) {
		CString pathName = dlgFile.GetPathName();
		if (OnSaveDocument(pathName)) {
			// reset the title and change the document name
			if (bReplace)
				SetPathName(pathName, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDrewPlotDoc::OnSaveDocument(LPCTSTR pszPathName)
{
	CString pathName = pszPathName;
	CString fileExtension = pathName.Mid(pathName.ReverseFind(_T('.')) + 1, pathName.GetLength());
	if(fileExtension == _T("emf")) {
		POSITION pos = GetFirstViewPosition();
		if (pos == NULL)
			return FALSE;

		CDrewPlotView* pView = (CDrewPlotView*) GetNextView(pos);
		pView->SaveAsEnhancedMetaFile(pszPathName);
		SetModifiedFlag(FALSE);
		return TRUE;
	}

	ofstream outfile(pszPathName);
	for(int i = 0; i < resultarray.size(); ++i)
		outfile << resultarray[i] << '\n';
	outfile.close();
	SetModifiedFlag(FALSE);
	return TRUE;
}

// CHydroDoc serialization

void CDrewPlotDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void CDrewPlotDoc::SetModifiedFlag(BOOL bModified)
{
	CDocument::SetModifiedFlag(FALSE);
	CString title = GetTitle();
	title.Replace(_T(" *"), _T(""));
	SetTitle(title);
}

BOOL CDrewPlotDoc::CalculateAntigenicity(const string& dataStr, int plotType)
{
	m_plotType = plotType;
	CAntigenicity antigenicity(dataStr, theApp.antigenicityOptions);
	return antigenicity.CalculateAntigenicity(resultarray);
}

BOOL CDrewPlotDoc::CalculateHydrophobicity(const string& dataStr, int plotType)
{
	m_plotType = plotType;
	CHydrophobicity hydrophobicity(dataStr, theApp.hydrophobicityOptions);
	return hydrophobicity.CalculateHydrophobicity(resultarray);	
}

BOOL CDrewPlotDoc::CalculateHydrophilicity(const string& dataStr, int plotType)
{
	m_plotType = plotType;
	CHydrophilicity hydrophilicity(dataStr, theApp.hydrophilicityOptions);
	return hydrophilicity.CalculateHydrophilicity(resultarray);
}

void CDrewPlotDoc::Stop()
{
	if (m_pThread)
	{
		AfxEndThread(0);
		m_bStop = true;
		::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		m_pThread->m_hThread = NULL;
		m_pThread = NULL;
	}
}

/* static */
UINT CDrewPlotDoc::ThreadProc(LPVOID pParam)
{
	CDrewPlotDoc* pThis = (CDrewPlotDoc*) pParam;

	pThis->Calculate();
	pThis->m_pThread = NULL;
	pThis->m_bStop = false;
	//theApp.ProgressComplete();
	return 0;
}

// called by ThreadProc
BOOL CDrewPlotDoc::Calculate()
{
	m_bStop = false;
	if (m_pDrewCalladine->CalculatePositioningSignal()) {
		m_nLength = m_pDrewCalladine->GetNumberOfValues();
		m_nStartBase = m_pDrewCalladine->GetDyadStart();
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL) {
			CView* pView = GetNextView(pos);
			pView->PostMessage(WM_PAINT);
		}
		return TRUE;
	}
	return FALSE;
}

void CDrewPlotDoc::CalculateNucleosome(const string& dataStr, int* pPercentCompleted, int selStart)
{
	m_pDrewCalladine = make_unique<CDrewCalladine>(dataStr, &resultarray, pPercentCompleted, selStart);
	m_pThread = ::AfxBeginThread(ThreadProc, this);
}

// CHydroDoc commands

// CHydroDoc diagnostics

#ifdef _DEBUG
void CDrewPlotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDrewPlotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ResultDoc.cpp : implementation of the CResultDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "ResultDoc.h"
#include "ResultView.h"

#include "model/DNASearch.h"

#include <algorithm>
#include <fstream>      // std::ofstream
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CResultDoc

IMPLEMENT_DYNCREATE(CResultDoc, CDocument)

BEGIN_MESSAGE_MAP(CResultDoc, CDocument)
END_MESSAGE_MAP()


// CResultDoc construction/destruction

CResultDoc::CResultDoc()
{
	DeleteContents();
}

CResultDoc::~CResultDoc()
{
}

void CResultDoc::DeleteContents()
{
	result = NULL;
}

BOOL CResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CResultDoc::OnOpenDocument(LPCTSTR pszPathName)
{		
	return FALSE;
}

BOOL CResultDoc::DoFileSave()
{
	CString pathName = GetPathName();
	if (!PathFileExists(pathName)) {
		return DoSave(pathName, TRUE);
	}

	return OnSaveDocument(pathName);
}

BOOL CResultDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CFileDialog dlgFile(FALSE, (displayflags & DF_GRAPHIC) ? _T("emf") : _T("rtf"), NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
	_tcsncpy_s(dlgFile.m_ofn.lpstrFile, MAX_PATH, GetTitle(), MAX_PATH);
	if (displayflags == DF_GRAPHIC)
		dlgFile.m_ofn.lpstrFilter = _T("Enhanced Metafile (*.emf)\0\0");
	else if (displayflags & DF_GRAPHIC)
		dlgFile.m_ofn.lpstrFilter = _T("Enhanced Metafile (*.emf)\0*.emf\0Rich Text Format (RTF) (*.rtf)\0*.rtf\0Text file (*.txt)\0*.txt\0\0");
	else
		dlgFile.m_ofn.lpstrFilter = _T("Rich Text Format (RTF) (*.rtf)\0*.rtf\0Text file (*.txt)\0*.txt\0\0");
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

BOOL CResultDoc::OnSaveDocument(LPCTSTR pszPathName)
{
	CString pathName = pszPathName;
	CString fileExtension = pathName.Mid(pathName.ReverseFind(_T('.')) + 1, pathName.GetLength());
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
		return FALSE;

	CResultView* pView = (CResultView*) GetNextView(pos);
	if (fileExtension == _T("emf")) {
		pView->SaveAsEnhancedMetaFile(pszPathName);
		SetModifiedFlag(FALSE);
		return TRUE;
	}
	if(fileExtension == _T("rtf")) {
		ofstream outfile((LPCTSTR) pathName);
		outfile << pView->GetRichTextFormat();
		outfile.close();
		SetModifiedFlag(FALSE);
		return TRUE;
	}

	ofstream outfile(pszPathName);
	outfile << pView->GetTextFormat();
	outfile.close();
	SetModifiedFlag(FALSE);
	return TRUE;
}

// CResultDoc serialization

void CResultDoc::Serialize(CArchive& ar)
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

void CResultDoc::SetModifiedFlag(BOOL bModified)
{
	CDocument::SetModifiedFlag(FALSE);
	CString title = GetTitle();
	title.Replace(_T(" *"), _T(""));
	SetTitle(title);
}

int CResultDoc::GetNumberNoCutters()
{
	// The enzymes must be sorted by cutfrequency, and less-frequent cutters at the
	//	top of the list
	if (result->size() == 0)
		return 0;

	int x = 0;
	while (x < result->size()) {
		auto rc = result->GetPointer(sortedindexes[x]);
		if (!rc->GetNumberCuts())
			x++;
		else
			return x;
	}
	return x;
}

size_t CResultDoc::GetFirstUniqueCutter()
{
	if (result->size() == 0)
		return 0;

	int x = 0;
	size_t members = result->size();
	while (x < members) {
		if (result->GetPointer(sortedindexes[x])->GetNumberCuts() != 1)
			x++;
		else
			return x;
	}
	return members;
}

int CResultDoc::GetNumberUniqueCutters()
{
	int x = GetFirstUniqueCutter();
	if (x == result->size())
		return 0;
	int y = x;
	while (x < result->size()) {
		if (result->GetPointer(sortedindexes[x])->GetNumberCuts() == 1)
			x++;
		else
			return x - y;
	}
	return x - y;
}

bool CResultDoc::RestrictionMapping(const string& seq, char sequence_type, char seqForm, CEnzymeList* enzymelist, CRestrictionCutList* resultarray)
{
	int flags = theApp.GetEnzymeAttributeOptions();
	CDNASearch searchobject(seq, seq.length(), sequence_type, seqForm);
	for (int x = 0; x < enzymelist->GetNumberOfEnzymes(); x++) {
		int enzymeattr = enzymelist->GetAttribute(x);
		if (flags & enzymeattr) {
			vector<int> intarray;
			string site;
			enzymelist->GetSite(x, site);
			searchobject.FuzzySearchMultiple(site, enzymelist->GetSiteLength(x), intarray);
			intarray.pop_back();//Delete last entry, which is always the length of the tested sequence
			if ((enzymeattr & EL_PALINDROME) != EL_PALINDROME) {
				vector<int> temparray;
				string siteComplement;
				enzymelist->GetSiteComplement(x, siteComplement);
				searchobject.FuzzySearchMultiple(siteComplement, enzymelist->GetSiteLength(x), temparray);
				temparray.pop_back();//Delete last entry, which is always the length of the tested sequence
				intarray.insert(intarray.end(), temparray.cbegin(), temparray.cend());
				sort(intarray.begin(), intarray.end());
			}

			for (int y = 0; y < intarray.size(); ++y)
				intarray[y]++;//Add 1 to every RE cut position, since it starts at 0 and should start at 1

							  //	Make a new CRestrictionEnzymeCut object from the enzymelist entry, and
							  //	add it to the CRestrictionCutList
			string name, formattedsite;
			enzymelist->GetName(x, name);
			enzymelist->GetSiteFormatted(x, formattedsite);
			CRestrictionEnzymeCut rec(name, site, formattedsite, enzymeattr, intarray, seqForm, seq.length(), enzymelist->IsSelected(x));
			resultarray->AddEntry(rec);
		}
	}
	resultarray->MakeSequentialArray();
	return true;
}

bool CResultDoc::TranscriptionFactors(const string& seq, char sequence_type, char seqForm, CEnzymeList* factorlist, CRestrictionCutList* resultarray)
{
	int flags = theApp.GetEnzymeAttributeOptions();
	CDNASearch searchobject(seq, seq.length(), sequence_type, seqForm);
	for (int x = 0; x < factorlist->GetNumberOfEnzymes(); x++) {
		vector<int> intarray;
		string site;
		factorlist->GetSite(x, site);
		string siteComplement;
		factorlist->GetSiteComplement(x, siteComplement);
		searchobject.FuzzySearchMultiple(site, factorlist->GetSiteLength(x), intarray);
		intarray.pop_back();//Delete last entry, which is always the length of the tested sequence
		if (!factorlist->IsSymmetric(site)) {
			vector<int> temparray;
			searchobject.FuzzySearchMultiple(siteComplement, factorlist->GetSiteLength(x), temparray);
			temparray.pop_back();//Delete last entry, which is always the length of the tested sequence
			intarray.insert(intarray.end(), temparray.cbegin(), temparray.cend());
			sort(intarray.begin(), intarray.end());
		}

		for (int y = 0; y < intarray.size(); ++y)
			intarray[y]++;//Add 1 to every RE cut position, since it starts at 0 and should start at 1

						  //	Make a new CRestrictionEnzymeCut object from the factorlist entry, and
						  //	add it to the CRestrictionCutList
		string name;
		factorlist->GetName(x, name);
		CRestrictionEnzymeCut rec(name, site, siteComplement, 0, intarray, seqForm, seq.length(), true);
		resultarray->AddEntry(rec);
	}
	resultarray->MakeSequentialArray();
	return true;
}

void CResultDoc::SetData(const string& dataStr,
	int sel_start, int sel_length,
	short dispflags, int enzymeflags,
	CRestrictionCutList* pResult,
	char seq_type, char seq_form,
	CCodonTable* pCodon,
	int translationphase)
{
	sequence = dataStr;
	selectionstart = sel_start;
	selectionlength = sel_length;
	displayflags = dispflags;
	analysisflags = enzymeflags;
	result = pResult;
	m_seqType = seq_type;
	m_form = seq_form;
	codontable = pCodon;
	phase = translationphase;

	// Sort enzymelist from least cutters to most cutters
	if(result)
		result->SortEnzymesByCutFrequency(sortedindexes);
	UpdateAllViews(NULL);
}

CAnalyzeDNAList* CResultDoc::GetAnalyzeDNAList(const CPoint& chardim, int characters_per_row, int lines_per_page, vector<int>& pagebreaks)
{
	auto printFormatTransferBuffer = theApp.GetPrintFormatTransferBuffer();
	//	If this is for a nucleic acid translation, do only that, then return
	if (displayflags & DF_TRANSLATE) {
		analyzednalist = make_unique<CAnalyzeDNAList>(sequence, selectionstart, selectionlength, nullptr,
			printFormatTransferBuffer, chardim.x, chardim.y, characters_per_row,
			lines_per_page, pagebreaks,
			m_seqType, codontable);
	}

	if (displayflags & DF_LIST) {
		analyzednalist = make_unique<CAnalyzeDNAList>(sequence, selectionstart, selectionlength, result,
			printFormatTransferBuffer, chardim.x, chardim.y, characters_per_row,
			lines_per_page, pagebreaks, m_seqType);
	}

	return analyzednalist.get();
}

// CResultDoc commands

// CResultDoc diagnostics

#ifdef _DEBUG
void CResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

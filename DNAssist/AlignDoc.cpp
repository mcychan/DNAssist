
// AlignDoc.cpp : implementation of the CAlignDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "AlignDoc.h"
#include "AlignView.h"

#include <algorithm>
#include <fstream>      // std::ofstream
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAlignDoc

IMPLEMENT_DYNCREATE(CAlignDoc, CDocument)

BEGIN_MESSAGE_MAP(CAlignDoc, CDocument)
END_MESSAGE_MAP()


// CAlignDoc construction/destruction

CAlignDoc::CAlignDoc()
{
	m_pThread = NULL;
	DeleteContents();
}

CAlignDoc::~CAlignDoc()
{
}

void CAlignDoc::DeleteContents()
{
	m_seqType = 0;
	Stop();
}

BOOL CAlignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CAlignDoc::OnOpenDocument(LPCTSTR pszPathName)
{		
	return FALSE;
}

BOOL CAlignDoc::DoFileSave()
{
	CString pathName = GetPathName();
	if (!PathFileExists(pathName)) {
		return DoSave(pathName, TRUE);
	}

	return OnSaveDocument(pathName);
}

BOOL CAlignDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CFileDialog dlgFile(FALSE, _T("rtf"), NULL, OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, NULL, AfxGetMainWnd());
	_tcsncpy_s(dlgFile.m_ofn.lpstrFile, MAX_PATH, GetTitle(), MAX_PATH);
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

BOOL CAlignDoc::OnSaveDocument(LPCTSTR pszPathName)
{
	CString pathName = pszPathName;
	CString fileExtension = pathName.Mid(pathName.ReverseFind(_T('.')) + 1, pathName.GetLength());
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
		return FALSE;

	CAlignView* pView = (CAlignView*) GetNextView(pos);
	if(fileExtension == _T("rtf")) {
		ofstream outfile((LPCTSTR) pathName);
		outfile << pView->GetRTFText();
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

// CAlignDoc serialization

void CAlignDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAlignDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAlignDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAlignDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

void CAlignDoc::SetModifiedFlag(BOOL bModified)
{
	CDocument::SetModifiedFlag(FALSE);
	CString title = GetTitle();
	title.Replace(_T(" *"), _T(""));
	SetTitle(title);
}

void CAlignDoc::Initialise()
{
	auto alignOptionsBuffer = theApp.GetAlignOptionsBuffer();
	m_SeqAlignment.gap_open = (float)alignOptionsBuffer->gapopeningpenalty;
	m_SeqAlignment.gap_extend = alignOptionsBuffer->gapextensionpenalty;
	m_SeqAlignment.divergence_cutoff = alignOptionsBuffer->delaydivergentsequences;
	m_SeqAlignment.transition_weight = alignOptionsBuffer->dnatransitionweight;
	m_SeqAlignment.matnum = alignOptionsBuffer->proteinweightmatrix;
	m_SeqAlignment.dnamatnum = alignOptionsBuffer->dnaweightmatrix;
	m_SeqAlignment.no_pref_penalties = (bool)alignOptionsBuffer->residuespecificpenalty;
	m_SeqAlignment.no_hyd_penalties = (bool)alignOptionsBuffer->hydrophilicpenalty;
	m_SeqAlignment.hyd_residues = CStringA(theApp.hydrophilicresidues);
	m_SeqAlignment.gap_dist = alignOptionsBuffer->gapseparationdistance;
	m_SeqAlignment.use_endgaps = (bool)alignOptionsBuffer->endgapseparation;
	m_SeqAlignment.pw_dnamtrxname = alignOptionsBuffer->dnaweightmatrix == 0 ? "iub" : "clustalw";
	m_SeqAlignment.mtrxname = GetMtrxname(alignOptionsBuffer->proteinweightmatrix);
}

/* static */
UINT CAlignDoc::ThreadProc(LPVOID pParam)
{
	CAlignDoc* pThis = (CAlignDoc*) pParam;

	pThis->Calculate();
	pThis->m_pThread = NULL;
	pThis->m_bStop = false;
	return 0;
}

// called by ThreadProc
BOOL CAlignDoc::Calculate()
{
	m_bStop = false;	

	if (!m_SeqAlignment.main_menu()) {// The alignments start there
		m_SeqAlignment.hpsequences.clear();
		return FALSE;
	}

	numberofsequences = GetNumberOfSequences();
	alignmentlength = GetLengthOfSequence();

	aminoacidmatrix.resize(alignmentlength);
	nucleicacidmatrix.resize(alignmentlength);
	for (int i = 0; i<alignmentlength; ++i) {
		vector<int> row(5);
		nucleicacidmatrix[i] = row;

		vector<int> row2(21);
		aminoacidmatrix[i] = row2;
	}	
	
	RemoveLeadingAndTrailingHyphens();
	IdentifyIdenticalAndConservatives();

	*(m_SeqAlignment.pPercent) = 101;
	
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		pView->PostMessage(WM_PAINT);
	}
	return m_SeqAlignment.numberofseqs;
}

void CAlignDoc::Stop()
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

void CAlignDoc::CalculateAlignments(char seqType, int* pPercent, wstring* pCaption, const vector<CString>* pNameList, vector<string>& seqList)
{	
	Initialise();
	m_seqType = seqType;
	m_SeqAlignment.pPercent = pPercent;
	m_SeqAlignment.pCaption = pCaption;
	m_SeqAlignment.numberofseqs = seqList.size();

	m_SeqAlignment.hpsequences.clear();
	m_SeqAlignment.hpsequences.insert(m_SeqAlignment.hpsequences.end(), seqList.cbegin(), seqList.cend());
	if (m_seqType == 'D' || m_seqType == 'G' || m_seqType == 'R')
		m_SeqAlignment.dnaflag = 1;
	else
		m_SeqAlignment.dnaflag = 0;
	m_sequencenames.insert(m_sequencenames.begin(), pNameList->cbegin(), pNameList->cend());

	m_pThread = ::AfxBeginThread(ThreadProc, this);
}

void CAlignDoc::RemoveLeadingAndTrailingHyphens()
{
	for(int x = 0; x < numberofsequences; x++) {
		string* temp = GetAlignment(x);
		if (temp == nullptr)
			return;
		int length = temp->length();
		int y = 0;
		while(y < length && temp->at(y) == '-')
			temp->at(y++) = ' ';
		int z = length - 1;
		while(z >=0 && temp->at(z) == '-')
			temp->at(z--) = ' ';
	}
}

void CAlignDoc::IdentifyIdenticalAndConservatives()
{
	if (m_seqType == 'D' || m_seqType == 'R' || m_seqType == 'G') {
		wstring caption = L"Identifying nucleotide similarities";
		m_SeqAlignment.copy_chars(caption), *(m_SeqAlignment.pPercent) = 0;
	}
	else {
		wstring caption = L"Identifying amino acid similarities";
		m_SeqAlignment.copy_chars(caption), *(m_SeqAlignment.pPercent) = 0;
	}

	backgroundcolorarray.resize(numberofsequences);
	for (int x = 0; x < numberofsequences; x++) {
		vector<short> row(alignmentlength);
		backgroundcolorarray[x] = row;
	}

	for(int y = 0; y < alignmentlength; y++) {
		*(m_SeqAlignment.pPercent) = y * 100 / alignmentlength;
		if(m_seqType == 'D' || m_seqType == 'R'  || m_seqType == 'G')
			IdentifyIdenticalLetters(nucleicacidmatrix, y);
		else if(m_seqType == 'P') {
			IdentifyIdenticalLetters(aminoacidmatrix, y);
			IdentifyConservedLetters(y);
		}
	}
}

int CAlignDoc::NumberOfEntriesInColumn(const vector<vector<int> >& array, int column)
{
	int result = 0;
	if (column >= array.size())
		return result;

	size_t numberofrows = array.size();
	for(int x = 0; x < numberofrows; x++) {
		if(array[x][column])
			result++;
	}
	return result;
}

int CAlignDoc::GetColumnWithMostEntries(const vector<vector<int> >& array)
{
	int max = 0, number = 0;
	if(array.empty())
		return number;

	size_t numberofcolumns = array[0].size();
	for(int x = 0; x < numberofcolumns; x++) {
		int result = NumberOfEntriesInColumn(array, x);
		if(result > max) {
			max = result;
			number = x;
		}
	}
	return number;
}


void CAlignDoc::IdentifyConservedLetters(int index)
{
	conservedaminoacidmatrix.resize(numberofsequences);
	for(int x = 0; x < numberofsequences; x++) {
		vector<int> row(5);
		string* temp = GetAlignment(x);
		if(temp != nullptr)
			row[GetConservationGroup(temp->at(index))] = 1;
		conservedaminoacidmatrix[x] = row;		
	}

	int hitcolumn = GetColumnWithMostEntries(conservedaminoacidmatrix);
	int hits = NumberOfEntriesInColumn(conservedaminoacidmatrix, hitcolumn);
	if((100 * hits) / numberofsequences >= theApp.GetAlignOptionsBuffer()->percentageidentical) {
		for(int x = 0; x < numberofsequences; x++) {
			if(conservedaminoacidmatrix[x][hitcolumn] && !backgroundcolorarray[x][index])
				backgroundcolorarray[x][index] = 2;
		}
	}
}

void CAlignDoc::IdentifyIdenticalLetters(vector<vector<int> >& matrix, int index)
{
	for(int x = 0; x < numberofsequences; x++) {
		size_t numberofcolumns = matrix[x].size();
		for (int y = 0; y < numberofcolumns; ++y)
			matrix[x][y] = 0;

		string* temp = GetAlignment(x);
		if (temp == nullptr)
			break;
		char code = temp->at(index);
		if(m_seqType == 'D' || m_seqType == 'G')
			matrix[x][GetDNABaseIndex(code)] = 1;
		else if (m_seqType == 'R')
			matrix[x][GetRNABaseIndex(code)] = 1;
		else
			matrix[x][GetAminoAcidIndex(code)] = 1;
	}

	int hitcolumn = GetColumnWithMostEntries(matrix);
	int hits = NumberOfEntriesInColumn(matrix, hitcolumn);
	if((100 * hits) / numberofsequences >= theApp.GetAlignOptionsBuffer()->percentageidentical) {
		for(int x = 0; x < numberofsequences; x++) {
			if(matrix[x][hitcolumn]) {
				if((m_seqType == 'D' || m_seqType == 'G' || m_seqType == 'R') && (hitcolumn < 4))
					backgroundcolorarray[x][index] = 1;
				else if((m_seqType == 'P') && (hitcolumn < 20))
					backgroundcolorarray[x][index] = 1;
			}
		}
	}
}

int CAlignDoc::GetLongestNameString()
{
	int maxLength = 0;
	int number = m_sequencenames.size();

	for(int x = 0; x < number; x++) {
		CString* temp = GetSeqName(x);
		if (temp == nullptr)
			break;
		int length = temp->GetLength();
		maxLength = max(length, maxLength);
	}
	return maxLength;
}

int CAlignDoc::GetLongestPositionString()
{
	string* temp = GetAlignment(0);
	if (temp == nullptr)
		return 1;
	int length = temp->length();
	if(!length)
		return 1;
	return log10(length + 0.0f) + 1;
}

int CAlignDoc::GetDNABaseIndex(char letter)
{
	switch (letter) {
		case 'G' : return 0;
		case 'A' : return 1;
		case 'T' : return 2;
		case 'C' : return 3;
		default : return 4;
	}
}

int CAlignDoc::GetRNABaseIndex(char letter)
{
	switch (letter) {
		case 'G' : return 0;
		case 'A' : return 1;
		case 'U' : return 2;
		case 'C' : return 3;
		default : return 4;
	}
}

//	This function returns a number corresponding to one of the 4 groups
//	(non-polar, polar, acidic and basic) of conserved amino acids
int CAlignDoc::GetConservationGroup(char letter)
{
	switch (letter) {
		case 'G' :
		case 'A' :
		case 'V' :
		case 'I' :
		case 'L' :
		case 'F' :
		case 'P' :
		case 'M' :
			return 0;
		case 'S' :
		case 'T' :
		case 'Y' :
		case 'W' :
		case 'N' :
		case 'Q' :
		case 'C' :
			return 1;
		case 'D' :
		case 'E' :
			return 2;
		case 'K' :
		case 'R' :
		case 'H' :
			return 3;
		default :
			return 4;
   }
}

int CAlignDoc::GetAminoAcidIndex(char letter)
{
	switch (letter) {
		case 'G' : return 0;
		case 'A' : return 1;
		case 'V' : return 2;
		case 'I' : return 3;
		case 'L' : return 4;
		case 'F' : return 5;
		case 'P' : return 6;
		case 'M' : return 7;
		case 'S' : return 8;
		case 'T' : return 9;
		case 'Y' : return 10;
		case 'W' : return 11;
		case 'N' : return 12;
		case 'Q' : return 13;
		case 'C' : return 14;
		case 'D' : return 15;
		case 'E' : return 16;
		case 'K' : return 17;
		case 'R' : return 18;
		case 'H' : return 19;
		default : return 20;
	}
}

int CAlignDoc::GetLengthOfSequence()
{
	if(m_SeqAlignment.result.empty())
		return 0;
	return m_SeqAlignment.result[0].length();
}

int CAlignDoc::GetNumberOfSequences()
{
	if(m_SeqAlignment.result.empty())
		return 0;
	return m_SeqAlignment.result.size();
}

int CAlignDoc::GetSequenceNumber(const string& startofstring, int offset, int firstvalue)
{
	int sequencenumber = firstvalue;
	int start = 0, substringlength = 0;

	while(start < offset) {
		auto end = startofstring.find('-', start);
		if(end != string::npos && end <= offset) {
			substringlength = end - start;
			start = end + 1;
		}
		else {
			substringlength = offset - start;
			start = offset;
		}
		sequencenumber += substringlength;
	}
	return sequencenumber;
}

// CAlignDoc commands

// CAlignDoc diagnostics

#ifdef _DEBUG
void CAlignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAlignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

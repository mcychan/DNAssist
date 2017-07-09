
// DNAssistDoc.cpp : implementation of the CDNAssistDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "DNAssistDoc.h"
#include "SequenceTypeDialog.h"

#include <algorithm>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDNAssistDoc

IMPLEMENT_DYNCREATE(CDNAssistDoc, CDocument)

BEGIN_MESSAGE_MAP(CDNAssistDoc, CDocument)
	ON_COMMAND(ID_CHANGECASE_UPPERCASE, OnChangecaseUppercase)
	ON_COMMAND(ID_CHANGECASE_LOWERCASE, OnChangecaseLowercase)	
	ON_UPDATE_COMMAND_UI(ID_CONVERT_DNA2RNA, OnUpdateConvertDna2rna)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_RNA2DNA, OnUpdateConvertRna2dna)	
	ON_COMMAND(ID_CONVERT_CIRCULARIZE, OnConvertCircularize)
	ON_COMMAND(ID_CONVERT_LINEARIZE, OnConvertLinearize)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_CIRCULARIZE, OnUpdateConvertCircularize)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_LINEARIZE, OnUpdateConvertLinearize)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_DNA2PRO, OnUpdateConvertDna2pro)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_RNA2PRO, OnUpdateConvertRna2pro)
	ON_UPDATE_COMMAND_UI(ID_CONVERT_REVERSETRANSLATE, OnUpdateConvertReversetranslate)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATE_1PHASE, OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATE_3PHASE, OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(ID_TRANSLATE_6PHASE, OnUpdateTranslate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_FINDPATTERN, OnUpdateAnalyzeFindpattern)	
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_FINDORFS, OnUpdateAnalyzeFindorfs)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_PHYSICOCHEMICAL, OnUpdatePropertiesPhysicochemical)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_HYDROPHOBICITY, OnUpdatePropertiesHydrophobicity)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_HYDROPHILICITY, OnUpdatePropertiesHydrophilicity)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_ANTIGENICITY, OnUpdatePropertiesAntigenicity)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_LIST, OnUpdateAnalyzeList)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_RESTRICTIONMAPPING, OnUpdateAnalyzeRestrictionmapping)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_TRANSCRIPTIONFACTORS, OnUpdateAnalyzeTranscriptionfactors)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_NUCLEOSOMEPOSITIONS, OnUpdateAnalyzeNucleosomepositions)	
END_MESSAGE_MAP()


// CDNAssistDoc construction/destruction

CDNAssistDoc::CDNAssistDoc()
{
	m_pCodontable = make_unique<CCodonTable>((LPCTSTR)theApp.codonPath);
	DeleteContents();
}

CDNAssistDoc::~CDNAssistDoc()
{
}

void CDNAssistDoc::DeleteContents()
{
	m_seqType = 'D';
	m_form = 'L';
	m_dataStr.clear();	
}

BOOL CDNAssistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

BOOL CDNAssistDoc::OnOpenDocument(LPCTSTR pszPathName)
{	
	CFileDecoder fileDecoder(pszPathName);
	if (!fileDecoder.IsValid())
		return FALSE;
	
	SetSeqType(fileDecoder.GetSequence(m_dataStr, m_form, m_comment, theApp.GetApplicationName()));	
	if (GetSeqType() == 0) {
		CSequenceTypeDialog seqTypeDialog;
		seqTypeDialog.DoModal();
		SetSeqType(seqTypeDialog.GetSeqType());		
	}
	if (GetSeqType() != 0)
		m_fileTypeDetector.RemoveBadCharacters(m_dataStr, GetSeqType());
	
	return TRUE;
}

BOOL CDNAssistDoc::DoFileSave()
{
	CString pathName = GetPathName();
	if (!PathFileExists(pathName)) {
		return DoSave(pathName, TRUE);
	}

	return OnSaveDocument(pathName);
}

BOOL CDNAssistDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			int nIndex = newName.ReverseFind(_T('.'));
			if (nIndex > 0)
				newName = newName.Mid(0, nIndex);
			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) && !strExt.IsEmpty()) {
				ASSERT(strExt[0] == _T('.'));
				newName += strExt;
			}
		}
		else {
			int nIndex = newName.ReverseFind(_T('.'));
			if (nIndex > 0)
				newName = newName.Mid(0, nIndex);
		}

		if (!theApp.DoPromptFileName(newName,
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, NULL))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
				CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				e->Delete();     //modified
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

BOOL CDNAssistDoc::OnSaveDocument(LPCTSTR pszPathName)
{
	CFileEncoder fileEncoder(pszPathName);
	bool bResult = fileEncoder.WriteFile(m_dataStr, GetSeqType(), m_form, m_comment, theApp.GetApplicationName(), theApp.GetVersion());
	if(bResult)
		SetModifiedFlag(FALSE);
	return bResult;
}

void CDNAssistDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString title = lpszTitle;
	
	if (IsModified()) // add the '*' to indicate a modified document
		title += " *";

	CDocument::SetTitle(title);;
}

// CDNAssistDoc serialization

void CDNAssistDoc::Serialize(CArchive& ar)
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
void CDNAssistDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CDNAssistDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDNAssistDoc::SetSearchContent(const CString& value)
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

void CDNAssistDoc::SetModifiedFlag(BOOL bModified)
{
	CDocument::SetModifiedFlag(bModified);
	CString title = GetTitle();
	title.Replace(_T(" *"), _T(""));
	SetTitle(title);
}

void CDNAssistDoc::AddToHistory()
{
	SetModifiedFlag(TRUE);	
	UpdateAllViews(NULL);
}

void CDNAssistDoc::SetData(string& dataStr, char seqType)
{
	m_dataStr = dataStr;
	m_seqType = seqType;
	AddToHistory();
}

// CDNAssistDoc commands

void CDNAssistDoc::OnChangecaseUppercase()
{
	string dataStr = m_dataStr;
	transform(m_dataStr.begin(), m_dataStr.end(), m_dataStr.begin(), ::toupper);
	AddToHistory();
}

void CDNAssistDoc::OnChangecaseLowercase()
{
	string dataStr = m_dataStr;
	transform(m_dataStr.begin(), m_dataStr.end(), m_dataStr.begin(), ::tolower);
	AddToHistory();
}

void CDNAssistDoc::OnConvertCircularize()
{
	m_form = 'C';
	UpdateAllViews(NULL);
}


void CDNAssistDoc::OnConvertLinearize()
{
	m_form = 'L';
	UpdateAllViews(NULL);
}

void CDNAssistDoc::OnUpdateConvertDna2rna(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'D');
}

void CDNAssistDoc::OnUpdateConvertRna2dna(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'R');
}

void CDNAssistDoc::OnUpdateConvertDna2pro(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'D');
}

void CDNAssistDoc::OnUpdateConvertRna2pro(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'R');
}

void CDNAssistDoc::OnUpdateConvertCircularize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'D' && m_form != 'C');
}

void CDNAssistDoc::OnUpdateConvertLinearize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'D' && m_form != 'L');
}

void CDNAssistDoc::OnUpdateConvertReversetranslate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_seqType == 'P');
}

void CDNAssistDoc::OnUpdateEditFind(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && m_seqType != 'G');
}

void CDNAssistDoc::OnUpdateTranslate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && m_seqType == 'D');
}

void CDNAssistDoc::OnUpdateAnalyzeFindpattern(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && m_seqType != 'G');
}

void CDNAssistDoc::OnUpdateAnalyzeFindorfs(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && (m_seqType == 'D' || m_seqType == 'R'));
}

void CDNAssistDoc::OnUpdatePropertiesPhysicochemical(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && (m_seqType == 'D' || m_seqType == 'P' || m_seqType == 'R'));
}

void CDNAssistDoc::OnUpdatePropertiesAntigenicity(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > theApp.antigenicityOptions && m_seqType == 'P');
}

void CDNAssistDoc::OnUpdatePropertiesHydrophobicity(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > theApp.hydrophobicityOptions && m_seqType == 'P');
}

void CDNAssistDoc::OnUpdatePropertiesHydrophilicity(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > theApp.hydrophilicityOptions && m_seqType == 'P');
}

void CDNAssistDoc::OnUpdateAnalyzeList(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0);
}


void CDNAssistDoc::OnUpdateAnalyzeRestrictionmapping(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && m_seqType == 'D');
}

void CDNAssistDoc::OnUpdateAnalyzeTranscriptionfactors(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 0 && m_seqType == 'D');
}

void CDNAssistDoc::OnUpdateAnalyzeNucleosomepositions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_dataStr.length() > 125 && m_seqType == 'D');
}

// CDNAssistDoc diagnostics

#ifdef _DEBUG
void CDNAssistDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDNAssistDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

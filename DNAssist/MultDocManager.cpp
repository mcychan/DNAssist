// MultDocManager.cpp: implementation of the CMultDocManager class.
//
//////////////////////////////////////////////////////////////////////

/*

	author:			Babak Taati ( http://qlink.queensu.ca/~3bt1/ )
	Download link:	http://qlink.queensu.ca/~3bt1/Miscellaneous/Downloads.htm	

	Description:	This is a simple project that allows opening / saving multiple types of documents
  
	References: 	1. MFC Document/View documentation and enhancements, by Roger Allen on CodeProject.com
					2. How to support two file extensions per MFC document type, Micorosft support document Article ID 141921
					3. DocMgrEx.exe Assoc Multiple File Extension w/2 Doc Tyle, Micorosft support document Article ID 198538
					4. Selecting multiple files in File Open dialog, by Kaus Gutter on CodeGuru.com
					5. Opening multiple documents of several types at once, by Ernesto Perales Soto on CodeProject.com

	Date:			Nov 2004

*/

#include "stdafx.h"
#include "DNAssist.h"
#include "MultDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMultDocManager::CMultDocManager(CString strFilterExts, CString strFilterNames)
{
	m_Index = 0;

	int nTokenPos = 0;
	CString strToken;
	do {
		strToken = strFilterExts.Tokenize(_T(";"), nTokenPos);
		m_strFilterExts.Add(strToken);
	} while(!strToken.IsEmpty());

	nTokenPos = 0;
	do {
		strToken = strFilterNames.Tokenize(_T(";"), nTokenPos);
		m_strFilterNames.Add(strToken);
	} while (!strToken.IsEmpty());
}

CMultDocManager::~CMultDocManager()
{

}


void CMultDocManager::AppendFilterSuffix(CString& filter, OPENFILENAME& ofn, CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&	 
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName))
	{
		strFilterExt = m_strFilterExts.GetAt(m_Index);
		strFilterName = m_strFilterNames.GetAt(m_Index++);
		// a file based document template - add to filter list
		if (strFilterExt.IsEmpty() || strFilterName.IsEmpty())
			return;

		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += _T('\0');  // next string please
		filter += _T('*');
		
		//Babak: if there is a semicolon, instert a * for the second extension (e.g. change .ex1;.ex2 to .ex1;*.ex2
		strFilterExt.Replace(_T(";."), _T(";*."));

		
		filter += strFilterExt;
		filter += _T('\0');  // next string please
		ofn.nMaxCustFilter++;
	}
}


// Babak: overriding DoPromptFileName so it shows all the file types (except the first one)
//		the first one (only for opening files)		*.ex1;*.ex2
//		the second one								*.ex1
//		the third one								*.ex2
//		the default one (added to everything)		*.*

BOOL CMultDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	m_Index = 0;
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL) {
		ASSERT_VALID(pTemplate);
		AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else {
		BOOL bFirst = TRUE;
		int i = 0;
		int nCount = bOpenFileDialog ? m_strFilterNames.GetCount() : m_strFilterNames.GetCount() - 2;
		while (i++ < nCount)
		{
			CDocTemplate* pTemplate = (CDocTemplate*) m_templateList.GetHead();			
			AppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}


	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrInitialDir = theApp.sequenceDirectory;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	INT_PTR nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}



void CMultDocManager::OnFileNew()
{
	if (m_templateList.IsEmpty())
	{
		TRACE0("Error: no document templates registered with CWinApp.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}

	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();
	
	
	// Babak:	I don't want the select document type dialog box 
	//			It will always assume that it is of the first type
	//			if you want anything else, use CreateNewDocument() below

	/*	

	if (m_templateList.GetCount() > 1)
	{
		// more than one document template to choose from
		// bring up dialog prompting user
		CNewTypeDlg dlg(&m_templateList);
		int nID = dlg.DoModal();
		if (nID == IDOK)
			pTemplate = dlg.m_pSelectedTemplate;
		else
			return;     // none - cancel operation
	}
	*/

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	pTemplate->OpenDocumentFile(NULL);
		// if returns NULL, the user has already been alerted
}


// Babak: so you can "New" documents of other types 
//			source: Roger Allen, CodeProject.com
	

CDocument* CMultDocManager::CreateNewDocument(int doc_index, CString filename)
{
    // Find the correct document template and create a document from it
    CDocument    *pDoc = NULL;
    if (m_templateList.GetCount() >= doc_index)
    {
        POSITION    pos = m_templateList.GetHeadPosition();
        CDocTemplate *pTemplate = NULL;
        // iterate through the list looking for the required document type
        if (doc_index == 0)
        {
            pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
        }
        while (pos != NULL && doc_index > 0)
        {
            pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
            ASSERT_KINDOF(CDocTemplate, pTemplate);
        }
        if (pTemplate != NULL)
        {
            // create the document
            if (filename == "")
            {
                pDoc = pTemplate->OpenDocumentFile(NULL);
            }
            else
            {
                pDoc = pTemplate->OpenDocumentFile(filename);
            }
        }
    }
    // return the document pointer or NULL if failed
    return pDoc;
}
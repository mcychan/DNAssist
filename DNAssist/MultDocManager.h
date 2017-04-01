// MultDocManager.h: interface for the CMultDocManager class.
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

#if !defined(AFX_MULTDOCMANAGER_H__D21C5F82_55B3_4E50_8675_F6F11EB8F20D__INCLUDED_)
#define AFX_MULTDOCMANAGER_H__D21C5F82_55B3_4E50_8675_F6F11EB8F20D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Babak: deriving a class from CDocManager

enum mdt_MyDocumentTypes
{
    mdt_FirstDocumentType = 0,
    mdt_SecondDocumentType,
    mdt_ThirdDocumentType                // ... add extras as required
};



class CMultDocManager : public CDocManager  
{
private:
	int m_Index;
	CStringArray m_strFilterExts, m_strFilterNames;

protected:
	void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn, CDocTemplate* pTemplate, CString* pstrDefaultExt);

public:
	CMultDocManager(CString strFilterExts, CString strFilterNames);
	virtual ~CMultDocManager();

	CDocument*    CreateNewDocument(int doc_index, CString filename = _T(""));
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

	void OnFileNew();
};

#endif // !defined(AFX_MULTDOCMANAGER_H__D21C5F82_55B3_4E50_8675_F6F11EB8F20D__INCLUDED_)

#pragma once
#include "DNAssistDoc.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

// CAlignDialog

class CAlignDialog : public CDialog
{
	DECLARE_DYNAMIC(CAlignDialog)

public:
	CAlignDialog(CWnd* pParent = NULL);
	virtual ~CAlignDialog();	
	char GetSequenceType();
	void GetSequenceList(vector<string>& seqList);
	inline vector<CString>* GetNameList() {
		return &m_sequencenames;
	}

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALIGN };
#endif

protected:
	int m_iImage;
	CListCtrl m_listCtrl;
	CImageList m_imageList;
	vector<CString> m_sequencenames;
	vector<CDNAssistDoc*> m_pDocList;
	map<CString, CDNAssistDoc*> m_pDocMap;	

	void GetAllDocuments();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	void SetupImages(CImageList* pImageList, int iSize);
	void SetupImages();	

	DECLARE_MESSAGE_MAP()
	afx_msg void OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
};

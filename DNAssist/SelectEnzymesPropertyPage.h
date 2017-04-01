#pragma once
#include "model/EnzymeList.h"

#include <memory>
#include <string>
#include "afxwin.h"
#include "SortClass.h"

using namespace std;

// CSelectEnzymesPropertyPage

class CSelectEnzymesPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSelectEnzymesPropertyPage)

public:
	CSelectEnzymesPropertyPage();
	virtual ~CSelectEnzymesPropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESTENDO };
#endif

protected:
	int m_nColumn;
	bool m_bAscending[2];
	CSortClass::EDataType dataTypes[2];
	CString m_enzymeDir, m_enzymeFile;
	CComboBox m_cmbEnzymeFile;
	CListCtrl m_listEnzymeNames;
	unique_ptr<CEnzymeList> m_pEnzymelist;
	afx_msg void OnEnChangeEnzymepath();
	void AddEnzyme(int enz = -1);
	BOOL LoadEnzymeList();
	BOOL SaveEnzymeFile(LPCTSTR filename);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnLbnSelchangeFile();
	afx_msg void OnBnClickedAddenzyme();
	afx_msg void OnBnClickedRemoveEnzyme();
	afx_msg void OnBnClickedSaveEnzymeList();
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
};

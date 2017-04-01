#pragma once
#include "afxwin.h"
#include "model/EnzymeList.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// CFactorSequencePropertyPage 

class CFactorSequencePropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFactorSequencePropertyPage)

public:
	CFactorSequencePropertyPage(CEnzymeList* factorlist);
	BOOL selectionchanged;
	virtual void OnOK();
	virtual ~CFactorSequencePropertyPage();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 
	virtual BOOL OnSetActive();	
	BOOL m_selectall;
	string field1, field2;
	map<CString, CString> dataMap;
	CListBox m_listbox1;
	CListBox m_listbox2;

	CEnzymeList* listoffactors;
	inline string GetField() const {
		return field1;
	}

	DECLARE_MESSAGE_MAP()
	afx_msg void GetListBoxSelectedStrings(vector<string>& entries);
	afx_msg int GetListBoxSelectionIndexes(vector<int>& entries);
	afx_msg void UpdateListBox();
	afx_msg void QueryDataBase(const vector<string>& selections, const string& searchBy = "Species");
	afx_msg void IssueQuery(const string& querystring);
	afx_msg void QueryDatabaseForSequences(const string& filter);
	afx_msg void UpdateSequenceListBox();
	afx_msg void OnBnClickedSelectall();
	afx_msg void OnLbnSelchangeFactor();
	afx_msg void OnLbnSelchangeSequence();
};

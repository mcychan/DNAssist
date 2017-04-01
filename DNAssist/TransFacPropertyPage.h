#pragma once
#include "afxwin.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// CTransFacPropertyPage

class CTransFacPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTransFacPropertyPage)

public:
	CTransFacPropertyPage(const string& pField);
	virtual ~CTransFacPropertyPage();
	afx_msg void GetListBoxSelectedStrings(vector<string>& entries);
	afx_msg void UpdateListBox(bool isLoaded = true);
	BOOL selectionchanged;
	inline string GetField() const {
		return field;
	}

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUP };
#endif

protected:
	string field;
	CString m_Title;
	CListBox m_listBox;
	afx_msg void QueryDataBase(const vector<string>& selections, const string& searchBy = "Kingdom");
	afx_msg void IssueQuery(const string& querystring);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLbnSelChangeGroup();
};

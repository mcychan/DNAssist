// FactorSequencePropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "FactorSequencePropertyPage.h"
#include "TransFacPropertyPage.h"
#include "TransFacPropertySheet.h"
#include "CppSQLite3.h"

// CFactorSequencePropertyPage 

IMPLEMENT_DYNAMIC(CFactorSequencePropertyPage, CPropertyPage)

CFactorSequencePropertyPage::CFactorSequencePropertyPage(CEnzymeList* factorlist)
	: listoffactors(factorlist), CPropertyPage(IDD_FACTOR)
	, m_selectall(FALSE)
{
	selectionchanged = true;
	field1 = "Factor";
	field2 = "Sequence";
}

CFactorSequencePropertyPage::~CFactorSequencePropertyPage()
{
}

void CFactorSequencePropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FACTOR, m_listbox1);
	DDX_Control(pDX, IDC_SEQUENCE, m_listbox2);
	DDX_Check(pDX, IDC_TRANSFAC_SELECTALL, m_selectall);
}


BEGIN_MESSAGE_MAP(CFactorSequencePropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_TRANSFAC_SELECTALL, OnBnClickedSelectall)
	ON_LBN_SELCHANGE(IDC_FACTOR, OnLbnSelchangeFactor)
	ON_LBN_SELCHANGE(IDC_SEQUENCE, OnLbnSelchangeSequence)
END_MESSAGE_MAP()


// CFactorSequencePropertyPage �T���B�z�`��
BOOL CFactorSequencePropertyPage::OnSetActive()
{
	if(selectionchanged)
		UpdateListBox();
	return CPropertyPage::OnSetActive();
}

void CFactorSequencePropertyPage::GetListBoxSelectedStrings(vector<string>& entries)
{
	int count = m_listbox1.GetSelCount();
	entries.clear();
	if(count) {
		entries.reserve(count);
		count = m_listbox1.GetCount();
		for (int i=0; i < count; ++i) {
			if(m_listbox1.GetSel(i) > 0) {
				CString temp;
				m_listbox1.GetText(i, temp);
				string str((LPCSTR) CStringA(temp));
				entries.emplace_back(str);
			}
		}
	}
	else {
		count = m_listbox1.GetCount();
		entries.resize(count);
		for (int i=0; i < count; ++i) {
			CString temp;
			m_listbox1.GetText(i, temp);
			string str((LPCSTR) CStringA(temp));
			entries[i] = str;
		}
	}
}

int CFactorSequencePropertyPage::GetListBoxSelectionIndexes(vector<int>& entries)
{
	int count = m_listbox1.GetCount();
	entries.clear();
	for (int i=0; i < count; ++i) {
		if(m_listbox1.GetSel(i) > 0)
			entries.push_back(i);
	}
	return count;
}

void CFactorSequencePropertyPage::UpdateListBox()
{
	vector<string> selections;
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	if (!pSheet->FindSelections(field1, selections))
		return;

	QueryDataBase(selections);
	UpdateSequenceListBox();
}

void CFactorSequencePropertyPage::QueryDataBase(const vector<string>& selections, const string& searchBy)
{
	string query = "SELECT DISTINCT " + field1 + " FROM Factor ";
	if(selections.size() > 0) {
		query +=  "WHERE " + searchBy + " IN (\"";
		string separator = "\", \"";
		stringstream s;
		copy(selections.begin(), selections.end(), ostream_iterator<string>(s, separator.c_str()));
		query += s.str().erase(s.str().length() - separator.length()) + "\")";
	}
	IssueQuery(query);
}

void CFactorSequencePropertyPage::IssueQuery(const string& querystring)
{
	m_listbox1.SetRedraw(FALSE);
	m_listbox1.ResetContent();
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	auto db = pSheet->GetDatabase();

	try {
		CppSQLite3Query q = db->execQuery(querystring.c_str());

		while (!q.eof()) {
			CString temp2(q.fieldValue(field1.c_str()));
			m_listbox1.AddString(CString(temp2));
			q.nextRow();
		}
		pSheet->QueryCompleted(field1);
	}
	catch (CppSQLite3Exception& e)
	{
		CString errMsg =_T("No table found.");
		AfxMessageBox(errMsg);
	}
	m_listbox1.SetRedraw(TRUE);
	m_listbox1.Invalidate();
}

void CFactorSequencePropertyPage::UpdateSequenceListBox()
{
	BeginWaitCursor();
	vector<string> selections;
	GetListBoxSelectedStrings(selections);
	string filter = "";
	if (selections.size() > 0) {
		filter += "WHERE F." + field1 + " IN (\"";
		string separator = "\", \"";
		stringstream s;
		copy(selections.begin(), selections.end(), ostream_iterator<string>(s, separator.c_str()));
		filter += s.str().erase(s.str().length() - separator.length()) + "\")";
	}
	QueryDatabaseForSequences(filter);
	EndWaitCursor();
}

void CFactorSequencePropertyPage::QueryDatabaseForSequences(const string& filter)
{
	string temp = "SELECT DISTINCT F." + field1 + ", " + "S." + field2 + " FROM Factor F ";
	temp += "INNER JOIN Reference R ON F.Number = R.Number ";
	temp += "INNER JOIN Sequence S ON R.Reference = S.Reference ";
	if(!filter.empty())
		temp += filter;

	m_listbox2.SetRedraw(FALSE);
	m_listbox2.ResetContent();
	dataMap.clear();
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	auto db = pSheet->GetDatabase();

	try {
		CppSQLite3Query q = db->execQuery(temp.c_str());

		while (!q.eof()) {
			CString temp1(q.fieldValue(field1.c_str()));
			CString temp2(q.fieldValue(field2.c_str()));
			if(temp2.GetLength()) {
				m_listbox2.AddString(temp2);
				dataMap[temp2] = temp1;
			}
			q.nextRow();
		}
		selectionchanged = FALSE;
	}
	catch (CppSQLite3Exception& e)
	{
		CString errMsg =_T("No table found.");
		AfxMessageBox(errMsg);
	}
	m_listbox2.SetRedraw(TRUE);
	m_listbox2.Invalidate();
}

void CFactorSequencePropertyPage::OnBnClickedSelectall()
{
	m_selectall = !m_selectall;
	UpdateData(FALSE);
	int count = m_listbox1.GetCount();
	for(int i = 0 ; i < count; ++i)
		m_listbox1.SetSel(i, m_selectall);
	UpdateSequenceListBox();
}

void CFactorSequencePropertyPage::OnOK()
{
	int numbers = m_listbox2.GetCount();
	for(int x = 0; x < numbers; x++) {
		CString temp2;
		m_listbox2.GetText(x, temp2);
		CString temp1 = dataMap[temp2];
		string str1((LPCSTR) CStringA(temp1));
		string str2((LPCSTR) CStringA(temp2));
		listoffactors->AddEnzyme(str1, str2, str2, "", "");
	}

	dataMap.clear();
	CPropertyPage::OnOK();
}


void CFactorSequencePropertyPage::OnLbnSelchangeFactor()
{
	UpdateSequenceListBox();
	selectionchanged = TRUE;
}


void CFactorSequencePropertyPage::OnLbnSelchangeSequence()
{
	selectionchanged = TRUE;
}

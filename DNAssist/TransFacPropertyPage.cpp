// TransFacPropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "TransFacPropertyPage.h"
#include "TransFacPropertySheet.h"
#include "CppSQLite3.h"

// CTransFacPropertyPage 

IMPLEMENT_DYNAMIC(CTransFacPropertyPage, CPropertyPage)

CTransFacPropertyPage::CTransFacPropertyPage(const string& pField)
	: field(pField), CPropertyPage(IDD_GROUP)
{
	selectionchanged = true;
	m_psp.dwFlags |= PSP_USETITLE;
	m_psp.pszTitle = m_Title = CString(field.c_str());	
}

CTransFacPropertyPage::~CTransFacPropertyPage()
{
}

void CTransFacPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP, m_listBox);
}


BEGIN_MESSAGE_MAP(CTransFacPropertyPage, CPropertyPage)
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_GROUP, OnLbnSelChangeGroup)
END_MESSAGE_MAP()


// CTransFacPropertyPage �T���B�z�`��
BOOL CTransFacPropertyPage::OnSetActive()
{
	if(selectionchanged)
		UpdateListBox(false);
	return CPropertyPage::OnSetActive();
}

void CTransFacPropertyPage::GetListBoxSelectedStrings(vector<string>& entries)
{
	int count = m_listBox.GetSelCount();
	entries.reserve(count);

	if(count) {
		count = m_listBox.GetCount();
		entries.clear();
		for (int i=0; i < count; ++i) {
			if(m_listBox.GetSel(i) > 0) {
				CString temp;
				m_listBox.GetText(i, temp);
				string str((LPCSTR) CStringA(temp));
				entries.emplace_back(str);
			}
		}
	}
}

void CTransFacPropertyPage::QueryDataBase(const vector<string>& selections, const string& searchBy)
{
	//HCURSOR old_cursor = ::SetCursor(::LoadCursor(0, IDC_WAIT));
	string temp = "SELECT DISTINCT " + field + " FROM Factor ";
	if(selections.empty())
		IssueQuery(temp);
	else {
		if(field == searchBy)
			return;

		temp += "WHERE " + searchBy + " IN (\"";
		string separator = "\", \"";
		stringstream s;
		copy(selections.begin(), selections.end(), ostream_iterator<string>(s, separator.c_str()));
		temp += s.str().erase(s.str().length() - separator.length()) + "\")";

		IssueQuery(temp);
	}

//::SetCursor(old_cursor);
}

void CTransFacPropertyPage::UpdateListBox(bool isLoaded)
{
	vector<int> indexes;
	vector<string> selections;
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	BOOL selectionHasChanged = pSheet->FindSelections(field, selections);
	if(!isLoaded || selectionHasChanged)
		QueryDataBase(selections);
}

void CTransFacPropertyPage::IssueQuery(const string& querystring)
{
	m_listBox.SetRedraw(FALSE);
	m_listBox.ResetContent();
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	auto db = pSheet->GetDatabase();
	try {
		CppSQLite3Query q = db->execQuery(querystring.c_str());
		while (!q.eof()) {
			CString val(q.fieldValue(field.c_str()));
			if(val.GetLength())
				m_listBox.AddString(val);
			q.nextRow();
		}
		pSheet->QueryCompleted(field);
	}
	catch (CppSQLite3Exception& e)
	{
		CString errMsg =_T("No table found.");
		AfxMessageBox(errMsg);
	}
	m_listBox.SetRedraw(TRUE);
	m_listBox.Invalidate();
}

void CTransFacPropertyPage::OnLbnSelChangeGroup()
{
	CTransFacPropertySheet* pSheet = (CTransFacPropertySheet*) GetParent();
	pSheet->OnLbSelChange(field);
	selectionchanged = true;
}

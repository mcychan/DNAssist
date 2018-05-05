// SelectEnzymesPropertyPage.cpp : 
//

#include "stdafx.h"
#include "DNAssist.h"
#include "SelectEnzymesPropertyPage.h"
#include "AddEnzymeDialog.h"
#include "RestrictEnzymesPropertySheet.h"

#include <fstream>      // std::ofstream

// CSelectEnzymesPropertyPage

IMPLEMENT_DYNAMIC(CSelectEnzymesPropertyPage, CPropertyPage)

CSelectEnzymesPropertyPage::CSelectEnzymesPropertyPage()
	: CPropertyPage(IDD_RESTENDO)
{
	dataTypes[0] = dataTypes[1] = CSortClass::dtSTRING;
	int iSlash = theApp.enzymePath.ReverseFind(_T('\\'));
	m_enzymeDir = theApp.enzymePath.Left(iSlash);
	m_enzymeFile = theApp.enzymePath.Mid(iSlash + 1);
}

CSelectEnzymesPropertyPage::~CSelectEnzymesPropertyPage()
{
}

void CSelectEnzymesPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SORT_BY, m_cmbEnzymeFile);
	DDX_Control(pDX, IDC_ENZYMELIST, m_listEnzymeNames);
}

BOOL CSelectEnzymesPropertyPage::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_listEnzymeNames.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	DWORD dwStyle = m_listEnzymeNames.GetExtendedStyle();
#if (WINVER >= 0x0600)
	dwStyle |= LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
	m_listEnzymeNames.SendMessage(WM_CHANGEUISTATE, MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowTheme(m_listEnzymeNames.GetSafeHwnd(), _T("Explorer"), NULL);
#elif (_WIN32_WINNT >= 0x0501)
	dwStyle |= LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
	m_listEnzymeNames.SendMessage(WM_CHANGEUISTATE, MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowTheme(m_listEnzymeNames.GetSafeHwnd(), _T("Explorer"), NULL);
#else
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
#endif
	m_listEnzymeNames.SetExtendedStyle(dwStyle);

	CRect r;
	m_listEnzymeNames.GetClientRect(&r);
	const int nWidth = 50;
	const int scrollWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_listEnzymeNames.InsertColumn(0, _T("Name"), LVCFMT_LEFT);
	m_listEnzymeNames.SetColumnWidth(0, 70);
	m_listEnzymeNames.InsertColumn(1, _T("Site"), LVCFMT_LEFT);
	m_listEnzymeNames.SetColumnWidth(1, r.Width() + scrollWidth - m_listEnzymeNames.GetColumnWidth(0) - nWidth * 2);
	m_listEnzymeNames.InsertColumn(2, _T("Upper"), LVCFMT_RIGHT);
	m_listEnzymeNames.SetColumnWidth(2, nWidth);
	m_listEnzymeNames.InsertColumn(3, _T("Lower"), LVCFMT_RIGHT);
	m_listEnzymeNames.SetColumnWidth(3, nWidth);
	OnEnChangeEnzymepath();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX property page
}

void CSelectEnzymesPropertyPage::OnEnChangeEnzymepath()
{
	m_cmbEnzymeFile.SetRedraw(FALSE);
	m_cmbEnzymeFile.ResetContent();

	TCHAR lpszOldPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpszOldPath);
	SetCurrentDirectory(m_enzymeDir);
	m_cmbEnzymeFile.Dir(DDL_READWRITE, _T("*.*"));
	SetCurrentDirectory(lpszOldPath);

	m_cmbEnzymeFile.SelectString(0, m_enzymeFile);
	m_cmbEnzymeFile.SetRedraw(TRUE);
	m_cmbEnzymeFile.Invalidate();
	LoadEnzymeList();
}

void CSelectEnzymesPropertyPage::OnOK()
{
	CDialog::OnOK();
	auto pSheet = (CRestrictEnzymesPropertySheet*) GetParent();
	pSheet->SetEnzymeList(m_pEnzymelist.release());
	CString enzymePath;
	enzymePath.Format(_T("%s\\%s"), m_enzymeDir, m_enzymeFile);
	if (PathFileExists(enzymePath))
		theApp.enzymePath = enzymePath;
}

void CSelectEnzymesPropertyPage::AddEnzyme(int enz)
{
	if (m_pEnzymelist->GetNumberOfEnzymes() == 0)
		m_listEnzymeNames.DeleteAllItems();

	if(enz < 0) {
		wstring name(_T("[No enzymes]"));
		LV_ITEM	lvitem;
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = 0;
		lvitem.iSubItem = 0;
		lvitem.pszText = _T("");
		int iActualItem = m_listEnzymeNames.InsertItem(&lvitem);
		lvitem.pszText = (LPTSTR) name.c_str();
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = iActualItem;
		lvitem.iSubItem = 1;
		m_listEnzymeNames.SetItem(&lvitem);
		m_listEnzymeNames.SetRedraw(TRUE);
		m_listEnzymeNames.Invalidate();
		return;
	}

	string name = m_pEnzymelist->GetEnzyme(enz);
	wstring Name(name.begin(), name.end());

	LV_ITEM	lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = 0;
	lvitem.iSubItem = 0;
	lvitem.pszText = (LPTSTR) Name.c_str();
	int iActualItem = m_listEnzymeNames.InsertItem(&lvitem);

	string site;
	m_pEnzymelist->GetSiteFormatted(enz, site);
	wstring Site(site.begin(), site.end());
	lvitem.pszText = (LPTSTR) Site.c_str();
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 1;
	m_listEnzymeNames.SetItem(&lvitem);

	string upper;
	m_pEnzymelist->GetUpperCut(enz, upper);
	wstring Upper(upper.begin(), upper.end());
	lvitem.pszText = (LPTSTR) Upper.c_str();
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 2;
	m_listEnzymeNames.SetItem(&lvitem);

	string lower;
	m_pEnzymelist->GetLowerCut(enz, lower);
	wstring Lower(lower.begin(), lower.end());
	lvitem.pszText = (LPTSTR) Lower.c_str();
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = iActualItem;
	lvitem.iSubItem = 3;
	m_listEnzymeNames.SetItem(&lvitem);
}

BOOL CSelectEnzymesPropertyPage::LoadEnzymeList()
{	
	m_listEnzymeNames.SetRedraw(FALSE);
	m_listEnzymeNames.DeleteAllItems();	

	CString enzymePath;
	enzymePath.Format(_T("%s\\%s"), m_enzymeDir, m_enzymeFile);
	wstring fullpath((LPCTSTR) enzymePath);
	m_pEnzymelist = make_unique<CEnzymeList>(fullpath);
	if (!PathFileExists(enzymePath)) {
		AddEnzyme();
		AfxMessageBox(_T("File not found: ") + enzymePath);
		return FALSE;
	}
	
	if(m_pEnzymelist->GetNumberOfEnzymes() == 0) {
		AddEnzyme();
		return FALSE;
	}

	m_pEnzymelist->SortList(EL_SORTNAMEDESCENDING);
	for(int enz = 0; enz < m_pEnzymelist->GetNumberOfEnzymes(); enz++) {
		if ((EL_NOSITE & m_pEnzymelist->GetAttribute(enz)) != EL_NOSITE)
			AddEnzyme(enz);
	}

	m_bAscending[0] = true;
	HD_ITEM hditem;
	hditem.mask = HDI_FORMAT;
	m_listEnzymeNames.GetHeaderCtrl()->GetItem(0, &hditem);
	hditem.fmt |= HDF_BITMAP_ON_RIGHT;
#if (_WIN32_WINNT >= 0x0501)
	hditem.fmt |= m_bAscending[0] ? HDF_SORTUP : HDF_SORTDOWN;
#endif
	VERIFY(m_listEnzymeNames.GetHeaderCtrl()->SetItem(0, &hditem));

	m_listEnzymeNames.SetRedraw(TRUE);
	m_listEnzymeNames.Invalidate();
	return m_pEnzymelist->GetNumberOfEnzymes() > 0;
}

BEGIN_MESSAGE_MAP(CSelectEnzymesPropertyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_LBN_SELCHANGE(IDC_SORT_BY, OnLbnSelchangeFile)
	ON_BN_CLICKED(IDC_ADDENZYME, OnBnClickedAddenzyme)
	ON_BN_CLICKED(IDC_REMOVEENZYME, OnBnClickedRemoveEnzyme)
	ON_BN_CLICKED(IDC_SAVEENZYMELIST, OnBnClickedSaveEnzymeList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ENZYMELIST, OnColumnClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ENZYMELIST, OnItemchangedListCtrl)
END_MESSAGE_MAP()


// CSelectEnzymesPropertyPage
void CSelectEnzymesPropertyPage::OnBnClickedBrowse()
{
	CFolderPickerDialog folderPickerDialog(m_enzymeDir, OFN_FILEMUSTEXIST | OFN_ENABLESIZING, this,
		sizeof(OPENFILENAME));
	if (folderPickerDialog.DoModal() == IDOK) {
		POSITION pos = folderPickerDialog.GetStartPosition();
		if (pos) {
			m_enzymeDir = folderPickerDialog.GetNextPathName(pos);
			OnEnChangeEnzymepath();
			UpdateData(FALSE);
		}
	}
}

void CSelectEnzymesPropertyPage::OnLbnSelchangeFile()
{
	LoadEnzymeList();
}

BOOL CSelectEnzymesPropertyPage::SaveEnzymeFile(LPCTSTR filename)
{
	ofstream file(filename);
	if(!file) {
		AfxMessageBox(_T("Enzyme file could not be updated.  Is it open in another application?"), MB_ICONERROR | MB_OK);
		return FALSE;
	}

	int nsel = m_listEnzymeNames.GetSelectedCount();
	if(nsel) {
		int count = m_listEnzymeNames.GetItemCount();
		for (int i=0; i < count; ++i) {
			UINT uState = m_listEnzymeNames.GetItemState(i, LVIS_SELECTED);
			if (uState & LVIS_SELECTED) {
				string name = m_pEnzymelist->GetName(i);
				if (name.empty())
					continue;
				if((m_pEnzymelist->GetAttribute(i) & EL_CUTOUTSIDE) == EL_CUTOUTSIDE)
					file.write("#", 1);
				file << name;
				file.write(",", 1);
				file.write("\t", 1);
				string formattedsite;
				m_pEnzymelist->GetSiteFormatted(i, formattedsite);
				file << formattedsite;
				file.write(",", 1);
				file.write("\n", 1);
			}
		}
	}
	else {
		int nsel = m_listEnzymeNames.GetItemCount();
		for(int x = 0; x < nsel; x++) {
			string name = m_pEnzymelist->GetName(x);
			if (name.empty())
				continue;
			if((m_pEnzymelist->GetAttribute(x) & EL_CUTOUTSIDE) == EL_CUTOUTSIDE)
				file.write("#", 1);
			file << name << ',' << '\t';
			string formattedsite;
			m_pEnzymelist->GetSiteFormatted(x, formattedsite);

			file << formattedsite << ',' << endl;
		}
	}
	return TRUE;
}

void CSelectEnzymesPropertyPage::OnBnClickedAddenzyme()
{
	AddEnzymeTransferBuffer addEnzymeTransferBuffer;
	CAddEnzymeDialog addEnzymeDialog(&addEnzymeTransferBuffer);
	if(addEnzymeDialog.DoModal() != IDOK)
		return;

	int uppersize = stoi(addEnzymeTransferBuffer.lowertext);
	int lowersize = stoi(addEnzymeTransferBuffer.uppertext);
	int attribute = m_pEnzymelist->AssignAttribute(addEnzymeTransferBuffer.sitetext, uppersize, lowersize);
	string attr = to_string(attribute);
	int entrynumber = -1;
	if(!addEnzymeTransferBuffer.nametext.empty() && !addEnzymeTransferBuffer.sitetext.empty() && !addEnzymeTransferBuffer.lowertext.empty() && !addEnzymeTransferBuffer.uppertext.empty() && !attr.empty()) {
		entrynumber = m_pEnzymelist->AddEnzyme(addEnzymeTransferBuffer.nametext, addEnzymeTransferBuffer.sitetext, addEnzymeTransferBuffer.uppertext, addEnzymeTransferBuffer.lowertext, attr);
	}

	//Add the enzyme to the list box
	if(entrynumber >= 0)
		AddEnzyme(entrynumber - 1);

	CString enzymePath;
	enzymePath.Format(_T("%s\\%s"), m_enzymeDir, m_enzymeFile);
	if (PathFileExists(enzymePath)) {
		if(SaveEnzymeFile(enzymePath))
			LoadEnzymeList();
	}
}

void CSelectEnzymesPropertyPage::OnBnClickedRemoveEnzyme()
{
	int nsel = m_listEnzymeNames.GetSelectedCount();
	if(nsel) {
		if (AfxMessageBox(_T("Are you sure?"), MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
			return;

		int count = m_listEnzymeNames.GetItemCount();
		for (int i = count; i > -1; --i) {
			UINT uState = m_listEnzymeNames.GetItemState(i, LVIS_SELECTED);
			if (uState & LVIS_SELECTED) {
				m_pEnzymelist->RemoveEnzymeBySortList(i);
				m_listEnzymeNames.DeleteItem(i);
			}
		}

		CString enzymePath;
		enzymePath.Format(_T("%s\\%s"), m_enzymeDir, m_enzymeFile);
		if (PathFileExists(enzymePath)) {
			if(SaveEnzymeFile(enzymePath))
				LoadEnzymeList();
		}
	}
	else
		AfxMessageBox(_T("No enzymes were selected"), MB_ICONERROR | MB_OK);
}

void CSelectEnzymesPropertyPage::OnBnClickedSaveEnzymeList()
{
	CFileDialog dlgFile(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, NULL, GetParent());
	dlgFile.m_ofn.lpstrFilter = _T("Text file (*.txt)\0*.txt\0\0");
	dlgFile.m_ofn.nFilterIndex = 0;

	if (IDOK == dlgFile.DoModal()) {
		CString enzymePath = dlgFile.GetPathName();
		if(SaveEnzymeFile(enzymePath)) {
			int iSlash = enzymePath.ReverseFind(_T('\\'));
			m_enzymeDir = enzymePath.Left(iSlash);
			m_enzymeFile = enzymePath.Mid(iSlash + 1);
			OnEnChangeEnzymepath();
		}
	}
}

void CSelectEnzymesPropertyPage::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nColumn = pNMListView->iSubItem;
	if(m_nColumn > 1)
		return;

	m_bAscending[m_nColumn] = !m_bAscending[m_nColumn];
	CSortClass csc(&m_listEnzymeNames, m_nColumn);
	csc.Sort(m_bAscending[m_nColumn], dataTypes[m_nColumn]);

	int sortBy = EL_SORTNAMEASCENDING;
	if(m_nColumn == 0 && !m_bAscending[m_nColumn])
		sortBy = EL_SORTNAMEDESCENDING;
	else if(m_nColumn == 1)
		sortBy = m_bAscending[m_nColumn] ? EL_SORTSITEASCENDING : EL_SORTSITEDESCENDING;
	m_pEnzymelist->SortList(sortBy);

	CHeaderCtrl *header = m_listEnzymeNames.GetHeaderCtrl();
	for (int i=0; i<header->GetItemCount(); ++i) {
		HD_ITEM hditem;
		hditem.mask = HDI_FORMAT;
		header->GetItem(i, &hditem);
		hditem.fmt |= HDF_BITMAP_ON_RIGHT;
#if (_WIN32_WINNT >= 0x0501)
		if(i == m_nColumn) {
			hditem.fmt &= ~(m_bAscending[i] ? HDF_SORTDOWN : HDF_SORTUP);
			hditem.fmt |= (!m_bAscending[i] ? HDF_SORTDOWN : HDF_SORTUP);
		}
		else {
			hditem.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);
			hditem.iImage = -1;
		}
#endif
		VERIFY(header->SetItem(i, &hditem));
	}	
	*pResult = 0;
}

void CSelectEnzymesPropertyPage::OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    ASSERT(pNMLV);
    if(pNMLV->uChanged & LVIF_STATE) {
		if ((pNMLV->uOldState & LVIS_SELECTED) == 0 && (pNMLV->uNewState & LVIS_SELECTED) != 0) {
			const CString& zText = m_listEnzymeNames.GetItemText(pNMLV->iItem, 0);
			if (!zText.IsEmpty()) {
				wstring Name((LPCTSTR) zText);
				string name(Name.begin(), Name.end());
				int index = m_pEnzymelist->GetIndex(name);
				m_pEnzymelist->SetSelected(index, true);
			}
        }
		else if ((pNMLV->uOldState & LVIS_SELECTED) != 0 && (pNMLV->uNewState & LVIS_SELECTED) == 0) {
			const CString& zText = m_listEnzymeNames.GetItemText(pNMLV->iItem, 0);
			if (!zText.IsEmpty()) {
				wstring Name((LPCTSTR)zText);
				string name(Name.begin(), Name.end());
				int index = m_pEnzymelist->GetIndex(name);
				m_pEnzymelist->SetSelected(index, false);
			}
		}
    }

    *pResult = FALSE;
}

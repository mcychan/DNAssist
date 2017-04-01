// AlignDialog.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "AlignDialog.h"

// CAlignDialog 

IMPLEMENT_DYNAMIC(CAlignDialog, CDialog)

CAlignDialog::CAlignDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ALIGN, pParent)
{
	m_iImage = -1;
	// get a list of all open documents
	GetAllDocuments();
}

CAlignDialog::~CAlignDialog()
{
}

void CAlignDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALIGN_SEQUENCES, m_listCtrl);
}

void CAlignDialog::GetAllDocuments()
{
	// clear the list
	m_pDocMap.clear();
	// loop through application's document templates
	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while(NULL != posDocTemplate) {
		CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while(NULL != posDoc) {
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
			if (pDoc->IsKindOf(RUNTIME_CLASS(CDNAssistDoc))) {
				CDNAssistDoc* pDNADoc = dynamic_cast<CDNAssistDoc*>(pDoc);
				const CString& strTitle = pDNADoc->GetTitle();
				int iSlash = strTitle.ReverseFind(_T('.'));
				if(iSlash > -1)
					m_pDocMap[strTitle.Left(iSlash)] = pDNADoc; // add document to list
				else
					m_pDocMap[strTitle] = pDNADoc; // add document to list
			}
		}
	}
}

void CAlignDialog::SetupImages(CImageList* pImageList, int iSize)
{
	//	ASSERT(m_pImageList != NULL);    // serious allocation failure checking
	pImageList->Create(iSize, iSize, ILC_COLOR8 | ILC_MASK, 9, 9);
	pImageList->Add(theApp.LoadIcon(ICO_DNA1));
	pImageList->Add(theApp.LoadIcon(ICO_DNA2));
	pImageList->Add(theApp.LoadIcon(ICO_RNA1));
	pImageList->Add(theApp.LoadIcon(ICO_RNA2));
	pImageList->Add(theApp.LoadIcon(ICO_P1));
	pImageList->Add(theApp.LoadIcon(ICO_P2));
	pImageList->Add(theApp.LoadIcon(ICO_D1));
	pImageList->Add(theApp.LoadIcon(ICO_D2));
}

void CAlignDialog::SetupImages()
{
	SetupImages(&m_imageList, 16);
	m_listCtrl.SetImageList(&m_imageList, LVSIL_SMALL);
}

BOOL CAlignDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow();

	m_listCtrl.SetRedraw(FALSE);
	m_listCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	DWORD dwStyle = m_listCtrl.GetExtendedStyle();
#if (WINVER >= 0x0600)
	dwStyle |= LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
	m_listCtrl.SendMessage(WM_CHANGEUISTATE, MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowTheme(m_listCtrl.GetSafeHwnd(), _T("Explorer"), NULL);
#elif (_WIN32_WINNT >= 0x0501)
	dwStyle |= LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
	m_listCtrl.SendMessage(WM_CHANGEUISTATE, MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowTheme(m_listCtrl.GetSafeHwnd(), _T("Explorer"), NULL);
#else
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_HEADERDRAGDROP;
#endif
	m_listCtrl.SetExtendedStyle(dwStyle);

	SetupImages();

	CRect r;
	m_listCtrl.GetClientRect(&r);
	m_listCtrl.InsertColumn(0, _T("Align Sequences"), LVCFMT_LEFT);	
	m_listCtrl.SetColumnWidth(0, r.Width());
	for (auto iter = m_pDocMap.rbegin(); iter != m_pDocMap.rend(); ++iter) {
		wstring strTitle(iter->first);
		char seqType = iter->second->GetSeqType();

		LV_ITEM	lvitem;
		ZeroMemory(&lvitem, sizeof(lvitem));
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = 0;
		lvitem.iSubItem = 0;
		if(seqType == 'D')
			lvitem.iImage = 0; // image index in the CImageList
		else if(seqType == 'R')
			lvitem.iImage = 2; // image index in the CImageList
		else if(seqType == 'P')
			lvitem.iImage = 4; // image index in the CImageList
		else
			lvitem.iImage = 6; // image index in the CImageList
		lvitem.pszText = (LPTSTR) strTitle.c_str();
		m_listCtrl.InsertItem(&lvitem);
	}
	
	m_listCtrl.SetRedraw(TRUE);
	m_listCtrl.Invalidate();
	return TRUE;
}

void CAlignDialog::OnOK()
{
	int count = m_listCtrl.GetSelectedCount();

	if (count) {
		m_pDocList.clear();
		m_sequencenames.resize(count);
		m_pDocList.resize(count);
		count = m_listCtrl.GetItemCount();
		for (int i = 0; i < count; ++i) {
			UINT uState = m_listCtrl.GetItemState(i, LVIS_SELECTED);
			if (uState & LVIS_SELECTED) {
				CString strTitle = m_listCtrl.GetItemText(i, 0);
				m_sequencenames[i] = strTitle;
				m_pDocList[i] = m_pDocMap[strTitle];
			}
		}
	}

	CDialog::OnOK();
}

char CAlignDialog::GetSequenceType()
{
	if(m_pDocList.empty())
		return 0;
	return m_pDocList[0]->GetSeqType();
}

void CAlignDialog::GetSequenceList(vector<string>& seqList)
{
	size_t count = m_pDocList.size();
	seqList.resize(count);
	for (int y = 0; y < count; y++) {
		if (m_pDocList[y]->GetSeqLength() == 0)
			continue;
		auto seq = m_pDocList[y]->GetData();
		string tmpSeq = " " + *seq;
		transform(tmpSeq.begin(), tmpSeq.end(), tmpSeq.begin(), ::toupper);
		seqList[y] = tmpSeq;
	}
}

BEGIN_MESSAGE_MAP(CAlignDialog, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_ALIGN_SEQUENCES, OnItemchangingListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ALIGN_SEQUENCES, OnItemchangedListCtrl)
END_MESSAGE_MAP()


// CAlignDialog
void CAlignDialog::OnItemchangingListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(m_iImage == -1) {
		*pResult = FALSE;
		return;
	}

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	ASSERT(pNMLV);
	if((pNMLV->uNewState ^ pNMLV->uOldState) & LVIS_SELECTED) {
		LVITEM item;
		ZeroMemory(&item, sizeof(item));
		item.mask = LVIF_IMAGE;
		item.iItem = pNMLV->iItem;
		m_listCtrl.GetItem(&item);
		if (m_iImage != item.iImage) {
			*pResult = TRUE;
			if (item.iImage % 2 == 0) {
				++item.iImage;
				m_listCtrl.SetItem(&item);
			}
		}
	}
}

void CAlignDialog::OnItemchangedListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    ASSERT(pNMLV);
    if(pNMLV->uChanged & LVIF_STATE) {
        if((pNMLV->uNewState ^ pNMLV->uOldState) & LVIS_SELECTED) {
        	LVITEM item;
        	ZeroMemory(&item, sizeof(item));
        	item.mask = LVIF_IMAGE;
        	item.iItem = pNMLV->iItem;
        	m_listCtrl.GetItem(&item);
        	m_iImage = item.iImage;
        }
    }

    *pResult = FALSE;
}

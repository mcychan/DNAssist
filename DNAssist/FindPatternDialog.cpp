// FindPatternDialog.cpp :
//

#include "stdafx.h"
#include "DNAssist.h"
#include "DNAssistDoc.h"
#include "FindPatternDialog.h"

#include "model/DNASearch.h"
#include "model/GapsInPattern.h"

// CFindPatternDialog

IMPLEMENT_DYNAMIC(CFindPatternDialog, CDialog)

CFindPatternDialog::CFindPatternDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FINDPATTERN, pParent)
{

}

CFindPatternDialog::~CFindPatternDialog()
{
}

void CFindPatternDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINDPATTERN, m_findpattern);
	DDX_Control(pDX, IDC_NUMBERGAPS, m_gapnumberlist);
	DDX_Control(pDX, IDC_GAPSIZE, m_gapsizelist);
	DDX_Control(pDX, IDC_MISMATCH, m_mismatchlist);
	DDX_Check(pDX, IDC_FINDPATTERN_SECONDSTRAND, m_secondstrand);
	DDX_Text(pDX, IDC_FINDPATTERN_GAP, m_gapsword);
	DDX_Text(pDX, IDC_FINDPATTERN_NUCLEOTIDE, m_nucleotideword);
	DDX_Text(pDX, IDC_FINDPATTERN_MISMATCH, m_mismatchword);
	DDX_Control(pDX, IDC_FINDPATTERN_SECONDSTRAND, m_secondstrandbox);
}

void CFindPatternDialog::MakeTranspanency(double transparencyPercentage)
{
	// Enable WS_EX_LAYERED window extended style.
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(),
		GWL_EXSTYLE);
	SetWindowLong(GetSafeHwnd(),
		GWL_EXSTYLE,
		ExtendedStyle | WS_EX_LAYERED);

	// Select the transparency percentage.
	// The alpha will be calculated accordingly.

	// Set the alpha for transparency.
	// 0 is transparent and 255 is opaque.
	double fAlpha = transparencyPercentage * (255.0 / 100);
	BYTE byAlpha = static_cast<BYTE>(fAlpha);
	SetLayeredWindowAttributes(0, byAlpha, LWA_ALPHA);
}

BEGIN_MESSAGE_MAP(CFindPatternDialog, CDialog)
	ON_EN_CHANGE(IDC_FINDPATTERN, OnFindPatternChanged)
	ON_BN_CLICKED(IDC_FINDPATTERN_FIND, OnBnClickedFind)
	ON_BN_CLICKED(IDC_FINDPATTERN_FINDNEXT, OnBnClickedFindAgain)
END_MESSAGE_MAP()


// CFindPatternDialog
void CFindPatternDialog::OnFindPatternChanged()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDPATTERN);	
}

void CFindPatternDialog::AdjustListBoxes(char sequence_type)
{
	string strFind((LPCSTR)CStringA(m_findpattern));
	if (!m_seqValidator.IsValid(sequence_type, strFind, 0))
		m_findpattern.Left(m_findpattern.GetLength() - 1);
	
	int sequence_length = m_findpattern.GetLength();
	if(sequence_length) {
		m_gapnumberlist.EnableWindow(TRUE);
		m_gapsizelist.EnableWindow(TRUE);
		m_mismatchlist.EnableWindow(TRUE);
	}

	CString data;
	int maxgaps = sequence_length - 1;
	int maxsize = sequence_length;
	if(maxgaps == 0)
		maxsize = 0;
	int maxmisses = sequence_length;
	int oldgapindex = m_gapnumberlist.GetCurSel();
	m_gapnumberlist.ResetContent();
	for(int x = 0; x <= maxgaps;x++) {
		data.Format(_T("%d"), x);
		m_gapnumberlist.AddString(data);
	}

	if(oldgapindex > maxgaps)
		m_gapnumberlist.SetCurSel(maxgaps);
	else if (oldgapindex < 0)
		m_gapnumberlist.SetCurSel(maxgaps);
	else
		m_gapnumberlist.SetCurSel(oldgapindex);

	int index = m_gapnumberlist.GetCurSel();
	if(index >= 0) {
		CString temp;
		m_gapnumberlist.GetLBText(index, temp);
		number_of_gaps = _ttoi(temp);
		if(number_of_gaps == 1)
			m_gapsword = _T("gap");
		else
			m_gapsword = _T("gaps");
	}

	int oldsizeindex = m_gapsizelist.GetCurSel();
	m_gapsizelist.ResetContent();
	for(int y = 0; y <= maxsize; y++) {
		data.Format(_T("%d"), y);
		m_gapsizelist.AddString(data);
	}
	if(oldsizeindex > maxsize || oldsizeindex < 0)
		m_gapsizelist.SetCurSel(maxsize);
	else
		m_gapsizelist.SetCurSel(oldsizeindex);

	index = m_gapsizelist.GetCurSel();
	if(index >= 0) {
		CString temp;
		m_gapsizelist.GetLBText(index, temp);
		maximum_gap_size = _ttoi(temp);
		if(maximum_gap_size == 1) {
			if(sequence_type == 'D' || sequence_type == 'R')
				m_nucleotideword = _T("nucleotide maximum gap size");
			else
				m_nucleotideword = _T("amino acid maximum gap size");
		}
		else {
			if(sequence_type == 'D' || sequence_type == 'R')
				m_nucleotideword = _T("nucleotides maximum gap size");
			else
				m_nucleotideword = _T("amino acids maximum gap size");
		}
   }

	int oldmissindex = m_mismatchlist.GetCurSel();
	m_mismatchlist.ResetContent();
	for(int z = 0; z <= maxmisses; z++) {
		data.Format(_T("%d"), z);
		m_mismatchlist.AddString(data);
	}
	if(oldmissindex > maxmisses)
		m_mismatchlist.SetCurSel(maxmisses);
	else if (oldmissindex < 0)
		m_mismatchlist.SetCurSel(0);
	else
		m_mismatchlist.SetCurSel(oldmissindex);

	index = m_mismatchlist.GetCurSel();
	if(index >= 0) {
		CString temp;
		m_mismatchlist.GetLBText(index, temp);
		allow_misses = _ttoi(temp);
		if(allow_misses == 1)
			m_mismatchword = _T("mismatch");
		else
			m_mismatchword = _T("mismatches");
	}
	m_secondstrandbox.EnableWindow(sequence_type == 'D');
	UpdateData(FALSE);
}

void CFindPatternDialog::OnBnClickedFind()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDPATTERN_FIND);	
}

void CFindPatternDialog::FindCallback(const string& seq, char sequence_type, char seqForm, vector<int>& patternLocation, vector<int>& patternLength)
{
	if (sequence_type == 'D')
		m_secondstrandbox.EnableWindow(FALSE);
	int combinations;	
	patternLocation.clear();
	patternLength.clear();
	//HCURSOR old_cursor = ::SetCursor(::LoadCursor(0, IDC_WAIT));

	string reversecomplementpattern;
	string strFind((LPCSTR)CStringA(m_findpattern));
	int patternlength = strFind.length();
	if (m_secondstrand)
		m_seqConvertor.SequenceReverseComplement(strFind, reversecomplementpattern, 'D');

	vector<string> searchsequences, reversecomplementsearchsequences;
	vector<vector<int> > result;
	CGapsInPattern permuteobject(number_of_gaps, maximum_gap_size);
	combinations = permuteobject.PermuteSequence(strFind.c_str(), patternlength, result);
	permuteobject.FormatStringArray(strFind, combinations, searchsequences, "N", result);
	if (m_secondstrand)
		permuteobject.FormatStringArray(reversecomplementpattern, combinations, reversecomplementsearchsequences, "N", result);

	//Do the search
	CDNASearch searchobject(seq, seq.length(), sequence_type, seqForm);
	vector<int> temparray, temparray2;
	for (int x = 0; x < combinations; x++) {
		searchobject.FuzzySearchMultiple(searchsequences[x], searchsequences[x].length(), temparray, 0, allow_misses);
		temparray.pop_back();
		patternLocation.insert(patternLocation.end(), temparray.cbegin(), temparray.cend());
		temparray2.resize(temparray.size(), searchsequences[x].length());
		patternLength.insert(patternLength.end(), temparray2.cbegin(), temparray2.cend());
		if (m_secondstrand) {
			temparray.clear();
			searchobject.FuzzySearchMultiple(reversecomplementsearchsequences[x], reversecomplementsearchsequences[x].length(), temparray, 0, allow_misses);
			temparray.pop_back();
			temparray2.clear();
			temparray2.resize(temparray.size(), reversecomplementsearchsequences[x].length());
			//Get rid of any duplicates (palindromes)
			int members1 = temparray.size();
			int members2 = patternLocation.size();
			for (int x = 0; x < members1; x++) {
				auto hit = find(patternLocation.begin(), patternLocation.end(), temparray[x]);
				size_t index = distance(patternLocation.begin(), hit);
				if (hit != patternLocation.end() && index < members2) {
					temparray.erase(temparray.begin() + x);
					temparray2.erase(temparray2.begin() + x);
				}
			}
			patternLocation.insert(patternLocation.end(), temparray.cbegin(), temparray.cend());
			patternLength.insert(patternLength.end(), temparray2.cbegin(), temparray2.cend());
		}
	}	
}

void CFindPatternDialog::OnBnClickedFindAgain()
{
	UpdateData(TRUE);
	NotifyView(IDC_FINDPATTERN_FINDNEXT);
	
}

LRESULT CFindPatternDialog::NotifyView(UINT id)
{
	// Make it work in MFC SDI and MDI apps, with or without doc-view
	CFrameWnd * pFrame = (CFrameWnd*) ::AfxGetMainWnd();
	pFrame = pFrame->GetActiveFrame();

	CWnd * pView = pFrame->GetActiveView();

	if (!pView) pView = pFrame->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pView);

	LRESULT r = 0;
	if (pView) r = pView->SendMessage(WM_FIND_PATTERN_DIALOG, (WPARAM)id);
	return r;
}

#pragma once
#include "model/SeqConvertor.h"
#include "model/SeqValidator.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// CFindPatternDialog ��ܤ��

class CFindPatternDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindPatternDialog)
private:
	int number_of_gaps;
	int maximum_gap_size;
	int allow_misses;	
	BOOL m_secondstrand;	
	CString m_findpattern;	
	CString m_gapsword;
	CString m_nucleotideword;
	CString m_mismatchword;
	CButton m_secondstrandbox;
	CComboBox m_gapnumberlist;
	CComboBox m_gapsizelist;
	CComboBox m_mismatchlist;
	CSeqConvertor m_seqConvertor;
	CSeqValidator m_seqValidator;

public:
	CFindPatternDialog(CWnd* pParent = NULL);   // �зǫغc�禡
	void MakeTranspanency(double transparencyPercentage = 80.0);
	afx_msg void AdjustListBoxes(char sequence_type);
	afx_msg void FindCallback(const string& seq, char sequence_type, char seqForm, vector<int>& patternLocation, vector<int>& patternLength);
	inline CStringA GetFindString() {
		return CStringA(m_findpattern);
	}
	virtual ~CFindPatternDialog();

	enum { IDD = IDD_FINDPATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	LRESULT NotifyView(UINT id);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnFindPatternChanged();	
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedFindAgain();
	
};

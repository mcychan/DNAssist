#pragma once
#include "model/CodonTable.h"
#include "model/SeqValidator.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
using namespace std;

// CFindORF ��ܤ��

class CFindORFDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindORFDialog)

	// Attributes
private:
	int currentdisplayedindex;	
	CSeqValidator m_SeqValidator;

public:
	CFindORFDialog(CWnd* pParent = NULL);   // �зǫغc�禡
	void MakeTranspanency(double transparencyPercentage = 80.0);
	inline BOOL IsGlobal() {
		return !m_global;
	}
	void SearchForORFs(CCodonTable* pCodontable, const string& sequence, vector<CPoint>& points, char seqType, char seqForm);
	virtual ~CFindORFDialog();

	enum { IDD = IDD_FINDORF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	void GeneratePointArrayOfORFs(const vector<int>& startpositions, const vector<int>& stoppositions, vector<CPoint>& points, int sequencelength);
	LRESULT NotifyView(UINT id);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedFind();
	afx_msg void OnBnClickedFindAgain();
	BOOL m_global;
};

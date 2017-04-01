#pragma once
#include <memory>
#include <string>

using namespace std;

#include "model/DnaHelixParameters.h"

// message sent to the parent window which signals that the user changed the caret position
#define WM_DATA_CHANGED	(WM_USER+0x1)

// CHelixParamsDialog 對話方塊

class CHelixParamsDialog : public CDialog
{
	DECLARE_DYNAMIC(CHelixParamsDialog)

public:
	CHelixParamsDialog(const string& sequence, const char seqType = 'D', CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CHelixParamsDialog();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELIXPARAMS };
#endif

private:
	char type;
	int seqlength;
	CString degreescelsius;
	CString m_Gnucleootides;
	CString m_Anucleootides;
	CString m_Tnucleootides;
	CString m_Cnucleootides;
	CString m_GPlusCPercentage;
	CString m_APlusTPercentage;
	CString m_Length;
	CString m_melt;
	CString m_anneal;
	CString m_MolecularWeightSingle;
	CString m_MolecularWeightDouble;
	CString m_AbsorbanceDoubleMolar;
	CString m_AbsorbanceDoubleMilligram;
	CString m_AbsorbanceSingleMolar;
	CString m_AbsorbanceSingleMilligram;
	CString m_APlusTLabel;
	CString m_Tlabel;
	unique_ptr<CDnaHelixParameters> helixparameter;

protected:
	void Calculate();
	virtual BOOL OnInitDialog();
	CString MakeCelsiusString(int degrees);
	float ConvertCaloriesToJoules(float number);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
};

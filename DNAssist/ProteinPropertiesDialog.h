#pragma once
#include "model/ProteinProperties.h"

#include <memory>
#include <string>
using namespace std;

// message sent to the parent window which signals that the user changed the caret position
#define WM_DATA_CHANGED	(WM_USER+0x1)

// CProteinPropertiesDialog 對話方塊

class CProteinPropertiesDialog : public CDialog
{
	DECLARE_DYNAMIC(CProteinPropertiesDialog)

public:
	CProteinPropertiesDialog(const string& sequence, CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CProteinPropertiesDialog();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTEINPROPERTIES };
#endif

protected:
	string proteinsequence;
	CString m_Length;
	CString m_molecular_weight;
	CString m_pI;
	CString m_A280;
	CString m_ChargeAtpH;
	CString m_E280;

	void LoadNumberOfResiduesControls();
	void LoadMolePercentageControls();
	void LoadMolecularWeightControl();
	void LoadpIControl();
	void LoadChargeAtpHControl();
	void LoadMolarAbsorptivityControl();
	void LoadEOnePercentControl();
	void Calculate();
	unique_ptr<CProteinProperties> propertiesobject;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
};

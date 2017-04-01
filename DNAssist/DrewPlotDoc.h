
// DrewPlotDoc.h : interface of the CHydroDoc class
//

#pragma once
#include "model/DrewCalladine.h"

#include <memory>
#include <string>
#include <vector>

using namespace std;

class CDrewPlotDoc : public CDocument
{
protected: // create from serialization only
	CDrewPlotDoc();
	DECLARE_DYNCREATE(CDrewPlotDoc)

	// Attributes
private:
	char m_plotType;
	int m_nLength;
	vector<float> resultarray;

	unique_ptr<CDrewCalladine> m_pDrewCalladine;
	CWinThread *m_pThread;
	volatile bool m_bStop;
	void Stop();	// Stop working thread.
	BOOL Calculate();	
	static UINT ThreadProc(LPVOID pParam);

	// Operations
public:
	size_t m_nStartBase;
	CString Xaxistitle, Yaxistitle;
	BOOL CalculateAntigenicity(const string& dataStr, int plotType);
	BOOL CalculateHydrophobicity(const string& dataStr, int plotType);
	BOOL CalculateHydrophilicity(const string& dataStr, int plotType);
	void CalculateNucleosome(const string& dataStr, int* pPercentCompleted, int selStart = 0);
	inline char GetPlotType() const {
		return m_plotType;
	}
	inline void SetPlotType(char plotType) {
		m_plotType = plotType;
	}
	inline vector<float>* GetData() {
		return &resultarray;
	}
	inline int GetLength() const {
		return m_nLength > -1 ? m_nLength : resultarray.size();
	}
	inline bool IsCalculating() const {
		return m_pThread != NULL;
	}

	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetModifiedFlag(BOOL bModified);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CDrewPlotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DeleteContents();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

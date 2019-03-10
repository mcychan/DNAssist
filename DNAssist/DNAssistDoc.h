
// DNAssistDoc.h : interface of the CDNAssistDoc class
//

#pragma once
#include <memory>
#include <string>

#include "model/CodonTable.h"
#include "model/FileTypeDetector.h"
#include "model/FileDecoder.h"
#include "model/FileEncoder.h"

class CDNAssistDoc : public CDocument
{
protected: // create from serialization only
	CDNAssistDoc();
	DECLARE_DYNCREATE(CDNAssistDoc)

// Attributes
private:
	char m_form, m_seqType;	
	TCHAR szInitDir[MAX_PATH];
	string m_dataStr, m_comment;
	CFileTypeDetector m_fileTypeDetector;
	unique_ptr<CCodonTable> m_pCodontable;

// Operations
public:
	void SetData(string& dataStr, char seqType);
	inline CCodonTable* GetCodonTable() const {
		return m_pCodontable.get();
	}
	inline char GetSeqForm() const {
		return m_form;
	}
	inline size_t GetSeqLength() const {
		return m_dataStr.length();
	}
	inline char GetSeqType() const {
		return m_seqType;
	}
	inline CString GetSeqTypeDescr() const {
		switch (m_seqType) {
		case 'R':
			return _T("RNA");
		case 'P':
				return _T("Protein");
		case 'G':
			return _T("Degenerate DNA");
		}
		return _T("DNA");
	}
	inline CString GetFormDescr() const {
		return m_form == 'L' ? _T("Linear") : _T("Circular");
	}
	inline void SetSeqType(char seqType) {
		m_seqType = seqType;
	}
	inline string* GetData() {
		return &m_dataStr;
	}
	inline BOOL IsCodonTableValid() {
		return GetCodonTable()->IsCodonTableValid();
	}

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void SetModifiedFlag(BOOL bModified);
	virtual void Serialize(CArchive& ar);
	void AddToHistory();

// Implementation
public:
	virtual ~CDNAssistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DeleteContents();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChangecaseUppercase();
	afx_msg void OnChangecaseLowercase();
	afx_msg void OnUpdateConvertDna2rna(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertRna2dna(CCmdUI *pCmdUI);
	afx_msg void OnConvertCircularize();
	afx_msg void OnConvertLinearize();
	afx_msg void OnUpdateConvertCircularize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertLinearize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertDna2pro(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertRna2pro(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConvertReversetranslate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTranslate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalyzeFindorfs(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePropertiesPhysicochemical(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePropertiesHydrophobicity(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePropertiesHydrophilicity(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePropertiesAntigenicity(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalyzeTranscriptionfactors(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalyzeNucleosomepositions(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateAnalyzeList(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalyzeRestrictionmapping(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnalyzeFindpattern(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
};

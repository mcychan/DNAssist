
// ResultDoc.h : interface of the CResultDoc class
//

#pragma once
#include <string>
#include <vector>

using namespace std;

#include "model/AnalyzeDNAList.h"
#include "model/CodonTable.h"
#include "model/EnzymeList.h"
#include "model/RestrictionCutList.h"

class CResultDoc : public CDocument
{
protected: // create from serialization only
	CResultDoc();
	DECLARE_DYNCREATE(CResultDoc)

	// Attributes
private:
	char m_form, m_seqType;		
	int selectionstart, selectionlength;
	string sequence;
	CCodonTable* codontable;
	vector<int> sortedindexes;
	unique_ptr<CAnalyzeDNAList> analyzednalist;
	CRestrictionCutList* result;

	// Operations
public:
	size_t m_nStartBase;
	int analysisflags;
	short displayflags;
	int phase;
	int GetNumberNoCutters();
	size_t GetFirstUniqueCutter();
	int GetNumberUniqueCutters();
	bool RestrictionMapping(const string& seq, char sequence_type, char seqForm, CEnzymeList* enzymelist, CRestrictionCutList* resultarray);
	bool TranscriptionFactors(const string& seq, char sequence_type, char seqForm, CEnzymeList* factorlist, CRestrictionCutList* resultarray);
	void SetData(const string& dataStr,
		int sel_start, int sel_length,
		short dispflags, int enzymeflags,
		CRestrictionCutList* pResult,
		char seq_type = 'D', char seq_form = 'L',
		CCodonTable* pCodon = 0,
		int translationphase = 1);

	CAnalyzeDNAList* GetAnalyzeDNAList(const CPoint& chardim, int characters_per_row, int lines_per_page, vector<int>& pagebreaks);
	inline char GetSeqForm() const {
		return m_form;
	}
	inline char GetSeqType() const {
		return m_seqType;
	}
	inline size_t GetSeqLength() const {
		return sequence.length();
	}
	inline CRestrictionCutList* GetData() {
		return result;
	}
	inline CRestrictionEnzymeCut* GetRow(int index) {
		return result->GetPointer(sortedindexes[index]);
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

	// Implementation
public:
	virtual ~CResultDoc();
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

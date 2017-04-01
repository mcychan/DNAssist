
// AlignDoc.h : interface of the CAlignDoc class
//

#pragma once

#include "model/MultAlignment.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class CAlignDoc : public CDocument
{
protected: // create from serialization only
	CAlignDoc();
	DECLARE_DYNCREATE(CAlignDoc)

	// Attributes
private:
	char m_seqType;
	int alignmentlength, numberofsequences;
	vector<CString> m_sequencenames;
	vector<vector<short> > backgroundcolorarray;
	vector<vector<int> > nucleicacidmatrix, aminoacidmatrix, conservedaminoacidmatrix;

	CWinThread *m_pThread;
	volatile bool m_bStop;
	void Stop();	// Stop working thread.
	BOOL Calculate();
	static UINT ThreadProc(LPVOID pParam);

	CMultAlignment m_SeqAlignment;
	inline string GetMtrxname(int proteinweightmatrix) {
		if (proteinweightmatrix == 0)
			return "blosum";
		if (proteinweightmatrix == 1)
			return "pam";
		if (proteinweightmatrix == 2)
			return "gonnet";
		if (proteinweightmatrix == 3)
			return "id";
		return "";
	}

	// Operations
public:
	void CalculateAlignments(char seqType, int* pPercent, wstring* pCaption, const vector<CString>* pNameList, vector<string>& seqList);
	int GetLengthOfSequence();
	int GetNumberOfSequences();
	int GetSequenceNumber(const string& startofstring, int offset, int firstvalue = 0);
	int GetLongestNameString();
	int GetLongestPositionString();	
	inline char GetSeqType() const {
		return m_seqType;
	}
	inline string* GetAlignment(int index) {
		if(index < m_SeqAlignment.result.size())
			return &m_SeqAlignment.result[index];
		return nullptr;
	}
	inline CString* GetSeqName(int index) {
		if(index < m_sequencenames.size())
			return &m_sequencenames[index];
		return nullptr;
	}
	inline int GetSortOrder(int index) {
		return m_SeqAlignment.order[index];
	}
	inline short GetBgColor(int row, int column) {
		if(row < backgroundcolorarray.size())
			return backgroundcolorarray[row][column];
		return 0;
	}
	inline bool IsCalculating() const {
		//return false;
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
	virtual ~CAlignDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void Initialise();
	virtual void DeleteContents();	
	int NumberOfEntriesInColumn(const vector<vector<int> >& array, int column);
	int GetColumnWithMostEntries(const vector<vector<int> >& array);
	void IdentifyConservedLetters(int index);
	void IdentifyIdenticalLetters(vector<vector<int> >& matrix, int index);
	int GetDNABaseIndex(char letter);
	int GetRNABaseIndex(char letter);
	int GetConservationGroup(char letter);
	int GetAminoAcidIndex(char letter);
	void IdentifyIdenticalAndConservatives();
	void RemoveLeadingAndTrailingHyphens();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

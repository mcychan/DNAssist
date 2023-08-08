
// DNAssist.h : main header file for the DNAssist application
//
#pragma once
#include "VersionNo.h"

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "AlignDoc.h"
#include "DrewPlotDoc.h"
#include "ResultDoc.h"
#include "MultDocManager.h"
#include "model/FormatTransfer.h"

#include <memory>

using namespace std;

#define WM_USER_THREAD_COMPLETE (WM_USER+1236)
#define WM_USER_THREAD_UPDATE_PROGRESS (WM_USER+1237)
#define WM_FIND_DIALOG	WM_APP + CFindDialog::IDD
#define WM_FIND_ORF_DIALOG	WM_APP + CFindORFDialog::IDD
#define WM_FIND_PATTERN_DIALOG	WM_APP + CFindPatternDialog::IDD

// CDNAssistApp:
// See DNAssist.cpp for the implementation of this class
//

class CDNAssistApp : public CWinApp
{
private:
	BOOL GeMajorVersion(DWORD majorVersion) const;
public:
	CDNAssistApp();
	inline const char* GetApplicationName() {
		return "DNAssist";
	}
	inline const char* GetVersion() {
		return VER_FILE_VERSION_STR;
	}
	inline int GetAlignmentId() {
		return ++alignmentId;
	}
	inline int GetNucleosomeId() {
			return ++nucleosomeId;
	}
	inline int GetRestrictionId() {
		return ++restrictionId;
	}
	inline int GetSeqListId() {
		return ++seqListId;
	}
	inline int GetTransfacId() {
		return ++transfacId;
	}
	inline int GetTranslationId() {
		return ++translationId;
	}
	CString GetAppLoc();
	double GetSystemScaleFactor();
	CStatusBar* m_pStatusBar;
	int ConversionOutputTransferBuffer, TransFacOptions;
	CString sequenceDirectory, codonPath, enzymePath, transfacPath;
	CString editorFontFace, printFontFace, hydrophilicresidues;
	//! Contains total number of groups
	size_t numberOfGroups;
	size_t hydrophobicityOptions, hydrophilicityOptions, antigenicityOptions, undoLevels;

	inline AlignOptionsBuffer* GetAlignOptionsBuffer(BOOL create = FALSE) {
		if(create)
			m_alignOptionsBuffer = make_unique<AlignOptionsBuffer>();
		return m_alignOptionsBuffer.get();
	}
	inline void SetAlignOptionsBuffer(AlignOptionsBuffer* pBuffer) {
		m_alignOptionsBuffer.reset(pBuffer);
	}
	inline EnzymeOptionsTransferBuffer* GetEnzymeOptionsTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_enzymeOptionsTransferBuffer = make_unique<EnzymeOptionsTransferBuffer>();
		return m_enzymeOptionsTransferBuffer.get();
	}
	inline void SetEnzymeOptionsTransferBuffer(EnzymeOptionsTransferBuffer* pBuffer) {
		m_enzymeOptionsTransferBuffer.reset(pBuffer);
	}
	inline PrintFontTransferBuffer* GetEditorFontTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_editorFontTransferBuffer = make_unique<PrintFontTransferBuffer>();
		return m_editorFontTransferBuffer.get();
	}
	inline void SetEditorFontTransferBuffer(PrintFontTransferBuffer* pBuffer) {
		m_editorFontTransferBuffer.reset(pBuffer);
	}
	inline FindORFTransferBuffer* GetFindORFTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_findORFTransferBuffer = make_unique<FindORFTransferBuffer>();
		return m_findORFTransferBuffer.get();
	}
	inline void SetFindORFTransferBuffer(FindORFTransferBuffer* pBuffer) {
		m_findORFTransferBuffer.reset(pBuffer);
	}
	inline FormatTransferBuffer* GetFormatTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_formatTransferBuffer = make_unique<FormatTransferBuffer>();
		return m_formatTransferBuffer.get();
	}
	inline void SetFormatTransferBuffer(FormatTransferBuffer* pBuffer) {
		m_formatTransferBuffer.reset(pBuffer);
	}
	inline PrintFontTransferBuffer* GetPrintFontTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_printFontTransferBuffer = make_unique<PrintFontTransferBuffer>();
		return m_printFontTransferBuffer.get();
	}
	inline void SetPrintFontTransferBuffer(PrintFontTransferBuffer* pBuffer) {
		m_printFontTransferBuffer.reset(pBuffer);
	}
	inline PrintFormatTransferBuffer* GetPrintFormatTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_printFormatTransferBuffer = make_unique<PrintFormatTransferBuffer>();
		return m_printFormatTransferBuffer.get();
	}
	inline void SetPrintFormatTransferBuffer(PrintFormatTransferBuffer* pBuffer) {
		m_printFormatTransferBuffer.reset(pBuffer);
	}
	inline TransFacFormatTransferBuffer* GetTransFacFormatTransferBuffer(BOOL create = FALSE) {
		if(create)
			m_transFacFormatTransferBuffer = make_unique<TransFacFormatTransferBuffer>();
		return m_transFacFormatTransferBuffer.get();
	}
	inline void SetTransFacFormatTransferBuffer(TransFacFormatTransferBuffer* pBuffer) {
		m_transFacFormatTransferBuffer.reset(pBuffer);
	}

	CDrewPlotDoc* OnNewDrewPlot();
	CResultDoc* OnNewResultDoc();
	CAlignDoc* OnNewAlignDoc();

protected:
	int alignmentId = -1, nucleosomeId = -1, restrictionId = -1, seqListId = -1, transfacId = -1, translationId = -1;
	double m_scale = 0.0;
	unique_ptr<AlignOptionsBuffer> m_alignOptionsBuffer;
	unique_ptr<EnzymeOptionsTransferBuffer> m_enzymeOptionsTransferBuffer;
	unique_ptr<PrintFontTransferBuffer> m_editorFontTransferBuffer;
	unique_ptr<FindORFTransferBuffer> m_findORFTransferBuffer;
	unique_ptr<FormatTransferBuffer> m_formatTransferBuffer;
	unique_ptr<PrintFontTransferBuffer> m_printFontTransferBuffer;
	unique_ptr<PrintFormatTransferBuffer> m_printFormatTransferBuffer;
	unique_ptr<TransFacFormatTransferBuffer> m_transFacFormatTransferBuffer;
	unique_ptr<TransFacOptionsTransferBuffer> m_transFacOptionsTransferBuffer;

	CDocTemplate* m_pAlignViewTemplate;
	CDocTemplate* m_pDrewPlotTemplate;
	CDocTemplate* m_pDNAViewTemplate;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CDocument* GetActiveDocument();
	int GetDisplayFormatOptions();
	int GetTransFacFormatOptions();
	int GetEnzymeAttributeOptions();
	int GetTransFacOptions();
	inline bool IsWin7() const
	{
		return GeMajorVersion(6);
	}

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNewDna();
	afx_msg void OnNewRna();
	afx_msg void OnNewProtein();
	afx_msg void OnNewDegeneratedna();
	afx_msg void OnHelpEntrez();
	afx_msg void OnHelpBlast();
	afx_msg void OnHelpContents();	
};

extern CDNAssistApp theApp;

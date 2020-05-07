#pragma once

#include "CitiesView.h"

////////////////////////////////////////////////////////////
// CCitiesDialog

/// <summary>Клас диалог за CITIES</summary>
class CCitiesDialog : public CDialog
{
	// Macros
	// ----------------

	DECLARE_DYNAMIC(CCitiesDialog)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CCitiesDialog(CITIES& recCity, CCitiesDocument::Operations eOperation);   // constructor
	virtual ~CCitiesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES_DIALOG };
#endif

	// MFC Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	virtual BOOL OnInitDialog() override;

	// MFC Message Handlers
	// ----------------
protected:
	afx_msg virtual void OnBnClickedBtnCitiesOk();
	afx_msg virtual void OnBnClickedBtnCitiesCancel();

	// Members
	// -------------
protected:
	CITIES& m_recCity;
	CCitiesDocument::Operations m_eOperation;
	
	CEdit m_oEdbName;
	CEdit m_oEdbRegion;
	CButton m_oBtnOk;
	CButton m_oBtnCancel;
};
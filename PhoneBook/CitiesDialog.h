#pragma once

#include "CitiesView.h"
#include "Utilities.h"

#define CITY_NAME_MIN_LENGTH 3
#define CITY_REGION_MIN_LENGTH 3

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
	CCitiesDialog(CITIES& recCity, Operations eOperation);   // constructor
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
	void OnOK() override;

	// Methods
	// ----------------
private:
	INT ValidateCityName(CString& strName, CString* strResultingMessage = NULL);
	INT ValidateCityRegion(CString& strRegion, CString* strResultingMessage = NULL);
	void RemoveOldWarnings();

	// Members
	// -------------
private:
	CITIES& m_recCity;
	Operations m_eOperation;

	CEdit m_edbName;
	CEdit m_edbRegion;
	CButton m_btnOk;
	
	CStatic m_sttCitiesNameInfo;
	CStatic m_sttCitiesRegionInfo;
};

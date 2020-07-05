#pragma once

#include "PhoneTypesView.h"
#include "Utilities.h"
#include "resource.h"

#define PHONE_TYPES_TYPE_MIN_LENGTH 3

////////////////////////////////////////////////////////////
// CPhoneTypesDialog

/// <summary>Клас диалог за PHONE_TYPES</summary>
class CPhoneTypesDialog : public CDialog
{
	// Macros
	// ----------------

	DECLARE_DYNAMIC(CPhoneTypesDialog)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesDialog(PHONE_TYPES& recPhoneType, Utilities::Operations eOperation);   // constructor
	virtual ~CPhoneTypesDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_TYPES_DIALOG };
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
	INT ValidatePhoneType(CString& strType, CString* strResultingMessage = NULL);
	void RemoveOldWarnings();

	// Members
	// -------------
private:
	PHONE_TYPES& m_recPhoneType;
	Utilities::Operations m_eOperation;

	CEdit m_edbType;

	CStatic m_sttPhoneTypesTypeValidationInfo;
};

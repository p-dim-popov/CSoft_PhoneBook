#pragma once
#include "Structures.h"
#include "Utilities.h"

///////////////////////////////////////////////////////////////////
// CPhoneNumbersDialog dialog

class CPhoneNumbersDialog : public CDialog
{
	DECLARE_DYNAMIC(CPhoneNumbersDialog)
	DECLARE_MESSAGE_MAP()

public:
	CPhoneNumbersDialog(PHONE_NUMBERS& recPhoneNumber, PHONE_TYPES& recPhoneType, CPhoneTypesArray& oPhoneTypesArray);
	// standard constructor
	virtual ~CPhoneNumbersDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_NUMBERS_DIALOG };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	virtual BOOL OnInitDialog() override;

	// MFC Message Handlers
	// ----------------
protected:
	void OnOK() override;

	// Methods
	// ----------------
private:
	INT ValidatePhoneType(CString& strType);
	INT ValidatePhoneNumber(CString& strNumber);
	void RemoveOldWarnings();

	// Members
	// -------------
private:	
	PHONE_TYPES& m_recPhoneType;
	PHONE_NUMBERS& m_recPhoneNumber;
	CPhoneTypesArray& m_oPhoneTypesArray;
	
	CComboBox m_cmbPhoneNumberType;
	CEdit m_edbPhoneNumberNumber;
	CStatic m_sttPhoneNumbersTypeValidationInfo;
	CStatic m_sttPhoneNumberNumberValidationInfo;
};

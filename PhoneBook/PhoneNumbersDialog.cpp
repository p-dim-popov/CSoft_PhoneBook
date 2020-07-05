// PhoneNumbersDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "PhoneNumbersDialog.h"
#include "afxdialogex.h"
#include "Structures.h"

#define PHONE_NUMBER_MIN_LENGTH 6

// CPhoneNumbersDialog dialog

IMPLEMENT_DYNAMIC(CPhoneNumbersDialog, CDialog)

CPhoneNumbersDialog::CPhoneNumbersDialog(PHONE_NUMBERS& recPhoneNumber, PHONE_TYPES& recPhoneType, CPhoneTypesArray& oPhoneTypesArray)
	: CDialog(IDD_PHONE_NUMBERS_DIALOG),
	m_recPhoneType(recPhoneType),
	m_recPhoneNumber(recPhoneNumber),
	m_oPhoneTypesArray(oPhoneTypesArray)
{

}

CPhoneNumbersDialog::~CPhoneNumbersDialog()
{
}

void CPhoneNumbersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PHONE_NUMBERS_TYPE, m_cmbPhoneNumberType);
	DDX_Control(pDX, IDC_EDB_PHONE_NUMBERS_NUMBER, m_edbPhoneNumberNumber);
	DDX_Control(pDX, IDC_STT_PHONE_NUMBERS_TYPE_VALIDATION_INFO, m_sttPhoneNumbersTypeValidationInfo);
	DDX_Control(pDX, IDC_STT_PHONE_NUMBERS_NUMBER_VALIDATION_INFO, m_sttPhoneNumberNumberValidationInfo);
}

BOOL CPhoneNumbersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// populate phone types in combo box
	for (INT_PTR i = 0; i < m_oPhoneTypesArray.GetSize(); i++)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(i);
		m_cmbPhoneNumberType.AddString(pPhoneType->szType);
	}

	m_cmbPhoneNumberType.SelectString(0, m_recPhoneType.szType);
	m_edbPhoneNumberNumber.SetWindowText(m_recPhoneNumber.szPhone);
	m_edbPhoneNumberNumber.SetLimitText(PHONE_NUMBERS_PHONE_LENGTH);

	this->SetWindowText(_T("Edit phone number"));

	return TRUE;
}

void CPhoneNumbersDialog::OnOK()
{
	RemoveOldWarnings();
	
	CString strType;
	CString strNumber;

	if (ValidatePhoneType(strType) ||
		ValidatePhoneNumber(strNumber))
		return;

	m_recPhoneNumber.SetPhone(strNumber.GetString());
	m_recPhoneType.SetType(strType.GetString());

	CDialog::OnOK();
}

INT CPhoneNumbersDialog::ValidatePhoneType(CString& strType)
{
	m_cmbPhoneNumberType.GetWindowText(strType);
	if (strType.IsEmpty())
	{
		m_sttPhoneNumbersTypeValidationInfo.ShowWindow(SW_SHOW);
		m_sttPhoneNumbersTypeValidationInfo.SetWindowText(_T("selection cannot be empty"));
		return 1;
	}

	return 0;
}

INT CPhoneNumbersDialog::ValidatePhoneNumber(CString& strNumber)
{
	m_edbPhoneNumberNumber.GetWindowText(strNumber);

	Utilities::CStringValidator oValidator(PHONE_NUMBER_MIN_LENGTH, _T("`.,_!"));

	const INT nValidationsResult = oValidator.ValidateString(
		strNumber,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasLetters
	);

	if (nValidationsResult)
	{
		m_edbPhoneNumberNumber.SetFocus();
		m_sttPhoneNumberNumberValidationInfo.ShowWindow(SW_SHOW);
		m_sttPhoneNumberNumberValidationInfo.SetWindowText(oValidator.GetValidationMessage());
	}

	return nValidationsResult;
}

void CPhoneNumbersDialog::RemoveOldWarnings()
{
	m_sttPhoneNumbersTypeValidationInfo.ShowWindow(SW_HIDE);
	m_sttPhoneNumberNumberValidationInfo.ShowWindow(SW_HIDE);
}


BEGIN_MESSAGE_MAP(CPhoneNumbersDialog, CDialog)
END_MESSAGE_MAP()


// CPhoneNumbersDialog message handlers

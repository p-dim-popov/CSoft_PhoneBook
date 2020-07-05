// PhoneTypesDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneTypesDialog.h"


/////////////////////////////////////////////////////////////////////
// CPhoneTypesDialog
#pragma region CPhoneTypesDialog

IMPLEMENT_DYNAMIC(CPhoneTypesDialog, CDialog)

CPhoneTypesDialog::CPhoneTypesDialog(PHONE_TYPES& recPhoneType, Utilities::Operations eOperation)
	: CDialog(IDD_PHONE_TYPES_DIALOG), m_recPhoneType(recPhoneType), m_eOperation(eOperation)
{
}

CPhoneTypesDialog::~CPhoneTypesDialog()
{
}

void CPhoneTypesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDB_PHONE_TYPES_TYPE, m_edbType);
	DDX_Control(pDX, IDC_STT_PHONE_TYPES_TYPE_VALIDATION_INFO, m_sttPhoneTypesTypeValidationInfo);
}

BEGIN_MESSAGE_MAP(CPhoneTypesDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CPhoneTypesDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &CPhoneTypesDialog::OnCancel)
END_MESSAGE_MAP()

BOOL CPhoneTypesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbType.SetWindowText(m_recPhoneType.szType);

	m_edbType.SetLimitText(PHONE_TYPES_TYPE_LENGTH);

	switch (m_eOperation)
	{
	case Utilities::OperationsCreate:
		this->SetWindowText(_T("Add phone type"));

		break;
	case Utilities::OperationsUpdate:
		this->SetWindowText(_T("Update phone type"));

		break;
	case Utilities::OperationsRead:
		this->SetWindowText(_T("PhoneType info"));

		m_edbType.EnableWindow(FALSE);
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void CPhoneTypesDialog::OnOK()
{
	RemoveOldWarnings();

	CString strNewType;

	m_edbType.GetWindowText(strNewType);

	if (m_eOperation == Utilities::OperationsRead ||
		!StrCmp(strNewType, m_recPhoneType.szType) &&
		m_eOperation != Utilities::OperationsCreate)
	{
		CDialog::OnOK();
		return;
	}

	CString strTypeValidationMessage;
	const INT nNameValidation(ValidatePhoneType(strNewType, &strTypeValidationMessage));
	if (nNameValidation)
	{
		m_edbType.SetFocus();
		m_sttPhoneTypesTypeValidationInfo.ShowWindow(SW_SHOW);
		m_sttPhoneTypesTypeValidationInfo.SetWindowText(strTypeValidationMessage);

	}

	if (!!nNameValidation)
		return;

	m_recPhoneType.SetType(strNewType);

	CDialog::OnOK();
}

INT CPhoneTypesDialog::ValidatePhoneType(CString& strType, CString* strResultingMessage)
{
	Utilities::CStringValidator oValidator(PHONE_TYPES_TYPE_MIN_LENGTH, _T(",*+"));

	const INT nValidationsResult = oValidator.ValidateString(
		strType,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasDigits
	);

	if (strResultingMessage && nValidationsResult)
		*strResultingMessage = oValidator.GetValidationMessage();

	return nValidationsResult;
}

void CPhoneTypesDialog::RemoveOldWarnings()
{
	m_sttPhoneTypesTypeValidationInfo.ShowWindow(SW_HIDE);
}


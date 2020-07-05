// CitiesDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "CitiesDialog.h"
#include "afxdialogex.h"


/////////////////////////////////////////////////////////////////////
// CCitiesDialog
#pragma region CCitiesDialog

IMPLEMENT_DYNAMIC(CCitiesDialog, CDialog)

CCitiesDialog::CCitiesDialog(CITIES& recCity, Utilities::Operations eOperation)
	: CDialog(IDD_CITIES_DIALOG), m_recCity(recCity), m_eOperation(eOperation)
{
}

CCitiesDialog::~CCitiesDialog()
{
}

void CCitiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_CITIES_REGION, m_edbRegion);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_STT_CITIES_NAME_INFO, m_sttCitiesNameInfo);
	DDX_Control(pDX, IDC_STT_CITIES_REGION_INFO, m_sttCitiesRegionInfo);
}

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCitiesDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &CCitiesDialog::OnCancel)
END_MESSAGE_MAP()

BOOL CCitiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbName.SetWindowText(m_recCity.szName);
	m_edbRegion.SetWindowText(m_recCity.szRegion);

	m_edbName.SetLimitText(CITIES_NAME_LENGTH);
	m_edbRegion.SetLimitText(CITIES_REGION_LENGTH);

	switch (m_eOperation)
	{
	case Utilities::OperationsCreate:
		this->SetWindowText(_T("Add city"));

		break;
	case Utilities::OperationsUpdate:
		this->SetWindowText(_T("Update city"));

		break;
	case Utilities::OperationsRead:
		this->SetWindowText(_T("City info"));

		m_edbName.EnableWindow(FALSE);
		m_edbRegion.EnableWindow(FALSE);

		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void CCitiesDialog::OnOK()
{
	RemoveOldWarnings();

	CString strNewName;
	CString strNewRegion;

	m_edbName.GetWindowText(strNewName);
	m_edbRegion.GetWindowText(strNewRegion);

	if (m_eOperation == Utilities::OperationsRead ||
		!StrCmp(strNewName, m_recCity.szName) &&
		!StrCmp(strNewRegion, m_recCity.szRegion) &&
		m_eOperation != Utilities::OperationsCreate)
	{
		CDialog::OnOK();
		return;
	}

	CString strNameValidationMessage;
	const INT nNameValidation(ValidateCityName(strNewName, &strNameValidationMessage));
	if (nNameValidation)
	{
		m_edbName.SetFocus();
		m_sttCitiesNameInfo.ShowWindow(SW_SHOW);
		m_sttCitiesNameInfo.SetWindowText(strNameValidationMessage);

	}

	CString strRegionValidationMessage;
	const INT nRegionValidation(ValidateCityRegion(strNewRegion, &strRegionValidationMessage));
	if (nRegionValidation)
	{
		m_edbRegion.SetFocus();
		m_sttCitiesRegionInfo.ShowWindow(SW_SHOW);
		m_sttCitiesRegionInfo.SetWindowText(strRegionValidationMessage);
	}

	if (!!nNameValidation || !!nRegionValidation)
		return;

	m_recCity.SetName(strNewName);
	m_recCity.SetRegion(strNewRegion);

	CDialog::OnOK();
}

INT CCitiesDialog::ValidateCityName(CString& strName, CString* strResultingMessage)
{
	Utilities::CStringValidator oValidator(CITY_NAME_MIN_LENGTH, _T(".,_!*+"));

	const INT nValidationsResult = oValidator.ValidateString(
		strName,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasDigits
	);

	if (strResultingMessage && nValidationsResult)
		*strResultingMessage = oValidator.GetValidationMessage();

	return nValidationsResult;
}

INT CCitiesDialog::ValidateCityRegion(CString& strRegion, CString* strResultingMessage)
{
	Utilities::CStringValidator oValidator(CITY_NAME_MIN_LENGTH, _T(".,_!*+"));

	const INT nValidationsResult = oValidator.ValidateString(
		strRegion,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength
	);

	if (strResultingMessage && nValidationsResult)
		*strResultingMessage = oValidator.GetValidationMessage();

	return nValidationsResult;
}

void CCitiesDialog::RemoveOldWarnings()
{
	m_sttCitiesNameInfo.ShowWindow(SW_HIDE);
	m_sttCitiesRegionInfo.ShowWindow(SW_HIDE);
}


//// PersonsDialog.cpp : implementation file
////
//
#include "pch.h"
#include "PhoneBook.h"
#include "PersonsDialog.h"
#include "afxdialogex.h"
#include "PhoneNumbersDialog.h"


#define PHONE_NUMBER_TYPE_COLUMN 0
#define PHONE_NUMBER_NUMBER_COLUMN 1
#define PHONE_NUMBER_TYPE_COLUMN_LABEL _T("Phone Type")
#define PHONE_NUMBER_NUMBER_COLUMN_LABEL _T("Phone Number")
#define PHONE_NUMBER_TYPE_COLUMN_WIDTH 100
#define PHONE_NUMBER_NUMBER_COLUMN_WIDTH 150

#define PHONE_TYPE_MIN_LENGTH 3
#define PHONE_NUMBER_MIN_LENGTH 6
#define PERSON_UCN_MIN_LENGTH 10
#define PERSON_ADDRESS_MIN_LENGTH 3

#define CITY_NAME_MIN_LENGTH 3
#define CITY_REGION_MIN_LENGTH 3

/////////////////////////////////////////////////////////////////////
// CPersonsDialog

CPersonsDialog::CPersonsDialog(
	Utilities::Operations eOperation,

	PERSONS& recPerson,
	CPhoneNumbersArray& oPersonPhoneNumbersArray,
	CITIES& recPersonCity,

	CPhoneTypesArray& oPhoneTypesArray,
	CCitiesArray& oCitiesArray,

	CPhoneTypesArray* pNewPhoneTypes
)
	:
	CDialog(IDD_PERSONS_DIALOG),
	m_recPerson(recPerson),
	m_oPersonPhoneNumbersArray(oPersonPhoneNumbersArray),
	m_recPersonCity(recPersonCity),

	m_oPhoneTypesArray(oPhoneTypesArray),
	m_oCitiesArray(oCitiesArray),

	m_eOperation(eOperation)
{
}


IMPLEMENT_DYNAMIC(CPersonsDialog, CDialog)

void CPersonsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDB_PERSONS_FIRST_NAME, m_edbFirstName);
	DDX_Control(pDX, IDC_EDB_PERSONS_MIDDLE_NAME, m_edbMiddleName);
	DDX_Control(pDX, IDC_EDB_PERSONS_LAST_NAME, m_edbLastName);
	DDX_Control(pDX, IDC_EDB_PERSONS_UCN, m_edbUCN);
	DDX_Control(pDX, IDC_EDB_PERSONS_ADDRESS, m_edbAddress);
	DDX_Control(pDX, IDC_EDB_PERSONS_PHONE_NUMBER, m_edbPhoneNumber);

	DDX_Control(pDX, IDC_CMB_PERSONS_CITY, m_cmbCity);
	DDX_Control(pDX, IDC_CMB_PERSONS_PHONE_TYPE, m_cmbPhoneType);

	DDX_Control(pDX, IDC_BTN_PERSONS_ADD_PHONE, m_btnAddPhone);
	DDX_Control(pDX, IDC_BTN_PERSONS_REMOVE_NUMBER, m_btnRemovePhone);
	DDX_Control(pDX, IDC_BTN_PERSONS_EDIT_NUMBER, m_btnEditPhone);

	DDX_Control(pDX, IDC_LSC_PERSONS_PHONE_NUMBERS, m_lscPhoneNumbers);

	DDX_Control(pDX, IDC_STT_PERSONS_FIRST_NAME_VALIDATION_INFO, m_sttPersonsFirstNameValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_MIDDLE_NAME_VALIDATION_INFO, m_sttPersonsMiddleNameValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_LAST_NAME_VALIDATION_INFO, m_sttPersonsLastNameValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_UCN_VALIDATION_INFO, m_sttPersonsUCNValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_ADDRESS_VALIDATION_INFO, m_sttPersonsAddressValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_CITY_NAME_VALIDATION_INFO, m_sttPersonsCityValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_PHONE_TYPE_VALIDATION_INFO, m_sttPersonsPhoneTypeValidationInfo);
	DDX_Control(pDX, IDC_STT_PERSONS_PHONE_NUMBER_VALIDATION_INFO, m_sttPersonsPhoneNumberValidationInfo);
	DDX_Control(pDX, IDOK, m_btnOK);
}



BEGIN_MESSAGE_MAP(CPersonsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CPersonsDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &CPersonsDialog::OnCancel)
	ON_BN_CLICKED(IDC_BTN_PERSONS_ADD_PHONE, &CPersonsDialog::OnBnClickedBtnPersonsAddPhone)
	ON_BN_CLICKED(IDC_BTN_PERSONS_REMOVE_NUMBER, &CPersonsDialog::OnBnClickedBtnPersonsRemoveNumber)
	ON_BN_CLICKED(IDC_BTN_PERSONS_EDIT_NUMBER, &CPersonsDialog::OnBnClickedBtnPersonsEditNumber)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSC_PERSONS_PHONE_NUMBERS, &CPersonsDialog::OnLvnItemchangedLscPersonsPhoneNumbers)
END_MESSAGE_MAP()

BOOL CPersonsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbFirstName.SetWindowText(m_recPerson.szFirstName);
	m_edbMiddleName.SetWindowText(m_recPerson.szMiddleName);
	m_edbLastName.SetWindowText(m_recPerson.szLastName);
	m_edbUCN.SetWindowText(m_recPerson.szUCN);
	m_edbAddress.SetWindowText(m_recPerson.szAddress);

	m_edbFirstName.SetLimitText(PERSONS_FIRST_NAME_LENGTH);
	m_edbMiddleName.SetLimitText(PERSONS_MIDDLE_NAME_LENGTH);
	m_edbLastName.SetLimitText(PERSONS_LAST_NAME_LENGTH);
	m_edbUCN.SetLimitText(PERSONS_UCN_LENGTH);
	m_edbAddress.SetLimitText(PERSONS_ADDRESS_LENGTH);

	m_edbPhoneNumber.SetLimitText(PHONE_NUMBERS_PHONE_LENGTH);


	m_oCitiesArray
		.Sort(
			[](CITIES& recFirst, CITIES& recSecond)
			-> BOOL { return StrCmp(recFirst.szRegion, recSecond.szRegion) > 0; }
	);

	// populate cities in combo box
	for (INT_PTR i = 0; i < m_oCitiesArray.GetSize();i++)
	{
		CITIES* pCity = m_oCitiesArray.GetAt(i);
		CString strCityFormat;
		strCityFormat.Format(_T("%s - %s"), pCity->szRegion, pCity->szName);
		m_cmbCity.AddString(strCityFormat.GetString());
	}

	CString strCity;
	strCity.Format(_T("%s - %s"), m_recPersonCity.szRegion, m_recPersonCity.szName);
	m_cmbCity.SelectString(0, strCity);

	m_oPhoneTypesArray
		.Sort(
			[](PHONE_TYPES& recFirst, PHONE_TYPES& recSecond)
			-> BOOL { return StrCmp(recFirst.szType, recSecond.szType) > 0;}
	);

	// populate phone types in combo box
	for (INT_PTR i = 0; i < m_oPhoneTypesArray.GetSize(); i++)
	{
		PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(i);
		m_cmbPhoneType.AddString(pPhoneType->szType);
	}

	// Вмъкване на колони
	m_lscPhoneNumbers.InsertColumn(PHONE_NUMBER_TYPE_COLUMN, PHONE_NUMBER_TYPE_COLUMN_LABEL);
	m_lscPhoneNumbers.InsertColumn(PHONE_NUMBER_NUMBER_COLUMN, PHONE_NUMBER_NUMBER_COLUMN_LABEL);

	// Задаване на ширина на колоните
	m_lscPhoneNumbers.SetColumnWidth(PHONE_NUMBER_TYPE_COLUMN, PHONE_NUMBER_TYPE_COLUMN_WIDTH);
	m_lscPhoneNumbers.SetColumnWidth(PHONE_NUMBER_NUMBER_COLUMN, PHONE_NUMBER_NUMBER_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	m_lscPhoneNumbers.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_REPORT);
	m_lscPhoneNumbers.ModifyStyle(NULL, LVS_SINGLESEL, 0);

	// Зареждане на всички данни в списък в лист контролата
	for (INT_PTR i = 0; i < m_oPersonPhoneNumbersArray.GetCount(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = m_oPersonPhoneNumbersArray.GetAt(i);
		PHONE_TYPES recPhoneType = m_oPhoneTypesArray
			.First(
				[](PHONE_TYPES& oFirst, void* pPhoneNumber) -> BOOL { return oFirst.lID == static_cast<PHONE_NUMBERS*>(pPhoneNumber)->lPhoneTypeId; },
				pPhoneNumber
			);

		// Вмъкване на редове
		const INT nInsertIndex = m_lscPhoneNumbers.InsertItem(i, recPhoneType.szType);
		m_lscPhoneNumbers.SetItemText(nInsertIndex, PHONE_NUMBER_NUMBER_COLUMN, pPhoneNumber->szPhone);
		m_lscPhoneNumbers.SetItemData(nInsertIndex, i);
	}

	switch (m_eOperation)
	{
	case Utilities::OperationsCreate:
		PrepareForOperationCreate();
		break;
	case Utilities::OperationsUpdate:
		PrepareForOperationUpdate();
		break;
	case Utilities::OperationsRead:
		PrepareForOperationRead();
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

INT CPersonsDialog::CollectAndValidateCityAndDisplayMessage(CString& strCityName, CString& strCityRegion)
{
	INT nValidationsResult = 0;

	CString strCity;
	m_cmbCity.GetWindowText(strCity);
	if (strCity.IsEmpty())
	{
		m_cmbCity.SetFocus();
		m_sttPersonsCityValidationInfo.SetWindowText(_T("selection cannot be empty"));
		m_sttPersonsCityValidationInfo.ShowWindow(SW_SHOW);
		return ++nValidationsResult;
	}

	CStringArray oStringArray;
	Utilities::StringSplit(strCity, _T(" - "), oStringArray);
	strCityRegion = oStringArray.GetAt(0);
	strCityName = oStringArray.GetAt(1);
	return nValidationsResult;
}

INT CPersonsDialog::CollectAndValidateNameAndDisplayMessage(CString& strName, CEdit& edbEditBox, CStatic& sttStatic)
{
	edbEditBox.GetWindowText(strName);

	Utilities::CStringValidator oValidator(PERSON_NAME_MIN_LENGTH, _T(".,_!+-?"));

	const INT nValidationsResult = oValidator.ValidateString(
		strName,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasDigits
	);

	if (nValidationsResult)
	{
		edbEditBox.SetFocus();
		sttStatic.ShowWindow(SW_SHOW);
		sttStatic.SetWindowText(oValidator.GetValidationMessage());
	}

	return nValidationsResult;
}

INT CPersonsDialog::CollectAndValidateUCNAndDisplayMessage(CString& strUCN)
{
	m_edbUCN.GetWindowText(strUCN);

	Utilities::CStringValidator oValidator(PERSON_UCN_MIN_LENGTH, _T(".,_!+-?"));

	const INT nValidationsResult = oValidator.ValidateString(strUCN,
		Utilities::CStringValidator::StringValidationsHasLetters |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength);

	if (nValidationsResult)
	{
		m_edbUCN.SetFocus();
		m_sttPersonsUCNValidationInfo.ShowWindow(SW_SHOW);
		m_sttPersonsUCNValidationInfo.SetWindowText(oValidator.GetValidationMessage());
	}

	return nValidationsResult;
}

INT CPersonsDialog::CollectAndValidateAddressAndDisplayMessage(CString& strAddress)
{
	m_edbAddress.GetWindowText(strAddress);

	Utilities::CStringValidator oValidator(PERSON_ADDRESS_MIN_LENGTH);

	const INT nValidationsResult = oValidator.ValidateString(strAddress, Utilities::CStringValidator::StringValidationsUnderSpecifiedLength);

	if (nValidationsResult)
	{
		m_edbAddress.SetFocus();
		m_sttPersonsAddressValidationInfo.ShowWindow(SW_SHOW);
		m_sttPersonsAddressValidationInfo.SetWindowText(oValidator.GetValidationMessage());
	}

	return nValidationsResult;

}

void CPersonsDialog::OnOK()
{
	if (m_eOperation == Utilities::OperationsRead)
	{
		CDialog::OnOK();
		return;
	}

	RemoveAllWarnings();

	CString strFirstName;
	CString strMiddleName;
	CString strLastName;
	CString strUCN;
	CString strAddress;

	CString strCityName;
	CString strCityRegion;

	const INT nFirstNameValidation = CollectAndValidateNameAndDisplayMessage(strFirstName, m_edbFirstName, m_sttPersonsFirstNameValidationInfo);
	const INT nMiddleNameValidation = CollectAndValidateNameAndDisplayMessage(strMiddleName, m_edbMiddleName, m_sttPersonsMiddleNameValidationInfo);
	const INT nLastNameValidation = CollectAndValidateNameAndDisplayMessage(strLastName, m_edbLastName, m_sttPersonsLastNameValidationInfo);
	const INT nUCNValidation = CollectAndValidateUCNAndDisplayMessage(strUCN);
	const INT nAddressValidation = CollectAndValidateAddressAndDisplayMessage(strAddress);
	
	const INT nCityValidation = CollectAndValidateCityAndDisplayMessage(strCityName, strCityRegion);

	if (!!nFirstNameValidation ||
		!!nMiddleNameValidation ||
		!!nLastNameValidation ||
		!!nUCNValidation ||
		!!nAddressValidation ||
		!!nCityValidation)
		return;

	m_recPerson.SetFirstName(strFirstName.GetString());
	m_recPerson.SetMiddleName(strMiddleName.GetString());
	m_recPerson.SetLastName(strLastName.GetString());
	m_recPerson.SetUCN(strUCN.GetString());
	m_recPerson.SetAddress(strAddress);


	CITIES recComparerCity;
	recComparerCity.SetName(strCityName);
	recComparerCity.SetRegion(strCityRegion);

	const CITIES recCity = m_oCitiesArray
		.First([](CITIES& recCity, void* pComparerCity)
			-> BOOL { return !StrCmpW(recCity.szName, static_cast<CITIES*>(pComparerCity)->szName) &&
			!StrCmpW(recCity.szRegion, static_cast<CITIES*>(pComparerCity)->szRegion); },
			static_cast<void*>(&recComparerCity));

	m_recPerson.lCityId = recCity.lID;

	CDialog::OnOK();
}

void CPersonsDialog::RemovePhoneNumbersWarnings()
{
	m_sttPersonsPhoneTypeValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsPhoneNumberValidationInfo.ShowWindow(SW_HIDE);
}

void CPersonsDialog::RemoveAllWarnings()
{
	m_sttPersonsFirstNameValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsMiddleNameValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsLastNameValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsUCNValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsAddressValidationInfo.ShowWindow(SW_HIDE);
	m_sttPersonsCityValidationInfo.ShowWindow(SW_HIDE);
	RemovePhoneNumbersWarnings();
}

void CPersonsDialog::PrepareForOperationRead()
{
	this->SetWindowText(_T("Person info"));

	m_edbFirstName.EnableWindow(FALSE);
	m_edbMiddleName.EnableWindow(FALSE);
	m_edbLastName.EnableWindow(FALSE);

	m_edbUCN.EnableWindow(FALSE);
	m_edbAddress.EnableWindow(FALSE);

	m_cmbCity.EnableWindow(FALSE);
	m_cmbPhoneType.EnableWindow(FALSE);
	m_edbPhoneNumber.EnableWindow(FALSE);
	m_btnAddPhone.ShowWindow(SW_HIDE);
}

void CPersonsDialog::PrepareForOperationCreate()
{
	this->SetWindowText(_T("Add person"));
}

void CPersonsDialog::PrepareForOperationUpdate()
{
	this->SetWindowText(_T("Update person"));

}

void CPersonsDialog::RepairListCtrlIndexes(INT_PTR nIndex)
{
	for (int j = nIndex ? nIndex - 1 : 0; j < m_oPersonPhoneNumbersArray.GetSize(); j++)
	{
		m_lscPhoneNumbers.SetItemData(j, j);
	}
}

void CPersonsDialog::RemovePersonNumberAndRepairListCtrlData(PHONE_NUMBERS* pPhoneNumber)
{
	for (INT_PTR i = 0; i < m_oPersonPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pCurrent = m_oPersonPhoneNumbersArray.GetAt(i);
		if (!pPhoneNumber->Compare(*pCurrent))
		{
			m_oPersonPhoneNumbersArray.RemoveAt(i);
			m_lscPhoneNumbers.DeleteItem(i);

			RepairListCtrlIndexes(i);
			break;
		}
	}
}

INT CPersonsDialog::ValidatePhoneType(const CString& strPhoneType, CString* strPhoneTypeValidationMessage)
{
	Utilities::CStringValidator oValidator(PHONE_TYPE_MIN_LENGTH, _T(",*+"));

	const INT nValidationsResult = oValidator.ValidateString(
		strPhoneType,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasDigits
	);

	if (strPhoneTypeValidationMessage && nValidationsResult)
		*strPhoneTypeValidationMessage = oValidator.GetValidationMessage();

	return nValidationsResult;
}

INT CPersonsDialog::ValidatePhoneNumber(const CString& strPhoneNumber, CString* strPhoneValidationMessage)
{
	Utilities::CStringValidator oValidator(PHONE_NUMBER_MIN_LENGTH, _T("`.,_!"));

	const INT nValidationsResult = oValidator.ValidateString(
		strPhoneNumber,
		Utilities::CStringValidator::StringValidationsNotAllowedChars |
		Utilities::CStringValidator::StringValidationsUnderSpecifiedLength |
		Utilities::CStringValidator::StringValidationsHasLetters
	);

	if (strPhoneValidationMessage && nValidationsResult)
		*strPhoneValidationMessage = oValidator.GetValidationMessage();

	return nValidationsResult;
}

void CPersonsDialog::CollectAndValidatePhoneTypeAndDisplayMessage(CString& strPhoneType, INT& nPhoneTypeValidationResult)
{
	m_cmbPhoneType.GetWindowText(strPhoneType);
	if (strPhoneType.IsEmpty())
	{
		nPhoneTypeValidationResult++;
		m_sttPersonsPhoneTypeValidationInfo.ShowWindow(SW_SHOW);
		m_sttPersonsPhoneTypeValidationInfo.SetWindowText(_T("selection cannot be empty"));
	}
}

void CPersonsDialog::CollectAndValidatePhoneNumberAndDisplayMessage(CString& strPhoneNumber, INT& nPhoneNumberValidationResult)
{
	m_edbPhoneNumber.GetWindowText(strPhoneNumber);

	CString strPhoneValidationMessage;
	nPhoneNumberValidationResult = ValidatePhoneNumber(strPhoneNumber, &strPhoneValidationMessage);
	if (nPhoneNumberValidationResult)
	{
		m_edbPhoneNumber.SetFocus();
		m_sttPersonsPhoneNumberValidationInfo.ShowWindow(SW_SHOW);
		m_sttPersonsPhoneNumberValidationInfo.SetWindowText(strPhoneValidationMessage);
	}
}

void CPersonsDialog::OnBnClickedBtnPersonsAddPhone()
{
	RemovePhoneNumbersWarnings();

	CString strPhoneType;
	INT nPhoneTypeValidationResult = 0;
	CollectAndValidatePhoneTypeAndDisplayMessage(strPhoneType, nPhoneTypeValidationResult);

	CString strPhoneNumber;
	INT nPhoneNumberValidationResult = 0;
	CollectAndValidatePhoneNumberAndDisplayMessage(strPhoneNumber, nPhoneNumberValidationResult);

	if (!!nPhoneNumberValidationResult || !!nPhoneTypeValidationResult)
		return;

	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray
		.FirstOrDefault(
			[](PHONE_TYPES& oFirst, void* pPhoneTypeName)
			-> BOOL { return !StrCmp(oFirst.szType, static_cast<CString*>(pPhoneTypeName)->GetString()); },
			&strPhoneType
		);

	PHONE_NUMBERS recPhoneNumber;
	recPhoneNumber.SetPhone(strPhoneNumber.GetString());
	recPhoneNumber.lPhoneTypeId = pPhoneType->lID;

	const INT nPersonNumbersArrayIndex = m_oPersonPhoneNumbersArray.Add(new PHONE_NUMBERS(recPhoneNumber));

	const INT nInsertIndex = m_lscPhoneNumbers.InsertItem(m_lscPhoneNumbers.GetItemCount(), pPhoneType->szType);
	m_lscPhoneNumbers.SetItemText(nInsertIndex, PHONE_NUMBER_NUMBER_COLUMN, recPhoneNumber.szPhone);
	m_lscPhoneNumbers.SetItemData(nInsertIndex, nPersonNumbersArrayIndex);

	m_cmbPhoneType.SetTopIndex(-1);
	m_edbPhoneNumber.SetWindowText(L"");
}

void CPersonsDialog::OnBnClickedBtnPersonsRemoveNumber()
{
	const int nIndex = m_lscPhoneNumbers.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PHONE_NUMBERS* pPhoneNumber = m_oPersonPhoneNumbersArray.GetAt(m_lscPhoneNumbers.GetItemData(nIndex));

	RemovePersonNumberAndRepairListCtrlData(pPhoneNumber);
}

void CPersonsDialog::OnBnClickedBtnPersonsEditNumber()
{
	const int nIndex = m_lscPhoneNumbers.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PHONE_NUMBERS* pPhoneNumber = m_oPersonPhoneNumbersArray.GetAt(m_lscPhoneNumbers.GetItemData(nIndex));
	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray
		.FirstOrDefault(
			[](PHONE_TYPES& oFirst, void* pPhoneNumber)-> BOOL { return oFirst.lID == static_cast<PHONE_NUMBERS*>(pPhoneNumber)->lPhoneTypeId; },
			pPhoneNumber
		);

	CPhoneNumbersDialog oPhonenumbersDialog(*pPhoneNumber, *pPhoneType, m_oPhoneTypesArray);
	if (oPhonenumbersDialog.DoModal() != IDOK)
		return;

	m_lscPhoneNumbers.SetItemText(nIndex, PHONE_NUMBER_TYPE_COLUMN, pPhoneType->szType);
	m_lscPhoneNumbers.SetItemText(nIndex, PHONE_NUMBER_NUMBER_COLUMN, pPhoneNumber->szPhone);
}

void CPersonsDialog::OnLvnItemchangedLscPersonsPhoneNumbers(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_eOperation != Utilities::OperationsRead)
	{
		const LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

		if (pNMLV->uNewState & LVIS_SELECTED)
		{
			m_btnRemovePhone.EnableWindow();
			m_btnEditPhone.EnableWindow();
			return;
		}

		m_btnRemovePhone.EnableWindow(FALSE);
		m_btnEditPhone.EnableWindow(FALSE);

		*pResult = 0;
	}
}

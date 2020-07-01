#pragma once
#include "PersonsView.h"
#include "Utilities.h"
#include "resource.h"

#define PERSON_NAME_MIN_LENGTH 3
#define PERSON_NUMBER_MIN_LENGTH 3

////////////////////////////////////////////////////////////
// CPersonsDialog

/// <summary>Клас диалог за PERSONS</summary>
class CPersonsDialog : public CDialog
{
	// Macros
	// ----------------

	DECLARE_DYNAMIC(CPersonsDialog)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CPersonsDialog(
		Operations eOperation,
		PERSONS& recPerson,
		CPhoneNumbersArray& oPersonPhoneNumbersArray,
		CITIES& recPersonCity,

		CPhoneTypesArray& oPhoneTypesArray,
		CCitiesArray& oCitiesArray,

		CPhoneTypesArray* pNewPhoneTypes = NULL
	);  // constructor

	virtual ~CPersonsDialog() {}

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONS_DIALOG };
#endif

	// MFC Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	virtual BOOL OnInitDialog() override;
	
	// MFC Message Handlers
	// ----------------
private:
	INT CollectAndValidateCityAndDisplayMessage(CString& strCityName, CString& strCityRegion);
	void OnOK() override;
	
	afx_msg void OnBnClickedBtnPersonsAddNewCity();
	afx_msg void OnBnClickedBtnPersonsAddNewType();
	void RevertOnBnClickedBtnPersonsAddNewType();
	void RemovePersonNumberAndRepairListCtrlData(PHONE_NUMBERS* pPhoneNumber);
	void OnBnClickedBtnPersonsRemoveNumber();
	afx_msg void OnBnClickedBtnPersonsEditNumber();
	afx_msg void OnLvnItemchangedLscPersonsPhoneNumbers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnPersonsAddPhone();

	// Methods
	// ----------------
public:
	BOOL AreNewPhoneTypesAdded();
	BOOL IsNewCityAdded();
	
private:
	void RepairListCtrlIndexes(INT_PTR nIndex = 0);
	
	void RemovePhoneNumbersWarnings();
	void RemoveAllWarnings();
		
	void PrepareForOperationRead();
	void PrepareForOperationCreate();
	void PrepareForOperationUpdate();
		
	INT ValidatePhoneType(const CString& strPhoneType, CString* strPhoneTypeValidationMessage);
	INT ValidatePhoneNumber(const CString& strPhoneNumber, CString* strPhoneValidationMessage);

	void CollectAndValidatePhoneTypeFromEditBox(CString& strPhoneType, INT& nPhoneTypeValidationResult);
	void CollectAndValidatePhoneTypeFromComboBox(CString& strPhoneType, INT& nPhoneTypeValidationResult);
	void CollectAndValidatePhoneTypeAndDisplayMessage(CString& strPhoneType, INT& nPhoneTypeValidationResult);
	void CollectAndValidatePhoneNumberAndDisplayMessage(CString& strPhoneNumber, INT& nPhoneNumberValidationResult);

	INT CollectAndValidateNameAndDisplayMessage(CString& strName, CEdit& edbEditBox, CStatic& sttStatic);
	INT CollectAndValidateUCNAndDisplayMessage(CString& strUCN);
	INT CollectAndValidateAddressAndDisplayMessage(CString& strAddress);

	// Members
	// -------------
private:
	PERSONS& m_recPerson;
	CPhoneNumbersArray& m_oPersonPhoneNumbersArray;
	CITIES& m_recPersonCity;

	CPhoneTypesArray& m_oPhoneTypesArray;
	CCitiesArray& m_oCitiesArray;

	CPhoneTypesArray* m_pNewPhoneTypes;

	BOOL m_bAreNewPhoneTypesAdded;
	BOOL m_bIsNewCityAdded;

	Operations m_eOperation;

#pragma region CompileTimeVisible
	CEdit m_edbFirstName;
	CEdit m_edbMiddleName;
	CEdit m_edbLastName;
	CEdit m_edbUCN;
	CEdit m_edbAddress;
	CEdit m_edbPhoneNumber;

	CComboBox m_cmbCity;
	CComboBox m_cmbPhoneType;

	CButton m_btnAddNewCity;
	CButton m_btnAddNewPhoneType;
	CButton m_btnAddPhone;
	CButton m_btnRemovePhone;
	CButton m_btnEditPhone;

	CListCtrl m_lscPhoneNumbers;
#pragma endregion

#pragma region RuntimeSelectable
	CEdit m_edbCityName;
	CEdit m_edbCityRegion;
	CEdit m_edbPhoneType;

	CStatic m_sttCityName;
	CStatic m_sttCityRegion;
#pragma endregion 

#pragma region ValidationStatics
	CStatic m_sttPersonsFirstNameValidationInfo;
	CStatic m_sttPersonsMiddleNameValidationInfo;
	CStatic m_sttPersonsLastNameValidationInfo;
	CStatic m_sttPersonsUCNValidationInfo;
	CStatic m_sttPersonsAddressValidationInfo;
	CStatic m_sttPersonsCityNameValidationInfo;
	CStatic m_sttPersonsCityRegionValidationInfo;
	CStatic m_sttPersonsPhoneTypeValidationInfo;
	CStatic m_sttPersonsPhoneNumberValidationInfo;
#pragma endregion 
};

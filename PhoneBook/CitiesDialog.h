#pragma once

#include "CitiesView.h"

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

////////////////////////////////////////////////////////////
// CStringValidator

class CStringValidator
{
	// Constructor / Destructor
	// ----------------
public:
	explicit CStringValidator(INT nMinStringLength = NULL, TCHAR* pszNotAllowedChars = NULL) :
		m_nResultingValidations(Nothing),
		m_nMinStringLength(nMinStringLength),
		m_pszNotAllowedChars(pszNotAllowedChars),
		m_nRequiredValidations(All) {}

	~CStringValidator() {}

	// Enum
	// -------------
public:
	/// <summary>
	/// Flag enum за маркиране на даден тип валидация
	/// </summary>
	enum StringValidations
	{
		Nothing = 0b0000,
		HasDigits = 0b0001,
		UnderSpecifiedLength = 0b0010,
		NotAllowedChars = 0b0100,
		FirstLetterNotUpperCase = 0b1000,
		All = 0b1111
	};

	// Methods
	// ---------------
public:
	INT ValidateString(const CString& strInputString, INT nValidations = All)
	{
		m_nRequiredValidations = nValidations;

		if (HasTheFollowingValidations(UnderSpecifiedLength) && strInputString.GetLength() < m_nMinStringLength)
		{
			RaiseValidatorFlag(UnderSpecifiedLength);
			return m_nResultingValidations;
		}

		if (HasTheFollowingValidations(FirstLetterNotUpperCase) && IsCharLower(strInputString.GetAt(0)))
		{
			RaiseValidatorFlag(FirstLetterNotUpperCase);
			return m_nResultingValidations;
		}

		if (HasTheFollowingValidations(NotAllowedChars | HasDigits))
			IterativeValidation(strInputString);

		return m_nResultingValidations;
	}

	CArray<CString, CString>& GetValidationMessages()
	{
		if (m_nResultingValidations & HasDigits)
			m_oValidationMessages.Add(_T("digits not allowed"));

		if (m_nResultingValidations & UnderSpecifiedLength)
			m_oValidationMessages.Add(_T("under specified length"));

		if (m_nResultingValidations & NotAllowedChars)
			m_oValidationMessages.Add(_T("not allowed chars"));

		if (m_nResultingValidations & FirstLetterNotUpperCase)
			m_oValidationMessages.Add(_T("first letter should be uppercase"));

		return m_oValidationMessages;
	}

	CString GetValidationMessage()
	{
		return GetValidationMessages().GetAt(0);
	}

private:
	void RaiseValidatorFlag(StringValidations eStringValidation)
	{
		m_nResultingValidations |= eStringValidation;
	}

	BOOL IsInNotAllowedCharsList(const TCHAR currentChar) const
	{
		for (size_t j = 0; j < strlenT(m_pszNotAllowedChars); j++)
		{
			if (currentChar == m_pszNotAllowedChars[j])
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL HasTheFollowingValidations(const INT nValidations) const
	{
		return m_nRequiredValidations & nValidations;
	}

	void IterativeValidation(const CString& strInputString)
	{
		for (int i = 0; i < strInputString.GetLength(); i++)
		{
			const TCHAR currentChar = strInputString.GetAt(i);

			if (HasTheFollowingValidations(NotAllowedChars) && IsInNotAllowedCharsList(currentChar))
			{
				RaiseValidatorFlag(NotAllowedChars);
				break;
			}

			if (HasTheFollowingValidations(HasDigits) && _istdigit(currentChar))
			{
				RaiseValidatorFlag(HasDigits);
				break;
			}
		}

	}

	// Members
	// -------------
private:
	INT m_nResultingValidations;
	CArray<CString, CString> m_oValidationMessages;
	INT m_nMinStringLength;
	TCHAR* m_pszNotAllowedChars;
	INT m_nRequiredValidations;
};

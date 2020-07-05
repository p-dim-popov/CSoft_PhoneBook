#pragma once

namespace Utilities
{
	inline long InvertIndex(long lIndex)
	{
		return (lIndex * -1) - 1;
	}

	enum Operations
	{
		OperationsCreate = 1,
		OperationsRead = 2,
		OperationsUpdate = 3,
		OperationsDelete = 4
	};

	inline CStringArray& StringSplit(CString strLine, const CString strDeleimiter, CStringArray& oStringArray)
	{
		while (strLine.GetLength() > 0)
		{
			const int nIndex = strLine.Find(strDeleimiter);
			if (nIndex != -1)
			{
				oStringArray.Add(strLine.Left(nIndex));
				strLine = strLine.Mid(nIndex + strDeleimiter.GetLength());
			}
			else
			{
				oStringArray.Add(strLine);
				strLine = "";
			}
		}
		return oStringArray;
	}

	////////////////////////////////////////////////////////////
	// CStringValidator

	class CStringValidator
	{
		// Constructor / Destructor
		// ----------------
	public:
		explicit CStringValidator(INT nMinStringLength = NULL, TCHAR* pszNotAllowedChars = NULL) :
			m_nResultingValidations(StringValidationsNothing),
			m_nMinStringLength(nMinStringLength),
			m_pszNotAllowedChars(pszNotAllowedChars),
			m_nRequiredValidations(StringValidationsNothing) {}

		~CStringValidator() {}

		// Enum
		// -------------
	public:
		/// <summary>
		/// Flag enum за маркиране на даден тип валидация
		/// </summary>
		enum StringValidations
		{
			StringValidationsNothing = 0b00000,
			StringValidationsHasDigits = 0b00001,
			StringValidationsUnderSpecifiedLength = 0b00010,
			StringValidationsNotAllowedChars = 0b00100,
			StringValidationsFirstLetterNotUpperCase = 0b01000,
			StringValidationsHasLetters = 0b10000,
		};

		// Methods
		// ---------------
	public:
		INT ValidateString(const CString& strInputString, INT nValidations)
		{
			m_nRequiredValidations = nValidations;

			if (HasTheFollowingValidations(StringValidationsUnderSpecifiedLength) && strInputString.GetLength() < m_nMinStringLength)
			{
				RaiseValidatorFlag(StringValidationsUnderSpecifiedLength);
				return m_nResultingValidations;
			}

			if (HasTheFollowingValidations(StringValidationsFirstLetterNotUpperCase) && IsCharLower(strInputString.GetAt(0)))
			{
				RaiseValidatorFlag(StringValidationsFirstLetterNotUpperCase);
				return m_nResultingValidations;
			}

			if (HasTheFollowingValidations(StringValidationsNotAllowedChars | StringValidationsHasDigits | StringValidationsHasLetters))
				IterativeValidation(strInputString);

			return m_nResultingValidations;
		}

		CArray<CString, CString>& GetValidationMessages()
		{
			if (m_nResultingValidations & StringValidationsHasDigits)
				m_oValidationMessages.Add(_T("digits not allowed"));

			if (m_nResultingValidations & StringValidationsUnderSpecifiedLength)
				m_oValidationMessages.Add(_T("under specified length"));

			if (m_nResultingValidations & StringValidationsNotAllowedChars)
				m_oValidationMessages.Add(_T("not allowed chars"));

			if (m_nResultingValidations & StringValidationsFirstLetterNotUpperCase)
				m_oValidationMessages.Add(_T("first letter should be uppercase"));

			if (m_nResultingValidations & StringValidationsHasLetters)
			{
				m_oValidationMessages.Add(_T("letters not allowed"));
			}

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

				if (HasTheFollowingValidations(StringValidationsNotAllowedChars) && IsInNotAllowedCharsList(currentChar))
				{
					RaiseValidatorFlag(StringValidationsNotAllowedChars);
					break;
				}

				if (HasTheFollowingValidations(StringValidationsHasDigits) && _istdigit(currentChar))
				{
					RaiseValidatorFlag(StringValidationsHasDigits);
					break;
				}

				if (HasTheFollowingValidations(StringValidationsHasLetters) && _istalpha(currentChar))
				{
					RaiseValidatorFlag(StringValidationsHasLetters);
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
}

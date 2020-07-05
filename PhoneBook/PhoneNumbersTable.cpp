#include "pch.h"
#include "PhoneNumbersTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTableAccessor

// Constructor / Destructor
// ----------------

CPhoneNumbersTableAccessor::CPhoneNumbersTableAccessor()
{
}

CPhoneNumbersTableAccessor::~CPhoneNumbersTableAccessor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTable

// Constructor / Destructor
// ----------------

CPhoneNumbersTable::CPhoneNumbersTable() :
	CTable<PHONE_NUMBERS, CPhoneNumbersTableAccessor>(_T("PHONE_NUMBERS"))
{
}

CPhoneNumbersTable::~CPhoneNumbersTable()
{
}

BOOL CPhoneNumbersTable::SelectWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM [PHONE_NUMBERS] WHERE [PERSON_ID] = %i"), lID);
	
	// Изпълняваме командата
	hResult = this->Open(*m_pSession, strQuery);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->FailedCloseAll();

		return FALSE;
	}

	hResult = this->MoveNext();
	// Прочитаме всички данни и записваме всичко в oArray
	while (hResult == S_OK)
	{
		oPhoneNumbersArray.Add(new PHONE_NUMBERS(this->m_recTableRecord));
		hResult = this->MoveNext();
	}

	// Проверяваме дали сме прочели всички данни
	if (hResult != DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

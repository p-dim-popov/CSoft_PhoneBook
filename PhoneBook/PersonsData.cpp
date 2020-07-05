#include "pch.h"
#include "PersonsData.h"

#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsData

// Constructor / Destructor
// ----------------


CPersonsData::CPersonsData()
{
	m_oPersonsTable.SetDataSource(&m_oDataSource);
	m_oPersonsTable.SetSession(&m_oSession);

	m_oPhoneNumbersTable.SetDataSource(&m_oDataSource);
	m_oPhoneNumbersTable.SetSession(&m_oSession);

	m_oPhoneTypesTable.SetDataSource(&m_oDataSource);
	m_oPhoneTypesTable.SetSession(&m_oSession);

	m_oCitiesTable.SetDataSource(&m_oDataSource);
	m_oCitiesTable.SetSession(&m_oSession);
}

CPersonsData::~CPersonsData()
{
}

// Methods
// ----------------

BOOL CPersonsData::SelectAll(CPersonsArray& oPersonsArray)
{
	const BOOL bResult = m_oPersonsTable.SelectAll(oPersonsArray);

	if (!bResult)
	{
		TRACE(_T("Getting all cities in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectWhereID(const long lID, PERSONS& recPerson)
{
	const BOOL bResult = m_oPersonsTable.SelectWhereID(lID, recPerson);

	if (!bResult)
	{
		TRACE(_T("Getting city by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::UpdateWhereID(const long lID, PERSONS& recPerson)
{
	const BOOL bResult = m_oPersonsTable.UpdateWhereID(lID, recPerson);

	if (!bResult)
	{
		TRACE(_T("Updating city by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::InsertPhoneTypes(CPhoneTypesArray& oPhoneTypesArray, CMap<long, long, long, long>& oNewPhoneTypesOldIndexesMap)
{
	for (INT_PTR i = 0; i < oPhoneTypesArray.GetSize(); i++)
	{
		PHONE_TYPES* pPhoneType = oPhoneTypesArray.GetAt(i);
		const long lOldID = pPhoneType->lID;
		if (!InsertPhoneType(pPhoneType))
		{
			TRACE(_T("Inserting phone type in data level failed."));
			return FALSE;
		}

		oNewPhoneTypesOldIndexesMap.SetAt(lOldID, pPhoneType->lID);
	}

	return TRUE;
}

BOOL CPersonsData::InsertPersonWithData(PERSONS& recPerson, CPhoneNumbersArray& oPersonPhoneNumbersArray)
{
	HRESULT hResult = S_OK;
	if (!m_oDataSource.m_spInit || !m_oSession.m_spOpenRowset)
	{
		return FALSE;
	}

	// Започваме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		return FALSE;
	}

	BOOL bResult = m_oPersonsTable.Insert(recPerson);
	if (!bResult)
	{
		TRACE(_T("Inserting person in data level failed."));

		return FALSE;
	}

	for (INT_PTR i = 0; i < oPersonPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oPersonPhoneNumbersArray.GetAt(i);

		pPhoneNumber->lPersonId = recPerson.lID;

		bResult = m_oPhoneNumbersTable.Insert(*pPhoneNumber);
		if (!bResult)
		{
			TRACE(_T("Inserting phone number in data level failed."));

			return FALSE;
		}
	}

	// Приключване на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::DeleteWhereID(const long lID)
{
	const BOOL bResult = m_oPersonsTable.DeleteWhereID(lID);

	if (!bResult)
	{
		TRACE(_T("Deleting person in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectPhoneNumbersWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	const BOOL bResult = m_oPhoneNumbersTable.SelectWherePersonID(lID, oPhoneNumbersArray);

	if (!bResult)
	{
		TRACE(_T("Getting all phone numbers of a person in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectPhoneTypeWhereID(long lID, PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesTable.SelectWhereID(lID, recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Getting phone type in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectAllPhoneNumbers(CPhoneNumbersArray& oPhoneNumbersArray)
{
	const BOOL bResult = m_oPhoneNumbersTable.SelectAll(oPhoneNumbersArray);

	if (!bResult)
	{
		TRACE(_T("Getting all phone numbers in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectAllPhoneTypes(CPhoneTypesArray& oPhoneTypesArray)
{
	const BOOL bResult = m_oPhoneTypesTable.SelectAll(oPhoneTypesArray);

	if (!bResult)
	{
		TRACE(_T("Getting all phone types in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::SelectAllCities(CCitiesArray& oCitiesArray)
{
	const BOOL bResult = m_oCitiesTable.SelectAll(oCitiesArray);

	if (!bResult)
	{
		TRACE(_T("Getting all cities in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::InsertPhoneType(PHONE_TYPES* pPhoneType)
{
	if (!pPhoneType)
		return FALSE;

	const BOOL bResult = m_oPhoneTypesTable.Insert(*pPhoneType);

	if (!bResult)
	{
		TRACE(_T("Inserting phone type in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::InsertCity(CITIES* pCity)
{
	if (!pCity)
		return FALSE;

	const BOOL bResult = m_oCitiesTable.Insert(*pCity);

	if (!bResult)
	{
		TRACE(_T("Inserting city in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::DeletePhoneNumberWhereID(const long lID)
{
	const BOOL bResult = m_oPhoneNumbersTable.DeleteWhereID(lID);

	if (!bResult)
	{
		TRACE(_T("Deleting phone number in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::UpdatePersonWithData(
	PERSONS& recPerson,
	CPhoneNumbersArray& oDeletedPhoneNumbersArray,
	CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
	CPhoneNumbersArray& oNewPhoneNumbersArray
)
{
	HRESULT hResult = S_OK;
	if (!m_oDataSource.m_spInit || !m_oSession.m_spOpenRowset)
	{
		TRACE(_T("No data source or session opened."));
		return FALSE;
	}

	// Започваме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		return FALSE;
	}

	BOOL bResult = m_oPersonsTable.UpdateWhereID(recPerson.lID, recPerson, TRUE);
	if (!bResult)
	{
		TRACE(_T("Updating person in data level failed."));
		return FALSE;
	}

	for (INT_PTR i = 0; i < oUpdatedPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oUpdatedPhoneNumbersArray.GetAt(i);

		bResult = m_oPhoneNumbersTable.UpdateWhereID(pPhoneNumber->lID, *pPhoneNumber, TRUE);
		if (!bResult)
		{
			TRACE(_T("Updating phone number in data level failed."));

			return FALSE;
		}
	}

	for (INT_PTR i = 0; i < oNewPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oNewPhoneNumbersArray.GetAt(i);

		pPhoneNumber->lPersonId = recPerson.lID;

		bResult = m_oPhoneNumbersTable.Insert(*pPhoneNumber);
		if (!bResult)
		{
			TRACE(_T("Inserting phone number in data level failed."));

			return FALSE;
		}
	}

	for (INT_PTR i = 0; i < oDeletedPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oDeletedPhoneNumbersArray.GetAt(i);

		bResult = m_oPhoneNumbersTable.DeleteWhereID(pPhoneNumber->lID);
		if (!bResult)
		{
			TRACE(_T("Deleting phone number in data level failed."));

			return FALSE;
		}
	}

	// Приключване на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		return FALSE;
	}

	return TRUE;

}

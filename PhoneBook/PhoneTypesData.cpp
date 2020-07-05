#include "pch.h"
#include "PhoneTypesData.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTableAccessor

// Constructor / Destructor
// ----------------

CPhoneTypesData::CPhoneTypesData()
{
	m_oPhoneTypesTable.SetDataSource(&m_oDataSource);
	m_oPhoneTypesTable.SetSession(&m_oSession);
}

CPhoneTypesData::~CPhoneTypesData()
{
}

// Methods
// ----------------

BOOL CPhoneTypesData::SelectAll(CPhoneTypesArray& oPhoneTypesArray)
{
	const BOOL bResult = m_oPhoneTypesTable.SelectAll(oPhoneTypesArray);

	if (!bResult)
	{
		TRACE(_T("Getting all cities in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPhoneTypesData::SelectWhereID(const long lID, PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesTable.SelectWhereID(lID, recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Getting phone type by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPhoneTypesData::UpdateWhereID(const long lID, PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesTable.UpdateWhereID(lID, recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Updating phone type by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPhoneTypesData::InsertPhoneType(PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesTable.Insert(recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Inserting phone type in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CPhoneTypesData::DeleteWhereID(const long lID)
{
	const BOOL bResult = m_oPhoneTypesTable.DeleteWhereID(lID);

	if (!bResult)
	{
		TRACE(_T("Deleting phone type in data level failed."));
		return FALSE;
	}

	return TRUE;
}

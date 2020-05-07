#include "pch.h"
#include "CitiesData.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTableAccessor

// Constructor / Destructor
// ----------------

CCitiesData::CCitiesData()
{
}

CCitiesData::~CCitiesData()
{
}

// Methods
// ----------------

BOOL CCitiesData::SelectAll(CCitiesArray& oCitiesArray)
{
	const BOOL bResult = m_oCitiesTable.SelectAll(oCitiesArray);
	
	if (!bResult)
	{
		TRACE(_T("Getting all cities in data level failed."));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCitiesData::SelectWhereId(long lId, CITIES& recCity)
{
	const BOOL bResult = m_oCitiesTable.SelectWhereID(lId, recCity);
	
	if (!bResult)
	{
		TRACE(_T("Getting city by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CCitiesData::UpdateWhereId(long lId, CITIES& recCity)
{
	const BOOL bResult = m_oCitiesTable.UpdateWhereID(lId, recCity);

	if (!bResult)
	{
		TRACE(_T("Updating city by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CCitiesData::InsertCity(CITIES& recCity)
{
	const BOOL bResult = m_oCitiesTable.Insert(recCity);

	if (!bResult)
	{
		TRACE(_T("Inserting city in data level failed."));
		return FALSE;
	}

	return TRUE;
}

BOOL CCitiesData::DeleteWhereId(const long lId)
{
	const BOOL bResult = m_oCitiesTable.DeleteWhereID(lId);

	if (!bResult)
	{
		TRACE(_T("Deleting city in data level failed."));
		return FALSE;
	}

	return TRUE;
}

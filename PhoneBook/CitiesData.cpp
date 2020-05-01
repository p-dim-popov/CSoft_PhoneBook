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

BOOL CCitiesData::GetAllCities(CCitiesArray& oCitiesArray)
{
	const BOOL bResult = m_oCitiesTable.SelectAll(oCitiesArray);
	
	if (!bResult)
	{
		TRACE(_T("Getting all cities in data level failed."));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCitiesData::GetCityById(long lId, CITIES& recCity)
{
	const BOOL bResult = m_oCitiesTable.SelectWhereID(lId, recCity);
	
	if (!bResult)
	{
		TRACE(_T("Getting city by id in data level failed."));
		return FALSE;
	}

	return TRUE;
}

#include "pch.h"
#include "CitiesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTableAccessor

// Constructor / Destructor
// ----------------

CCitiesTableAccessor::CCitiesTableAccessor()
{
}

CCitiesTableAccessor::~CCitiesTableAccessor()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

// Constructor / Destructor
// ----------------

CCitiesTable::CCitiesTable(CDataSource* pDataSource, CSession* pSession) :
	CTable<CITIES, CCitiesTableAccessor>(pDataSource, pSession, _T("CITIES"))
{
}

BOOL CCitiesTable::SelectWhereName(const TCHAR* pszCityName, CITIES& recCity)
{
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE [Name] = %s"), pszCityName);
	return this->ExecuteCustomSelectQuery(strQuery.GetString(), recCity);
}

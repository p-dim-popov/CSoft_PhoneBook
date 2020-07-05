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

CCitiesTable::CCitiesTable() :
	CTable<CITIES, CCitiesTableAccessor>(_T("CITIES"))
{
}
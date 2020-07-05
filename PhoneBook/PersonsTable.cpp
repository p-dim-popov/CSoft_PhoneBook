#include "pch.h"
#include "PersonsTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTableAccessor

// Constructor / Destructor
// ----------------

CPersonsTableAccessor::CPersonsTableAccessor()
{
}

CPersonsTableAccessor::~CPersonsTableAccessor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

// Constructor / Destructor
// ----------------

CPersonsTable::CPersonsTable() :
	CTable<PERSONS, CPersonsTableAccessor>(_T("PERSONS"))
{
}

CPersonsTable::~CPersonsTable()
{
}
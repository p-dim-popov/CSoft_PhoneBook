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

CPersonsTable::CPersonsTable(CDataSource* pDataSource, CSession* pSession) :
	CTable<PERSONS, CPersonsTableAccessor>(pDataSource, pSession, _T("PERSONS"))
{
}

CPersonsTable::~CPersonsTable()
{
}
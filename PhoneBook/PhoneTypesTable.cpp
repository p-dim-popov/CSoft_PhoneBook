#include "pch.h"
#include "PhoneTypesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTableAccessor

// Constructor / Destructor
// ----------------

CPhoneTypesTableAccessor::CPhoneTypesTableAccessor()
{
}

CPhoneTypesTableAccessor::~CPhoneTypesTableAccessor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

// Constructor / Destructor
// ----------------

CPhoneTypesTable::CPhoneTypesTable(CDataSource* pDataSource, CSession* pSession) :
	CTable<PHONE_TYPES, CPhoneTypesTableAccessor>(pDataSource, pSession, _T("PHONE_TYPES"))
{
}

CPhoneTypesTable::~CPhoneTypesTable()
{
}
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

CPhoneTypesTable::CPhoneTypesTable() :
	CTable<PHONE_TYPES, CPhoneTypesTableAccessor>(_T("PHONE_TYPES"))
{
}

CPhoneTypesTable::~CPhoneTypesTable()
{
}
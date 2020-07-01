#include "pch.h"
#include "PhoneNumbersTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTableAccessor

// Constructor / Destructor
// ----------------

CPhoneNumbersTableAccessor::CPhoneNumbersTableAccessor()
{
}

CPhoneNumbersTableAccessor::~CPhoneNumbersTableAccessor()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTable

// Constructor / Destructor
// ----------------

CPhoneNumbersTable::CPhoneNumbersTable(CDataSource* pDataSource, CSession* pSession) :
	CTable<PHONE_NUMBERS, CPhoneNumbersTableAccessor>(pDataSource, pSession, _T("PHONE_NUMBERS"))
{
}

CPhoneNumbersTable::~CPhoneNumbersTable()
{
}

BOOL CPhoneNumbersTable::SelectWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE [PersonID] = %i"), lID);
	return this->ExecuteCustomSelectQuery(strQuery.GetString(), oPhoneNumbersArray);
}

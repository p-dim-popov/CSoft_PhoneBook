#pragma once
#include "TableAccessor.h"
#include "Table.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTableAccessor

/// <summary>Аксесор за таблица CITIES oт CSOFT_PHONEBOOK</summary>
class CPhoneNumbersTableAccessor : public CTableAccessor<PHONE_NUMBERS>
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneNumbersTableAccessor();
	~CPhoneNumbersTableAccessor();

	// Constants
	// ----------------
protected:
	BEGIN_ACCESSOR_MAP(CPhoneNumbersTableAccessor, 2)
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recTableRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recTableRecord.lUpdateCounter)
		COLUMN_ENTRY(3, m_recTableRecord.lPersonId)
		COLUMN_ENTRY(4, m_recTableRecord.lPhoneTypeId)
		COLUMN_ENTRY(5, m_recTableRecord.szPhone)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
class CPhoneNumbersTable : public ::CTable<PHONE_NUMBERS, CPhoneNumbersTableAccessor>
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneNumbersTable();
	~CPhoneNumbersTable();

	// Methods
	// ----------------
public:
	BOOL SelectWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray);
};

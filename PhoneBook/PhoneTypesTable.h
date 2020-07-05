#pragma once
#include "TableAccessor.h"
#include "Table.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTableAccessor

/// <summary>Аксесор за таблица CITIES oт CSOFT_PHONEBOOK</summary>
class CPhoneTypesTableAccessor : public CTableAccessor<PHONE_TYPES>
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesTableAccessor();
	~CPhoneTypesTableAccessor();

	// Constants
	// ----------------
protected:
	BEGIN_ACCESSOR_MAP(CPhoneTypesTableAccessor, 2)
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recTableRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recTableRecord.lUpdateCounter)
		COLUMN_ENTRY(3, m_recTableRecord.szType)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
class CPhoneTypesTable : public ::CTable<PHONE_TYPES, CPhoneTypesTableAccessor>
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesTable();
	~CPhoneTypesTable();
};

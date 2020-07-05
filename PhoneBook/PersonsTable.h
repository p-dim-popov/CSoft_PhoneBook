#pragma once
#include "TableAccessor.h"
#include "Table.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsTableAccessor

/// <summary>Аксесор за таблица CITIES oт CSOFT_PHONEBOOK</summary>
class CPersonsTableAccessor : public CTableAccessor<PERSONS>
{

	// Constructor / Destructor
	// ----------------
public:
	CPersonsTableAccessor();
	~CPersonsTableAccessor();

	// Constants
	// ----------------
protected:
	BEGIN_ACCESSOR_MAP(CPersonsTableAccessor, 2)
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recTableRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recTableRecord.lUpdateCounter)
		COLUMN_ENTRY(3, m_recTableRecord.szFirstName)
		COLUMN_ENTRY(4, m_recTableRecord.szMiddleName)
		COLUMN_ENTRY(5, m_recTableRecord.szLastName)
		COLUMN_ENTRY(6, m_recTableRecord.szUCN)
		COLUMN_ENTRY(7, m_recTableRecord.lCityId)
		COLUMN_ENTRY(8, m_recTableRecord.szAddress)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPersonsTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
class CPersonsTable : public ::CTable<PERSONS, CPersonsTableAccessor>
{
	// Constructor / Destructor
	// ----------------
public:
	CPersonsTable();
	~CPersonsTable();
};

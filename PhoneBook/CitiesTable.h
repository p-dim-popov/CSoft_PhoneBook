#pragma once
#include "Structures.h"
#include "TableAccessor.h"
#include "Table.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTableAccessor

/// <summary>Аксесор за таблица CITIES oт CSOFT_PHONEBOOK</summary>
class CCitiesTableAccessor : public CTableAccessor<CITIES>
{

	// Constructor / Destructor
	// ----------------
public:
	CCitiesTableAccessor();
	~CCitiesTableAccessor();

	// Constants
	// ----------------
protected:
	BEGIN_ACCESSOR_MAP(CCitiesTableAccessor, 2)
		BEGIN_ACCESSOR(0, TRUE)
		COLUMN_ENTRY(1, m_recTableRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, TRUE)
		COLUMN_ENTRY(2, m_recTableRecord.lUpdateCounter)
		COLUMN_ENTRY(3, m_recTableRecord.szName)
		COLUMN_ENTRY(4, m_recTableRecord.szRegion)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
class CCitiesTable : public ::CTable<CITIES, CCitiesTableAccessor>
{
	// Constructor / Destructor
	// ----------------
public:
	CCitiesTable();
};


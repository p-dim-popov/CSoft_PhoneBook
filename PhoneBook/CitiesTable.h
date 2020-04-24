#pragma once
#include "Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTableAccessor

/// <summary>
/// Аксесор за таблица CITIES oт CSOFT_PHONEBOOK
/// </summary>
class CCitiesTableAccessor
{
	
// Constructor / Destructor
// ----------------
public:
	CCitiesTableAccessor() = default;
	~CCitiesTableAccessor() = default;

// Constants
// ----------------
protected:
	BEGIN_ACCESSOR_MAP( CCitiesTableAccessor, 2 )
		BEGIN_ACCESSOR( 0, true )
			COLUMN_ENTRY( 1, m_recCity.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR( 1, true )
			COLUMN_ENTRY( 2, m_recCity.lUpdateCounter)
			COLUMN_ENTRY( 3, m_recCity.szName)
			COLUMN_ENTRY( 4, m_recCity.szRegion)
		END_ACCESSOR()
	END_ACCESSOR_MAP()

// Members
// ----------------
protected:
	CITIES m_recCity;
};

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
class CCitiesTable : private CCommand<CAccessor<CCitiesTableAccessor>>
{

// Constructor / Destructor
// ----------------
public:
	CCitiesTable();
	~CCitiesTable();

// Methods
// ----------------
public:
	/// <summary>Функция за селектиране на всички записи от базата данни</summary>
	/// <param name="oCitiesArray">Масив, в който ще бъдат записани записите от БД</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	BOOL SelectAll(CCitiesArray& oCitiesArray);
	
	/// <summary>Функция за селектиране на запис по дадено ID</summary>
	/// <param name="lID">ID което се търси</param>
	/// <param name="recCity">Структура CITIES върху която ще бъдат записани данните от БД</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	BOOL SelectWhereID(const long lID, CITIES& recCity);
	
	/// <summary>Функция за промяна на запис по дадено ID</summary>
	/// <param name="lID">ID на град, който ще се ъпдейтва</param>
	/// <param name="recCity">Град, който е ъпдейтнат</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	BOOL UpdateWhereID(const long lID, const CITIES& recCity);
	
	/// <summary>Функция за добавяне на запис в базата данни</summary>
	/// <param name="recCity">Нов град, който ще се добавя</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	BOOL Insert(const CITIES& recCity);
	
	/// <summary>Функция за изтриване на запис от базата данни</summary>
	/// <param name="lID">ID на град, който ще се премахва</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	BOOL DeleteWhereID(const long lID);

private:
	/// <summary>Настройка на DBPropSet преди установяване на връзка към БД</summary>
	/// <param name="oDBPropSet">Обект за сет от параметри за БД</param>
	void SetUpDBInit(CDBPropSet& oDBPropSet);
	
	/// <summary>Функция за отваряне на Data Source</summary>
	/// <param name="oDataSource">Обект за връзка към БД</param>
	/// <returns>HRESULT - Статус за успеваемостта</returns>
	HRESULT OpenDataSource(CDataSource& oDataSource);

	/// <summary>Затваря DataSource, Session и CitiesTable</summary>
	/// <param name="oDataSource">Обект с информация за базата</param>
	/// <param name="oSession">Установена сесия</param>
	/// <param name="oCitiesTable">Команда към базата</param>
	/// <remarks>Не всички параметри са задължителни, зависи от етапа, в който трябва да бъдат затворени всички връзки</remarks>
	void CloseAll(CDataSource* oDataSource = nullptr, CSession* oSession = nullptr, CCitiesTable* oCitiesTable = nullptr);

	/// <summary>Настройка на типа на Rowset-а</summary>
	/// <param name="oUpdateDBPropSet">Обект за сет от параметри за БД</param>
	void SetUpRowset(CDBPropSet& oUpdateDBPropSet);

// Members
// ----------------
private:
	CDataSource m_oDataSource;
	CSession m_oSession;
};


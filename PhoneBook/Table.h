#pragma once
#include "Structures.h"
/////////////////////////////////////////////////////////////////////////////
// CTable

/// <summary>Клас за работа с таблица CITIES от CSOFT_PHONEBOOK</summary>
template<typename RECORD_TYPE, class TABLE_ACCESSOR>
class CTable : public CCommand<CAccessor<TABLE_ACCESSOR>>
{
	// Constructor / Destructor
	// ----------------
public:
	CTable(TCHAR* pszTableName);
	~CTable();

	// Methods
	// ----------------
public:
	/// <summary>Функция за селектиране на всички записи от базата данни</summary>
	/// <param name="oArray">Масив, в който ще бъдат записани записите от БД</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	virtual BOOL SelectAll(CAutoDeleteTypedPtrArray<RECORD_TYPE>& oArray);

	/// <summary>Функция за селектиране на запис по дадено ID</summary>
	/// <param name="lID">ID което се търси</param>
	/// <param name="recTableRecord">Структура CITIES върху която ще бъдат записани данните от БД</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	virtual BOOL SelectWhereID(const long lID, RECORD_TYPE& recTableRecord);

	/// <summary>Функция за промяна на запис по дадено ID</summary>
	/// <param name="lID">ID на град, който ще се ъпдейтва</param>
	/// <param name="recTableRecord">Град, който е ъпдейтнат</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	virtual BOOL UpdateWhereID(const long lID, RECORD_TYPE& recTableRecord, const BOOL bUseUpperLevelTransaction = FALSE);

	/// <summary>Функция за добавяне на запис в базата данни</summary>
	/// <param name="recTableRecord">Нов град, който ще се добавя</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	virtual BOOL Insert(RECORD_TYPE& recTableRecord);

	/// <summary>Функция за изтриване на запис от базата данни</summary>
	/// <param name="lID">ID на град, който ще се премахва</param>
	/// <returns>BOOL - TRUE ако е успешно и FALSE ако е неуспешно</returns>
	virtual BOOL DeleteWhereID(const long lID);

	void SetDataSource(CDataSource* pDataSource);
	void SetSession(CSession* pSession);
protected:
	/// <summary>Настройка на DBPropSet преди установяване на връзка към БД</summary>
	/// <param name="oDBPropSet">Обект за сет от параметри за БД</param>
	virtual void SetUpDBInit(CDBPropSet& oDBPropSet);

	/// <summary>Функция за отваряне на Data Source</summary>
	/// <returns>HRESULT - Статус за успеваемостта</returns>
	virtual HRESULT OpenDataSource();

	/// <summary>Функция за отваряне на Session</summary>
	/// <returns>HRESULT - Статус за успеваемостта</returns>
	virtual HRESULT OpenSession();

	/// <summary>Функция за отваряне на Data Source и Session</summary>
	/// <returns>HRESULT - Статус за успеваемостта</returns>
	virtual HRESULT OpenDataSourceAndSession();

	/// <summary>Затваря DataSource, Session и CitiesTable</summary>
	void FailedCloseAll();

	/// <summary>Настройка на Rowset-а</summary>
	/// <param name="oUpdateDBPropSet">Обект за сет от параметри за БД</param>
	virtual void PrepareDbForUpdate(CDBPropSet& oUpdateDBPropSet);

	/// <summary>Изпълнява команда за взимане на запис</summary>
	/// <param name="strQuery">Заявка за изпълнение към БД</param>
	/// <param name="pUpdateDBPropSet">Указател към сет от параметри за БД</param>
	/// <returns>HRESULT - Статус за успеваемостта</returns>
	virtual HRESULT ExecuteCommandSelectWhereId(const long lID, CDBPropSet* pUpdateDBPropSet = NULL, const TCHAR* pszLockType = NULL);

	// Members
	// ----------------
protected:
	CDataSource* m_pDataSource;
	CSession* m_pSession;
	CString m_strTableName;
};

#include "Table.inl"
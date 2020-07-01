#pragma once
#include "afxwin.h"
#include "Table.h"

#define FIRST_ACCESSOR 0
#define SECOND_ACCESSOR 1
#define ERROR_COMMIT_MESSAGE _T("Error ending transaction. Error: %d\n")
#define ERROR_DATA_SOURCE_OPEN_MESSAGE _T("Unable to connect to SQL Server database. Error: %d\n")
#define ERROR_SESSION_OPEN_MESSAGE _T("Unable to open session. Error: %d\n")
#define ERROR_STARTING_TRANSACTION_MESSAGE _T("Error starting transaction. Error: %d.\n")
#define ERROR_COMMAND_OPEN_MESSAGE _T("Error executing query. Error: %d. Query: %s\n")
#define ERROR_MOVE_LAST_MESSAGE _T("Error moving to last record. Error: %d.\n")
#define ERROR_INSERT_MESSAGE _T("Unable to insert. Error: %d\n")
#define ERROR_MOVE_FIRST_MESSAGE _T("Error moving to first record. Error: %d.\n")
#define ERROR_DELETE_MESSAGE _T("Unable to delete record with id: %d. Error: %d.\n")
#define ERROR_VERSION_CONTROL_MESSAGE _T("Error in version control - Update Counters does not match. Error: %d, Id: %d.\n")
#define ERROR_SET_DATA_MESSAGE _T("Error updating record. Error: %d.\n")
#define ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE _T("Failed - END OF ROWSET not reached. Error: %d.\n")
#define ERROR_NO_RECORD_WITH_ID_MESSAGE _T("Error - no record with that id. Query: %s HRESULT: %d.\n")
#define ERROR_ExecuteCommandGetRequestedRecord_MESSAGE _T("Getting record failed.")
#define ERROR_OpenDataSourceAndSession_MESSAGE _T("Opening data source and session failed.")
#define ERROR_GETTING_RESULTING_ID_MESSAGE _T("Error getting id after insert: %d.")


/////////////////////////////////////////////////////////////////////////////
// CTable

// Constructor / Destructor
// ----------------
template<typename RECORD_TYPE, class TABLE_ACCESSOR>
::CTable<RECORD_TYPE, TABLE_ACCESSOR>::CTable(CDataSource* pDataSource, CSession* pSession, TCHAR* pszTableName) :
	m_pDataSource(pDataSource),
	m_pSession(pSession),
	m_pszTableName(pszTableName)
{
}

template <typename RECORD_TYPE, class TABLE_ACCESSOR>
::CTable<RECORD_TYPE, TABLE_ACCESSOR>::CTable(const CTable& oTable) :
	m_pDataSource(oTable.m_pDataSource),
	m_pSession(oTable.m_pSession),
	m_pszTableName(oTable.m_pszTableName)
{
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
::CTable<RECORD_TYPE, TABLE_ACCESSOR>::~CTable()
{
	this->Close();
	this->m_pSession->Close();
	this->m_pDataSource->Close();
}


// Methods
// ----------------

// Public

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::SelectAll(CAutoDeleteTypedPtrArray<RECORD_TYPE>& oArray))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Конструираме заявката
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM %s"), this->m_pszTableName);

	// Изпълняваме командата
	hResult = this->Open(*m_pSession, strQuery);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->FailedCloseAll();

		return FALSE;
	}

	hResult = CCommand<CAccessor<TABLE_ACCESSOR>>::MoveNext();
	// Прочитаме всички данни и записваме всичко в oArray
	while (hResult == S_OK)
	{
		oArray.Add(new RECORD_TYPE(this->m_recTableRecord));
		hResult = CCommand<CAccessor<TABLE_ACCESSOR>>::MoveNext();
	}

	// Проверяваме дали сме прочели всички данни
	if (hResult != DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::SelectWhereID(const long lID, RECORD_TYPE& recTableRecord))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	hResult = ExecuteCommandSelectWhereId(lID);
	if (FAILED(hResult))
	{
		TRACE(ERROR_ExecuteCommandGetRequestedRecord_MESSAGE);

		this->FailedCloseAll();

		return FALSE;
	}

	// Присвояване на стойностите от БД върху подадения запис в програмата
	recTableRecord = this->m_recTableRecord;

	// Затваряне на командата
	this->Close();

	return TRUE;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::UpdateWhereID(const long lID, RECORD_TYPE& recTableRecord, const BOOL bUseUpperLevelTransaction))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Започваме транзакция
	if (!bUseUpperLevelTransaction)
	{
		hResult = m_pSession->StartTransaction();
	}
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		m_pSession->Abort();

		this->FailedCloseAll();

		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->PrepareDbForUpdate(oUpdateDBPropSet);

	TCHAR szLockType[] = _T("WITH (SERIALIZABLE)");

	hResult = ExecuteCommandSelectWhereId(lID, &oUpdateDBPropSet, szLockType);
	if (FAILED(hResult))
	{
		TRACE(ERROR_ExecuteCommandGetRequestedRecord_MESSAGE);

		this->FailedCloseAll();

		return FALSE;
	}

	// Проверка на версиите - на подадения запис и този от базата
	if (this->m_recTableRecord.lUpdateCounter != recTableRecord.lUpdateCounter)
	{
		TRACE(ERROR_VERSION_CONTROL_MESSAGE, hResult, lID);

		m_pSession->Abort();

		this->FailedCloseAll();

		return FALSE;
	}

	// Инкрементиране на брояча за версиите
	++recTableRecord.lUpdateCounter;

	// Записване на новите стойности върху записа от базата данни
	this->m_recTableRecord = recTableRecord;

	// Записване на новите стойности в базата данни
	hResult = this->SetData(SECOND_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(ERROR_SET_DATA_MESSAGE, hResult);

		m_pSession->Abort();

		this->FailedCloseAll();

		return FALSE;
	}

	// Приключване на транзакция
	if (!bUseUpperLevelTransaction)
	{
		hResult = m_pSession->Commit();
	}
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		m_pSession->Abort();

		this->FailedCloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::Insert(RECORD_TYPE& recTableRecord))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);
		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->PrepareDbForUpdate(oUpdateDBPropSet);
	oUpdateDBPropSet.AddProperty(DBPROP_SERVERDATAONINSERT, true);

	// Конструиране на заявката
	CString strQuery;
	strQuery.Format(_T("SELECT TOP 0 * FROM %s WITH (SERIALIZABLE)"), this->m_pszTableName);

	// Изпълняване на заявката
	hResult = this->Open(*m_pSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->FailedCloseAll();

		return FALSE;
	}

	// Преместване към последния запис
	hResult = this->MoveLast();
	if (FAILED(hResult))
	{
		TRACE(ERROR_MOVE_LAST_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Подготвяне на подадения обект/запис за изпращане към БД
	this->m_recTableRecord = recTableRecord;

	// Вмъкване в БД
	hResult = CCommand<CAccessor<TABLE_ACCESSOR>>::Insert(SECOND_ACCESSOR, true);
	if (FAILED(hResult))
	{
		TRACE(ERROR_INSERT_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	hResult = this->GetData(FIRST_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(ERROR_GETTING_RESULTING_ID_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Записване на новия запис (от БД) върху подадения обект
	recTableRecord = this->m_recTableRecord;

	// Затваряне на командата
	this->Close();

	return TRUE;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::DeleteWhereID(const long lID))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->PrepareDbForUpdate(oUpdateDBPropSet);

	hResult = ExecuteCommandSelectWhereId(lID, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_ExecuteCommandGetRequestedRecord_MESSAGE);

		this->FailedCloseAll();

		return FALSE;
	}

	// Изтриване на записа
	hResult = this->Delete();
	if (FAILED(hResult))
	{
		TRACE(ERROR_DELETE_MESSAGE, lID, hResult);

		m_pSession->Abort();

		this->FailedCloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

// Protected

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
void ::CTable<RECORD_TYPE, TABLE_ACCESSOR>::SetUpDBInit(CDBPropSet& oDBPropSet)
{
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("PJS-PC-WIN10"));	// сървър
	oDBPropSet.AddProperty(DBPROP_AUTH_USERID, _T("PJS"));			// потребител
	oDBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, _T("123"));			// парола
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("CSOFT_PHONEBOOK"));	// база данни
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
HRESULT(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::OpenDataSource())
{
	if (!m_pDataSource->m_spInit)
	{
		CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
		this->SetUpDBInit(oDBPropSet);
		return m_pDataSource->Open(_T("SQLOLEDB.1"), &oDBPropSet);
	}

	return S_OK;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
HRESULT(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::OpenSession())
{
	if (!m_pSession->m_spOpenRowset)
	{
		return m_pSession->Open(*m_pDataSource);
	}

	return S_OK;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
HRESULT(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::OpenDataSourceAndSession())
{
	// Свързваме се към базата данни, ако не сме се вързали
	HRESULT hResult = OpenDataSource();
	if (FAILED(hResult))
	{
		TRACE(ERROR_DATA_SOURCE_OPEN_MESSAGE, hResult);

		return FALSE;
	}

	// Отваряме сесия, ако няма отворена
	hResult = OpenSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_SESSION_OPEN_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	return hResult;
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
void ::CTable<RECORD_TYPE, TABLE_ACCESSOR>::FailedCloseAll()
{
	this->Close();
	m_pSession->Abort();
	m_pSession->Close();
	m_pDataSource->Close();
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
void ::CTable<RECORD_TYPE, TABLE_ACCESSOR>::PrepareDbForUpdate(CDBPropSet& oUpdateDBPropSet)
{
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

template<typename RECORD_TYPE, class TABLE_ACCESSOR>
HRESULT(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::ExecuteCommandSelectWhereId(const long lID, CDBPropSet* pUpdateDBPropSet, const TCHAR* pszLockType))
{
	CString strQuery;
	// конструиране на заявката
	strQuery.Format(_T("SELECT * FROM %s %s WHERE ID = %d"), this->m_pszTableName, pszLockType ? pszLockType : _T(""), lID);

	// Изпълняваме командата
	HRESULT hResult = this->Open(*m_pSession, strQuery, pUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->FailedCloseAll();

		return FALSE;
	}

	// Взимане на записа
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(ERROR_MOVE_FIRST_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_NO_RECORD_WITH_ID_MESSAGE, strQuery.GetString(), hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	return hResult;
}

template <typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::ExecuteCustomSelectQuery(
	const TCHAR* pszQuery,
	CAutoDeleteTypedPtrArray<RECORD_TYPE>& oRecordsArray,
	CDBPropSet* pUpdateDBPropSet
))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Изпълняваме командата
	hResult = this->Open(*m_pSession, pszQuery, pUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, pszQuery);

		this->FailedCloseAll();

		return FALSE;
	}

	hResult = CCommand<CAccessor<TABLE_ACCESSOR>>::MoveNext();
	// Прочитаме всички данни и записваме всичко в oArray
	while (hResult == S_OK)
	{
		oRecordsArray.Add(new RECORD_TYPE(this->m_recTableRecord));
		hResult = CCommand<CAccessor<TABLE_ACCESSOR>>::MoveNext();
	}

	// Проверяваме дали сме прочели всички данни
	if (hResult != DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

template <typename RECORD_TYPE, class TABLE_ACCESSOR>
BOOL(::CTable<RECORD_TYPE, TABLE_ACCESSOR>::ExecuteCustomSelectQuery(
	const TCHAR* pszQuery,
	RECORD_TYPE& recTableRecord,
	CDBPropSet* pUpdateDBPropSet
))
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Изпълняваме командата
	hResult = this->Open(*m_pSession, pszQuery, pUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, pszQuery);

		this->FailedCloseAll();

		return FALSE;
	}

	// Взимане на записа
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(ERROR_MOVE_FIRST_MESSAGE, hResult);

		this->FailedCloseAll();

		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_NO_RECORD_WITH_ID_MESSAGE, pszQuery, hResult);

		this->FailedCloseAll();

		return FALSE;
	}


	// Присвояване на стойностите от БД върху подадения запис в програмата
	recTableRecord = this->m_recTableRecord;

	// Затваряне на командата
	this->Close();

	return TRUE;

}

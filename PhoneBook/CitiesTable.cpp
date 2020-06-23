#include "pch.h"
#include "CitiesTable.h"

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
// CCitiesTableAccessor

// Constructor / Destructor
// ----------------

CCitiesTableAccessor::CCitiesTableAccessor()
{
}

CCitiesTableAccessor::~CCitiesTableAccessor()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

// Constructor / Destructor
// ----------------

CCitiesTable::CCitiesTable()
{
}

CCitiesTable::~CCitiesTable()
{
	this->CloseAll();
}


// Methods
// ----------------

// Public

BOOL CCitiesTable::SelectAll(CCitiesArray& oCitiesArray)
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Конструираме заявката
	const CString strQuery = _T("SELECT * FROM CITIES");

	// Изпълняваме командата
	hResult = this->Open(m_oSession, strQuery);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->CloseAll();

		return FALSE;
	}

	hResult = this->MoveNext();
	// Прочитаме всички данни и записваме всичко в oCitiesArray
	while (hResult == S_OK)
	{
		oCitiesArray.Add(new CITIES(m_recCity));
		hResult = this->MoveNext();
	}

	// Проверяваме дали сме прочели всички данни
	if (hResult != DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE, hResult);

		this->CloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

BOOL CCitiesTable::SelectWhereID(const long lID, CITIES& recCity)
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

		this->CloseAll();

		return FALSE;
	}

	// Присвояване на стойностите от БД върху подадения запис в програмата
	recCity = m_recCity;

	// Затваряне на командата
	this->Close();

	return TRUE;
}

BOOL CCitiesTable::UpdateWhereID(const long lID, CITIES& recCity)
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Започваме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

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

		this->CloseAll();

		return FALSE;
	}

	// Проверка на версиите - на подадения запис и този от базата
	if (m_recCity.lUpdateCounter != recCity.lUpdateCounter)
	{
		TRACE(ERROR_VERSION_CONTROL_MESSAGE, hResult, lID);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Инкрементиране на брояча за версиите
	recCity.lUpdateCounter++;
	
	// Записване на новите стойности върху записа от базата данни
	m_recCity = recCity;

	// Записване на новите стойности в базата данни
	hResult = this->SetData(SECOND_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(ERROR_SET_DATA_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Приключване на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Затваряне на командата
	Close();

	return TRUE;
}

BOOL CCitiesTable::Insert(CITIES& recCity)
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Започваме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->PrepareDbForUpdate(oUpdateDBPropSet);
	oUpdateDBPropSet.AddProperty(DBPROP_SERVERDATAONINSERT, true);

	// Конструиране на заявката
	const CString strQuery = _T("SELECT TOP 0 * FROM CITIES WITH (SERIALIZABLE)");

	// Изпълняване на заявката
	hResult = this->Open(m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Преместване към последния запис
	hResult = this->MoveLast();
	if (FAILED(hResult))
	{
		TRACE(ERROR_MOVE_LAST_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Подготвяне на подадения обект/запис за изпращане към БД
	m_recCity = recCity;

	// Вмъкване в БД
	hResult = CRowset::Insert(SECOND_ACCESSOR, true);
	if (FAILED(hResult))
	{
		TRACE(ERROR_INSERT_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	hResult = GetData(FIRST_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(ERROR_GETTING_RESULTING_ID_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Записване на новия запис (от БД) върху подадения обект
	recCity = m_recCity;
	
	// Приключване на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

BOOL CCitiesTable::DeleteWhereID(const long lID)
{
	HRESULT hResult = S_OK;

	hResult = OpenDataSourceAndSession();
	if (FAILED(hResult))
	{
		TRACE(ERROR_OpenDataSourceAndSession_MESSAGE);

		return FALSE;
	}

	// Започваме транзакция
	hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(ERROR_STARTING_TRANSACTION_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->PrepareDbForUpdate(oUpdateDBPropSet);

	hResult = ExecuteCommandSelectWhereId(lID, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_ExecuteCommandGetRequestedRecord_MESSAGE);

		this->CloseAll();

		return FALSE;
	}

	// Изтриване на записа
	hResult = this->Delete();
	if (FAILED(hResult))
	{
		TRACE(ERROR_DELETE_MESSAGE, lID, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Приключване на транзакция
	hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMIT_MESSAGE, hResult);

		m_oSession.Abort();

		this->CloseAll();

		return FALSE;
	}

	// Затваряне на командата
	this->Close();

	return TRUE;
}

// Private

void CCitiesTable::SetUpDBInit(CDBPropSet& oDBPropSet)
{
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("PJS-PC-WIN10"));	// сървър
	oDBPropSet.AddProperty(DBPROP_AUTH_USERID, _T("PJS"));			// потребител
	oDBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, _T("123"));			// парола
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("CSOFT_PHONEBOOK"));	// база данни
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));
}

HRESULT CCitiesTable::OpenDataSource()
{
	if (!m_oDataSource.m_spInit)
	{
		CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
		this->SetUpDBInit(oDBPropSet);
		return m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	}

	return S_OK;
}

HRESULT CCitiesTable::OpenSession()
{
	if (!m_oSession.m_spOpenRowset)
	{
		return m_oSession.Open(m_oDataSource);
	}

	return S_OK;
}

HRESULT CCitiesTable::OpenDataSourceAndSession()
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

		this->CloseAll();

		return FALSE;
	}

	return hResult;
}

void CCitiesTable::CloseAll()
{
	this->Close();
	m_oSession.Close();
	m_oDataSource.Close();
}

void CCitiesTable::PrepareDbForUpdate(CDBPropSet& oUpdateDBPropSet)
{
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
}

HRESULT CCitiesTable::ExecuteCommandSelectWhereId(const long lID, CDBPropSet* pUpdateDBPropSet, const TCHAR* pszLockType)
{
	CString strQuery;
	// конструиране на заявката
	strQuery.Format(_T("SELECT * FROM CITIES %s WHERE ID = %d"), pszLockType ? pszLockType : _T(""), lID);

	// Изпълняваме командата
	HRESULT hResult = this->Open(m_oSession, strQuery, pUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(ERROR_COMMAND_OPEN_MESSAGE, hResult, strQuery.GetString());

		this->CloseAll();

		return FALSE;
	}

	// Взимане на записа
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(ERROR_MOVE_FIRST_MESSAGE, hResult);

		this->CloseAll();

		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(ERROR_NO_RECORD_WITH_ID_MESSAGE, strQuery.GetString(), hResult);

		this->CloseAll();

		return FALSE;
	}

	return hResult;
}
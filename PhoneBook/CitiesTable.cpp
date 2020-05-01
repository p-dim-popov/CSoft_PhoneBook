#include "pch.h"
#include "CitiesTable.h"

#define SECOND_ACCESSOR 1
#define ERROR_COMMIT_MESSAGE "Error ending transaction. Error: %d\n"
#define ERROR_DATA_SOURCE_OPEN_MESSAGE "Unable to connect to SQL Server database. Error: %d\n"
#define ERROR_SESSION_OPEN_MESSAGE "Unable to open session. Error: %d\n"
#define ERROR_STARTING_TRANSACTION_MESSAGE "Error starting transaction. Error: %d.\n"
#define ERROR_COMMAND_OPEN_MESSAGE "Error executing query. Error: %d. Query: %s\n"
#define ERROR_MOVE_LAST_MESSAGE "Error moving to last record. Error: %d.\n"
#define ERROR_INSERT_MESSAGE "Unable to insert. Error: %d\n"
#define ERROR_MOVE_FIRST_MESSAGE "Error moving to first record. Error: %d.\n"
#define ERROR_DELETE_MESSAGE "Unable to delete record with id: %d. Error: %d.\n"
#define ERROR_VERSION_CONTROL_MESSAGE "Error in version control - Update Counters does not match. Error: %d, Query: %s.\n"
#define ERROR_SET_DATA_MESSAGE "Error updating record. Error: %d.\n"
#define ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE "Failed - END OF ROWSET not reached. Error: %d.\n"
#define ERROR_NO_RECORD_WITH_ID_MESSAGE "Error - no record with id: %d. HRESULT: %d.\n"


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
	this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);
}

// Methods
// ----------------

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

HRESULT CCitiesTable::OpenDataSource(CDataSource& oDataSource)
{
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	this->SetUpDBInit(oDBPropSet);
	const HRESULT hResult = oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);

	return hResult;
}

void CCitiesTable::CloseAll(CDataSource* oDataSource, CSession* oSession, CCitiesTable* oCitiesTable)
{
	if (oCitiesTable)
	{
		oCitiesTable->Close();
	}

	if (oSession)
	{
		oSession->Close();
	}
	
	if (oDataSource)
	{
		oDataSource->Close();
	}
}

void CCitiesTable::SetUpRowset(CDBPropSet& oUpdateDBPropSet)
{
	oUpdateDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oUpdateDBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

}

// Public

BOOL CCitiesTable::SelectAll(CCitiesArray& oCitiesArray)
{
	HRESULT hResult = S_OK;
	
	// Свързваме се към базата данни, ако не сме се вързали
	if (!this->m_oDataSource.m_spInit)
	{
		hResult = OpenDataSource(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_DATA_SOURCE_OPEN_MESSAGE), hResult);

			return FALSE;
		}
	}

	// Отваряме сесия, ако няма отворена
	if (!this->m_oSession.m_spOpenRowset)
	{
		hResult = this->m_oSession.Open(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_SESSION_OPEN_MESSAGE), hResult);

			this->CloseAll(&this->m_oDataSource);

			return FALSE;
		}
	}
	
	// Конструираме заявката
	const CString strQuery = _T("SELECT * FROM CITIES");
	
	// Изпълняваме командата
	hResult = this->Open(this->m_oSession, strQuery);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMAND_OPEN_MESSAGE), hResult, strQuery.GetString());

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);
		
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
		TRACE(_T(ERROR_END_OF_ROWSET_NOT_REACHED_MESSAGE), hResult);

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Затваряне на командата
	this->Close();
	
	return TRUE;
}

BOOL CCitiesTable::SelectWhereID(const long lID, CITIES& recCity)
{
	HRESULT hResult = S_OK;

	// Свързваме се към базата данни, ако не сме се вързали
	if (!this->m_oDataSource.m_spInit)
	{
		hResult = OpenDataSource(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_DATA_SOURCE_OPEN_MESSAGE), hResult);

			return FALSE;
		}
	}

	// Отваряме сесия, ако няма отворена
	if (!this->m_oSession.m_spOpenRowset)
	{
		hResult = this->m_oSession.Open(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_SESSION_OPEN_MESSAGE), hResult);

			this->CloseAll(&this->m_oDataSource);

			return FALSE;
		}
	}

	// Конструираме заявката
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE ID = %d"), lID);
	
	// Изпълняваме командата
	hResult = this->Open(this->m_oSession, strQuery);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMAND_OPEN_MESSAGE), hResult, strQuery.GetString());

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);
		
		return FALSE;
	}

	// Взимане на записа
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_MOVE_FIRST_MESSAGE), hResult);

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(_T(ERROR_NO_RECORD_WITH_ID_MESSAGE), lID, hResult);

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Присвояване на стойностите от БД върху подадения запис в програмата
	recCity = this->m_recCity;

	// Затваряне на командата
	this->Close();
	
	return TRUE;
}

BOOL CCitiesTable::UpdateWhereID(const long lID, const CITIES& recCity)
{
	HRESULT hResult = S_OK;

	// Свързваме се към базата данни, ако не сме се вързали
	if (!this->m_oDataSource.m_spInit)
	{
		hResult = OpenDataSource(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_DATA_SOURCE_OPEN_MESSAGE), hResult);

			return FALSE;
		}
	}

	// Отваряме сесия, ако няма отворена
	if (!this->m_oSession.m_spOpenRowset)
	{
		hResult = this->m_oSession.Open(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_SESSION_OPEN_MESSAGE), hResult);

			this->CloseAll(&this->m_oDataSource);

			return FALSE;
		}
	}

	// Започваме транзакция
	hResult = this->m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_STARTING_TRANSACTION_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);
		
		return FALSE;
	}

	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->SetUpRowset(oUpdateDBPropSet);
	
	CString strQuery;
	// конструиране на заявката
	strQuery.Format(_T("SELECT * FROM CITIES  WITH (SERIALIZABLE) WHERE ID = %d"), lID);
	
	// изпълняване на командатa
	hResult = this->Open(this->m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMAND_OPEN_MESSAGE), hResult, strQuery.GetString());

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);
		
		return FALSE;
	}
	
	// взимане на поискания запис
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_MOVE_FIRST_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(_T(ERROR_NO_RECORD_WITH_ID_MESSAGE), lID, hResult);

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}
	
	// Проверка на версиите - на подадения запис и този от базата
	if (this->m_recCity.lUpdateCounter != recCity.lUpdateCounter)
	{
		TRACE(_T(ERROR_VERSION_CONTROL_MESSAGE), hResult, strQuery.GetString());

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Записване на новите стойности върху записа от базата данни
	this->m_recCity = recCity;

	// Инкрементиране на брояча за версиите
	this->m_recCity.lUpdateCounter++;

	// Записване на новите стойности в базата данни
	hResult = this->SetData(SECOND_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_SET_DATA_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Приключване на транзакция
	hResult = this->m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMIT_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Затваряне на командата
	this->Close();
	
	return TRUE;
}

BOOL CCitiesTable::Insert(const CITIES& recCity)
{
	HRESULT hResult = S_OK;

	// Свързваме се към базата данни, ако не сме се вързали
	if (!this->m_oDataSource.m_spInit)
	{
		hResult = OpenDataSource(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_DATA_SOURCE_OPEN_MESSAGE), hResult);

			return FALSE;
		}
	}

	// Отваряме сесия, ако няма отворена
	if (!this->m_oSession.m_spOpenRowset)
	{
		hResult = this->m_oSession.Open(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_SESSION_OPEN_MESSAGE), hResult);

			this->CloseAll(&this->m_oDataSource);

			return FALSE;
		}
	}

	// Започваме транзакция
	hResult = this->m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_STARTING_TRANSACTION_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);

		return FALSE;
	}
	
	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->SetUpRowset(oUpdateDBPropSet);

	// Конструиране на заявката
	const CString strQuery = _T("SELECT TOP 0 * FROM CITIES");

	// Изпълняване на заявката
	hResult = this->Open(this->m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMAND_OPEN_MESSAGE), hResult, strQuery.GetString());

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);

		return FALSE;
	}

	// Преместване към последния запис
	hResult = this->MoveLast();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_MOVE_LAST_MESSAGE), hResult);

		this->m_oSession.Abort();
		
		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Подготвяне на подадения обект/запис за изпращане към БД
	this->m_recCity = recCity;

	// Вмъкване в БД
	hResult = CRowset::Insert(SECOND_ACCESSOR);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_INSERT_MESSAGE), hResult);
		
		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);
		
		return FALSE;
	}

	// Приключване на транзакция
	hResult = this->m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMIT_MESSAGE), hResult);
		
		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Затваряне на командата
	this->Close();
	
	return TRUE;
}

BOOL CCitiesTable::DeleteWhereID(const long lID)
{
	HRESULT hResult = S_OK;

	// Свързваме се към базата данни, ако не сме се вързали
	hResult = OpenDataSource(this->m_oDataSource);
	if (!this->m_oDataSource.m_spInit)
	{
		hResult = OpenDataSource(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_DATA_SOURCE_OPEN_MESSAGE), hResult);

			return FALSE;
		}
	}

	// Отваряме сесия, ако няма отворена
	if (!this->m_oSession.m_spOpenRowset)
	{
		hResult = this->m_oSession.Open(this->m_oDataSource);
		if (FAILED(hResult))
		{
			TRACE(_T(ERROR_SESSION_OPEN_MESSAGE), hResult);

			this->CloseAll(&this->m_oDataSource);

			return FALSE;
		}
	}

	// Започваме транзакция
	hResult = this->m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_STARTING_TRANSACTION_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);

		return FALSE;
	}
	
	// Настройка на типа на Rowset-а
	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	this->SetUpRowset(oUpdateDBPropSet);

	// конструиране на заявката
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM CITIES WHERE ID = %d"), lID);
	
	// изпълняване на командатa
	hResult = this->Open(this->m_oSession, strQuery, &oUpdateDBPropSet);
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMAND_OPEN_MESSAGE), hResult, strQuery.GetString());

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession);
		
		return FALSE;
	}

	// Взимане на поискания запис
	hResult = this->MoveFirst();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_MOVE_FIRST_MESSAGE), hResult);

		this->m_oSession.Abort();
		
		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);
		
		return FALSE;
	}

	// Проверка дали записът съществува
	if (hResult == DB_S_ENDOFROWSET)
	{
		TRACE(_T(ERROR_NO_RECORD_WITH_ID_MESSAGE), lID, hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Изтриване на записа
	hResult = this->Delete();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_DELETE_MESSAGE), lID, hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession,this);
		
		return FALSE;
	}

	// Приключване на транзакция
	hResult = this->m_oSession.Commit();
	if (FAILED(hResult))
	{
		TRACE(_T(ERROR_COMMIT_MESSAGE), hResult);

		this->m_oSession.Abort();

		this->CloseAll(&this->m_oDataSource, &this->m_oSession, this);

		return FALSE;
	}

	// Затваряне на командата
	this->Close();
	
	return TRUE;
}
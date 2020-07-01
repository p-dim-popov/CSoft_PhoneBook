#include "pch.h"
#include "PersonsDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDocument

// Constructor / Destructor
// ----------------

CPersonsDocument::CPersonsDocument()
{
}

CPersonsDocument::~CPersonsDocument()
{
}

IMPLEMENT_DYNCREATE(CPersonsDocument, CDocument)

// Overrides
// ----------------

BOOL CPersonsDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		TRACE(_T("CDocument::OnNewDocument() in CPersonsDocument - Failed"));
		return FALSE;
	}

	RefreshData(TRUE);

	return TRUE;
}

void CPersonsDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// Methods
// ----------------

// Public

CPersonsArray& CPersonsDocument::GetAllPersons()
{
	return m_oPersonsArray;
}

CPhoneTypesArray& CPersonsDocument::GetAllPhoneTypes()
{
	return m_oPhoneTypesArray;
}

CCitiesArray& CPersonsDocument::GetAllCities()
{
	return m_oCitiesArray;
}

CPhoneNumbersArray CPersonsDocument::GetAllPhonesForPersonByID(long lID)
{
	return m_oPhoneNumbersArray
		.Where([](PHONE_NUMBERS& recPhoneNumber, void* pID)
			-> BOOL {return recPhoneNumber.lPersonId == *static_cast<long*>(pID);}, static_cast<void*>(&lID));
}

PERSONS* CPersonsDocument::GetPersonById(long lID)
{
	PERSONS* pPerson = GetRecordFromRepositoryByID<PERSONS>(lID, m_oPersonsArray, m_oPersonsIndexesOfIds);
	ASSERT(pPerson);
	return pPerson;
}

BOOL CPersonsDocument::RefreshData(BOOL bFirstTime)
{
	if (bFirstTime)
	{
		CleanRepositories();

		BOOL bResult = m_oPersonsData.SelectAll(m_oPersonsArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all persons in document"));
			return FALSE;
		}

		bResult = m_oPersonsData.SelectAllPhoneNumbers(m_oPhoneNumbersArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all phone numbers in document"));
			return FALSE;
		}

		bResult = m_oPersonsData.SelectAllPhoneTypes(m_oPhoneTypesArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all phone types in document"));
			return FALSE;
		}

		bResult = m_oPersonsData.SelectAllCities(m_oCitiesArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all cities in document"));
			return FALSE;
		}

		// create index by id
		InitRecordsIndexMap<PERSONS>(m_oPersonsArray, m_oPersonsIndexesOfIds);

		return TRUE;
	}

	CPersonsArray oNewPersons;
	CMap<long, long, long, long>  oNewPersonsIndexOfIds;
	BOOL bResult = m_oPersonsData.SelectAll(oNewPersons);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}

	// add new entries
	for (INT_PTR i = 0; i < oNewPersons.GetSize(); i++)
	{
		PERSONS* pNewPerson = oNewPersons.GetAt(i);
		oNewPersonsIndexOfIds.SetAt(pNewPerson->lID, i);

		PERSONS* pOldPerson = GetRecordFromRepositoryByID<PERSONS>(pNewPerson->lID, m_oPersonsArray, m_oPersonsIndexesOfIds);
		if (!pOldPerson)
		{
			AddRecordToRepository<PERSONS>(*pNewPerson, m_oPersonsArray, m_oPersonsIndexesOfIds);
		}
		else
		{
			*pOldPerson = *pNewPerson;
		}
	}

	// remove deleted entries
	for (INT_PTR i = 0; i < m_oPersonsArray.GetSize(); i++)
	{
		PERSONS* pOldPerson = m_oPersonsArray.GetAt(i);
		long lID = -1;
		if (!oNewPersonsIndexOfIds.Lookup(pOldPerson->lID, lID))
		{
			m_oPersonsArray.RemoveAt(i);
			delete pOldPerson;
		}
	}

	InitRecordsIndexMap<PERSONS>(m_oPersonsArray, m_oPersonsIndexesOfIds);

	m_oPhoneNumbersArray.DeleteAll();
	bResult = m_oPersonsData.SelectAllPhoneNumbers(m_oPhoneNumbersArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all phone numbers in document"));
		return FALSE;
	}

	m_oPhoneTypesArray.DeleteAll();
	bResult = m_oPersonsData.SelectAllPhoneTypes(m_oPhoneTypesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all phone types in document"));
		return FALSE;
	}

	m_oCitiesArray.DeleteAll();
	bResult = m_oPersonsData.SelectAllCities(m_oCitiesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}


	return TRUE;
}

BOOL CPersonsDocument::AddPerson(PERSONS& recPerson, CPhoneNumbersArray& oPhoneNumbersArray, CPhoneTypesArray* pPhoneTypesArray, CITIES* pCity)
{
	const BOOL bResult = m_oPersonsData.InsertPersonWithData(recPerson, oPhoneNumbersArray, pPhoneTypesArray, pCity);
	if (!bResult)
	{
		TRACE(_T("Insert in document failed. Person id - %d"), recPerson.lID);
		return FALSE;
	}

	const INT_PTR nPersonIndex = AddRecordToRepository<PERSONS>(recPerson, m_oPersonsArray, m_oPersonsIndexesOfIds);
	PERSONS* pPerson = m_oPersonsArray.GetAt(nPersonIndex);

	for (INT_PTR i = 0; i < oPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oPhoneNumbersArray.GetAt(i);
		m_oPhoneNumbersArray.Add(new PHONE_NUMBERS(*pPhoneNumber));
	}

	if (pPhoneTypesArray)
	{
		for (INT_PTR i = 0; i < pPhoneTypesArray->GetSize(); i++)
		{
			PHONE_TYPES* pPhoneType = pPhoneTypesArray->GetAt(i);
			m_oPhoneTypesArray.Add(new PHONE_TYPES(*pPhoneType));
		}
	}

	if (pCity)
	{
		m_oCitiesArray.Add(new CITIES(*pCity));
	}

	CPersonsUpdateObject oPersonsUpdateObject(reinterpret_cast<DWORD_PTR>(pPerson));

	OnUpdateAllViews(OperationsCreate, &oPersonsUpdateObject);
	return TRUE;
}

void CPersonsDocument::CombineNewAndUpdatedPhoneNumbers(
	const CPhoneNumbersArray& oNewPhoneNumbersArray,
	const CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
	CPhoneNumbersArray& oPhoneNumbersArray
)
{
	const INT_PTR nNewNumbersCount = oNewPhoneNumbersArray.GetSize();
	const INT_PTR nUpdatedNumbersCount = oUpdatedPhoneNumbersArray.GetSize();

	const INT_PTR nBiggerSize = nNewNumbersCount > nUpdatedNumbersCount
		? nUpdatedNumbersCount
		: nUpdatedNumbersCount;

	for (INT_PTR i = 0; i < nBiggerSize; i++)
	{
		if (i < nNewNumbersCount)
		{
			oPhoneNumbersArray
				.Add(oNewPhoneNumbersArray.GetAt(i));
		}

		if (i < nUpdatedNumbersCount)
		{
			oPhoneNumbersArray
				.Add(oUpdatedPhoneNumbersArray.GetAt(i));
		}
	}
}

BOOL CPersonsDocument::EditPerson(
	PERSONS& recPerson,
	CPhoneNumbersArray& oDeletedPhoneNumbersArray,
	CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
	CPhoneNumbersArray& oNewPhoneNumbersArray,
	CPhoneTypesArray* pNewPhoneTypesArray,
	CITIES* pNewCity
)
{
	const BOOL bResult = m_oPersonsData
		.UpdatePersonWithData(
			recPerson,
			oDeletedPhoneNumbersArray,
			oUpdatedPhoneNumbersArray,
			oNewPhoneNumbersArray,
			pNewPhoneTypesArray,
			pNewCity
		);
	if (!bResult)
	{
		TRACE(_T("Insert in document failed. Person id - %d"), recPerson.lID);
		return FALSE;
	}

	for (INT_PTR i = 0; i < oUpdatedPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pNewPhoneNumber = oUpdatedPhoneNumbersArray.GetAt(i);
		PHONE_NUMBERS& recPhoneNumber = m_oPhoneNumbersArray
			.First([](PHONE_NUMBERS& recPhoneNumber, void* pID)
				-> BOOL { return recPhoneNumber.lID == *static_cast<long*>(pID); },
				&pNewPhoneNumber->lID
			);

		recPhoneNumber = *pNewPhoneNumber;
	}

	for (INT_PTR i = 0; i < oNewPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumber = oNewPhoneNumbersArray.GetAt(i);
		m_oPhoneNumbersArray.Add(new PHONE_NUMBERS(*pPhoneNumber));
		delete pPhoneNumber;
	}

	for (INT_PTR i = 0; i < oDeletedPhoneNumbersArray.GetSize(); i++)
	{
		PHONE_NUMBERS* pDeletedPhoneNumber = oDeletedPhoneNumbersArray.GetAt(i);

		const INT_PTR nIndex = m_oPhoneNumbersArray
			.IndexOf([](PHONE_NUMBERS& recPhoneNumber, void* pID)
				-> BOOL { return recPhoneNumber.lID == *static_cast<long*>(pID); },
				& pDeletedPhoneNumber->lID
			);

		delete pDeletedPhoneNumber;

		if (nIndex > -1)
		{
			m_oPhoneNumbersArray.RemoveAt(nIndex);
		}
	}

	if (pNewPhoneTypesArray)
	{
		for (INT_PTR i = 0; i < pNewPhoneTypesArray->GetSize(); i++)
		{
			PHONE_TYPES* pPhoneType = pNewPhoneTypesArray->GetAt(i);
			m_oPhoneTypesArray.Add(new PHONE_TYPES(*pPhoneType));
			delete pPhoneType;
		}
	}

	if (pNewCity)
	{
		m_oCitiesArray.Add(new CITIES(*pNewCity));
	}

	PERSONS* pPerson = GetRecordFromRepositoryByID<PERSONS>(recPerson.lID, m_oPersonsArray, m_oPersonsIndexesOfIds);

	*pPerson = recPerson;
	CPersonsUpdateObject oPersonsUpdateObject(reinterpret_cast<DWORD_PTR>(pPerson));
	OnUpdateAllViews(OperationsUpdate, &oPersonsUpdateObject);
	return TRUE;
}

BOOL CPersonsDocument::DeleteAllPhonesForPersonByID(const long lID, CPhoneNumbersArray& oPhoneNumbersToDelete)
{
	for (INT_PTR i = 0; i < oPhoneNumbersToDelete.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumberToDelete = oPhoneNumbersToDelete.GetAt(i);

		const BOOL bResult = m_oPersonsData.DeletePhoneNumberWhereID(pPhoneNumberToDelete->lID);
		if (!bResult)
		{
			TRACE(_T("Deletion from database returned error. Phone number id: %d"), pPhoneNumberToDelete->lID);
			return FALSE;
		}

		const INT_PTR nPhoneNumberIndex = m_oPhoneNumbersArray
			.IndexOf(
				[](PHONE_NUMBERS& recPhoneNumber, void* pID)->BOOL { return recPhoneNumber.lPersonId = *static_cast<long*>(pID); }, &pPhoneNumberToDelete->lID
			);

		const DWORD_PTR dwDeletedPhoneNumberAddress = reinterpret_cast<DWORD_PTR>(m_oPhoneNumbersArray.GetAt(nPhoneNumberIndex));
		delete m_oPhoneNumbersArray.GetAt(nPhoneNumberIndex);
		m_oPhoneNumbersArray.RemoveAt(nPhoneNumberIndex);
	}

	return TRUE;
}

BOOL CPersonsDocument::DeletePerson(const PERSONS& recPerson)
{
	CPhoneNumbersArray oPersonPhoneNumbersArray(GetAllPhonesForPersonByID(recPerson.lID));

	BOOL bResult = DeleteAllPhonesForPersonByID(recPerson.lID, oPersonPhoneNumbersArray);
	if (!bResult)
	{
		TRACE(_T("Deletion of phone numbers returned error. Check logs."));
		return FALSE;
	}

	bResult = m_oPersonsData.DeleteWhereID(recPerson.lID);

	if (!bResult)
	{
		TRACE(_T("Deletion from database returned error. Person id: %d"), recPerson.lID);
		return FALSE;
	}

	const DWORD_PTR dwDeletedPersonAddress = RemoveRecordFromRepositoryByID<PERSONS>(recPerson.lID, m_oPersonsArray, m_oPersonsIndexesOfIds);

	if (dwDeletedPersonAddress == NULL)
	{
		TRACE(_T("Deletion from repository returned error. Person id: %d"), recPerson.lID);
		return FALSE;
	}

	CPersonsUpdateObject oPersonsUpdateObject(dwDeletedPersonAddress);

	OnUpdateAllViews(OperationsDelete, &oPersonsUpdateObject);

	return TRUE;
}

// Private

void CPersonsDocument::CleanRepositories()
{
	CleanRepository<PERSONS>(m_oPersonsArray, m_oPersonsIndexesOfIds);
	m_oPhoneNumbersArray.DeleteAll();
	m_oPhoneTypesArray.DeleteAll();
	m_oCitiesArray.DeleteAll();

}

void CPersonsDocument::OnUpdateAllViews(LPARAM lHint, CObject* pHint)
{
	SetModifiedFlag();
	UpdateAllViews(NULL, lHint, pHint);
}

void CPersonsDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPersonsDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
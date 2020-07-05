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

	this->SetTitle(_T("People"));

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

BOOL CPersonsDocument::GetAllPhonesForPersonByID(long lID, CPhoneNumbersArray& oPhoneNumbers)
{
	if (!m_oPersonsData.SelectPhoneNumbersWherePersonID(lID, oPhoneNumbers))
	{
		return FALSE;
	}

	return TRUE;
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

BOOL CPersonsDocument::AddPerson(PERSONS& recPerson, CPhoneNumbersArray& oPhoneNumbersArray)
{
	const BOOL bResult = m_oPersonsData.InsertPersonWithData(recPerson, oPhoneNumbersArray);
	if (!bResult)
	{
		TRACE(_T("Insert in document failed. Person id - %d"), recPerson.lID);
		return FALSE;
	}

	const INT_PTR nPersonIndex = AddRecordToRepository<PERSONS>(recPerson, m_oPersonsArray, m_oPersonsIndexesOfIds);
	PERSONS* pPerson = m_oPersonsArray.GetAt(nPersonIndex);

	CPersonsUpdateObject oPersonsUpdateObject(reinterpret_cast<DWORD_PTR>(pPerson));

	OnUpdateAllViews(Utilities::OperationsCreate, &oPersonsUpdateObject);
	return TRUE;
}

BOOL CPersonsDocument::EditPerson(
	PERSONS& recPerson,
	CPhoneNumbersArray& oDeletedPhoneNumbersArray,
	CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
	CPhoneNumbersArray& oNewPhoneNumbersArray
)
{
	const BOOL bResult = m_oPersonsData
		.UpdatePersonWithData(
			recPerson,
			oDeletedPhoneNumbersArray,
			oUpdatedPhoneNumbersArray,
			oNewPhoneNumbersArray
		);

	if (!bResult)
	{
		TRACE(_T("Update in document failed. Person id - %d"), recPerson.lID);
		return FALSE;
	}

	PERSONS* pPerson = GetRecordFromRepositoryByID<PERSONS>(recPerson.lID, m_oPersonsArray, m_oPersonsIndexesOfIds);

	*pPerson = recPerson;
	CPersonsUpdateObject oPersonsUpdateObject(reinterpret_cast<DWORD_PTR>(pPerson));
	OnUpdateAllViews(Utilities::OperationsUpdate, &oPersonsUpdateObject);
	return TRUE;
}

BOOL CPersonsDocument::DeleteAllPhonesForPersonByID(const long lID, CPhoneNumbersArray& oPhoneNumbersToDelete)
{
	for (INT_PTR i = 0; i < oPhoneNumbersToDelete.GetSize(); i++)
	{
		PHONE_NUMBERS* pPhoneNumberToDelete = oPhoneNumbersToDelete.GetAt(i);

		if (!m_oPersonsData.DeletePhoneNumberWhereID(pPhoneNumberToDelete->lID))
		{
			TRACE(_T("Deletion from database returned error. Phone number id: %d"), pPhoneNumberToDelete->lID);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPersonsDocument::DeletePerson(const PERSONS& recPerson)
{
	CPhoneNumbersArray oPersonPhoneNumbersArray;
	GetAllPhonesForPersonByID(recPerson.lID, oPersonPhoneNumbersArray);

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

	OnUpdateAllViews(Utilities::OperationsDelete, &oPersonsUpdateObject);

	return TRUE;
}

// Private

void CPersonsDocument::CleanRepositories()
{
	CleanRepository<PERSONS>(m_oPersonsArray, m_oPersonsIndexesOfIds);
	m_oPhoneTypesArray.DeleteAll();
	m_oCitiesArray.DeleteAll();

}

void CPersonsDocument::OnUpdateAllViews(LPARAM lHint, CObject* pHint)
{
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
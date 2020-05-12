#include "pch.h"
#include "CitiesDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

// Constructor / Destructor
// ----------------

CCitiesDocument::CCitiesDocument()
{
}

CCitiesDocument::~CCitiesDocument()
{
}

IMPLEMENT_DYNCREATE(CCitiesDocument, CDocument)

// Overrides
// ----------------

BOOL CCitiesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		TRACE(_T("CDocument::OnNewDocument() in CCitiesDocument - Failed"));
		return FALSE;
	}

	RefreshData();

	return TRUE;
}

void CCitiesDocument::Serialize(CArchive& ar)
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

CCitiesArray& CCitiesDocument::GetAllCities()
{
	return m_oCitiesArray;
}

CITIES& CCitiesDocument::GetCityById(const long lID)
{
	long nIndexOfCity = -1;

	// Търси си се дали записът съществува във вече изтелглените записи
	const BOOL bIsSuccessful = m_oCitiesIndexesOfIds.Lookup(lID, nIndexOfCity);

	ASSERT(nIndexOfCity > -1 && bIsSuccessful);

	CITIES* pCity = m_oCitiesArray.GetAt(nIndexOfCity);

	ASSERT(pCity);

	return *pCity;
}

BOOL CCitiesDocument::RefreshData()
{
	/*
	CCitiesArray oCitiesArray;
	CCitiesArray oCitiesNewArray;

	const BOOL bResult = m_oCitiesData.SelectAll(oCitiesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}

	// save only new and updated ones in oCitiesNewArray
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = oCitiesArray.GetAt(i);
		CITIES* pCityFromRepo = GetCityFromRepositoryByID(pCity->lID);

		// if city is new
		if (!pCityFromRepo)
		{
			const INT_PTR nIndex = oCitiesNewArray.Add(new CITIES(*pCity));
			CITIES* pNewCity = oCitiesNewArray.GetAt(nIndex);
			CCitiesUpdateObject oCitiesUpdateObject = reinterpret_cast<DWORD_PTR>(pNewCity);
			OnUpdateAllViews(OperationsCreate, &oCitiesUpdateObject);
			continue;
		}

		// if city is not updated
		if (*pCity == *pCityFromRepo)
		{
			oCitiesNewArray.Add(pCityFromRepo);
			continue;
		}

		// if city is updated
		*pCityFromRepo = *pCity;
		oCitiesNewArray.Add(pCityFromRepo);
		CCitiesUpdateObject oCitiesUpdateObject = reinterpret_cast<DWORD_PTR>(pCityFromRepo);
		OnUpdateAllViews(OperationsUpdate, &oCitiesUpdateObject);
	}

	CArray<long, long> oIndexesToSave;

	// save only deleted ones' index in oIndexesToDelete
	for (INT_PTR i = 0; i < oCitiesNewArray.GetCount(); i++)
	{
		CITIES* pNewCity = oCitiesNewArray.GetAt(i);
		CITIES* pOldCity = GetCityFromRepositoryByID(pNewCity->lID);

		if (pOldCity)
		{
			long lIndex = -1;
			m_oCitiesIndexesOfIds.Lookup(pOldCity->lID, lIndex);
			oIndexesToSave.Add(lIndex);
		}
	}

	std::sort(oIndexesToSave.GetData(), oIndexesToSave.GetData() + oIndexesToSave.GetSize());

	// leave deleted ones only
	for (INT_PTR i = oIndexesToSave.GetCount() - 1; i > 0 ; --i)
	{
		const int nIndex = oIndexesToSave.GetAt(i);
		m_oCitiesArray.RemoveAt(nIndex);
	}

	// allocate deleted ones memory
	m_oCitiesArray.DeleteAll();

	// remove old indexes
	m_oCitiesIndexesOfIds.RemoveAll();

	// copy new ones and updated ones in m_oCitiesArray and create index by id
	for (INT_PTR i = 0; i < oCitiesNewArray.GetCount(); i++)
	{
		CITIES* pCurrent = oCitiesNewArray.GetAt(i);
		const int nIndex = m_oCitiesArray.Add(pCurrent);
		m_oCitiesIndexesOfIds.SetAt(pCurrent->lID, nIndex);
	}

	// remove pointers so that memory won't be auto allocated
	oCitiesNewArray.RemoveAll();
	oCitiesArray.DeleteAll();
	*/

	CleanRepository();

	const BOOL bResult = m_oCitiesData.SelectAll(m_oCitiesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}

	// create index by id
	BuildCitiesIndexMap();

	return TRUE;
}

BOOL CCitiesDocument::EditCity(CITIES& recCity)
{
	const BOOL bResult = m_oCitiesData.UpdateWhereID(recCity.lID, recCity);

	if (!bResult)
	{
		TRACE(_T("Edit city in document failed. City id - %d"), recCity.lID);
		return FALSE;
	}

	CITIES* pCity = GetCityFromRepositoryByID(recCity.lID);

	if (!pCity)
	{
		TRACE(_T("Finding city in document failed. City id - %d"), recCity.lID);
		return FALSE;
	}

	*pCity = recCity;

	CCitiesUpdateObject oCitiesUpdateObject(reinterpret_cast<DWORD_PTR>(pCity));

	OnUpdateAllViews(OperationsUpdate, &oCitiesUpdateObject);

	return TRUE;
}

BOOL CCitiesDocument::AddCity(CITIES& recCity)
{
	recCity.lID = -1;
	const BOOL bResult = m_oCitiesData.InsertCity(recCity);

	if (!bResult || recCity.lID == -1)
	{
		TRACE(_T("Insert in document failed. City id - %d"), recCity.lID);
		return FALSE;
	}

	const INT_PTR nCityIndex = AddCityToRepository(recCity);
	CITIES* pCity = m_oCitiesArray.GetAt(nCityIndex);

	CCitiesUpdateObject oCitiesUpdateObject(reinterpret_cast<DWORD_PTR>(pCity));

	OnUpdateAllViews(OperationsCreate, &oCitiesUpdateObject);
	return TRUE;
}

BOOL CCitiesDocument::DeleteCity(const CITIES& recCity)
{
	const BOOL bResult = m_oCitiesData.DeleteWhereID(recCity.lID);

	if (!bResult)
	{
		return FALSE;
	}

	const DWORD_PTR dwDeletedCityAddress = RemoveCityFromRepositoryByID(recCity.lID);

	if (dwDeletedCityAddress == -1)
	{
		TRACE(_T("Deletion from repository returned error. City id: %d"), recCity.lID);
	}

	CCitiesUpdateObject oCitiesUpdateObject(dwDeletedCityAddress);

	OnUpdateAllViews(OperationsDelete, &oCitiesUpdateObject);
	return TRUE;
}

void CCitiesDocument::CleanRepository()
{
	m_oCitiesArray.DeleteAll();
	m_oCitiesIndexesOfIds.RemoveAll();
}

void CCitiesDocument::OnUpdateAllViews(LPARAM lHint, CObject* pHint)
{
	SetModifiedFlag();
	UpdateAllViews(NULL, lHint, pHint);
}

INT_PTR CCitiesDocument::AddCityToRepository(const CITIES& recCity)
{
	const INT_PTR nIndexOfCity = m_oCitiesArray.Add(new CITIES(recCity));
	m_oCitiesIndexesOfIds.SetAt(recCity.lID, nIndexOfCity);
	return nIndexOfCity;
}

DWORD_PTR CCitiesDocument::RemoveCityFromRepositoryByID(const long lID)
{
	long lIndex = -1;
	m_oCitiesIndexesOfIds.Lookup(lID, lIndex);

	if (lIndex < 0 || lIndex >= m_oCitiesArray.GetCount())
	{
		TRACE(_T("City not found in document, but claimed. City id - %d"), lID);
		return NULL;
	}

	CITIES* pCity = m_oCitiesArray.GetAt(lIndex);

	if (!pCity)
	{
		TRACE(_T("City not found in document, but exists in index map. City id - %d"), lID);
		return NULL;
	}

	const DWORD_PTR dwDeletedCityAddress = reinterpret_cast<DWORD_PTR>(pCity);
	delete pCity;
	m_oCitiesArray.RemoveAt(lIndex);

	//TODO: optimize to replace only needed indexes later
	BuildCitiesIndexMap();

	return dwDeletedCityAddress;
}

CITIES* CCitiesDocument::GetCityFromRepositoryByID(const long lID)
{
	long lIndex = -1;

	m_oCitiesIndexesOfIds.Lookup(lID, lIndex);

	if (lIndex < 0 || lIndex >= m_oCitiesArray.GetCount())
	{
		TRACE(_T("City not found in document, but claimed. City id - %d"), lID);
		return NULL;
	}

	CITIES* pCity = m_oCitiesArray.GetAt(lIndex);

	if (!pCity)
	{
		TRACE(_T("City not found in document, but exists in index map. City id - %d"), lID);
		return NULL;
	}

	return pCity;
}

void CCitiesDocument::BuildCitiesIndexMap()
{
	m_oCitiesIndexesOfIds.RemoveAll();
	for (INT_PTR i = 0; i < m_oCitiesArray.GetCount(); i++)
	{
		CITIES* pCurrentCity = m_oCitiesArray.GetAt(i);
		m_oCitiesIndexesOfIds.SetAt(pCurrentCity->lID, i);
	}
}

void CCitiesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
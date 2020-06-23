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
	if (!m_oCitiesArray.GetSize())
	{
		CleanRepository();

		const BOOL bResult = m_oCitiesData.SelectAll(m_oCitiesArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all cities in document"));
			return FALSE;
		}

		// create index by id
		InitCitiesIndexMap();

		return TRUE;
	}

	CCitiesArray oNewCities;
	CMap<long, long, long, long>  oNewCitiesIndexOfIds;
	const BOOL bResult = m_oCitiesData.SelectAll(oNewCities);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}

	for (INT_PTR i = 0; i < oNewCities.GetSize(); i++)
	{
		CITIES* pNewCity = oNewCities.GetAt(i);
		oNewCitiesIndexOfIds.SetAt(pNewCity->lID, i);

		CITIES* pOldCity = GetCityFromRepositoryByID(pNewCity->lID);
		if (!pOldCity)
		{
			AddCityToRepository(*pNewCity);
		}
		else
		{
			*pOldCity = *pNewCity;
		}
	}

	for (INT_PTR i = 0; i < m_oCitiesArray.GetSize(); i++)
	{
		CITIES* pOldCity = m_oCitiesArray.GetAt(i);
		long lID = -1;
		if (!oNewCitiesIndexOfIds.Lookup(pOldCity->lID, lID))
		{
			m_oCitiesArray.RemoveAt(i);
			delete pOldCity;
		}
	}

	InitCitiesIndexMap();

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

	OnUpdateAllViews(Operations::OperationsUpdate, &oCitiesUpdateObject);

	return TRUE;
}

BOOL CCitiesDocument::AddCity(CITIES& recCity)
{
	const BOOL bResult = m_oCitiesData.InsertCity(recCity);

	if (!bResult)
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
		TRACE(_T("Deletion from database returned error. City id: %d"), recCity.lID);
		return FALSE;
	}

	const DWORD_PTR dwDeletedCityAddress = RemoveCityFromRepositoryByID(recCity.lID);

	if (dwDeletedCityAddress == NULL)
	{
		TRACE(_T("Deletion from repository returned error. City id: %d"), recCity.lID);
		return FALSE;
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
	InitCitiesIndexMap();

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

void CCitiesDocument::InitCitiesIndexMap()
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
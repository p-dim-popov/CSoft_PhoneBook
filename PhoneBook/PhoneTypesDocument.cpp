#include "pch.h"
#include "PhoneTypesDocument.h"

#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDocument

// Constructor / Destructor
// ----------------

CPhoneTypesDocument::CPhoneTypesDocument()
{
}

CPhoneTypesDocument::~CPhoneTypesDocument()
{
}

IMPLEMENT_DYNCREATE(CPhoneTypesDocument, CDocument)

// Overrides
// ----------------

BOOL CPhoneTypesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		TRACE(_T("CDocument::OnNewDocument() in CPhoneTypesDocument - Failed"));
		return FALSE;
	}

	RefreshData();

	return TRUE;
}

void CPhoneTypesDocument::Serialize(CArchive& ar)
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

CPhoneTypesArray& CPhoneTypesDocument::GetAllPhoneTypes()
{
	return m_oPhoneTypesArray;
}

PHONE_TYPES& CPhoneTypesDocument::GetPhoneTypeById(const long lID)
{
	long nIndexOfPhoneType = -1;

	// Търси си се дали записът съществува във вече изтелглените записи
	const BOOL bIsSuccessful = m_oPhoneTypesIndexesOfIds.Lookup(lID, nIndexOfPhoneType);

	ASSERT(nIndexOfPhoneType > -1 && bIsSuccessful);

	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(nIndexOfPhoneType);

	ASSERT(pPhoneType);

	return *pPhoneType;
}

BOOL CPhoneTypesDocument::RefreshData()
{
	if (!m_oPhoneTypesArray.GetSize())
	{
		CleanRepository();

		const BOOL bResult = m_oPhoneTypesData.SelectAll(m_oPhoneTypesArray);
		if (!bResult)
		{
			TRACE(_T("Error getting all cities in document"));
			return FALSE;
		}

		// create index by id
		InitPhoneTypesIndexMap();

		return TRUE;
	}

	CPhoneTypesArray oNewPhoneTypes;
	CMap<long, long, long, long>  oNewPhoneTypesIndexOfIds;
	const BOOL bResult = m_oPhoneTypesData.SelectAll(oNewPhoneTypes);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		return FALSE;
	}

	for (INT_PTR i = 0; i < oNewPhoneTypes.GetSize(); i++)
	{
		PHONE_TYPES* pNewPhoneType = oNewPhoneTypes.GetAt(i);
		oNewPhoneTypesIndexOfIds.SetAt(pNewPhoneType->lID, i);

		PHONE_TYPES* pOldPhoneType = GetPhoneTypeFromRepositoryByID(pNewPhoneType->lID);
		if (!pOldPhoneType)
		{
			AddPhoneTypeToRepository(*pNewPhoneType);
		}
		else
		{
			*pOldPhoneType = *pNewPhoneType;
		}
	}

	for (INT_PTR i = 0; i < m_oPhoneTypesArray.GetSize(); i++)
	{
		PHONE_TYPES* pOldPhoneType = m_oPhoneTypesArray.GetAt(i);
		long lID = -1;
		if (!oNewPhoneTypesIndexOfIds.Lookup(pOldPhoneType->lID, lID))
		{
			m_oPhoneTypesArray.RemoveAt(i);
			delete pOldPhoneType;
		}
	}

	InitPhoneTypesIndexMap();

	return TRUE;
}

BOOL CPhoneTypesDocument::EditPhoneType(PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesData.UpdateWhereID(recPhoneType.lID, recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Edit phone type in document failed. PhoneType id - %d"), recPhoneType.lID);
		return FALSE;
	}

	PHONE_TYPES* pPhoneType = GetPhoneTypeFromRepositoryByID(recPhoneType.lID);

	if (!pPhoneType)
	{
		TRACE(_T("Finding phone type in document failed. PhoneType id - %d"), recPhoneType.lID);
		return FALSE;
	}

	*pPhoneType = recPhoneType;

	CPhoneTypesUpdateObject oPhoneTypesUpdateObject(reinterpret_cast<DWORD_PTR>(pPhoneType));

	OnUpdateAllViews(OperationsUpdate, &oPhoneTypesUpdateObject);

	return TRUE;
}

BOOL CPhoneTypesDocument::AddPhoneType(PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesData.InsertPhoneType(recPhoneType);

	if (!bResult)
	{
		TRACE(_T("Insert in document failed. PhoneType id - %d"), recPhoneType.lID);
		return FALSE;
	}

	const INT_PTR nPhoneTypeIndex = AddPhoneTypeToRepository(recPhoneType);
	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(nPhoneTypeIndex);

	CPhoneTypesUpdateObject oPhoneTypesUpdateObject(reinterpret_cast<DWORD_PTR>(pPhoneType));

	OnUpdateAllViews(OperationsCreate, &oPhoneTypesUpdateObject);
	return TRUE;
}

BOOL CPhoneTypesDocument::DeletePhoneType(const PHONE_TYPES& recPhoneType)
{
	const BOOL bResult = m_oPhoneTypesData.DeleteWhereID(recPhoneType.lID);

	if (!bResult)
	{
		TRACE(_T("Deletion from database returned error. PhoneType id: %d"), recPhoneType.lID);
		return FALSE;
	}

	const DWORD_PTR dwDeletedPhoneTypeAddress = RemovePhoneTypeFromRepositoryByID(recPhoneType.lID);

	if (dwDeletedPhoneTypeAddress == NULL)
	{
		TRACE(_T("Deletion from repository returned error. PhoneType id: %d"), recPhoneType.lID);
		return FALSE;
	}

	CPhoneTypesUpdateObject oPhoneTypesUpdateObject(dwDeletedPhoneTypeAddress);

	OnUpdateAllViews(OperationsDelete, &oPhoneTypesUpdateObject);
	return TRUE;
}

void CPhoneTypesDocument::CleanRepository()
{
	m_oPhoneTypesArray.DeleteAll();
	m_oPhoneTypesIndexesOfIds.RemoveAll();
}

void CPhoneTypesDocument::OnUpdateAllViews(LPARAM lHint, CObject* pHint)
{
	SetModifiedFlag();
	UpdateAllViews(NULL, lHint, pHint);
}

INT_PTR CPhoneTypesDocument::AddPhoneTypeToRepository(const PHONE_TYPES& recPhoneType)
{
	const INT_PTR nIndexOfPhoneType = m_oPhoneTypesArray.Add(new PHONE_TYPES(recPhoneType));
	m_oPhoneTypesIndexesOfIds.SetAt(recPhoneType.lID, nIndexOfPhoneType);
	return nIndexOfPhoneType;
}

DWORD_PTR CPhoneTypesDocument::RemovePhoneTypeFromRepositoryByID(const long lID)
{
	long lIndex = -1;
	m_oPhoneTypesIndexesOfIds.Lookup(lID, lIndex);

	if (lIndex < 0 || lIndex >= m_oPhoneTypesArray.GetCount())
	{
		TRACE(_T("PhoneType not found in document, but claimed. PhoneType id - %d"), lID);
		return NULL;
	}

	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(lIndex);

	if (!pPhoneType)
	{
		TRACE(_T("PhoneType not found in document, but exists in index map. PhoneType id - %d"), lID);
		return NULL;
	}

	const DWORD_PTR dwDeletedPhoneTypeAddress = reinterpret_cast<DWORD_PTR>(pPhoneType);
	delete pPhoneType;
	m_oPhoneTypesArray.RemoveAt(lIndex);

	//TODO: optimize to replace only needed indexes later
	InitPhoneTypesIndexMap();

	return dwDeletedPhoneTypeAddress;
}

PHONE_TYPES* CPhoneTypesDocument::GetPhoneTypeFromRepositoryByID(const long lID)
{
	long lIndex = -1;

	m_oPhoneTypesIndexesOfIds.Lookup(lID, lIndex);

	if (lIndex < 0 || lIndex >= m_oPhoneTypesArray.GetCount())
	{
		TRACE(_T("PhoneType not found in document, but claimed. PhoneType id - %d"), lID);
		return NULL;
	}

	PHONE_TYPES* pPhoneType = m_oPhoneTypesArray.GetAt(lIndex);

	if (!pPhoneType)
	{
		TRACE(_T("PhoneType not found in document, but exists in index map. PhoneType id - %d"), lID);
		return NULL;
	}

	return pPhoneType;
}

void CPhoneTypesDocument::InitPhoneTypesIndexMap()
{
	m_oPhoneTypesIndexesOfIds.RemoveAll();
	for (INT_PTR i = 0; i < m_oPhoneTypesArray.GetCount(); i++)
	{
		PHONE_TYPES* pCurrentPhoneType = m_oPhoneTypesArray.GetAt(i);
		m_oPhoneTypesIndexesOfIds.SetAt(pCurrentPhoneType->lID, i);
	}
}

void CPhoneTypesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhoneTypesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
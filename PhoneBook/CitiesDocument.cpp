#include "pch.h"
#include "CitiesDocument.h"


CCitiesDocument::CCitiesDocument()
{
}

CCitiesDocument::~CCitiesDocument()
{
}

IMPLEMENT_DYNCREATE(CCitiesDocument, CDocument)

BOOL CCitiesDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
	{
		TRACE(_T("CDocument::OnNewDocument() - Failed"));
		return FALSE;
	}
	const BOOL bResult = m_oCitiesData.GetAllCities(m_oCitiesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		//TODO: msg
		return FALSE;
	}

	for (INT_PTR i = 0; i < m_oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = m_oCitiesArray.GetAt(i);
		m_oCitiesIndexesOfIds.SetAt(pCity->lID, i);
	}
	
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

CCitiesArray* CCitiesDocument::GetAllCities()
{
	return &m_oCitiesArray;
}

CITIES* CCitiesDocument::GetCityById(long lId)
{
	CITIES* pCity;
	long nIndexOfCity;
	
	BOOL bIsSuccessful = m_oCitiesIndexesOfIds.Lookup(lId, nIndexOfCity);
	if (bIsSuccessful)
	{
		pCity = m_oCitiesArray.GetAt(nIndexOfCity);
		return pCity;
	}

	CITIES recCity;
	bIsSuccessful = m_oCitiesData.GetCityById(lId, recCity);
	if (!bIsSuccessful)
	{
		TRACE("Error: could not get city from data in document.");
		return nullptr;
	}

	nIndexOfCity = AddCityToRepository(recCity);

	pCity = m_oCitiesArray.GetAt(nIndexOfCity);
	return pCity;
}

INT_PTR CCitiesDocument::AddCityToRepository(CITIES& recCity)
{
	const INT_PTR nIndexOfCity = m_oCitiesArray.Add(new CITIES(recCity));
	m_oCitiesIndexesOfIds.SetAt(recCity.lID, nIndexOfCity);
	return nIndexOfCity;
}

void CCitiesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

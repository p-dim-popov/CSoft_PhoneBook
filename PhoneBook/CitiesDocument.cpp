#include "pch.h"
#include "CitiesDocument.h"

#define CITIES_CREATE_ERROR 0
#define CITIES_READ_ERROR 1
#define CITIES_UPDATE_ERROR 2
#define CIITES_DELETE_ERROR 3

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

bool CCitiesDocument::EditCity(CITIES& recCity)
{
	const BOOL bResult = m_oCitiesData.UpdateCityWithId(recCity.lID, recCity);

	if (!bResult)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_UPDATE_ERROR,
			_T("Възникна грешка. \
Възможна причина: записът вече е обновен преди настъпване на настоящите промени. \
Моля обновете вашите данни и опитайте да направите редакциите отново. \
Ако сте сигурни, че това не е проблемът, може да опитате отново."));
		
		if (!bShouldRetry)
		{
			return false;	
		}

		EditCity(recCity);
	}

	return true;
}

bool CCitiesDocument::AddCityToDb(CITIES& recCity)
{
	const BOOL bResult = m_oCitiesData.InsertCity(recCity);

	if (!bResult)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_CREATE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \
Ако смятате, че проблемът не е от вас, може да опитате отново."));
		
		if (!bShouldRetry)
		{
			return false;
		}

		AddCityToDb(recCity);
	}

	return true;
}

bool CCitiesDocument::DeleteCity(const long lId)
{
	const BOOL bResult = m_oCitiesData.DeleteCity(lId);

	if (!bResult)
	{
		PromptErrorOn(
			CITIES_CREATE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \
Ако смятате, че проблемът не е от вас, може да опитате отново."));

		return false;
	}

	return true;
}


bool CCitiesDocument::PromptErrorOn(const INT nError, const TCHAR* pszMessage)
{
	//TODO: may need to handle different errors diferently -> nError
	
	const int nReply = MessageBox(nullptr, pszMessage, _T("Информация"), MB_ICONINFORMATION | MB_RETRYCANCEL);

	switch (nReply)
	{
	case IDCANCEL:
		return false;
	case IDRETRY:
		return true;
	default:
		MessageBox(nullptr, _T("Не бяха извършени промени!"), _T("Информация"), MB_ICONWARNING);
		return false;
	}
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

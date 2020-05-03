#include "pch.h"
#include "CitiesDocument.h"

#define CITIES_CREATE_ERROR 0
#define CITIES_READ_ERROR 1
#define CITIES_UPDATE_ERROR 2
#define CIITES_DELETE_ERROR 3

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
		TRACE(_T("CDocument::OnNewDocument() - Failed"));
		return FALSE;
	}

	UpdateCitiesInDocument();
	
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

// Methods
// ----------------

CCitiesArray* CCitiesDocument::GetAllCities()
{
	return &m_oCitiesArray;
}

CITIES* CCitiesDocument::GetCityById(long lId)
{
	CITIES* pCity;
	long nIndexOfCity;

	// Търси си се дали записът съществува във вече изтелглените записи
	BOOL bIsSuccessful = m_oCitiesIndexesOfIds.Lookup(lId, nIndexOfCity);
	if (bIsSuccessful)
	{
		pCity = m_oCitiesArray.GetAt(nIndexOfCity);
		return pCity;
	}

	TRACE(_T("Record does not exist in document. Proceeding database search."));
	
	CITIES recCity;
	// Записът се търси в бд и се записва в recCity, ако не е намерен, функцията се връща с nullptr
	bIsSuccessful = m_oCitiesData.GetCityById(lId, recCity);
	if (!bIsSuccessful)
	{
		TRACE("Error: could not get city from data in document.");
		return nullptr;
	}

	// Документът се ъпдейтва, ако записът съществува и известява всички изгледи
	OnUpdateAllViews();

	// Записът се извлича
	bIsSuccessful = m_oCitiesIndexesOfIds.Lookup(lId, nIndexOfCity);
	if (!bIsSuccessful)
	{
		TRACE(_T("Record does not exist in document."));
	}
	
	pCity = m_oCitiesArray.GetAt(nIndexOfCity);
	
	return pCity;
}

bool CCitiesDocument::UpdateCitiesInDocument()
{
	DoEmptyRepository();
	
	const BOOL bResult = m_oCitiesData.GetAllCities(m_oCitiesArray);
	if (!bResult)
	{
		TRACE(_T("Error getting all cities in document"));
		//TODO: msg
		return FALSE;
	}

	return TRUE;
}

bool CCitiesDocument::EditCity(CITIES& recCity, CView* pView)
{
	const BOOL bResult = m_oCitiesData.UpdateCityWithId(recCity.lID, recCity);

	if (!bResult)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_UPDATE_ERROR,
			_T("Възникна грешка. \n\
Възможна причина: записът вече е обновен преди настъпване на настоящите промени. \n\
Моля обновете вашите данни и опитайте да направите редакциите отново. \n\
Ако сте сигурни, че това не е проблемът, може да опитате отново."));
		
		if (bShouldRetry)
		{
			return EditCity(recCity, pView);
		}

		return false;
	}

	OnUpdateAllViews(pView);
	return true;
}

bool CCitiesDocument::AddCityToDb(CITIES& recCity, CView* pView)
{
	const BOOL bResult = m_oCitiesData.InsertCity(recCity);

	if (!bResult)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_CREATE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас, може да опитате отново."));
		
		if (bShouldRetry)
		{
			return AddCityToDb(recCity, pView);
		}

		return false;
	}

	OnUpdateAllViews(pView);
	return true;
}

bool CCitiesDocument::DeleteCity(const long lId, CView* pView)
{
	const BOOL bResult = m_oCitiesData.DeleteCity(lId);

	if (!bResult)
	{
		PromptErrorOn(
			CITIES_CREATE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас, може да опитате отново."));

		return false;
	}

	OnUpdateAllViews(pView);
	return true;
}

void CCitiesDocument::DoEmptyRepository()
{
	m_oCitiesArray.DeleteAll();
	m_oCitiesIndexesOfIds.RemoveAll();
}

void CCitiesDocument::OnUpdateAllViews(CView* pView)
{
	UpdateCitiesInDocument();
	SetModifiedFlag();
	UpdateAllViews(pView);
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

void CCitiesDocument::RemoveCityFromRepository(const long lId)
{
	long nIndex;
	m_oCitiesIndexesOfIds.Lookup(lId, nIndex);
	m_oCitiesArray.RemoveAt(nIndex);
	m_oCitiesIndexesOfIds.RemoveKey(lId);
}

void CCitiesDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCitiesDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

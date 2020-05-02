#pragma once

#include "CitiesData.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

/// <summary>Клас документ на CITIES</summary>
class CCitiesDocument :	public CDocument
{

// Constructor / Destructor
// ----------------
public:
	CCitiesDocument();
	~CCitiesDocument();

DECLARE_DYNCREATE(CCitiesDocument)
	
// Methods
// ----------------

public:
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
	CCitiesArray* GetAllCities();
	CITIES* GetCityById(long lId);
	bool EditCity(CITIES& recCity);
	bool AddCityToDb(CITIES& recCity);
	bool DeleteCity(const long lId);
	
private:
	INT_PTR AddCityToRepository(CITIES& recCity);
	bool PromptErrorOn(const INT nError, const TCHAR* pszMessage);
	
// Members
// ----------------
private:
	CCitiesData m_oCitiesData;
	CCitiesArray m_oCitiesArray;
	CMap<long, long, long, long> m_oCitiesIndexesOfIds;
	
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif
};


#pragma once
#include "CitiesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesData

/// <summary> Клас за бизнес логика на CITIES </summary>
class CCitiesData
{
	
// Constructor / Destructor
// ----------------
public:
	CCitiesData();
	~CCitiesData();

// Methods
// ----------------
public:
	BOOL GetAllCities(CCitiesArray& oCitiesArray);
	BOOL GetCityById(long lId, CITIES& recCity);
	
// Members
// ----------------
private:
	CCitiesTable m_oCitiesTable;
	
};


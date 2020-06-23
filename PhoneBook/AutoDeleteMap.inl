#pragma once
#include "AutoDeleteMap.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteMap

// Constructor / Destructor
// ----------------

template<typename KEY, typename VALUE>
CAutoDeleteMap<KEY, VALUE>::CAutoDeleteMap() {}
template<typename KEY, typename VALUE>
CAutoDeleteMap<KEY, VALUE>::~CAutoDeleteMap()
{
	this->DeleteAll();
}

// Methods
// ----------------

 template<typename KEY, typename VALUE>
 void CAutoDeleteMap<KEY, VALUE>::DeleteAll()
{
	typename CAutoDeleteMap<KEY, VALUE>::CPair* pCurrentCity = this->PGetFirstAssoc();
	for (INT_PTR i = 0; i < this->GetCount(); i++)
	{
		delete pCurrentCity->value;
		pCurrentCity = this->PGetNextAssoc(pCurrentCity);
	}

	this->RemoveAll();
}
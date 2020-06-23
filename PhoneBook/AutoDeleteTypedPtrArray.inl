#pragma once
#include "AutoDeleteTypedPtrArray.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteTypedPtrArray

template<typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::CAutoDeleteTypedPtrArray() {}

template<typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::CAutoDeleteTypedPtrArray(CAutoDeleteTypedPtrArray& oAutoDeleteTypedPtrArray)
{
	for (INT_PTR i = 0; i < oAutoDeleteTypedPtrArray.GetCount(); i++)
	{
		this->Add(oAutoDeleteTypedPtrArray.GetAt(i));
	}
}
template<typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::~CAutoDeleteTypedPtrArray()
{
	DeleteAll();
}

// Methods
// ----------------

template<typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::DeleteAll()
{
	for (INT_PTR i = 0; i < this->GetCount(); i++)
	{
		delete this->GetAt(i);

	}
	this->RemoveAll();
}
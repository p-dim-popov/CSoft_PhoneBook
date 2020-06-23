#pragma once
/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteTypedPtrArray

/// <summary>Клас, наследяващ CTypedPtrArray и имплементиращ Garbage Collector чрез метод DeleteAll</summary>
template<typename TYPE>
class CAutoDeleteTypedPtrArray : public CTypedPtrArray<CPtrArray, TYPE>
{

	// Constructor / Destructor
	// ----------------
public:
	CAutoDeleteTypedPtrArray();
	CAutoDeleteTypedPtrArray(CAutoDeleteTypedPtrArray& oAutoDeleteTypedPtrArray);
	~CAutoDeleteTypedPtrArray();

	// Methods
	// ----------------
public:
	/// <summary>Освобождава динамичната памет и премахва всички елементи от масива</summary>
	/// <returns>void</returns>
	void DeleteAll();
};

#include "AutoDeleteTypedPtrArray.inl"
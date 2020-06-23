#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteMap

/// <summary>Клас, наследяващ CMap и имплементиращ Garbage Collector чрез метод DeleteAll</summary>
template<typename KEY, typename VALUE>
class CAutoDeleteMap : public CMap<KEY, KEY, VALUE, VALUE>
{

	// Constructor / Destructor
	// ----------------
public:
	CAutoDeleteMap();
	~CAutoDeleteMap();

	// Methods
	// ----------------
public:
	/// <summary>Освобождава динамичната памет и премахва всички елементи от масива</summary>
	/// <returns>void</returns>
	void DeleteAll();
};

#include "AutoDeleteMap.inl"
#pragma once
#include "Structures.h"

////////////////////////////////////////////////////////////////////////////
/// IComapre interface for the usage of CAutoDeleteTypedPtrArray
template<typename TYPE>
struct ICompare
{
	virtual BOOL Compare(TYPE&) const = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteTypedPtrArray

/// <summary>Клас, наследяващ CTypedPtrArray и имплементиращ Garbage Collector чрез метод DeleteAll</summary>
template<typename TYPE>
class CAutoDeleteTypedPtrArray : public CTypedPtrArray<CPtrArray, ICompare<TYPE>*>
{

	// Constructor / Destructor
	// ----------------
public:
	CAutoDeleteTypedPtrArray(BOOL bEnableAutoDelete = TRUE);
	CAutoDeleteTypedPtrArray(const CAutoDeleteTypedPtrArray<TYPE>& oArray);
	virtual ~CAutoDeleteTypedPtrArray();

	// Methods
	// ----------------
public:
	/// <summary>Освобождава динамичната памет и премахва всички елементи от масива</summary>
	/// <returns>void</returns>
	void DeleteAll();

	/// <summary> Mutable sorting function </summary>
	/// <param name="funcComparer">Sorting criteria</param>
	void Sort(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond));

	/// <summary> Immutable sorting function </summary>
	/// <param name="funcComparer">Sorting criteria</param>
	CAutoDeleteTypedPtrArray<TYPE> OrderedBy(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond));

	/// <summary> Delegate to CTypedPtrArray </summary>
	TYPE* GetAt(INT_PTR nIndex) const;

	/// <summary> Delegate to CTypedPtrArray </summary>
	void SetAt(INT_PTR nIndex, TYPE* pItem);

	/// <summary> Delegate to CTypedPtrArray </summary>
	INT_PTR Add(TYPE* pItem);

	void DeepCopy(const CAutoDeleteTypedPtrArray<TYPE>& oArray);
	
	/// <summary> Custom implementation of C#'s LINQ FirstOrDefault</summary>
	/// <param name="funcPredicate"> Predicate on which the array will be filtered </param>
	/// <param name="pCapture"> Capture group similar to lambda's [], just to bypass using <functional> library</param>
	/// <param name="funcComparer"> Sorting criteria. If passed array will be sorted and then performed binary search </param>
	/// <returns> The first found occurence item or NULL if nothing found </returns>
	TYPE* FirstOrDefault(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture = NULL, BOOL bIsSorted = FALSE);

	TYPE& First(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture = NULL, BOOL bIsSorted = FALSE);

	INT_PTR IndexOf(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture = NULL, BOOL bIsSorted = FALSE);
	INT_PTR IndexOf(TYPE& oItem);

	/// <summary> Swap by index </summary>
	/// <param name="nFirstIndex">INT_PTR</param>
	/// <param name="nSecondIndex">INT_PTR</param>
	void Swap(INT_PTR nFirstIndex, INT_PTR nSecondIndex, CAutoDeleteTypedPtrArray<TYPE>& oArray);

	void EnableAutoDelete(BOOL bEnable = TRUE);

	CAutoDeleteTypedPtrArray<TYPE> Where(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture = NULL);
private:
	/// <summary> Simplest sorting algorithm </summary>
	/// <param name="funcComparer">Sorting criteria</param>
	void BubbleSort(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond), CAutoDeleteTypedPtrArray<TYPE>& oArray);

	/// <summary>Binary search algorithm</summary>
	/// <param name="nBeg">Beginning array index to search</param>
	/// <param name="nEnd">Ending array index to search</param>
	/// <param name="funcPredicate">Predicate on which the array will be filtered</param>
	/// <param name="pCapture">Capture group similar to lambda's [], just to bypass using <functional> library</param>
	/// <returns>Index of searched element</returns>
	INT BinarySearch(int nBeg, int nEnd, BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture = NULL);

	// Members
	// -------------
private:
	BOOL m_bAutoDeleteEnabled;
};

#include "AutoDeleteTypedPtrArray.inl"

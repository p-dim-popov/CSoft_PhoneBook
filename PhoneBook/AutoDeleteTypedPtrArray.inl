#pragma once
#include "AutoDeleteTypedPtrArray.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoDeleteTypedPtrArray

template <typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::CAutoDeleteTypedPtrArray(BOOL bEnableAutoDelete) :
	m_bAutoDeleteEnabled(bEnableAutoDelete)
{
}


template <typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::CAutoDeleteTypedPtrArray(const CAutoDeleteTypedPtrArray<TYPE>& oArray) :
	m_bAutoDeleteEnabled(oArray.m_bAutoDeleteEnabled)
{
	for (INT_PTR i = 0; i < oArray.GetCount(); i++)
	{
		this->Add(oArray.GetAt(i));
	}
}

template<typename TYPE>
CAutoDeleteTypedPtrArray<TYPE>::~CAutoDeleteTypedPtrArray()
{
	if (m_bAutoDeleteEnabled)
	{
		DeleteAll();
	}
}

// Methods
// ----------------

template<typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::DeleteAll()
{
	for (INT_PTR i = 0; i < this->GetCount(); i++)
	{
		delete static_cast<TYPE*>(this->GetAt(i));
	}
	this->RemoveAll();
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::Sort(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond))
{
	BubbleSort(funcComparer, *this);
}

template <typename TYPE>
CAutoDeleteTypedPtrArray<TYPE> CAutoDeleteTypedPtrArray<TYPE>::OrderedBy(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond))
{
	CAutoDeleteTypedPtrArray<TYPE> oOrderedArray(*this);
	oOrderedArray.EnableAutoDelete(FALSE);
	BubbleSort(funcComparer, oOrderedArray);
	return oOrderedArray;
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::Swap(INT_PTR nFirstIndex, INT_PTR nSecondIndex, CAutoDeleteTypedPtrArray<TYPE>& oArray)
{
	TYPE* oFirst = oArray.GetAt(nFirstIndex);
	TYPE* oSecond = oArray.GetAt(nSecondIndex);

	this->SetAt(nFirstIndex, oSecond);
	this->SetAt(nSecondIndex, oFirst);
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::EnableAutoDelete(BOOL bEnable)
{
	m_bAutoDeleteEnabled = bEnable;
}

template <typename TYPE>
CAutoDeleteTypedPtrArray<TYPE> CAutoDeleteTypedPtrArray<TYPE>::Where(
	BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture)
{
	CAutoDeleteTypedPtrArray<TYPE> oFilteredArray;
	oFilteredArray.EnableAutoDelete(FALSE);

	for (INT_PTR i = 0; i < this->GetSize(); i++)
	{
		TYPE* pCurrent = this->GetAt(i);
		if (funcPredicate(*pCurrent, pCapture))
		{
			oFilteredArray.Add(pCurrent);
		}
	}

	return oFilteredArray;
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::BubbleSort(BOOL(*funcComparer)(TYPE& oFirst, TYPE& oSecond), CAutoDeleteTypedPtrArray<TYPE>& oArray)
{
	for (int i = 0; i < oArray.GetSize() - 1; i++)
	{
		bool bSwapped = false;
		for (int j = 0; j < oArray.GetSize() - i - 1; j++)
		{
			if (funcComparer(*oArray.GetAt(j), *oArray.GetAt(j + 1)))
			{
				Swap(j, j + 1, oArray);
				bSwapped = true;
			}
		}

		// if the two elements were not swapped by inner loop, then break 
		if (!bSwapped)
			break;
	}
}

template <typename TYPE>
INT CAutoDeleteTypedPtrArray<TYPE>::BinarySearch(int nBeg, int nEnd,
	BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture)
{
	int nMid = -1;
	if (nEnd >= nBeg) {
		nMid = (nBeg + nEnd) / 2;
		TYPE* pKey = this->GetAt(nMid);

		if (!funcPredicate(*pKey, pCapture))
		{
			return nMid + 1;
		}

		if (funcPredicate(*pKey, pCapture) < 0) {
			return BinarySearch(nMid + 1, nEnd, funcPredicate);
		}

		return BinarySearch(nBeg, nMid - 1, funcPredicate);
	}
	return nMid;
}

template <typename TYPE>
TYPE* CAutoDeleteTypedPtrArray<TYPE>::FirstOrDefault(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture, BOOL bIsSorted)
{
	TYPE* pResult = NULL;

	if (bIsSorted)
	{
		const INT nIndex = BinarySearch(0, this->GetSize(), funcPredicate, pCapture);
		if (nIndex > -1)
			pResult = reinterpret_cast<TYPE*>(this->GetAt(nIndex));
		return pResult;
	}

	for (INT_PTR i = 0; i < this->GetSize(); i++)
	{
		TYPE* pCurrent = this->GetAt(i);
		if (funcPredicate(*pCurrent, pCapture))
		{
			pResult = reinterpret_cast<TYPE*>(pCurrent);
			break;
		}
	}

	return pResult;
}

template <typename TYPE>
TYPE& CAutoDeleteTypedPtrArray<TYPE>::First(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture, BOOL bIsSorted)
{
	TYPE* pResult = this->FirstOrDefault(funcPredicate, pCapture, bIsSorted);
	ASSERT(pResult);
	return *pResult;
}

template <typename TYPE>
INT_PTR CAutoDeleteTypedPtrArray<TYPE>::IndexOf(BOOL(*funcPredicate)(TYPE& oFirst, void* pCapture), void* pCapture,
	BOOL bIsSorted)
{
	INT_PTR nResult = -1;

	if (bIsSorted)
	{
		const INT nIndex = BinarySearch(0, this->GetSize(), funcPredicate, pCapture);
		if (nIndex > -1)
			nResult = nIndex;
		return nResult;
	}

	for (INT_PTR i = 0; i < this->GetSize(); i++)
	{
		TYPE* pCurrent = this->GetAt(i);
		if (funcPredicate(*pCurrent, pCapture))
		{
			nResult = i;
			break;
		}
	}

	return nResult;
}

template <typename TYPE>
INT_PTR CAutoDeleteTypedPtrArray<TYPE>::IndexOf(TYPE& oItem)
{
	INT_PTR nResult = 0;

	for (INT_PTR i = 0; i < this->GetSize(); i++)
	{
		TYPE* pCurrent = this->GetAt(i);
		if (!((static_cast<ICompare<TYPE>*>(pCurrent))->Compare(oItem)))
		{
			nResult = i;
			break;
		}
	}

	return nResult;
}

template <typename TYPE>
TYPE* CAutoDeleteTypedPtrArray<TYPE>::GetAt(INT_PTR nIndex) const
{
	return reinterpret_cast<TYPE*>(CTypedPtrArray<CPtrArray, ICompare<TYPE>*>::GetAt(nIndex));
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::SetAt(INT_PTR nIndex, TYPE* pItem)
{
	CTypedPtrArray<CPtrArray, ICompare<TYPE>*>::SetAt(nIndex, reinterpret_cast<ICompare<TYPE>*>(pItem));
}

template <typename TYPE>
INT_PTR CAutoDeleteTypedPtrArray<TYPE>::Add(TYPE* pItem)
{
	return CTypedPtrArray<CPtrArray, ICompare<TYPE>*>::Add(reinterpret_cast<ICompare<TYPE>*>(pItem));
}

template <typename TYPE>
void CAutoDeleteTypedPtrArray<TYPE>::DeepCopy(const CAutoDeleteTypedPtrArray<TYPE>& oArray)
{
	this->SetSize(oArray.GetSize());

	for (INT_PTR i = 0; i < oArray.GetSize(); i++)
	{
		this->SetAt(i, new TYPE(*oArray.GetAt(i)));
	}
}

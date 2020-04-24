#pragma once
#include "Structures.h"

#define MAX_NAMES_CHARACTERS_COUNT 128
#define EXAMPLE_CITIES_COUNT 5

void TestCString();
void TestCArray();
void TestCPtrArray();
void TestCTypedPtrArray();
void TestCMap();
void TestCTypedPtrArrayWithMeaningfulExamples();
void FillCityRecord(const long lCityID, const TCHAR* pszCityName, const TCHAR* pszCityRegion, CITIES& recCity);
BOOL IsBelowOrEqualToZero(const long lNumber);

void TestCString()
{
	typedef void (*CallbackRemoveLastCommaIfAny)(CString&);
	/// <summary>
	/// Функция за премахване на последваща запетайка ако съществува такава
	/// </summary>
	/// <param name="strString"> Низ върху който ще се извършши проверката и промяната </param> 
	/// <returns> Функцията връща void </returns>
	CallbackRemoveLastCommaIfAny pCallbackRemoveLastCommaIfAny = [](CString& strString)
	{
		if (strString.Right(1) == ",")
		{
			strString.Delete(strString.ReverseFind(','));
		}
	};

	CString strNamesSeparatedByComma = _T("Ivan,Stoqn,Petar");

	// добавяме невалидно име ако има място
	if (strNamesSeparatedByComma.GetLength() < MAX_NAMES_CHARACTERS_COUNT)
	{
		strNamesSeparatedByComma.AppendChar(',');
		strNamesSeparatedByComma += "		";
	}

	CString strNamesOldValue = strNamesSeparatedByComma;
	// ако има празно място отляво или отдясно на низа, да го премахне и да провери дали има остатъчен разделител от операцията
	BOOL bStringsAreDifferent = strNamesOldValue.Compare(strNamesSeparatedByComma.Trim()) != 0;
	if (bStringsAreDifferent)
	{
		pCallbackRemoveLastCommaIfAny(strNamesSeparatedByComma);
	}
	
	// променяме формата на записите от Comma Separated Values на Tab Separated Values
	// проверяваме дали strNamesSeparatedByComma реално е в нужния формат като заменяме всички ','
	// с '\t' и ако е успешно може да запишем резултата към резервираната променлива strNamesSeparatebByTab
	// защото Replace e функция, променяща оригиналната колекция, в случая CString - колекция от символи,
	// се налага да възстановим съдържанието на strNamesSeparatedByComma така, че да отговаря на името си -
	// стойности разделени чрез ',', като заменим всички '\t' с ','
	CString strNamesSeparatebByTab;
	if (strNamesSeparatedByComma.Replace(_T(","), _T("\t")))
	{
		strNamesSeparatebByTab = strNamesSeparatedByComma;
		strNamesSeparatedByComma.Replace(_T("\t"), _T(","));
	}
};

void TestCArray()
{
	CArray<CITIES> oCitiesArray;
	CITIES recCity;
	CITIES* pCity = new CITIES();
	
	recCity.lID = 1;
	pCity->lID = 2;

	const short sIndexOfFirstElement = oCitiesArray.Add(CITIES());
	CITIES& oFirstCityFromArrayByReference = oCitiesArray.GetAt(sIndexOfFirstElement);
	oFirstCityFromArrayByReference.lID = -1;

	oCitiesArray.Add(recCity);
	// oFirstCityFromArrayByReference вече не сочи към първия обект, защото обектите са на друго място в паметта, понеже масивът е увеличил размера си
	oCitiesArray.Add(*pCity);

	recCity.lID = 3;
	oCitiesArray.InsertAt(1, recCity);
	pCity->lID = 4;
	oCitiesArray.InsertAt(2,* pCity);

	// намиране на град с ID = 1 и замяна с ID = -1
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES oCityFromCCitiesArray = oCitiesArray.GetAt(i);
		if (i == 1)
		{
			oCitiesArray.GetAt(i).lID = -1;
		}
	}

	oCitiesArray.RemoveAll();
	delete pCity; // обект на хийпа
};

void TestCPtrArray()
{
	CPtrArray oPtrArray;
	int nInt = 5;
	CITIES recCity;
	recCity.lID = 1;
	CITIES* pCity = new CITIES();
	pCity->lID = 2;

	// не се препоръчва слагането на различни типове данни в CPtrArray, както и стекови и хийп обекти едновременно
	oPtrArray.Add(&nInt);
	oPtrArray.Add(&recCity);
	oPtrArray.Add(pCity);
	oPtrArray.Add(new CString("Test text"));

	CITIES* pRecordCityFromCPtrArray = (CITIES* )oPtrArray.GetAt(1);
	// delete pRecordCityFromCPtrArray; // стеков обект => грешка
	CITIES* pObjectCityFromCPtrArray = (CITIES* )oPtrArray.GetAt(2);
	// delete pObjectCityFromCPtrArray; // обект в хийпа => работи
	CString* pStringFromCPtrArray = (CString*)oPtrArray.GetAt(3);
	// delete pStringFromCPtrArray; // обект в хийпа => работи

	for (INT_PTR i = 0; i < oPtrArray.GetCount(); i++)
	{
		void* pCurrent = oPtrArray.GetAt(i);
		// delete pCurrent; // няма информация дали обектът е стеков и ако е => грешка
	}

	oPtrArray.RemoveAt(0, 2); // целта е да останат само heap обекти

	for (INT_PTR i = 0; i < oPtrArray.GetCount(); i++)
	{
		CITIES* pCurrent = (CITIES* )oPtrArray.GetAt(i);
		delete pCurrent; // премахва и CString обектът, но предполагам, че грешно
	}

	oPtrArray.RemoveAll(); // резултатът дотук е фрагментирана и заета неизползваема памет
};

void TestCTypedPtrArray()
{
	CTypedPtrArray<CPtrArray, CITIES*> oCitiesArray;
	int nInt = 5;
	CITIES recCity;
	recCity.lID = 1;
	CITIES* pCity = new CITIES();
	pCity->lID = 2;

	// CCitiesArray.Add(&nInt); // грешка при компилация -> невалиден аргумент
	oCitiesArray.Add(&recCity); // лоша практика е да се добавят stack обекти, добавям го само заради примера
	oCitiesArray.Add(pCity);

	CITIES* pRecordCityFromCPtrArray = (CITIES*)oCitiesArray.GetAt(0);
	// delete pRecordCityFromCPtrArray; // стеков обект => грешка
	CITIES* pObjectCityFromCPtrArray = (CITIES*)oCitiesArray.GetAt(1);
	// delete pObjectCityFromCPtrArray; // обект в хийпа => работи

	// освобождаване на паметта от oCitiesArray
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		void* pCurrentCity = oCitiesArray.GetAt(i);
		// delete pCurrentCity; // няма информация дали обектът е стеков и ако е => грешка
	}

	oCitiesArray.RemoveAt(0); // остават само heap обекти

	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCurrentCity = (CITIES*)oCitiesArray.GetAt(i);
		delete pCurrentCity;
	}

	oCitiesArray.RemoveAll();
};

void TestCTypedPtrArrayWithMeaningfulExamples()
{
	// логиката, която ще ползвам е, че областните градове имат ID, което започва от 9000 нагоре
	// и ID-тата трябва да са положителни

	short sLastRegionCityID = 9000;
	CCitiesArray oCitiesArray;

	// добавяне на празни структури на град в масива oCitiesArray
	for (INT_PTR i = 0; i < EXAMPLE_CITIES_COUNT; i++)
	{
		oCitiesArray.Add(new CITIES());
	}

	// попълване на елементите от масива oCitiesArray с примерни данни
	FillCityRecord(1001, _T("SOZOPOL"), _T("BURGAS"), * oCitiesArray.GetAt(0)); 
	FillCityRecord(1002, _T("VARNA"), _T("VARNA"), * oCitiesArray.GetAt(1)); // грешно въведено ID
	FillCityRecord(-1023, _T("AKSAKOVO"), _T("VARNA"), * oCitiesArray.GetAt(2)); // невалидно ID
	FillCityRecord(4321, _T("BURGAS"), _T("BURGAS"), * oCitiesArray.GetAt(3)); // грешно въведено ID
	FillCityRecord(1234, _T("KICHEVO"), _T("VARNA"), * oCitiesArray.GetAt(4));

	// премахване на всички градове с невалидни ID и коригиране на грешните ID-та на регионалните градове
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCurrentCity = oCitiesArray.ElementAt(i);
		/// премахване на град с невалидно ID
		if (IsBelowOrEqualToZero(pCurrentCity->lID))
		{
			delete pCurrentCity;
			oCitiesArray.RemoveAt(i);
			i--;
			continue;
		}

		const BOOL bIsRegionCity = _tcscmp(pCurrentCity->szName, pCurrentCity->szRegion) == 0;
		/// коригиране на ID на регионален град
		if (bIsRegionCity)
		{
			pCurrentCity->lID = sLastRegionCityID++;
		}
	}

	// Сортиране по възходящ ред според ID на град
	for (INT_PTR i = oCitiesArray.GetCount() - 1; i > 0; i--)
	{
		BOOL bHasNotSwapped = true;

		for (INT_PTR j = 0; j < i; j++)
		{
			CITIES* pCurrentCity = oCitiesArray.ElementAt(j);
			CITIES* pNextCity = oCitiesArray.ElementAt(j + 1);
			if (pCurrentCity->lID > pNextCity->lID)
			{
				oCitiesArray.SetAt(j, pNextCity);
				oCitiesArray.SetAt(j + 1, pCurrentCity);

				bHasNotSwapped = false;
			}
		}
		
		if (bHasNotSwapped)
		{
			break;
		}
	}

	// Освобождаване на паметта от данните в oCitiesArray
	// UPDATE: CCitiesArray вече е клас с Auto Delete
	//for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	//{
	//	CITIES* pCityToDelete = oCitiesArray[i];
	//	delete pCityToDelete;
	//}
}

void TestCMap()
{	
	short sLastRegionCityID = 9000;
	CMap<long , long , CITIES, CITIES> oCitiesMappedByID;
	CCitiesArray oCitiesArray;

	// добавяне на празни структури на град в масива oCitiesArray
	for (INT_PTR i = 0; i < EXAMPLE_CITIES_COUNT; i++)
	{
		oCitiesArray.Add(new CITIES());
	}

	// попълване на елементите от масива oCitiesArray с примерни данни
	FillCityRecord(1001, _T("SOZOPOL"), _T("BURGAS"), * oCitiesArray.GetAt(0));
	FillCityRecord(1002, _T("VARNA"), _T("VARNA"), * oCitiesArray.GetAt(1)); // грешно въведено ID
	FillCityRecord(1023, _T("AKSAKOVO"), _T("VARNA"), * oCitiesArray.GetAt(2));
	FillCityRecord(4321, _T("BURGAS"), _T("BURGAS"), * oCitiesArray.GetAt(3)); // грешно въведено ID
	FillCityRecord(-1234, _T("KICHEVO"), _T("VARNA"), * oCitiesArray.GetAt(4)); // невалидно ID

	// копиране на елементите от масива oCitiesArray към oCitiesMappedByID
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCurrentCityToCopy = oCitiesArray.GetAt(i);
		CITIES recCityCopied;
		FillCityRecord(pCurrentCityToCopy->lID, pCurrentCityToCopy->szName, pCurrentCityToCopy->szRegion, recCityCopied);
		oCitiesMappedByID.SetAt(recCityCopied.lID, recCityCopied);
	}

	CMap<long, long, CITIES, CITIES>::CPair* pCurrentCity = oCitiesMappedByID.PGetFirstAssoc();
	// премахване на всички градове с невалидни ID и коригиране на грешните ID-та на регионалните градове
	for (INT_PTR i = 0; i < oCitiesMappedByID.GetCount(); i++)
	{
		CMap<long, long, CITIES, CITIES>::CPair* pNextCity = oCitiesMappedByID.PGetNextAssoc(pCurrentCity);

		/// премахване на град с невалидно ID
		if (IsBelowOrEqualToZero(pCurrentCity->value.lID))
		{
			oCitiesMappedByID.RemoveKey(pCurrentCity->key);
			pCurrentCity = pNextCity;
			continue;
		}

		const BOOL bIsRegionCity = _tcscmp(pCurrentCity->value.szName, pCurrentCity->value.szRegion) == 0;
		/// коригиране на ID на регионален град
		if (bIsRegionCity)
		{
			pCurrentCity->value.lID = sLastRegionCityID++;
			oCitiesMappedByID.SetAt(pCurrentCity->value.lID, pCurrentCity->value);
			oCitiesMappedByID.RemoveKey(pCurrentCity->key);
		}

		pCurrentCity = pNextCity;
	}

	// Взимам записът с ID 9000 и го променям така, че да е запис за VARNA
	CITIES recCityVarna;
	const BOOL bCityExists = oCitiesMappedByID.Lookup(9000, recCityVarna);
	if (bCityExists)
	{
		FillCityRecord(9000, _T("VARNA"), _T("VARNA"), recCityVarna);
	}
	
	// Oсвобождаване на паметта от oCitiesArray
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCityToDelete = oCitiesArray.GetAt(i);
		delete pCityToDelete;
		oCitiesArray.RemoveAt(i);
	}

	pCurrentCity = oCitiesMappedByID.PGetFirstAssoc();
	for (INT_PTR i = 0; i < oCitiesMappedByID.GetCount(); i++)
	{
		pCurrentCity = oCitiesMappedByID.PGetNextAssoc(pCurrentCity);
	}
};

/// <summary>
/// попълва CITIES структура с подадените параметри
/// </summary>
/// <param name="lCityID">ID на град</param>
/// <param name="pszCityName">Име на град</param>
/// <param name="pszCityRegion">Регион на град</param>
/// <param name="recCity">CITIES структурата, която трябва да бъде попълнена</param>
/// <returns>void</returns>
void FillCityRecord(const long lCityID, const TCHAR* pszCityName, const TCHAR* pszCityRegion, CITIES& recCity)
{
	recCity.lID = lCityID;
	for (size_t i = 0; i < sizeof(pszCityName) / sizeof(TCHAR); i++)
	{
		recCity.szName[i] = pszCityName[i];
	}

	for (size_t i = 0; i < sizeof(pszCityRegion) / sizeof(TCHAR); i++)
	{
		recCity.szRegion[i] = pszCityRegion[i];
	}
}

/// <summary>
/// Проверка дали подаденият параметър е отрицателен или равен на нула
/// </summary>
/// <param name="lNumber">число - long</param>
/// <returns>BOOL: true ако числото е отрицателно или равно на нула, false ако е положително</returns>
BOOL IsBelowOrEqualToZero(const long lNumber)
{
	return lNumber <= 0;
}

// Стартира всички функции за запознаване с класовете
void TestAll()
{
	TestCString();
	TestCArray();
	TestCPtrArray();
	TestCTypedPtrArray();
	TestCMap(); 
	TestCTypedPtrArrayWithMeaningfulExamples();
};
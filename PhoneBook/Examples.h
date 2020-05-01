#pragma once

#include "CitiesTable.h"
#include "Task2.h"

#define PRINT_CITY_TEMPLATE "|| CityName: %s; CityRegion: %s; CityID:%d; CityUpdateCounter: %d\n"
#define DO_NOT_PRINT false
#define DO_NOT_RUN false

/// <summary>Примери със SelectAll</summary>
INT_PTR SelectAllExample(bool bShouldBePrinted = true)
{
	CCitiesArray oCitiesArray;
	CCitiesTable oCitiesTable;

	// Всички записи от БД се изтеглят и се записват в oCitiesArray, bIsDataGatherSuccessful съхранява резултатът от заявката
	const BOOL bIsDataGatherSuccessful = oCitiesTable.SelectAll(oCitiesArray);
	const INT_PTR nCitiesArrayCount = oCitiesArray.GetCount();
	
	// Ако изпълнението на SELECT-а е успешно, се принтират всички данни от таблица CITIES в посочения формат
	if (!bIsDataGatherSuccessful)
	{
		return NULL;
	}

	if (!bShouldBePrinted)
	{
		return nCitiesArrayCount;
	}
	
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = oCitiesArray.GetAt(i);
		TRACE(_T(PRINT_CITY_TEMPLATE), pCity->szName, pCity->szRegion, pCity->lID, pCity->lUpdateCounter);
	}

	TRACE("\n");

	return nCitiesArrayCount;
}

/// <summary>Пример със SelectWhereID</summary>
void SelectWhereIDExample()
{
	CCitiesTable oCitiesTable;
	CITIES recCity;

	// Изпраща се заявка за запис с ID = 1 и резултатът от заявката се запазва в bIsDataGatherSuccessful, а записът в recCity
	const BOOL bIsDataGatherSuccessful = oCitiesTable.SelectWhereID(1, recCity);
	// Ако изпълнението на SELECT-а е успешно, се принтира градът в посочения формат
	if (!bIsDataGatherSuccessful)
	{
		return;
	}
	
	TRACE(_T(PRINT_CITY_TEMPLATE), recCity.szName, recCity.szRegion, recCity.lID, recCity.lUpdateCounter);
	TRACE("\n");
}

/// <summary>Пример с UpdateWhereID</summary>
void UpdateWhereIDExample()
{
	CCitiesTable oCitiesTable;
	CITIES recCity;
	
	// Изпраща се заявка за запис с ID = 1 и резултатът от заявката се запазва в bIsDataGatherSuccessful, а записът в recCity
	const BOOL bIsDataGatherSuccessful = oCitiesTable.SelectWhereID(1, recCity);

	// Ако изпълнението на SELECT-а е успешно се преминава към проверка дали името е изписано с главни букви и ако е
	// се променя с малки букви и записа се ъпдейтва в базата данни
	// Ако името на градът е с малки букви, се променя с главни и записа се ъпдейтва в базата данни
	// След това се принтира резултатът
	
	if (!bIsDataGatherSuccessful)
	{
		return;
	}
	
	CString strCityName = recCity.szName;
	if(CString(recCity.szName).MakeUpper() == strCityName)
	{
		strCityName.MakeLower();
		_tcscpy_s(recCity.szName, strCityName.GetLength() + 1, strCityName);	
	}
	else
	{
		strCityName.MakeUpper();
		_tcscpy_s(recCity.szName, strCityName.GetLength() + 1, strCityName);
	}

	// Записът се обновява
	oCitiesTable.UpdateWhereID(recCity.lID, recCity);

	// Записър се обновява втори път без селектиране втори път
	recCity.lUpdateCounter++;
	oCitiesTable.UpdateWhereID(recCity.lID, recCity);
	
	// Записът се изтегля обратно, за да се изведе промяната
	oCitiesTable.SelectWhereID(recCity.lID, recCity);
	
	TRACE(_T(PRINT_CITY_TEMPLATE), recCity.szName, recCity.szRegion, recCity.lID, recCity.lUpdateCounter);
	TRACE("\n");
}

/// <summary>Пример с Insert</summary>
void InsertExample()
{
	CCitiesTable oCitiesTable;
	CITIES recCity;
	
	_tcscpy_s(recCity.szName, sizeof(recCity.szName)/sizeof(TCHAR), _T("DummyName"));
	_tcscpy_s(recCity.szRegion, sizeof(recCity.szRegion)/sizeof(TCHAR), _T("DummyRegion"));

	// Записът се изпраща към БД и резултатът се записва в bIsDataInsertSuccessful
	const BOOL bIsDataInsertSuccessful = oCitiesTable.Insert(recCity);
	
	// Ако изпълнението на SELECT-а е успешно, се принтират всички градове в посочения формат
	if (!bIsDataInsertSuccessful)
	{
		return;
	}
	
	SelectAllExample();
}

/// <summary>Пример с DeleteWhereID</summary>
void DeleteWhereIDExample()
{
	CCitiesArray oCitiesArray;
	CCitiesTable oCitiesTable;

	// Всички записи от БД се изтеглят и се записват в oCitiesArray, bIsDataGatherSuccessful съхранява резултатът от заявката
	const BOOL bIsDataGatherSuccessful = oCitiesTable.SelectAll(oCitiesArray);

	// Ако изпълнението на SELECT-а е успешно се търси Dummy записа и се изтрива
	if (!bIsDataGatherSuccessful)
	{
		return;
	}
	
	// Колекцията се обикаля с цел да се намери Dummy записа
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = oCitiesArray.GetAt(i);
		if (_tcscmp(pCity->szName,_T("DummyName")) == 0 && _tcscmp(pCity->szRegion, _T("DummyRegion")) == 0)
		{
			oCitiesTable.DeleteWhereID(pCity->lID);
			break;
		}
	}

	// Промените се извеждат
	SelectAllExample();
}

/// <summary>Пример с всички CRUD операции</summary>
void CRUDExample()
{
	typedef CMap<long, long, CITIES*, CITIES*> CCitiesMapByID;
	
	/// <summary>
	/// Помощен клас за извършване на операции свързани с логиката на тази функция като: разпределяне на ID-та по съответните масиви,
	/// премахване и добавяне на елементи към БД и добавяне на Dummy записи
	/// </summary>
	class CCRUDExampleHelper
	{
	public:
		/// <summary>
		/// ID-та от масива се разпределят по четност и нечетност в съответните масиви: oEvenIDsArray и oOddIDsArray
		/// </summary>
		/// <param name="oCitiesArray">Масив с записи CITIES</param>
		/// <param name="oEvenIDsArray">Масив с четни ID</param>
		/// <param name="oOddIDsArray">Масив с нечетни ID</param>
		static void ArrangeIDsToCorrespondingArray(const CCitiesArray& oCitiesArray, CArray<long>& oEvenIDsArray, CArray<long>& oOddIDsArray)
		{
			for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
			{
				CITIES* pCity = oCitiesArray.GetAt(i);
				if (pCity->lID % 2 == 0)
				{
					oEvenIDsArray.Add(pCity->lID);
				}
				else
				{
					oOddIDsArray.Add(pCity->lID);
				}
			}
		}

		/// <summary>
		/// Премахва всички записи по ID, като ID-тата се взимат от oIDsArray, и след това се връщат отново с цел да получат ново ID 
		/// Тези записи, които се използват от други таблици остават непроменени
		/// </summary>
		/// <param name="oCitiesMappedByID">Карта с записи CITIES и ключ ID на град</param>
		/// <param name="oIDsArray">Масив с ID-та</param>
		/// <param name="oCitiesTable">Обект за работа с таблицата CITIES</param>
		static void RemoveAllByIDAndReInsert(const CCitiesMapByID& oCitiesMappedByID, const CArray<long>& oIDsArray, CCitiesTable& oCitiesTable)
		{
			for (INT_PTR i = 0; i < oIDsArray.GetCount(); i++)
			{
				const long lCurrentID = oIDsArray.GetAt(i);
				CITIES* pCity;
				const BOOL bIsCityFound = oCitiesMappedByID.Lookup(lCurrentID, pCity);
				if (bIsCityFound == FALSE)
				{
					continue;
				}

				const BOOL bIsSuccessful = oCitiesTable.DeleteWhereID(lCurrentID);
				if (bIsSuccessful == FALSE)
				{
					continue;
				}

				oCitiesTable.Insert(*pCity);
			}
		};

		/// <summary>
		/// Добавя Dummy запис като името на града е DummyNameX, а региона DummyRegionX.
		/// Х е броят на записите в таблицата + 1
		/// </summary>
		/// <param name="oCitiesTable">Обект за работа с таблицата CITIES</param>
		/// <param name="nRecordsCount">Текущият брой записи</param>
		static void AddDummyRecordIntoCities(CCitiesTable& oCitiesTable, INT_PTR nRecordsCount)
		{
			nRecordsCount++;
			CITIES recCity;
			CString strCityName;
			strCityName.Format(_T("DummyName%d"), nRecordsCount);
			CString strCityRegion;
			strCityRegion.Format(_T("DummyName%d"), nRecordsCount);
			_tcscpy_s(recCity.szName, sizeof(recCity.szName) / sizeof(TCHAR), strCityName);
			_tcscpy_s(recCity.szRegion, sizeof(recCity.szRegion) / sizeof(TCHAR), strCityRegion);

			oCitiesTable.Insert(recCity);
		}
	};

	CArray<long> oEvenIDsArray;
	CArray<long> oOddIDsArray;
	CCitiesTable oCitiesTable;
	CCitiesArray oCitiesArray;
	CCitiesMapByID oCitiesMappedByID;

	// Ако "случайното" число е четно, се добавя Dummy запис
	srand(time(nullptr));
	if (rand() % 2 == 0)
	{
		const INT_PTR nCitiesTableRecordsCount = SelectAllExample(DO_NOT_PRINT);
		
		if (!nCitiesTableRecordsCount)
		{
			return;
		}
		
		CCRUDExampleHelper::AddDummyRecordIntoCities(oCitiesTable, nCitiesTableRecordsCount);
	}

	// Всички записи от БД се изтеглят и се записват в oCitiesArray, bIsDataGatherSuccessful съхранява резултатът от заявката
	const BOOL bIsDataGatherSuccessful = oCitiesTable.SelectAll(oCitiesArray);

	if (!bIsDataGatherSuccessful)
	{
		return;
	}
	
	// Всички ID-та от базата данни се разпределят по четност и нечетност в съответните масиви: oEvenIDsArray и oOddIDsArray
	CCRUDExampleHelper::ArrangeIDsToCorrespondingArray(oCitiesArray, oEvenIDsArray, oOddIDsArray);

	// Всички данни от масива oCitiesArray се вкарват в oCitiesMappedByID и oCitiesArray няма да ни трябва повече
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCurrentCity = oCitiesArray.GetAt(i);
		oCitiesMappedByID.SetAt(pCurrentCity->lID, pCurrentCity);
	}

	oCitiesArray.RemoveAll();
	
	// Ако четните ID-та преобладават, всички записи с четно ID се премахват и се вмъкват наново в базата данни
	// Ако нечетните преобладават, всички записи с нечетно ID се премахват и се вмъкват наново в базата данни
	// Ако са равни на брой, се добавя нов dummy запис с име и регион - DummyNameX и DummyRegionX, като X представлява текущият брой записи + 1
	if (oEvenIDsArray.GetCount() > oOddIDsArray.GetCount())
	{
		CCRUDExampleHelper::RemoveAllByIDAndReInsert(oCitiesMappedByID, oEvenIDsArray, oCitiesTable);
	}
	else if (oEvenIDsArray.GetCount() < oOddIDsArray.GetCount())
	{
		CCRUDExampleHelper::RemoveAllByIDAndReInsert(oCitiesMappedByID, oOddIDsArray, oCitiesTable);
	}
	else
	{
		CCRUDExampleHelper::AddDummyRecordIntoCities(oCitiesTable, oCitiesMappedByID.GetCount());
	}

	CCitiesMapByID::CPair* pCurrentCity = oCitiesMappedByID.PGetFirstAssoc();
	for (INT_PTR i = 0; i < oCitiesMappedByID.GetCount(); i++)
	{
		delete pCurrentCity->value;
		pCurrentCity = oCitiesMappedByID.PGetNextAssoc(pCurrentCity);
	}

	oCitiesMappedByID.RemoveAll();

	SelectAllExample();
}

/// <summary>Стартиране на всички примери</summary>
void StartExamples(bool bShouldItRun = true) //TODO: to run remove DO_NOT_RUN in usage
{
	if (!bShouldItRun)
	{
		return;
	}
	
	TestAll(); // Задачите от Task 2
	TRACE(_T("SelectAllExample\n"));
	SelectAllExample();
	TRACE(_T("SelectWhereIDExample\n"));
	SelectWhereIDExample();
	TRACE(_T("UpdateWhereIDExample\n"));
	UpdateWhereIDExample();
	TRACE(_T("InsertExample\n"));
	InsertExample(); // Въвежда Dummy запис в БД, който ще бъде търсен за премахване от DeleteWhereIDExample
	TRACE(_T("DeleteWhereIDExample\n"));
	DeleteWhereIDExample(); // Търси Dummy записа и след това го изтрива
	TRACE(_T("CRUDExample\n"));
	CRUDExample();
}

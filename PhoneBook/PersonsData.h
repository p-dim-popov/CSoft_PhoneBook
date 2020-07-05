#pragma once
#include "PersonsTable.h"
#include "PhoneNumbersTable.h"
#include "PhoneTypesTable.h"
#include "CitiesTable.h"
#include "Data.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsData

/// <summary> Клас за бизнес логика на Persons </summary>
class CPersonsData : public IData
{

	// Constructor / Destructor
	// ----------------
public:
	CPersonsData();
	~CPersonsData();

	// Methods
	// ----------------
public:
	/// <summary>Изтегля всички записи от бд и ги записва в подадения обект като параметър на функцията</summary>
	/// <param name="oPersonsArray">Масив, в който ще се добавят всички записи</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectAll(CPersonsArray& oPersonsArray);

	/// <summary>Изтегля запис по Id от бд и го записва в подадения обект като параметър на функцията</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recPerson">Структура за данни, в която ще се запише информацията</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectWhereID(long lID, PERSONS& recPerson);

	/// <summary>Обновява запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recPerson">Структура с обновената информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL UpdateWhereID(const long lID, PERSONS& recPerson);

	/// <summary>Добавя запис в бд</summary>
	/// <param name="recPerson">Структура с информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL InsertPersonWithData(
		PERSONS& recPerson,
		CPhoneNumbersArray& oPersonPhoneNumbersArray
	);

	/// <summary>Изтрива запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL DeleteWhereID(const long lID);

	BOOL SelectPhoneNumbersWherePersonID(long lID, CPhoneNumbersArray& oPhoneNumbersArray);
	BOOL SelectPhoneTypeWhereID(long lID, PHONE_TYPES& recPhoneType);
	BOOL SelectAllPhoneNumbers(CPhoneNumbersArray& oPhoneNumbersArray);
	BOOL SelectAllPhoneTypes(CPhoneTypesArray& oPhoneTypesArray);
	BOOL SelectAllCities(CCitiesArray& oCitiesArray);
	BOOL InsertPhoneType(PHONE_TYPES* pPhoneType);
	BOOL InsertCity(CITIES* pCity);
	BOOL DeletePhoneNumberWhereID(const long lID);
	BOOL UpdatePersonWithData(
		PERSONS& recPerson, 
		CPhoneNumbersArray& oDeletedPhoneNumbersArray, 
		CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
		CPhoneNumbersArray& oNewPhoneNumbersArray
	);

private:
	BOOL InsertPhoneTypes(CPhoneTypesArray& oPhoneTypesArray, CMap<long, long, long, long>& oNewPhoneTypesOldIndexesMap);

	// Members
	// ----------------
private:
	CPersonsTable m_oPersonsTable;
	CPhoneNumbersTable m_oPhoneNumbersTable;
	CPhoneTypesTable m_oPhoneTypesTable;
	CCitiesTable m_oCitiesTable;
};


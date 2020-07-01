#pragma once
#include "PhoneTypesTable.h"
#include "Data.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesData

/// <summary> Клас за бизнес логика на PHONE_TYPES </summary>
class CPhoneTypesData : public IData
{
	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesData();
	~CPhoneTypesData();

	// Methods
	// ----------------
public:
	/// <summary>Изтегля всички записи от бд и ги записва в подадения обект като параметър на функцията</summary>
	/// <param name="oPhoneTypesArray">Масив, в който ще се добавят всички записи</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectAll(CPhoneTypesArray& oPhoneTypesArray);

	/// <summary>Изтегля запис по Id от бд и го записва в подадения обект като параметър на функцията</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recPhoneType">Структура за данни, в която ще се запише информацията</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectWhereID(long lID, PHONE_TYPES& recPhoneType);

	/// <summary>Обновява запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recPhoneType">Структура с обновената информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL UpdateWhereID(const long lID, PHONE_TYPES& recPhoneType);

	/// <summary>Добавя запис в бд</summary>
	/// <param name="recPhoneType">Структура с информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL InsertPhoneType(PHONE_TYPES& recPhoneType);

	/// <summary>Изтрива запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL DeleteWhereID(const long lID);

	// Members
	// ----------------
private:
	/// <summary>Поле за достъп до бд</summary>
	CPhoneTypesTable m_oPhoneTypesTable;
};


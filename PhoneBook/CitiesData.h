#pragma once
#include "CitiesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesData

/// <summary> Клас за бизнес логика на CITIES </summary>
class CCitiesData
{

	// Constructor / Destructor
	// ----------------
public:
	CCitiesData();
	~CCitiesData();

	// Methods
	// ----------------
public:
	/// <summary>Изтегля всички записи от бд и ги записва в подадения обект като параметър на функцията</summary>
	/// <param name="oCitiesArray">Масив, в който ще се добавят всички записи</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectAll(CCitiesArray& oCitiesArray);

	/// <summary>Изтегля запис по Id от бд и го записва в подадения обект като параметър на функцията</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recCity">Структура за данни, в която ще се запише информацията</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL SelectWhereID(long lID, CITIES& recCity);

	/// <summary>Обновява запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <param name="recCity">Структура с обновената информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL UpdateWhereID(const long lID, CITIES& recCity);

	/// <summary>Добавя запис в бд</summary>
	/// <param name="recCity">Структура с информация</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL InsertCity(CITIES& recCity);

	/// <summary>Изтрива запис по Id от бд</summary>
	/// <param name="lID">Id на търсения запис</param>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL DeleteWhereID(const long lID);

	// Members
	// ----------------
private:
	/// <summary>Поле за достъп до бд</summary>
	CCitiesTable m_oCitiesTable;

};


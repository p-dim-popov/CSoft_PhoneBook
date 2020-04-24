#pragma once

#define CITIES_NAME_LENGTH 32
#define CITIES_REGION_LENGTH 32
#define PERSONS_FIRST_NAME_LENGTH 64
#define PERSONS_MIDDLE_NAME_LENGTH 64
#define PERSONS_LAST_NAME_LENGTH 64
#define PERSONS_UCN_LENGTH 32
#define PERSONS_ADDRESS_LENGTH 128
#define PHONE_NUMBERS_PHONE_LENGTH 32
#define PHONE_TYPES_TYPE_LENGTH 128

/// <summary>Структура репрезентираща град от БД - CITIES</summary>
struct CITIES
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Име на град</summary>
	TCHAR szName[CITIES_NAME_LENGTH + 1];
	/// <summary>Име на регион</summary>
	TCHAR szRegion[CITIES_REGION_LENGTH + 1];

	CITIES()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

/// <summary>Структура репрезентираща абонат от БД - PERSONS</summary>
struct PERSONS
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Собствено име на абонат</summary>
	TCHAR szFirstName[PERSONS_FIRST_NAME_LENGTH + 1];
	/// <summary>Презиме на абонат</summary>
	TCHAR szMiddleName[PERSONS_MIDDLE_NAME_LENGTH + 1];
	/// <summary>Фамилно име на абонат</summary>
	TCHAR szLastName[PERSONS_LAST_NAME_LENGTH + 1];
	/// <summary>ЕГН на абонат</summary>
	TCHAR szUCN[PERSONS_UCN_LENGTH + 1];
	/// <summary>Уникален идентификатор за град</summary>
	long lCityId;
	/// <summary>Адрес на абонат</summary>
	TCHAR szAddress[PERSONS_ADDRESS_LENGTH + 1];

	PERSONS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

/// <summary>Структура репрезентираща телефонен номер от БД - PHONE_NUMBERS</summary>
struct PHONE_NUMBERS
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Уникален идентификатор за абонат</summary>
	long lPersonId;
	/// <summary>Уникален идентификатор за тип на телефон</summary>
	long lPhoneTypeId;
	/// <summary>Телефонен номер на абонат</summary>
	TCHAR szPhone[PHONE_NUMBERS_PHONE_LENGTH + 1];

	PHONE_NUMBERS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

/// <summary>Структура репрезентираща тип на телефон от БД - PHONE_TYPES</summary>
struct PHONE_TYPES
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Тип на телефон</summary>
	TCHAR szType[PHONE_TYPES_TYPE_LENGTH + 1];

	PHONE_TYPES()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

/// <summary>Клас, наследяващ CTypedPtrArray и имплементиращ Garbage Collector чрез метод DeleteAll</summary>
template<typename T>
class CAutoDeleteTypedPtrArray : public CTypedPtrArray<CPtrArray, T>
{

// Constructor / Destructor
// ----------------
public:
	CAutoDeleteTypedPtrArray() {}
	~CAutoDeleteTypedPtrArray()
	{
		this->DeleteAll();
	}

// Methods
// ----------------
public:
	/// <summary>Освобождава динамичната памет и премахва всички елементи от масива</summary>
	/// <returns>void</returns>
	void DeleteAll()
	{
		for (INT_PTR i = 0; i < this->GetCount(); i++)
		{
			delete this->GetAt(i);
			
		}
		this->RemoveAll();
	}
};

typedef CAutoDeleteTypedPtrArray<CITIES*> CCitiesArray;
typedef CAutoDeleteTypedPtrArray<PERSONS*> CPersonsArray;
typedef CAutoDeleteTypedPtrArray<PHONE_NUMBERS*> CPhoneNumbersArray;
typedef CAutoDeleteTypedPtrArray<PHONE_TYPES*> CPhoneTypesArray;
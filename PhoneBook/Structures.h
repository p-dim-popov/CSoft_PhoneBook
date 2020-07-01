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

template<typename TYPE>
struct ICompare
{
	virtual BOOL Compare(TYPE&) const = 0;
};

/// <summary>Структура репрезентираща град от БД - CITIES</summary>
struct CITIES : ICompare<CITIES>
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Име на град</summary>
	TCHAR szName[CITIES_NAME_LENGTH + 1];
	/// <summary>Име на регион</summary>
	TCHAR szRegion[CITIES_REGION_LENGTH + 1];

	/// <summary> Setter за ID </summary>
	void SetID(const long lId) { this->lID = lId; }

	/// <summary> Setter за UpdateCounter </summary>
	void SetUpdateCounter(const long lUpdateCounter) { this->lUpdateCounter = lUpdateCounter; }

	/// <summary> Setter за Name </summary>
	void SetName(const TCHAR* pszName) { _tcscpy_s(this->szName, pszName); }

	/// <summary> Setter за Region </summary>
	void SetRegion(const TCHAR* pszRegion) { _tcscpy_s(this->szRegion, pszRegion); }

	/// <summary> Функция за инициализиране на всички параметри на град </summary>
	void InitData(const long lId, const long lUpdateCounter, const TCHAR* pszName, const TCHAR* pszRegion)
	{
		SetID(lId);
		SetUpdateCounter(lUpdateCounter);
		SetName(pszName);
		SetRegion(pszRegion);
	}

	/// <summary> Фунцкия за сравнение на 2 града </summary>
	/// <param name="recCity"> Град за сравнение </param>
	/// <returns> 0 ако двата града са идентични по всички параметри </returns>
	BOOL Compare(CITIES& recCity) const override
	{
		return _tcscmp(this->szName, recCity.szName) ||
			_tcscmp(this->szRegion, recCity.szRegion) ||
			(this->lID - recCity.lID) ||
			(this->lUpdateCounter - recCity.lUpdateCounter);
	}

	CITIES()
	{
		SecureZeroMemory(this, sizeof(*this));
	}

	~CITIES() {}
};

/// <summary>Структура репрезентираща абонат от БД - PERSONS</summary>
struct PERSONS : ICompare<PERSONS>
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

	/// <summary> Setter за FirstName </summary>
	void SetFirstName(const TCHAR* pszFirstName) { _tcscpy_s(this->szFirstName, pszFirstName); }

	/// <summary> Setter за MiddleName </summary>
	void SetMiddleName(const TCHAR* pszMiddleName) { _tcscpy_s(this->szMiddleName, pszMiddleName); }

	/// <summary> Setter за LastName </summary>
	void SetLastName(const TCHAR* pszLastName) { _tcscpy_s(this->szLastName, pszLastName); }

	/// <summary> Setter за UCN </summary>
	void SetUCN(const TCHAR* pszUCN) { _tcscpy_s(this->szUCN, pszUCN); }

	/// <summary> Setter за Address </summary>
	void SetAddress(const TCHAR* pszAddress) { _tcscpy_s(this->szAddress, pszAddress); }

	BOOL Compare(PERSONS& recPerson) const override
	{
		return _tcscmp(this->szFirstName, recPerson.szFirstName) ||
			_tcscmp(this->szMiddleName, recPerson.szMiddleName) ||
			_tcscmp(this->szLastName, recPerson.szLastName) ||
			_tcscmp(this->szAddress, recPerson.szAddress) ||
			_tcscmp(this->szUCN, recPerson.szUCN) ||
			(this->lID - recPerson.lID) ||
			(this->lUpdateCounter - recPerson.lUpdateCounter);
	}

	PERSONS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}

	~PERSONS() {}
};

/// <summary>Структура репрезентираща телефонен номер от БД - PHONE_NUMBERS</summary>
struct PHONE_NUMBERS : ICompare<PHONE_NUMBERS>
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

	/// <summary> Setter за Phone </summary>
	void SetPhone(const TCHAR* pszPhone) { _tcscpy_s(this->szPhone, pszPhone); }

	BOOL Compare(PHONE_NUMBERS& recPhoneNumber) const override
	{
		return _tcscmp(this->szPhone, recPhoneNumber.szPhone) ||
			(this->lID - recPhoneNumber.lID) ||
			(this->lPersonId - recPhoneNumber.lPersonId) ||
			(this->lPhoneTypeId - recPhoneNumber.lPhoneTypeId) ||
			(this->lUpdateCounter - recPhoneNumber.lUpdateCounter);
	}

	PHONE_NUMBERS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}

	~PHONE_NUMBERS() {}
};

/// <summary>Структура репрезентираща тип на телефон от БД - PHONE_TYPES</summary>
struct PHONE_TYPES : ICompare<PHONE_TYPES>
{
	/// <summary>Уникален идентификатор за ред</summary>
	long lID;
	/// <summary>Контрол на версиите</summary>
	long lUpdateCounter;
	/// <summary>Тип на телефон</summary>
	TCHAR szType[PHONE_TYPES_TYPE_LENGTH + 1];

	/// <summary> Setter за Type </summary>
	void SetType(const TCHAR* pszType) { _tcscpy_s(this->szType, pszType); }

	BOOL Compare(PHONE_TYPES& recPhoneType) const override
	{
		return _tcscmp(this->szType, recPhoneType.szType) ||
			(this->lID - recPhoneType.lID) ||
			(this->lUpdateCounter - recPhoneType.lUpdateCounter);
	}

	PHONE_TYPES()
	{
		SecureZeroMemory(this, sizeof(*this));
	}

	~PHONE_TYPES() {}
};
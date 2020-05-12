#pragma once

#include "CitiesData.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

/// <summary>Клас документ на CITIES</summary>
class CCitiesDocument : public CDocument
{

	// Constructor / Destructor
	// ----------------
public:
	CCitiesDocument();
	~CCitiesDocument();

	DECLARE_DYNCREATE(CCitiesDocument)

	// Overrides
	// ----------------

private:
	/// <summary>Събира първоначална информация при инстанциране на нов документ</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех (ако не успее да изтегли данните от бд)- FALSE</returns>
	/// <remarks>Изтегля всички записи от бд и ги записва в полето за данни m_oCitiesArray, като създава индекс за индексите от m_oCitiesArray по Id в m_oCitiesIndexesOfIds</remarks>
	BOOL OnNewDocument() override;

	void Serialize(CArchive& ar) override;

	// Methods
	// ----------------

public:
	/// <summary>Достъп до поле m_oCitiesArray</summary>
	/// <returns>Референция към m_oCitiesArray</returns>
	CCitiesArray& GetAllCities();

	/// <summary>Достъп до запис от m_oCitiesArray</summary>
	/// <param name="lID">Id на град, който се търси</param>
	/// <returns>Референция към запис от m_oCitiesArray</returns>
	/// <remarks>Ако записът не е намерен в полето, се търси в бд и ако се намери, документът се обновява</remarks>
	CITIES& GetCityById(const long lID);

	/// <summary>Обновяване на полето за записи</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL RefreshData();

	/// <summary>Обновяване на единичен запис в бд</summary>
	/// <param name="recCity">Град, с приложените промени</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL EditCity(CITIES& recCity);

	/// <summary>Добавяне на единичен запис в бд</summary>
	/// <param name="recCity">Град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL AddCity(CITIES& recCity);

	/// <summary>Изтриване на единичен запис от бд</summary>
	/// <param name="recCity">Id на град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL DeleteCity(const CITIES& recCity);

private:
	/// <summary>Изтриване на всички записи от m_oCitiesArray и m_oCitiesIndexesOfIds</summary>
	void CleanRepository();

	/// <summary>Започва процедура по обновяване на всички изгледи</summary>
	/// <remarks>Обновява полетата за данни, вдига флаг за промяна и извиква UpdateAllViews</remarks>
	void OnUpdateAllViews(LPARAM lHint, CObject* pHint);

	/// <summary>Добавя запис на град към документа</summary>
	/// <param name="recCity">Запис на град - CITIES</param>
	/// <remarks>Записва в полето за данни m_oCitiesArray, като създава индекс за индексите от m_oCitiesArray по Id в m_oCitiesIndexesOfIds</remarks>
	/// <returns>INT_PTR: индекса на новия град в m_oCitiesArray</returns>
	INT_PTR AddCityToRepository(const CITIES& recCity);

	/// <summary>Премахва запис на град от документа</summary>
	/// <param name="lID">ID на запис на град</param>
	/// <remarks>Премахва от запис от полето за данни m_oCitiesArray и m_oCitiesIndexesOfIds</remarks>
	/// <returns> Връща адреса на паметта, която записът е заемал, като число. Ако изтриването е неуспешно: -1 </returns>
	DWORD_PTR RemoveCityFromRepositoryByID(const long lID);

	/// <summary>Връща указател към запис на град от документа</summary>
	/// <param name="lID">ID на запис на град</param>
	/// <param name="pCity"> Указател, в който ще се запише резултатът </param>
	/// <returns> BOOL - TRUE при успешно намиране и FALSE при неуспех </returns>
	CITIES* GetCityFromRepositoryByID(const long lID);

	/// <summary> Изчиства индексирането в m_oCitiesIndexesOfIds и го създава наново </summary>
	void BuildCitiesIndexMap();

	// Enum
	// ----------------
public:
	enum Operations
	{
		OperationsCreate = 1,
		OperationsRead = 2,
		OperationsUpdate = 3,
		OperationsDelete = 4
	};

	// Class
	// ---------------
public:
	class CCitiesUpdateObject : public CObject
	{
		// Constructor/Destructor
	public:
		explicit CCitiesUpdateObject(DWORD_PTR dwCityData)
			: m_dwCityData(dwCityData) {}
		~CCitiesUpdateObject() {}

		//CCitiesUpdateObject& operator=(const CITIES& recCity);

	// Methods
	// ----------------
		DWORD_PTR GetUpdateCityData() const { return this->m_dwCityData; };

		// Members
		// ----------------
	private:
		DWORD_PTR m_dwCityData;
	};

	// Members
	// ----------------
private:
	CCitiesData m_oCitiesData;
	CCitiesArray m_oCitiesArray;
	CMap<long, long, long, long> m_oCitiesIndexesOfIds;

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif
};





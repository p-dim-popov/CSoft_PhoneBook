#pragma once

#include "PhoneTypesData.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDocument

/// <summary>Клас документ на PHONE_TYPES</summary>
class CPhoneTypesDocument : public CDocument
{

	// Constructor / Destructor
	// ----------------
public:
	CPhoneTypesDocument();
	~CPhoneTypesDocument();

	DECLARE_DYNCREATE(CPhoneTypesDocument)

	// Overrides
	// ----------------

private:
	/// <summary>Събира първоначална информация при инстанциране на нов документ</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех (ако не успее да изтегли данните от бд)- FALSE</returns>
	/// <remarks>Изтегля всички записи от бд и ги записва в полето за данни m_oPhoneTypesArray, като създава индекс за индексите от m_oPhoneTypesArray по Id в m_oPhoneTypesIndexesOfIds</remarks>
	BOOL OnNewDocument() override;

	void Serialize(CArchive& ar) override;

	// Methods
	// ----------------

public:
	/// <summary>Достъп до поле m_oPhoneTypesArray</summary>
	/// <returns>Референция към m_oPhoneTypesArray</returns>
	CPhoneTypesArray& GetAllPhoneTypes();

	/// <summary>Достъп до запис от m_oPhoneTypesArray</summary>
	/// <param name="lID">Id на град, който се търси</param>
	/// <returns>Референция към запис от m_oPhoneTypesArray</returns>
	/// <remarks>Ако записът не е намерен в полето, се търси в бд и ако се намери, документът се обновява</remarks>
	PHONE_TYPES& GetPhoneTypeById(const long lID);

	/// <summary>Обновяване на полето за записи</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL RefreshData();

	/// <summary>Обновяване на единичен запис в бд</summary>
	/// <param name="recPhoneType">Град, с приложените промени</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL EditPhoneType(PHONE_TYPES& recPhoneType);

	/// <summary>Добавяне на единичен запис в бд</summary>
	/// <param name="recPhoneType">Град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL AddPhoneType(PHONE_TYPES& recPhoneType);

	/// <summary>Изтриване на единичен запис от бд</summary>
	/// <param name="recPhoneType">Id на град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL DeletePhoneType(const PHONE_TYPES& recPhoneType);

private:
	/// <summary>Изтриване на всички записи от m_oPhoneTypesArray и m_oPhoneTypesIndexesOfIds</summary>
	void CleanRepository();

	/// <summary>Започва процедура по обновяване на всички изгледи</summary>
	/// <remarks>Обновява полетата за данни, вдига флаг за промяна и извиква UpdateAllViews</remarks>
	void OnUpdateAllViews(LPARAM lHint, CObject* pHint);

	/// <summary>Добавя запис на град към документа</summary>
	/// <param name="recPhoneType">Запис на град - PHONE_TYPES</param>
	/// <remarks>Записва в полето за данни m_oPhoneTypesArray, като създава индекс за индексите от m_oPhoneTypesArray по Id в m_oPhoneTypesIndexesOfIds</remarks>
	/// <returns>INT_PTR: индекса на новия град в m_oPhoneTypesArray</returns>
	INT_PTR AddPhoneTypeToRepository(const PHONE_TYPES& recPhoneType);

	/// <summary>Премахва запис на град от документа</summary>
	/// <param name="lID">ID на запис на град</param>
	/// <remarks>Премахва от запис от полето за данни m_oPhoneTypesArray и m_oPhoneTypesIndexesOfIds</remarks>
	/// <returns> Връща адреса на паметта, която записът е заемал, като число. Ако изтриването е неуспешно: -1 </returns>
	DWORD_PTR RemovePhoneTypeFromRepositoryByID(const long lID);

	/// <summary>Връща указател към запис на град от документа</summary>
	/// <param name="lID">ID на запис на град</param>
	/// <param name="pPhoneType"> Указател, в който ще се запише резултатът </param>
	/// <returns> BOOL - TRUE при успешно намиране и FALSE при неуспех </returns>
	PHONE_TYPES* GetPhoneTypeFromRepositoryByID(const long lID);

	/// <summary> Изчиства индексирането в m_oPhoneTypesIndexesOfIds и го създава наново </summary>
	void InitPhoneTypesIndexMap();

	// Class
	// ---------------
public:
	class CPhoneTypesUpdateObject : public CObject
	{
		// Constructor/Destructor
	public:
		explicit CPhoneTypesUpdateObject(DWORD_PTR dwPhoneTypeData)
			: m_dwPhoneTypeData(dwPhoneTypeData) {}
		~CPhoneTypesUpdateObject() {}

		//CPhoneTypesUpdateObject& operator=(const PHONE_TYPES& recPhoneType);

	// Methods
	// ----------------
		DWORD_PTR GetUpdatePhoneTypeData() const { return this->m_dwPhoneTypeData; };

		// Members
		// ----------------
	private:
		DWORD_PTR m_dwPhoneTypeData;
	};

	// Members
	// ----------------
private:
	CPhoneTypesData m_oPhoneTypesData;
	CPhoneTypesArray m_oPhoneTypesArray;
	CMap<long, long, long, long> m_oPhoneTypesIndexesOfIds;

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif
};





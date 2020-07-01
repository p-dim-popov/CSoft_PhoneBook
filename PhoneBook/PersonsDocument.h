#pragma once

#include "PersonsData.h"
#include "Utilities.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDocument

/// <summary>Клас документ на PERSONS</summary>
class CPersonsDocument : public CDocument
{

	// Constructor / Destructor
	// ----------------
public:
	CPersonsDocument();
	~CPersonsDocument();

	DECLARE_DYNCREATE(CPersonsDocument)

	// Overrides
	// ----------------

private:
	/// <summary>Събира първоначална информация при инстанциране на нов документ</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех (ако не успее да изтегли данните от бд)- FALSE</returns>
	/// <remarks>Изтегля всички записи от бд и ги записва в полето за данни m_oPersonsArray, като създава индекс за индексите от m_oPersonsArray по Id в m_oPersonsIndexesOfIds</remarks>
	BOOL OnNewDocument() override;

	void Serialize(CArchive& ar) override;

	// Methods
	// ----------------

public:
	/// <summary>Достъп до поле m_oPersonsArray</summary>
	/// <returns>Референция към m_oPersonsArray</returns>
	CPersonsArray& GetAllPersons();

	CPhoneTypesArray& GetAllPhoneTypes();
	CCitiesArray& GetAllCities();
	CPhoneNumbersArray GetAllPhonesForPersonByID(long lID);

	/// <summary>Достъп до запис от m_oPersonsArray</summary>
	/// <param name="lID">Id на град, който се търси</param>
	/// <returns>Референция към запис от m_oPersonsArray</returns>
	/// <remarks>Ако записът не е намерен в полето, се търси в бд и ако се намери, документът се обновява</remarks>
	PERSONS* GetPersonById(long lID);

	//CITIES* GetCityByID(long lID);
	//PHONE_TYPES* GetPhoneTypeByID(long lID);

	/// <summary>Обновяване на полето за записи</summary>
	/// <returns>BOOL: при успех - TRUE, при неуспех - FALSE</returns>
	BOOL RefreshData(BOOL bFirstTime = FALSE);

	/// <summary>Добавяне на единичен запис в бд</summary>
	/// <param name="recPerson">Град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL AddPerson(PERSONS& recPerson, CPhoneNumbersArray& oPhoneNumbersArray, CPhoneTypesArray* pPhoneTypesArray = NULL, CITIES* pCity = NULL);

	void CombineNewAndUpdatedPhoneNumbers(const CPhoneNumbersArray& oNewPhoneNumbersArray, const CPhoneNumbersArray& oUpdatedPhoneNumbersArray, CPhoneNumbersArray& oPhoneNumbersArray);

	BOOL EditPerson(
		PERSONS& recPerson,
		CPhoneNumbersArray& oDeletedPhoneNumbersArray,
		CPhoneNumbersArray& oUpdatedPhoneNumbersArray,
		CPhoneNumbersArray& oNewPhoneNumbersArray,
		CPhoneTypesArray* pNewPhoneTypesArray = NULL,
		CITIES* pNewCity = NULL
	);

	BOOL DeleteAllPhonesForPersonByID(const long lID, CPhoneNumbersArray& oPhoneNumbersToDelete);

	/// <summary>Изтриване на единичен запис от бд</summary>
	/// <param name="recPerson">Id на град</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	BOOL DeletePerson(const PERSONS& recPerson);

private:
	/// <summary>Изтриване на всички записи от m_oPersonsArray и m_oPersonsIndexesOfIds</summary>
	void CleanRepositories();

	template<typename RECORD>
	void CleanRepository(CAutoDeleteTypedPtrArray<RECORD>& oRecordsArray, CMap<long, long, long, long>& oRecordsIndexesOfIds)
	{
		oRecordsArray.DeleteAll();
		oRecordsIndexesOfIds.RemoveAll();
	}

	/// <summary> Изчиства индексирането и го създава наново </summary>
	template<typename RECORD>
	void InitRecordsIndexMap(CAutoDeleteTypedPtrArray<RECORD>& oRecordsArray, CMap<long, long, long, long>& oRecordsIndexesOfIds)
	{
		oRecordsIndexesOfIds.RemoveAll();
		for (INT_PTR i = 0; i < oRecordsArray.GetCount(); i++)
		{
			RECORD* pCurrentRecord = oRecordsArray.GetAt(i);
			oRecordsIndexesOfIds.SetAt(pCurrentRecord->lID, i);
		}
	}

	template<typename RECORD>
	INT_PTR AddRecordToRepository(RECORD& recTableRecord, CAutoDeleteTypedPtrArray<RECORD>& oRecordsArray, CMap<long, long, long, long>& oRecordsIndexesOfIds)
	{
		const INT_PTR nIndexOfRecord = oRecordsArray.Add(new RECORD(recTableRecord));
		oRecordsIndexesOfIds.SetAt(recTableRecord.lID, nIndexOfRecord);
		return nIndexOfRecord;
	}

	/// <summary>Премахва запис на град от документа</summary>
	/// <remarks>Премахва от запис от полето за данни m_oPersonsArray и m_oPersonsIndexesOfIds</remarks>
	/// <returns> Връща адреса на паметта, която записът е заемал, като число. Ако изтриването е неуспешно: -1 </returns>
	template<typename RECORD>
	DWORD_PTR RemoveRecordFromRepositoryByID(long lID, CAutoDeleteTypedPtrArray<RECORD>& oRecordsArray, CMap<long, long, long, long>& oRecordsIndexesOfIds)
	{
		long lIndex = -1;

		oRecordsIndexesOfIds.Lookup(lID, lIndex);

		if (lIndex < 0 || lIndex >= oRecordsArray.GetCount())
		{
			TRACE(_T("Record not found in document, but claimed. Record id - %d"), lID);
			return NULL;
		}

		RECORD* pRecord = oRecordsArray.GetAt(lIndex);

		if (!pRecord)
		{
			TRACE(_T("Record not found in document, but exists in index map. Record id - %d"), lID);
			return NULL;
		}

		const DWORD_PTR dwDeletedRecordAddress = reinterpret_cast<DWORD_PTR>(pRecord);
		delete pRecord;
		oRecordsArray.RemoveAt(lIndex);

		InitRecordsIndexMap(oRecordsArray, oRecordsIndexesOfIds);

		return dwDeletedRecordAddress;
	}

	/// <summary>Връща указател към запис на град от документа</summary>
	template<typename RECORD>
	RECORD* GetRecordFromRepositoryByID(long lID, CAutoDeleteTypedPtrArray<RECORD>& oRecordsArray, CMap<long, long, long, long>& oRecordsIndexesOfIds)
	{
		long lIndex = -1;

		oRecordsIndexesOfIds.Lookup(lID, lIndex);

		if (lIndex < 0 || lIndex >= oRecordsArray.GetCount())
		{
			TRACE(_T("Record not found in document, but claimed. Record id - %d"), lID);
			return NULL;
		}

		RECORD* pRecord = oRecordsArray.GetAt(lIndex);

		if (!pRecord)
		{
			TRACE(_T("Record not found in document, but exists in index map. Record id - %d"), lID);
			return NULL;
		}

		return pRecord;
	}

	/// <summary>Започва процедура по обновяване на всички изгледи</summary>
	/// <remarks>Обновява полетата за данни, вдига флаг за промяна и извиква UpdateAllViews</remarks>
	void OnUpdateAllViews(LPARAM lHint, CObject* pHint);

	// Class
	// ---------------
public:
	class CPersonsUpdateObject : public CObject
	{
		// Constructor/Destructor
		// ---------------
	public:
		explicit CPersonsUpdateObject(DWORD_PTR dwData) : m_dwData(dwData) {}

		~CPersonsUpdateObject() {}

		// Methods
		// ----------------
	public:
		DWORD_PTR GetData() const { return m_dwData; }
		// Members
		// ----------------
	private:
		DWORD_PTR m_dwData;
	};

	// Members
	// ----------------
private:
	CPersonsData m_oPersonsData;

	CPersonsArray m_oPersonsArray;
	CPhoneNumbersArray m_oPhoneNumbersArray; //TODO: choose wheter to select from database or iterate to find in existing
	CPhoneTypesArray m_oPhoneTypesArray;
	CCitiesArray m_oCitiesArray;

	CMap<long, long, long, long> m_oPersonsIndexesOfIds;
	//CMap<long, long, long, long> m_oPhoneNumbersIndexesOfIds;
	//CMap<long, long, long, long> m_oPhoneTypesIndexesOfIds;
	//CMap<long, long, long, long> m_oCitiesIndexesOfIds;

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif
};





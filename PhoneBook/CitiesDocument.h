#pragma once

#include "CitiesData.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDocument

/// <summary>Клас документ на CITIES</summary>
class CCitiesDocument :	public CDocument
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
	/// <returns>Указател към m_oCitiesArray</returns>
	CCitiesArray* GetAllCities();

	/// <summary>Достъп до запис от m_oCitiesArray</summary>
	/// <param name="lId">Id на град, който се търси</param>
	/// <returns>Указател към запис от m_oCitiesArray</returns>
	/// <remarks>Ако записът не е намерен в полето, се търси в бд и ако се намери, документът се обновява</remarks>
	CITIES* GetCityById(long lId);

	/// <summary>Обновяване на полето за записи</summary>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	bool UpdateCitiesInDocument();

	/// <summary>Обновяване на единичен запис в бд</summary>
	/// <param name="recCity">Град, с приложените промени</param>
	/// <param name="pView">Указател към изглед, извършил промяната</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	bool EditCity(CITIES& recCity, CView* pView = nullptr);

	/// <summary>Добавяне на единичен запис в бд</summary>
	/// <param name="recCity">Град</param>
	/// <param name="pView">Указател към изглед, извършил промяната</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	bool AddCityToDb(CITIES& recCity, CView* pView = nullptr);

	/// <summary>Изтриване на единичен запис от бд</summary>
	/// <param name="lId">Id на град</param>
	/// <param name="pView">Указател към изглед, извършил промяната</param>
	/// <returns>bool: при успех - true, при неуспех - false</returns>
	bool DeleteCity(const long lId, CView* pView = nullptr);
	
private:
	/// <summary>Изтриване на всички записи от m_oCitiesArray и m_oCitiesIndexesOfIds</summary>
	void DoEmptyRepository();

	/// <summary>Започва процедура по обновяване на всички изгледи</summary>
	/// <param name="pView">Указател към изглед, извършил промяната</param>
	/// <remarks>Обновява полетата за данни, вдига флаг за промяна и извиква UpdateAllViews</remarks>
	void OnUpdateAllViews(CView* pView = nullptr);

	/// <summary>Добавя запис на град към документа</summary>
	/// <param name="recCity">Запис на град - CITIES</param>
	/// <remarks>Записва в полето за данни m_oCitiesArray, като създава индекс за индексите от m_oCitiesArray по Id в m_oCitiesIndexesOfIds</remarks>
	INT_PTR AddCityToRepository(CITIES& recCity);

	/// <summary>Премахва запис на град от документа</summary>
	/// <param name="lId">Id на запис на град</param>
	/// <remarks>Премахва от запис от полето за данни m_oCitiesArray и m_oCitiesIndexesOfIds</remarks>
	void RemoveCityFromRepository(const long lId);

	/// <summary>Изкарва MessageBox с подаденото съобщение и грешка</summary>
	/// <param name="nError">Грешка, която се е случила</param>
	/// <param name="pszMessage">Съобщение, което ще се изведе</param>
	/// <returns>bool: при OK - true, при CANCEL - false</returns>
	bool PromptErrorOn(const INT nError, const TCHAR* pszMessage);
	
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





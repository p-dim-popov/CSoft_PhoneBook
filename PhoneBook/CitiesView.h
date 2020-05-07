#pragma once
#include "CitiesDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

/// <summary>Изглед клас за CITIES</summary>
class CCitiesView : public CListView
{

// Macros
// ----------------

	DECLARE_DYNCREATE(CCitiesView)
	DECLARE_MESSAGE_MAP()
	
// Constructor / Destructor
// ----------------

protected:
	CCitiesView();
	virtual ~CCitiesView();

// Overrides
// ----------------

public:
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	/// <summary>Визуализиране на данните при първоначално зареждане на изглед</summary>
	void OnInitialUpdate() override;

	/// <summary>(Handler) Обновяване на данните на вече създаден изглед.</summary>
	/// <param name="pSender">Указател към изглед, направил промените</param>
	/// <param name="lHint">...</param>
	/// <param name="pHint">Обект с информация за промените</param>
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif
	
// Methods
// ----------------
private:
	/// <summary>(Handler) При избор на елемент от лист контролата отваря диалог за редакция</summary>
	afx_msg void OnLvnItemActivate(NMHDR* pNMHDR = nullptr, LRESULT* pResult = nullptr);

	// Опции на контекстно меню върху ред от лист контролата
	#pragma region ContextMenuOptions

	/// <summary>Отваря диалог за изтриване на запис</summary>
	void OnContextMenuBtnDelete();

	/// <summary>Отваря диалог за добавяне на запис</summary>
	void OnContextMenuBtnInsert();
	
	/// <summary>Отваря диалог за редактиране на запис</summary>
	void OnContextMenuBtnUpdate();
	
	/// <summary>Отваря диалог за опресняване на данните</summary>
	void OnContextMenuBtnRefresh();
	
	#pragma endregion ContextMenuOptions

	/// <summary>(Handler) При отваряне на контекстно меню в лист контролата. Визуализира меню с CRUD операции и Refresh</summary>
	/// <param name="pWnd">Указател към прозорец с осъщественото действие</param>
	/// <param name="oPoint">Обект с информация за местоположението на курсора</param>
	void OnContextMenu(CWnd* pWnd, CPoint oPoint);

	/// <summary>Обновяване на данните в лист контрола</summary>
	void LoadRowsData();

	/// <summary>Достъп до документа</summary>
	/// <returns>Указател към документа - CCitiesDocument
	CCitiesDocument* GetDocument() const;

	/// <summary>Изкарва MessageBox с подаденото съобщение и грешка</summary>
	/// <param name="nError">Грешка, която се е случила</param>
	/// <param name="pszMessage">Съобщение, което ще се изведе</param>
	/// <returns>bool: при OK - true, при CANCEL - false</returns>
	bool PromptErrorOn(const INT nError, const TCHAR* pszMessage);

	INT GetCityIndexInListCtrlByItemData(DWORD_PTR dwPtr) const;
	
	void UpdateOnOperationCreate(const DWORD_PTR dwCityItemData);
	void UpdateOnOperationUpdate(const DWORD_PTR dwCityItemData);
	void UpdateOnOperationDelete(const DWORD_PTR dwCityItemData);
	void ClearRowsData();


// Members
// ----------------
private:
	CITIES m_recCity;
};



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
	void OnSelectDeleteInContextMenu();

	/// <summary>Отваря диалог за добавяне на запис</summary>
	void OnSelectInsertInContextMenu();
	
	/// <summary>Отваря диалог за редактиране на запис</summary>
	void OnSelectUpdateInContextMenu();
	
	/// <summary>Отваря диалог за опресняване на данните</summary>
	void OnSelectRefreshInContextMenu();
	
	#pragma endregion ContextMenuOptions

	/// <summary>(Handler) При отваряне на контекстно меню в лист контролата. Визуализира меню с CRUD операции и Refresh</summary>
	/// <param name="pWnd">Указател към прозорец с осъщественото действие</param>
	/// <param name="oPoint">Обект с информация за местоположението на курсора</param>
	void OnContextMenu(CWnd* pWnd, CPoint oPoint);
	
	/// <summary>Обновяване на данните в лист контрола</summary>
	/// <param name="pListCtrl">Указател към лист контрола</param>
	void UpdateRowsData(CListCtrl* pListCtrl = nullptr);

	/// <summary>Достъп до документа</summary>
	/// <returns>Указател към документа - CCitiesDocument
	CCitiesDocument* GetDocument() const;
	
// Members
// ----------------
private:

};



#pragma once
#include "PersonsDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsView

/// <summary>Изглед клас за PERSONS</summary>
class CPersonsView : public CListView
{

	// Macros
	// ----------------

	DECLARE_DYNCREATE(CPersonsView)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------

protected:
	CPersonsView() {}
	virtual ~CPersonsView() {}

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

	// Methods
	// ----------------
private:
	/// <summary>Достъп до документа</summary>
	/// <returns>Указател към документа - CPersonsDocument
	CPersonsDocument* GetDocument() const;

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

	/// <summary> Опресняване на данните</summary>
	void OnContextMenuBtnRefresh();

#pragma endregion ContextMenuOptions

	/// <summary>(Handler) При отваряне на контекстно меню в лист контролата. Визуализира меню с CRUD операции и Refresh</summary>
	/// <param name="pWnd">Указател към прозорец с осъщественото действие</param>
	/// <param name="oPoint">Обект с информация за местоположението на курсора</param>
	void OnContextMenu(CWnd* pWnd, CPoint oPoint);

	/// <summary>Обновяване на данните в лист контрола</summary>
	void LoadRowsData();

	/// <summary> Намира индекс на град от лист контролата по атрибута за данни </summary>
	/// <returns>INT: индекса от ListCtrl</returns>
	INT GetPersonIndexInListCtrlByItemData(DWORD_PTR dwData) const;

#pragma region OnUpdateCases
	/// <summary>Вмъква нов град в ListCtrl при сигнал за създаване</summary>
	/// <param name="dwPersonItemData"> входящата информация от документа </param>
	void UpdateOnOperationCreate(const DWORD_PTR dwPersonItemData);
	/// <summary>Ъпдейтва град от ListCtrl при сигнал за ъпдейт</summary>
	/// <param name="dwPersonItemData">входящата информация от документа</param>
	void UpdateOnOperationUpdate(const DWORD_PTR dwPersonItemData);
	/// <summary>Изтрива град от ListCtrl при сигнал за изтриване</summary>
	/// <param name="dwPersonItemData">входящата информация от документа</param>
	void UpdateOnOperationDelete(const DWORD_PTR dwPersonItemData);
#pragma endregion OnUpdateCases

	/// <summary> Зачиства всички редове от List контролата</summary>
	void ClearRowsData();

#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif

	// Members
	// ----------------
private:
};



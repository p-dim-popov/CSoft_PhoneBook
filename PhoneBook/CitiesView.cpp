// CitiesView.cpp : implementation file
//

#include "pch.h"
#include "CitiesView.h"

#include "CitiesDialog.h"
#include "resource.h"

#define CITY_NAME_COLUMN 0
#define CITY_REGION_COLUMN 1
#define CITY_NAME_COLUMN_LABEL _T("Name")
#define CITY_REGION_COLUMN_LABEL _T("Region")
#define CITY_NAME_COLUMN_WIDTH 150
#define CITY_REGION_COLUMN_WIDTH 150
#define CITIES_CREATE_ERROR 0
#define CITIES_READ_ERROR 1
#define CITIES_UPDATE_ERROR 2
#define CITIES_DELETE_ERROR 3

#define GENERAL_ERROR_MESSAGE _T("Sorry for the inconvenience but error ocurred! Try refreshing the list.")

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)
// Връзване на флаг съобщения с методи
BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CCitiesView::OnLvnItemActivate)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------

CCitiesView::CCitiesView()
{

}

CCitiesView::~CCitiesView()
{
}

// Overrides
// ----------------

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& oListCtrl = GetListCtrl();

	// Вмъкване на колони
	oListCtrl.InsertColumn(CITY_NAME_COLUMN, CITY_NAME_COLUMN_LABEL);
	oListCtrl.InsertColumn(CITY_REGION_COLUMN, CITY_REGION_COLUMN_LABEL);

	// Задаване на ширина на колоните
	oListCtrl.SetColumnWidth(CITY_NAME_COLUMN, CITY_NAME_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(CITY_REGION_COLUMN, CITY_REGION_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);

	LoadRowsData();
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CView::OnUpdate(pSender, lHint, pHint);

	if (lHint && pHint)
	{
		const CCitiesDocument::CCitiesUpdateObject* oCitiesUpdateObject = reinterpret_cast<CCitiesDocument::CCitiesUpdateObject*>(pHint);
		const DWORD_PTR dwCityItemData = oCitiesUpdateObject->GetUpdateCityData();

		switch (lHint)
		{
		case Utilities::OperationsCreate:
			UpdateOnOperationCreate(dwCityItemData);
			break;
		case Utilities::OperationsUpdate:
			UpdateOnOperationUpdate(dwCityItemData);
			break;
		case Utilities::OperationsDelete:
			UpdateOnOperationDelete(dwCityItemData);
			break;
		default:
			break;
		}
	}
}

// Methods
// ----------------

CCitiesDocument* CCitiesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDocument)));
	return dynamic_cast<CCitiesDocument*>(m_pDocument);
}

// CCitiesView message handlers
// Функции за обработка на флаг/съобщения

void CCitiesView::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CCitiesDocument* pCitiesDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	CITIES recCity = *reinterpret_cast<CITIES*>(oListCtrl.GetItemData(nIndex));

	CCitiesDialog oCitiesDialog(recCity, Utilities::OperationsRead);

	oCitiesDialog.DoModal();

	if (pResult)
	{
		*pResult = 0;
	}
}

// Функции за избор при контекстно меню
#pragma region ContextMenuOptions
void CCitiesView::OnContextMenuBtnDelete()
{
	CCitiesDocument* pCitiesDocument = GetDocument();
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	CITIES recCity = *reinterpret_cast<CITIES*>(oListCtrl.GetItemData(nIndex));
	CString strAreYouSurePrompt;
	strAreYouSurePrompt.Format(_T("Are you sure that you want to delete city %s?"), recCity.szName);

	const int nResult = MessageBox(strAreYouSurePrompt, _T("Are you sure?"), MB_ICONINFORMATION | MB_OKCANCEL);

	if (nResult != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pCitiesDocument->DeleteCity(recCity);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE, _T("Information"),MB_ICONINFORMATION | MB_OK);
	}
}

void CCitiesView::OnContextMenuBtnInsert()
{
	CITIES recCity = CITIES();
	CCitiesDocument* pCitiesDocument = GetDocument();
	CCitiesDialog oCitiesDialog(recCity, Utilities::OperationsCreate);

	if (oCitiesDialog.DoModal() != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pCitiesDocument->AddCity(recCity);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE, _T("Information"), MB_ICONINFORMATION | MB_OK);
	}
}

void CCitiesView::OnContextMenuBtnUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();
	CCitiesDocument* pCitiesDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	CITIES recCity = *reinterpret_cast<CITIES*>(oListCtrl.GetItemData(nIndex));

	CCitiesDialog oCitiesUpdateDialog(recCity, Utilities::OperationsUpdate);

	if (oCitiesUpdateDialog.DoModal() != IDOK)
	{
		return;
	}

	const BOOL bIsSuccessful = pCitiesDocument->EditCity(recCity);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE,_T("Information"),MB_ICONINFORMATION | MB_OK);
	}
}

void CCitiesView::OnContextMenuBtnRefresh()
{
	// Ъпдейт при ползване от повече от една инстанция на приложението
	CCitiesDocument* pCitiesDocument = GetDocument();
	pCitiesDocument->RefreshData();

	ClearRowsData();
	LoadRowsData();
}
#pragma endregion ContextMenuOptions

void CCitiesView::OnContextMenu(CWnd* pWnd /*= nullptr*/, CPoint oMousePos)
{
	ScreenToClient(&oMousePos);

	CMenu oMenu;
	oMenu.LoadMenu(IDR_CLISTCTRL_CONTEXT_MENU);

	CListCtrl& oListCtrl = GetListCtrl();
	const int nSelectedItem = oListCtrl.HitTest(oMousePos);

	// Ако менюто не е отворено върху елемент, се скриват опциите за елемент
	if (nSelectedItem == -1)
	{
		oMenu.RemoveMenu(ID_CONTEXT_OPTIONS_EDIT, MF_BYCOMMAND);
		oMenu.RemoveMenu(ID_CONTEXT_OPTIONS_DELETE, MF_BYCOMMAND);
	}

	CMenu* pPopup = oMenu.GetSubMenu(0);

	ClientToScreen(&oMousePos);
	const int nSelectedOption = int(pPopup->TrackPopupMenu(
		TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_RETURNCMD,
		oMousePos.x,
		oMousePos.y,
		this));

	switch (nSelectedOption)
	{
	case ID_CONTEXT_OPTIONS_EDIT:
		OnContextMenuBtnUpdate();
		break;
	case ID_CONTEXT_OPTIONS_DELETE:
		OnContextMenuBtnDelete();
		break;
	case ID_CONTEXT_OPTIONS_INSERT:
		OnContextMenuBtnInsert();
		break;
	case ID_CONTEXT_OPTIONS_REFRESH:
		OnContextMenuBtnRefresh();
		break;
	default:
		break;
	}
}

void CCitiesView::LoadRowsData()
{
	CListCtrl& oListCtrl = GetListCtrl();

	// Документ - за работа с данните
	CCitiesDocument* pCitiesDocument = GetDocument();

	// Данни - CITIES
	CCitiesArray& oCitiesArray = pCitiesDocument->GetAllCities();

	// Зареждане на всички градове в списък в лист контролата
	for (INT_PTR i = 0; i < oCitiesArray.GetCount(); i++)
	{
		CITIES* pCity = oCitiesArray.GetAt(i);

		// Вмъкване на редове
		const INT nInsertIndex = oListCtrl.InsertItem(i, pCity->szName);
		oListCtrl.SetItemText(nInsertIndex, CITY_REGION_COLUMN, pCity->szRegion);
		oListCtrl.SetItemData(nInsertIndex, reinterpret_cast<DWORD_PTR>(pCity));
	}
}

INT CCitiesView::GetCityIndexInListCtrlByItemData(const DWORD_PTR dwData) const
{
	CListCtrl& oListCtrl = GetListCtrl();

	INT nIndex = -1;

	for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
	{
		if (oListCtrl.GetItemData(i) == dwData)
		{
			nIndex = i;
		}
	}

	return nIndex;
}

#pragma region OnUpdateCases
void CCitiesView::UpdateOnOperationCreate(const DWORD_PTR dwCityItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CITIES* pCity = reinterpret_cast<CITIES*>(dwCityItemData);

	// Вмъкване на редове
	const INT nInsertIndex = oListCtrl.InsertItem(oListCtrl.GetItemCount(), pCity->szName);
	oListCtrl.SetItemText(nInsertIndex, CITY_REGION_COLUMN, pCity->szRegion);
	oListCtrl.SetItemData(nInsertIndex, dwCityItemData);
}

void CCitiesView::UpdateOnOperationUpdate(const DWORD_PTR dwCityItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CITIES* pCity = reinterpret_cast<CITIES*>(dwCityItemData);

	pCity = reinterpret_cast<CITIES*>(dwCityItemData);
	const INT nIndexInListCtrl = GetCityIndexInListCtrlByItemData(dwCityItemData);

	oListCtrl.SetItemText(nIndexInListCtrl, CITY_NAME_COLUMN, pCity->szName);
	oListCtrl.SetItemText(nIndexInListCtrl, CITY_REGION_COLUMN, pCity->szRegion);
}

void CCitiesView::UpdateOnOperationDelete(const DWORD_PTR dwCityItemData)
{
	const INT nIndexInListCtrl = GetCityIndexInListCtrlByItemData(dwCityItemData);
	GetListCtrl().DeleteItem(nIndexInListCtrl);
}
#pragma endregion OnUpdateCases

void CCitiesView::ClearRowsData()
{
	CListCtrl& oListCtrl = GetListCtrl();

	const int nItemsCount = oListCtrl.GetItemCount();
	if (nItemsCount > 0)
	{
		oListCtrl.SetRedraw(FALSE);
		oListCtrl.DeleteAllItems();
		oListCtrl.SetRedraw(TRUE);
	}
}


// CCitiesView diagnostics
#ifdef _DEBUG
void CCitiesView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CCitiesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

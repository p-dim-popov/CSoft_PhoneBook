// CitiesView.cpp : implementation file
//

#include "pch.h"
#include "CitiesView.h"

#include "CitiesDialog.h"
#include "resource.h"

#define CITY_ID_COLUMN 0
#define CITY_NAME_COLUMN 1
#define CITY_REGION_COLUMN 2
#define CITY_UPDATE_COUNTER_COLUMN 3
#define CITY_ID_COLUMN_LABEL _T("ID")
#define CITY_NAME_COLUMN_LABEL _T("Име")
#define CITY_REGION_COLUMN_LABEL _T("Регион")
#define CITY_UPDATE_COUNTER_COLUMN_LABEL _T("")
#define CITY_ID_COLUMN_WIDTH 0
#define CITY_NAME_COLUMN_WIDTH 150
#define CITY_REGION_COLUMN_WIDTH 150
#define CITY_UPDATE_COUNTER_COLUMN_WIDTH 0

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

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

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& oListCtrl = GetListCtrl();

	// Вмъкване на колони
	oListCtrl.InsertColumn(CITY_NAME_COLUMN, CITY_NAME_COLUMN_LABEL);
	oListCtrl.InsertColumn(CITY_REGION_COLUMN, CITY_REGION_COLUMN_LABEL);
	oListCtrl.InsertColumn(CITY_ID_COLUMN, CITY_ID_COLUMN_LABEL);
	oListCtrl.InsertColumn(CITY_UPDATE_COUNTER_COLUMN, CITY_UPDATE_COUNTER_COLUMN_LABEL);

	// Задаване на ширина на колоните
	oListCtrl.SetColumnWidth(CITY_NAME_COLUMN, CITY_NAME_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(CITY_REGION_COLUMN, CITY_REGION_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(CITY_ID_COLUMN, CITY_ID_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(CITY_UPDATE_COUNTER_COLUMN, CITY_UPDATE_COUNTER_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	UpdateRowsData(&oListCtrl);
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CView::OnUpdate(pSender, lHint, pHint);

	UpdateRowsData();
}

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

// Връзване на флаг съобщения с методи
BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CCitiesView::OnLvnItemActivate)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

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

// Methods
// ----------------

CCitiesDocument* CCitiesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDocument)));
	return static_cast<CCitiesDocument*>(m_pDocument);
}

void CCitiesView::UpdateRowsData(CListCtrl* pListCtrl)
{
	if (!pListCtrl)
	{
		pListCtrl = &GetListCtrl();
	}
	
	const int nItemsCount = pListCtrl->GetItemCount();
	if (nItemsCount > 0)
	{
		pListCtrl->SetRedraw(FALSE);
		pListCtrl->DeleteAllItems();
		pListCtrl->SetRedraw(TRUE);

		ASSERT(pListCtrl->GetItemCount() == 0);
	}

	// Документ - за работа с данните
	CCitiesDocument* pCitiesDocument = GetDocument();
	
	// Данни - CITIES
	CCitiesArray* pCitiesArray = pCitiesDocument->GetAllCities();

	// Зареждане на всички градове в списък в лист контролата
	for (INT_PTR i = 0; i < pCitiesArray->GetCount(); i++)
	{
		CITIES* pCity = pCitiesArray->GetAt(i);

		// Вмъкване на редове
		CString strId;
		strId.Format(_T("%d"), pCity->lID);
		CString strUpdateCounter;
		strUpdateCounter.Format(_T("%d"), pCity->lUpdateCounter);

		const INT nInsertIndex = pListCtrl->InsertItem(i, strId);
		pListCtrl->SetItemText(nInsertIndex, CITY_NAME_COLUMN, pCity->szName);
		pListCtrl->SetItemText(nInsertIndex, CITY_REGION_COLUMN, pCity->szRegion);
		pListCtrl->SetItemText(nInsertIndex, CITY_UPDATE_COUNTER_COLUMN, strUpdateCounter);
	}
}

// CCitiesView message handlers
// Функции за обработка на флаг/съобщения

void CCitiesView::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	CITIES recCity;
	CListCtrl& oListCtrl = GetListCtrl();
	CCitiesDocument* pCitiesDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();
	
	if (nIndex == -1)
	{
		return;
	}

	recCity.InitData(
		_tstol(oListCtrl.GetItemText(nIndex, CITY_ID_COLUMN).GetString()),
		_tstol(oListCtrl.GetItemText(nIndex, CITY_UPDATE_COUNTER_COLUMN).GetString()),
		oListCtrl.GetItemText(nIndex, CITY_NAME_COLUMN).GetString(),
		oListCtrl.GetItemText(nIndex, CITY_REGION_COLUMN).GetString());

	CCitiesUpdateDialog oCitiesUpdateDialog(this, &recCity);

	const INT_PTR nDialogEndResult = oCitiesUpdateDialog.DoModal();

	if (nDialogEndResult == IDOK)
	{		
		const bool bIsSuccessful = pCitiesDocument->EditCity(recCity);
		
		if (bIsSuccessful)
		{
			MessageBox(_T("Промените бяха запазени успешно!"), _T("Информация"));
		}
	}

	if (!pResult)
	{
		return;
	}
	
	*pResult = 0;
}

// Функции за избор при контекстно меню

void CCitiesView::OnSelectDeleteInContextMenu()
{
	CITIES recCity;
	CCitiesDocument* pCitiesDocument = GetDocument();
	CCitiesDeleteDialog oCitiesDeleteDialog(this, &recCity);
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	recCity.InitData(
		_tstol(oListCtrl.GetItemText(nIndex, CITY_ID_COLUMN).GetString()),
		_tstol(oListCtrl.GetItemText(nIndex, CITY_UPDATE_COUNTER_COLUMN).GetString()),
		oListCtrl.GetItemText(nIndex, CITY_NAME_COLUMN).GetString(),
		oListCtrl.GetItemText(nIndex, CITY_REGION_COLUMN).GetString());
	
	const INT_PTR nDialogEndResult = oCitiesDeleteDialog.DoModal();

	if (nDialogEndResult == IDOK)
	{
		const bool bIsSuccessful = pCitiesDocument->DeleteCity(recCity.lID);

		if (bIsSuccessful)
		{
			MessageBox(_T("Промените бяха запазени успешно!"), _T("Информация"));
		}
	}
}

void CCitiesView::OnSelectInsertInContextMenu()
{
	CITIES recCity;
	CCitiesDocument* pCitiesDocument = GetDocument();
	CCitiesInsertDialog oCitiesInsertDialog(this, &recCity);

	const INT_PTR nDialogEndResult = oCitiesInsertDialog.DoModal();

	if (nDialogEndResult == IDOK)
	{
		const bool bIsSuccessful = pCitiesDocument->AddCityToDb(recCity);

		if (bIsSuccessful)
		{
			MessageBox(_T("Промените бяха запазени успешно!"), _T("Информация"));
		}
	}
}

void CCitiesView::OnSelectUpdateInContextMenu()
{
	OnLvnItemActivate();
}

void CCitiesView::OnSelectRefreshInContextMenu()
{
	// Ъпдейт при ползване от повече от една инстанция на приложението
	CCitiesDocument* pCitiesDocument = GetDocument();
	pCitiesDocument->UpdateCitiesInDocument();
	
	OnUpdate(nullptr, NULL, nullptr);
}

void CCitiesView::OnContextMenu(CWnd* pWnd /*= nullptr*/, CPoint oMousePos /*= nullptr*/)
{
	CListCtrl& oListCtrl = GetListCtrl();
	ScreenToClient(&oMousePos);

	const int htItem = oListCtrl.HitTest(oMousePos);

	if (htItem == -1)
	{
		return;
	}
	
	CMenu oMenu;

	oMenu.LoadMenu(IDR_CITIES_CONTEXT_MENU);
	CMenu* pPopup = oMenu.GetSubMenu(0);
	
	ClientToScreen(&oMousePos);
	const int nSelectedOption = int(pPopup->TrackPopupMenu(
		TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN | TPM_RETURNCMD, 
		oMousePos.x, 
		oMousePos.y, 
		this));
	
	switch (nSelectedOption)
	{
	case ID_OPTIONS_EDIT_CITIES:
		OnSelectUpdateInContextMenu();
		break;
	case ID_OPTIONS_DELETE_CITIES:
		OnSelectDeleteInContextMenu();
		break;
	case ID_OPTIONS_INSERT_CITIES:
		OnSelectInsertInContextMenu();
		break;
	case ID_OPTIONS_REFRESH_CITIES:
		OnSelectRefreshInContextMenu();
		break;
	default:
		break;
	}
	
}

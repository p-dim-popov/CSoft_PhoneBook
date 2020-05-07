// CitiesView.cpp : implementation file
//

#include "pch.h"
#include "CitiesView.h"

#include "CitiesDialog.h"
#include "resource.h"

#define CITY_NAME_COLUMN 0
#define CITY_REGION_COLUMN 1
#define CITY_NAME_COLUMN_LABEL _T("Име")
#define CITY_REGION_COLUMN_LABEL _T("Регион")
#define CITY_NAME_COLUMN_WIDTH 150
#define CITY_REGION_COLUMN_WIDTH 150
#define CITIES_CREATE_ERROR 0
#define CITIES_READ_ERROR 1
#define CITIES_UPDATE_ERROR 2
#define CITIES_DELETE_ERROR 3

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

	// Задаване на ширина на колоните
	oListCtrl.SetColumnWidth(CITY_NAME_COLUMN, CITY_NAME_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(CITY_REGION_COLUMN, CITY_REGION_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

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
		case CCitiesDocument::OperationsCreate:
			UpdateOnOperationCreate(dwCityItemData);
			break;
		case CCitiesDocument::OperationsUpdate:
			UpdateOnOperationUpdate(dwCityItemData);
			break;
		case CCitiesDocument::OperationsDelete:
			UpdateOnOperationDelete(dwCityItemData);
			break;
		default:
			break;
		}
	}
}

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
	return dynamic_cast<CCitiesDocument*>(m_pDocument);
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

	m_recCity = *reinterpret_cast<CITIES*>(oListCtrl.GetItemData(nIndex));

	CCitiesDialog oCitiesUpdateDialog(m_recCity, CCitiesDocument::OperationsUpdate);

	if (oCitiesUpdateDialog.DoModal() != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pCitiesDocument->EditCity(m_recCity);

	if (!bIsSuccessful)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_UPDATE_ERROR,
			_T("Възникна грешка. \n\
Възможна причина: записът вече е обновен преди настъпване на настоящите промени. \n\
Моля обновете вашите данни и опитайте да направите редакциите отново. \n\
Ако сте сигурни, че това не е проблемът, може да опитате отново."));

		if (bShouldRetry)
		{
			//return EditCity(recCity);
			//TODO: implement
		}
	}
	//TODO: msg in view err only

	if (!pResult)
	{
		return;
	}

	*pResult = 0;
}

// Функции за избор при контекстно меню

void CCitiesView::OnContextMenuBtnDelete()
{
	CCitiesDocument* pCitiesDocument = GetDocument();
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	m_recCity = *reinterpret_cast<CITIES*>(oListCtrl.GetItemData(nIndex));
	CCitiesDialog oCitiesDialog(m_recCity, CCitiesDocument::OperationsDelete);

	if (oCitiesDialog.DoModal() != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pCitiesDocument->DeleteCity(m_recCity);

	if (!bIsSuccessful)
	{
		PromptErrorOn(
			CITIES_DELETE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас, може да опитате отново."));
	}
}

void CCitiesView::OnContextMenuBtnInsert()
{
	m_recCity = CITIES();
	CCitiesDocument* pCitiesDocument = GetDocument();
	CCitiesDialog oCitiesDialog(m_recCity, CCitiesDocument::OperationsCreate);

	if (oCitiesDialog.DoModal() != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pCitiesDocument->AddCity(m_recCity);

	if (!bIsSuccessful)
	{
		const bool bShouldRetry = PromptErrorOn(
			CITIES_CREATE_ERROR,
			_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас или е отстранен, може да опитате отново."));

		if (bShouldRetry)
		{
			//return AddCity(recCity);
			//TODO: implement
		}
	}
}

void CCitiesView::OnContextMenuBtnUpdate()
{
	OnLvnItemActivate();
}

void CCitiesView::OnContextMenuBtnRefresh()
{
	// Ъпдейт при ползване от повече от една инстанция на приложението
	CCitiesDocument* pCitiesDocument = GetDocument();
	pCitiesDocument->RefreshData();

	ClearRowsData();
	LoadRowsData();
}

void CCitiesView::OnContextMenu(CWnd* pWnd /*= nullptr*/, CPoint oMousePos)
{
	ScreenToClient(&oMousePos);

	CMenu oMenu;
	oMenu.LoadMenu(IDR_CITIES_CONTEXT_MENU);

	CListCtrl& oListCtrl = GetListCtrl();
	const int nSelectedItem = oListCtrl.HitTest(oMousePos);

	// Ако менюто не е отворено върху елемент, се скриват опциите за елемент
	if (nSelectedItem == -1)
	{
		oMenu.RemoveMenu(ID_OPTIONS_EDIT_CITIES, MF_BYCOMMAND);
		oMenu.RemoveMenu(ID_OPTIONS_DELETE_CITIES, MF_BYCOMMAND);
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
	case ID_OPTIONS_EDIT_CITIES:
		OnContextMenuBtnUpdate();
		break;
	case ID_OPTIONS_DELETE_CITIES:
		OnContextMenuBtnDelete();
		break;
	case ID_OPTIONS_INSERT_CITIES:
		OnContextMenuBtnInsert();
		break;
	case ID_OPTIONS_REFRESH_CITIES:
		OnContextMenuBtnRefresh();
		break;
	default:
		break;
	}
}

bool CCitiesView::PromptErrorOn(const INT nError, const TCHAR* pszMessage)
{
	//TODO: may need to handle different errors diferently -> nError

	const int nReply = MessageBox(pszMessage, _T("Информация"), MB_ICONINFORMATION | MB_RETRYCANCEL);

	switch (nReply)
	{
	case IDCANCEL:
		return false;
	case IDRETRY:
		return true;
	default:
		MessageBox(_T("Не бяха извършени промени!"), _T("Информация"), MB_ICONWARNING);
		return false;
	}
}

INT CCitiesView::GetCityIndexInListCtrlByItemData(DWORD_PTR dwPtr) const
{
	CListCtrl& oListCtrl = GetListCtrl();
	for (int i = 0; i < oListCtrl.GetItemCount(); ++i)
	{
		if (oListCtrl.GetItemData(i) == dwPtr)
		{
			return i;
		}
	}

	return -1;
}

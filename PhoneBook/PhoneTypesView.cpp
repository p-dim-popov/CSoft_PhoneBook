// PhoneTypesView.cpp : implementation file
//

#include "pch.h"
#include "PhoneTypesView.h"

#include "PhoneTypesDialog.h"
#include "resource.h"

#define PHONE_TYPE_NAME_COLUMN 0
#define PHONE_TYPE_NAME_COLUMN_LABEL _T("Type")
#define PHONE_TYPE_NAME_COLUMN_WIDTH 150

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesView

IMPLEMENT_DYNCREATE(CPhoneTypesView, CListView)
// Връзване на флаг съобщения с методи
BEGIN_MESSAGE_MAP(CPhoneTypesView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CPhoneTypesView::OnLvnItemActivate)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// Constructor / Destructor
// ----------------

CPhoneTypesView::CPhoneTypesView()
{

}

CPhoneTypesView::~CPhoneTypesView()
{
}

// Overrides
// ----------------

BOOL CPhoneTypesView::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CPhoneTypesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& oListCtrl = GetListCtrl();

	// Вмъкване на колони
	oListCtrl.InsertColumn(PHONE_TYPE_NAME_COLUMN, PHONE_TYPE_NAME_COLUMN_LABEL);

	// Задаване на ширина на колоните
	oListCtrl.SetColumnWidth(PHONE_TYPE_NAME_COLUMN, PHONE_TYPE_NAME_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);

	LoadRowsData();
}

void CPhoneTypesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CView::OnUpdate(pSender, lHint, pHint);

	if (lHint && pHint)
	{
		const CPhoneTypesDocument::CPhoneTypesUpdateObject* oPhoneTypesUpdateObject = reinterpret_cast<CPhoneTypesDocument::CPhoneTypesUpdateObject*>(pHint);
		const DWORD_PTR dwPhoneTypeItemData = oPhoneTypesUpdateObject->GetUpdatePhoneTypeData();

		switch (lHint)
		{
		case OperationsCreate:
			UpdateOnOperationCreate(dwPhoneTypeItemData);
			break;
		case OperationsUpdate:
			UpdateOnOperationUpdate(dwPhoneTypeItemData);
			break;
		case OperationsDelete:
			UpdateOnOperationDelete(dwPhoneTypeItemData);
			break;
		default:
			break;
		}
	}
}

// Methods
// ----------------

CPhoneTypesDocument* CPhoneTypesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneTypesDocument)));
	return dynamic_cast<CPhoneTypesDocument*>(m_pDocument);
}

// CPhoneTypesView message handlers
// Функции за обработка на флаг/съобщения

void CPhoneTypesView::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PHONE_TYPES recPhoneType = *reinterpret_cast<PHONE_TYPES*>(oListCtrl.GetItemData(nIndex));

	CPhoneTypesDialog oPhoneTypesDialog(recPhoneType, OperationsRead);

	oPhoneTypesDialog.DoModal();

	if (pResult)
	{
		*pResult = 0;
	}
}

// Функции за избор при контекстно меню
#pragma region ContextMenuOptions
void CPhoneTypesView::OnContextMenuBtnDelete()
{
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PHONE_TYPES recPhoneType = *reinterpret_cast<PHONE_TYPES*>(oListCtrl.GetItemData(nIndex));
	CString strAreYouSurePrompt;
	strAreYouSurePrompt.Format(_T("Are you sure that you want to delete phone type %s?"), recPhoneType.szType);

	const int nResult = MessageBox(strAreYouSurePrompt, _T("Are you sure?"), MB_ICONINFORMATION | MB_OKCANCEL);

	if (nResult != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pPhoneTypesDocument->DeletePhoneType(recPhoneType);

	if (!bIsSuccessful)
	{
		MessageBox(_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас, може да опитате отново."),
_T("Информация"),
MB_ICONINFORMATION | MB_OK);
	}
}

void CPhoneTypesView::OnContextMenuBtnInsert()
{
	PHONE_TYPES recPhoneType = PHONE_TYPES();
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	CPhoneTypesDialog oPhoneTypesDialog(recPhoneType, OperationsCreate);

	if (oPhoneTypesDialog.DoModal() != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pPhoneTypesDocument->AddPhoneType(recPhoneType);

	if (!bIsSuccessful)
	{
		MessageBox(_T("Съжаляваме за неудобството, но възникна грешка. \n\
Възможна причина: проблем с връзката към базата данни (пр.: липса на достъп до Интернет). \n\
Ако смятате, че проблемът не е от вас или е отстранен, може да опитате отново."), _T("Информация"), MB_ICONINFORMATION | MB_OK);
	}
}

void CPhoneTypesView::OnContextMenuBtnUpdate()
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PHONE_TYPES recPhoneType = *reinterpret_cast<PHONE_TYPES*>(oListCtrl.GetItemData(nIndex));

	CPhoneTypesDialog oPhoneTypesUpdateDialog(recPhoneType, OperationsUpdate);

	if (oPhoneTypesUpdateDialog.DoModal() != IDOK)
	{
		return;
	}

	const BOOL bIsSuccessful = pPhoneTypesDocument->EditPhoneType(recPhoneType);

	if (!bIsSuccessful)
	{
		MessageBox(_T("Възникна грешка. \n\
Възможна причина: записът вече е обновен преди настъпване на настоящите промени. \n\
Моля обновете вашите данни и опитайте да направите редакциите отново. \n\
Ако сте сигурни, че това не е проблемът, може да опитате отново."),
_T("Информация"),
MB_ICONINFORMATION | MB_OK);
	}
}

void CPhoneTypesView::OnContextMenuBtnRefresh()
{
	// Ъпдейт при ползване от повече от една инстанция на приложението
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();
	pPhoneTypesDocument->RefreshData();

	ClearRowsData();
	LoadRowsData();
}
#pragma endregion ContextMenuOptions

void CPhoneTypesView::OnContextMenu(CWnd* pWnd /*= nullptr*/, CPoint oMousePos)
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
	const int nSelectedOption = static_cast<int>(pPopup->TrackPopupMenu(
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

void CPhoneTypesView::LoadRowsData()
{
	CListCtrl& oListCtrl = GetListCtrl();

	// Документ - за работа с данните
	CPhoneTypesDocument* pPhoneTypesDocument = GetDocument();

	// Данни - PHONE_TYPES
	CPhoneTypesArray& oPhoneTypesArray = pPhoneTypesDocument->GetAllPhoneTypes();

	// Зареждане на всички градове в списък в лист контролата
	for (INT_PTR i = 0; i < oPhoneTypesArray.GetCount(); i++)
	{
		PHONE_TYPES* pPhoneType = oPhoneTypesArray.GetAt(i);

		// Вмъкване на редове
		const INT nInsertIndex = oListCtrl.InsertItem(i, pPhoneType->szType);
		oListCtrl.SetItemData(nInsertIndex, reinterpret_cast<DWORD_PTR>(pPhoneType));
	}
}

INT CPhoneTypesView::GetPhoneTypeIndexInListCtrlByItemData(const DWORD_PTR dwData) const
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
void CPhoneTypesView::UpdateOnOperationCreate(const DWORD_PTR dwPhoneTypeItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	PHONE_TYPES* pPhoneType = reinterpret_cast<PHONE_TYPES*>(dwPhoneTypeItemData);

	// Вмъкване на редове
	const INT nInsertIndex = oListCtrl.InsertItem(oListCtrl.GetItemCount(), pPhoneType->szType);
	oListCtrl.SetItemData(nInsertIndex, dwPhoneTypeItemData);
}

void CPhoneTypesView::UpdateOnOperationUpdate(const DWORD_PTR dwPhoneTypeItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	PHONE_TYPES* pPhoneType = reinterpret_cast<PHONE_TYPES*>(dwPhoneTypeItemData);

	pPhoneType = reinterpret_cast<PHONE_TYPES*>(dwPhoneTypeItemData);
	const INT nIndexInListCtrl = GetPhoneTypeIndexInListCtrlByItemData(dwPhoneTypeItemData);

	oListCtrl.SetItemText(nIndexInListCtrl, PHONE_TYPE_NAME_COLUMN, pPhoneType->szType);
}

void CPhoneTypesView::UpdateOnOperationDelete(const DWORD_PTR dwPhoneTypeItemData)
{
	const INT nIndexInListCtrl = GetPhoneTypeIndexInListCtrlByItemData(dwPhoneTypeItemData);
	GetListCtrl().DeleteItem(nIndexInListCtrl);
}
#pragma endregion OnUpdateCases

void CPhoneTypesView::ClearRowsData()
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


// CPhoneTypesView diagnostics
#ifdef _DEBUG
void CPhoneTypesView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CPhoneTypesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

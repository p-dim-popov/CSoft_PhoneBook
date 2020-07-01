// PersonsView.cpp : implementation file
//

#include "pch.h"
#include "PersonsView.h"
#include "PersonsDialog.h"
#include "resource.h"

#define PERSON_FIRST_NAME_COLUMN 0
#define PERSON_MIDDLE_NAME_COLUMN 1
#define PERSON_LAST_NAME_COLUMN 2
#define PERSON_FIRST_NAME_COLUMN_LABEL _T("First name")
#define PERSON_MIDDLE_NAME_COLUMN_LABEL _T("Middle name")
#define PERSON_LAST_NAME_COLUMN_LABEL _T("Last name")
#define PERSON_FIRST_NAME_COLUMN_WIDTH 150
#define PERSON_MIDDLE_NAME_COLUMN_WIDTH 150
#define PERSON_LAST_NAME_COLUMN_WIDTH 150

#define GENERAL_ERROR_MESSAGE _T("Sorry for the inconvenience but error ocurred! Try refreshing the list.")

/////////////////////////////////////////////////////////////////////////////
// CPersonsView

IMPLEMENT_DYNCREATE(CPersonsView, CListView)
// Връзване на флаг съобщения с методи
BEGIN_MESSAGE_MAP(CPersonsView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CPersonsView::OnLvnItemActivate)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// Overrides
// ----------------

BOOL CPersonsView::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CPersonsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& oListCtrl = GetListCtrl();

	// Вмъкване на колони
	oListCtrl.InsertColumn(PERSON_FIRST_NAME_COLUMN, PERSON_FIRST_NAME_COLUMN_LABEL);
	oListCtrl.InsertColumn(PERSON_MIDDLE_NAME_COLUMN, PERSON_MIDDLE_NAME_COLUMN_LABEL);
	oListCtrl.InsertColumn(PERSON_LAST_NAME_COLUMN, PERSON_LAST_NAME_COLUMN_LABEL);

	// Задаване на ширина на колоните
	oListCtrl.SetColumnWidth(PERSON_FIRST_NAME_COLUMN, PERSON_FIRST_NAME_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(PERSON_MIDDLE_NAME_COLUMN, PERSON_MIDDLE_NAME_COLUMN_WIDTH);
	oListCtrl.SetColumnWidth(PERSON_LAST_NAME_COLUMN, PERSON_LAST_NAME_COLUMN_WIDTH);

	// Допълнителна стилизация на контролата
	oListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	oListCtrl.ModifyStyle(NULL, LVS_SINGLESEL, 0);

	LoadRowsData();
}

void CPersonsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CView::OnUpdate(pSender, lHint, pHint);

	if (lHint && pHint)
	{
		const CPersonsDocument::CPersonsUpdateObject* oPersonsUpdateObject = reinterpret_cast<CPersonsDocument::CPersonsUpdateObject*>(pHint);
		const DWORD_PTR dwPersonItemData = oPersonsUpdateObject->GetData();

		switch (lHint)
		{
		case OperationsCreate:
			UpdateOnOperationCreate(dwPersonItemData);
			break;
		case OperationsUpdate:
			UpdateOnOperationUpdate(dwPersonItemData);
			break;
		case OperationsDelete:
			UpdateOnOperationDelete(dwPersonItemData);
			break;
		default:
			break;
		}
	}
}

// Methods
// ----------------

CPersonsDocument* CPersonsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersonsDocument)));
	return dynamic_cast<CPersonsDocument*>(m_pDocument);
}

// CPersonsView message handlers
// Функции за обработка на флаг/съобщения

void CPersonsView::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	CListCtrl& oListCtrl = GetListCtrl();
	CPersonsDocument* pPersonsDocument = GetDocument();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
		return;

	PERSONS* pPerson = reinterpret_cast<PERSONS*>(oListCtrl.GetItemData(nIndex));

	CPhoneNumbersArray oPersonPhoneNumbersArray(pPersonsDocument->GetAllPhonesForPersonByID(pPerson->lID));
	oPersonPhoneNumbersArray.EnableAutoDelete(FALSE);

	CITIES* pPersonCity = pPersonsDocument
		->GetAllCities()
		.FirstOrDefault(
			[](CITIES& recCity, void* lID) -> BOOL { return recCity.lID == *static_cast<long*>(lID); }, &pPerson->lCityId);

	CPhoneTypesArray& oPhoneTypesArray = pPersonsDocument->GetAllPhoneTypes();
	CCitiesArray& oCitiesArray = pPersonsDocument->GetAllCities();

	CPersonsDialog oPersonsDialog(OperationsRead, *pPerson, oPersonPhoneNumbersArray, *pPersonCity, oPhoneTypesArray, oCitiesArray);

	oPersonsDialog.DoModal();

	if (pResult)
		*pResult = 0;
}

// Функции за избор при контекстно меню
#pragma region ContextMenuOptions
void CPersonsView::OnContextMenuBtnDelete()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

	PERSONS* pPerson = reinterpret_cast<PERSONS*>(oListCtrl.GetItemData(nIndex));
	CString strAreYouSurePrompt;
	strAreYouSurePrompt.Format(_T("Are you sure that you want to delete %s and all related contacts?"), pPerson->szFirstName);

	const int nResult = MessageBox(strAreYouSurePrompt, _T("Are you sure?"), MB_ICONINFORMATION | MB_OKCANCEL);

	if (nResult != IDOK)
	{
		return;
	}

	const bool bIsSuccessful = pPersonsDocument->DeletePerson(*pPerson);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE, _T("Информация"), MB_ICONINFORMATION | MB_OK);
		OnContextMenuBtnRefresh();
	}
}

void CPersonsView::OnContextMenuBtnInsert()
{
	PERSONS recNewPerson;
	CITIES recNewPersonCity;
	CPhoneNumbersArray oNewPersonPhoneNumbers;

	CPersonsDocument* pPersonsDocument = GetDocument();

	CPhoneTypesArray oPhoneTypesArray(pPersonsDocument->GetAllPhoneTypes());
	oPhoneTypesArray.EnableAutoDelete(FALSE);

	CCitiesArray oCitiesArray(pPersonsDocument->GetAllCities());
	oCitiesArray.EnableAutoDelete(FALSE);

	CPhoneTypesArray oNewPhoneTypesArray;

	CPersonsDialog oPersonsDialog(
		OperationsCreate,

		recNewPerson,
		oNewPersonPhoneNumbers,
		recNewPersonCity,

		oPhoneTypesArray,
		oCitiesArray,

		&oNewPhoneTypesArray
	);

	if (oPersonsDialog.DoModal() != IDOK)
		return;

	CPhoneTypesArray* pNewPhoneTypesArray = oPersonsDialog.AreNewPhoneTypesAdded()
		? &oNewPhoneTypesArray
		: NULL;

	CITIES* pNewCity = oPersonsDialog.IsNewCityAdded()
		? &recNewPersonCity
		: NULL;

	const BOOL bIsSuccessful = pPersonsDocument->AddPerson(
		recNewPerson,
		oNewPersonPhoneNumbers,
		pNewPhoneTypesArray,
		pNewCity
	);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE, _T("Information"), MB_ICONINFORMATION | MB_OK);
		OnContextMenuBtnRefresh();
	}
}

void CPersonsView::OnContextMenuBtnUpdate()
{
	CPersonsDocument* pPersonsDocument = GetDocument();
	CListCtrl& oListCtrl = GetListCtrl();
	const int nIndex = oListCtrl.GetSelectionMark();

	if (nIndex == -1)
	{
		return;
	}

#pragma region CurrentPersonData
	PERSONS* pOldPerson = reinterpret_cast<PERSONS*>(oListCtrl.GetItemData(nIndex));
	CPhoneNumbersArray oOldPersonPhoneNumbers = pPersonsDocument
		->GetAllPhonesForPersonByID(pOldPerson->lID);
	CITIES& recOldPersonCity = pPersonsDocument
		->GetAllCities()
		.First(
			[](CITIES& recCity, void* pID)
			-> BOOL { return recCity.lID == *static_cast<long*>(pID); },
			&pOldPerson->lCityId
		);
#pragma endregion

#pragma region ToBeUpdatedPersonData
	PERSONS recUpdatedPerson = *pOldPerson;
	CPhoneNumbersArray oUpdatedPersonPhoneNumbers(oOldPersonPhoneNumbers);
	CITIES recToBeUpdatedPersonCity = recOldPersonCity;
#pragma endregion 

	CPhoneTypesArray oPhoneTypesArray(pPersonsDocument->GetAllPhoneTypes());
	oPhoneTypesArray.EnableAutoDelete(FALSE);
	CCitiesArray oCitiesArray(pPersonsDocument->GetAllCities());
	oCitiesArray.EnableAutoDelete(FALSE);

	CPhoneTypesArray oNewPhoneTypesArray(FALSE);

	CPersonsDialog oPersonsDialog(
		OperationsCreate,

		recUpdatedPerson,
		oUpdatedPersonPhoneNumbers,
		recToBeUpdatedPersonCity,

		oPhoneTypesArray,
		oCitiesArray,

		&oNewPhoneTypesArray
	);

	if (oPersonsDialog.DoModal() != IDOK)
		return;

	CPhoneTypesArray* pNewPhoneTypesArray = oPersonsDialog.AreNewPhoneTypesAdded()
		? &oNewPhoneTypesArray
		: NULL;

	CITIES* pNewCity = oPersonsDialog.IsNewCityAdded()
		? &recToBeUpdatedPersonCity
		: NULL;

	BOOL bAreUpdatesMade =
		!pOldPerson->Compare(recUpdatedPerson) ||
		!recOldPersonCity.Compare(recToBeUpdatedPersonCity) ||
		oOldPersonPhoneNumbers.GetSize() != oUpdatedPersonPhoneNumbers.GetSize();

	CPhoneNumbersArray oDeletedPhoneNumbersArray(
		oOldPersonPhoneNumbers
		.Where( // phone numbers does not exist in
			[](PHONE_NUMBERS& recOldPhoneNumber, void* pUpdatedPhoneNumbersArray)
			-> BOOL { return static_cast<CPhoneNumbersArray*>(pUpdatedPhoneNumbersArray)->IndexOf(
				[](PHONE_NUMBERS& recNewPhoneNumber, void* pPhoneNumberID)
				-> BOOL { return recNewPhoneNumber.lID == *static_cast<long*>(pPhoneNumberID); },
				&recOldPhoneNumber.lID
			) == -1; },
			&oUpdatedPersonPhoneNumbers
		)
	);

	CPhoneNumbersArray oUpdatedPhoneNumbersArray(FALSE);
	CPhoneNumbersArray oNewPhoneNumbersArray(FALSE);
	for (INT_PTR i = 0; i < oUpdatedPersonPhoneNumbers.GetSize(); i++)
	{
		PHONE_NUMBERS* pNewPhoneNumber = oUpdatedPersonPhoneNumbers.GetAt(i);
		PHONE_NUMBERS* pOldPhoneNumber = oOldPersonPhoneNumbers
			.FirstOrDefault(
				[](PHONE_NUMBERS& recOldPhoneNumber, void* pID)
				-> BOOL { return recOldPhoneNumber.lID == *static_cast<long*>(pID);},
				& oUpdatedPersonPhoneNumbers.GetAt(i)->lID
			);

		if (!pOldPhoneNumber) // it is new =>
		{
			oNewPhoneNumbersArray.Add(pNewPhoneNumber);
			continue;
		}

		if (!pOldPhoneNumber->Compare(*pNewPhoneNumber)) // no change =>
		{
			continue;
		}

		oUpdatedPhoneNumbersArray.Add(pNewPhoneNumber);
	}

	bAreUpdatesMade = oDeletedPhoneNumbersArray.GetSize() || oUpdatedPersonPhoneNumbers.GetSize() || oNewPhoneNumbersArray.GetSize()
		? TRUE
		: bAreUpdatesMade;

	if (!bAreUpdatesMade)
	{
		return;
	}
	
	const BOOL bIsSuccessful = pPersonsDocument->EditPerson(
		recUpdatedPerson,
		oDeletedPhoneNumbersArray,
		oUpdatedPhoneNumbersArray,
		oNewPhoneNumbersArray,
		pNewPhoneTypesArray,
		pNewCity
	);

	if (!bIsSuccessful)
	{
		MessageBox(GENERAL_ERROR_MESSAGE, _T("Information"), MB_ICONINFORMATION | MB_OK);
		OnContextMenuBtnRefresh();
	}
}

void CPersonsView::OnContextMenuBtnRefresh()
{
	// Ъпдейт при ползване от повече от една инстанция на приложението
	CPersonsDocument* pPersonsDocument = GetDocument();
	pPersonsDocument->RefreshData(FALSE);

	ClearRowsData();
	LoadRowsData();
}
#pragma endregion

void CPersonsView::OnContextMenu(CWnd* pWnd, CPoint oMousePos)
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

void CPersonsView::LoadRowsData()
{
	CListCtrl& oListCtrl = GetListCtrl();

	// Документ - за работа с данните
	CPersonsDocument* pPersonsDocument = GetDocument();

	// Данни - PERSONS
	CPersonsArray& oPersonsArray = pPersonsDocument->GetAllPersons();

	// Зареждане на всички данни в списък в лист контролата
	for (INT_PTR i = 0; i < oPersonsArray.GetCount(); i++)
	{
		PERSONS* pPerson = oPersonsArray.GetAt(i);

		// Вмъкване на редове
		const INT nInsertIndex = oListCtrl.InsertItem(i, pPerson->szFirstName);
		oListCtrl.SetItemText(nInsertIndex, PERSON_MIDDLE_NAME_COLUMN, pPerson->szMiddleName);
		oListCtrl.SetItemText(nInsertIndex, PERSON_LAST_NAME_COLUMN, pPerson->szLastName);
		oListCtrl.SetItemData(nInsertIndex, reinterpret_cast<DWORD_PTR>(pPerson));
	}
}

INT CPersonsView::GetPersonIndexInListCtrlByItemData(const DWORD_PTR dwData) const
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
void CPersonsView::UpdateOnOperationCreate(const DWORD_PTR dwPersonItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	PERSONS* pPerson = reinterpret_cast<PERSONS*>(dwPersonItemData);

	const INT nInsertIndex = oListCtrl.InsertItem(oListCtrl.GetItemCount(), pPerson->szFirstName);
	oListCtrl.SetItemText(nInsertIndex, PERSON_MIDDLE_NAME_COLUMN, pPerson->szMiddleName);
	oListCtrl.SetItemText(nInsertIndex, PERSON_LAST_NAME_COLUMN, pPerson->szLastName);
	oListCtrl.SetItemData(nInsertIndex, dwPersonItemData);

}

void CPersonsView::UpdateOnOperationUpdate(const DWORD_PTR dwPersonItemData)
{
	CListCtrl& oListCtrl = GetListCtrl();
	PERSONS* pPerson = reinterpret_cast<PERSONS*>(dwPersonItemData);

	const INT nIndexInListCtrl = GetPersonIndexInListCtrlByItemData(dwPersonItemData);

	oListCtrl.SetItemText(nIndexInListCtrl, PERSON_FIRST_NAME_COLUMN, pPerson->szFirstName);
	oListCtrl.SetItemText(nIndexInListCtrl, PERSON_MIDDLE_NAME_COLUMN, pPerson->szMiddleName);
	oListCtrl.SetItemText(nIndexInListCtrl, PERSON_LAST_NAME_COLUMN, pPerson->szLastName);
}

void CPersonsView::UpdateOnOperationDelete(const DWORD_PTR dwPersonItemData)
{
	const INT nIndexInListCtrl = GetPersonIndexInListCtrlByItemData(dwPersonItemData);
	GetListCtrl().DeleteItem(nIndexInListCtrl);
}
#pragma endregion

void CPersonsView::ClearRowsData()
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


// CPersonsView diagnostics
#ifdef _DEBUG
void CPersonsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CPersonsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

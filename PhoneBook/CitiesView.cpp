// CitiesView.cpp : implementation file
//

#include "pch.h"
#include "CitiesView.h"

#include "CitiesDialog.h"

#define CITY_ID_COLUMN 0
#define CITY_NAME_COLUMN 1
#define CITY_REGION_COLUMN 2
#define CITY_ID_COLUMN_LABEL _T("ID")
#define CITY_NAME_COLUMN_LABEL _T("Име")
#define CITY_REGION_COLUMN_LABEL _T("Регион")
#define CITY_ID_COLUMN_WIDTH 50
#define CITY_NAME_COLUMN_WIDTH 150
#define CITY_REGION_COLUMN_WIDTH 150

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

CCitiesView::CCitiesView()
{

}

CCitiesView::~CCitiesView()
{
}

CCitiesDocument* CCitiesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDocument)));
	return static_cast<CCitiesDocument*>(m_pDocument);
}

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& оListCtrl = GetListCtrl();

	// Insert columns
	оListCtrl.InsertColumn(CITY_NAME_COLUMN, CITY_NAME_COLUMN_LABEL);
	оListCtrl.InsertColumn(CITY_REGION_COLUMN, CITY_REGION_COLUMN_LABEL);
	оListCtrl.InsertColumn(CITY_ID_COLUMN, CITY_ID_COLUMN_LABEL);

	// Set column widths
	оListCtrl.SetColumnWidth(CITY_NAME_COLUMN, CITY_NAME_COLUMN_WIDTH);
	оListCtrl.SetColumnWidth(CITY_REGION_COLUMN, CITY_REGION_COLUMN_WIDTH);
	оListCtrl.SetColumnWidth(CITY_ID_COLUMN, CITY_ID_COLUMN_WIDTH);

	CCitiesDocument* pCitiesDocument = GetDocument();
	CCitiesArray* pCitiesArray = pCitiesDocument->GetAllCities();

	оListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	for (INT_PTR i = 0; i < pCitiesArray->GetCount(); i++)
	{
		CITIES* pCity = pCitiesArray->GetAt(i);
		
		// Insert row
		CString strId;
		strId.Format(_T("%d"), pCity->lID);
		
		const INT nInsertIndex = оListCtrl.InsertItem(i, strId);
		оListCtrl.SetItemText(nInsertIndex, CITY_NAME_COLUMN, pCity->szName);
		оListCtrl.SetItemText(nInsertIndex, CITY_REGION_COLUMN, pCity->szRegion);
	}
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
}

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs) {
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, &CCitiesView::OnLvnItemActivate)
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


// CCitiesView message handlers


void CCitiesView::OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult)
{
	const LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CListCtrl& oListCtrl = GetListCtrl();
	const DWORD dwPos = ::GetMessagePos();
	CPoint point(int(LOWORD(dwPos)), int(HIWORD(dwPos)));
	CITIES recCities;

	oListCtrl.ScreenToClient(&point);

	int nIndex = reinterpret_cast<NM_LISTVIEW*>(pNMHDR)->iItem;
	
	if ((nIndex = oListCtrl.HitTest(point)) != -1)
	{
		_tcscpy_s(recCities.szName, oListCtrl.GetItemText(nIndex, CITY_NAME_COLUMN));
		_tcscpy_s(recCities.szRegion, oListCtrl.GetItemText(nIndex, CITY_REGION_COLUMN));
		recCities.lID = int(oListCtrl.GetItemText(nIndex, CITY_ID_COLUMN).GetString());
	}
	
	CCitiesDialog oCitiesDialog(recCities, this);

	oCitiesDialog.DoModal();
	
	*pResult = 0;
}

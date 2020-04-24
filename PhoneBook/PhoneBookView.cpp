
// PhoneBookView.cpp : implementation of the CPhoneBookView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

#include "PhoneBookDoc.h"
#include "PhoneBookView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhoneBookView

IMPLEMENT_DYNCREATE(CPhoneBookView, CView)

BEGIN_MESSAGE_MAP(CPhoneBookView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPhoneBookView construction/destruction

CPhoneBookView::CPhoneBookView() noexcept
{
	// TODO: add construction code here

}

CPhoneBookView::~CPhoneBookView()
{
}

BOOL CPhoneBookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPhoneBookView drawing

void CPhoneBookView::OnDraw(CDC* /*pDC*/)
{
	CPhoneBookDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPhoneBookView printing

BOOL CPhoneBookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPhoneBookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPhoneBookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPhoneBookView diagnostics

#ifdef _DEBUG
void CPhoneBookView::AssertValid() const
{
	CView::AssertValid();
}

void CPhoneBookView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhoneBookDoc* CPhoneBookView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneBookDoc)));
	return (CPhoneBookDoc*)m_pDocument;
}
#endif //_DEBUG


// CPhoneBookView message handlers

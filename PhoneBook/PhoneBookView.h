
// PhoneBookView.h : interface of the CPhoneBookView class
//

#pragma once


class CPhoneBookView : public CView
{
protected: // create from serialization only
	CPhoneBookView() noexcept;
	DECLARE_DYNCREATE(CPhoneBookView)

// Attributes
public:
	CPhoneBookDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPhoneBookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PhoneBookView.cpp
inline CPhoneBookDoc* CPhoneBookView::GetDocument() const
   { return reinterpret_cast<CPhoneBookDoc*>(m_pDocument); }
#endif


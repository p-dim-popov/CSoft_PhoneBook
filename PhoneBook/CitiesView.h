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
//	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnInitialUpdate() override;
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif

//protected:
//	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
//	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
//	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	
// Methods
// ----------------

public:
	CCitiesDocument* GetDocument() const;
	
// Members
// ----------------

private:
	
public:
	afx_msg void OnLvnItemActivate(NMHDR* pNMHDR, LRESULT* pResult);
};



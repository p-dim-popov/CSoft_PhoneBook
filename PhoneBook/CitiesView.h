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
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnInitialUpdate() override;
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	
#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif
	
// Methods
// ----------------
private:
	afx_msg void OnLvnItemActivate(NMHDR* pNMHDR = nullptr, LRESULT* pResult = nullptr);
	void OnSelectDeleteInContextMenu();
	void OnSelectInsertInContextMenu();
	void OnSelectUpdateInContextMenu();
	void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	CCitiesDocument* GetDocument() const;
	
// Members
// ----------------
private:

};



#pragma once

#include "Structures.h"

////////////////////////////////////////////////////////////
// CCitiesDialog

class CCitiesDialog : public CDialog
{
	DECLARE_DYNAMIC(CCitiesDialog)

public:
	CCitiesDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);   // constructor
	virtual ~CCitiesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
// Members
// -------------
protected:
	CITIES* m_pCity;
	
	CEdit m_oEdbName;
	CEdit m_oEdbRegion;
};

////////////////////////////////////////////////////////////
// CCitiesUpdateDialog

class CCitiesUpdateDialog : public CCitiesDialog
{
public:
	CCitiesUpdateDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesUpdateDialog();

// Methods
// -------------
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

////////////////////////////////////////////////////////////
// CCitiesInsertDialog

class CCitiesInsertDialog : public CCitiesDialog
{
public:
	CCitiesInsertDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesInsertDialog();

	// Methods
	// -------------
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

////////////////////////////////////////////////////////////
// CCitiesDeleteDialog

class CCitiesDeleteDialog : public CCitiesDialog
{
public:
	CCitiesDeleteDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesDeleteDialog();

 //Methods
 //-------------
public:
	virtual BOOL OnInitDialog();
};

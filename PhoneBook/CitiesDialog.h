#pragma once

#include "Structures.h"

////////////////////////////////////////////////////////////
// CCitiesDialog

/// <summary>Клас диалог за CITIES</summary>
class CCitiesDialog : public CDialog
{
	// Macros
	// ----------------

	DECLARE_DYNAMIC(CCitiesDialog)
	DECLARE_MESSAGE_MAP()

	// Constructor / Destructor
	// ----------------
public:
	CCitiesDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);   // constructor
	virtual ~CCitiesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES_DIALOG };
#endif

	// MFC Overrides
	// ----------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	// Members
	// -------------
protected:
	CITIES* m_pCity;
	
	CEdit m_oEdbName;
	CEdit m_oEdbRegion;
};

////////////////////////////////////////////////////////////
// CCitiesUpdateDialog

/// <summary>Клас диалог за ъпдейт на град</summary>
class CCitiesUpdateDialog : public CCitiesDialog
{
	// Constructor / Destructor
	// ----------------

public:
	CCitiesUpdateDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesUpdateDialog();

	// Overrides
	// ----------------
public:
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
};

////////////////////////////////////////////////////////////
// CCitiesInsertDialog

/// <summary>Клас диалог за добавяне на нов град</summary>
class CCitiesInsertDialog : public CCitiesDialog
{
	// Constructor / Destructor
	// ----------------
public:
	CCitiesInsertDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesInsertDialog();

	// Overrides
	// ----------------
public:
	virtual BOOL OnInitDialog() override;
	virtual void OnOK() override;
};

////////////////////////////////////////////////////////////
// CCitiesDeleteDialog

/// <summary>Клас диалог за изтриване на град</summary>
class CCitiesDeleteDialog : public CCitiesDialog
{
	// Constructor / Destructor
	// ----------------
public:
	CCitiesDeleteDialog(CWnd* pParent = nullptr, CITIES* pCity = nullptr);
	virtual ~CCitiesDeleteDialog();

	// Overrides
	// ----------------
public:
	virtual BOOL OnInitDialog() override;
};

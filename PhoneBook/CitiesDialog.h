#pragma once

#include "Structures.h"

////////////////////////////////////////////////////////////
// CCitiesDialog


class CCitiesDialog : public CDialog
{
	DECLARE_DYNAMIC(CCitiesDialog)

public:
	CCitiesDialog(CWnd* pParent = nullptr);   // standard constructor
	CCitiesDialog(CITIES oCity, CWnd* pParent = nullptr);   // constructor
	virtual ~CCitiesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Members
// -------------
private:
	CString m_strName;
	CString m_strRegion;
	long m_lId;
	
	CEdit m_oEdbName;
	CEdit m_oEdbRegion;
	//CListCtrl m_oLscCities;
};

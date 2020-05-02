// CitiesDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "CitiesDialog.h"
#include "afxdialogex.h"

/////////////////////////////////////////////////////////////////////
// CCitiesDialog
#pragma region CCitiesDialog

IMPLEMENT_DYNAMIC(CCitiesDialog, CDialog)

CCitiesDialog::CCitiesDialog(CWnd* pParent /*=nullptr*/, CITIES* pCity /*=nullptr*/)
	: CDialog(IDD_CITIES_DIALOG, pParent), m_pCity(pCity)
{
}

CCitiesDialog::~CCitiesDialog()
{
}

void CCitiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_oEdbName);
	DDX_Control(pDX, IDC_EDB_REGION, m_oEdbRegion);
}

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
END_MESSAGE_MAP()

#pragma endregion CCitiesDialog

/////////////////////////////////////////////////////////////////////
// CCitiesUpdateDialog
#pragma region CCitiesUpdateDialog

CCitiesUpdateDialog::CCitiesUpdateDialog(CWnd* pParent /*=nullptr*/, CITIES* pCity /*=nullptr*/)
	: CCitiesDialog(pParent, pCity)
{
}

CCitiesUpdateDialog::~CCitiesUpdateDialog()
{
}

// CCitiesDialog message handlers

BOOL CCitiesUpdateDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);

	m_oEdbName.SetLimitText(CITIES_NAME_LENGTH);
	m_oEdbRegion.SetLimitText(CITIES_REGION_LENGTH);
	
	return TRUE;
}


void CCitiesUpdateDialog::OnOK()
{
	CString strUpdatedName;
	CString strUpdatedRegion;

	m_oEdbName.GetWindowText(strUpdatedName);
	m_oEdbRegion.GetWindowText(strUpdatedRegion);

	if (strUpdatedName.IsEmpty() || strUpdatedRegion.IsEmpty())
	{
		MessageBox(_T("Не се допускат празни полета!"), _T("Внимание"), MB_ICONEXCLAMATION);
		return;
	}

	if (strUpdatedName.Compare(m_pCity->szName) == 0 &&
		strUpdatedRegion.Compare(m_pCity->szRegion) == 0)
	{
		// няма промени
		return;
	}

	m_pCity->SetName(strUpdatedName);
	m_pCity->SetRegion(strUpdatedRegion);

	CDialog::OnOK();
}

#pragma endregion CCitiesUpdateDialog

/////////////////////////////////////////////////////////////////////
// CCitiesInsertDialog
#pragma region CCitiesInsertDialog

CCitiesInsertDialog::CCitiesInsertDialog(CWnd* pParent /*=nullptr*/, CITIES* pCity /*=nullptr*/)
	: CCitiesDialog(pParent, pCity)
{
}

CCitiesInsertDialog::~CCitiesInsertDialog()
{
}

// CCitiesDialog message handlers

BOOL CCitiesInsertDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);

	m_oEdbName.SetLimitText(CITIES_NAME_LENGTH);
	m_oEdbRegion.SetLimitText(CITIES_REGION_LENGTH);
	
	return TRUE;
}


void CCitiesInsertDialog::OnOK()
{
	CString strUpdatedName;
	CString strUpdatedRegion;

	m_oEdbName.GetWindowText(strUpdatedName);
	m_oEdbRegion.GetWindowText(strUpdatedRegion);

	//TODO: validate with regex
	if (strUpdatedName.IsEmpty() || strUpdatedRegion.IsEmpty())
	{
		MessageBox(_T("Не се допускат празни полета!"), _T("Внимание"), MB_ICONEXCLAMATION);
		return;
	}
	
	*m_pCity = CITIES();
	m_pCity->SetName(strUpdatedName);
	m_pCity->SetRegion(strUpdatedRegion);

	CDialog::OnOK();
}

#pragma endregion CCitiesInsertDialog

/////////////////////////////////////////////////////////////////////
// CCitiesDeleteDialog
#pragma region CCitiesDeleteDialog

CCitiesDeleteDialog::CCitiesDeleteDialog(CWnd* pParent /*=nullptr*/, CITIES* pCity /*=nullptr*/)
	: CCitiesDialog(pParent, pCity)
{
}

CCitiesDeleteDialog::~CCitiesDeleteDialog()
{
}

// CCitiesDialog message handlers

BOOL CCitiesDeleteDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oEdbName.EnableWindow(FALSE);
	m_oEdbRegion.EnableWindow(FALSE);
	
	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);
	
	return TRUE;
}

#pragma endregion CCitiesDeleteDialog
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
	DDX_Control(pDX, IDC_EDB_CITIES_REGION, m_oEdbRegion);
	DDX_Control(pDX, IDC_BTN_CITIES_OK, m_oBtnOk);
	DDX_Control(pDX, IDC_BTN_CITIES_CANCEL, m_oBtnCancel);
}

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
	ON_BN_CLICKED(IDC_BTN_CITIES_OK, &CCitiesDialog::OnBnClickedBtnCitiesOk)
	ON_BN_CLICKED(IDC_BTN_CITIES_CANCEL, &CCitiesDialog::OnBnClickedBtnCitiesCancel)
END_MESSAGE_MAP()

void CCitiesDialog::OnBnClickedBtnCitiesCancel()
{
	CDialog::OnCancel();
}

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

// CCitiesUpdateDialog message handlers

BOOL CCitiesUpdateDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText(_T("Update city"));
	
	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);

	m_oEdbName.SetLimitText(CITIES_NAME_LENGTH);
	m_oEdbRegion.SetLimitText(CITIES_REGION_LENGTH);
	
	return TRUE;
}


void CCitiesUpdateDialog::OnBnClickedBtnCitiesOk()
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

// CCitiesInsertDialog message handlers

BOOL CCitiesInsertDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText(_T("Add city"));

	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);

	m_oEdbName.SetLimitText(CITIES_NAME_LENGTH);
	m_oEdbRegion.SetLimitText(CITIES_REGION_LENGTH);
	
	return TRUE;
}


void CCitiesInsertDialog::OnBnClickedBtnCitiesOk()
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

// CCitiesDeleteDialog message handlers

BOOL CCitiesDeleteDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText(_T("Delete city?"));
	
	m_oEdbName.EnableWindow(FALSE);
	m_oEdbRegion.EnableWindow(FALSE);
	
	m_oEdbName.SetWindowText(m_pCity->szName);
	m_oEdbRegion.SetWindowText(m_pCity->szRegion);

	m_oBtnOk.SetWindowText(_T("Delete"));
	
	return TRUE;
}

void CCitiesDeleteDialog::OnBnClickedBtnCitiesOk()
{
	CDialog::OnOK();
}

#pragma endregion CCitiesDeleteDialog


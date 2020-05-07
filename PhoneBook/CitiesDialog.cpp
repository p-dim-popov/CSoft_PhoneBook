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

CCitiesDialog::CCitiesDialog(CITIES& recCity, CCitiesDocument::Operations eOperation)
	: CDialog(IDD_CITIES_DIALOG), m_recCity(recCity), m_eOperation(eOperation)
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

BOOL CCitiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oEdbName.SetWindowText(m_recCity.szName);
	m_oEdbRegion.SetWindowText(m_recCity.szRegion);
	
	m_oEdbName.SetLimitText(CITIES_NAME_LENGTH);
	m_oEdbRegion.SetLimitText(CITIES_REGION_LENGTH);

	switch (m_eOperation)
	{
	case CCitiesDocument::Operations::OperationsCreate:
		this->SetWindowText(_T("Add city"));
		
		break;
	case CCitiesDocument::Operations::OperationsUpdate:
		this->SetWindowText(_T("Update city"));

		break;
	case CCitiesDocument::Operations::OperationsDelete:
		this->SetWindowText(_T("Delete city?"));

		m_oEdbName.EnableWindow(FALSE);
		m_oEdbRegion.EnableWindow(FALSE);

		m_oBtnOk.SetWindowText(_T("Delete"));

		break;
	default:
		return FALSE;
	}

	return TRUE;
}


void CCitiesDialog::OnBnClickedBtnCitiesCancel()
{
	CDialog::OnCancel();
}

void CCitiesDialog::OnBnClickedBtnCitiesOk()
{
	CString strNewName;
	CString strNewRegion;

	
	switch (m_eOperation)
	{
	case CCitiesDocument::Operations::OperationsCreate:
		m_oEdbName.GetWindowText(strNewName);
		m_oEdbRegion.GetWindowText(strNewRegion);

		//TODO: validate with regex
		if (strNewName.IsEmpty() || strNewRegion.IsEmpty())
		{
			MessageBox(_T("Не се допускат празни полета!"), _T("Внимание"), MB_ICONEXCLAMATION);
			return;
		}

		m_recCity = CITIES();
		m_recCity.SetName(strNewName);
		m_recCity.SetRegion(strNewRegion);

		break;
	case CCitiesDocument::Operations::OperationsUpdate:
		m_oEdbName.GetWindowText(strNewName);
		m_oEdbRegion.GetWindowText(strNewRegion);

		if (strNewName.IsEmpty() || strNewRegion.IsEmpty())
		{
			MessageBox(_T("Не се допускат празни полета!"), _T("Внимание"), MB_ICONEXCLAMATION);
			return;
		}

		if (strNewName.Compare(m_recCity.szName) == 0 &&
			strNewRegion.Compare(m_recCity.szRegion) == 0)
		{
			// няма промени
			return;
		}

		m_recCity.SetName(strNewName);
		m_recCity.SetRegion(strNewRegion);

		break;
	default:
		break;
	}

	CDialog::OnOK();
}
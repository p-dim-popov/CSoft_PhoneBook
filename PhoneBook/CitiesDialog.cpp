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

	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_edbName);
	DDX_Control(pDX, IDC_EDB_CITIES_REGION, m_edbRegion);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCitiesDialog::OnOK)
	ON_BN_CLICKED(IDCANCEL, &CCitiesDialog::OnCancel)
END_MESSAGE_MAP()

BOOL CCitiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbName.SetWindowText(m_recCity.szName);
	m_edbRegion.SetWindowText(m_recCity.szRegion);

	m_edbName.SetLimitText(CITIES_NAME_LENGTH);
	m_edbRegion.SetLimitText(CITIES_REGION_LENGTH);

	switch (m_eOperation)
	{
	case CCitiesDocument::Operations::OperationsCreate:
		this->SetWindowText(_T("Add city"));

		break;
	case CCitiesDocument::Operations::OperationsUpdate:
		this->SetWindowText(_T("Update city"));

		break;
	case CCitiesDocument::Operations::OperationsRead:
		this->SetWindowText(_T("City info"));

		m_edbName.EnableWindow(FALSE);
		m_edbRegion.EnableWindow(FALSE);

		break;
	default:
		return FALSE;
	}

	return TRUE;
}


void CCitiesDialog::OnCancel()
{
	CDialog::OnCancel();
}

void CCitiesDialog::OnOK()
{
	CString strNewName;
	CString strNewRegion;

	//TODO: Validations then write to record. Use focus on different edit boxes

	m_edbName.GetWindowText(strNewName);
	m_edbRegion.GetWindowText(strNewRegion);

	if (strNewName.IsEmpty() || strNewRegion.IsEmpty())
	{
		MessageBox(_T("Не се допускат празни полета!"), _T("Внимание"), MB_ICONEXCLAMATION);
		return;
	}

	m_recCity.SetName(strNewName);
	m_recCity.SetRegion(strNewRegion);

	CDialog::OnOK();
}
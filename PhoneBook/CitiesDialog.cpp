// CitiesDialog.cpp : implementation file
//

#include "pch.h"
#include "PhoneBook.h"
#include "CitiesDialog.h"
#include "afxdialogex.h"

// CCitiesDialog dialog

IMPLEMENT_DYNAMIC(CCitiesDialog, CDialog)

CCitiesDialog::CCitiesDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CITIES_DIALOG, pParent),
	m_strName(_T("")),
	m_strRegion(_T("")),
	m_lId(0)
{
}

CCitiesDialog::CCitiesDialog(CITIES oCity, CWnd* pParent)
	: CDialog(IDD_CITIES_DIALOG, pParent),
	m_strName(oCity.szName),
	m_strRegion(oCity.szRegion),
	m_lId(oCity.lID)
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
	//DDX_Control(pDX, IDC_LSC_CITIES, m_oLscCities);
}


BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
END_MESSAGE_MAP()


// CCitiesDialog message handlers


BOOL CCitiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oEdbName.SetWindowText(m_strName);
	m_oEdbRegion.SetWindowText(m_strRegion);

	return TRUE;
}


void CCitiesDialog::OnOK()
{
	CString strName;
	CString strRegion;

	m_oEdbName.GetWindowText(strName);
	m_oEdbRegion.GetWindowText(strRegion);

	if (strName.IsEmpty() || strRegion.IsEmpty())
	{
		//TODO: msg Cannot be empty!
		return;
	}

	CDialog::OnOK();
}

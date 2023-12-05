// delete_language_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "studiox.h"
#include "delete_language_dlg.h"
#include "afxdialogex.h"


// delete_language_dlg dialog

IMPLEMENT_DYNAMIC(delete_language_dlg, CDialogEx)

delete_language_dlg::delete_language_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(delete_language_dlg::IDD, pParent)
{

}

delete_language_dlg::~delete_language_dlg()
{
}

void delete_language_dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NAME_COMBO, mLangNameCombo);
}


BEGIN_MESSAGE_MAP(delete_language_dlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &delete_language_dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// delete_language_dlg message handlers


void delete_language_dlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}

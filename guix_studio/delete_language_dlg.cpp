/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/

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

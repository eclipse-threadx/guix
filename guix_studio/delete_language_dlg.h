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

#pragma once
#include "afxwin.h"


// delete_language_dlg dialog

class delete_language_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(delete_language_dlg)

public:
	delete_language_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~delete_language_dlg();

// Dialog Data
	enum { IDD = IDD_DELETE_LANGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CComboBox mLangNameCombo;
};

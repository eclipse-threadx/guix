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
#include <string>

class MacroRecordDlg : public express_dialog
{
public:
    MacroRecordDlg(CWnd *pParent = NULL);
    ~MacroRecordDlg(void);

public:
    CString &Getmacro_name(){ return mmacro_name;};
    CString &Getmacro_path(){ return mmacro_path;};

// Dialog Data
	enum { IDD = IDD_MACRO_RECORD_DLG};
    DECLARE_MESSAGE_MAP()

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnOK();

private:
    CString mmacro_name;
    CString mmacro_path;

public:
    afx_msg void OnBnClickedBrowsePath();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnCancel();
    virtual BOOL OnInitDialog();
};


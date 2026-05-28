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
#include "afxcmn.h"
#include "afxwin.h"


#ifndef _MESSAGE_DIALOG_
#define _MESSAGE_DIALOG_

class  message_dialog : public express_dialog
{
    DECLARE_DYNAMIC(message_dialog)
public:
    message_dialog(const char *caption, const char *msg, bool type, CWnd *pParent);
    virtual ~message_dialog();

    void AddCStaticControl(CString text = _T("This is a Static."));

    enum { IDD = IDD_MESSAGE_DLG};

protected:
    CStatic msg_text;
    CString mMessage;
    unsigned int m_type;
    BOOL    yesandno;

private:

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnTestMessage(WPARAM, LPARAM);
};

#endif
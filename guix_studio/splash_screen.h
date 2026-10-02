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
// Portions of this file were generated with AI assistance.


#ifndef _SPLASH_SCREEN_
#define _SPLASH_SCREEN_

#include <afxcview.h>

#define DATE_STRING_SIZE 128
#define SPLASH_SCREEN_WIDTH_96DPI 540
#define SPLASH_SCREEN_HEIGHT_96DPI 283

class splash_screen : public CDialog
{
    public:
        splash_screen(BOOL AutoClose);
        virtual BOOL PreCreateWindow(CREATESTRUCT &cs);

    // Generated message map functions
    protected:

        char       date_built[DATE_STRING_SIZE];

        CButton CloseButton;

        void ScreenReaderMessage();
	    afx_msg void OnPaint();
        afx_msg void OnTimer(UINT_PTR nIdEvent);
        afx_msg void OnDoneClicked();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    DECLARE_MESSAGE_MAP()
        BOOL mAutoClose;
        int m_dpi;
};

#endif

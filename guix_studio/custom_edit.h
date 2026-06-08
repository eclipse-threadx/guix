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

class custom_edit :public CEdit
{
public:
    custom_edit();
    ~custom_edit() {};
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    void EnableNegativeValue() { m_bNegativeValueAllowed = TRUE; }
    void EnableNumberCheck() { m_bNumberCheck = TRUE; };

protected:
    void ErrorMessage();
    BOOL OnPaste();
    BOOL OnChar(UINT key);
    BOOL m_bNegativeValueAllowed;
    BOOL m_bNumberCheck;
};

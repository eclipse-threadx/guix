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

#include "studiox_includes.h"
#include "express_combobox.h"

express_combobox::express_combobox()
{

}

express_combobox::~express_combobox()
{

}


BOOL express_combobox::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_SPACE:
        case VK_RETURN:
            if (!GetDroppedState())
            {
                // show drop list
                ShowDropDown(TRUE);
            }
            else
            {
                ShowDropDown(FALSE);
            }
            return TRUE;
        }
        
    }

    return CComboBox::PreTranslateMessage(pMsg);
}

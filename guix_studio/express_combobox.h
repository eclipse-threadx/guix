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

#ifndef _EXPRESS_COMBOBOX_
#define _EXPRESS_COMBOBOX_
class express_combobox : public CComboBox
{
public:
    express_combobox();
    ~express_combobox();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif

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

class express_table_row : public CWnd
{
public:
    express_table_row();
    ~express_table_row();
    CWnd *GetNextTabStopChild(CWnd *child, WPARAM keyvalue, int row_offset = 1);
    void AssignFocus(CWnd* child);

private:
    INT GetFocusCol(CWnd* focus_owner);
};


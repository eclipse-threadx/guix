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

#ifndef _COLOR_BOX_BUTTON_
#define _COLOR_BOX_BUTTON_

class color_box_button : public CButton
{
public:
    color_box_button();
    ~color_box_button();

    void SetBoxColor(COLORREF color);
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
    COLORREF m_boxcolor;
    BOOL  m_hover;
};
#endif


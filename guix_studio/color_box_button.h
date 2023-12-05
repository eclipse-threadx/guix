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


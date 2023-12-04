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


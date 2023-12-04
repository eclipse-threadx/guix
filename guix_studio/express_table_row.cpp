#include "stdafx.h"
#include "express_table_row.h"

express_table_row::express_table_row()
{

}
express_table_row::~express_table_row()
{

}

INT express_table_row::GetFocusCol(CWnd* focus_owner)
{
    int col = 0;
    CWnd* child;

    child = GetWindow(GW_CHILD);
    while (child)
    {
        if (child == focus_owner)
        {
            break;
        }

        col++;
        child = child->GetWindow(GW_HWNDNEXT);
    }

    return col;
}

CWnd* express_table_row::GetNextTabStopChild(CWnd *child, WPARAM keyvalue, int row_offset)
{
    UINT nCmd = 0;
    CWnd *parent = this;

    switch (keyvalue)
    {
    case VK_LEFT:
    case VK_RIGHT:
        if (keyvalue == VK_LEFT)
        {
            nCmd = GW_HWNDPREV;
        }
        else
        {
            nCmd = GW_HWNDNEXT;
        }

        while (child)
        {
            child = child->GetWindow(nCmd);

            if (child && (child->GetStyle() & WS_TABSTOP))
            {
                return child;
            }
        }
        break;

    case VK_UP:
    case VK_DOWN:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
        if ((keyvalue == VK_UP) || (keyvalue == VK_PRIOR))
        {
            nCmd = GW_HWNDPREV;
        }
        else if ((keyvalue == VK_DOWN) || (keyvalue == VK_NEXT))
        {
            nCmd = GW_HWNDNEXT;
        }
        else if (keyvalue == VK_HOME)
        {
            nCmd = GW_HWNDFIRST;
        }
        else if(keyvalue == VK_END)
        {
            nCmd = GW_HWNDLAST;
        }

        while (parent && row_offset)
        {
            parent = parent->GetWindow(nCmd);
            row_offset--;
        }

        if (parent)
        {
            //determine which child control should gain focus
            int col = GetFocusCol(child);

            child = parent->GetWindow(GW_CHILD);

            while (col && child)
            {
                child = child->GetWindow(GW_HWNDNEXT);
                col--;
            }

            return child;
        }
        break;
    }

    return NULL;
}

void express_table_row::AssignFocus(CWnd* child)
{
    if (!child)
    {
        return;
    }

    child->SetFocus();

    TCHAR  class_name[MAX_PATH];
    GetClassName(child->GetSafeHwnd(), class_name, MAX_PATH - 1);
    if (class_name[0] == 'E'||
        class_name[0] == 'R')
    {
        //"Rich Edit"
        //"Edit"
        CEdit* edit = (CEdit*)child;
        edit->SetSel(0, edit->GetWindowTextLength());
    }
}
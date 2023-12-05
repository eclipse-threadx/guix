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

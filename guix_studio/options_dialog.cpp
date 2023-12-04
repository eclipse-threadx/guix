

#include "studiox_includes.h"
#include "options_dialog.h"

enum OPTION_CONTROL_IDS {
    OPTION_0 = 20100,
    OPTION_1,
    OPTION_2,
    OPTION_3,
    OPTION_4
};

enum option_dlg_test_commands{
    TEST_SELECT_OPTION = 1,
};

BEGIN_MESSAGE_MAP(options_dialog, express_dialog)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_CONTROL_RANGE(BN_CLICKED, OPTION_0, OPTION_4, options_dialog::OnOptionClicked)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(options_dialog, express_dialog)

///////////////////////////////////////////////////////////////////////////////
options_dialog::options_dialog(CString title, CString msg, STRING_VAL_PAIR *option_list, CWnd* pParent /*=NULL*/)
    : express_dialog(options_dialog::IDD, pParent)
{
    IconId = IDB_WARNING;
    SetTitleText(title);
    m_message = msg;
    m_selected_option = 0;

    mp_option_list = option_list;
}

///////////////////////////////////////////////////////////////////////////////
options_dialog::~options_dialog()
{

}

///////////////////////////////////////////////////////////////////////////////
int options_dialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    m_message_box.Create(m_message, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);

    STRING_VAL_PAIR *entry = mp_option_list;
    m_option_count = 0;

    while (entry->val && (m_option_count < MAX_OPTION_COUNT))
    {
        m_option_buttons[m_option_count].Create(entry->name, BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            CRect(0, 0, 0, 0), this, OPTION_0 + m_option_count);

        entry++;
        m_option_count++;
    }

    PositionChildren();

    if (mTitleText)
    {
        SetWindowText(mTitleText);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void options_dialog::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
void options_dialog::PositionChildren()
{
    CRect window_size;
    CRect size;
    GetClientRect(&window_size);
    size = window_size;

    size.left += 16;
    size.right -= 16;
    size.top += 40;
    size.bottom = size.top + 40;
    m_message_box.MoveWindow(&size);


    for (int index = 0; index < m_option_count; index++)
    {
        size.top = size.bottom + 10;
        size.bottom = size.top + 30;
        m_option_buttons[index].MoveWindow(&size);
    }

    window_size.bottom = size.bottom + 60;
    MoveWindow(&window_size);
    CenterWindow();
}

///////////////////////////////////////////////////////////////////////////////
void options_dialog::OnOptionClicked(UINT nID)
{
    m_selected_option = mp_option_list[nID - OPTION_0].val;
    OnOK();
}

///////////////////////////////////////////////////////////////////////////////
LRESULT options_dialog::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SELECT_OPTION:
        OnOptionClicked(OPTION_0 + lParam);
        break;
    }

    return 0;
}
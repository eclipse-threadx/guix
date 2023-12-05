#include "studiox_includes.h"
#include "message_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(message_dialog, express_dialog)
    ON_BN_CLICKED(IDCANCEL, &message_dialog::OnCancel)
    ON_BN_CLICKED(IDOK, &message_dialog::OnOK)
    ON_WM_NCHITTEST()
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()

enum message_dialog_test_commands
{
    TEST_YES_TO_MESSAGE_DIALOG = 1,
    TEST_NO_TO_MESSAGE_DIALOG,
    TEST_CLOSE_MESSAGE_DIALOG
};

extern CFont MediumFont;
extern CFont NormalFont;

IMPLEMENT_DYNAMIC(message_dialog, express_dialog)
///////////////////////////////////////////////////////////////////////////////
message_dialog::message_dialog(const char *caption, const char *msg, bool type, CWnd *pParent)
    :express_dialog(message_dialog::IDD, pParent)
{
    IconId = IDB_WARNING;
    yesandno = type;

    mMessage = CString(msg);
    SetTitleText(CString(caption));
}

/////////////////////////////////////////////////////////////////////////
//////
message_dialog::~message_dialog()
{

}

///////////////////////////////////////////////////////////////////////////////
BOOL message_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    if (!mMessage.IsEmpty())
    {
        AddCStaticControl(mMessage);
    }

    if (yesandno)
    {
        AddCancelButton(_T("No"));
        AddSaveButton(_T("Yes"));
    }
    else
    {
        AddSaveButton(_T("OK"));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
void message_dialog::AddCStaticControl(CString text)
{
CRect   client;
CRect   size;
CString string_process;
CString temp_string;
CSize   temp_size;
int     index;

    int     i_count = 0;
    int     index_count = 0;
    CString string_left = text;
    CDC    *pDC = this->GetDC();
    CFont *old_font = pDC->SelectObject(&NormalFont);
    CSize   tsize = pDC->GetTextExtent(text);
    pDC->SelectObject(old_font);
    int     temp_width = 0;
    int     count = text.Replace(_T("\n"), _T("\n"));
    
    GetClientRect(&client);

    size.top = client.top + m_title_bar_height + 8;
    size.left = client.left + 16;
    size.bottom = size.top + tsize.cy;
    size.right = client.right - 16;

    /*Large window while it's not enough.*/
    while (i_count <= count)
    {
        index = 0;
        index_count = string_left.Find(_T("\n"));
        if (index_count == -1)
        {
            string_process = string_left;
        }
        else
        {
            string_process = string_left.Left(index_count);
            string_left = string_left.Mid(index_count + 1);
        }

        while (index != -1)
        {
            index = string_process.Find(' ');
            temp_string = string_process.Left(index);
            string_process = string_process.Mid(index + 1);
            temp_size = pDC->GetTextExtent(temp_string);
            temp_width += temp_size.cx;

            if (temp_size.cx > size.right)
            {
                size.right = temp_size.cx + 16;
                temp_width = 0;
                size.bottom += temp_size.cy;
                continue;
            }

            if (temp_width < size.right)
            {
                continue;
            }
            else
            {
                size.right = temp_width + 1;
                temp_width = 0;
                size.bottom += temp_size.cy;
            }
        }
        temp_size = pDC->GetTextExtent(string_process);
        if (temp_size.cx > size.right)
        {
            size.right = temp_size.cx + 16;
        }
        size.bottom += tsize.cy;
        i_count++;
    }
    
    size.bottom += tsize.cy + 1;
    client.right = size.right + 16;
    client.bottom = size.bottom + m_status_bar_height + 16;

    MoveWindow(client);
    CenterWindow();
    
    this->ReleaseDC(pDC);

    msg_text.Create(mMessage, WS_CHILD | WS_VISIBLE | SS_LEFT, size, this);
    msg_text.SetFont(&NormalFont);

    msg_text.ShowWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
LRESULT message_dialog::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_YES_TO_MESSAGE_DIALOG:
        message_dialog::OnOK();
        break;

    case TEST_NO_TO_MESSAGE_DIALOG:
    case TEST_CLOSE_MESSAGE_DIALOG:
        message_dialog::OnCancel();

        break;
    }

    return 0;
}
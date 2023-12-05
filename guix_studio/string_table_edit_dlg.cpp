// string_table_edit_dlg.cpp : implementation file
//

//#include "stdafx.h"
//#include "studiox.h"
#include "studiox_includes.h"
#include "string_table_edit_dlg.h"
#include "xliff_read_write.h"
#include "csv_read_write.h"
#include "string_export_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENABLE_SEARCH

extern "C" {
    extern GX_DISPLAY  target_win_display;
    extern VOID *_win32_canvas_memory_prepare(GX_CANVAS *canvas, GX_RECTANGLE *dirty);
}
//extern GX_WIDGET *one_app_widget_create(widget_info *info, GX_WIDGET *parent);
extern GX_WIDGET *app_child_widget_find(GX_WIDGET *parent, CString widget_name);


CString GetUtf8Text(CRichEditCtrl *edit)
{
    CString return_val("");
    wchar_t *mybuf; 
    GETTEXTEX gte ; 
    gte.cb = MAX_PATH; 
    gte.flags = GT_DEFAULT; 
    gte.codepage = 1200; // 1200 signifies UNICODE 
    int char_count;

    char_count = edit->GetTextLength();

    if (char_count)
    {
        char_count++;
        mybuf = new wchar_t[char_count];
        gte.cb = char_count * 2;
        // get the text in UNICODE 
        edit->SendMessage(EM_GETTEXTEX,(unsigned int)&gte,(long)mybuf);

        // convert Unicode to UTF8
#ifdef _UNICODE
        return_val = mybuf;
#else
        char *utf8buf = new char[char_count * 6];
        WideCharToMultiByte(CP_UTF8, 0, mybuf, -1, utf8buf, char_count * 6, NULL, NULL);
        return_val = CString(utf8buf);
        delete [] utf8buf;
#endif
        delete[] mybuf;
    }
    return return_val;
}

void SetUtf8Text(CRichEditCtrl *edit, CString &text)
{
#ifdef _UNICODE

    // set the text in UNICODE 
    //edit->SendMessage(EM_SETTEXTEX, (unsigned int)&ste, (long)text.GetString());

    edit->SetSel(0, -1);
    edit->ReplaceSel(text.GetString());
#else
    SETTEXTEX ste ; 
    ste.flags = GT_DEFAULT; 
    ste.codepage = 1200; // 1200 signifies UNICODE 

    if (text.IsEmpty())
    {
        edit->SetWindowText(_T(""));
    }
    else
    {
        wchar_t *mybuf;

        mybuf = CStringToWideChar(text);

        // set the text in UNICODE 
        edit->SendMessage(EM_SETTEXTEX, (unsigned int)&ste, (long)mybuf);
        delete[] mybuf;
    }
#endif

}


#define BUTTON_BAR_HEIGHT 28
#define BUTTON_HEIGHT 24
#define BUTTON_WIDTH  72

#define TABLE_HEADER_HEIGHT 24

#define STRING_INFO_COLUMN_WIDTH 280
#define INFO_FIELD_WIDTH 50
#define INFO_FIELD_SPACE 10

#define LABLE_WIDTH 70
#define LABLE_HEIGHT 20

#define MIN_STRING_DIALOG_WIDTH 650

#define STRING_REFERENCE_ROW_TEXT_HEIGHT 24

///////////////////////////////////////////////////////////////////////////////
enum table_button_ids {
    ID_ADD_STRING = 1024,
    ID_DELETE_STRING,
    ID_IMPORT,
    ID_EXPORT,
    ID_ID_EDIT,
    ID_TOP_STRING_EDIT,
    ID_BOTTOM_STRING_EDIT,
    ID_SEARCH_BUTTON,
    ID_SEARCH_EDIT,
    ID_FONT_SELECT,
    ID_NOTES_EDIT,
    ID_SORT_COMBOBOX,
    ID_SORT_LABEL
};

enum string_table_edit_dlg_test_command{
    TEST_GET_STRING_COUNT = 1,
    TEST_ADD_STRING,
    TEST_DELETE_STRING,
    TEST_IMPORT_XLIFF,
    TEST_EXPORT_XLIFF,
    TEST_TOGGLE_THREE_COLUMN_MODE,
    TEST_INCREMENT_TRANS_LANGUAGE,
    TEST_DECREMENT_TRANS_LANGUAGE,
    TEST_EDIT_TOP_STRING,
    TEST_EDIT_BOTTOM_STRING,
    TEST_EDIT_STRING_ID,
    TEST_SELECT_STRING,
    TEST_SAVE_STRING_EDIT,
    TEST_CANCEL_STRING_EDIT,
    TEST_SORT_STRING
};


extern CString target_class_name;

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(string_table_edit_dlg, express_dialog)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_EN_KILLFOCUS(ID_ID_EDIT, OnIdEdit)
    ON_EN_CHANGE(ID_TOP_STRING_EDIT, OnStringEdit)
    ON_EN_CHANGE(ID_BOTTOM_STRING_EDIT, OnStringEdit)
    ON_CBN_SELCHANGE(ID_FONT_SELECT, OnChangeFont)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    ON_WM_GETMINMAXINFO()
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)

    //ON_EN_KILLFOCUS(ID_TOP_STRING_EDIT, string_table_edit_dlg::SaveStringInfo)
    //ON_EN_KILLFOCUS(ID_ID_EDIT, string_table_edit_dlg::SaveStringInfo)
ON_WM_PAINT()
ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(table_row, CWnd)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(table_header, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(string_table_edit_dlg, express_dialog)

///////////////////////////////////////////////////////////////////////////////
string_table_edit_dlg::string_table_edit_dlg(string_table *table, CWnd* pParent /*=NULL*/)
	: express_dialog(string_table_edit_dlg::IDD, pParent)
{
    IconId = IDB_STRINGS;
    SetTitleText("String Table Editor");

    mpTable = table;
    mpWinHeader = NULL;
    mpButtonFrame = NULL;
    mpTableFrame = NULL;
    mpTableHeader = NULL;
    mpStrReferenceWin = NULL;
    mRowId = 0;
    mThreeColumnMode = FALSE;
    mNotesBrush = new CBrush(GetSysColor(COLOR_WINDOW));
    mViewEditBrush = new CBrush(GetSysColor(COLOR_WINDOW));

    m_sys_dpi = GetSystemDPI();

    int text_scaler = GetTextScaler();
    SetControlDimensions(text_scaler);
    CreateDialogFont(text_scaler);
}

///////////////////////////////////////////////////////////////////////////////
string_table_edit_dlg::~string_table_edit_dlg()
{
    if (mpWinHeader)
    {
        delete mpWinHeader;
    }
    if (mpButtonFrame)
    {
        delete mpButtonFrame;
    }

    if (mpTableFrame)
    {
        delete mpTableFrame;
    }
    if (mpTableHeader)
    {
        delete mpTableHeader;
    }

    if (mpStrReferenceWin)
    {
        delete mpStrReferenceWin;
    }
    delete mNotesBrush;
    delete mViewEditBrush;
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::SetControlDimensions(int text_scaler)
{
    m_button_bar_height = MulDiv(BUTTON_BAR_HEIGHT, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_table_header_height = MulDiv(TABLE_HEADER_HEIGHT, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_string_info_column_width = MulDiv(STRING_INFO_COLUMN_WIDTH, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_info_field_width = MulDiv(INFO_FIELD_WIDTH, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_info_field_space = MulDiv(INFO_FIELD_SPACE, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_min_string_dialog_width = MulDiv(MIN_STRING_DIALOG_WIDTH, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_lable_width = MulDiv(LABLE_WIDTH, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_lable_height = MulDiv(LABLE_HEIGHT, m_sys_dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
HBRUSH string_table_edit_dlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    if (pWnd == &mNotesEdit)
    {
        pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
        return (HBRUSH) mNotesBrush->GetSafeHandle();
    }

    return express_dialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


///////////////////////////////////////////////////////////////////////////////
int string_table_edit_dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRect size;

    GetTargetView()->GetWindowRect(&size);
    if (size.Width() < MIN_STRING_DIALOG_WIDTH)
    {
        int diff = (MIN_STRING_DIALOG_WIDTH - size.Width() + 2) / 2;
        size.left -= diff;
        size.right += diff;
    }
    MoveWindow(size);

    CFont *font = &m_dialog_font;

    mpButtonFrame = new button_frame(mpTable);
    mpButtonFrame->CreateEx(WS_EX_CONTROLPARENT, target_class_name, _T("button_frame"),
        WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, 0);

    // create a scrolling frame to contains table rows    
    mpTableFrame = new table_frame(this, mpTable);
    mpTableFrame->Create(target_class_name, _T("table_frame"),
        WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_TABSTOP, CRect(0, 0, 0, 0), this, 0);

    // create the header that goes across top of table rows
    mpTableHeader = new table_header(mpTableFrame, mpTable);
    mpTableHeader->Create(target_class_name, _T("table_header"),
        WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, 0);

    mStrReferenceLabel.Create(_T("Widgets that use this string:"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mStrReferenceLabel.SetFont(font);
    mpStrReferenceWin = new string_reference_win(this, mpTable, -1);
    mpStrReferenceWin->Create(target_class_name, _T("string_reference_win"),
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_BORDER, CRect(0, 0, 0, 0), this, 0);

    mIdLabel.Create(_T("String ID:"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mIdLabel.SetFont(font);
    mIdEdit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_UPPERCASE|WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_ID_EDIT);
    mIdEdit.SetFont(font);
    SetAccessibleFullDescription(mIdEdit.GetSafeHwnd(), L"Required");
    SetAccessibleDescription(mIdEdit.GetSafeHwnd(), L"Required");

    mRequiredIndicationLable.Create(_T("Indicates required field"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_REQUIRED_FIELD_LEGEND);
    mRequiredIndicationLable.SetFont(font);

    mTopEditLabel.Create(_T("String Text:"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mTopEditLabel.SetFont(font);
    mTopStringEdit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_TOP_STRING_EDIT);
    mBottomStringEdit.Create(WS_CHILD|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_BOTTOM_STRING_EDIT);

    if (string_table::IsRight2LeftLanguage(0))
    {
        //Displays text using right-to-left reading order.
        SetRichEditReadingDirection(&mTopStringEdit, PFE_RTLPARA);
    }
    
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = MulDiv(20, m_sys_dpi, DEFAULT_DPI_96);
    lf.lfWeight = FW_NORMAL;
    lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
    _tcscpy(lf.lfFaceName, _T("Arial Unicode MS"));
    mUniFont.CreateFontIndirect(&lf);
    
    mTopStringEdit.SetFont(&mUniFont);
    mBottomStringEdit.SetFont(&mUniFont);
    
    mNumRefLabel.Create(_T("Number of references:"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mNumRefLabel.SetFont(font);
    mNumReferences.Create(_T("0"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_SUNKEN, CRect(0, 0, 0, 0), this);
    mNumReferences.SetFont(font);

    mWidthLabel.Create(_T("String width (pixels):"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mWidthLabel.SetFont(font);
    mWidth.Create(_T("0"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_SUNKEN, CRect(0, 0, 0, 0), this);
    mWidth.SetFont(font);

    mFontLabel.Create(_T("Font:"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mFontLabel.SetFont(font);
    mFontCombo.Create(WS_CHILD|WS_TABSTOP|WS_VSCROLL|CBS_DROPDOWNLIST, CRect(0, 0, 0, 0), this, ID_FONT_SELECT);
    mFontCombo.SetFont(font);

    studiox_project *project = GetOpenProject();
    int font_id;
    int active_display = GetProjectView()->GetActiveDisplay();
    int active_theme = project->mDisplays[active_display].active_theme;

    for (font_id = 0; font_id < project->CountResources(active_display, RES_TYPE_FONT); font_id++)
    {
        res_info *info = project->FindResource(active_display, active_theme, RES_TYPE_FONT, font_id);
    
        if (info)
        {
            mFontCombo.AddString(info->name);
        }
    }
    mFontCombo.SetCurSel(0);
    mFontCombo.ShowWindow(SW_SHOW);

    mNotesLabel.Create(_T("Notes:"), WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this);
    mNotesLabel.SetFont(font);
    mNotesEdit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL|WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_NOTES_EDIT);
    mNotesEdit.SetFont(font);

    express_dialog::OnCreate(lpCreateStruct);
    AddCancelButton();
    AddSaveButton();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnSize(UINT nType, int cx, int cy)
{
    express_dialog::OnSize(nType, cx, cy);
    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::UpdateSearchMessage()
{
    CString HelpString;
    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();
    HelpString.Format(_T("Search found %d matching string table entries"), mpTableFrame->GetRowCount() - 1);

    if (pMain)
    {
        pMain->GetStatusMsgControl()->SetWindowText(HelpString);
        pMain->GetStatusMsgControl()->NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);
     }
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnGetMinMaxInfo(MINMAXINFO *lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);
    lpMMI->ptMinTrackSize.x = m_min_string_dialog_width;
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table_edit_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization 
    mpTableFrame->SelectFirstRow();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::PositionChildren()
{
    CRect size;
    CRect childsize;

    if (!mpTableFrame)
    {
        return;
    }

    GetClientRect(&size);

    int width = size.right - size.left;
    int height = size.bottom - size.top;
    int field_top;

    childsize = size;

    // position the frame around the buttonsr    
    childsize.top += (m_title_bar_height + 4);
    childsize.bottom = childsize.top + m_button_bar_height;
    childsize.left += 4;
    childsize.right -= 4;
    mpButtonFrame->MoveWindow(childsize);

    childsize.right = size.right - m_string_info_column_width - 4;
    childsize.top = childsize.bottom + 1;
    childsize.bottom = childsize.top + m_table_header_height;
    mpTableHeader->MoveWindow(childsize);

    childsize.top = childsize.bottom + 1;
    childsize.bottom = size.top + (height / 2) - 12;
    mpTableFrame->MoveWindow(childsize);
    childsize.bottom += 12;

    // position the label of string reference window
    childsize.right = size.right - 4;
    childsize.left = size.right - m_string_info_column_width + m_info_field_space;
    childsize.top -= m_table_header_height;
    childsize.bottom = childsize.top + m_table_header_height;
    mStrReferenceLabel.MoveWindow(childsize);

    // position string reference widnow
    childsize.top = childsize.bottom + 1;
    childsize.bottom = size.top + (height / 2) - 12;
    mpStrReferenceWin->MoveWindow(childsize);

    // position required indication lable.
    childsize.right = size.right - m_string_info_column_width - 4;
    childsize.top = childsize.bottom + 4;
    childsize.bottom = childsize.top + m_lable_height;
    CDC* dc = GetDC();
    SIZE sz;
    CFont *old_font = dc->SelectObject(&m_dialog_font);
    CString text;
    mRequiredIndicationLable.GetWindowTextW(text);
    GetTextExtentPoint32W(dc->GetSafeHdc(), text, text.GetLength(), &sz);
    childsize.left = childsize.right - sz.cx;
    childsize.top += (childsize.Height() - sz.cy) / 2;
    mRequiredIndicationLable.MoveWindow(childsize);

    childsize.left = size.left + 4;
    childsize.right = size.right - 4;

    // position the label for the id edit field
    mIdLabel.GetWindowTextW(text);
    GetTextExtentPoint32W(dc->GetSafeHdc(), text, text.GetLength(), &sz);
    dc->SelectObject(old_font);
    ReleaseDC(dc);
    childsize.right = childsize.left + sz.cx;
    childsize.top = childsize.bottom + 4;
    field_top = childsize.top;
    childsize.bottom = childsize.top + m_lable_height;
    mIdLabel.MoveWindow(childsize);

    int offset = m_lable_height + 4;
    // position the label for the string edit field
    childsize.OffsetRect(0, offset);
    childsize.right = childsize.left + m_lable_width;
    mTopEditLabel.MoveWindow(childsize);

    // position the id edit field.
    childsize.OffsetRect(0, -offset);
    childsize.left = childsize.right + 4;
    childsize.right = size.right - m_string_info_column_width - 4;
    mIdEdit.MoveWindow(childsize);

    // position the references label
    childsize.right = size.right - (m_info_field_width + 8);
    childsize.left = size.right - m_string_info_column_width + m_info_field_space;
    childsize.bottom = childsize.top + m_lable_height;
    mNumRefLabel.MoveWindow(childsize);

    // position the string width label
    childsize.OffsetRect(0, offset);
    mWidthLabel.MoveWindow(childsize);

    // position the references value
    childsize.OffsetRect(0, -offset);
    childsize.left = childsize.right + 8;
    childsize.right = size.right - 4;
    mNumReferences.MoveWindow(childsize);

    // position the width value:
    childsize.OffsetRect(0, offset);
    mWidth.MoveWindow(childsize);

    // position the font label:
    childsize.top = childsize.bottom + 4;
    childsize.bottom = childsize.top + m_lable_height;
    childsize.left = size.right - m_string_info_column_width + m_info_field_space;
    childsize.right = childsize.left + m_lable_width;
    mFontLabel.MoveWindow(childsize);

    // position the font combo
    childsize.left = childsize.right + 4;
    childsize.right = size.right - 4;
    childsize.bottom = childsize.top + 200;
    mFontCombo.MoveWindow(childsize);

    // position the notes label:
    childsize.top += 44;
    childsize.bottom = childsize.top + m_lable_height;
    childsize.left = size.right - m_string_info_column_width + m_info_field_space;
    childsize.right = childsize.left + m_lable_width;
    mNotesLabel.MoveWindow(childsize);

    // position the notes edit box:
    childsize.top += offset;
    childsize.bottom = size.bottom - (m_status_bar_height + 10);
    childsize.right = size.right - 4;
    mNotesEdit.MoveWindow(childsize);

    // position the top string edit field
    childsize.top = field_top + offset;
    childsize.left = size.left + m_lable_width + 8;
    childsize.right = size.right - m_string_info_column_width - 4;

    offset = m_status_bar_height + 10;

    if (mThreeColumnMode)
    {
        childsize.bottom = (childsize.top + (size.bottom - offset)) / 2;
    }
    else
    {
        childsize.bottom = size.bottom - offset;
    }
    mTopStringEdit.MoveWindow(childsize);

    if (mThreeColumnMode)
    {
        childsize.top = childsize.bottom;
        childsize.bottom = size.bottom - offset;
        mBottomStringEdit.MoveWindow(childsize);
    }

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::UpdateStringWidth(GX_RESOURCE_ID font_id, CString &str)
{
    GX_FONT *font;
    GX_VALUE string_width;
    CString width_val;
    GX_STRING string;

    if (str.IsEmpty())
    {
        mWidth.SetWindowText(_T("0"));
    }
    else
    {
        if (font_id < get_target_view_display()->gx_display_font_table_size)
        {
            font = get_target_view_display()->gx_display_font_table[font_id];
            string.gx_string_ptr = (GX_CHAR *) str.GetString();
            string.gx_string_length = str.GetLength();
            _gx_system_string_width_get_ext(font, &string, &string_width);
            width_val.Format(_T("%d"), string_width);
            mWidth.SetWindowText(width_val);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::UpdateEditFieldContent()
{
    CString ref_count;
    studiox_project *project = GetOpenProject();

    if (mRowId > 0)
    {
        int string_index = mpTableFrame->GetStringIndex(mRowId);
        m_record = mpTable->GetRecord(string_index);
        mIdEdit.SetWindowText(m_record.id_name);
        SetUtf8Text(&mTopStringEdit, m_record.strings[0]);

        if (mThreeColumnMode)
        {
            SetUtf8Text(&mBottomStringEdit, m_record.strings[mpTableFrame->GetTransLanguage()]);
            UpdateStringWidth(m_record.font_id, m_record.strings[mpTableFrame->GetTransLanguage()]);

            if (string_table::IsRight2LeftLanguage(mpTableFrame->GetTransLanguage()))
            {
                SetRichEditReadingDirection(&mBottomStringEdit, PFE_RTLPARA);
            }
            else
            {
                SetRichEditReadingDirection(&mBottomStringEdit, 0);
            }
        }
        else
        {
            UpdateStringWidth(m_record.font_id, m_record.strings[0]);
        }

        int resource_id = mpTable->GetResourceId(m_record.id_name);
        ref_count.Format(_T("%d"), widget_factory::CountReferences(project, RES_TYPE_STRING, resource_id));
        mNumReferences.SetWindowText(ref_count);
        mNotesEdit.SetWindowText(m_record.notes);

        CComboBox *pBox = (CComboBox *) GetDlgItem(ID_FONT_SELECT);

        if (pBox)
        {
            pBox->SetCurSel(m_record.font_id);
        }
    }
    else
    {
        mIdEdit.SetWindowText(_T(""));
        mTopStringEdit.SetWindowText(_T(""));
        mBottomStringEdit.SetWindowText(_T(""));
    }

}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table_edit_dlg::StringSelected(int row_id, BOOL bIdEdit)
{
    if (SaveStringInfo())
    {
        if (row_id < 0 || row_id > mpTableFrame->GetRowCount())
        {
            row_id = 0;
        }
        mRowId = row_id;

        UpdateEditFieldContent();

        if (row_id > 0)
        {
            if (bIdEdit)
            {
                mIdEdit.SetSel(0x0000ffff);
                //mIdEdit.SetFocus();
            }
            else
            {
                if (mThreeColumnMode)
                {
                    mBottomStringEdit.SetSel(0, 0x0000ffff);
                    //mBottomStringEdit.SetFocus();
                }
                else
                {
                    mTopStringEdit.SetSel(0, 0x0000ffff);
                    //mTopStringEdit.SetFocus();
                }
            }
        }

        mpStrReferenceWin->StringSelected(mpTableFrame->GetStringIndex(mRowId));
        return TRUE;
    }
    return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::SetThreeColumnMode(BOOL bOnOff)
{
    mThreeColumnMode = bOnOff;

    if (mThreeColumnMode)
    {
        mBottomStringEdit.ShowWindow(SW_SHOW);

        if (GetFocus() == &mTopStringEdit)
        {
            // Move focus to bottom edit before disable the top edit
            mBottomStringEdit.SetFocus();
        }

        mTopStringEdit.EnableWindow(FALSE);
    }
    else
    {
        mTopStringEdit.EnableWindow(TRUE);

        if (GetFocus() == &mBottomStringEdit)
        {
            // Move focus to top edit before hide bottom edit
            mTopStringEdit.SetFocus();
        }
        mBottomStringEdit.ShowWindow(SW_HIDE);
    }

    PositionChildren();
    UpdateEditFieldContent();
}


///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::Exit(int code)
{
    // TODO: Add your control notification handler code here
    //SaveStringInfo();
    
    if (code == IDOK)
    {
        if (SaveStringInfo())
        {
            express_dialog::OnOK();
        }
    }
    else
    {
        express_dialog::OnCancel();
    }
}

///////////////////////////////////////////////////////////////////////////////
// catch this event to prevent the dialog from closing every time the user
// presses the Enter key. Make them click a button.
void string_table_edit_dlg::OnOK()
{
    if (SaveStringInfo())
    {
        express_dialog::OnOK();
    }
}


///////////////////////////////////////////////////////////////////////////////
BOOL string_table_edit_dlg::SaveStringInfo()
{
    if (mRowId > 0)
    {
        CString val;
        CString IdString;
        CString TextString;
        GX_BOOL sort_string_table = FALSE;

        int string_index = mpTableFrame->GetStringIndex(mRowId);
        m_record = mpTable->GetRecord(string_index);
        mIdEdit.GetWindowText(IdString);

        if (IdString != m_record.id_name)
        {
            //check if the new id name is already exist
            int index = mpTable->FindStringIndex(IdString);
            if ((index != 0) && (index != string_index))
            {
                mIdEdit.SetWindowText(m_record.id_name);
                CString message;
                message.Format(_T("Duplicate String ID %s, String Id must be unique."), IdString);
                ErrorMsg(message, this);
                return FALSE;
            }

            //check if the new id name meet ANSI C variable naming rules
            if (!TestInputName(&mIdEdit, "String ID", m_record.id_name, this))
            {
                return FALSE;
            }

            if (mpTable->GetSortColumn() == 0)
            {
                sort_string_table = TRUE;
            }
        }

        if (mThreeColumnMode)
        {
            val = GetUtf8Text(&mBottomStringEdit);
            int language_id = mpTableFrame->GetTransLanguage();

            if ((mpTable->GetSortColumn() == language_id + 1) &&
                (m_record.strings[language_id] != val))
            {
                sort_string_table = TRUE;
            }

            mpTable->SetString(string_index, m_record.id_name, language_id, val);
        }
        else
        { 
            val = GetUtf8Text(&mTopStringEdit);

            if ((mpTable->GetSortColumn() == 1) &&
                (m_record.strings[0] != val))
            {
                sort_string_table = TRUE;
            }

            mpTable->SetString(string_index, m_record.id_name, 0, val);
        }

        mpStrReferenceWin->UpdateResourceTable();

        mNotesEdit.GetWindowText(TextString);
        mpTable->SetNotes(string_index, TextString);
        mpTableFrame->InvalidateRow(mRowId);

        if (sort_string_table)
        {
            mpButtonFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SORT_COMBOBOX, CBN_SELCHANGE), (LPARAM)(mpButtonFrame)->m_hWnd);
        }
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::UpdateStringFields()
{
    if (mRowId > 0)
    {
        CString IdString;
        CString TextString;
        int string_index = mpTableFrame->GetStringIndex(mRowId);

        mIdEdit.GetWindowText(IdString);

        if (mThreeColumnMode)
        {
            TextString = GetUtf8Text(&mBottomStringEdit);
            mpTable->SetString(string_index, IdString, mpTableFrame->GetTransLanguage(), TextString);
        }
        else
        {
            TextString = GetUtf8Text(&mTopStringEdit);
            mpTable->SetString(string_index, IdString, 0, TextString);
        }
        mpStrReferenceWin->UpdateResourceTable();
        mpTableFrame->Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::SetRichEditReadingDirection(CRichEditCtrl* edit, INT reading_order)
{
    // Modify the paragraph format
    PARAFORMAT pf;
    pf.cbSize = sizeof(PARAFORMAT);
    pf.dwMask = PFM_RTLPARA;
    pf.wEffects = reading_order;

    edit->SetSel(0, edit->GetTextLength() - 1);
    edit->SetParaFormat(pf);
    edit->SetSel(edit->GetTextLength(), edit->GetTextLength());
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnIdEdit()
{

}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnStringEdit()
{
    CString new_val;

    if (mRowId)
    {
        if (mThreeColumnMode)
        {
            new_val = GetUtf8Text(&mBottomStringEdit);
        }
        else
        {
            new_val = GetUtf8Text(&mTopStringEdit);
        }
        UpdateStringWidth(m_record.font_id, new_val);
        UpdateStringFields();
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnChangeFont()
{
    CComboBox *pBox = (CComboBox *) GetDlgItem(ID_FONT_SELECT);

    if (pBox && mpTable && mRowId)
    {
        int Selected = pBox->GetCurSel();
        int string_index = mpTableFrame->GetStringIndex(mRowId);
        mpTable->SetDisplayFont(string_index, Selected);
        m_record.font_id = Selected;
        UpdateStringWidth(m_record.font_id, m_record.strings[0]);
    }
}

///////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT string_table_edit_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;

    switch (wParam)
    {
    case TEST_GET_STRING_COUNT:
        return mpTable->CountStrings() - 1;

    case TEST_ADD_STRING:
        pWnd = mpButtonFrame->GetDlgItem(ID_ADD_STRING);
        mpButtonFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_ADD_STRING, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
        break;

    case TEST_EDIT_TOP_STRING:
        mTopStringEdit.SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(ID_TOP_STRING_EDIT, EN_CHANGE), (LPARAM)mTopStringEdit.m_hWnd);
        break;

    case TEST_EDIT_BOTTOM_STRING:
        mBottomStringEdit.SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(ID_BOTTOM_STRING_EDIT, EN_CHANGE), (LPARAM)mBottomStringEdit.m_hWnd);
        break;

    case TEST_EDIT_STRING_ID:
         mIdEdit.SetWindowText(GetTestingParam(0));
        break;

    case TEST_SELECT_STRING:
    {
        table_row *row = (table_row *)mpTableFrame->GetWindow(GW_CHILD);
        int string_id = lParam;

        while (string_id && row)
        {
            row = (table_row *)row->GetWindow(GW_HWNDNEXT);
            string_id--;
        }

        if (row)
        {
            CRect rect;
            row->GetWindowRect(&rect);
            row->ScreenToClient(&rect);
            row->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.left + 1, rect.top + 1));
        }

        break;
    }

    case TEST_DELETE_STRING:
        pWnd = mpButtonFrame->GetDlgItem(ID_DELETE_STRING);
        mpButtonFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DELETE_STRING, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
        break;

    case TEST_IMPORT_XLIFF:
        mpTableFrame->ImportString(GetTestingParam(1));
        break;

    case TEST_EXPORT_XLIFF:
        pWnd = mpButtonFrame->GetDlgItem(ID_EXPORT);
        mpButtonFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_EXPORT, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
        break;

    case TEST_TOGGLE_THREE_COLUMN_MODE:
    {
        CRect rect;
        CRect frame_rect;
        mpTableHeader->GetClientRect(&rect);
        mpTableFrame->GetClientRect(&frame_rect);

        if (mpTableFrame->IsThreeColumnMode())
        {
            mpTableHeader->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(frame_rect.Width() * 2 / 3 + 1, rect.top + 1));
        }
        else
        {
            mpTableHeader->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.right - 1, rect.top + 1));
        }

        break;
    }

    case TEST_INCREMENT_TRANS_LANGUAGE:
        if (mpTableFrame->IsThreeColumnMode())
        {
            CRect rect;
            mpTableHeader->GetWindowRect(&rect);
            mpTableHeader->ScreenToClient(&rect);

            mpTableHeader->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.right - 1, rect.top + 1));
        }
        break;

    case TEST_DECREMENT_TRANS_LANGUAGE:
        if (mpTableFrame->IsThreeColumnMode())
        {
            CRect rect;
            mpTableHeader->GetWindowRect(&rect);
            mpTableHeader->ScreenToClient(&rect);

            mpTableHeader->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(rect.right - mpTableHeader->GetIconWidth() - 1, rect.top + 1));
        }
        break;

    case TEST_SAVE_STRING_EDIT:
        OnOK();
        break;

    case TEST_CANCEL_STRING_EDIT:
        OnCancel();
        break;

    case TEST_SORT_STRING:
        pWnd = mpButtonFrame->GetDlgItem(ID_SORT_COMBOBOX);
        ((CComboBox *)pWnd)->SelectString(0, GetTestingParam(0));
        mpButtonFrame->SendMessage(WM_COMMAND, MAKEWPARAM(ID_SORT_COMBOBOX, CBN_SELCHANGE), (LPARAM)(mpButtonFrame->m_hWnd));
        break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(button_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(ID_DELETE_STRING, button_frame::OnDeleteClicked)
    ON_BN_CLICKED(ID_ADD_STRING, button_frame::OnAddString)
    ON_BN_CLICKED(ID_SEARCH_BUTTON, button_frame::OnSearchButton)
    ON_BN_CLICKED(ID_IMPORT, button_frame::OnImportString)
    ON_BN_CLICKED(ID_EXPORT, button_frame::OnExportString)
    ON_CBN_SELCHANGE(ID_SORT_COMBOBOX, OnSortStringTable)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
button_frame::button_frame(string_table *table)
{
    mpTable = table;

    UpdateControlDimensions(GetTextScaler());
}

///////////////////////////////////////////////////////////////////////////////
button_frame::~button_frame()
{
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::UpdateControlDimensions(int text_scaler)
{
    int dpi = GetSystemDPI();
    m_button_width = MulDiv(BUTTON_WIDTH, dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
    m_button_height = MulDiv(BUTTON_HEIGHT, dpi, DEFAULT_DPI_96) * text_scaler / DEFAULT_TEXT_SCALER;
}

///////////////////////////////////////////////////////////////////////////////
int button_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CWnd::OnCreate(lpCreateStruct);

    string_table_edit_dlg* dlg = (string_table_edit_dlg*)GetParent();
    CFont *font = dlg->GetDialogFont();

    // create the buttons along the top of the frame
    mAddButton.Create(_T("Add String"), BS_PUSHBUTTON|BS_CENTER|BS_VCENTER|WS_CHILD|WS_VISIBLE|WS_TABSTOP, CRect(0,0,0,0), this, ID_ADD_STRING);
    mAddButton.SetFont(font);
    mDeleteButton.Create(_T("Delete String"), BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_DELETE_STRING);
    mDeleteButton.SetFont(font);

    mImportButton.Create(_T("Import"), BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_IMPORT);
    mImportButton.SetFont(font);
    mExportButton.Create(_T("Export"), BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_EXPORT);
    mExportButton.SetFont(font);

    #ifdef ENABLE_SEARCH
    mSearchBitmap.LoadBitmap(IDB_SEARCH);
    mSearchString.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_SEARCH_EDIT);
    SetControlAccessibleName(mSearchString.GetSafeHwnd(), _T("Search"));

    mSearchButton.Create(_T("Search"), BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_SEARCH_BUTTON);
    mSearchButton.SetBitmap(mSearchBitmap);
    #endif

    mSortLabel.Create(_T("Sorting By:"), WS_CHILD | WS_VISIBLE | SS_RIGHT, CRect(0, 0, 0, 0), this, ID_SORT_LABEL);
    mSortLabel.SetFont(font);

    mSortCombobox.Create(WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 0, 0), this, ID_SORT_COMBOBOX);
    mSortCombobox.SetFont(font);
    mSortCombobox.SetItemHeight(-1, 16);

    int id = 0;
    studiox_project *project = GetOpenProject();

    mSortCombobox.AddString(_T("None"));
    mSortCombobox.AddString(_T("String ID"));
    // insert all languages
    for (id = 0; id < project->mHeader.num_languages; id++)
    {
        mSortCombobox.AddString(project->mHeader.languages[id].name);
    }
    mSortCombobox.AddString(_T("Reference Count"));
    mSortCombobox.SetCurSel(mpTable->GetSortColumn() + 1);
    mSortCombobox.ShowWindow(SW_SHOW);

    if (mpTable->GetSortColumn() == mpTable->CountLanguages() + 1)
    {
        mpTable->Sort();
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::PositionChildren()
{
    CRect client;
    CRect childsize;

    GetClientRect(&client);
    int width = client.right - client.left;
    int height = client.bottom - client.top;

    childsize = client;
    childsize.top += 2;
    childsize.left += 4;
    childsize.right = childsize.left + m_button_width;
    childsize.bottom = childsize.top + m_button_height;
    mAddButton.MoveWindow(&childsize);

    childsize.OffsetRect(m_button_width + 2, 0);
    mDeleteButton.MoveWindow(&childsize);

    childsize.OffsetRect(m_button_width + 2, 0);
    mImportButton.MoveWindow(&childsize);

    childsize.OffsetRect(m_button_width + 2, 0);
    mExportButton.MoveWindow(&childsize);

    #ifdef ENABLE_SEARCH
    childsize.OffsetRect(m_button_width + 12, 0);
    childsize.right += m_button_width / 2;
    mSearchString.MoveWindow(&childsize);

    childsize.OffsetRect(childsize.Width(), 0);
    childsize.right = childsize.left + 30;
    mSearchButton.MoveWindow(&childsize);
    #endif

    childsize.right = client.right - 4;
    childsize.left = childsize.right - m_button_width - 30;
    mSortCombobox.MoveWindow(&childsize);

    childsize.right = childsize.left - 4;
    childsize.left = childsize.right - m_button_width;
    childsize.top += 4;
    mSortLabel.MoveWindow(&childsize);
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
BOOL button_frame::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if ((pMsg->message == WM_KEYDOWN) &&
        (pMsg->wParam == VK_RETURN))
    {
        CWnd* focus_owner = GetFocus();
        if (focus_owner)
        {
            int ctrl_id = focus_owner->GetDlgCtrlID();

            switch(ctrl_id)
            {
            case ID_ADD_STRING:
                OnAddString();
                return TRUE;

            case ID_DELETE_STRING:
                OnDeleteClicked();
                return TRUE;

            case ID_IMPORT:
                OnImportString();
                return TRUE;

            case ID_EXPORT:
                OnExportString();
                return TRUE;

            case ID_SEARCH_BUTTON:
            case ID_SEARCH_EDIT:
                OnSearchButton();
                return TRUE;
            }
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnAddString()
{
    if (mpTable)
    {
        string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();

        dlg->SaveStringInfo();

        mpTable->AddString();
        
        dlg->GetTableFrame()->AddString();

        //Sort string table
        OnSortStringTable();
    }
}


///////////////////////////////////////////////////////////////////////////////
void button_frame::OnDeleteClicked()
{
    string_table_edit_dlg *dlg = (string_table_edit_dlg *) GetParent();
    dlg->StringSelected(0, TRUE);
    
    table_frame *p_table_frame = dlg->GetTableFrame();

    if (!p_table_frame)
    {
        return;
    }

    int selected_row_id = p_table_frame->GetSelectedId();
    int selected_string_index = p_table_frame->GetStringIndex(selected_row_id);
    studiox_project *project = GetOpenProject();

    if (project && selected_string_index > 0)
    {
        string_table_record record = mpTable->GetRecord(selected_string_index);

        int resource_id = mpTable->GetResourceId(record.id_name);
        if (mpTable->RemoveString(selected_string_index))
        {
            // ask the widget factory to update all references to this
            // string id (and decrement all higher ids as well).
            widget_factory::StringDeleted(project, resource_id);
            dlg->GetStrReferenceWin()->UpdateResourceTable();
            dlg->GetTableFrame()->DeleteString(selected_row_id);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnSearchButton()
{
    CString SearchString;
    string_table_edit_dlg *dlg = (string_table_edit_dlg *) GetParent();

    GetDlgItemText(ID_SEARCH_EDIT, SearchString);
    dlg->GetTableFrame()->SetSearchString(SearchString);
    dlg->UpdateSearchMessage();
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnImportString()
{
    CString pathname;

    if (BrowseForSingleFile(_T("Select XLIFF/CSV file"),
        _T("XLIFF/CSV Files\0*.xlf;*.xliff;*.xml;*.csv\0"),
        _T("xliff"), pathname, this))
    {
        string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();
        dlg->GetTableFrame()->ImportString(pathname);
    }
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnExportString()
{
    string_export_dlg dlg;

    dlg.SetOwner(GetParent());

    if (dlg.DoModal() == IDOK)
    {
        studiox_project *project = GetOpenProject();

        if(!project)
        {
            return;
        }

        if (project->mHeader.string_export_filetype == STRING_EXPORT_TYPE_XLIFF)
        {
            xliff_read_write writer;
            if (writer.ExportLanguage(project))
            {
                Notify("String data successfully exported to XLIFF file", GetParent());
            }
            else
            {
                ErrorMsg("Failed to write xliff file. Check path and filename.", GetParent());
            }
        }
        else
        {
            csv_read_write writer;
            if (writer.ExportLanguage(project))
            {
                Notify("String data successfully exported to CSV file", GetParent());
            }
            else
            {
                ErrorMsg("Failed to write csv file. Check path and filename.", GetParent());
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void button_frame::OnSortStringTable()
{
    int index = mSortCombobox.GetCurSel();
    string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();
    mpTable->SetSortColumn(index - 1);

    if (index > 0)
    {
        dlg->GetTableFrame()->SortStringTable();
    }
}

HBRUSH button_frame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

    if (pWnd->GetDlgCtrlID() == ID_SORT_LABEL)
    {
        //set sort label background to white
        pDC->SetBkColor(RGB(255, 255, 255));
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else
    {
        return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

///////////////////////////////////////////////////////////////////////////////
//void button_frame::OnSearchEdit()
//{
//    mSearchButton.SetCheck(BST_UNCHECKED);
//}

#if 0
BOOL string_id_win::OnEraseBkgnd(CDC* pDC)
   {
      // Set brush to desired background color
      CBrush backBrush(RGB(255, 128, 128));

      // Save old brush
      CBrush* pOldBrush = pDC->SelectObject(&backBrush);

      CRect rect;
      pDC->GetClipBox(&rect);     // Erase the area needed

      pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
          PATCOPY);
      pDC->SelectObject(pOldBrush);
      return TRUE;
   }
#endif


BEGIN_MESSAGE_MAP(table_frame, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    //ON_WM_ERASEBKGND()
    //ON_WM_PAINT()
    ON_WM_SIZE()
    //ON_WM_CTLCOLOR()
    //ON_WM_SYSCHAR()

    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_SETFOCUS()
    ON_WM_PAINT()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
table_frame::table_frame(CWnd *parent, string_table *table)
{
    mTransLanguage = 1;
    mpTable = table;
    mThreeColumnMode = FALSE;
    mSearchString = "";
    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);
    mSelectedRow = 0;
}

///////////////////////////////////////////////////////////////////////////////
table_frame::~table_frame()
{
    delete m_scroll_helper;
}

///////////////////////////////////////////////////////////////////////////////
int table_frame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CString filter;
    CWnd::OnCreate(lpCreateStruct);
    CreateTableRows();
    PositionChildren();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnDestroy()
{
    CWnd *child = GetWindow(GW_CHILD);

    while(child)
    {
        child->DestroyWindow();
        delete child;    
        child = GetWindow(GW_CHILD);
    }   
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CWnd::OnPaint() for painting messages

    if ((GetFocus() == this) && (mSelectedRow < 1))
    {
        CRect rect;
        GetClientRect(&rect);
        dc.DrawFocusRect(&rect);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL table_frame::IncludeInSearch(int RowId)
{
    CString text;
    int language;

    if (mSearchString.IsEmpty())
    {
        return TRUE;
    }
    text = mpTable->GetStringId(RowId);
    text.MakeUpper();

    if (text.Find(mSearchString) >= 0)
    {
        return TRUE;
    }

    for (language = 0; language < mpTable->CountLanguages(); language++)
    {
        text = mpTable->GetString(RowId, language);
        text.MakeUpper();
        if (text.Find(mSearchString) >= 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::SetSearchString(CString &search)
{
    mSearchString = search;
    mSearchString.MakeUpper();

    RowSelected(0, 0);
    CreateTableRows();

    m_scroll_helper->SetScrollPos(SB_VERT, 0);
 
    PositionChildren();
    SelectFirstRow();
}


///////////////////////////////////////////////////////////////////////////////
int table_frame::CountFilterRows()
{
    int count = mpTable->CountStrings();
    int passed = 0;
    int StringId;

    if (mSearchString.IsEmpty())
    {
        return count - 1;
    }

    for (StringId = 1; StringId < count; StringId++)
    {
        if (IncludeInSearch(StringId))
        {
            passed++;
        }
    }
    return passed;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::CreateTableRows()
{
    int index;
    table_row *row;
    CWnd *child = GetWindow(GW_CHILD);

    while(child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }   
    int rowcount = mpTable->CountStrings();

    index = 1;
    mRowCount = 1;

    while(index < rowcount)
    {
        if (IncludeInSearch(index))
        {
            row = new table_row(this, mRowCount++, index, mpTable);
            row->Create(target_class_name, _T("string_row"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);
        }
        index++;
    }
}



///////////////////////////////////////////////////////////////////////////////
void table_frame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Compute the desired change or delta in scroll position.
    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
BOOL table_frame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    BOOL wasScrolled = m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
    return wasScrolled;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SetFocus();
    CWnd::OnNcLButtonDown(nHitTest, point);
}

///////////////////////////////////////////////////////////////////////////////
int table_frame::GetTransLanguage()
{
    if (mThreeColumnMode)
    {
        return mTransLanguage;
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::ToggleThreeColumnMode()
{
    string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();

    if (!dlg->SaveStringInfo())
    {
        return;
    }

    int active_language = 0;
    BOOL update = FALSE;

    if (mThreeColumnMode)
    {
        mThreeColumnMode = FALSE;
        update = TRUE;
    }
    else
    {
        if (GetOpenProject()->mHeader.num_languages > 1)
        {
            mThreeColumnMode = TRUE;
            active_language = mTransLanguage;
            update = TRUE;
        }
    }
    
    if (update)
    {
        dlg->SetThreeColumnMode(mThreeColumnMode);

        string_reference_win* reference_win = dlg->GetStrReferenceWin();
        if (reference_win)
        {
            reference_win->SetActiveLanguage(active_language);
        }
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    CRect childsize;
    CRect client;
    GetClientRect(&client);

    table_row *row = (table_row *) GetWindow(GW_CHILD);

    if (row)
    {
        mRowHeight = row->GetRowHeight();
    }

    m_scroll_helper->OnSize(nType, cx, cy);
    PositionChildren();
}

///////////////////////////////////////////////////////////////////////////////
BOOL table_frame::PreTranslateMessage(MSG* pMsg)
{
    CRect rect;

    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_UP:
            if (mSelectedRow > 1)
            {
                RowSelected(mTransLanguage, mSelectedRow - 1, TRUE);
                SelectedVisible();
            }
            return TRUE;

        case VK_DOWN:
            if (mSelectedRow < mRowCount - 1)
            {
                RowSelected(mTransLanguage, mSelectedRow + 1, TRUE);
                SelectedVisible();
            }
            return TRUE;

        case VK_HOME:
            if (mSelectedRow > 1)
            {
                RowSelected(mTransLanguage, 1, TRUE);
                SelectedVisible();
            }
            break;

        case VK_END:
            if (mSelectedRow < mRowCount - 1)
            {
                RowSelected(mTransLanguage, mRowCount - 1, TRUE);
                SelectedVisible();
            }
            break;

        case VK_PRIOR:
            GetClientRect(&rect);
            if (m_scroll_helper->Scroll(SB_VERT, -rect.Height()))
            {
                table_row * row = (table_row*)GetWindow(GW_CHILD);
                int pagesize = rect.Height() / row->GetRowHeight();

                RowSelected(mTransLanguage, max(1, mSelectedRow - pagesize), TRUE);
                SelectedVisible();
            }
            break;

        case VK_NEXT:
            GetClientRect(&rect);
            if (m_scroll_helper->Scroll(SB_VERT, rect.Height()))
            {
                table_row* row = (table_row*)GetWindow(GW_CHILD);
                int pagesize = rect.Height() / row->GetRowHeight();

                RowSelected(mTransLanguage, min(mRowCount - 1, mSelectedRow + pagesize), TRUE);
                SelectedVisible();
            }
            break;

        case VK_LEFT:
            if (mThreeColumnMode)
            {
                if (!DecrementTransLanguage())
                {
                    ToggleThreeColumnMode();
                }
            }
            return TRUE;

        case VK_RIGHT:
            if (mThreeColumnMode)
            {
                IncrementTransLanguage();
            }
            else
            {
                ToggleThreeColumnMode();
            }
            return TRUE;

        case VK_TAB:
            InvalidateRow(mSelectedRow);
            break;
        }
    }

    return CWnd::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    CWnd* row = GetRow(mSelectedRow);
    if (row)
    {
        row->Invalidate();
        row->SetFocus();
    }
    else
    {
        Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);

    Invalidate();
}


///////////////////////////////////////////////////////////////////////////////
void table_frame::PositionChildren()
{
    int rowheight;
    CRect client;
    GetClientRect(&client);
    CRect childsize = client;
    table_row *row = (table_row *) GetWindow(GW_CHILD);

    if (row)
    {
        rowheight = row->GetRowHeight();

        childsize.top -=(m_scroll_helper->GetScrollPos()).cy;
        childsize.bottom = childsize.top + rowheight - 1;
        int index = 1;
        CString name;
        
        while(row)
        {
            //update row text
			GetRowName(index, name);
            row->SetWindowText(name);

            row->MoveWindow(&childsize);
            childsize.top += rowheight;
            childsize.bottom += rowheight;
            row = (table_row *) row->GetWindow(GW_HWNDNEXT);
            index++;
        }
    }

    m_scroll_helper->SetDisplaySize(0, CountFilterRows() * mRowHeight);

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::RowSelected(int column, int row_id, bool assign_focus)
{
    string_table_edit_dlg *dlg = (string_table_edit_dlg *) GetParent();

    if (column == 0)
    {
        if (dlg->StringSelected(row_id, TRUE))
            mSelectedRow = row_id;
    }
    else
    {
        if (dlg->StringSelected(row_id, FALSE))
            mSelectedRow = row_id;
    }

    if (mSelectedRow > 0)
    {
        CWnd* row = GetRow(mSelectedRow);
        if (row)
        {
            row->Invalidate();

            if (assign_focus)
            {
                row->SetFocus();
            }
        }
    }
}

void table_frame::SelectedVisible()
{
    if (mSelectedRow >= 0)
    {
        int min_scroll_value = (mSelectedRow * mRowHeight) - m_scroll_helper->GetPageSize().cy;
        int max_scroll_value = min_scroll_value + m_scroll_helper->GetPageSize().cy - mRowHeight;

        if (min_scroll_value < 0)
        {
            min_scroll_value = 0;
        }

        if (max_scroll_value > m_scroll_helper->GetDisplaySize().cy - m_scroll_helper->GetPageSize().cy)
        {
            max_scroll_value = m_scroll_helper->GetDisplaySize().cy - m_scroll_helper->GetPageSize().cy;
        }

        int current_scroll_value = m_scroll_helper->GetScrollPos().cy;
        if ((current_scroll_value < min_scroll_value) || (current_scroll_value > max_scroll_value))
        {
            // scroll so that selected item is visible
            m_scroll_helper->SetScrollPos(SB_VERT, min_scroll_value);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
// this is called when the user adds a new string. scroll to the end and 
// select it automatically
void table_frame::AddString()
{
    CRect size;

    if (!mSearchString.IsEmpty())
    {
        mSearchString = "";
        CreateTableRows();
    }
    else
    {
        table_row *row = new table_row(this, mRowCount, mRowCount, mpTable);
        row->Create(target_class_name, _T("string_row"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);
        mRowCount++;
    }

    PositionChildren();

    RowSelected(NULL, mpTable->CountStrings() - 1, FALSE);

    //scroll to make selected item visible
    SelectedVisible();
}

void table_frame::DeleteString(int row_id)
{
    table_row *row = (table_row *) GetWindow(GW_CHILD);

    if (row)
    {
        while(row->GetWindow(GW_HWNDNEXT))
        {
            row = (table_row *) row->GetWindow(GW_HWNDNEXT);
        }
        row->DestroyWindow();
        delete row;
        mRowCount--;

        PositionChildren();

        if (row_id >= mRowCount)
        {
            row_id--;
        }
        RowSelected(NULL, row_id, FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL table_frame::IncrementTransLanguage()
{
    if (mTransLanguage < GetOpenProject()->mHeader.num_languages - 1)
    {
        string_table_edit_dlg *dlg = (string_table_edit_dlg *) GetParent();
        if (!dlg->SaveStringInfo())
        {
            return FALSE;
        }
        mTransLanguage++;
        dlg->UpdateEditFieldContent();

        string_reference_win *reference_win = dlg->GetStrReferenceWin();
        if (reference_win)
        {
            reference_win->SetActiveLanguage(mTransLanguage);
        }
        dlg->InvalideTableHeader();
        Invalidate();

        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL table_frame::DecrementTransLanguage()
{
    if (mTransLanguage > 1)
    {
        string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();
        if (!dlg->SaveStringInfo())
        {
            return FALSE;
        }
        mTransLanguage--;
        dlg->UpdateEditFieldContent();

        string_reference_win *reference_win = dlg->GetStrReferenceWin();
        if (reference_win)
        {
            reference_win->SetActiveLanguage(mTransLanguage);
        }
        dlg->InvalideTableHeader();
        Invalidate();

        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::SelectFirstRow()
{
    table_row *row = (table_row *) GetWindow(GW_CHILD);

    if (row)
    {
        RowSelected(0, row->GetRowId());
    }
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::GetRowName(int row_id, CString& name)
{
    name.Format(_T("row %d"), row_id);
}

///////////////////////////////////////////////////////////////////////////////
CWnd *table_frame::GetRow(int row_id)
{
    CWnd* child;

    if (row_id <= 0)
    {
        return NULL;
    }

    child = GetWindow(GW_CHILD);

    while ((row_id > 1) && child)
    {
        child = child->GetWindow(GW_HWNDNEXT);
        row_id--;
    }

    return child;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::InvalidateRow(int row_id)
{

    CWnd *child = GetRow(row_id);

    if (child)
    {
        child->Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////
int table_frame::GetStringIndex(int row_id)
{
    table_row *row = (table_row *)GetRow(row_id);
    if (row)
    {
        return row->GetStringIndex();
    }

    return -1;
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::ImportString(CString pathname)
{
    BOOL successed = FALSE;
    CString extention = PathFindExtension(pathname);
    extention.MakeLower();

    if (extention == ".csv")
    {
        csv_read_write reader;
        if (reader.ImportCsvFile(GetOpenProject(), pathname))
        {
            successed = TRUE;
        }
    }
    else
    {
        xliff_read_write reader;

        if (reader.ImportXliffFile(GetOpenProject(), pathname))
        {
            successed = TRUE;
        }
    }

    if (successed)
    {
        CreateTableRows();
        PositionChildren();

        string_table_edit_dlg *dlg = (string_table_edit_dlg *)GetParent();
        dlg->UpdateEditFieldContent();

        Notify("String data import completed", this);
    }
    else
    {
        ErrorMsg("String table import failed", this);
    }
}

///////////////////////////////////////////////////////////////////////////////
CFont * table_frame::GetUniFont()
{
    string_table_edit_dlg *string_dlg = (string_table_edit_dlg *)GetParent();
    return string_dlg->GetUniFont();
}

///////////////////////////////////////////////////////////////////////////////
void table_frame::SortStringTable()
{
    //record old selected row
    string_table_record selected_record = mpTable->GetRecord(GetSelectedId());

    if (!mSearchString.IsEmpty())
    {
        mSearchString = "";

        CreateTableRows();
        PositionChildren();
    }

    //de-select table row
    RowSelected(NULL, -1);

    mpTable->Sort();

    //select table row
    string_table_record current;
    int index;
    CString text;
    CWnd *child = GetWindow(GW_CHILD);

    for (index = 1; index < mpTable->CountStrings(); index++)
    {
        current = mpTable->GetRecord(index);
        if (current.id_name == selected_record.id_name)
        {
            int column;
            if (IsThreeColumnMode())
            {
                column = 2;
            }
            else
            {
                column = 1;
            }

            RowSelected(column, index);
        }

        if (child)
        {
            //update row name
            GetRowName(index, text);
            child->SetWindowText(text);
            child = child->GetWindow(GW_HWNDNEXT);
        }
    }

    //scroll to make selected item visible
    SelectedVisible();

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
table_header::table_header(table_frame *frame, string_table *table)
{
    mpFrame = frame;
    mpTable = table;

    CBitmap map;
    BITMAP  bmp;

    map.LoadBitmap(IDB_LEFT_ARROW);
    map.GetBitmap(&bmp);

    int dpi = GetSystemDPI();
    m_icon_width = MulDiv(bmp.bmWidth, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
void table_header::PaintHeader(CDC *dc, CRect &size)
{
    CString text;
    CPen line_pen;
    CRect framesize;
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }
    CBrush gray_brush(RGB(229, 229, 229));
    //null_pen.CreatePen(PS_SOLID, 0, RGB(210, 210, 210));
    line_pen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));

    CPen *old_pen = dc->SelectObject(&line_pen);
    CBrush *old_brush = dc->SelectObject(&gray_brush);

    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    dc->FillRect(&size, &gray_brush);
    dc->MoveTo(size.left, size.top);
    dc->LineTo(size.right, size.top);
    dc->MoveTo(size.left, size.bottom - 1);
    dc->LineTo(size.right, size.bottom -1);
    dc->MoveTo(size.left, size.top);
    dc->LineTo(size.left, size.bottom -1);
    dc->MoveTo(size.right - 1, size.top);
    dc->LineTo(size.right - 1, size.bottom -1);

    string_table_edit_dlg* dlg = (string_table_edit_dlg*)GetParent();
    CFont *old_font = dc->SelectObject(dlg->GetDialogFont());
    mpFrame->GetClientRect(&framesize);
    CRect boxsize = size;
    boxsize.top += 2;
    boxsize.right = boxsize.left + framesize.Width() / 3;
    boxsize.left += 4;
    dc->DrawText(_T("StringId"), &boxsize, DT_LEFT);

    PaintBmp(dc, boxsize.left + dc->GetTextExtent(_T("String Id")).cx + 8, boxsize.top + 4, IDB_DOWN_ARROW);
    dc->MoveTo(boxsize.right, boxsize.top);
    dc->LineTo(boxsize.right, boxsize.bottom);

    boxsize.left = boxsize.right + 4;
  
    text = project->mHeader.languages[0].name;

    if (mpFrame->GetTransLanguage() < 0)
    {
        boxsize.right = size.right;
    }
    else
    {
        boxsize.right = boxsize.left + framesize.Width() / 3 - 4;
    }
    dc->DrawText(text, &boxsize, DT_LEFT);

    // paint down arrow for sorting
    PaintBmp(dc, boxsize.left + dc->GetTextExtent(text).cx + 8, boxsize.top + 4, IDB_DOWN_ARROW);

    // paint right or left arrow to toggle three column mode

    if (mpFrame->GetTransLanguage() >= 0)
    {
        boxsize.left = boxsize.right + 1;
        boxsize.right = size.right;
        dc->MoveTo(boxsize.left, boxsize.top);
        dc->LineTo(boxsize.left, boxsize.bottom);
        PaintBmp(dc, boxsize.left + 1, boxsize.top + 4, IDB_RIGHT_ARROW);
        boxsize.left += 20;
        text = project->mHeader.languages[mpFrame->GetTransLanguage()].name;
        dc->DrawText(text, &boxsize, DT_LEFT);

        // paint left-right arrows for selecting trans language
        PaintBmp(dc, boxsize.right - (m_icon_width << 1) - 1, boxsize.top + 3, IDB_LEFT_ARROW);
        PaintBmp(dc, boxsize.right - m_icon_width - 1, boxsize.top + 3, IDB_RIGHT_ARROW);
    }
    else
    {
        PaintBmp(dc, boxsize.right - m_icon_width - 1, boxsize.top + 4, IDB_LEFT_ARROW);
    }
 
    dc->SelectObject(old_brush);
    dc->SelectObject(old_pen);
    dc->SelectObject(old_font);
}


///////////////////////////////////////////////////////////////////////////////
void table_header::OnPaint()
{
    CRect size;
    GetClientRect(&size);
    CDC *dc = GetDC();
    CWnd::OnPaint();

    PaintHeader(dc, size);
    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
void table_header::OnLButtonDown(UINT di, CPoint cp)
{
    CRect size;
    CRect framesize;
    int xpos;    
    GetClientRect(&size);
    mpFrame->GetClientRect(&framesize);

    if (mpFrame->IsThreeColumnMode())
    {
        xpos = size.left + ((framesize.Width() * 2) / 3);
        if (cp.x > xpos)
        {
            if (cp.x <= xpos + m_icon_width)
            {
                mpFrame->ToggleThreeColumnMode();
            }
            else
            {
                if (cp.x >= size.right - (m_icon_width << 1))
                {
                    if (cp.x >= size.right - m_icon_width)
                    {
                        mpFrame->IncrementTransLanguage();
                    }
                    else
                    {
                        mpFrame->DecrementTransLanguage();
                    }
                }
            }
        }
    }
    else
    {
        if (cp.x >= size.right - m_icon_width)
        {
            mpFrame->ToggleThreeColumnMode();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
table_row::table_row(CWnd *parent, int Id, int string_index, string_table *table)
{
    mpFrame = (table_frame *) parent;
    mRowId = Id;
    mStringIndex = string_index;
    mRowHeight = 0;
    mpTable = table;
}

///////////////////////////////////////////////////////////////////////////////
void table_row::PaintStringRow(CDC *dc, CRect &size)
{
    CPen line_pen;
    BOOL bSelected = FALSE;
    CString text;
    CBrush view_only(GetSysColor(COLOR_WINDOW));
    CBrush edit_brush(GetSysColor(COLOR_HIGHLIGHTTEXT));

    line_pen.CreatePen(PS_SOLID, 0, RGB(128, 128, 128));
    CPen *old_pen = dc->SelectObject(&line_pen);
    CBrush *old_brush = dc->SelectObject(&edit_brush);

    dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    dc->SetBkMode(TRANSPARENT);

    table_frame *frame = (table_frame *) GetParent();

    CFont *old_font = dc->SelectObject(frame->GetUniFont());

    if (mRowId == frame->GetSelectedId())
    {
        bSelected = TRUE;
    }

    // paint the string id name:
    CRect boxsize = size;
    boxsize.right = boxsize.left + boxsize.Width() / 3;

    if (bSelected)
    {
        CBrush select_brush;

        if (GetFocus() == this)
        {
            select_brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
            dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
        }
        else
        {
            select_brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
            dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

        }

        // background highlight for the selected row
        dc->FillRect(size, &select_brush);
    }
    else
    {
        dc->FillRect(boxsize, &edit_brush);
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    }
    boxsize.left += 4;
    dc->DrawText(mpTable->GetStringId(mStringIndex), &boxsize, DT_LEFT);
    dc->MoveTo(boxsize.right, size.top);
    dc->LineTo(boxsize.right, size.bottom -1);

    // draw the reference language string
    boxsize.left = boxsize.right + 1;

    if (frame->IsThreeColumnMode())
    {
        boxsize.right = boxsize.left + size.Width() / 3 - 1;
    }
    else
    {
        boxsize.right = size.right;
    }

    if (!bSelected)
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        if (frame->IsThreeColumnMode())
        {
            dc->FillRect(boxsize, &view_only);
        }
        else
        {
            dc->FillRect(boxsize, &edit_brush);
        }
    }

    text = mpTable->GetString(mStringIndex, 0);

    if (text.IsEmpty() == FALSE)
    {
        boxsize.left += 4;
#ifdef _UNICODE
        DrawTextW(*dc, text.GetString(), -1, &boxsize, DT_LEFT);
#else
        wchar_t *uni_text = CStringToWideChar(text);

        if (uni_text)
        {
            DrawTextW(*dc, uni_text, -1, &boxsize, DT_LEFT);
            delete [] uni_text;
        }
#endif
    }

    if (mpFrame->GetTransLanguage() >= 0)
    {
        boxsize.left = boxsize.right + 1;
        boxsize.right = size.right;

        // vertical line to divide columns
        dc->MoveTo(boxsize.left, boxsize.top);
        dc->LineTo(boxsize.left, boxsize.bottom);

        text = mpTable->GetString(mStringIndex, mpFrame->GetTransLanguage());

        if (text.IsEmpty() == FALSE)
        {
            boxsize.left += 4;
#ifdef _UNICODE
            DrawTextW(*dc, text.GetString(), -1, &boxsize, DT_LEFT);
#else
            wchar_t *uni_text = CStringToWideChar(text);

            if (uni_text)
            {
                DrawTextW(*dc, uni_text, -1, &boxsize, DT_LEFT);
                delete [] uni_text;
            }
#endif
        }
    }

    dc->MoveTo(size.left, size.top);
    dc->LineTo(size.left, size.bottom -1);
    dc->MoveTo(size.right - 1, size.top);
    dc->LineTo(size.right - 1, size.bottom -1);
    dc->MoveTo(size.left, size.bottom - 1);
    dc->LineTo(size.right, size.bottom - 1);
    dc->SelectObject(old_brush);
    dc->SelectObject(old_pen);
    dc->SelectObject(old_font);
}

///////////////////////////////////////////////////////////////////////////////
void table_row::OnPaint()
{
    CRect size;
    GetClientRect(&size);
    CDC *dc = GetDC();
    CWnd::OnPaint();

    PaintStringRow(dc, size);
    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
VOID table_row::OnSetFocus(CWnd *Wnd)
{
    CMainFrame *pMain = (CMainFrame*) AfxGetApp()->GetMainWnd();

     if (pMain)
     {
        CString msg = _T("");
        msg.Format(_T("String Id: %s. String text: %s"), mpTable->GetStringId(mStringIndex), mpTable->GetString(mStringIndex));

        // This works for Narrator, but not for NVDA
        pMain->GetStatusMsgControl()->SetWindowText(msg);
        pMain->GetStatusMsgControl()->NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);

        // And this works for NVDA - but not for Narrator
        SetAccessibleDescription(GetSafeHwnd(), msg);
    }
}

///////////////////////////////////////////////////////////////////////////////
void table_row::OnLButtonDown(UINT di, CPoint cp)
{
    CRect size;
    int column = 0;

    if (mRowId > 0)
    {
        table_frame *ps = (table_frame *) GetParent();

        GetClientRect(&size);
        if (cp.x > size.left + size.Width() / 3)
        {
            if (ps->IsThreeColumnMode())
            {
                column = 2;
            }
            else
            {
                column = 1;
            }
        }                
        ps->RowSelected(column, mRowId);
        Invalidate();
    }

    SetFocus();
}
        
///////////////////////////////////////////////////////////////////////////////
int table_row::GetRowHeight()
{
    CDC *dc = GetDC();

    table_frame *frame = (table_frame*)GetParent();

    CFont *old_font = dc->SelectObject(frame->GetUniFont());

    mRowHeight = dc->GetTextExtent(_T("A"), 1).cy + 5;

    dc->SelectObject(old_font);
    ReleaseDC(dc);
    return mRowHeight;
}

BEGIN_MESSAGE_MAP(string_reference_win, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
string_reference_win::string_reference_win(CWnd *parent, string_table *table, int string_index)
{
    mStringIndex = string_index;
    mpTable = table;
    mpDisplay = NULL;

    m_scroll_helper = new CScrollHelper();
    m_scroll_helper->AttachWnd(this);

    m_text_height = MulDiv(STRING_REFERENCE_ROW_TEXT_HEIGHT, GetSystemDPI(), DEFAULT_DPI_96);

    CreateDisplay();
}

///////////////////////////////////////////////////////////////////////////////
string_reference_win::~string_reference_win()
{
    if (m_scroll_helper)
    {
        delete m_scroll_helper;
    }

    if (mpDisplay)
    {
        resource_view::CleanupDisplayResources(mpDisplay);
        guix_studio_delete_display(mpDisplay);
        delete mpDisplay;
    }
}

///////////////////////////////////////////////////////////////////////////////
int string_reference_win::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    CreateStringReferenceRows();

    m_scroll_helper->SetDisplaySize(0, 0);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::OnDestroy()
{
    CWnd::OnDestroy();

    // TODO: Add your message handler code here
    CWnd *child = GetWindow(GW_CHILD);

    while (child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    m_scroll_helper->OnHScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}


///////////////////////////////////////////////////////////////////////////////
void string_reference_win::CreateStringReferenceRows( widget_info *info, int string_id)
{
    string_reference_row *row;
    widget_info *child;
    int string_count;
    GX_RESOURCE_ID *string_list;

    while (info)
    {
        if (info->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
        {
            string_list = info->ewi.string_scroll_wheel.string_id_list;
            string_count = info->ewi.string_scroll_wheel.base.total_rows;
        }
        else
        {
            string_list = info->string_id;
            string_count = NUM_WIDGET_STRINGS;
        }
        if (string_list)
        {
            for (int index = 0; index < string_count; index++)
            {
                if (string_id && (string_list[index] == string_id))
                {
                    row = new string_reference_row(this, info, string_id);
                    row->Create(target_class_name, _T("string_reference_row"), WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, 0);
                    row->MakeWidgetPreview();
                    break;
                }
            }
        }
        child = info->GetChildWidgetInfo();

        if (child)
        {
            CreateStringReferenceRows(child, string_id);
        }

        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::CreateStringReferenceRows()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview || mStringIndex < 0)
    {
        return;
    }

    CWnd *child = GetWindow(GW_CHILD);

    while (child)
    {
        child->DestroyWindow();
        delete child;
        child = GetWindow(GW_CHILD);
    }

    CString id_name = mpTable->GetStringId(mStringIndex);
    int string_id = mpTable->GetResourceId(id_name);

    //get active display index
    int display_index = display_index = pview->GetActiveDisplay();

    folder_info *folder = project->mDisplays[display_index].GetFirstChildFolder();
    widget_info *info;

    //find widgets that reference the selected string
    while (folder)
    {
        info = folder->GetFirstChildWidget();

        CreateStringReferenceRows(info, string_id);
        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::PositionChildren()
{
    CRect childsize;
    CRect winrect;
    string_reference_row *row;
    GX_RECTANGLE size;
    int row_height;
    int row_width;
    int display_height = 0;
    int display_width = 0;

    CDC *dc = GetWindowDC();

    string_table_edit_dlg* dlg = (string_table_edit_dlg*)GetParent();
    CFont *oldFont = dlg->GetDialogFont();
    CSize text_extent;

    m_scroll_helper->ScrollToOrigin(TRUE, TRUE);

    // calculate maximum display width and display height
    row = (string_reference_row *)GetWindow(GW_CHILD);
    while (row)
    {
        size = row->GetWidgetPreviewSize();

        row_width = (size.gx_rectangle_right - size.gx_rectangle_left + 1);
        row_height = (size.gx_rectangle_bottom - size.gx_rectangle_top + 1);

        text_extent = dc->GetTextExtent(row->GetInfoText());

        if (text_extent.cx > row_width)
        {
            row_width = text_extent.cx;
        }

        if (row_width > display_width)
        {
            display_width = row_width;
        }

        display_height += row_height + 2 + m_text_height;

        row = (string_reference_row *)row->GetWindow(GW_HWNDNEXT);
    }

    //if display width + 4 < client width, make the display width = client width - 4
    GetClientRect(&winrect);

    int cxSB = ::GetSystemMetrics(SM_CXVSCROLL);
    int cySB = ::GetSystemMetrics(SM_CYHSCROLL);

    int style = GetStyle();
    if (style & WS_HSCROLL)
    {
        winrect.bottom += cySB;
    }

    if (style & WS_VSCROLL)
    {
        winrect.right += cxSB;
    }

    if (display_height + 2 > winrect.Height())
    {
        int cxSB = ::GetSystemMetrics(SM_CXVSCROLL);

        if (display_width + 4 < winrect.Width() - cxSB)
        {
            display_width = winrect.Width() - cxSB - 4;
        }
    }
    else
    {
        if (display_width + 4 < winrect.Width())
        {
            display_width = winrect.Width() - 4;
        }
    }


    //position children
    childsize.bottom = 0;
    childsize.left += 2;

    row = (string_reference_row *)GetWindow(GW_CHILD);
    while (row)
    {
        size = row->GetWidgetPreviewSize();

        row_height = (size.gx_rectangle_bottom - size.gx_rectangle_top + 1);

        childsize.top = childsize.bottom + 2;
        childsize.bottom = childsize.top + row_height + m_text_height;
        childsize.right = childsize.left + display_width;

        row->MoveWindow(&childsize);

        row = (string_reference_row *)row->GetWindow(GW_HWNDNEXT);
    }

    dc->SelectObject(oldFont);

    m_scroll_helper->SetDisplaySize(display_width + 4, display_height + 2);
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::CreateDisplay()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview)
    {
        return;
    }

    if (!mpDisplay)
    {
        // create a temporary screen and temporary
        // canvas to make a bitmap:
        GX_CANVAS *target_canvas = get_target_win_canvas();

        int canvas_width = target_canvas->gx_canvas_x_resolution;
        int canvas_height = target_canvas->gx_canvas_y_resolution;

        int active_display = pview->GetActiveDisplay();
        int active_theme = project->mDisplays[active_display].active_theme;

        display_info *info = &project->mDisplays[active_display];

        mpDisplay = new GX_DISPLAY;

        int memsize;

        memsize = guix_create_app_display(mpDisplay, "string_test_display",
                  canvas_width, canvas_height,
                  info->colorformat,
                  project->mHeader.target_cpu,
                  IsRenesasDave2D(project),
                  IsDave2dFontFormat(project, active_display),
                  info->themes[active_theme].palette,
                  info->themes[active_theme].palette_total_size,
                  project->mHeader.palette_mode_aa_text_colors);

        if (memsize)
        {
            GetResourceView()->BuildResourceTables(active_display, mpDisplay, FALSE);
            string_table::EnableDisableRuntimeBidiText(mpDisplay->gx_display_active_language);
        }
        else
        {
            delete mpDisplay;
            mpDisplay = GX_NULL;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::UpdateResourceTable()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview)
    {
        return;
    }

    int active_display = pview->GetActiveDisplay();
    GetResourceView()->BuildResourceTables(active_display, mpDisplay, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::SetActiveLanguage(int language)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    gx_display_active_language_set(mpDisplay, language);
    string_table::EnableDisableRuntimeBidiText(language);

    StringSelected(mStringIndex);
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_win::StringSelected(int string_index)
{
    if (mStringIndex != string_index)
    { 
        mStringIndex = string_index;

        CreateStringReferenceRows();
        PositionChildren();
    }
    else
    {
        CWnd *child = GetWindow(GW_CHILD);

        while (child)
        {
            ((string_reference_row *)child)->MakeWidgetPreview();
            child = child->GetWindow(GW_HWNDNEXT);
        }
    }

    Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
CFont *string_reference_win::GetDialogFont()
{
    string_table_edit_dlg *dlg = (string_table_edit_dlg*)GetParent();
    return dlg->GetDialogFont();
}

BEGIN_MESSAGE_MAP(string_reference_row, CWnd)
ON_WM_PAINT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
string_reference_row::string_reference_row(CWnd *parent, widget_info *info, int string_id)
{
    mpInfo = info;
    mStringId = string_id;
    mpWidget = NULL;
    mpCanvas = NULL;
    mpRoot = NULL;
    mInfoText = info->app_name;

    int dpi = GetSystemDPI();
    m_text_height = MulDiv(STRING_REFERENCE_ROW_TEXT_HEIGHT, dpi, DEFAULT_DPI_96);
    m_preview_size.gx_rectangle_left = MulDiv(info->size.gx_rectangle_left, dpi, DEFAULT_DPI_96);
    m_preview_size.gx_rectangle_top = MulDiv(info->size.gx_rectangle_top, dpi, DEFAULT_DPI_96);
    m_preview_size.gx_rectangle_right = MulDiv(info->size.gx_rectangle_right, dpi, DEFAULT_DPI_96);
    m_preview_size.gx_rectangle_bottom = MulDiv(info->size.gx_rectangle_bottom, dpi, DEFAULT_DPI_96);

    studiox_project *project = GetOpenProject();
    project_view *pView = GetProjectView();

    if (project || pView)
    {
        widget_info *parent = NULL; 
        parent = pView->FindTopLevelWidget(info);

        if (parent && (parent != info))
        {
            mInfoText.Format(_T("%s.%s"), parent->app_name, mpInfo->app_name);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
string_reference_row::~string_reference_row()
{
    if (mpWidget)
    {
        if (mpWidget->gx_widget_type == GX_TYPE_STRING_SCROLL_WHEEL)
        {
            GX_STRING_SCROLL_WHEEL *wheel = (GX_STRING_SCROLL_WHEEL *)mpWidget;

            if (wheel->gx_string_scroll_wheel_string_id_list)
            {
                delete wheel->gx_string_scroll_wheel_string_id_list;
                wheel->gx_string_scroll_wheel_string_id_list = NULL;
            }
        }
        widget_factory::DeleteWidgets(mpWidget);
    }

    if (mpRoot)
    {
        gx_window_root_delete(mpRoot);
        delete mpRoot;
    }

    if (mpCanvas)
    {
        if (mpCanvas->gx_canvas_memory)
        {
            delete mpCanvas->gx_canvas_memory;
        }
        gx_canvas_delete(mpCanvas);
        delete mpCanvas;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_reference_row::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages

    if (mpInfo)
    {
        CRect client;
        CRect rect;
        GetClientRect(&client);

        if (mpCanvas)
        {
            string_reference_win *parent = (string_reference_win *)GetParent();
            CFont *oldFont = dc.SelectObject(parent->GetDialogFont());
            dc.SetBkMode(TRANSPARENT);
            dc.SetTextColor(NORMAL_TEXT_COLOR);

            rect = client;
            rect.bottom = client.top + m_text_height - 2;
            dc.DrawText(mInfoText, &rect, DT_SINGLELINE | DT_VCENTER);
            dc.SelectObject(oldFont);

            UCHAR *memptr = (UCHAR *)_win32_canvas_memory_prepare(mpCanvas, &mpWidget->gx_widget_size);

            //Draw widget preview
            int des_width = (m_preview_size.gx_rectangle_right - m_preview_size.gx_rectangle_left + 1);
            int des_height = (m_preview_size.gx_rectangle_bottom - m_preview_size.gx_rectangle_top + 1);

            int src_width = m_bitmap.gx_bmp_header.biWidth;
            int src_height = m_bitmap.gx_bmp_header.biHeight;

            SetMapMode(dc.GetSafeHdc(), MM_TEXT);

            StretchDIBits(dc.GetSafeHdc(), client.left, client.top + m_text_height - 2, des_width, des_height,
                0, src_height + 1, src_width, -src_height, memptr,
                (BITMAPINFO*)&m_bitmap,
                DIB_RGB_COLORS,
                SRCCOPY);

            CPen brush(PS_SOLID, 1, LINE_COLOR);
            CPen *oldPen = dc.SelectObject(&brush);
            dc.MoveTo(client.left, client.bottom - 1);
            dc.LineTo(client.right, client.bottom - 1);
            dc.SelectObject(oldPen);
        }
    }
}

/**************************************************************************/
GX_WIDGET *one_app_widget_create(widget_info *info)
{
    GX_WIDGET *created = NULL;
    widget_info *child;

    child = info->GetChildWidgetInfo();
    info->SetChildWidgetInfo(NULL);
    created = widget_factory::GenerateAppScreen(NULL, info);
    info->SetChildWidgetInfo(child);
    return created;
}


///////////////////////////////////////////////////////////////////////////////
void string_reference_row::MakeWidgetPreview()
{
    studiox_project *project = GetOpenProject();
    project_view *pview = GetProjectView();

    if (!project || !pview)
    {
        return;
    }

    string_reference_win *parent = (string_reference_win *)GetParent();
    GX_DISPLAY *display = parent->GetDisplay();

    if (!display)
    {
        return;
    }

    int active_display = pview->GetActiveDisplay();

    if (!mpWidget)
    {
        int widget_width = mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1;
        int widget_height = mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1;
        GX_RECTANGLE fill_rect;
        gx_utility_rectangle_define(&fill_rect, 0, 0, widget_width - 1, widget_height - 1);

        int bpp = project->mDisplays[active_display].bits_per_pix;
        switch (bpp)
        {
        case 24:
            bpp = 32;
            break;
        }

        //calculate memory size
        int  memsize = widget_width * widget_height * bpp;
        memsize >>= 3;

        UCHAR *mem =  new UCHAR[memsize];
        mpCanvas = new GX_CANVAS;
        mpRoot = new GX_WINDOW_ROOT;

        //create canvas and root window for widget drawing
        gx_canvas_create(mpCanvas, "string_test_canvas",
            display, GX_CANVAS_SIMPLE,
            widget_width, widget_height, (GX_COLOR *)mem, memsize);

        gx_window_root_create(mpRoot, "string_test_root", mpCanvas,
            GX_STYLE_BORDER_NONE, GX_ID_NONE, &fill_rect);

        mpWidget = one_app_widget_create(mpInfo);

        if (mpWidget)
        {
            gx_widget_attach(mpRoot, mpWidget);

            gx_widget_style_remove(mpWidget, GX_STYLE_ENABLED);
            gx_widget_style_add(mpWidget, GX_STYLE_TRANSPARENT);

            if (mpWidget->gx_widget_type == GX_TYPE_STRING_SCROLL_WHEEL)
            {
                GX_STRING_SCROLL_WHEEL *wheel = (GX_STRING_SCROLL_WHEEL *)mpWidget;

                int list_count = mpInfo->ewi.string_scroll_wheel.base.total_rows;
                GX_RESOURCE_ID *id_list = new GX_RESOURCE_ID[list_count];
                for (int index = 0; index < list_count; index++)
                {
                    id_list[index] = mStringId;
                }
                wheel->gx_string_scroll_wheel_string_id_list = id_list;
            }

            int xpos = mpInfo->size.gx_rectangle_left;
            int ypos = mpInfo->size.gx_rectangle_top;

            if (xpos || ypos)
            {
                gx_widget_shift(mpWidget, -xpos, -ypos, FALSE);
            }

            GX_WIN32_DISPLAY_DRIVER_DATA *data = (GX_WIN32_DISPLAY_DRIVER_DATA *)target_win_display.gx_display_driver_data;
            m_bitmap = data->win32_driver_bmpinfo;
            m_bitmap.gx_bmp_header.biWidth = widget_width;
            m_bitmap.gx_bmp_header.biHeight = widget_height;
        }
    }

    if (mpWidget)
    {
        GX_CHAR *text;
        switch (mpInfo->basetype)
        {
        case GX_TYPE_MULTI_LINE_TEXT_INPUT:
        case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
            text = new GX_CHAR[mpInfo->ewi.text_info.buffer_size + 1];
            widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);
            if (provider)
            {
                provider->GetStringText((char*)text, mpInfo->ewi.text_info.buffer_size, mpInfo);
            }
            GX_STRING string;
            string.gx_string_ptr = text;
            string.gx_string_length = strlen(text);
            if (mpInfo->basetype == GX_TYPE_MULTI_LINE_TEXT_INPUT)
            {
                gx_multi_line_text_input_text_set_ext((GX_MULTI_LINE_TEXT_INPUT *)mpWidget, &string);
            }
            else
            {
                gx_single_line_text_input_text_set_ext((GX_SINGLE_LINE_TEXT_INPUT *)mpWidget, &string);
            }
            delete [] text;
            break;
        }
        //Draw widget
        gx_canvas_drawing_initiate(mpCanvas, NULL, &mpWidget->gx_widget_size);

        GX_BRUSH *brush;
        gx_context_brush_get(&brush);

        int old_brush_width = brush->gx_brush_width;
        int old_brush_style = brush->gx_brush_style;
        brush->gx_brush_style |= GX_BRUSH_SOLID_FILL;
        brush->gx_brush_width = 0;
        gx_context_raw_fill_color_set(GX_COLOR_LIGHTGRAY);
        gx_canvas_rectangle_draw(&mpWidget->gx_widget_size);

        brush->gx_brush_width = old_brush_width;
        brush->gx_brush_style = old_brush_style;
        if (display)
        {
            int text_color_id = mpInfo->color_id[DISABLED_TEXT_COLOR_INDEX];
            gx_display_color_set(display, text_color_id, GX_COLOR_BLACK);
        }

        gx_widget_show(mpWidget);
        mpWidget->gx_widget_draw_function(mpWidget);
        gx_widget_hide(mpWidget);

        gx_canvas_drawing_complete(mpCanvas, FALSE);
    }
}


///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    PaintRequiredAsterisk(ID_ID_EDIT);
    PaintRequiredAsterisk(IDC_REQUIRED_FIELD_LEGEND);
}

///////////////////////////////////////////////////////////////////////////////
INT_PTR string_table_edit_dlg::DoModal()
{
    int new_text_scaler = GetTextScaler();
    if (m_text_scaler != new_text_scaler)
    {
        SetControlDimensions(new_text_scaler);
    }

    return express_dialog::DoModal();
}

///////////////////////////////////////////////////////////////////////////////
void string_table_edit_dlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    int new_text_scaler = GetTextScaler();
    if (m_text_scaler != new_text_scaler)
    {
        SetControlDimensions(new_text_scaler);
        mpButtonFrame->UpdateControlDimensions(new_text_scaler);
    }

    express_dialog::OnSettingChange(uFlags, lpszSection);
}
// font_path_dialog.cpp : implementation file
//

//#include "stdafx.h"
#include "studiox_includes.h"
#include "font_path_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_START_FIELD 4096
#define ID_END_FIELD   4097
#define ID_ENABLE_BOX  4098

#define RANGE_ROW_HEIGHT 26

BEGIN_MESSAGE_MAP(font_path_dialog, express_dialog)
    ON_BN_CLICKED(IDC_CUSTOM_OUTPUT, &font_path_dialog::OnBnClickedCustomOutput)
    ON_BN_CLICKED(IDC_BROWSE, &font_path_dialog::OnBnClickedBrowse)
    ON_BN_CLICKED(IDCANCEL, &font_path_dialog::OnCancel)
    ON_BN_CLICKED(IDOK, &font_path_dialog::OnBnClickedOk)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDB_PATH_TYPE, &font_path_dialog::OnBnClickedPathType)
    ON_COMMAND(ID_PATHTYPE_PROJECTRELATIVE, SetPathProjectRelative)
    ON_COMMAND(ID_PATHTYPE_STUDIORELATIVE, SetPathStudioRelative)
    ON_COMMAND(ID_PATHTYPE_ABSOLUTE, SetPathAbsolute)
    ON_EN_KILLFOCUS(IDC_FONT_PATH, OnKillFontPathEdit)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_SUPPORT_EXTENDED_UNICODE, &font_path_dialog::OnBnClickedSupportExtendedUnicode)
    ON_EN_KILLFOCUS(IDC_FONT_HEIGHT, &font_path_dialog::OnEnChangeFontHeight)
    ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(range_list_win, CWnd)
    ON_WM_CREATE()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(font_range_row, CWnd)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_BN_CLICKED(ID_ENABLE_BOX, OnChangeEnabled)
    ON_EN_KILLFOCUS(ID_START_FIELD, OnEditRangeField)
    ON_EN_KILLFOCUS(ID_END_FIELD, OnEditRangeField)
END_MESSAGE_MAP()

extern CString target_class_name;

// font_path_dialog dialog
 
//IMPLEMENT_DYNAMIC(font_path_dialog, CDialog)

typedef struct {
    int first_char;
    int last_char;
    char *name;
} GLYPH_RANGE;

///////////////////////////////////////////////////////////////////////////////
GLYPH_RANGE DefaultGlyphRanges[NUM_FONT_CHAR_RANGES + 1] = {
    { 0x0020,  0x007e,  "Ascii"},
    {0x0080,  0x017f,   "Latin Extended-A"},
    {0x0180,  0x0217,   "Latin Extended-B"},
    {0x0250,  0x02ab,   "IPA Extensions"},
    {0x02b0,  0x02e9,   "Spacing Modifiers"},
    {0x0300,  0x0345,   "Combining Diacritics"},
    {0x0374,  0x03f3,   "Greek and Coptic"},
    {0x0401,  0x04f9,   "Cyrillic"},
    {0x0531,  0x0589,   "Armenian"},
    {0x0591,  0x05f4,   "Hebrew"},
    {0x0600,  0x06ff,   "Arabic"},
    {0x0700,  0x074a,   "Syriac"},
    {0x0780,  0x07b0,   "Thaana"},
    {0x0901,  0x0970,   "Devanagari"},
    {0x0981,  0x09fa,   "Bengali"},
    {0x0a02,  0x0a74,   "Gurmukhi"},
    {0x0a81,  0x0aef,   "Gujarati"},
    {0x0b01,  0x0b70,   "Oriya"},
    {0x0b82,  0x0bf2,   "Tamil"},
    {0x0c01,  0x0c6f,   "Telugu"},
    {0x0c82,  0x0cef,   "Kannada"},
    {0x0d02,  0x0d6f,   "Malayalam"},
    {0x0d80,  0x0df4,   "Sinhala"},
    {0x0e01,  0x0e5b,   "Thai"},
    {0x0e81,  0x0edd,   "Lao"},
    {0x0f00,  0x0fb9,   "Tibetan"},
    {0x1000,  0x1059,   "Myanmar"},
    {0x10a0,  0x10fb,   "Georgian"},
    {0x1100,  0x11f9,   "Hangul Jamo"},
    {0x1800,  0x18a9,   "Mongolian"},
    {0x1e00,  0x1ef9,   "Latin Ext. Additional"},
    {0x1f00,  0x1ffe,   "Greek Extended"},
    {0x2000,  0x2046,   "General Punctuation"},
    {0x20a0,  0x20ac,   "Currency Symbols"},
    {0x2100,  0x2138,   "Letterlike Symbols"},
    {0x2153,  0x2182,   "Number Forms"},
    {0x2200,  0x22f1,   "Mathematical Operators"},
    {0x3041,  0x309e,   "Hiragana"},
    {0x30a1,  0x30fe,   "Katakana"},
    {0x3105,  0x312c,   "Bopomofo"},
    {0x3131,  0x318e,   "Hangul Jamo"},
    {0x3190,  0x319f,   "Kanbun"},
    {0x3300,  0x33fe,   "CJK Compatibility"},
    {0x4e00,  0x9fb0,   "Kanji Ideographs"},
    {0xac00,  0xd7af,   "Hangul Syllables"},
    {0xf900,  0xfa2d,   "Kanji Composites"},
    {-1, -1, NULL}
};

///////////////////////////////////////////////////////////////////////////////
GLYPH_RANGE ExtendedGlyphRanges[NUM_FONT_EXTENDED_CHAR_RANGES + 1] = {
    { 0, 0, "User Defined Page1" },
    { 0, 0, "User Defined Page2" },
    { 0, 0, "User Defined Page3" },
    { 0, 0, "User Defined Page4" },
    {-1, -1, NULL}
};

enum font_path_dlg_test_commands{
    TEST_SET_FONT_PATH = 1,
    TEST_SET_FONT_NAME,
    TEST_SET_FONT_HEIGHT,
    TEST_SET_FONT_HEIGHT_POST,
    TEST_SET_FONT_FORMAT,
    TEST_SET_FONT_COMPRESSION,
    TEST_SET_FONT_GENERATE_KERNING,
    TEST_SET_PAGE_RANGE,
    TEST_CHECK_EXTENDED_UNICODE,
    TEST_CHECK_CUSTOM_OUTPUT,
    TEST_SET_CUSTOM_OUTPUT_FILE,
    TEST_SAVE_FONT_EDIT,
    TEST_CANCEL_FONT_EDIT
};

BEGIN_MESSAGE_MAP(font_range_header, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
void font_range_header::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CWnd::OnPaint() for painting messages

    CRect client;
    GetClientRect(&client);

    CBrush brush(GetSysColor(CTLCOLOR_DLG));
    CPen line_pen;
    
    line_pen.CreatePen(PS_SOLID, 0, GetSysColor(COLOR_WINDOWFRAME));

    CPen* old_pen = dc.SelectObject(&line_pen);
    CBrush* old_brush = dc.SelectObject(&brush);
    dc.SelectObject(mpFont);

    dc.FillRect(&client, &brush);
    dc.MoveTo(client.left, client.top);
    dc.LineTo(client.right, client.top);
    dc.MoveTo(client.left, client.bottom - 1);
    dc.LineTo(client.right, client.bottom - 1);
    dc.MoveTo(client.left, client.top);
    dc.LineTo(client.left, client.bottom - 1);
    dc.MoveTo(client.right - 1, client.top);
    dc.LineTo(client.right - 1, client.bottom - 1);

    dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    dc.SetBkMode(TRANSPARENT);

    int width = (client.Width() - ::GetSystemMetrics(SM_CXVSCROLL)) / 3;

    CRect size = client;
    size.right = width;

    CSize sz = dc.GetTextExtent(_T("Name"));

    CRect rect = size;
    rect.top += (size.Height() - sz.cy) / 2;
    dc.DrawText(_T("Font Page"), rect, DT_CENTER);
    dc.MoveTo(size.right, size.top);
    dc.LineTo(size.right, size.bottom);

    size.OffsetRect(CPoint(width, 0));
    rect = size;
    rect.top += (size.Height() - sz.cy) / 2;

    dc.DrawText(_T("<U> Page Start"), rect, DT_CENTER);
    dc.MoveTo(size.right, size.top);
    dc.LineTo(size.right, size.bottom);

    size.OffsetRect(CPoint(width, 0));
    rect = size;
    rect.top += (size.Height() - sz.cy) / 2;

    dc.DrawText(_T("<U> Page End"), rect, DT_CENTER);

    dc.SelectObject(old_brush);
    dc.SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
range_list_win::range_list_win(res_info *info, CFont *font)
{
    m_info = info;
    mpFont = font;

    page_count = NUM_FONT_CHAR_RANGES;
    if (info->font_support_extended_unicode)
    {
        page_count += NUM_FONT_EXTENDED_CHAR_RANGES;
    }
    memcpy_s(mPageInfo, sizeof(mPageInfo), info->font_pages, sizeof(font_page_info) * page_count);

    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);

    int dpi = GetSystemDPI();

    m_range_row_height = MulDiv(RANGE_ROW_HEIGHT, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
range_list_win::~range_list_win()
{
    for (int loop = 0; loop < page_count; loop++)
    {
        delete range_entry[loop];
    }

    delete m_scroll_helper;
}

///////////////////////////////////////////////////////////////////////////////
int range_list_win::OnCreate(LPCREATESTRUCT lpcs)
{
    CWnd::OnCreate(lpcs);

    GLYPH_RANGE *range = DefaultGlyphRanges;

    CRect RangeListEntrySize;
    GetClientRect(&RangeListEntrySize);

    RangeListEntrySize.bottom = RangeListEntrySize.top + m_range_row_height;

    CString name;

    for (int loop = 0; loop < NUM_FONT_CHAR_RANGES; loop++)
    {
        name.Format(_T("%s range"), CString(range->name));
        range_entry[loop] = new font_range_row(CString(range->name), 
            &mPageInfo[loop], FALSE, loop, mpFont);
        range_entry[loop]->Create(target_class_name, name, WS_CHILD | WS_VISIBLE,
            RangeListEntrySize, this, 0, NULL);

        RangeListEntrySize.OffsetRect(0, m_range_row_height);
        range++;
    }

    if (m_info->font_support_extended_unicode)
    {
        CreateExtendedRangeEntry();
    }

    // update scrollbar info
    m_scroll_helper->SetDisplaySize(0, page_count * m_range_row_height);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void range_list_win::CreateExtendedRangeEntry()
{
    GLYPH_RANGE *range = ExtendedGlyphRanges;
    memcpy_s(mPageInfo + NUM_FONT_CHAR_RANGES, sizeof(font_page_info) * NUM_FONT_EXTENDED_CHAR_RANGES, m_info->font_pages + NUM_FONT_CHAR_RANGES, sizeof(font_page_info) * NUM_FONT_EXTENDED_CHAR_RANGES);

    CRect childsize;
    GetClientRect(&childsize);

    childsize.top -= (m_scroll_helper->GetScrollPos()).cy;
    childsize.top += NUM_FONT_CHAR_RANGES * m_range_row_height;
    childsize.bottom = childsize.top + m_range_row_height - 1;

    page_count = NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES;

    CString name;

    // Create extended range entries
    for (int loop = NUM_FONT_CHAR_RANGES; loop < page_count; loop++)
    {
        name.Format(_T("%s range"), CString(range->name));
        range_entry[loop] = new font_range_row(CString(range->name), &mPageInfo[loop], TRUE, loop, mpFont);
        range_entry[loop]->Create(target_class_name, name, WS_CHILD | WS_VISIBLE,
            childsize, this, 0, NULL);
        range++;

        childsize.top += m_range_row_height;
        childsize.bottom += m_range_row_height;
    }

    m_scroll_helper->SetDisplaySize(0, page_count * m_range_row_height);
}

///////////////////////////////////////////////////////////////////////////////
void range_list_win::DeleteExtendedRangeEntry()
{
    // Delete extended range entries
    for (int loop = NUM_FONT_CHAR_RANGES; loop < page_count; loop++)
    {
        delete range_entry[loop];
    }

    page_count = NUM_FONT_CHAR_RANGES;

    m_scroll_helper->SetDisplaySize(0, page_count * m_range_row_height);
}

///////////////////////////////////////////////////////////////////////////////
BOOL range_list_win::Scroll(int delta)
{
    return m_scroll_helper->Scroll(SB_VERT, delta);
}

///////////////////////////////////////////////////////////////////////////////
void range_list_win::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Compute the desired change or delta in scroll position.
    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL range_list_win::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    BOOL wasScrolled = m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
    return wasScrolled;
}


void range_list_win::SaveRangeInfo()
{
    memcpy_s(m_info->font_pages, sizeof(font_page_info) * page_count, mPageInfo, sizeof(font_page_info) * page_count);
}

void range_list_win::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    //pick first row
    CWnd* child = GetWindow(GW_CHILD);
    CRect list_client;
    CRect rect;

    GetClientRect(&list_client);
    ClientToScreen(&list_client);

    // set focus to the first visible child
    while (child)
    {
        child->GetWindowRect(&rect);

        if (rect.top < list_client.top)
        {
            child = child->GetWindow(GW_HWNDNEXT);
        }
        else
        {
            //pick first child of the row
            child = child->GetWindow(GW_CHILD);

            if (child)
            {
                child->SetFocus();
            }
            break;
        }
    }
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// font_range_entry- the CWnd class that implements each row of glyph range table
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
font_range_row::font_range_row(CString &name, font_page_info *page_info, BOOL extended, INT row_id, CFont *font)
{
    mNameText = name;
    mpInfo = page_info;
    mExtended = extended;
    mRowId = row_id;
    mpFont = font;
}

///////////////////////////////////////////////////////////////////////////////
int font_range_row::OnCreate(LPCREATESTRUCT lpcs)
{
    CRect size;
    CRect fieldsize;
    CString tval;
    CString name;

    express_table_row::OnCreate(lpcs);
    GetClientRect(size);
    fieldsize = size;
    fieldsize.top++;
    fieldsize.bottom--;

    int width = size.Width() / 3;
    fieldsize.left += 4;
    fieldsize.right = width;

    mEnabledBox.Create(NULL, WS_CHILD|WS_TABSTOP|BS_AUTOCHECKBOX|WS_VISIBLE, fieldsize, this, ID_ENABLE_BOX);
    mEnabledBox.SetFont(mpFont);
    mEnabledBox.SetWindowText(mNameText);
    name.Format(_T("row %d, column 0, %s"), mRowId, mNameText);
    SetControlAccessibleName(mEnabledBox.GetSafeHwnd(), name);

    if (mpInfo->enabled)
    {
        mEnabledBox.SetCheck(TRUE);
    }

    fieldsize.left = fieldsize.right + 3;
    fieldsize.right = fieldsize.left + width - 3;
    mStartField.Create(WS_BORDER|WS_CHILD|WS_TABSTOP|SS_CENTER|ES_AUTOHSCROLL|WS_VISIBLE, fieldsize, this, ID_START_FIELD);
    mStartField.SetFont(mpFont);
    if (!mExtended)
    {
        tval.Format(_T("%04x"), mpInfo->first_char);
    }
    else
    {
        tval.Format(_T("%06x"), mpInfo->first_char);
    }
    mStartField.SetWindowText(tval);

    name.Format(_T("row %d, column 1, %s Page Start"), mRowId, mNameText);
    SetControlAccessibleName(mStartField.GetSafeHwnd(), name);
    SetAccessibleHelpString(mStartField.GetSafeHwnd(), _T("Enter a Unicode value."));

    fieldsize.OffsetRect(CPoint(width, 0));
    mEndField.Create(WS_BORDER|WS_CHILD|WS_TABSTOP|SS_CENTER|ES_AUTOHSCROLL|WS_VISIBLE, fieldsize, this, ID_END_FIELD);
    mEndField.SetFont(mpFont);
    if (!mExtended)
    {
        tval.Format(_T("%04x"), mpInfo->last_char);
    }
    else
    {
        tval.Format(_T("%06x"), mpInfo->last_char);
    }
    mEndField.SetWindowText(tval);

    name.Format(_T("row %d, column 2, %s Page End"), mRowId, mNameText);
    SetControlAccessibleName(mEndField.GetSafeHwnd(), name);
    SetAccessibleHelpString(mEndField.GetSafeHwnd(), _T("Enter a Unicode value."));

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void font_range_row::OnPaint()
{
    express_table_row::OnPaint();
}

///////////////////////////////////////////////////////////////////////////////
BOOL font_range_row::OnCommand(WPARAM wParam, LPARAM lParam)
{
    if (wParam == IDOK)
    {
        CWnd *child = GetFocus();

        if (child)
        {
            SetFocus();
            child->SetFocus();
            return TRUE;
        }
    }
    return express_table_row::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
void font_range_row::OnChangeEnabled()
{
    if (mEnabledBox.GetCheck())
    {
        mpInfo->enabled = TRUE;
    }
    else
    {
        mpInfo->enabled = FALSE;
    } 
}

///////////////////////////////////////////////////////////////////////////////
void font_range_row::OnEditRangeField()
{
    int val;
    CString text;

    mStartField.GetWindowText(text);
    val = _tcstol(text.GetBuffer(), NULL, 16);

    if (!mExtended)
    {
        if (val >= 0 && val <= mpInfo->last_char)
        {
            mpInfo->first_char = val;
        }
        else
        {
            text.Format(_T("%04x"), mpInfo->first_char);
            mStartField.SetWindowText(text);
        }
    }
    else
    {
        if (val >= 0 && val <= 0x10ffff)
        {
            mpInfo->first_char = val;
        }
        else
        {
            text.Format(_T("%06x"), mpInfo->first_char);
            mStartField.SetWindowText(text);
        }
    }

    mEndField.GetWindowText(text);
    val = _tcstol(text.GetBuffer(), NULL, 16);

    if (!mExtended)
    {
        if (val >= mpInfo->first_char && val < 0xffff)
        {
            mpInfo->last_char = val;
        }
        else
        {
            text.Format(_T("%04x"), mpInfo->last_char);
            mEndField.SetWindowText(text);
        }
    }
    else
    {
        if (val >= 0 && val <= 0x10ffff)
        {
            mpInfo->last_char = val;
        }
        else
        {
            text.Format(_T("%06x"), mpInfo->last_char);
            mEndField.SetWindowText(text);
        }
    }
}

BOOL font_range_row::PreTranslateKeyDown(WPARAM keyvalue)
{
    CWnd* focus_owner = GetFocus();
    CWnd* next_focus_owner = NULL;
    range_list_win* list = (range_list_win*)GetParent();
    CRect list_client;
    CRect row_client;

    list->GetClientRect(&list_client);
    GetClientRect(&row_client);

    switch (keyvalue)
    {
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_HOME:
    case VK_END:
        next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue);
        break;

    case VK_PRIOR:
        if (list->Scroll(-list_client.Height()))
        {
            int offset = list_client.Height() / row_client.Height();

            if (mRowId - offset < 0)
            {
                offset = mRowId;
            }

            next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue, offset);
        }
        break;

    case VK_NEXT:
        if(list->Scroll(list_client.Height()))
        {
            int offset = list_client.Height() / row_client.Height();
            int max_row_id = NUM_FONT_CHAR_RANGES - 1;
            if (mExtended)
            {
                max_row_id += NUM_FONT_EXTENDED_CHAR_RANGES;
            }

            if (mRowId + offset > max_row_id)
            {
                offset = max_row_id - mRowId;
            }
            next_focus_owner = GetNextTabStopChild(focus_owner, keyvalue, offset);
        }
        break;

    default:
        return FALSE;
    }

    if (next_focus_owner)
    {
        CRect rect;

        list->GetClientRect(&list_client);
        list->ClientToScreen(&list_client);
        
        next_focus_owner->GetWindowRect(&rect);

        if (rect.top < list_client.top)
        {
            list->Scroll(rect.top - list_client.top);
        }
        else if (rect.bottom > list_client.bottom)
        {
            list->Scroll(rect.bottom - list_client.bottom);
        }

        AssignFocus(next_focus_owner);
    }

    return TRUE;
}

BOOL font_range_row::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN)
    {
        if (PreTranslateKeyDown(pMsg->wParam))
        {
            return TRUE;
        }
    }

    return express_table_row::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////
font_page_info *font_path_dialog::CreateDefaultFontPages(BOOL include_extended_unicode)
{
    int loop;
    GLYPH_RANGE *range = DefaultGlyphRanges;

    int font_count = NUM_FONT_CHAR_RANGES;

    if (include_extended_unicode)
    {
        font_count += NUM_FONT_EXTENDED_CHAR_RANGES;
    }

    font_page_info *info = new font_page_info[font_count];
    memset(info, 0, sizeof(font_page_info) * font_count);

    for (loop = 0; loop < NUM_FONT_CHAR_RANGES; loop++)
    {
        info[loop].first_char = range->first_char;
        info[loop].last_char = range->last_char;
        range++;
    }

    if (include_extended_unicode)
    {
        range = ExtendedGlyphRanges;

        for (loop = NUM_FONT_CHAR_RANGES; loop < font_count; loop++)
        {
            info[loop].first_char = range->first_char;
            info[loop].last_char = range->last_char;
            range++;
        }
    }
    info[0].enabled = TRUE;
    return info;
}


///////////////////////////////////////////////////////////////////////////////
font_page_info *font_path_dialog::CreateExtendedFontPages()
{
    int loop;
    GLYPH_RANGE *range = ExtendedGlyphRanges;

    font_page_info *info = new font_page_info[NUM_FONT_EXTENDED_CHAR_RANGES];
    memset(info, 0, sizeof(font_page_info) * NUM_FONT_EXTENDED_CHAR_RANGES);

    for (loop = 0; loop < NUM_FONT_EXTENDED_CHAR_RANGES; loop++)
    {
        info[loop].first_char = range->first_char;
        info[loop].last_char = range->last_char;
        range++;
    }
    return info;
}

///////////////////////////////////////////////////////////////////////////////
font_path_dialog::font_path_dialog(int display_index, int color_format, res_info *info, CWnd* pParent /*=NULL*/)
    : express_dialog(font_path_dialog::IDD, pParent)
{
    IconId = IDB_FONTS;
    SetTitleText("Edit Font");

    m_info = info;
    pathinfo = m_info->pathinfo;
    m_color_format = color_format;
    m_display_index = display_index;

    if (!m_info->font_pages)
    {
        m_info->font_pages = CreateDefaultFontPages();
    }

    int dpi = GetSystemDPI();
    m_range_row_height = MulDiv(RANGE_ROW_HEIGHT, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
font_path_dialog::~font_path_dialog()
{
    mpRangeListWin->DestroyWindow();
    delete mpRangeListWin;

    mpRangeListHeader->DestroyWindow();
    delete mpRangeListHeader;
}

///////////////////////////////////////////////////////////////////////////////
void font_path_dialog::DoDataExchange(CDataExchange* pDX)
{
    //int aliased = m_info->anti_aliased;

    express_dialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_FONT_HEIGHT, m_info->font_height);
    DDX_Text(pDX, IDC_FONT_NAME, m_info->name);
    DDX_Check(pDX, IDC_INCLUDE_STRING_TABLE, m_info->font_charset_include_string_table);
    DDX_Check(pDX, IDC_SUPPORT_EXTENDED_UNICODE, m_info->font_support_extended_unicode);
    DDX_Control(pDX, IDC_INCLUDE_RANGE_PROMPT, mPagesPrompt);
    DDX_Check(pDX, IDC_COMPRESS, m_info->compress);
    DDX_Check(pDX, IDC_KERNING, m_info->font_kerning);
    DDX_Check(pDX, IDC_CUSTOM_OUTPUT, m_info->output_file_enabled);
    DDX_Check(pDX, IDC_BINARY_MODE, m_info->binary_mode);
    DDX_Control(pDX, IDC_OUTPUT_FILE, mCustomFile);
    DDX_Control(pDX, IDC_BINARY_MODE, mBinaryMode);
    
    CButton *pb;

    mHeight = m_info->font_height;

    pb = (CButton *)GetDlgItem(IDC_SUPPORT_EXTENDED_UNICODE);
    if (m_info->font_support_extended_unicode)
    {
        pb->SetCheck(BST_CHECKED);
    }
    else
    {
        pb->SetCheck(BST_UNCHECKED);
    }

    if (m_color_format < GX_COLOR_FORMAT_4BIT_GRAY)
    {
        pb = (CButton *) GetDlgItem(IDC_4BIT_ALIASED);
        pb->EnableWindow(FALSE);
        m_info->font_bits = 1;
    }

    if (m_color_format <= GX_COLOR_FORMAT_8BIT_PACKED_PIXEL)
    {
        pb = (CButton *) GetDlgItem(IDC_8BIT_ALIASED);
        pb->EnableWindow(FALSE);

        if (m_info->font_bits == 8)
        {
            m_info->font_bits = 4;
        }
    }
    if (pDX->m_bSaveAndValidate)
    {
        DDX_Text(pDX, IDC_FONT_PATH, pathinfo.pathname);

        if (IsDlgButtonChecked(IDC_1BIT_MONO))
        {
            m_info->font_bits = 1;
        }
        else
        {
            if (IsDlgButtonChecked(IDC_4BIT_ALIASED))
            {
                m_info->font_bits = 4;
            }
            else
            {
                m_info->font_bits = 8;
            }
        }
    }
    else
    {
        if (m_info->output_file_enabled)
        {
            mCustomFile.ShowWindow(TRUE);
            mCustomFile.SetWindowText(m_info->output_file);

            mBinaryMode.ShowWindow(TRUE);
        }

        SetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
        OnPathnameChange();

        switch(m_info->font_bits)
        {
        case 1:
            pb = (CButton *) GetDlgItem(IDC_1BIT_MONO);
            break;

        case 4:
            pb = (CButton *) GetDlgItem(IDC_4BIT_ALIASED);
            break;

        case 8:
        default:
            pb = (CButton *) GetDlgItem(IDC_8BIT_ALIASED);
        }
        if (pb)
        {
            pb->SetCheck(BST_CHECKED);
        }
    }
    
    CEdit* edit = (CEdit*)GetDlgItem(IDC_FONT_NAME);

    if (m_info->is_default)
    {
        edit->SetReadOnly(TRUE);

        GetDlgItem(IDC_REQUIRED_FIELD_LEGEND)->ShowWindow(FALSE);
    }
    else
    {
        SetAccessibleFullDescription(edit->GetSafeHwnd(), L"Required");
        SetAccessibleDescription(edit->GetSafeHwnd(), L"Required");
    }
    DDX_Control(pDX, IDB_PATH_TYPE, mPathTypeButton);

    if (!pDX->m_bSaveAndValidate)
    {
        mPathBitmap.LoadBitmap(IDB_DOWN_ARROW);
        mPathTypeButton.SetBitmap(mPathBitmap);
        SetPathPrefix();

        SetControlAccessibleName(GetDlgItem(IDC_OUTPUT_FILE)->GetSafeHwnd(), L"Custom Output File");
    }
}

// font_path_dialog message handlers

///////////////////////////////////////////////////////////////////////////////
void font_path_dialog::OnBnClickedBrowse()
{
    // TODO: Add your control notification handler code here
    CString pathname;
    CString oldname = pathname;
    BrowseForSingleFile(_T("Select font file"),
                        _T("Font Files (*.ttf; *.otf)\0*.ttf;*.otf\0"), _T("ttf"), pathname, this);
    if (pathname.IsEmpty() || oldname == pathname)
    {
        return;
    }

    SaveToPathInfo(pathinfo, pathname);

    SetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
    OnPathnameChange();
}

///////////////////////////////////////////////////////////////////////////////
BOOL font_path_dialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CWnd *parent1;
    CWnd *parent2;

    if (wParam == IDOK)
    {
        /* figure out if they hit return while on an edit field */
        CWnd *child = GetFocus();

        if (child)
        {
            if (child->GetDlgCtrlID() == ID_START_FIELD ||
                child->GetDlgCtrlID() == ID_END_FIELD)
            { 
                parent1 = child->GetParent();       // could be range row?
            
                if (parent1)
                {
                    parent2 = parent1->GetParent(); // could be range list win?
                
                    if (parent2 == mpRangeListWin)
                    {
                        // tell the row to save it's data
                        font_range_row *row = (font_range_row *) parent1;
                        row->OnEditRangeField();
                        return TRUE;
                    }
                }
            }
        }
    }
    return CWnd::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
void font_path_dialog::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    CEdit* edit = (CEdit *)GetDlgItem(IDC_FONT_NAME);
    CString font_name;

    if (!m_info->is_default)
    {
        if (pathinfo.pathname.IsEmpty())
        {
            Notify("Please set the font path.", this);
            return;
        }

        GetDlgItemText(IDC_FONT_NAME, font_name);
        if (m_info->parent == NULL)
        {
            if (NameExists(m_display_index, RES_TYPE_FONT, font_name))
            {
                ErrorMsg("Font Name already Exists.", this);
                return;
            }
        }
        else
        {
            if (_tcscmp(font_name, m_info->name))
            {
                if (NameExists(m_display_index, RES_TYPE_FONT, font_name))
                {
                    ErrorMsg("Font Name already Exists.", this);
                    SetDlgItemText(IDC_FONT_NAME, m_info->name);
                    return;
                }
            }
        }
    }

    if (TestInputName(edit, "Font Name", m_info->name, this))
    {
        m_info->pathinfo = pathinfo;
        mpRangeListWin->SaveRangeInfo();

        mCustomFile.GetWindowText(m_info->output_file);

        if (m_info->output_file.IsEmpty())
        {
            m_info->output_file_enabled = FALSE;
        }

        OnOK();
    }
}

void font_path_dialog::OnBnClickedPathType()
{
    // TODO: Add your control notification handler code here
    CMenu menu;
    CMenu *pPopup;
    CPoint pos;
    CRect  buttonsize;
    mPathTypeButton.GetWindowRect(&buttonsize);

    pos.x = buttonsize.left;
    pos.y = buttonsize.bottom + 2;

    menu.LoadMenu(IDR_PATHTYPE);
    pPopup = menu.GetSubMenu(0);
    //ClientToScreen(&pos);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);
}

void font_path_dialog::OnBnClickedCustomOutput()
{
    // TODO: Add your control notification handler code here
    CButton* pb = (CButton*)GetDlgItem(IDC_CUSTOM_OUTPUT);

    if (pb->GetCheck() == BST_CHECKED)
    {
        mCustomFile.ShowWindow(SW_SHOW);
        mBinaryMode.ShowWindow(SW_SHOW);
    }
    else
    {
        mCustomFile.ShowWindow(SW_HIDE);
        mBinaryMode.ShowWindow(SW_HIDE);
    }
}

void font_path_dialog::SetPathPrefix()
{
    switch(pathinfo.pathtype)
    {
    case PATH_TYPE_PROJECT_RELATIVE:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<Project Relative Path>"));
        break;

    case PATH_TYPE_INSTALL_RELATIVE:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<GUIX Repo Relative Path>"));
        break;

    case PATH_TYPE_ABSOLUTE:
    default:
        SetDlgItemText(IDC_PATH_PREFIX, _T("<Absolute Path>"));
        break;
            
    }
}

void font_path_dialog::SetPathProjectRelative()
{
    if (!pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(pathinfo);
        pathinfo.pathtype = PATH_TYPE_PROJECT_RELATIVE;
        SaveToPathInfo(pathinfo, abspath);
        SetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
        SetPathPrefix();
    }
}

void font_path_dialog::SetPathStudioRelative()
{
    if (!pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(pathinfo);
        pathinfo.pathtype = PATH_TYPE_INSTALL_RELATIVE;
        SaveToPathInfo(pathinfo, abspath);
        SetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
        SetPathPrefix();
    }
}

void font_path_dialog::SetPathAbsolute()
{
    if (!pathinfo.pathname.IsEmpty())
    {
        CString abspath = MakeAbsolutePathname(pathinfo);
        pathinfo.pathtype = PATH_TYPE_ABSOLUTE;
        SaveToPathInfo(pathinfo, abspath);
        SetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
        SetPathPrefix();
    }
}

void font_path_dialog::OnKillFontPathEdit()
{
    GetDlgItemText(IDC_FONT_PATH, pathinfo.pathname);
    OnPathnameChange();
}

void font_path_dialog::OnPathnameChange()
{
    CEdit *edit = (CEdit*)GetDlgItem(IDC_FONT_HEIGHT);
    CButton *button = (CButton*)GetDlgItem(IDC_KERNING);
    CButton *custom_output_check = (CButton*)GetDlgItem(IDC_CUSTOM_OUTPUT);

    if (m_info->is_default && pathinfo.pathname.IsEmpty())
    {
        edit->EnableWindow(FALSE);
        button->EnableWindow(FALSE);
        custom_output_check->SetCheck(0);
        GetDlgItem(IDC_OUTPUT_FILE)->ShowWindow(FALSE);
        GetDlgItem(IDC_BINARY_MODE)->ShowWindow(FALSE);
        custom_output_check->EnableWindow(FALSE);
        return;
    }
    else
    {
        edit->EnableWindow(TRUE);
        button->EnableWindow(TRUE);
        custom_output_check->EnableWindow(TRUE);
    }
    Invalidate();
}

void font_path_dialog::OnEnChangeFontHeight()
{
    int val = GetDlgItemInt(IDC_FONT_HEIGHT);
    if (val >= 0 && val <= 255)
    {
        mHeight = val;
    }
    else
    {
        Notify("Invalid value for font height. Valid range is from 0 to 255.", this);
        SetDlgItemInt(IDC_FONT_HEIGHT, mHeight);
    }
}

BOOL font_path_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();

    CRect RangeListSize;
    CRect Size;

    GetClientRect(&RangeListSize);

    GetDlgItem(IDC_INCLUDE_RANGE_PROMPT)->GetWindowRect(&Size);
    ScreenToClient(&Size);
    
    // Postion include range prompt
    RangeListSize.top = Size.bottom + m_range_row_height + 8;

    GetDlgItem(IDC_FONT_FORMAT_GROUP)->GetWindowRect(&Size);
    ScreenToClient(&Size);
    RangeListSize.right = Size.left - 10;

    RangeListSize.left += 12;
    RangeListSize.bottom -= (m_status_bar_height + 8);

    CFont* font = GetDlgItem(IDC_FONT_NAME)->GetFont();
    mpRangeListWin = new range_list_win(m_info, font);
    mpRangeListWin->Create(target_class_name, _T("Character Pages"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | WS_TABSTOP,
                           RangeListSize, this, 0);
    mpRangeListWin->SetWindowPos(GetDlgItem(IDC_FONT_FORMAT_GROUP), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    Size.left = RangeListSize.left;
    Size.top = RangeListSize.top - m_range_row_height - 4;
    Size.right = Size.left + RangeListSize.Width();
    Size.bottom = RangeListSize.top - 1;

    mpRangeListHeader = new font_range_header(font);
    mpRangeListHeader->Create(target_class_name, _T("Character Page Header"), WS_CHILD | WS_VISIBLE, Size, this, 0);

    studiox_project *project = GetOpenProject();
    if ((!IsRenesasDave2D(project))|
        (project->mHeader.guix_version < 50207) ||
        (project->mDisplays[m_display_index].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE))
    {
        // Add checkbox to allow the user to specify RLE compression enabled only for synergy.
        GetDlgItem(IDC_COMPRESS)->ShowWindow(GX_FALSE);
    }

    if ((IsRenesasDave2D(project)) || (project->mHeader.guix_version < 50304))
    {
        GetDlgItem(IDC_KERNING)->ShowWindow(FALSE);
    }

    AddCancelButton();
    AddSaveButton();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void font_path_dialog::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call express_dialog::OnPaint() for painting messages

    if (!m_info->is_default)
    {
        PaintRequiredAsterisk(IDC_FONT_NAME);
        PaintRequiredAsterisk(IDC_REQUIRED_FIELD_LEGEND);
    }
}


LRESULT font_path_dialog::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;
    int ctrl_id;
    CStringArray param;
    int page_index;
    int check;
    CString range_start;
    CString range_end;

    switch (wParam)
    {
    case TEST_SET_FONT_PATH:
        //set font path
        SetDlgItemText(IDC_FONT_PATH, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_FONT_PATH);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_FONT_PATH, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FONT_NAME:
        //set font name
        SetDlgItemText(IDC_FONT_NAME, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_FONT_NAME);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_FONT_NAME, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FONT_HEIGHT:
        //set font height
        SetDlgItemText(IDC_FONT_HEIGHT, GetTestingParam(0));
        pWnd = GetDlgItem(IDC_FONT_HEIGHT);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_FONT_HEIGHT, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FONT_HEIGHT_POST:
        //set font height
        SetDlgItemText(IDC_FONT_HEIGHT, GetTestingParam(1));
        pWnd = GetDlgItem(IDC_FONT_HEIGHT);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_FONT_HEIGHT, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FONT_FORMAT:
        //set font format
        switch (lParam)
        {
        case GX_FONT_FORMAT_1BPP:
            ctrl_id = IDC_1BIT_MONO;
            break;

        case GX_FONT_FORMAT_4BPP:
            ctrl_id = IDC_4BIT_ALIASED;
            break;

        case GX_FONT_FORMAT_8BPP:
            ctrl_id = IDC_8BIT_ALIASED;
            break;

        default:
            return 0;
        }

        CheckRadioButton(IDC_1BIT_MONO, IDC_8BIT_ALIASED, ctrl_id);
        break;

    case TEST_SET_FONT_COMPRESSION:
        //set compression property
        pWnd = GetDlgItem(IDC_COMPRESS);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_COMPRESS, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_FONT_GENERATE_KERNING:
        pWnd = GetDlgItem(IDC_KERNING);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_KERNING, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_PAGE_RANGE:
        //set range info
        SplitString(GetTestingParam(0), ',', &param);
        page_index = _tstoi(param.GetAt(0));
        check = _tstoi(param.GetAt(1));
        range_start = param.GetAt(2);
        range_end = param.GetAt(3);

        if ((page_index >= 0) && (page_index < NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES))
        {
            font_range_row *row = (font_range_row *)mpRangeListWin->GetWindow(GW_CHILD);

            while (page_index && row)
            {
                row = (font_range_row *)row->GetWindow(GW_HWNDNEXT);
                page_index--;
            }

            if (row)
            {
                //check page range
                pWnd = row->GetDlgItem(ID_ENABLE_BOX);
                ((CButton *)pWnd)->SetCheck(check);
                row->SendMessage(WM_COMMAND, MAKEWPARAM(ID_ENABLE_BOX, BN_CLICKED), (LPARAM)pWnd->m_hWnd);

                //set range start
                row->SetDlgItemText(ID_START_FIELD, range_start);
                pWnd = row->GetDlgItem(ID_START_FIELD);
                row->SendMessage(WM_COMMAND, MAKEWPARAM(ID_START_FIELD, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);

                //set range end
                row->SetDlgItemText(ID_END_FIELD, range_end);
                pWnd = row->GetDlgItem(ID_END_FIELD);
                row->SendMessage(WM_COMMAND, MAKEWPARAM(ID_END_FIELD, EN_KILLFOCUS), (LPARAM)pWnd->m_hWnd);
            }
            break;
        }
        break;

    case TEST_CHECK_EXTENDED_UNICODE:
        pWnd = GetDlgItem(IDC_SUPPORT_EXTENDED_UNICODE);
        ((CButton *)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SUPPORT_EXTENDED_UNICODE, BN_CLICKED), (LPARAM)((CButton *)pWnd)->m_hWnd);
        break;

    case TEST_CHECK_CUSTOM_OUTPUT:
        pWnd = GetDlgItem(IDC_CUSTOM_OUTPUT);
        ((CButton*)pWnd)->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CUSTOM_OUTPUT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        break;

    case TEST_SET_CUSTOM_OUTPUT_FILE:
        pWnd = GetDlgItem(IDC_OUTPUT_FILE);
        ((CEdit*)pWnd)->SetWindowText(GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_OUTPUT_FILE, EN_KILLFOCUS), (LPARAM)((CEdit*)pWnd)->m_hWnd);
        break;

    case TEST_SAVE_FONT_EDIT:
        //save modifications
        OnBnClickedOk();
        break;

    case TEST_CANCEL_FONT_EDIT:
        //cancel modifications
        OnCancel();
        break;
    }

    return 0;
}

void font_path_dialog::OnBnClickedSupportExtendedUnicode()
{
    // TODO: Add your control notification handler code here
    CButton *pb = (CButton *)GetDlgItem(IDC_SUPPORT_EXTENDED_UNICODE);

    if (pb->GetCheck())
    {
        font_page_info *extended_font_pages = CreateExtendedFontPages();
        font_page_info *new_pages = new font_page_info[NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES];
        memcpy_s(new_pages, sizeof(font_page_info) * NUM_FONT_CHAR_RANGES, m_info->font_pages, sizeof(font_page_info) * NUM_FONT_CHAR_RANGES);
        memcpy_s(new_pages + NUM_FONT_CHAR_RANGES, sizeof(font_page_info) * NUM_FONT_EXTENDED_CHAR_RANGES, extended_font_pages, sizeof(font_page_info) * NUM_FONT_EXTENDED_CHAR_RANGES);
        delete[] extended_font_pages;
        delete[] m_info->font_pages;
        m_info->font_pages = new_pages;
        mpRangeListWin->CreateExtendedRangeEntry();
    }
    else
    {
        font_page_info *new_pages = new font_page_info[NUM_FONT_CHAR_RANGES];
        memcpy_s(new_pages, sizeof(font_page_info) * NUM_FONT_CHAR_RANGES, m_info->font_pages, sizeof(font_page_info) * NUM_FONT_CHAR_RANGES);
        delete[] m_info->font_pages;
        m_info->font_pages = new_pages;
        mpRangeListWin->DeleteExtendedRangeEntry();
    }
}
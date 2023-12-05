
#include "studiox_includes.h"
#include "sprite_edit_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CFont TitleFont;
extern CFont MediumFont;
extern CFont NormalFont;

#define SORT_FLAG CBS_SORT
//#define SORT_FLAG 0

#define PROPERTY_ITEM_HEIGHT 18
#define PROPERTY_ITEM_SPACE  2
#define LEFT_COLUMN_WIDTH 140

///////////////////////////////////////////////////////////////////////////////
enum CONTROL_IDS { 
    ID_WIDGET_NAME = 4096,
    ID_WIDGET_ID,
    ID_USER_DATA,
    ID_EVENT_FUNCTION,
    ID_DRAW_FUNCTION,
    ID_ALLOCATOR_NAME,
    ID_TRANSPARENT,
    ID_SELECTED,
    ID_ENABLED,
    ID_ACCEPTS_FOCUS,
    ID_TEMPLATE,
    ID_VISIBLE_AT_STARTUP,
    ID_BORDER_COMBO,
    ID_NORMAL_COLOR,
    ID_SELECTED_COLOR,
    ID_DISABLED_COLOR,
    ID_READONLY_COLOR,
    ID_XPOS,
    ID_YPOS,
    ID_WIDTH,
    ID_HEIGHT,
    ID_TILE_WALLPAPER,
    ID_DYNAMIC_WIDGET,
    ID_DYNAMIC_BUFFER,

    ID_PIXELMAP_0,
    ID_PIXELMAP_1,
    ID_PIXELMAP_2,
    ID_PIXELMAP_3,
    ID_PIXELMAP_4,
    ID_PIXELMAP_5,
    ID_PIXELMAP_6,
    ID_PIXELMAP_7,

    ID_TEXT_ALIGN,
    ID_TEXT_COPY,
    ID_FONT_SELECT_0,
    ID_FONT_SELECT_1,
    ID_FONT_SELECT_2,
    ID_FONT_SELECT_3,
    ID_NORMAL_TEXT_COLOR,
    ID_SELECTED_TEXT_COLOR,
    ID_DISABLED_TEXT_COLOR,
    ID_READONLY_TEXT_COLOR,
    ID_BUTTON_STYLE_PUSHED,
    ID_BUTTON_STYLE_TOGGLE,
    ID_BUTTON_STYLE_RADIO,
    ID_BUTTON_STYLE_REPEAT,
    ID_PIXMAP_HALIGN,
    ID_PIXMAP_VALIGN,

    ID_SLIDER_MINVAL,
    ID_SLIDER_MAXVAL,
    ID_SLIDER_CURRENTVAL,
    ID_SLIDER_INCREMENT,
    ID_SLIDER_MINTRAVEL,
    ID_SLIDER_MAXTRAVEL,
    ID_SLIDER_NEEDLE_WIDTH,
    ID_SLIDER_NEEDLE_HEIGHT,
    ID_SLIDER_NEEDLE_INSET,
    ID_NEEDLE_HOTSPOT,
    ID_TICKMARK_COUNT,

    ID_SLIDER_VERTICAL,
    ID_SHOW_TICKMARKS,
    ID_SLIDER_TILE_BKGND,

    ID_PROGRESS_VERTICAL,
    ID_PROGRESS_SHOW_TEXT,
    ID_PROGRESS_PERCENT,

    ID_PROGRESS_MINVAL,
    ID_PROGRESS_MAXVAL,
    ID_PROGRESS_CURRENTVAL,

    ID_RADIAL_PROGRESS_BACKTRACK,
    ID_RADIAL_PROGRESS_SHOW_TEXT,
    ID_RADIAL_PROGRESS_PERCENT,
    ID_RADIAL_PROGRESS_ALIAS,
    ID_RADIAL_PROGRESS_ROUND,
    ID_RADIAL_PROGRESS_XCENTER,
    ID_RADIAL_PROGRESS_YCENTER,
    ID_RADIAL_PROGRESS_RADIUS,
    ID_RADIAL_PROGRESS_ANCHORVAL,
    ID_RADIAL_PROGRESS_CURRENTVAL,
    ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR,
    ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR,
    ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH,
    ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH,

    ID_RADIAL_SLIDER_XCENTER,
    ID_RADIAL_SLIDER_YCENTER,
    ID_RADIAL_SLIDER_RADIUS,
    ID_RADIAL_SLIDER_TRACK_WIDTH,
    ID_RADIAL_SLIDER_NEEDLE_OFFSET,
    ID_RADIAL_SLIDER_CURRENT_ANGLE,
    ID_RADIAL_SLIDER_MIN_ANGLE,
    ID_RADIAL_SLIDER_MAX_ANGLE,
    ID_RADIAL_SLIDER_TOTAL_STEPS,
    ID_RADIAL_SLIDER_DELAY,
    ID_RADIAL_SLIDER_ANIMATION_STYLE,

    ID_SPRITE_AUTO,
    ID_SPRITE_LOOP,
    ID_SPRITE_EDIT_FRAMES,

    ID_SCROLL_THUMB_COLOR,
    ID_SCROLL_THUMB_BORDER_COLOR,
    ID_SCROLL_BUTTON_COLOR,
    ID_THUMB_WIDTH,
    ID_THUMB_TRAVEL_MIN,
    ID_THUMB_TRAVEL_MAX,
    ID_SCROLL_THUMB_BORDER_STYLE,
    ID_SCROLL_END_BUTTONS,
    ID_SCROLL_TILE_BKGND,
    ID_SCROLL_RELATIVE_THUMB,
    ID_SCROLL_IS_DEFAULT,

    ID_LIST_WRAP,
    ID_LIST_REPEAT_SELECT,
    ID_LIST_ROWS,
    ID_OPEN_HEIGHT,
    ID_CALLBACK_NAME,

    ID_ML_TEXT_VCENTER,
    ID_TEXT_VIEW_WHITESPACE,
    ID_TEXT_VIEW_LINE_SPACE,

    ID_TEXT_INPUT_READONLY,
    ID_BUFFER_SIZE,

    ID_STRING_ID,
    ID_STRING_VAL,

    ID_GAUGE_START_ANGLE,
    ID_GAUGE_ANIMATION_STEPS,
    ID_GAUGE_ANIMATION_DELAY,
    ID_GAUGE_NEEDLE_XPOS,
    ID_GAUGE_NEEDLE_YPOS,
    ID_GAUGE_NEEDLE_XCOR,
    ID_GAUGE_NEEDLE_YCOR,

    ID_CHART_MIN_VAL,
    ID_CHART_MAX_VAL,
    ID_CHART_LEFT_MARGIN,
    ID_CHART_TOP_MARGIN,
    ID_CHART_RIGHT_MARGIN,
    ID_CHART_BOTTOM_MARGIN,
    ID_CHART_MAX_COUNT,
    ID_CHART_AXIS_WIDTH,
    ID_CHART_LINE_WIDTH,
    ID_CHART_AXIS_COLOR,
    ID_CHART_LINE_COLOR,

    ID_SCROLL_WHEEL_TOTAL_ROWS,
    ID_SCROLL_WHEEL_SELECTED_ROW,
    ID_SCROLL_WHEEL_ROW_HEIHGT,
    ID_SCROLL_WHEEL_START_ALPHA,
    ID_SCROLL_WHEEL_END_ALPHA,

    ID_TEXT_SCROLL_WHEEL_NORMAL_FONT,
    ID_TEXT_SCROLL_WHEEL_SELECTED_FONT,
    ID_TEXT_SCROLL_WHEEL_ROUND,

    ID_STRING_SCROLL_WHEEL_EDIT_STRINGS,

    ID_NUMERIC_SCROLL_WHEEL_START_VAL,
    ID_NUMERIC_SCROLL_WHEEL_END_VAL,

    ID_TILE_DROP_LIST_BACKGROUND,
    ID_TILE_DROP_LIST_POPLIST_WALLPAPER,

    ID_NUMERIC_PROMPT_VALUE,
    ID_FORMAT_FUNC,

    ID_MENU_EXPANDED,
    ID_MENU_INSERT_MODE,
    ID_MENU_TEXT_X_OFFSET,
    ID_MENU_TEXT_Y_OFFSET,

    ID_TREE_VIEW_ROOT_LINE_COLOR,
    ID_TREE_VIEW_SHOW_ROOT_LINES,
    ID_TREE_VIEW_INDENTATION,

    ID_RICH_TEXT_VIEW_TEXT_EDIT
};

///////////////////////////////////////////////////////////////////////////////
STRING_VAL_PAIR border_types[] = {
    {_T("No Border"),           GX_STYLE_BORDER_NONE},
    {_T("Thin Border"),         GX_STYLE_BORDER_THIN},
    {_T("Raised Border"),       GX_STYLE_BORDER_RAISED},
    {_T("Recessed Border"),     GX_STYLE_BORDER_RECESSED},
    {_T("Thick Border"),        GX_STYLE_BORDER_THICK},
    {_T(""),                     0}
};

///////////////////////////////////////////////////////////////////////////////
STRING_VAL_PAIR button_halign_styles[] = {
    { _T("Center"), GX_STYLE_HALIGN_CENTER },
    { _T("Left"), GX_STYLE_HALIGN_LEFT },
    { _T("Right"), GX_STYLE_HALIGN_RIGHT },
    {NULL, 0}
};

///////////////////////////////////////////////////////////////////////////////
STRING_VAL_PAIR button_valign_styles[] = {
    { _T("Center"), GX_STYLE_VALIGN_CENTER },
    { _T("Top"), GX_STYLE_VALIGN_TOP },
    { _T("Bottom"), GX_STYLE_VALIGN_BOTTOM },
    {NULL, 0}
};

///////////////////////////////////////////////////////////////////////////////
STRING_VAL_PAIR text_align_styles[] = {
    { _T("Left"), GX_STYLE_TEXT_LEFT },
    { _T("Center"), GX_STYLE_TEXT_CENTER },
    { _T("Right"), GX_STYLE_TEXT_RIGHT },
    {NULL, 0}
};

extern STRING_VAL_PAIR easing_function_list[];

enum properties_win_test_commands{
    TEST_EDIT_WIDGET_PROPS = 1,
    TEST_EDIT_WIDGET_PROPS_POST
};

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(properties_win, CWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_WM_SYSCHAR()

    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    //ON_WM_COMMAND()

    ON_EN_KILLFOCUS(ID_WIDGET_NAME, OnChangeWidgetName)
    ON_EN_KILLFOCUS(ID_WIDGET_ID, OnChangeWidgetId)
    ON_EN_KILLFOCUS(ID_EVENT_FUNCTION, OnChangeEventFunction)
    ON_EN_KILLFOCUS(ID_DRAW_FUNCTION, OnChangeDrawFunction)
    ON_EN_KILLFOCUS(ID_USER_DATA, OnChangeWidgetUserData)

    ON_EN_KILLFOCUS(ID_XPOS, OnChangeWidgetPos)
    ON_EN_KILLFOCUS(ID_YPOS, OnChangeWidgetPos)
    ON_EN_KILLFOCUS(ID_WIDTH, OnChangeWidgetPos)
    ON_EN_KILLFOCUS(ID_HEIGHT, OnChangeWidgetPos)

    ON_EN_KILLFOCUS(ID_NUMERIC_PROMPT_VALUE, OnChangeNumericPrompt)
    ON_EN_KILLFOCUS(ID_FORMAT_FUNC, OnChangeNumericPrompt)

    ON_EN_KILLFOCUS(ID_LIST_ROWS, OnChangeListRows)
    ON_EN_KILLFOCUS(ID_OPEN_HEIGHT, OnChangeOpenHeight)
    ON_EN_KILLFOCUS(ID_TEXT_VIEW_LINE_SPACE, OnChangeTextViewLineSpace)
    ON_EN_KILLFOCUS(ID_TEXT_VIEW_WHITESPACE, OnChangeTextViewWhitespace)
    ON_EN_KILLFOCUS(ID_BUFFER_SIZE, OnChangeTextBufferSize)
    ON_EN_KILLFOCUS(ID_SLIDER_MINVAL, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_MAXVAL, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_CURRENTVAL, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_INCREMENT, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_MINTRAVEL, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_MAXTRAVEL, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_NEEDLE_WIDTH, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_NEEDLE_HEIGHT, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_SLIDER_NEEDLE_INSET, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_NEEDLE_HOTSPOT, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_TICKMARK_COUNT, OnChangeSliderInfo)
    ON_EN_KILLFOCUS(ID_CALLBACK_NAME, OnChangeCallbackName)
    ON_EN_KILLFOCUS(ID_STRING_VAL, OnChangeStringText)
    ON_EN_KILLFOCUS(ID_THUMB_WIDTH, OnScrollAppearanceChange)
    ON_EN_KILLFOCUS(ID_THUMB_TRAVEL_MIN, OnScrollAppearanceChange)
    ON_EN_KILLFOCUS(ID_THUMB_TRAVEL_MAX, OnScrollAppearanceChange)

    ON_EN_KILLFOCUS(ID_PROGRESS_MINVAL, OnProgressInfoChange)
    ON_EN_KILLFOCUS(ID_PROGRESS_MAXVAL, OnProgressInfoChange)
    ON_EN_KILLFOCUS(ID_PROGRESS_CURRENTVAL, OnProgressInfoChange)

    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_XCENTER, OnChangeRadialProgressBarInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_YCENTER, OnChangeRadialProgressBarInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_RADIUS, OnChangeRadialProgressBarInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_CURRENTVAL, OnChangeRadialProgressBarInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH, OnChangeRadialProgressBarInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH, OnChangeRadialProgressBarInfo)
    ON_CBN_SELCHANGE(ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR, OnChangeRadialProgressBarInfo)
    ON_CBN_SELCHANGE(ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR, OnChangeRadialProgressBarInfo)

    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_XCENTER, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_YCENTER, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_RADIUS, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_TRACK_WIDTH, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_NEEDLE_OFFSET, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_CURRENT_ANGLE, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_MIN_ANGLE, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_MAX_ANGLE, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_TOTAL_STEPS, OnChangeRadialSliderInfo)
    ON_EN_KILLFOCUS(ID_RADIAL_SLIDER_DELAY, OnChangeRadialSliderInfo)
    ON_CBN_SELCHANGE(ID_RADIAL_SLIDER_ANIMATION_STYLE, OnChangeRadialSliderInfo)

    ON_CBN_SELCHANGE(ID_BORDER_COMBO, OnChangeBorder)
    ON_CBN_SELCHANGE(ID_NORMAL_COLOR, OnChangeColor)
    ON_CBN_SELCHANGE(ID_SELECTED_COLOR, OnChangeColor)
    ON_CBN_SELCHANGE(ID_DISABLED_COLOR, OnChangeColor)

    ON_CBN_SELCHANGE(ID_PIXELMAP_0, OnChangePixelmap0)
    ON_CBN_SELCHANGE(ID_PIXELMAP_1, OnChangePixelmap1)
    ON_CBN_SELCHANGE(ID_PIXELMAP_2, OnChangePixelmap2)
    ON_CBN_SELCHANGE(ID_PIXELMAP_3, OnChangePixelmap3)
    ON_CBN_SELCHANGE(ID_PIXELMAP_4, OnChangePixelmap4)
    ON_CBN_SELCHANGE(ID_PIXELMAP_5, OnChangePixelmap5)
    ON_CBN_SELCHANGE(ID_PIXELMAP_6, OnChangePixelmap6)
    ON_CBN_SELCHANGE(ID_PIXELMAP_7, OnChangePixelmap7)

    ON_CBN_SELCHANGE(ID_TEXT_ALIGN, OnTextAlignmentChange)
    ON_CBN_SELCHANGE(ID_FONT_SELECT_0, OnFontChange0)
    ON_CBN_SELCHANGE(ID_FONT_SELECT_1, OnFontChange1)
    ON_CBN_SELCHANGE(ID_FONT_SELECT_2, OnFontChange2)
    ON_CBN_SELCHANGE(ID_FONT_SELECT_3, OnFontChange3)
    ON_CBN_SELCHANGE(ID_NORMAL_TEXT_COLOR, OnChangeNormalTextColor)
    ON_CBN_SELCHANGE(ID_SELECTED_TEXT_COLOR, OnChangeSelectedTextColor)
    ON_CBN_SELCHANGE(ID_DISABLED_TEXT_COLOR, OnChangeDisabledTextColor)
    ON_CBN_SELCHANGE(ID_READONLY_COLOR, OnChangeReadonlyFillColor)
    ON_CBN_SELCHANGE(ID_READONLY_TEXT_COLOR, OnChangeReadonlyTextColor)
    ON_CBN_SELCHANGE(ID_PIXMAP_HALIGN, OnChangePixmapAlign)
    ON_CBN_SELCHANGE(ID_PIXMAP_VALIGN, OnChangePixmapAlign)

    ON_CBN_SELCHANGE(ID_STRING_ID, OnChangeStringId)

    ON_BN_CLICKED(ID_DYNAMIC_WIDGET, OnChangeDynamicWidget)
    ON_BN_CLICKED(ID_DYNAMIC_BUFFER, OnChangeDynamicBuffer)
    ON_BN_CLICKED(ID_SLIDER_VERTICAL, OnSliderStyleChange)
    ON_BN_CLICKED(ID_SHOW_TICKMARKS, OnSliderStyleChange)
    ON_BN_CLICKED(ID_SLIDER_TILE_BKGND, OnSliderStyleChange)
    ON_BN_CLICKED(ID_TILE_DROP_LIST_BACKGROUND, OnDropListStyleChange)
    ON_BN_CLICKED(ID_TILE_DROP_LIST_POPLIST_WALLPAPER, OnDropListStyleChange)

    ON_BN_CLICKED(ID_PROGRESS_VERTICAL, OnProgressInfoChange)
    ON_BN_CLICKED(ID_PROGRESS_SHOW_TEXT, OnProgressInfoChange)
    ON_BN_CLICKED(ID_PROGRESS_PERCENT, OnProgressInfoChange)
    ON_BN_CLICKED(ID_RADIAL_PROGRESS_SHOW_TEXT, OnChangeRadialProgressBarInfo)
    ON_BN_CLICKED(ID_RADIAL_PROGRESS_PERCENT, OnChangeRadialProgressBarInfo)
    ON_BN_CLICKED(ID_RADIAL_PROGRESS_BACKTRACK, OnChangeRadialProgressBarInfo)
    ON_BN_CLICKED(ID_RADIAL_PROGRESS_ALIAS, OnChangeRadialProgressBarInfo)
    ON_BN_CLICKED(ID_RADIAL_PROGRESS_ROUND, OnChangeRadialProgressBarInfo)

    ON_BN_CLICKED(ID_SPRITE_AUTO, OnSpriteStyleChange)
    ON_BN_CLICKED(ID_SPRITE_LOOP, OnSpriteStyleChange)
    ON_BN_CLICKED(ID_SPRITE_EDIT_FRAMES, OnEditSpriteFrames)

    ON_CBN_SELCHANGE(ID_SCROLL_THUMB_COLOR, OnScrollAppearanceChange)
    ON_CBN_SELCHANGE(ID_SCROLL_THUMB_BORDER_COLOR, OnScrollAppearanceChange)
    ON_CBN_SELCHANGE(ID_SCROLL_BUTTON_COLOR, OnScrollAppearanceChange)
    ON_CBN_SELCHANGE(ID_SCROLL_THUMB_BORDER_STYLE, OnScrollAppearanceChange)
    ON_BN_CLICKED(ID_SCROLL_END_BUTTONS, OnScrollStyleChange)
    ON_BN_CLICKED(ID_SCROLL_TILE_BKGND, OnScrollStyleChange)
    ON_BN_CLICKED(ID_SCROLL_RELATIVE_THUMB, OnScrollStyleChange)

    ON_BN_CLICKED(ID_TRANSPARENT, OnChangeTransparent)
    ON_BN_CLICKED(ID_SELECTED, OnChangeSelected)
    ON_BN_CLICKED(ID_ENABLED, OnChangeEnabled)
    ON_BN_CLICKED(ID_TEXT_COPY, OnChangeTextCopy)
    ON_BN_CLICKED(ID_ML_TEXT_VCENTER, OnChangeMLTextVCenter)
    ON_BN_CLICKED(ID_ACCEPTS_FOCUS, OnChangeFocus)
    ON_BN_CLICKED(ID_TILE_WALLPAPER, OnChangeTileWallpaper)
    ON_BN_CLICKED(ID_BUTTON_STYLE_PUSHED, OnButtonStyleChange)
    ON_BN_CLICKED(ID_BUTTON_STYLE_TOGGLE, OnButtonStyleChange)
    ON_BN_CLICKED(ID_BUTTON_STYLE_RADIO, OnButtonStyleChange)
    ON_BN_CLICKED(ID_BUTTON_STYLE_REPEAT, OnButtonStyleChange)
    ON_BN_CLICKED(ID_LIST_WRAP, OnChangeListWrap)
    ON_BN_CLICKED(ID_LIST_REPEAT_SELECT, OnChangeListRepeatSelect)
    ON_BN_CLICKED(ID_TEMPLATE, OnChangeTemplate)
    ON_BN_CLICKED(ID_VISIBLE_AT_STARTUP, OnChangeVisibleAtStartup)
    ON_BN_CLICKED(ID_TEXT_INPUT_READONLY, OnChangeReadonly)

    ON_EN_KILLFOCUS(ID_GAUGE_ANIMATION_STEPS, OnChangeGaugeInfo)
    ON_EN_KILLFOCUS(ID_GAUGE_ANIMATION_DELAY, OnChangeGaugeInfo)
    ON_EN_KILLFOCUS(ID_GAUGE_NEEDLE_XPOS, OnChangeGaugeInfo)
    ON_EN_KILLFOCUS(ID_GAUGE_NEEDLE_YPOS, OnChangeGaugeInfo)
    ON_EN_KILLFOCUS(ID_GAUGE_NEEDLE_XCOR, OnChangeGaugeInfo)
    ON_EN_KILLFOCUS(ID_GAUGE_NEEDLE_YCOR, OnChangeGaugeInfo)

    ON_EN_KILLFOCUS(ID_CHART_MIN_VAL, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_MAX_VAL, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_LEFT_MARGIN, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_TOP_MARGIN, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_RIGHT_MARGIN, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_BOTTOM_MARGIN, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_MAX_COUNT, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_AXIS_WIDTH, OnChangeChartInfo)
    ON_EN_KILLFOCUS(ID_CHART_LINE_WIDTH, OnChangeChartInfo)
    ON_CBN_SELCHANGE(ID_CHART_AXIS_COLOR, OnChangeChartInfo)
    ON_CBN_SELCHANGE(ID_CHART_LINE_COLOR, OnChangeChartInfo)

    ON_EN_KILLFOCUS(ID_SCROLL_WHEEL_SELECTED_ROW, OnChangeScrollWheelProps)
    ON_EN_KILLFOCUS(ID_SCROLL_WHEEL_ROW_HEIHGT, OnChangeScrollWheelProps)
    ON_EN_KILLFOCUS(ID_SCROLL_WHEEL_TOTAL_ROWS, OnChangeScrollWheelProps)
    ON_EN_KILLFOCUS(ID_SCROLL_WHEEL_START_ALPHA, OnChangeScrollWheelProps)
    ON_EN_KILLFOCUS(ID_SCROLL_WHEEL_END_ALPHA, OnChangeScrollWheelProps)

    ON_CBN_SELCHANGE(ID_TEXT_SCROLL_WHEEL_NORMAL_FONT, OnChangeTextScrollWheelProps)
    ON_CBN_SELCHANGE(ID_TEXT_SCROLL_WHEEL_SELECTED_FONT, OnChangeTextScrollWheelProps)
    ON_BN_CLICKED(ID_TEXT_SCROLL_WHEEL_ROUND, OnChangeTextScrollWheelProps)

    ON_BN_CLICKED(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS, OnEditStringScrolWheelStrings)

    ON_EN_KILLFOCUS(ID_NUMERIC_SCROLL_WHEEL_START_VAL, OnChangeNumericScrollWheelRange)
    ON_EN_KILLFOCUS(ID_NUMERIC_SCROLL_WHEEL_END_VAL, OnChangeNumericScrollWheelRange)

    ON_BN_CLICKED(ID_MENU_EXPANDED, OnChangeMenuInfo)
    ON_BN_CLICKED(ID_MENU_INSERT_MODE, OnChangeMenuInfo)
    ON_EN_KILLFOCUS(ID_MENU_TEXT_X_OFFSET, OnChangeMenuInfo)
    ON_EN_KILLFOCUS(ID_MENU_TEXT_Y_OFFSET, OnChangeMenuInfo)

    ON_EN_KILLFOCUS(ID_TREE_VIEW_INDENTATION, OnChangeTreeViewInfo)
    ON_BN_CLICKED(ID_TREE_VIEW_SHOW_ROOT_LINES, OnChangeTreeViewInfo)
    ON_CBN_SELCHANGE(ID_TREE_VIEW_ROOT_LINE_COLOR, OnChangeTreeViewInfo)
    ON_BN_CLICKED(ID_RICH_TEXT_VIEW_TEXT_EDIT, OnEditRichTextViewText)

    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_WM_LBUTTONDOWN()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_SETTINGCHANGE()
    END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
properties_win::properties_win()
{
    m_top_pos = 0;
    mpInfo = NULL;
    mpProject = NULL;
    m_scroll_helper = new CScrollHelper;
    m_scroll_helper->AttachWnd(this);

    SetControlSize();
}

///////////////////////////////////////////////////////////////////////////////
properties_win::~properties_win()
{
    delete m_scroll_helper;
}

void properties_win::SetControlSize()
{
    int dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();
    m_item_height = GetScaledValue(PROPERTY_ITEM_HEIGHT, dpi, text_scaler);
    m_row_height = m_item_height + PROPERTY_ITEM_SPACE;
    m_leftcol_width = GetScaledValue(LEFT_COLUMN_WIDTH, dpi, text_scaler);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnTabKey()
{
    int nShiftKey = GetKeyState(VK_SHIFT);
    int bPrevious = FALSE;

    if(nShiftKey & KEY_STATE_DOWN)
    {
        bPrevious = TRUE;
    }

    CWnd *next = GetNextDlgTabItem(GetFocus(), bPrevious);

    if (next)
    {
        CRect size;
        next->GetWindowRect(&size);
        ScreenToClient(&size);

        CRect client;
        GetClientRect(&client);

        int shift = 0;

        // test if next focus owner visible
        if (size.top < client.top)
        {
            shift = size.top - client.top;
        }
        else if (size.bottom > client.bottom)
        {
            shift = size.bottom - client.bottom;
        }

        // Scroll to make next focus owner visible
        if (shift)
        {
            m_scroll_helper->Scroll(SB_VERT, shift);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnPageUp()
{
    CRect client;

    GetClientRect(&client);

    m_scroll_helper->Scroll(SB_VERT, -client.Height());

    CWnd* next = GetFocus();
    CRect size;

    // If current focus owner is not visible,
    // try to move focus to next visible tab item
    while (next)
    {
        //Get control size
        next->GetWindowRect(&size);
        ScreenToClient(&size);

        if (size.bottom > client.bottom)
        {
            next = GetNextDlgTabItem(next, TRUE);
        }
        else
        {
            next->SetFocus();
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnPageDown()
{
    CRect client;

    GetClientRect(&client);

    m_scroll_helper->Scroll(SB_VERT, client.Height());

    CWnd* next = GetFocus();
    CRect size;

    // If current focus owner is not visible,
    // try to move focus to next visible tab item
    while (next)
    {
        //Get control size
        next->GetWindowRect(&size);
        ScreenToClient(&size);

        if (size.top < client.top)
        {
            next = GetNextDlgTabItem(next, FALSE);
        }
        else
        {
            next->SetFocus();
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL properties_win::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_MOUSEWHEEL)
    {
        TCHAR  class_name[MAX_PATH];
        GetClassName(pMsg->hwnd, class_name, MAX_PATH - 1);

        if (_tccmp(class_name, _T("ComboBox")) == 0)
        {
            PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
            return TRUE;
        }

    }
    else if (pMsg->message == WM_KEYDOWN)
    {
        CWnd *focus_owner = GetFocus();
        switch (pMsg->wParam)
        {
        case VK_RETURN:
            if (focus_owner && (GetDlgItem(ID_STRING_VAL) == focus_owner))
            {
                SendMessage(WM_COMMAND, MAKEWPARAM(focus_owner->GetDlgCtrlID(), EN_KILLFOCUS), (LPARAM)(focus_owner->m_hWnd));
                return TRUE;
            }
            break;

        case VK_TAB:
            OnTabKey();
            break;

        case VK_PRIOR:
            OnPageUp();
            return TRUE;

        case VK_NEXT:
            OnPageDown();
            return TRUE;

        default:
            break;
        }
    }
    else if(pMsg->message == WM_SYSKEYDOWN)
    {
        int nAltKey = GetKeyState(VK_MENU);

        switch (pMsg->wParam)
        {
        case VK_UP:
        case VK_LEFT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //move focus to project view
                project_view* view = GetProjectView();
                if (view)
                {
                    view->SetFocus();
                    return TRUE;
                }
            }
            break;

        case VK_DOWN:
        case VK_RIGHT:
            if (nAltKey & KEY_STATE_DOWN)
            {
                //Move focus to target view
                target_view* view = GetTargetView();
                if (view)
                {
                    CMainFrame* frame = (CMainFrame*)GetParentFrame();
                    frame->SetActiveView(view);
                    return TRUE;
                }
            }
            break;

        default:
            break;
        }
    }
    
    return CDialog::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
int properties_win::OnCreate(LPCREATESTRUCT lpcs)
{
    CWnd::OnCreate(lpcs);

    CMainFrame *frame = (CMainFrame *) GetParentFrame();
    frame ->SetPropsWin(this);
    SetWindowText(_T("Properties Win"));
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
BOOL properties_win::OnEraseBkgnd(CDC* pDC)
{
      // Set brush to desired background color
      CBrush backBrush(RGB(210, 210, 210));

      // Save old brush
      CBrush* pOldBrush = pDC->SelectObject(&backBrush);

      CRect rect;
      pDC->GetClipBox(&rect);     // Erase the area needed

      pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),
          PATCOPY);
      pDC->SelectObject(pOldBrush);
      return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scroll_helper->OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////////////////
BOOL properties_win::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    BOOL wasScrolled = m_scroll_helper->OnMouseWheel(nFlags, zDelta, pt);
    return wasScrolled;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SetFocus();
    CDialog::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    SetFocus();
    CDialog::OnNcLButtonDown(nHitTest, point);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    m_scroll_helper->OnSize(nType, cx, cy);
    OnWidgetSelect(mpInfo);
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::OnPaint()
{
    CDC *dc = GetDC();
    CRect rect;

    CDialog::OnPaint();
    ReleaseDC(dc);
}

///////////////////////////////////////////////////////////////////////////////
HBRUSH properties_win::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    int nCtrlId = pWnd->GetDlgCtrlID();    

    HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}

///////////////////////////////////////////////////////////////////////////////
BOOL properties_win::OnCommand(WPARAM wParam, LPARAM lParam)
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
    return CWnd::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == 0x0d)
    {
        CWnd *pctrl = GetFocus();
        int ctrl_id = pctrl->GetDlgCtrlID();

        switch(ctrl_id)
        {
        case ID_WIDGET_NAME:
        case ID_WIDGET_ID:
        case ID_EVENT_FUNCTION:
        case ID_DRAW_FUNCTION:
            OnChangeWidgetName();
            break;

        case ID_USER_DATA:
            OnChangeWidgetUserData();
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AssignChildFont()
{
    CWnd *child = GetWindow(GW_CHILD);

    while(child)
    {
        child->SetFont(&NormalFont, FALSE);
        child->ShowWindow(SW_SHOW);

        child = child->GetNextWindow();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::DestroyChildren()
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
void properties_win::AddComboBox(char *label, int res_type, int id, ULONG current_val)
{
    int right;
    int list_index;
    CRect size;
    CString cs(label);
    GetClientRect(&size);

    size.top += m_top_pos;
    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    right = size.right;
    size.right = size.left + m_leftcol_width;
    size.bottom = size.top + m_item_height;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD | SS_LEFT, size, this);

    size.top -= 2;
    size.left = size.right + 1;
    size.right = right;
    size.bottom = size.top + 200;

    express_combobox *pBox = new express_combobox();
    pBox->Create(WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | SORT_FLAG, size, this, id);
    pBox->SetItemHeight(-1, m_item_height - 2);

    int active_display = GetProjectView()->GetActiveDisplay();
    int res_id;
    int start_id = 0;
    int end_id = mpProject->CountResources(active_display, res_type);

    if (res_type == RES_TYPE_PIXELMAP)
    {
        start_id++;
        end_id++;
    }
  
    int active_theme = mpProject->mDisplays[active_display].active_theme;

    for (res_id = start_id; res_id < end_id; res_id++)
    {
        res_info *info = mpProject->FindResource(active_display, active_theme, res_type, res_id);

        if (info)
        {
            list_index = pBox->AddString(info->name);
            pBox->SetItemData(list_index, res_id);
        }
    }

    if (res_type == RES_TYPE_PIXELMAP)
    {
        pBox->InsertString(0, _T("None"));
        pBox->SetItemData(0, 0);
    }

    SelectDropListItem(pBox, current_val);
    m_top_pos += m_row_height;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddStringSelect(int current_val)
{
    int right;
    int box_index;
    CRect size;
    CString cs("String ID");
    GetClientRect(&size);

    size.top += m_top_pos;
    size.left +=  PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    right = size.right;
    size.right = size.left + m_leftcol_width;
    size.bottom = size.top + m_item_height;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.top -= 2;
    size.left = size.right + 1;
    size.right = right;
    size.bottom = size.top + 200;

    CComboBox *pBox = new CComboBox();
    pBox->Create(WS_CHILD|WS_TABSTOP|WS_VSCROLL|CBS_DROPDOWNLIST|SORT_FLAG, size, this, ID_STRING_ID);

    int string_id = 1;
    string_table *table = GetActiveStringTable();

    int string_ref = 0;

    if (table)
    {
        
        if (current_val >= 0 && current_val < table->CountStrings())
        {
            string_ref = current_val;
        }

        while(string_id < table->CountStrings())
        {
            box_index = pBox->AddString(table->GetResourceIdName(string_id));
            pBox->SetItemData(box_index, string_id);
            string_id++;
        }   
        CString id_name = table->GetResourceIdName(string_ref);
        m_top_pos += m_row_height;
        AddRichEditControl("Text", table->GetString(id_name), ID_STRING_VAL);
    }

    pBox->InsertString(0, _T("None"));
    pBox->SetItemData(0, 0);

    SelectDropListItem(pBox, string_ref);
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddComboBox(char *label, STRING_VAL_PAIR *list, int id,
    ULONG current_val)
{
    int right;
    CRect size;
    CString cs(label);
    GetClientRect(&size);

    size.top += m_top_pos;
    size.left +=  PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    right = size.right;
    size.right = size.left + m_leftcol_width;
    size.bottom = size.top + m_item_height;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.top -= 2;
    size.left = size.right + 1;
    size.right = right;
    size.bottom = size.top + 200;

    express_combobox *pBox = new express_combobox();
    pBox->Create(WS_CHILD|WS_TABSTOP|CBS_DROPDOWNLIST, size, this, id);

    STRING_VAL_PAIR *current = list;
    int index;
    int sel_index = -1;

    while(current->name.GetLength())
    {
        index = pBox->AddString(current->name);
        pBox->SetItemData(index, current->val);

        if (current->val == current_val)
        {
            sel_index = index;
        }
        current++;
    }

    if (id == ID_RADIAL_SLIDER_ANIMATION_STYLE)
    {
        pBox->InsertString(0, _T("None"));
        pBox->SetItemData(0, 0);
    }

    SelectDropListItem(pBox, current_val);

    pBox->ShowWindow(SW_SHOW);

    m_top_pos += m_row_height;
}

///////////////////////////////////////////////////////////////////////////////
CButton *properties_win::AddRadioButton(char *description, ULONG checked, int id)
{
    int right;
    CRect size;
    CRect groupsize;
    CString cs(description);
    GetClientRect(&size);
    
    size.top += m_top_pos;
    size.left +=  PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    right = size.right;
    size.right = size.left + m_leftcol_width;
    size.bottom = size.top + m_item_height;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.left = size.right + 1;
    size.right = right;

    CButton *pButton = new CButton();
    pButton->Create(NULL, WS_CHILD|WS_TABSTOP|BS_AUTORADIOBUTTON, size, this, id);

    if (checked)
    {
        pButton->SetCheck(BST_CHECKED);
    }
    else
    {
        pButton->SetCheck(BST_UNCHECKED);
    }
    
    m_top_pos += m_row_height;
    return pButton;
}

///////////////////////////////////////////////////////////////////////////////
CButton *properties_win::AddCheckBox(char *description, ULONG checked, int id)
{
    CRect size;
    CString cs(description);
    GetClientRect(&size);
    
    size.top += m_top_pos;
    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    size.bottom = size.top + m_item_height;

    CButton *pButton = new CButton();
    pButton->Create(cs, WS_CHILD|WS_TABSTOP|BS_LEFTTEXT|BS_AUTOCHECKBOX, size, this, id);
    
    if (checked)
    {
        pButton->SetCheck(BST_CHECKED);
    }
    else
    {
        pButton->SetCheck(BST_UNCHECKED);
    }
    
    m_top_pos += m_row_height;
    return pButton;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddPushButton(char *description, int id)
{
    CRect size;
    CString cs(description);
    GetClientRect(&size);
    
    size.top += m_top_pos;
    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    size.bottom = size.top + m_item_height + 4;

    CButton *pButton = new CButton();
    pButton->Create(cs, WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, size, this, id);

    m_top_pos += m_row_height + 4;
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddEditControl(char *description, int value, int id)
{
    CString valstring;
    valstring.Format(_T("%d"), value); 
    AddEditControl(description, valstring, id);
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddEditControl(char *description, CString &name, int id)
{
    int right;
    CRect size;
    CString cs(description);
    GetClientRect(&size);

    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;

    right = size.right;
    
    size.top += m_top_pos;
    size.bottom = size.top + m_item_height;
    size.right = size.left + m_leftcol_width;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.left = size.right + 1;
    size.right = right;

    CEdit *pEdit = new CEdit();
    pEdit->Create(WS_CHILD|WS_TABSTOP|SS_LEFT|ES_AUTOHSCROLL, size, this, id);
    pEdit->SetWindowText(name);
    
    m_top_pos += m_row_height;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddRichEditControl(char *description, CString &name, int id)
{
    int right;
    CRect size;
    CString cs(description);
    GetClientRect(&size);

    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;

    right = size.right;
    
    size.top += m_top_pos;
    size.bottom = size.top + m_item_height;
    size.right = size.left + m_leftcol_width;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.left = size.right + 1;
    size.right = right;

    CRichEditCtrl *pEdit = new CRichEditCtrl();

    int style = WS_CHILD | WS_TABSTOP | SS_LEFT | ES_AUTOHSCROLL;
    if (id == ID_STRING_VAL)
    {
        style |= ES_MULTILINE | ES_AUTOVSCROLL;
    }
    pEdit->Create(style, size, this, id);
    //pEdit->SetWindowText(name);
    SetUtf8Text(pEdit, name);
    
    SetAccessibleHelpString(pEdit->GetSafeHwnd(), name);
    m_top_pos += m_row_height;
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddPromptControl(char *description, CString &name)
{
    int right;
    CRect size;
    CString cs(description);
    GetClientRect(&size);

    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;

    right = size.right;
    
    size.top += m_top_pos;
    size.bottom = size.top + m_item_height;
    size.right = size.left + m_leftcol_width;

    CStatic *pDesc = new CStatic();
    pDesc->Create(cs, WS_CHILD|SS_LEFT, size, this);

    size.left = size.right + 1;
    size.right = right;

    CStatic *pval = new CStatic();
    pval->Create(name, WS_CHILD|SS_LEFT, size, this);
    
    m_top_pos += m_row_height;
}

///////////////////////////////////////////////////////////////////////////////
CButton *properties_win::CreateGroup(char *caption, int rows)
{
    CRect size;
    CString cs(caption);
    GetClientRect(&size);

    size.left += PROPERTY_ITEM_SPACE;
    size.right -= PROPERTY_ITEM_SPACE;
    size.top += m_top_pos;
    size.bottom = size.top + ((rows + 1) * m_row_height);
    CButton *group = new CButton;
    group->Create(cs, WS_CHILD|BS_GROUPBOX, size, this, 0);
    m_group_pos = m_row_height;
    m_top_pos += ((rows + 2) * m_row_height);
    return group;
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddWidgetProps()
{
    CButton *box;

    AddPromptControl("Widget Type", mpInfo->base_name);
    AddEditControl("Widget Name", mpInfo->app_name, ID_WIDGET_NAME);
    AddEditControl("Widget Id", mpInfo->id_name, ID_WIDGET_ID);
    AddEditControl("User Data", mpInfo->user_data, ID_USER_DATA);

    if (mpInfo->basetype != GX_TYPE_RADIAL_PROGRESS_BAR)
    {
        AddEditControl("Left", mpInfo->size.gx_rectangle_left, ID_XPOS);
        AddEditControl("Top", mpInfo->size.gx_rectangle_top, ID_YPOS);
    }

    if (mpInfo->basetype != GX_TYPE_TEMPLATE)
    {
        if (mpInfo->basetype != GX_TYPE_RADIAL_PROGRESS_BAR)
        {
            AddEditControl("Width",
                mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1,
                ID_WIDTH);
            AddEditControl("Height",
                mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1,
                ID_HEIGHT);
        }

        AddComboBox("Border", border_types, ID_BORDER_COMBO,
            mpInfo->style & GX_STYLE_BORDER_MASK);

        AddCheckBox("Transparent",      mpInfo->style & GX_STYLE_TRANSPARENT, ID_TRANSPARENT);
        AddCheckBox("Draw Selected",    mpInfo->style & GX_STYLE_DRAW_SELECTED,    ID_SELECTED);
    }
    AddCheckBox("Enabled",          mpInfo->style & GX_STYLE_ENABLED,     ID_ENABLED);
    AddCheckBox("Accepts Focus",    mpInfo->accepts_focus,     ID_ACCEPTS_FOCUS);

    if (mpProject->mHeader.guix_version > 50000)
    {
        switch(mpInfo->allocation)
        {
        case STATICALLY_ALLOCATED:
            AddCheckBox("Runtime Allocate", FALSE, ID_DYNAMIC_WIDGET);
            break;

        case DYNAMIC_ALLOCATION_ROOT:
            AddCheckBox("Runtime Allocate", TRUE, ID_DYNAMIC_WIDGET);
            break;

        case DYNAMIC_ALLOCATION_CHILD:
            box = AddCheckBox("Runtime Allocate", TRUE, ID_DYNAMIC_WIDGET);
            box->EnableWindow(FALSE);
            break;

        default:
            ErrorMsg("Internal error: Invalid allocation parameter");
            break;
        }
    }

    if (mpInfo->basetype != GX_TYPE_TEMPLATE)
    {
        AddComboBox("Normal fill", RES_TYPE_COLOR, ID_NORMAL_COLOR, mpInfo->color_id[NORMAL_FILL_COLOR_INDEX]);
        AddComboBox("Selected fill", RES_TYPE_COLOR, ID_SELECTED_COLOR, mpInfo->color_id[SELECTED_FILL_COLOR_INDEX]);
        AddComboBox("Disabled fill", RES_TYPE_COLOR, ID_DISABLED_COLOR, mpInfo->color_id[DISABLED_FILL_COLOR_INDEX]);

        switch (mpInfo->basetype)
        {
        case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
        case GX_TYPE_MULTI_LINE_TEXT_INPUT:
            AddComboBox("Readonly fill", RES_TYPE_COLOR, ID_READONLY_COLOR, mpInfo->color_id[READONLY_FILL_COLOR_INDEX]);
            break;
        default:
            break;
        }
    }
    
    if (GetProjectView()->IsTopLevelWidget(mpInfo))
    {
        AddCheckBox("Template", mpInfo->is_template, ID_TEMPLATE);
        AddCheckBox("Visible At Startup", mpInfo->visible_at_startup, ID_VISIBLE_AT_STARTUP);
    }

    AddEditControl("Draw Function", mpInfo->draw_func, ID_DRAW_FUNCTION);
    AddEditControl("Event Function", mpInfo->event_func, ID_EVENT_FUNCTION);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddTextWidgetProperties(widget_info *info)
{
    AddStringSelect(info->string_id[0]);
    AddComboBox("Font", RES_TYPE_FONT, ID_FONT_SELECT_0, info->font_id[0]);
    AddComboBox("Text Align", text_align_styles, ID_TEXT_ALIGN, info->style & GX_STYLE_TEXT_ALIGNMENT_MASK);
    AddComboBox("Normal Text Color", RES_TYPE_COLOR, ID_NORMAL_TEXT_COLOR, info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    AddComboBox("Selected Text Color", RES_TYPE_COLOR, ID_SELECTED_TEXT_COLOR, info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    AddComboBox("Disabled Text Color", RES_TYPE_COLOR, ID_DISABLED_TEXT_COLOR, info->color_id[DISABLED_TEXT_COLOR_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddTextCopyProperty(widget_info *info)
{
    AddCheckBox("Private Text Copy", info->style & GX_STYLE_TEXT_COPY, ID_TEXT_COPY);
}

void properties_win::AddNumericPromptProps(widget_info *info)
{
    AddEditControl("Numeric Value", info->ewi.numeric_prompt_value, ID_NUMERIC_PROMPT_VALUE);
    AddEditControl("Format Func", info->format_func, ID_FORMAT_FUNC);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddTextInputProperties(widget_info *info)
{
    AddTextWidgetProperties(info);
    AddComboBox("Readonly Text Color", RES_TYPE_COLOR, ID_READONLY_TEXT_COLOR, info->color_id[READONLY_TEXT_COLOR_INDEX]);
    AddCheckBox("Read Only", info->style & GX_STYLE_TEXT_INPUT_READONLY, ID_TEXT_INPUT_READONLY);
    AddEditControl("Buffer Size", mpInfo->ewi.text_info.buffer_size, ID_BUFFER_SIZE);
    AddCheckBox("Dynamic Buffer", mpInfo->ewi.text_info.dynamic_buffer, ID_DYNAMIC_BUFFER);
    AddEditControl("Allocator",  mpInfo->callback_func, ID_CALLBACK_NAME);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddMLTextViewProperties(widget_info *info)
{
    AddTextWidgetProperties(info);
    AddTextCopyProperty(info);
    AddCheckBox("Center Text Vertically", info->style & GX_STYLE_VALIGN_CENTER, ID_ML_TEXT_VCENTER);
    AddEditControl("Whitespace", mpInfo->ewi.text_info.whitespace, ID_TEXT_VIEW_WHITESPACE);
    AddEditControl("Line Space", mpInfo->ewi.text_info.line_space, ID_TEXT_VIEW_LINE_SPACE);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddMLTextInputProperties(widget_info *info)
{
    AddTextWidgetProperties(info);
    AddComboBox("Readonly Text Color", RES_TYPE_COLOR, ID_READONLY_TEXT_COLOR, info->color_id[READONLY_TEXT_COLOR_INDEX]);
    AddCheckBox("Read Only", info->style & GX_STYLE_TEXT_INPUT_READONLY, ID_TEXT_INPUT_READONLY);
    AddEditControl("Whitespace", mpInfo->ewi.text_info.whitespace, ID_TEXT_VIEW_WHITESPACE);
    AddEditControl("Line Space", mpInfo->ewi.text_info.line_space, ID_TEXT_VIEW_LINE_SPACE);

    AddEditControl("Buffer Size", mpInfo->ewi.text_info.buffer_size, ID_BUFFER_SIZE);
    AddCheckBox("Dynamic Buffer", mpInfo->ewi.text_info.dynamic_buffer, ID_DYNAMIC_BUFFER);
    AddEditControl("Allocator",  mpInfo->callback_func, ID_CALLBACK_NAME);
}

void properties_win::AddRichTextViewProperties(widget_info *info)
{
    AddStringSelect(info->string_id[0]);
    AddPushButton("Edit Rich Text", ID_RICH_TEXT_VIEW_TEXT_EDIT);

    AddComboBox("Text Align", text_align_styles, ID_TEXT_ALIGN, info->style & GX_STYLE_TEXT_ALIGNMENT_MASK);
    AddComboBox("Normal Font", RES_TYPE_FONT, ID_FONT_SELECT_0, info->font_id[RICH_TEXT_FONT_NORMAL]);
    AddComboBox("Bold Font", RES_TYPE_FONT, ID_FONT_SELECT_1, info->font_id[RICH_TEXT_FONT_BOLD]);
    AddComboBox("Italic Font", RES_TYPE_FONT, ID_FONT_SELECT_2, info->font_id[RICH_TEXT_FONT_ITALIC]);
    AddComboBox("Bold Italic Font", RES_TYPE_FONT, ID_FONT_SELECT_3, info->font_id[RICH_TEXT_FONT_BOLD_ITALIC]);

    AddComboBox("Normal Text Color", RES_TYPE_COLOR, ID_NORMAL_TEXT_COLOR, info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    AddComboBox("Selected Text Color", RES_TYPE_COLOR, ID_SELECTED_TEXT_COLOR, info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    AddComboBox("Disabled Text Color", RES_TYPE_COLOR, ID_DISABLED_TEXT_COLOR, info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    AddTextCopyProperty(info);
    AddEditControl("Whitespace", mpInfo->ewi.text_info.whitespace, ID_TEXT_VIEW_WHITESPACE);
    AddEditControl("Line Space", mpInfo->ewi.text_info.line_space, ID_TEXT_VIEW_LINE_SPACE);
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddButtonStyles(widget_info *info)
{
    AddCheckBox("Pushed", info->style & GX_STYLE_BUTTON_PUSHED, ID_BUTTON_STYLE_PUSHED);
    AddCheckBox("Toggle", info->style & GX_STYLE_BUTTON_TOGGLE, ID_BUTTON_STYLE_TOGGLE);
    AddCheckBox("Radio",  info->style & GX_STYLE_BUTTON_RADIO,  ID_BUTTON_STYLE_RADIO);
    AddCheckBox("Auto Repeat",  info->style & GX_STYLE_BUTTON_REPEAT,  ID_BUTTON_STYLE_REPEAT);

    /* FIXME get these added at some point */
    //#define GX_STYLE_BUTTON_EVENT_ON_PUSH           0x00000080
}


///////////////////////////////////////////////////////////////////////////////
// for older GUIX library versions, the pixelmap alignment styles may not be
// used. Set them to a reasonable default:
INT properties_win::ValidateAlignStyle(INT style)
{
   if ((style & GX_PIXELMAP_HALIGN_MASK) == 0)
    {
        style |= GX_STYLE_HALIGN_LEFT;
    }

    if ((style & GX_PIXELMAP_VALIGN_MASK) == 0)
    {
        style |= GX_STYLE_VALIGN_TOP;
    }
    return style;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddPixelmapButtonProps(widget_info *info)
{
    AddButtonStyles(info);
    AddComboBox("Normal Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    AddComboBox("Selected Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
    AddComboBox("Disabled Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[DISABLED_PIXELMAP_INDEX]);

    INT style = ValidateAlignStyle(info->style);
 
    AddComboBox("Horizontal Align", button_halign_styles, ID_PIXMAP_HALIGN, style & GX_PIXELMAP_HALIGN_MASK);
    AddComboBox("Vertical Align", button_valign_styles, ID_PIXMAP_VALIGN, style & GX_PIXELMAP_VALIGN_MASK);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddCheckboxProps(widget_info *info)
{
    AddButtonStyles(info);
    AddTextWidgetProperties(info);
    AddTextCopyProperty(info);

    AddComboBox("Unchecked Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[UNCHECKED_PIXELMAP_INDEX]);
    AddComboBox("Checked Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[CHECKED_PIXELMAP_INDEX]);
    AddComboBox("Unchecked Disabled Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[UNCHECKED_DISABLED_PIXELMAP_INDEX]);
    AddComboBox("Checked Disabled Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_3, info->pixelmap_id[CHECKED_DISABLED_PIXELMAP_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddRadioButtonProps(widget_info *info)
{
    AddButtonStyles(info);
    AddTextWidgetProperties(info);
    AddTextCopyProperty(info);

    AddComboBox("Radio Off Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[RADIO_OFF_PIXELMAP_INDEX]);
    AddComboBox("Radio On Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[RADIO_ON_PIXELMAP_INDEX]);
    AddComboBox("Radio Off Disabled Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[RADIO_OFF_DISABLED_PIXELMAP_INDEX]);
    AddComboBox("Radio On Disabled Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_3, info->pixelmap_id[RADIO_ON_DISABLED_PIXELMAP_INDEX]);

}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddIconButtonProps(widget_info *info)
{
    AddButtonStyles(info);
    AddComboBox("Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);

    INT style = ValidateAlignStyle(info->style);

    AddComboBox("Horizontal Align", button_halign_styles, ID_PIXMAP_HALIGN, style & GX_PIXELMAP_HALIGN_MASK);
    AddComboBox("Vertical Align", button_valign_styles, ID_PIXMAP_VALIGN, style & GX_PIXELMAP_VALIGN_MASK);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddIconProps(widget_info *info)
{
    AddComboBox("Normal Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    AddComboBox("Selected Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);

    INT style = ValidateAlignStyle(info->style);
 
    AddComboBox("Horizontal Align", button_halign_styles, ID_PIXMAP_HALIGN, style & GX_PIXELMAP_HALIGN_MASK);
    AddComboBox("Vertical Align", button_valign_styles, ID_PIXMAP_VALIGN, style & GX_PIXELMAP_VALIGN_MASK);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddPixelmapPromptProps(widget_info *info)
{
    AddTextWidgetProperties(info);

    AddComboBox("Norm Left PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[LEFT_PIXELMAP_INDEX]);
    AddComboBox("Norm Fill PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[FILL_PIXELMAP_INDEX]);
    AddComboBox("Norm Right PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[RIGHT_PIXELMAP_INDEX]);
    AddComboBox("Sel Left PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_3, info->pixelmap_id[SELECTED_LEFT_PIXELMAP_INDEX]);
    AddComboBox("Sel Fill PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_4, info->pixelmap_id[SELECTED_FILL_PIXELMAP_INDEX]);
    AddComboBox("Sel Right PixMap", RES_TYPE_PIXELMAP, ID_PIXELMAP_5, info->pixelmap_id[SELECTED_RIGHT_PIXELMAP_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddWindowProps(widget_info *info)
{
    AddComboBox("Wallpaper", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]);
    AddCheckBox("Tile Wallpaper", info->style & GX_STYLE_TILE_WALLPAPER, ID_TILE_WALLPAPER);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddVerticalListProps(widget_info *info)
{
    AddWindowProps(info);
    AddCheckBox("Wrap", info->style & GX_STYLE_WRAP, ID_LIST_WRAP);
    AddCheckBox("Repeat Select", info->style & GX_STYLE_REPEAT_SELECT, ID_LIST_REPEAT_SELECT);
    AddEditControl("Total Rows", info->ewi.vlist.total_rows, ID_LIST_ROWS);
    AddEditControl("Callback Function", info->callback_func, ID_CALLBACK_NAME);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddHorizontalListProps(widget_info *info)
{
    AddWindowProps(info);
    AddCheckBox("Wrap", info->style & GX_STYLE_WRAP, ID_LIST_WRAP);
    AddCheckBox("Repeat Select", info->style & GX_STYLE_REPEAT_SELECT, ID_LIST_REPEAT_SELECT);
    AddEditControl("Total Columns", info->ewi.vlist.total_rows, ID_LIST_ROWS);
    AddEditControl("Callback Function", info->callback_func, ID_CALLBACK_NAME);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::AddDropListProps(widget_info *info)
{
    AddComboBox("List Wallpaper", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]);
    AddCheckBox("Tile List Wallpaper", info->style & GX_STYLE_TILE_WALLPAPER, ID_TILE_DROP_LIST_POPLIST_WALLPAPER);
    AddEditControl("Total Rows", info->ewi.drop_list.total_rows, ID_LIST_ROWS);
    AddEditControl("Open Height", info->ewi.drop_list.open_height, ID_OPEN_HEIGHT);
    AddComboBox("Background Image", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[DROP_LIST_PIXELMAP_INDEX]);
    AddCheckBox("Tile Background", info->style & GX_STYLE_TILE_BACKGROUND, ID_TILE_DROP_LIST_BACKGROUND);

    AddEditControl("Callback Function", info->callback_func, ID_CALLBACK_NAME);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddChartProps(widget_info *info)
{
    AddWindowProps(info);

    AddEditControl("Data Min Value", info->ewi.line_chart_info.gx_line_chart_min_val, ID_CHART_MIN_VAL);
    AddEditControl("Data Max Value", info->ewi.line_chart_info.gx_line_chart_max_val, ID_CHART_MAX_VAL);
    AddEditControl("Left Margin", info->ewi.line_chart_info.gx_line_chart_left_margin, ID_CHART_LEFT_MARGIN);
    AddEditControl("Top Margin", info->ewi.line_chart_info.gx_line_chart_top_margin, ID_CHART_TOP_MARGIN);
    AddEditControl("Right Margin", info->ewi.line_chart_info.gx_line_chart_right_margin, ID_CHART_RIGHT_MARGIN);
    AddEditControl("Bottom Margin", info->ewi.line_chart_info.gx_line_chart_bottom_margin, ID_CHART_BOTTOM_MARGIN);
    AddEditControl("Num Data Points", info->ewi.line_chart_info.gx_line_chart_max_data_count, ID_CHART_MAX_COUNT);
    AddEditControl("Axis Line Width", info->ewi.line_chart_info.gx_line_chart_axis_line_width, ID_CHART_AXIS_WIDTH);
    AddEditControl("Data Line Width", info->ewi.line_chart_info.gx_line_chart_data_line_width, ID_CHART_LINE_WIDTH);
    AddComboBox("Axis Color", RES_TYPE_COLOR, ID_CHART_AXIS_COLOR, info->ewi.line_chart_info.gx_line_chart_axis_color);
    AddComboBox("Data Line Color", RES_TYPE_COLOR, ID_CHART_LINE_COLOR, info->ewi.line_chart_info.gx_line_chart_line_color);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddScrollWheelProps(widget_info *info)
{
    AddWindowProps(info);

    AddCheckBox("Wrap", info->style & GX_STYLE_WRAP, ID_LIST_WRAP);
    AddComboBox("Selected Background", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]);

    // The total rows of numeric scroll wheell is determined by value range
    AddEditControl("Total Rows", info->ewi.scroll_wheel.total_rows, ID_SCROLL_WHEEL_TOTAL_ROWS);

    AddEditControl("Selected Row", info->ewi.scroll_wheel.selected_row, ID_SCROLL_WHEEL_SELECTED_ROW);
    AddEditControl("Row Height", info->ewi.scroll_wheel.row_height, ID_SCROLL_WHEEL_ROW_HEIHGT);
    AddEditControl("Fade Edge Value", info->ewi.scroll_wheel.start_alpha, ID_SCROLL_WHEEL_START_ALPHA);
    AddEditControl("Fade Center Value", info->ewi.scroll_wheel.end_alpha, ID_SCROLL_WHEEL_END_ALPHA);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddMenuProps(widget_info *info)
{
    AddCheckBox("Expanded", info->style & GX_STYLE_MENU_EXPANDED, ID_MENU_EXPANDED);
    AddCheckBox("Insert As Menu Item", info->ewi.menu.insert_as_menu_item, ID_MENU_INSERT_MODE);
    AddEditControl("Text x Offset", info->ewi.menu.text_x_offset, ID_MENU_TEXT_X_OFFSET);
    AddEditControl("Text y Offset", info->ewi.menu.text_y_offset, ID_MENU_TEXT_Y_OFFSET);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddTextScrollWheelProps(widget_info *info)
{
    //Add scroll wheel properties
    AddScrollWheelProps(info);

    // Add extended properties
    AddCheckBox("Rounding Transform", info->style & GX_STYLE_TEXT_SCROLL_WHEEL_ROUND, ID_TEXT_SCROLL_WHEEL_ROUND);
    AddComboBox("Normal Font", RES_TYPE_FONT, ID_TEXT_SCROLL_WHEEL_NORMAL_FONT, info->font_id[NORMAL_FONT_INDEX]);
    AddComboBox("Selected Font", RES_TYPE_FONT, ID_TEXT_SCROLL_WHEEL_SELECTED_FONT, info->font_id[SELECTED_FONT_INDEX]);
    AddComboBox("Text Align", text_align_styles, ID_TEXT_ALIGN, info->style & GX_STYLE_TEXT_ALIGNMENT_MASK);
    AddComboBox("Normal Text Color", RES_TYPE_COLOR, ID_NORMAL_TEXT_COLOR, info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    AddComboBox("Selected Text Color", RES_TYPE_COLOR, ID_SELECTED_TEXT_COLOR, info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    AddComboBox("Disabled Text Color", RES_TYPE_COLOR, ID_DISABLED_TEXT_COLOR, info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    int display_index = mpProject->GetDisplayIndex(info);

    if (display_index < 0)
    {
        // Invalid display index
        return;
    }

    if (mpProject->mDisplays[display_index].colorformat <= GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        // Background fading feature is not supported in low depth color format
        GetDlgItem(ID_SCROLL_WHEEL_START_ALPHA)->EnableWindow(FALSE);
        GetDlgItem(ID_SCROLL_WHEEL_END_ALPHA)->EnableWindow(FALSE);

        // Rounding transform feature is not supported in low depth color format
        GetDlgItem(ID_TEXT_SCROLL_WHEEL_ROUND)->EnableWindow(FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddStringScrollWheelProps(widget_info *info)
{
    // Add text scroll wheel properties
    AddTextScrollWheelProps(info);

    // Add extended properties
    AddTextCopyProperty(info);
    AddEditControl("String Get Callback", mpInfo->callback_func, ID_CALLBACK_NAME);
    AddPushButton("Edit Strings", ID_STRING_SCROLL_WHEEL_EDIT_STRINGS);

    if (mpInfo->callback_func.IsEmpty())
    {
        GetDlgItem(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS)->EnableWindow(FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddNumericScrollWheelProps(widget_info *info)
{
    // Add text scroll wheel properties
    AddTextScrollWheelProps(info);

    AddEditControl("Start Val", mpInfo->ewi.numeric_scroll_wheel.start_val, ID_NUMERIC_SCROLL_WHEEL_START_VAL);
    AddEditControl("End Val", mpInfo->ewi.numeric_scroll_wheel.end_val, ID_NUMERIC_SCROLL_WHEEL_END_VAL);
    AddEditControl("Value Format Callback", mpInfo->callback_func, ID_CALLBACK_NAME);
}

void properties_win::AddGenericScrollWheelProps(widget_info* info)
{
    AddScrollWheelProps(info);

    AddEditControl("Callback Function", info->callback_func, ID_CALLBACK_NAME);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddTreeViewProps(widget_info *info)
{
    AddWindowProps(info);

    AddComboBox("Root Line Color", RES_TYPE_COLOR, ID_TREE_VIEW_ROOT_LINE_COLOR, info->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX]);
    AddCheckBox("Show Root Lines", info->style & GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES, ID_TREE_VIEW_SHOW_ROOT_LINES);
    AddEditControl("Indentation", info->ewi.tree_view_indentation, ID_TREE_VIEW_INDENTATION);
    AddComboBox("Expand Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[TREE_VIEW_EXPAND_PIXELMAP_INDEX]);
    AddComboBox("Collapse Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[TREE_VIEW_COLLAPSE_PIXELMAP_INDEX]);

}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddSliderProps(widget_info *info)
{
    AddCheckBox("Vertical", info->style & GX_STYLE_SLIDER_VERTICAL, ID_SLIDER_VERTICAL);
    
    if (info->basetype == GX_TYPE_SLIDER)
    {
        AddCheckBox("Show Tickmarks", info->style & GX_STYLE_SHOW_TICKMARKS, ID_SHOW_TICKMARKS);
        AddEditControl("Tickmark Count", info->misc_value, ID_TICKMARK_COUNT);
    }

    AddEditControl("Min Value", info->ewi.slider.gx_slider_info_min_val, ID_SLIDER_MINVAL);
    AddEditControl("Max Value", info->ewi.slider.gx_slider_info_max_val, ID_SLIDER_MAXVAL);
    AddEditControl("Current Value", info->ewi.slider.gx_slider_info_current_val, ID_SLIDER_CURRENTVAL);
    AddEditControl("Increment", info->ewi.slider.gx_slider_info_increment, ID_SLIDER_INCREMENT);
    AddEditControl("Min Travel", info->ewi.slider.gx_slider_info_min_travel, ID_SLIDER_MINTRAVEL);
    AddEditControl("Max Travel", info->ewi.slider.gx_slider_info_max_travel, ID_SLIDER_MAXTRAVEL);
    AddEditControl("Needle Width", info->ewi.slider.gx_slider_info_needle_width, ID_SLIDER_NEEDLE_WIDTH);
    AddEditControl("Needle Height", info->ewi.slider.gx_slider_info_needle_height, ID_SLIDER_NEEDLE_HEIGHT);
    AddEditControl("Needle Inset", info->ewi.slider.gx_slider_info_needle_inset, ID_SLIDER_NEEDLE_INSET);
    AddEditControl("Needle Hotspot", info->ewi.slider.gx_slider_info_needle_hotspot_offset, ID_NEEDLE_HOTSPOT);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddPixelmapSliderProps(widget_info *info)
{
    AddSliderProps(info);
    AddCheckBox("Tile Background", info->style & GX_STYLE_TILE_BACKGROUND, ID_SLIDER_TILE_BKGND);
    AddComboBox("Needle Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
    AddComboBox("Lower Background", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[LOWER_PIXELMAP_INDEX]);
    AddComboBox("Upper Background", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[UPPER_PIXELMAP_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddProgressBarProps(widget_info *info)
{
    AddCheckBox("Vertical", info->style & GX_STYLE_PROGRESS_VERTICAL, ID_PROGRESS_VERTICAL);
    AddCheckBox("Show Text", info->style & GX_STYLE_PROGRESS_TEXT_DRAW, ID_PROGRESS_SHOW_TEXT);
    AddCheckBox("Show Percentage", info->style & GX_STYLE_PROGRESS_PERCENT, ID_PROGRESS_PERCENT);
    
    AddEditControl("Min Value", info->ewi.progress.min_val, ID_PROGRESS_MINVAL);
    AddEditControl("Max Value", info->ewi.progress.max_val, ID_PROGRESS_MAXVAL);
    AddEditControl("Current Value", info->ewi.progress.current_val, ID_PROGRESS_CURRENTVAL);

    AddComboBox("Font", RES_TYPE_FONT, ID_FONT_SELECT_0, info->font_id[0]);
    AddComboBox("Normal Text Color", RES_TYPE_COLOR, ID_NORMAL_TEXT_COLOR, info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    AddComboBox("Selected Text Color", RES_TYPE_COLOR, ID_SELECTED_TEXT_COLOR, info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    AddComboBox("Disabled Text Color", RES_TYPE_COLOR, ID_DISABLED_TEXT_COLOR, info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    AddComboBox("Fill Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddRadialProgressBarProps(widget_info *info)
{
    GX_RADIAL_PROGRESS_BAR_INFO *bar_info = &info->ewi.radial_progress;

    AddCheckBox("Show Text", info->style & GX_STYLE_PROGRESS_TEXT_DRAW, ID_RADIAL_PROGRESS_SHOW_TEXT);
    AddCheckBox("Show Percentage", info->style & GX_STYLE_PROGRESS_PERCENT, ID_RADIAL_PROGRESS_PERCENT);
    AddCheckBox("Show Background Track", !(info->style & GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK), ID_RADIAL_PROGRESS_BACKTRACK);
    AddCheckBox("Alias", info->style & GX_STYLE_RADIAL_PROGRESS_ALIAS, ID_RADIAL_PROGRESS_ALIAS);
    AddCheckBox("Round End", info->style & GX_STYLE_RADIAL_PROGRESS_ROUND, ID_RADIAL_PROGRESS_ROUND);
    AddEditControl("X Center", bar_info->gx_radial_progress_bar_info_xcenter, ID_RADIAL_PROGRESS_XCENTER);
    AddEditControl("Y Center", bar_info->gx_radial_progress_bar_info_ycenter, ID_RADIAL_PROGRESS_YCENTER);
    AddEditControl("Radius", bar_info->gx_radial_progress_bar_info_radius, ID_RADIAL_PROGRESS_RADIUS);
    AddEditControl("Anchor Value", bar_info->gx_radial_progress_bar_info_anchor_val, ID_RADIAL_PROGRESS_ANCHORVAL);
    AddEditControl("Current Value", bar_info->gx_radial_progress_bar_info_current_val, ID_RADIAL_PROGRESS_CURRENTVAL);
    AddEditControl("Normal Brush Width", bar_info->gx_radial_progress_bar_info_normal_brush_width, ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH);
    AddEditControl("Selected Brush Width", bar_info->gx_radial_progress_bar_info_selected_brush_width, ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH);

    AddComboBox("Font", RES_TYPE_FONT, ID_FONT_SELECT_0, info->font_id[0]);
    AddComboBox("Normal Text Color", RES_TYPE_COLOR, ID_NORMAL_TEXT_COLOR, info->color_id[NORMAL_TEXT_COLOR_INDEX]);
    AddComboBox("Selected Text Color", RES_TYPE_COLOR, ID_SELECTED_TEXT_COLOR, info->color_id[SELECTED_TEXT_COLOR_INDEX]);
    AddComboBox("Disabled Text Color", RES_TYPE_COLOR, ID_DISABLED_TEXT_COLOR, info->color_id[DISABLED_TEXT_COLOR_INDEX]);

    AddComboBox("Normal Brush Color", RES_TYPE_COLOR, ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR, bar_info->gx_radial_progress_bar_info_normal_brush_color);
    AddComboBox("Selected Brush Color", RES_TYPE_COLOR, ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR, bar_info->gx_radial_progress_bar_info_selected_brush_color);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddSpriteProps(widget_info *info)
{
    AddComboBox("Pixelmap ID", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    AddCheckBox("Auto-run", info->style & GX_STYLE_SPRITE_AUTO, ID_SPRITE_AUTO);
    AddCheckBox("Continous Loop", info->style & GX_STYLE_SPRITE_LOOP, ID_SPRITE_LOOP);
    AddPushButton("Edit Framelist", ID_SPRITE_EDIT_FRAMES);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddRadialSliderProps(widget_info *info)
{
    radial_slider_info *slider_info = &info->ewi.radial_slider;

    AddEditControl("X Center", slider_info->xcenter, ID_RADIAL_SLIDER_XCENTER);
    AddEditControl("Y Center", slider_info->ycenter, ID_RADIAL_SLIDER_YCENTER);
    AddEditControl("Radius", slider_info->radius, ID_RADIAL_SLIDER_RADIUS);
    AddEditControl("Track Width", slider_info->track_width, ID_RADIAL_SLIDER_TRACK_WIDTH);
    AddEditControl("Needle Offset", slider_info->needle_offset, ID_RADIAL_SLIDER_NEEDLE_OFFSET);
    AddEditControl("Current Angle", slider_info->current_angle, ID_RADIAL_SLIDER_CURRENT_ANGLE);
    AddEditControl("Min Angle", slider_info->min_angle, ID_RADIAL_SLIDER_MIN_ANGLE);
    AddEditControl("Max Angle", slider_info->max_angle, ID_RADIAL_SLIDER_MAX_ANGLE);
    AddEditControl("Total Steps", slider_info->animation_total_steps, ID_RADIAL_SLIDER_TOTAL_STEPS);
    AddEditControl("Delay", slider_info->animation_delay, ID_RADIAL_SLIDER_DELAY);
    AddComboBox("Animation Style", easing_function_list, ID_RADIAL_SLIDER_ANIMATION_STYLE, slider_info->animation_style);
    AddEditControl("Animation Callback", info->callback_func, ID_CALLBACK_NAME);
    AddComboBox("Normal Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    AddComboBox("Needle Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::AddScrollbarProps(widget_info *info)
{
    AddComboBox("Thumb Color", RES_TYPE_COLOR, ID_SCROLL_THUMB_COLOR, info->ewi.scroll.gx_scroll_thumb_color);
    AddComboBox("Thumb Border Color", RES_TYPE_COLOR, ID_SCROLL_THUMB_BORDER_COLOR, info->ewi.scroll.gx_scroll_thumb_border_color);
    AddComboBox("Thumb Border Style", border_types, ID_SCROLL_THUMB_BORDER_STYLE,
                 info->ewi.scroll.gx_scroll_thumb_border_style);
    AddEditControl("Thumb Width", info->ewi.scroll.gx_scroll_thumb_width, ID_THUMB_WIDTH);
    AddEditControl("Thumb Travel Min", info->ewi.scroll.gx_scroll_thumb_travel_min, ID_THUMB_TRAVEL_MIN);
    AddEditControl("Thumb Travel Max", info->ewi.scroll.gx_scroll_thumb_travel_max, ID_THUMB_TRAVEL_MAX);

    AddComboBox("Button Color", RES_TYPE_COLOR, ID_SCROLL_BUTTON_COLOR, info->ewi.scroll.gx_scroll_button_color);

    AddComboBox("Bkgnd Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->ewi.scroll.gx_scroll_fill_pixelmap);
    AddComboBox("Thumb Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->ewi.scroll.gx_scroll_thumb_pixelmap);

    if (info->widget->gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
    {
        AddComboBox("Up Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->ewi.scroll.gx_scroll_up_pixelmap);
        AddComboBox("Down Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_3, info->ewi.scroll.gx_scroll_down_pixelmap);
    }
    else
    {
        AddComboBox("Left Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->ewi.scroll.gx_scroll_up_pixelmap);
        AddComboBox("Right Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_3, info->ewi.scroll.gx_scroll_down_pixelmap);
    }    
    AddCheckBox("End Buttons", info ->style & GX_SCROLLBAR_END_BUTTONS, ID_SCROLL_END_BUTTONS);
    AddCheckBox("Tile Background", info->style & GX_STYLE_TILE_BACKGROUND, ID_SCROLL_TILE_BKGND);
    AddCheckBox("Relative Thumb Size", info ->style & GX_SCROLLBAR_RELATIVE_THUMB, ID_SCROLL_RELATIVE_THUMB);
//    AddCheckBox("Use as Default", info->style & GX_SCROLLBAR_IS_DEFAULT, ID_SCROLL_IS_DEFAULT);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AddGaugeProps(widget_info *info)
{
    AddEditControl("Start Angle", info->ewi.gauge.start_angle, ID_GAUGE_START_ANGLE);
    AddEditControl("Animation Steps", info->ewi.gauge.info.gx_circular_gauge_info_animation_steps, ID_GAUGE_ANIMATION_STEPS);
    AddEditControl("Animation Delay", info->ewi.gauge.info.gx_circular_gauge_info_animation_delay, ID_GAUGE_ANIMATION_DELAY);
    AddEditControl("Needle Xpos", info->ewi.gauge.info.gx_circular_gauge_info_needle_xpos, ID_GAUGE_NEEDLE_XPOS);
    AddEditControl("Needle Ypos", info->ewi.gauge.info.gx_circular_gauge_info_needle_ypos, ID_GAUGE_NEEDLE_YPOS);
    AddEditControl("Needle Xcor", info->ewi.gauge.info.gx_circular_gauge_info_needle_xcor, ID_GAUGE_NEEDLE_XCOR);
    AddEditControl("Needle Ycor", info->ewi.gauge.info.gx_circular_gauge_info_needle_ycor, ID_GAUGE_NEEDLE_YCOR);
    AddComboBox("Normal Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_0, info->pixelmap_id[NORMAL_PIXELMAP_INDEX]);
    AddComboBox("Selected Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_1, info->pixelmap_id[SELECTED_PIXELMAP_INDEX]);
    AddComboBox("Needle Pixelmap", RES_TYPE_PIXELMAP, ID_PIXELMAP_2, info->pixelmap_id[NEEDLE_PIXELMAP_INDEX]);
}



///////////////////////////////////////////////////////////////////////////////
// this function is called by the target screen when the user drags the selected
// widget. Update the properties values.
void properties_win::WidgetWasMoved()
{
    CString val;

    if (mpInfo)
    {
        if (mpInfo->widget->gx_widget_type == GX_TYPE_RADIAL_PROGRESS_BAR)
        {

            val.Format(_T("%d"), mpInfo->ewi.radial_progress.gx_radial_progress_bar_info_xcenter);
            SetDlgItemText(ID_RADIAL_PROGRESS_XCENTER, val);

            val.Format(_T("%d"), mpInfo->ewi.radial_progress.gx_radial_progress_bar_info_ycenter);
            SetDlgItemText(ID_RADIAL_PROGRESS_YCENTER, val);

            val.Format(_T("%d"), mpInfo->ewi.radial_progress.gx_radial_progress_bar_info_radius);
            SetDlgItemText(ID_RADIAL_PROGRESS_RADIUS, val);
        }
        else
        {
            val.Format(_T("%d"), mpInfo->size.gx_rectangle_left);
            SetDlgItemText(ID_XPOS, val);

            val.Format(_T("%d"), mpInfo->size.gx_rectangle_top);
            SetDlgItemText(ID_YPOS, val);

            val.Format(_T("%d"),
                mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1);
            SetDlgItemText(ID_WIDTH, val);

            val.Format(_T("%d"),
                mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1);
            SetDlgItemText(ID_HEIGHT, val);
        }
            
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::WidgetWasModified(widget_info *info)
{
    // if the modified widget is the widget that I have displayed,
    // then update all my child fields.
    if (info == mpInfo)
    {
        OnWidgetSelect(info);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::WidgetWasModified(GX_WIDGET *widget)
{
    // if the modified widget is the widget that I have displayed,
    // then update all my child fields.
    if (mpInfo && mpProject)
    {
        if (mpInfo->widget == widget)
        {
            OnWidgetSelect(mpInfo);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::ResourcesChanged()
{
    if (mpInfo)
    {
        OnWidgetSelect(mpInfo);
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnWidgetSelect(widget_info *info)
{
    ShowWindow(SW_HIDE);

    //send a kill focus message to the current focus owner
    SendEditFocusLoseMessage();

    DestroyChildren();
    CRect rect;
    CRect winrect;
    mpInfo = info;
    mpProject = NULL;
   
    if (info)
    {
        mpProject = GetOpenProject();

        int scrollpos = m_scroll_helper->GetScrollPos().cy;
        m_scroll_helper->ScrollToOrigin(0, TRUE);
        int istart = 3;
        m_top_pos = istart;

        if (GetProjectView()->GetActiveDisplay() >= 0)
        {
            GetClientRect(&rect);
            GetWindowRect(&winrect);

            //m_top_pos = rect.top + 3;

            AddWidgetProps();

            switch(info->basetype)
            {
            case GX_TYPE_PROMPT:
                AddTextWidgetProperties(info);
                AddTextCopyProperty(info);
                break;

            case GX_TYPE_NUMERIC_PROMPT:
                AddTextWidgetProperties(info);
                AddNumericPromptProps(info);
                break;

            case GX_TYPE_BUTTON:
                AddButtonStyles(info);
                break;

            case GX_TYPE_TEXT_BUTTON:
            case GX_TYPE_MULTI_LINE_TEXT_BUTTON:
                AddButtonStyles(info);
                AddTextWidgetProperties(info);
                AddTextCopyProperty(info);
                break;

            case GX_TYPE_CHECKBOX:
                AddCheckboxProps(info);
                break;

            case GX_TYPE_RADIO_BUTTON:
                AddRadioButtonProps(info);
                break;

            case GX_TYPE_ICON_BUTTON:
                AddIconButtonProps(info);
                break;

            case GX_TYPE_PIXELMAP_BUTTON:
                AddPixelmapButtonProps(info);
                break;

            case GX_TYPE_ICON:
                AddIconProps(info);
                break;

            case GX_TYPE_PIXELMAP_PROMPT:
                AddPixelmapPromptProps(info);
                AddTextCopyProperty(info);
                break;

            case GX_TYPE_NUMERIC_PIXELMAP_PROMPT:
                AddPixelmapPromptProps(info);
                AddNumericPromptProps(info);
                break;

            case GX_TYPE_SLIDER:
                AddSliderProps(info);
                break;

            case GX_TYPE_PIXELMAP_SLIDER:
                AddPixelmapSliderProps(info);
                break;

            case GX_TYPE_RADIAL_SLIDER:
                AddRadialSliderProps(info);
                break;

            case GX_TYPE_PROGRESS_BAR:
                AddProgressBarProps(info);
                break;

            case GX_TYPE_RADIAL_PROGRESS_BAR:
                AddRadialProgressBarProps(info);
                break;

            case GX_TYPE_SPRITE:
                AddSpriteProps(info);
                break;

            case GX_TYPE_WINDOW:
                AddWindowProps(info);
                break;

            case GX_TYPE_VERTICAL_LIST:
                AddVerticalListProps(info);
                break;

            case GX_TYPE_HORIZONTAL_LIST:
                AddHorizontalListProps(info);
                break;

            case GX_TYPE_DROP_LIST:
                AddDropListProps(info);
                break;

            case GX_TYPE_LINE_CHART:
                AddChartProps(info);
                break;

            case GX_TYPE_STRING_SCROLL_WHEEL:
                AddStringScrollWheelProps(info);
                break;

            case GX_TYPE_NUMERIC_SCROLL_WHEEL:
                AddNumericScrollWheelProps(info);
                break;

            case GX_TYPE_GENERIC_SCROLL_WHEEL:
                AddGenericScrollWheelProps(info);
                break;

            case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
                AddTextInputProperties(info);
                break;

            case GX_TYPE_MULTI_LINE_TEXT_VIEW:
                AddMLTextViewProperties(info);
                break;

            case GX_TYPE_MULTI_LINE_TEXT_INPUT:
                AddMLTextInputProperties(info);
                break;

            case GX_TYPE_RICH_TEXT_VIEW:
                AddRichTextViewProperties(info);
                break;

            case GX_TYPE_VERTICAL_SCROLL:
            case GX_TYPE_HORIZONTAL_SCROLL:
                AddScrollbarProps(info);
                break;

            case GX_TYPE_CIRCULAR_GAUGE:
                AddGaugeProps(info);
                break;

            case GX_TYPE_MENU:
                AddPixelmapPromptProps(info);
                AddMenuProps(info);
                AddTextCopyProperty(info);
                break;

            case GX_TYPE_TREE_VIEW:
                AddTreeViewProps(info);
                break;
            }
        }

        /* for some reason the dialog font is not automatically
           assigned to dynamically created children, so assign the
           font here
        */
        AssignChildFont();

        int display_size = m_top_pos + m_row_height;
        int win_size = rect.Height();

        m_scroll_helper->SetDisplaySize(0, display_size);

        if (display_size > 0)
        {
            if ((scrollpos > display_size - win_size))
            {
                scrollpos = display_size - win_size;
            }
        }
        else
        {
            scrollpos = 0;
        }
        m_scroll_helper->SetScrollPos(SB_VERT, scrollpos);
    }
    ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::ValidateTemplateName(CString &name)
{
    if (mpInfo)
    {
        if (name.IsEmpty())
        {
            ErrorMsg("Template must have a valid name");

            if (mpInfo->app_name.IsEmpty())
            {
                mpInfo->app_name = CString("Template");
            }
            name = mpInfo->app_name;
        }
        SetDlgItemText(ID_WIDGET_NAME, name);
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeWidgetName()
{
    CEdit *edit = (CEdit *)GetDlgItem(ID_WIDGET_NAME);
    BOOL save = TRUE;

    if (mpProject && mpInfo && edit)
    {
        CString new_name;

        GetDlgItemText(ID_WIDGET_NAME, new_name);

        if (new_name != mpInfo->app_name)
        {
            project_view *view = GetProjectView();
            BOOL search_child;

            if (view->IsTopLevelWidget(mpInfo))
            {
                // check for duplicate widget name through top level widgets
                folder_info *folder = view->GetRootFolder();

                if (folder)
                {
                    search_child = FALSE;

                    if (widget_factory::FindAppName(folder, new_name, search_child))
                    {
                        save = FALSE;
                    }
                }
            }
            else
            {
                // check for dupliate widget name through child widgets
                widget_info *root = view->FindTopLevelWidget(mpInfo);

                if (root)
                {
                    search_child = TRUE;

                    if (widget_factory::FindAppName(root->GetChildWidgetInfo(), new_name, search_child))
                    {
                        save = FALSE;
                    }
                }
            }

            if (!save)
            {
                ErrorMsg("Widget name already exists!");
            }

            
            new_name = mpInfo->app_name;
            if(save && TestInputName(edit, "Widget Name", new_name))
            {
                if (mpInfo->is_template && widget_factory::FindBaseName(new_name))
                {
                    ErrorMsg("Template name already exists!");

                    // The template name already exist, revert name change
                    SetDlgItemText(ID_WIDGET_NAME, mpInfo->app_name);
                }
                else
                {
                    if (mpInfo->is_template)
                    {
                        // Update base type name of derived widgets
                        template_service_provider::CheckRenameTemplate(mpInfo->app_name, &new_name);
                    }

                    UndoManager()->AddEntry(UNDO_TYPE_NAMES, mpInfo);

                    if (GetProjectView()->IsTopLevelWidget(mpInfo))
                    {
                        int display = mpProject->GetDisplayIndex(mpInfo);
                        if (mpProject->mDisplays[display].screenflow)
                        {
                            mpProject->mDisplays[display].screenflow->UpdateScreenName(mpInfo->app_name, new_name);
                        }
                    }

                    // Change widget name
                    mpInfo->app_name = new_name;

                    GetProjectView()->NotifyNameChange(mpInfo);

                    mpProject->SetModified();
                }
            }
            else
            {
                // Revert to original widget name
                SetDlgItemText(ID_WIDGET_NAME, mpInfo->app_name);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL properties_win::CheckChangeWidgetId(CString new_name)
{
    int display = mpProject->GetDisplayIndex(mpInfo);
    screen_flow *flow = mpProject->mDisplays[display].screenflow;;

    if (flow)
    {
        //old id name has been removed, update id name in screenflow
        if (new_name.IsEmpty())
        {
            project_view *view = GetProjectView();
            widget_info *screen_info = NULL;
            flow_item *item = NULL;
            if (view)
            {
                //get top level widget info
                screen_info = view->FindTopLevelWidget(mpInfo);
            }

            if (screen_info)
            {
                //get flow item
                item = flow->GetFlowItem(screen_info->app_name);
            }

            // check if the old id name is reference by a trigger or
            // duplicate id name exist
            if (flow->FindTrigger(item, mpInfo->id_name) &&
                !view->FindWidgetIdName(screen_info, mpInfo, mpInfo->id_name))
            {
                CString  msg;
                msg.Format(_T("ID name \"%s\" is referenced by a trigger, remove the id name will invalid the trigger, continue?"), mpInfo->id_name);

                if (!AskUser(CT2A(msg)))
                {
                    return FALSE;
                }
            }
        }
        else
        {
            // update id name that referenced by the trigger
            mpProject->mDisplays[display].screenflow->UpdateIdName(mpInfo, mpInfo->id_name, new_name);
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeWidgetId()
{
    BOOL Is_Modified = FALSE;
    CEdit *edit = (CEdit *)GetDlgItem(ID_WIDGET_ID);

    if (mpProject && mpInfo && edit)
    {
        CString new_name;

        /*Check Widget ID.*/
        GetDlgItemText(ID_WIDGET_ID, new_name);

        if (new_name != mpInfo->id_name)
        {
            if (new_name.IsEmpty())
            {
                Is_Modified = TRUE;
            }
            else
            {
                new_name = mpInfo->id_name;
                if (TestInputName(edit, "Widget Id", new_name))
                {
                    Is_Modified = TRUE;
                }
                else
                {
                    // revert to original id name
                    SetDlgItemText(ID_WIDGET_ID, mpInfo->id_name);
                }
            }

            if (Is_Modified)
            {
                if (!CheckChangeWidgetId(new_name))
                {
                    SetDlgItemText(ID_WIDGET_ID, mpInfo->id_name);
                    return;
                }

                UndoManager()->AddEntry(UNDO_TYPE_NAMES, mpInfo);

                CString old_id_name = mpInfo->id_name;
                // Change widget id name
                mpInfo->id_name = new_name;

                int display = mpProject->GetDisplayIndex(mpInfo);

                if (!new_name.IsEmpty())
                {
                    mpProject->AddToIdDictionary(display, ID_TYPE_WIDGET, new_name);
                }
                
                if (!old_id_name.IsEmpty())
                {
                    mpProject->RemoveFromIdDictionary(display, ID_TYPE_WIDGET, old_id_name);
                }

                mpProject->SetModified();
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeEventFunction()
{
    BOOL Is_Modified = FALSE;
    CEdit *edit = (CEdit *)GetDlgItem(ID_EVENT_FUNCTION);

    if (mpProject && mpInfo && edit)
    {
        CString new_event_func;

        /*Check function name.*/
        GetDlgItemText(ID_EVENT_FUNCTION, new_event_func);

        if (new_event_func != mpInfo->event_func)
        {
            if (new_event_func.IsEmpty())
            {
                Is_Modified = TRUE;
            }
            else
            {
                new_event_func = mpInfo->event_func;
                if (TestInputName(edit, "Event Function", new_event_func))
                {
                    Is_Modified = TRUE;
                }
                else
                {
                    // revert to original event function
                    SetDlgItemText(ID_EVENT_FUNCTION, mpInfo->event_func);
                }
            }

            if (Is_Modified)
            {
                UndoManager()->AddEntry(UNDO_TYPE_NAMES, mpInfo);

                // Change widget event function name
                mpInfo->event_func = new_event_func;

                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeDrawFunction()
{
    BOOL Is_Modified = FALSE;
    CEdit *edit = (CEdit *)GetDlgItem(ID_DRAW_FUNCTION);

    if (mpProject && mpInfo && edit)
    {
        CString new_draw_func;

        /*Check function name.*/
        GetDlgItemText(ID_DRAW_FUNCTION, new_draw_func);

        if (new_draw_func != mpInfo->draw_func)
        {
            if (new_draw_func.IsEmpty())
            {
                Is_Modified = TRUE;
            }
            else
            {
                new_draw_func = mpInfo->draw_func;
                if (TestInputName(edit, "Draw Function", new_draw_func))
                {
                    Is_Modified = TRUE;
                }
                else
                {
                    // revert to original draw function
                    SetDlgItemText(ID_DRAW_FUNCTION, mpInfo->draw_func);
                }
            }

            if (Is_Modified)
            {
                UndoManager()->AddEntry(UNDO_TYPE_NAMES, mpInfo);

                // Change widget draw function name
                mpInfo->draw_func = new_draw_func;

                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeWidgetUserData()
{
    if (mpProject && mpInfo && GetDlgItem(ID_USER_DATA))
    {
        CString data;

        GetDlgItemText(ID_USER_DATA, data);

        if (data != mpInfo->user_data)
        {
            UndoManager()->AddEntry(UNDO_TYPE_USER_DATA, mpInfo);

            // Update user data
            mpInfo->user_data = data;

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeCallbackName()
{
    CEdit *edit = (CEdit *)GetDlgItem(ID_CALLBACK_NAME);

    if (mpProject && mpInfo && edit)
    {
        CString name;
        GX_BOOL Is_Modified = GX_FALSE;

        GetDlgItemText(ID_CALLBACK_NAME, name);

        if (name != mpInfo->callback_func)
        {
            if (name.IsEmpty())
            {
                Is_Modified = TRUE;
            }
            else
            {
                name = mpInfo->callback_func;

                if (TestInputName(edit, "Callback Function", name))
                {
                    Is_Modified = TRUE;
                }
                else
                {
                    SetDlgItemText(ID_CALLBACK_NAME, mpInfo->callback_func);
                }
            }

            if (Is_Modified)
            {
                if (mpInfo->basetype == GX_TYPE_STRING_SCROLL_WHEEL)
                {
                    OnChangeStringScrollWheelProps();
                }
                else
                {
                    UndoManager()->AddEntry(UNDO_TYPE_NAMES, mpInfo);

                    //update callback func name
                    mpInfo->callback_func = name;

                    mpProject->SetModified();
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::SetChildAllocation(widget_info *info, int allocation_type)
{
    while(info)
    {
        if (info->GetChildWidgetInfo())
        {
            SetChildAllocation(info->GetChildWidgetInfo(), allocation_type);
        }
        info->allocation = allocation_type;
        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeDynamicWidget()
{
    CButton *pBox = (CButton *) GetDlgItem(ID_DYNAMIC_WIDGET);
    if (mpProject && mpInfo && pBox)
    {
        if (pBox->GetCheck())
        {
            if (mpInfo->is_template)
            {
                Notify("Templates cannot be dynamically allocated. Only widgets based on the template can be dynamically allocated");
                pBox->SetCheck(0);
                return;
            }
            UndoManager()->AddEntry(UNDO_TYPE_ALLOCATION, mpInfo);
            mpInfo->allocation = DYNAMIC_ALLOCATION_ROOT;
            SetChildAllocation(mpInfo->GetChildWidgetInfo(), DYNAMIC_ALLOCATION_CHILD);
        }
        else
        {
            UndoManager()->AddEntry(UNDO_TYPE_ALLOCATION, mpInfo);
            mpInfo->allocation = STATICALLY_ALLOCATED;
            SetChildAllocation(mpInfo->GetChildWidgetInfo(), STATICALLY_ALLOCATED);
        }
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeDynamicBuffer()
{
    if (mpProject && mpInfo && GetDlgItem(ID_DYNAMIC_BUFFER))
    {
        UndoManager()->AddEntry(UNDO_TYPE_DYNAMIC_TEXT_BUFFER, mpInfo);
        
        if (IsDlgButtonChecked(ID_DYNAMIC_BUFFER))
        {
            mpInfo->ewi.text_info.dynamic_buffer = true;
        }
        else
        {
            mpInfo->ewi.text_info.dynamic_buffer = false;
        }
        mpProject->SetModified();
    }
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeWidgetPos()
{
    GX_RECTANGLE newsize;

    if (mpProject && mpInfo && GetDlgItem(ID_XPOS))
    {
        if (mpInfo->widget == NULL)
        {
            return;
        }

        if (mpInfo->widget->gx_widget_type == GX_TYPE_HORIZONTAL_SCROLL ||
            mpInfo->widget->gx_widget_type == GX_TYPE_VERTICAL_SCROLL)
        {
            OnScrollAppearanceChange();
            mpInfo->size = mpInfo->widget->gx_widget_size;
            return;
        }

        int width, height;

        if (mpInfo->basetype == GX_TYPE_TEMPLATE)
        {
            width = mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1;
            height = mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1;
        }
        else
        {
            width = GetDlgItemInt(ID_WIDTH, NULL, TRUE);
            height = GetDlgItemInt(ID_HEIGHT, NULL, TRUE);
        }

        newsize.gx_rectangle_left = GetDlgItemInt(ID_XPOS, NULL, TRUE);
        newsize.gx_rectangle_top = GetDlgItemInt(ID_YPOS, NULL, TRUE);
        newsize.gx_rectangle_right = newsize.gx_rectangle_left + width - 1;
        newsize.gx_rectangle_bottom = newsize.gx_rectangle_top + height - 1;

        int parent_type = GetTargetScreen()->GetWidgetParentType(mpInfo);

        //Check of widget moving is allowed.
        if (!GetTargetScreen()->IsWidgetResizable(parent_type, mpInfo, newsize))
        {
            SetDlgItemInt(ID_XPOS, mpInfo->size.gx_rectangle_left);
            SetDlgItemInt(ID_YPOS, mpInfo->size.gx_rectangle_top);
            SetDlgItemInt(ID_WIDTH, mpInfo->size.gx_rectangle_right - mpInfo->size.gx_rectangle_left + 1);
            SetDlgItemInt(ID_HEIGHT, mpInfo->size.gx_rectangle_bottom - mpInfo->size.gx_rectangle_top + 1);
            return;
        }

        if (newsize.gx_rectangle_left != mpInfo->size.gx_rectangle_left ||
            newsize.gx_rectangle_top != mpInfo->size.gx_rectangle_top ||
            newsize.gx_rectangle_right != mpInfo->size.gx_rectangle_right ||
            newsize.gx_rectangle_bottom != mpInfo->size.gx_rectangle_bottom)
        {
            UndoManager()->AddEntry(UNDO_TYPE_SIZE, mpInfo);
            widget_factory::MoveWidget(mpInfo, newsize);

            GetTargetScreen()->WidgetWasMoved(mpInfo);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeBorder()
{
    CComboBox *pBox = (CComboBox *) GetDlgItem(ID_BORDER_COMBO);

    if (mpProject && mpInfo && pBox)
    {
        int Selected = pBox->GetCurSel();
        ULONG style = (ULONG) pBox->GetItemData(Selected);

        if (!(mpInfo->style & style))
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

            gx_widget_border_style_set(mpInfo->widget, style);

            mpInfo->style = mpInfo->widget->gx_widget_style;

            GX_WIDGET *child = mpInfo->widget->gx_widget_first_child;
            GX_EVENT show_event;

            while (child)
            {
                if ((child->gx_widget_type == GX_TYPE_VERTICAL_SCROLL) ||
                    (child->gx_widget_type == GX_TYPE_HORIZONTAL_SCROLL))
                {
                    memset(&show_event, 0, sizeof(GX_EVENT));
                    show_event.gx_event_type = GX_EVENT_SHOW;
                    child->gx_widget_event_process_function(child, &show_event);
                }

                child = child->gx_widget_next;
            }

            gx_system_dirty_mark(mpInfo->widget)

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTransparent()
{
    CButton *pb = (CButton *) GetDlgItem(ID_TRANSPARENT);

    if (mpProject && mpInfo && mpInfo->widget && pb)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (pb->GetCheck() == BST_CHECKED)
        {
            mpInfo->style |= GX_STYLE_TRANSPARENT;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_TRANSPARENT;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);

        if ((mpInfo->basetype == GX_TYPE_VERTICAL_SCROLL) ||
            (mpInfo->basetype == GX_TYPE_HORIZONTAL_SCROLL))
        {
            // Send show event to update parent client area
            GX_EVENT  show_event;

            memset(&show_event, 0, sizeof(GX_EVENT));
            show_event.gx_event_type = GX_EVENT_SHOW;
            show_event.gx_event_target = mpInfo->widget;
            gx_system_event_send(&show_event);
        }

        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeSelected()
{
    CButton *pb = (CButton *) GetDlgItem(ID_SELECTED);

    if (mpProject && mpInfo && pb)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (pb->GetCheck() == BST_CHECKED)
        {
            mpInfo->style |= GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_DRAW_SELECTED;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeEnabled()
{
    if (mpProject && mpInfo && GetDlgItem(ID_ENABLED))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (IsDlgButtonChecked(ID_ENABLED))
        {
            mpInfo->style |= GX_STYLE_ENABLED;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_ENABLED;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTextCopy()
{
    if (mpProject && mpInfo && GetDlgItem(ID_TEXT_COPY))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (IsDlgButtonChecked(ID_TEXT_COPY))
        {
            mpInfo->style |= GX_STYLE_TEXT_COPY;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_TEXT_COPY;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeMLTextVCenter()
{
    if (mpProject && mpInfo && GetDlgItem(ID_ML_TEXT_VCENTER))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (IsDlgButtonChecked(ID_ML_TEXT_VCENTER))
        {
            GX_SCROLLBAR *scroll;
            gx_window_scrollbar_find((GX_WINDOW *) mpInfo->widget, GX_TYPE_VERTICAL_SCROLL, &scroll);
            if (scroll)
            {
                Notify("This style has no effect if the text view has a vertical scrollbar.", this);
            }
            mpInfo->style |= GX_STYLE_VALIGN_CENTER;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_VALIGN_CENTER;
            GX_MULTI_LINE_TEXT_VIEW *mltv = (GX_MULTI_LINE_TEXT_VIEW *) mpInfo->widget;
            mltv->gx_multi_line_text_view_text_scroll_shift = 0;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeFocus()
{
    if (mpProject && mpInfo && GetDlgItem(ID_ACCEPTS_FOCUS))
    {
        UndoManager()->AddEntry(UNDO_TYPE_FOCUS, mpInfo);

        if (IsDlgButtonChecked(ID_ACCEPTS_FOCUS))
        {
            mpInfo->accepts_focus = TRUE;
        }
        else
        {
            mpInfo->accepts_focus = FALSE;
        }
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeReadonly()
{
    if (mpProject && mpInfo && GetDlgItem(ID_TEXT_INPUT_READONLY))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (IsDlgButtonChecked(ID_TEXT_INPUT_READONLY))
        {
            mpInfo->style |= GX_STYLE_TEXT_INPUT_READONLY;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_TEXT_INPUT_READONLY;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeColor()
{
    CComboBox *pNormBox = (CComboBox *)GetDlgItem(ID_NORMAL_COLOR);
    CComboBox *pSelBox = (CComboBox *)GetDlgItem(ID_SELECTED_COLOR);
    CComboBox *pDisabledBox = (CComboBox *)GetDlgItem(ID_DISABLED_COLOR);
    GX_BOOL    Is_Modified = GX_FALSE;

    if (mpProject && mpInfo && pNormBox)
    {
        int index, color_id;

        color_id = pNormBox->GetItemData(pNormBox->GetCurSel());
        if (color_id != mpInfo->color_id[NORMAL_FILL_COLOR_INDEX])
        {
            //normal fill color changed
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, NORMAL_FILL_COLOR_INDEX);
            index = NORMAL_FILL_COLOR_INDEX;
            Is_Modified = GX_TRUE;
        }

        if (!Is_Modified)
        {
            color_id = pSelBox->GetItemData(pSelBox->GetCurSel());

            if (color_id != mpInfo->color_id[SELECTED_FILL_COLOR_INDEX])
            {
                //selected fill color changed
                UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, SELECTED_FILL_COLOR_INDEX);
                index = SELECTED_FILL_COLOR_INDEX;
                Is_Modified = GX_TRUE;
            }
        }

        if (!Is_Modified)
        {
            color_id = pDisabledBox->GetItemData(pDisabledBox->GetCurSel());
            if (color_id != mpInfo->color_id[DISABLED_FILL_COLOR_INDEX])
            {
                //disabled fill color changed
                UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, DISABLED_FILL_COLOR_INDEX);
                index = DISABLED_FILL_COLOR_INDEX;
                Is_Modified = GX_TRUE;
            }
        }

        if (Is_Modified)
        {
            widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);
            if (provider)
            {
                //assign new colors
                provider->AssignColor(mpInfo, index, color_id);
            }

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTileWallpaper()
{
    CButton *pb = (CButton *) GetDlgItem(ID_TILE_WALLPAPER);

    if (mpProject && mpInfo && pb)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (pb->GetCheck() == BST_CHECKED)
        {
            mpInfo->style |= GX_STYLE_TILE_WALLPAPER;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_TILE_WALLPAPER;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeNumericPrompt()
{
    CComboBox *pBox = (CComboBox *)GetDlgItem(ID_STRING_ID);
    CEdit *pEdit = (CEdit *)GetDlgItem(ID_NUMERIC_PROMPT_VALUE);
    BOOL Is_Modified = GX_FALSE;

    if (mpProject && mpInfo && pBox && pEdit)
    {
        int numeric_value = GetDlgItemInt(ID_NUMERIC_PROMPT_VALUE, 0, TRUE);
        CString format_func;
        
        GetDlgItemText(ID_FORMAT_FUNC, format_func);

        if (numeric_value != mpInfo->ewi.numeric_prompt_value)
        {
            Is_Modified = TRUE;
        }

        if (format_func != mpInfo->format_func)
        {
            if (format_func.IsEmpty())
            {
                Is_Modified = TRUE;
            }

            if (!format_func.IsEmpty())
            {
                pEdit = (CEdit *)GetDlgItem(ID_FORMAT_FUNC);
                if (TestInputName(pEdit, "Format Func", format_func))
                {
                    Is_Modified = TRUE;
                }
                else
                {
                    pEdit->SetWindowText(mpInfo->format_func);
                }
            }
        }

        if (Is_Modified)
        {
            UndoManager()->AddEntry(UNDO_TYPE_NUMERIC_PROMPT_INFO, mpInfo);

            prompt_service_provider *provider = (prompt_service_provider *)(widget_factory::GetServiceProvider(mpInfo->basetype));

            /* Assign numeric value. */
            provider->AssignNumericValue(mpInfo, numeric_value);

            /* Clear string id setting. */
            pBox->SetCurSel(0);

            mpInfo->format_func = format_func;

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnPixelmapChange(int index)
{
    CComboBox *pImageBox = (CComboBox *) GetDlgItem(ID_PIXELMAP_0 + index);

    if (mpProject && mpInfo && pImageBox)
    {
        int box_index = pImageBox->GetCurSel();
        int image_id = pImageBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((mpInfo->pixelmap_id[index] != image_id) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_PIXELMAP, mpInfo, index);
            provider->AssignPixelmap(mpInfo, index, image_id);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap0()
{
    OnPixelmapChange(0);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap1()
{
    OnPixelmapChange(1);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap2()
{
    OnPixelmapChange(2);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap3()
{
    OnPixelmapChange(3);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap4()
{
    OnPixelmapChange(4);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap5()
{
    OnPixelmapChange(5);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap6()
{
    OnPixelmapChange(6);
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixelmap7()
{
    OnPixelmapChange(7);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnFontChange(int index)
{
    CComboBox *pFontBox = (CComboBox *) GetDlgItem(ID_FONT_SELECT_0 + index);

    if (mpProject && mpInfo && pFontBox)
    {
        int box_index = pFontBox->GetCurSel();
        int FontId = pFontBox->GetItemData(box_index);

        if (FontId != mpInfo->font_id[index])
        {
            widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

            if (provider)
            {
                UndoManager()->AddEntry(UNDO_TYPE_FONT, mpInfo, index);

                provider->AssignFont(mpInfo, index, FontId);
                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnFontChange0()
{
    OnFontChange(0);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnFontChange1()
{
    OnFontChange(1);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnFontChange2()
{
    OnFontChange(2);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnFontChange3()
{
    OnFontChange(3);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeNormalTextColor()
{
    CComboBox *pColorBox = (CComboBox *) GetDlgItem(ID_NORMAL_TEXT_COLOR);

    if (mpProject && mpInfo && pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);
        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[NORMAL_TEXT_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, NORMAL_TEXT_COLOR_INDEX);

            provider->AssignColor(mpInfo, NORMAL_TEXT_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeSelectedTextColor()
{
    CComboBox *pColorBox = (CComboBox *) GetDlgItem(ID_SELECTED_TEXT_COLOR);

    if (mpProject && mpInfo && pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[SELECTED_TEXT_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, SELECTED_TEXT_COLOR_INDEX);
            provider->AssignColor(mpInfo, SELECTED_TEXT_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeDisabledTextColor()
{
    CComboBox *pColorBox = (CComboBox *)GetDlgItem(ID_DISABLED_TEXT_COLOR);

    if (mpProject && mpInfo && pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[DISABLED_TEXT_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, DISABLED_TEXT_COLOR_INDEX);
            provider->AssignColor(mpInfo, DISABLED_TEXT_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeReadonlyFillColor()
{
    CComboBox *pColorBox = (CComboBox *)GetDlgItem(ID_READONLY_COLOR);

    if (mpProject && mpInfo && pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[READONLY_FILL_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, READONLY_FILL_COLOR_INDEX);
            provider->AssignColor(mpInfo, READONLY_FILL_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeReadonlyTextColor()
{
    CComboBox *pColorBox = (CComboBox *)GetDlgItem(ID_READONLY_TEXT_COLOR);

    if (mpProject && mpInfo && pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[READONLY_TEXT_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, READONLY_TEXT_COLOR_INDEX);
            provider->AssignColor(mpInfo, READONLY_TEXT_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnTextAlignmentChange()
{
    CComboBox *pAlign = (CComboBox *) GetDlgItem(ID_TEXT_ALIGN);
    ULONG style = 0;
    
    if (mpProject && mpInfo && pAlign)
    {
        int selected = pAlign->GetCurSel();

        if (selected >= 0)
        {
            style = (ULONG) pAlign->GetItemData(selected);
        }

        if ((mpInfo->style & GX_STYLE_TEXT_ALIGNMENT_MASK) != style)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

            mpInfo->style &= ~(GX_STYLE_TEXT_ALIGNMENT_MASK);
            mpInfo->style |= style;

            gx_widget_style_set(mpInfo->widget, mpInfo->style);

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangePixmapAlign()
{
    CComboBox *pHAlign = (CComboBox *) GetDlgItem(ID_PIXMAP_HALIGN);
    CComboBox *pVAlign = (CComboBox *) GetDlgItem(ID_PIXMAP_VALIGN);

    if (mpProject && mpInfo && pHAlign && pVAlign)
    {
        int selected = pHAlign->GetCurSel();
        ULONG hStyle = 0;
        ULONG vStyle = 0;

        if (selected >= 0)
        {
            hStyle = (ULONG) pHAlign->GetItemData(selected);
            hStyle &= GX_PIXELMAP_HALIGN_MASK;
        }

        selected = pVAlign->GetCurSel();
    
        if (selected >= 0)
        {
            vStyle = (ULONG) pVAlign->GetItemData(selected);
            vStyle &= GX_PIXELMAP_VALIGN_MASK;
        }

        if ((mpInfo->style & (GX_PIXELMAP_HALIGN_MASK | GX_PIXELMAP_VALIGN_MASK)) != (hStyle | vStyle))
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

            mpInfo->style &= ~(GX_PIXELMAP_HALIGN_MASK | GX_PIXELMAP_VALIGN_MASK);
            mpInfo->style |= hStyle | vStyle;
            gx_widget_style_set(mpInfo->widget, mpInfo->style);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnButtonStyleChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_BUTTON_STYLE_PUSHED))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        ULONG style = mpInfo->style;

        if (IsDlgButtonChecked(ID_BUTTON_STYLE_PUSHED))
        {
            style |= GX_STYLE_BUTTON_PUSHED;
        }
        else
        {
            style &= ~GX_STYLE_BUTTON_PUSHED;
        }

        if (IsDlgButtonChecked(ID_BUTTON_STYLE_TOGGLE))
        {
            style |= GX_STYLE_BUTTON_TOGGLE;
        }
        else
        {
            style &= ~GX_STYLE_BUTTON_TOGGLE;
        }
               
        if (IsDlgButtonChecked(ID_BUTTON_STYLE_RADIO))
        {
            style |= GX_STYLE_BUTTON_RADIO;
        }
        else
        {
            style &= ~GX_STYLE_BUTTON_RADIO;
        }
        if (IsDlgButtonChecked(ID_BUTTON_STYLE_REPEAT))
        {
            style |= GX_STYLE_BUTTON_REPEAT;
        }
        else
        {
            style &= ~GX_STYLE_BUTTON_REPEAT;
        }

        mpInfo->style = style;
        gx_widget_style_set(mpInfo->widget, style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnDropListStyleChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_TILE_DROP_LIST_BACKGROUND))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);
        ULONG style = mpInfo->style;

        if (IsDlgButtonChecked(ID_TILE_DROP_LIST_BACKGROUND))
        {
            style |= GX_STYLE_TILE_BACKGROUND;
            gx_widget_style_add(mpInfo->widget, GX_STYLE_TILE_BACKGROUND);
        }
        else
        {
            style &= ~GX_STYLE_TILE_BACKGROUND;
            gx_widget_style_remove(mpInfo->widget, GX_STYLE_TILE_BACKGROUND);
        }

        if (IsDlgButtonChecked(ID_TILE_DROP_LIST_POPLIST_WALLPAPER))
        {
            style |= GX_STYLE_TILE_WALLPAPER;
            gx_widget_style_add((GX_WIDGET *)&(((GX_DROP_LIST *)mpInfo->widget)->gx_drop_list_popup.gx_popup_list_list), GX_STYLE_TILE_WALLPAPER);

        }
        else
        {
            style &= ~GX_STYLE_TILE_WALLPAPER;
            gx_widget_style_remove((GX_WIDGET *)&(((GX_DROP_LIST *)mpInfo->widget)->gx_drop_list_popup.gx_popup_list_list), GX_STYLE_TILE_WALLPAPER);
        }
        
        mpInfo->style = style;
        mpProject->SetModified();
    }
}
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnSliderStyleChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_SLIDER_VERTICAL))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        ULONG style = mpInfo->style;

        if (IsDlgButtonChecked(ID_SLIDER_VERTICAL))
        {
            style |= GX_STYLE_SLIDER_VERTICAL;
        }
        else
        {
            style &= ~GX_STYLE_SLIDER_VERTICAL;
        }
        if (IsDlgButtonChecked(ID_SLIDER_TILE_BKGND))
        {
            style |= GX_STYLE_TILE_BACKGROUND;
        }
        else
        {
            style &= ~GX_STYLE_TILE_BACKGROUND;
        }

        if (mpInfo->basetype == GX_TYPE_SLIDER)
        {
            if (IsDlgButtonChecked(ID_SHOW_TICKMARKS))
            {
                style |= GX_STYLE_SHOW_TICKMARKS;
            }
            else
            {
                style &= ~GX_STYLE_SHOW_TICKMARKS;
            }
        }
        mpInfo->style = style;
        gx_widget_style_set(mpInfo->widget, style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeSliderInfo()
{
    if (mpProject && mpInfo && GetDlgItem(ID_SLIDER_MINVAL))
    {
        GX_SLIDER *slider = (GX_SLIDER *) mpInfo->widget;

        GX_SLIDER_INFO info = mpInfo->ewi.slider;
        info.gx_slider_info_min_val = GetDlgItemInt(ID_SLIDER_MINVAL, NULL, TRUE);
        info.gx_slider_info_max_val = GetDlgItemInt(ID_SLIDER_MAXVAL, NULL, TRUE);
        info.gx_slider_info_current_val = GetDlgItemInt(ID_SLIDER_CURRENTVAL, NULL, TRUE);
        info.gx_slider_info_increment = GetDlgItemInt(ID_SLIDER_INCREMENT, NULL, TRUE);
        info.gx_slider_info_min_travel = GetDlgItemInt(ID_SLIDER_MINTRAVEL, NULL, TRUE);
        info.gx_slider_info_max_travel = GetDlgItemInt(ID_SLIDER_MAXTRAVEL, NULL, TRUE);
        info.gx_slider_info_needle_width = GetDlgItemInt(ID_SLIDER_NEEDLE_WIDTH, NULL, TRUE);
        info.gx_slider_info_needle_height = GetDlgItemInt(ID_SLIDER_NEEDLE_HEIGHT, NULL, TRUE);
        info.gx_slider_info_needle_inset = GetDlgItemInt(ID_SLIDER_NEEDLE_INSET, NULL, TRUE);
        info.gx_slider_info_needle_hotspot_offset = GetDlgItemInt(ID_NEEDLE_HOTSPOT, NULL, TRUE);

        INT tick_count = mpInfo->misc_value;

        if (mpInfo->basetype == GX_TYPE_SLIDER)
        {
            tick_count = GetDlgItemInt(ID_TICKMARK_COUNT);
        }

        if (memcmp(&info, &mpInfo->ewi.slider, sizeof(GX_SLIDER_INFO)) ||
            (tick_count != mpInfo->misc_value))
        {
            UndoManager()->AddEntry(UNDO_TYPE_SLIDER_INFO, mpInfo);

            mpInfo->ewi.slider = info;
            gx_slider_info_set(slider, &mpInfo->ewi.slider);
            mpInfo->misc_value = tick_count;
            slider->gx_slider_tick_count = tick_count;

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::AssignProgressBarInfo(widget_info *wi)
{
    GX_PROGRESS_BAR_INFO info;
    info.gx_progress_bar_font_id = wi->font_id[0];
    info.gx_progress_bar_fill_pixelmap = wi->pixelmap_id[NORMAL_PIXELMAP_INDEX];
    info.gx_progress_bar_info_current_val = wi->ewi.progress.current_val;
    info.gx_progress_bar_info_min_val = wi->ewi.progress.min_val;
    info.gx_progress_bar_info_max_val = wi->ewi.progress.max_val;
    info.gx_progress_bar_normal_text_color = wi->color_id[NORMAL_TEXT_COLOR_INDEX];
    info.gx_progress_bar_selected_text_color = wi->color_id[SELECTED_TEXT_COLOR_INDEX];
    info.gx_progress_bar_disabled_text_color = wi->color_id[DISABLED_TEXT_COLOR_INDEX];

    GX_PROGRESS_BAR *bar = (GX_PROGRESS_BAR *) wi->widget;        
    gx_progress_bar_info_set(bar, &info);
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::SendEditFocusLoseMessage()
{
    CWnd *focus_owner = GetFocus();
    TCHAR  class_name[MAX_PATH];

    GetClassName(focus_owner->GetSafeHwnd(), class_name, MAX_PATH - 1);

    CString rich_edit_str(_T("RichEdit"));
    CString edit_str(_T("Edit"));

    if (_tcsncmp(class_name, rich_edit_str, rich_edit_str.GetLength()) == 0 ||
        _tcsncmp(class_name, edit_str, edit_str.GetLength()) == 0)
    {
        SendMessage(WM_COMMAND, MAKEWPARAM(focus_owner->GetDlgCtrlID(), EN_KILLFOCUS), (LPARAM)(focus_owner->m_hWnd));
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnProgressInfoChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_PROGRESS_VERTICAL))
    {
        ULONG style = mpInfo->style;
        GX_BOOL Is_Modified = FALSE;

        if (IsDlgButtonChecked(ID_PROGRESS_VERTICAL))
        {
            style |= GX_STYLE_PROGRESS_VERTICAL;
        }
        else
        {
            style &= ~GX_STYLE_PROGRESS_VERTICAL;
        }

        if (IsDlgButtonChecked(ID_PROGRESS_SHOW_TEXT))
        {
            style |= GX_STYLE_PROGRESS_TEXT_DRAW;
        }
        else
        {
            style &= ~GX_STYLE_PROGRESS_TEXT_DRAW;
        }

        if (IsDlgButtonChecked(ID_PROGRESS_PERCENT))
        {
            style |= GX_STYLE_PROGRESS_PERCENT;
        }
        else
        {
            style &= ~GX_STYLE_PROGRESS_PERCENT;
        }

        if (style != mpInfo->style)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);
            mpInfo->style = style;
            gx_widget_style_set(mpInfo->widget, style);
            Is_Modified = TRUE;
        }

        int min, max, current;

        min = GetDlgItemInt(ID_PROGRESS_MINVAL);
        max = GetDlgItemInt(ID_PROGRESS_MAXVAL);
        current = GetDlgItemInt(ID_PROGRESS_CURRENTVAL);

        if (min != mpInfo->ewi.progress.min_val ||
            max != mpInfo->ewi.progress.max_val ||
            current != mpInfo->ewi.progress.current_val)
        {
            UndoManager()->AddEntry(UNDO_TYPE_PROGRESS_BAR_INFO, mpInfo);

            mpInfo->ewi.progress.min_val = min;
            mpInfo->ewi.progress.max_val = max;
            mpInfo->ewi.progress.current_val = current;

            AssignProgressBarInfo(mpInfo);
            Is_Modified = TRUE;
        }

        if (Is_Modified)
        {
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeRadialProgressBarInfo()
{
    if (mpProject && mpInfo && GetDlgItem(ID_RADIAL_PROGRESS_SHOW_TEXT))
    {
        ULONG style = mpInfo->style;
        BOOL Is_Modified = FALSE;

        if (IsDlgButtonChecked(ID_RADIAL_PROGRESS_SHOW_TEXT))
        {
            style |= GX_STYLE_PROGRESS_TEXT_DRAW;
        }
        else
        {
            style &= ~GX_STYLE_PROGRESS_TEXT_DRAW;
        }

        if (IsDlgButtonChecked(ID_RADIAL_PROGRESS_PERCENT))
        {
            style |= GX_STYLE_PROGRESS_PERCENT;
        }
        else
        {
            style &= ~GX_STYLE_PROGRESS_PERCENT;
        }

        if (IsDlgButtonChecked(ID_RADIAL_PROGRESS_BACKTRACK))
        {
            style &= ~GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK;
        }
        else
        {
            style |= GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK;
        }

        if (IsDlgButtonChecked(ID_RADIAL_PROGRESS_ALIAS))
        {
            style |= GX_STYLE_RADIAL_PROGRESS_ALIAS;
        }
        else
        {
            style &= ~GX_STYLE_RADIAL_PROGRESS_ALIAS;
        }

        if (IsDlgButtonChecked(ID_RADIAL_PROGRESS_ROUND))
        {
            style |= GX_STYLE_RADIAL_PROGRESS_ROUND;
        }
        else
        {
            style &= ~GX_STYLE_RADIAL_PROGRESS_ROUND;
        }

        if (style != mpInfo->style)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);
            mpInfo->style = style;
            gx_widget_style_set(mpInfo->widget, style);
            Is_Modified = TRUE;
        }
        else
        {
            GX_RADIAL_PROGRESS_BAR_INFO info = mpInfo->ewi.radial_progress;

            info.gx_radial_progress_bar_info_xcenter = GetDlgItemInt(ID_RADIAL_PROGRESS_XCENTER, NULL, TRUE);;
            info.gx_radial_progress_bar_info_ycenter = GetDlgItemInt(ID_RADIAL_PROGRESS_YCENTER, NULL, TRUE);;
            info.gx_radial_progress_bar_info_radius = GetDlgItemInt(ID_RADIAL_PROGRESS_RADIUS, NULL, TRUE);;
            info.gx_radial_progress_bar_info_anchor_val = GetDlgItemInt(ID_RADIAL_PROGRESS_ANCHORVAL, NULL, TRUE);;
            info.gx_radial_progress_bar_info_current_val = GetDlgItemInt(ID_RADIAL_PROGRESS_CURRENTVAL, NULL, TRUE);;
            info.gx_radial_progress_bar_info_normal_brush_width = GetDlgItemInt(ID_RADIAL_PROGRESS_NORMAL_BRUSH_WIDTH, NULL, TRUE);;
            info.gx_radial_progress_bar_info_selected_brush_width = GetDlgItemInt(ID_RADIAL_PROGRESS_SELECTED_BRUSH_WIDTH, NULL, TRUE);;

            CComboBox *pBrushColor = (CComboBox *)GetDlgItem(ID_RADIAL_PROGRESS_NORMAL_BRUSH_COLOR);

            if (pBrushColor)
            {
                int box_index = pBrushColor->GetCurSel();

                info.gx_radial_progress_bar_info_normal_brush_color = pBrushColor->GetItemData(box_index);
            }

            pBrushColor = (CComboBox *)GetDlgItem(ID_RADIAL_PROGRESS_SELECTED_BRUSH_COLOR);

            if (pBrushColor)
            {
                int box_index = pBrushColor->GetCurSel();

                info.gx_radial_progress_bar_info_selected_brush_color = pBrushColor->GetItemData(box_index);
            }

            if (memcmp(&info, &mpInfo->ewi.radial_progress, sizeof(GX_RADIAL_PROGRESS_BAR_INFO)))
            {
                UndoManager()->AddEntry(UNDO_TYPE_RADIAL_PROGRES_BAR_INFO, mpInfo);
                mpInfo->ewi.radial_progress = info;
                gx_radial_progress_bar_info_set((GX_RADIAL_PROGRESS_BAR *)mpInfo->widget, &mpInfo->ewi.radial_progress);
                Is_Modified = TRUE;
            }
        }

        if (Is_Modified)
        {
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeChartInfo()
{
    if (mpProject && mpInfo && GetDlgItem(ID_CHART_AXIS_COLOR))
    {
        GX_LINE_CHART_INFO chart_info;
        int box_index;
        int colorId;

        CComboBox *pColorBox = (CComboBox *)GetDlgItem(ID_CHART_AXIS_COLOR);
        if (pColorBox)
        {
            box_index = pColorBox->GetCurSel();
            colorId = pColorBox->GetItemData(box_index);
            chart_info.gx_line_chart_axis_color = colorId;
        }

        pColorBox = (CComboBox *)GetDlgItem(ID_CHART_LINE_COLOR);
        if (pColorBox)
        {
            box_index = pColorBox->GetCurSel();
            colorId = pColorBox->GetItemData(box_index);
            chart_info.gx_line_chart_line_color = colorId;
        }

        chart_info.gx_line_chart_min_val = GetDlgItemInt(ID_CHART_MIN_VAL, NULL, TRUE);
        chart_info.gx_line_chart_max_val = GetDlgItemInt(ID_CHART_MAX_VAL, NULL, TRUE);

        chart_info.gx_line_chart_left_margin = GetDlgItemInt(ID_CHART_LEFT_MARGIN, NULL, TRUE);
        chart_info.gx_line_chart_top_margin = GetDlgItemInt(ID_CHART_TOP_MARGIN, NULL, TRUE);
        chart_info.gx_line_chart_right_margin = GetDlgItemInt(ID_CHART_RIGHT_MARGIN, NULL, TRUE);
        chart_info.gx_line_chart_bottom_margin = GetDlgItemInt(ID_CHART_BOTTOM_MARGIN, NULL, TRUE);

        chart_info.gx_line_chart_min_val = GetDlgItemInt(ID_CHART_MIN_VAL, NULL, TRUE);
        chart_info.gx_line_chart_min_val = GetDlgItemInt(ID_CHART_MIN_VAL, NULL, TRUE);
        chart_info.gx_line_chart_min_val = GetDlgItemInt(ID_CHART_MIN_VAL, NULL, TRUE);
        chart_info.gx_line_chart_max_data_count = GetDlgItemInt(ID_CHART_MAX_COUNT, NULL, TRUE);
        chart_info.gx_line_chart_axis_line_width = GetDlgItemInt(ID_CHART_AXIS_WIDTH, NULL, TRUE);
        chart_info.gx_line_chart_data_line_width = GetDlgItemInt(ID_CHART_LINE_WIDTH, NULL, TRUE);

        chart_info.gx_line_chart_active_data_count = 0;
        chart_info.gx_line_chart_data = NULL;

        if (memcmp(&chart_info, &mpInfo->ewi.line_chart_info, sizeof(GX_LINE_CHART_INFO)))
        {
            UndoManager()->AddEntry(UNDO_TYPE_CHART_INFO, mpInfo);

            mpInfo->ewi.line_chart_info = chart_info;

            GX_LINE_CHART *chart = (GX_LINE_CHART *)mpInfo->widget;
            chart->gx_line_chart_info = chart_info;
            gx_system_dirty_mark((GX_WIDGET *)chart);

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeRadialSliderInfo()
{
    if (mpProject && mpInfo && GetDlgItem(ID_RADIAL_SLIDER_XCENTER))
    {
        radial_slider_info info = mpInfo->ewi.radial_slider;

        info.xcenter = GetDlgItemInt(ID_RADIAL_SLIDER_XCENTER, NULL, TRUE);
        info.ycenter = GetDlgItemInt(ID_RADIAL_SLIDER_YCENTER, NULL, TRUE);
        info.radius = GetDlgItemInt(ID_RADIAL_SLIDER_RADIUS, NULL, TRUE);
        info.track_width = GetDlgItemInt(ID_RADIAL_SLIDER_TRACK_WIDTH, NULL, TRUE);
        info.needle_offset = GetDlgItemInt(ID_RADIAL_SLIDER_NEEDLE_OFFSET, NULL, TRUE);
        info.current_angle = GetDlgItemInt(ID_RADIAL_SLIDER_CURRENT_ANGLE, NULL, TRUE);
        info.min_angle = GetDlgItemInt(ID_RADIAL_SLIDER_MIN_ANGLE, NULL, TRUE);
        info.max_angle = GetDlgItemInt(ID_RADIAL_SLIDER_MAX_ANGLE, NULL, TRUE);
        info.animation_total_steps = GetDlgItemInt(ID_RADIAL_SLIDER_TOTAL_STEPS, NULL, TRUE);
        info.animation_delay = GetDlgItemInt(ID_RADIAL_SLIDER_DELAY, NULL, TRUE);

        CComboBox *pBox = (CComboBox *)GetDlgItem(ID_RADIAL_SLIDER_ANIMATION_STYLE);
        int box_index = pBox->GetCurSel();
        info.animation_style = (GX_VALUE)pBox->GetItemData(box_index);

        if (memcmp(&info, &mpInfo->ewi.radial_slider, sizeof(radial_slider_info)))
        {
            UndoManager()->AddEntry(UNDO_TYPE_RADIAL_SLIDER_INFO, mpInfo);

            radial_slider_service_provider *provider = (radial_slider_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);
            provider->SetRadialSliderInfo(mpInfo, &info);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnSpriteStyleChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_SPRITE_AUTO))
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (IsDlgButtonChecked(ID_SPRITE_AUTO))
        {
            mpInfo->style |= GX_STYLE_SPRITE_AUTO;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_SPRITE_AUTO;
        }
        if (IsDlgButtonChecked(ID_SPRITE_LOOP))
        {
            mpInfo->style |= GX_STYLE_SPRITE_LOOP;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_SPRITE_LOOP;
        }

        // we don't update the sprite instance, because we don't want it running
        // inside Studio.
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnEditSpriteFrames()
{
    sprite_edit_dialog dlg(mpInfo, this);
    if (dlg.DoModal() == IDOK)
    {
        GX_SPRITE *sprite = (GX_SPRITE *)mpInfo->widget;

        if (sprite)
        {
            gx_sprite_frame_list_set(sprite, mpInfo->ewi.sprite.framelist, mpInfo->ewi.sprite.frame_count);
            mpProject->SetModified();
        }

        GetDlgItem(ID_SPRITE_EDIT_FRAMES)->SetFocus();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeGaugeInfo()
{
    if (mpProject && mpInfo && GetDlgItem(ID_GAUGE_START_ANGLE))
    {
        GX_CIRCULAR_GAUGE_INFO gauge_info = mpInfo->ewi.gauge.info;
        INT start_angle = mpInfo->ewi.gauge.start_angle;

        start_angle = GetDlgItemInt(ID_GAUGE_START_ANGLE, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_animation_steps = GetDlgItemInt(ID_GAUGE_ANIMATION_STEPS, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_animation_delay = GetDlgItemInt(ID_GAUGE_ANIMATION_DELAY, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_needle_xpos = GetDlgItemInt(ID_GAUGE_NEEDLE_XPOS, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_needle_ypos = GetDlgItemInt(ID_GAUGE_NEEDLE_YPOS, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_needle_xcor = GetDlgItemInt(ID_GAUGE_NEEDLE_XCOR, NULL, TRUE);
        gauge_info.gx_circular_gauge_info_needle_ycor = GetDlgItemInt(ID_GAUGE_NEEDLE_YCOR, NULL, TRUE);

        if (memcmp(&gauge_info, &mpInfo->ewi.gauge.info, sizeof(GX_CIRCULAR_GAUGE_INFO)) ||
            (start_angle != mpInfo->ewi.gauge.start_angle))
        {
            UndoManager()->AddEntry(UNDO_TYPE_CIRCULAR_GAUGE_INFO, mpInfo);

            GX_CIRCULAR_GAUGE *gauge = (GX_CIRCULAR_GAUGE *)mpInfo->widget;
            mpInfo->ewi.gauge.info = gauge_info;
            mpInfo->ewi.gauge.start_angle = start_angle;
            gauge->gx_circular_gauge_info = mpInfo->ewi.gauge.info;
            gx_system_dirty_mark((GX_WIDGET *)gauge);

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeScrollWheelProps()
{
    if (mpProject && mpInfo && GetDlgItem(ID_XPOS))
    {
        scroll_wheel_info wheel_info = mpInfo->ewi.scroll_wheel;

        wheel_info.total_rows = GetDlgItemInt(ID_SCROLL_WHEEL_TOTAL_ROWS, NULL, FALSE);
        wheel_info.selected_row = GetDlgItemInt(ID_SCROLL_WHEEL_SELECTED_ROW, NULL, FALSE);
        wheel_info.start_alpha = GetDlgItemInt(ID_SCROLL_WHEEL_START_ALPHA, NULL, FALSE);
        wheel_info.end_alpha = GetDlgItemInt(ID_SCROLL_WHEEL_END_ALPHA, NULL, FALSE);
        wheel_info.row_height = GetDlgItemInt(ID_SCROLL_WHEEL_ROW_HEIHGT, NULL, FALSE);

        if (memcmp(&wheel_info, &mpInfo->ewi.scroll_wheel, sizeof(scroll_wheel_info)))
        {
            UndoManager()->AddEntry(UNDO_TYPE_SCROLL_WHEEL_INFO, mpInfo);

            scroll_wheel_service_provider *provider = (scroll_wheel_service_provider *) widget_factory::GetServiceProvider(mpInfo->basetype);

            if (provider)
            {
                provider->AssignScrollWheelInfo(mpInfo, &wheel_info);
            }

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTextScrollWheelProps()
{
    if (mpProject && mpInfo && GetDlgItem(ID_XPOS))
    {
        ULONG style = mpInfo->style;

        CComboBox *pFontBox = (CComboBox *)GetDlgItem(ID_TEXT_SCROLL_WHEEL_NORMAL_FONT);
        int box_index = pFontBox->GetCurSel();
        int normal_font = pFontBox->GetItemData(box_index);
        
        pFontBox = (CComboBox *)GetDlgItem(ID_TEXT_SCROLL_WHEEL_SELECTED_FONT);
        box_index = pFontBox->GetCurSel();
        int selected_font = pFontBox->GetItemData(box_index);

        if (IsDlgButtonChecked(ID_TEXT_SCROLL_WHEEL_ROUND))
        {
            style |= GX_STYLE_TEXT_SCROLL_WHEEL_ROUND;
        }
        else
        {
            style &= ~GX_STYLE_TEXT_SCROLL_WHEEL_ROUND;
        }

        if (style != mpInfo->style)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

            mpInfo->style = style;
            gx_widget_style_set(mpInfo->widget, mpInfo->style);

            mpProject->SetModified();
        }
        else if ((normal_font != mpInfo->font_id[NORMAL_FONT_INDEX]) ||
            (selected_font != mpInfo->font_id[SELECTED_FONT_INDEX]))
        {
            UndoManager()->AddEntry(UNDO_TYPE_TEXT_SCROLL_WHEEL_INFO, mpInfo);

            text_scroll_wheel_service_provider *provider = (text_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);

            if (provider)
            {
                provider->AssignFont(mpInfo, NORMAL_FONT_INDEX, normal_font);
                provider->AssignFont(mpInfo, SELECTED_FONT_INDEX, selected_font);
            }

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeStringScrollWheelProps()
{
    if (mpProject && mpInfo && GetDlgItem(ID_XPOS))
    {
        CString callback;

        GetDlgItemText(ID_CALLBACK_NAME, callback);

        if (callback.IsEmpty())
        {
            GetDlgItem(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS)->EnableWindow(FALSE);
        }

        if (callback != mpInfo->callback_func)
        {
            UndoManager()->AddEntry(UNDO_TYPE_STRING_SCROLL_WHEEL_INFO, mpInfo);
            string_scroll_wheel_service_provider *provider = (string_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);

            if (provider)
            {
                provider->StringEditEnableDisable(mpInfo, callback.IsEmpty());
            }

            mpInfo->callback_func = callback;

            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnEditStringScrolWheelStrings()
{
    string_scroll_wheel_service_provider *provider;
    provider = (string_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);

    if (mpProject && mpInfo && provider && GetDlgItem(ID_XPOS))
    {
        if (!mpInfo->ewi.string_scroll_wheel.base.total_rows)
        {
            ErrorMsg(L"Please set total rows first.", this);
        }
        else
        {

            CArray<GX_RESOURCE_ID> list_array;

            provider->InitStringIdListArray(mpInfo->ewi.string_scroll_wheel.string_id_list, mpInfo->ewi.string_scroll_wheel.base.total_rows, list_array);

            int display = mpProject->GetDisplayIndex(mpInfo);
            string_scroll_wheel_edit_dlg dlg(display, &list_array, this);

            if (dlg.DoModal() == IDOK)
            {
                UndoManager()->AddEntry(UNDO_TYPE_STRING_SCROLL_WHEEL_INFO, mpInfo);

                if (mpInfo->ewi.string_scroll_wheel.string_id_list)
                {
                    delete mpInfo->ewi.string_scroll_wheel.string_id_list;
                    mpInfo->ewi.string_scroll_wheel.string_id_list = NULL;
                }

                provider->CreateStringIdList(mpInfo, list_array);

                mpProject->SetModified();
            }
        }

        GetDlgItem(ID_STRING_SCROLL_WHEEL_EDIT_STRINGS)->SetFocus();
    }
}

///////////////////////////////////////////////////////////////////////////////
afx_msg void properties_win::OnChangeNumericScrollWheelRange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_XPOS))
    {
        int start_val = GetDlgItemInt(ID_NUMERIC_SCROLL_WHEEL_START_VAL, FALSE, TRUE);
        int end_val = GetDlgItemInt(ID_NUMERIC_SCROLL_WHEEL_END_VAL, FALSE, TRUE);

        if ((start_val != mpInfo->ewi.numeric_scroll_wheel.start_val) ||
            (end_val != mpInfo->ewi.numeric_scroll_wheel.end_val))
        {
            numeric_scroll_wheel_service_provider *provider = (numeric_scroll_wheel_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);
            if (provider)
            {
                UndoManager()->AddEntry(UNDO_TYPE_NUMRIC_SCROLL_WHEEL_INFO, mpInfo);

                if (mpInfo->ewi.numeric_scroll_wheel.base.total_rows != 0)
                {
                    mpInfo->ewi.numeric_scroll_wheel.base.total_rows = 0;
                    SetDlgItemInt(ID_SCROLL_WHEEL_TOTAL_ROWS, 0);
                }
                provider->AssignValueRange(mpInfo, start_val, end_val);
                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeListWrap()
{
    CButton *pb = (CButton *)GetDlgItem(ID_LIST_WRAP);

    if (mpProject && mpInfo && pb)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (pb->GetCheck() == BST_CHECKED)
        {
            mpInfo->style |= GX_STYLE_WRAP;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_WRAP;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeListRepeatSelect()
{
    CButton *pb = (CButton *)GetDlgItem(ID_LIST_REPEAT_SELECT);

    if (mpProject && mpInfo && pb)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);

        if (pb->GetCheck() == BST_CHECKED)
        {
            mpInfo->style |= GX_STYLE_REPEAT_SELECT;
        }
        else
        {
            mpInfo->style &= ~GX_STYLE_REPEAT_SELECT;
        }
        gx_widget_style_set(mpInfo->widget, mpInfo->style);
        mpProject->SetModified();
    }
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeListRows()
{
    if (mpProject && mpInfo && GetDlgItem(ID_LIST_ROWS))
    {
        INT total_rows;

        switch(mpInfo->basetype)
        {
        case GX_TYPE_VERTICAL_LIST:
        case GX_TYPE_HORIZONTAL_LIST:
            total_rows = GetDlgItemInt(ID_LIST_ROWS, NULL, TRUE);

            if (total_rows != mpInfo->ewi.vlist.total_rows)
            {
                UndoManager()->AddEntry(UNDO_TYPE_LIST_ROWS, mpInfo);
                mpInfo->ewi.vlist.total_rows = total_rows;
                mpProject->SetModified();
            }
            break;

        case GX_TYPE_DROP_LIST:
            total_rows = GetDlgItemInt(ID_LIST_ROWS, NULL, TRUE);

            if (total_rows != mpInfo->ewi.drop_list.total_rows)
            {
                UndoManager()->AddEntry(UNDO_TYPE_LIST_ROWS, mpInfo);
                mpInfo->ewi.drop_list.total_rows = total_rows;
                mpProject->SetModified();
            }
            break;

        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeOpenHeight()
{
    if (mpProject && mpInfo && GetDlgItem(ID_OPEN_HEIGHT))
    {
        INT open_height = GetDlgItemInt(ID_OPEN_HEIGHT, NULL, TRUE);

        if (open_height != mpInfo->ewi.drop_list.open_height)
        {
            UndoManager()->AddEntry(UNDO_TYPE_OPEN_HEIGHT, mpInfo);

            mpInfo->ewi.drop_list.open_height = open_height;

            GX_DROP_LIST *drop = (GX_DROP_LIST *)mpInfo->widget;
            if (drop->gx_drop_list_popup_open)
            {
                gx_drop_list_close(drop);
                gx_drop_list_open(drop);
            }
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTextViewLineSpace()
{
    if (mpProject && mpInfo && GetDlgItem(ID_TEXT_VIEW_LINE_SPACE))
    {

        int line_space = GetDlgItemInt(ID_TEXT_VIEW_LINE_SPACE);

        if (line_space != mpInfo->ewi.text_info.line_space)
        {
            if ((line_space < -128) || (line_space > 127))
            {
                Notify("Invalid value for line space. Valid range is from -128 to 127.", this);
                SetDlgItemInt(ID_TEXT_VIEW_LINE_SPACE, mpInfo->ewi.text_info.line_space);
            }
            else
            {
                UndoManager()->AddEntry(UNDO_TYPE_TEXT_VIEW_LINE_SPACE, mpInfo);

                mpInfo->ewi.text_info.line_space = line_space;
                gx_multi_line_text_view_line_space_set((GX_MULTI_LINE_TEXT_VIEW*)mpInfo->widget, line_space);

                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTextViewWhitespace()
{
    if (mpProject && mpInfo && GetDlgItem(ID_TEXT_VIEW_WHITESPACE))
    {
        int whitespace = GetDlgItemInt(ID_TEXT_VIEW_WHITESPACE);

        if (whitespace != mpInfo->ewi.text_info.whitespace)
        {
            if ((whitespace < 0) || (whitespace > 255))
            {
                Notify("Invalid value for whitespace. Valid range is from 0 to 255.", this);
                SetDlgItemInt(ID_TEXT_VIEW_WHITESPACE, mpInfo->ewi.text_info.whitespace);
            }
            else
            {
                UndoManager()->AddEntry(UNDO_TYPE_TEXT_VIEW_WHITESPACE, mpInfo);

                mpInfo->ewi.text_info.whitespace = whitespace;
                gx_multi_line_text_view_whitespace_set((GX_MULTI_LINE_TEXT_VIEW*)mpInfo->widget, whitespace);

                mpProject->SetModified();
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTextBufferSize(void)
{
    GX_SINGLE_LINE_TEXT_INPUT *pi;
    GX_MULTI_LINE_TEXT_INPUT *mi;

    if (mpProject && mpInfo && GetDlgItemInt(ID_BUFFER_SIZE))
    {
        int newsize = GetDlgItemInt(ID_BUFFER_SIZE);
        if (newsize == 1 || newsize > (100 * 1024))
        {
            SetDlgItemInt(ID_BUFFER_SIZE, mpInfo->ewi.text_info.buffer_size);
            return;
        }

        if (newsize != mpInfo->ewi.text_info.buffer_size)
        {
            UndoManager()->AddEntry(UNDO_TYPE_TEXT_BUFFER_SIZE, mpInfo);

            if (newsize > mpInfo->ewi.text_info.buffer_size)
            {
                // allocate a new buffer and delete the old one

                GX_CHAR *newbuffer = new GX_CHAR[newsize + 1];
                GX_CHAR *oldbuffer = NULL;

                if (mpInfo->widget)
                {
                    switch (mpInfo->basetype)
                    {
                    case GX_TYPE_SINGLE_LINE_TEXT_INPUT:
                        pi = (GX_SINGLE_LINE_TEXT_INPUT *)mpInfo->widget;
                        oldbuffer = pi->gx_single_line_text_input_buffer;
                        pi->gx_single_line_text_input_buffer = newbuffer;
                        break;

                    case GX_TYPE_MULTI_LINE_TEXT_INPUT:
                        mi = (GX_MULTI_LINE_TEXT_INPUT *)mpInfo->widget;
                        oldbuffer = (GX_CHAR *) mi->gx_multi_line_text_view_text.gx_string_ptr;
                        mi->gx_multi_line_text_view_text.gx_string_ptr = newbuffer;
                        break;

                    default:
                        break;
                    }
                }
                if (oldbuffer)
                {
                    delete[] oldbuffer;
                }
            }
            mpInfo->ewi.text_info.buffer_size = newsize;
            if (mpInfo->string_id[0])
            {
                // if this guy has a string_id, re-assign the text:
                OnChangeStringId();
            }
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeStringId()
{
    CComboBox *pbox = (CComboBox *) GetDlgItem(ID_STRING_ID);
    string_table *table = GetActiveStringTable();

    if (mpProject && mpInfo && pbox && table)
    {
        int text_id = pbox->GetItemData(pbox->GetCurSel());

        if ((mpInfo->string_id[0] != text_id) ||
            (mpInfo->basetype == GX_TYPE_MULTI_LINE_TEXT_INPUT) ||
            (mpInfo->basetype == GX_TYPE_SINGLE_LINE_TEXT_INPUT))
        {
            UndoManager()->AddEntry(UNDO_TYPE_STRING, mpInfo, 0);
            mpInfo->string_id[0] = text_id;
            widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);
        
            if (provider)
            {
                provider->AssignText(mpInfo, 0, text_id);
            }
            CRichEditCtrl *pEdit = (CRichEditCtrl *) GetDlgItem(ID_STRING_VAL);
            CString id_name = table->GetResourceIdName(text_id);
            SetUtf8Text(pEdit, table->GetString(id_name));
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeStringText()
{
    CString tval;

    string_table *table = GetActiveStringTable();

    if (mpProject && mpInfo && table)
    {
        CRichEditCtrl *pEdit = (CRichEditCtrl *) GetDlgItem(ID_STRING_VAL);

        if (pEdit == NULL)
        {
            return;
        }
        tval = GetUtf8Text(pEdit);
        if (!pEdit)
        {
            return;
        }

        CComboBox *pbox = (CComboBox *) GetDlgItem(ID_STRING_ID);

        if (!pbox)
        {
            return;
        }
        CString id_name;
        GX_RESOURCE_ID resource_id;

        if (pbox)
        {
            resource_id = pbox->GetItemData(pbox->GetCurSel());

            if (resource_id)
            {
                id_name = table->GetResourceIdName(resource_id);

                if (table->GetString(id_name) == tval)
                {
                    // no change, just return
                    return;
                }
            }

            if (!tval.IsEmpty())
            {
                int string_index = table->CheckAddString(tval);
                id_name = table->GetStringId(string_index);
                resource_id = table->GetResourceId(id_name);

                table->Sort();
                GetResourceView()->RebuildStringItems();
            }
            else
            {
                resource_id = 0;
            }

            // select this string id in drop down list. We might have
            // to add a new entry before selecting
            if (pbox->GetCount() == resource_id)
            {
                int index = pbox->AddString(id_name);
                pbox->SetItemData(index, resource_id);
            }
            SelectDropListItem(pbox, resource_id);

            // assign this string id to the widget:
            widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

            if (provider)
            {
                UndoManager()->AddEntry(UNDO_TYPE_STRING, mpInfo, 0);
                provider->AssignText(mpInfo, 0, resource_id);
            }
            mpProject->SetModified();
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void properties_win::OnScrollAppearanceChange()
{
    if (mpProject && mpInfo && GetDlgItem(ID_SCROLL_THUMB_COLOR))
    {
        GX_SCROLLBAR_APPEARANCE appearance = mpInfo->ewi.scroll;

        CComboBox *pBox = (CComboBox *) GetDlgItem(ID_SCROLL_THUMB_COLOR);

        if (pBox)
        {
            int box_index = pBox->GetCurSel();
            int ColorId = pBox->GetItemData(box_index);
            appearance.gx_scroll_thumb_color = ColorId;
        }

        pBox = (CComboBox *) GetDlgItem(ID_SCROLL_THUMB_BORDER_COLOR);

        if (pBox)
        {
            int box_index = pBox->GetCurSel();
            int ColorId = pBox->GetItemData(box_index);
            appearance.gx_scroll_thumb_border_color = ColorId;
        }

        pBox = (CComboBox *) GetDlgItem(ID_SCROLL_BUTTON_COLOR);

        if (pBox)
        {
            int box_index = pBox->GetCurSel();
            int ColorId = pBox->GetItemData(box_index);
            appearance.gx_scroll_button_color = ColorId;
        }

        pBox = (CComboBox *) GetDlgItem(ID_SCROLL_THUMB_BORDER_STYLE);
        if (pBox)
        {
            int Selected = pBox->GetCurSel();
            GX_UBYTE style = (GX_UBYTE) pBox->GetItemData(Selected);
            appearance.gx_scroll_thumb_border_style = style;
        }

        if (mpInfo->widget->gx_widget_style & GX_SCROLLBAR_VERTICAL)
        {
            appearance.gx_scroll_width = GetDlgItemInt(ID_WIDTH, NULL, TRUE);
        }
        else
        {
            appearance.gx_scroll_width = GetDlgItemInt(ID_HEIGHT, NULL, TRUE);
        }
        appearance.gx_scroll_thumb_width = GetDlgItemInt(ID_THUMB_WIDTH, NULL, TRUE);
        appearance.gx_scroll_thumb_travel_min = GetDlgItemInt(ID_THUMB_TRAVEL_MIN, NULL, TRUE);
        appearance.gx_scroll_thumb_travel_max = GetDlgItemInt(ID_THUMB_TRAVEL_MAX, NULL, TRUE);

        if (memcmp(&appearance, &mpInfo->ewi.scroll, sizeof(GX_SCROLLBAR_APPEARANCE)))
        {
            UndoManager()->AddEntry(UNDO_TYPE_SCROLL_APPEARANCE, mpInfo);

            widget_factory::ReplaceScrollbar(mpInfo, &appearance);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnScrollStyleChange()
{
    if (!mpProject || !mpInfo || !GetDlgItem(ID_SCROLL_END_BUTTONS))
    {
        return;
    }

    ULONG style = mpInfo->style;

    CButton *pb = (CButton *) GetDlgItem(ID_SCROLL_END_BUTTONS);
    if (pb)
    {
        if (pb->GetCheck() == BST_CHECKED)
        {
            style |= GX_SCROLLBAR_END_BUTTONS;
        }
        else
        {
            style &= ~GX_SCROLLBAR_END_BUTTONS;
        }
    }
    pb = (CButton *) GetDlgItem(ID_SCROLL_TILE_BKGND);
    if (pb)
    {
        if (pb->GetCheck() == BST_CHECKED)
        {
            style |= GX_STYLE_TILE_BACKGROUND;
        }
        else
        {
            style &= ~GX_STYLE_TILE_BACKGROUND;
        }
    }
    pb = (CButton *) GetDlgItem(ID_SCROLL_RELATIVE_THUMB);
    if (pb)
    {
        if (pb->GetCheck() == BST_CHECKED)
        {
            style |= GX_SCROLLBAR_RELATIVE_THUMB;
        }
        else
        {
            style &= ~GX_SCROLLBAR_RELATIVE_THUMB;
        }
    }

    if (style != mpInfo->style)
    {
        UndoManager()->AddEntry(UNDO_TYPE_SCROLL_STYLE, mpInfo);

        mpInfo->style = style;
        GX_SCROLLBAR_APPEARANCE appearance = mpInfo->ewi.scroll;
        widget_factory::ReplaceScrollbar(mpInfo, &appearance);

        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTemplate()
{
    CButton *pb = (CButton *)GetDlgItem(ID_TEMPLATE);
    BOOL  check;

    if (mpProject && mpInfo && mpInfo->widget && pb)
    {
        check = pb->GetCheck();
        // We always check valid name when edit widget name, no need to check again

        if (check)
        {
            if (!widget_factory::FindBaseName(mpInfo->app_name))
            {
                UndoManager()->AddEntry(UNDO_TYPE_TEMPLATE, mpInfo);
                mpInfo->is_template = check;

                // templates cannot be dynamically allocated, only widgets derived from templates
                // are dynammically allocated
                mpInfo->allocation = STATICALLY_ALLOCATED;
                SetChildAllocation(mpInfo->GetChildWidgetInfo(), STATICALLY_ALLOCATED);
            }
            else
            {
                ErrorMsg("Template name already exists.");
                pb->SetFocus();
                pb->SetCheck(FALSE);
                mpInfo->is_template = FALSE;
            }
        }
        else
        {
            if (template_service_provider::CountDerivedWidgets(mpInfo) == 0)
            {
                // OK to remove template status from this widget:
                UndoManager()->AddEntry(UNDO_TYPE_TEMPLATE, mpInfo);
                mpInfo->is_template = check;
            }
            else
            {
                ErrorMsg("This template cannot be removed until all derived widgets are first deleted.");
                pb->SetCheck(TRUE);
                mpInfo->is_template = TRUE;
            }
        }

        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeVisibleAtStartup()
{
    if (mpProject && mpInfo && GetDlgItem(ID_WIDGET_NAME))
    {
        UndoManager()->AddEntry(UNDO_TYPE_VISIBLE_AT_STARTUP, mpInfo);

        if (IsDlgButtonChecked(ID_VISIBLE_AT_STARTUP))
        {
            mpInfo->visible_at_startup = TRUE;
        }
        else
        {
            mpInfo->visible_at_startup = FALSE;
        }

        mpProject->SetModified();
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeMenuInfo()
{
    if (!mpProject || !mpInfo || !GetDlgItem(ID_MENU_EXPANDED))
    {
        return;
    }

    ULONG style = mpInfo->style;

    CButton *pb = (CButton *)GetDlgItem(ID_MENU_EXPANDED);
    if (pb)
    {
        if (pb->GetCheck() == BST_CHECKED)
        {
            style |= GX_STYLE_MENU_EXPANDED;
        }
        else
        {
            style &= ~GX_STYLE_MENU_EXPANDED;
        }
    }

    menu_service_provider *provider = (menu_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);

    if (style != mpInfo->style)
    {
        UndoManager()->AddEntry(UNDO_TYPE_MENU_INFO, mpInfo);

        provider->AssignStyle(mpInfo, style);
        mpProject->SetModified();
        return;
    }

    int text_x_offset = GetDlgItemInt(ID_MENU_TEXT_X_OFFSET, NULL, TRUE);
    int text_y_offset = GetDlgItemInt(ID_MENU_TEXT_Y_OFFSET, NULL, TRUE);

    int insert_as_menu_item;
    pb = (CButton *)GetDlgItem(ID_MENU_INSERT_MODE);
    if (pb)
    {
        insert_as_menu_item = pb->GetCheck();
    }

    if ((text_x_offset != mpInfo->ewi.menu.text_x_offset) ||
        (text_y_offset != mpInfo->ewi.menu.text_y_offset) ||
        (insert_as_menu_item != mpInfo->ewi.menu.insert_as_menu_item))
    {
        UndoManager()->AddEntry(UNDO_TYPE_MENU_INFO, mpInfo);
        mpInfo->ewi.menu.insert_as_menu_item = insert_as_menu_item;

        provider->AssignTextOffset(mpInfo, text_x_offset, text_y_offset);
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnChangeTreeViewInfo()
{
    if (!mpProject || !mpInfo || !GetDlgItem(ID_TREE_VIEW_INDENTATION))
    {
        return;
    }

    int indentation;
    indentation = GetDlgItemInt(ID_TREE_VIEW_INDENTATION, GX_FALSE, GX_TRUE);
    if (indentation != mpInfo->ewi.tree_view_indentation)
    {
        //tree view indentation is modified
        UndoManager()->AddEntry(UNDO_TYPE_TREE_VIEW_INFO, mpInfo);
        mpInfo->ewi.tree_view_indentation = indentation;
        gx_tree_view_indentation_set((GX_TREE_VIEW *)mpInfo->widget, indentation);
        GetProjectView()->CheckParentRefresh(mpInfo);
        return;
    }

    ULONG style = mpInfo->style;

    CButton *pb = (CButton *)GetDlgItem(ID_TREE_VIEW_SHOW_ROOT_LINES);
    if (pb)
    {
        if (pb->GetCheck() == BST_CHECKED)
        {
            style |= GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES;
        }
        else
        {
            style &= ~GX_STYLE_TREE_VIEW_SHOW_ROOT_LINES;
        }
    }

    if (style != mpInfo->style)
    {
        UndoManager()->AddEntry(UNDO_TYPE_STYLE, mpInfo);
        mpInfo->style = style;
        gx_widget_style_set(mpInfo->widget, style);
        mpProject->SetModified();
        return;
    }

    CComboBox *pColorBox = (CComboBox *)GetDlgItem(ID_TREE_VIEW_ROOT_LINE_COLOR);
    if (pColorBox)
    {
        int box_index = pColorBox->GetCurSel();
        int ColorId = pColorBox->GetItemData(box_index);

        widget_service_provider *provider = widget_factory::GetServiceProvider(mpInfo->basetype);

        if ((ColorId != mpInfo->color_id[TREE_VIEW_ROOT_LINE_COLOR_INDEX]) && provider)
        {
            UndoManager()->AddEntry(UNDO_TYPE_COLOR, mpInfo, TREE_VIEW_ROOT_LINE_COLOR_INDEX);
            provider->AssignColor(mpInfo, TREE_VIEW_ROOT_LINE_COLOR_INDEX, ColorId);
            mpProject->SetModified();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnEditRichTextViewText()
{
    rich_text_view_service_provider *provider;
    provider = (rich_text_view_service_provider *)widget_factory::GetServiceProvider(mpInfo->basetype);

    if (mpProject && mpInfo && provider && GetDlgItem(ID_RICH_TEXT_VIEW_TEXT_EDIT))
    {
        rich_text_edit_dlg dlg(mpInfo, this);

        if (dlg.DoModal() == IDOK)
        {
            CString text = dlg.GetText();

            GetDlgItem(ID_STRING_VAL)->SetWindowText(text);
            OnChangeStringText();
        }

        GetDlgItem(ID_RICH_TEXT_VIEW_TEXT_EDIT)->SetFocus();
    }
}

///////////////////////////////////////////////////////////////////////////////
void properties_win::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDialog::OnSettingChange(uFlags, lpszSection);

    SetControlSize();
    ResourcesChanged();
}

///////////////////////////////////////////////////////////////////////////////
LRESULT properties_win::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CStringArray param_list;
    int ctrl_id;
    CString param2;

    switch (wParam)
    {
    case TEST_EDIT_WIDGET_PROPS:
    case TEST_EDIT_WIDGET_PROPS_POST:
        if (wParam == TEST_EDIT_WIDGET_PROPS)
        {
            SplitString(GetTestingParam(0), ',', &param_list);
        }
        else
        {
            SplitString(GetTestingParam(1), ',', &param_list);
        }

        if (param_list.GetCount() == 2)
        {
            ctrl_id = _tstoi(param_list.GetAt(0));
            param2 = param_list.GetAt(1);

            CWnd *pWnd = GetDlgItem(ctrl_id);

            if (!pWnd)
            {
                return 0;
            }

            TCHAR  class_name[MAX_PATH];
            GetClassName(pWnd->GetSafeHwnd(), class_name, MAX_PATH - 1);
                
            switch (class_name[0])
            {
            case 'R':
                // "Rich Edit"

            case 'E':
                //"Edit"
                ((CEdit *)pWnd)->SetWindowText(param2);
                SendMessage(WM_COMMAND, MAKEWPARAM(ctrl_id, EN_KILLFOCUS), (LPARAM)((CEdit *)pWnd)->m_hWnd);
                break;
            case 'C':
                //"ComboBox"
                ((CComboBox *)pWnd)->SelectString(-1, param2);
                SendMessage(WM_COMMAND, MAKEWPARAM(ctrl_id, CBN_SELCHANGE), (LPARAM)((CComboBox *)pWnd)->m_hWnd);
                break;
            case 'B':
                //"Button"
                ((CButton *)pWnd)->SetCheck(_tstoi(param2));
                SendMessage(WM_COMMAND, MAKEWPARAM(ctrl_id, BN_CLICKED), (LPARAM)((CButton *)pWnd)->m_hWnd);
                break;
            }
        }
        break;

    default:
        break;
    }
    return 0;
}

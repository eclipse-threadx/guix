#include <math.h>
#include "studiox_includes.h"
#include "easing_function_select_dlg.h"


// easing_function_select_dlg dialog

#define ANIMATION_TOTAL_STEP 50
#define ANIMATION_TIMER_ID   1024
#define ANIMATION_BOX_WIDTH  30
#define ANIMATION_BOX_HEIGHT 16
#define ANIMATION_DOT_RADIUS 10


enum easing_function_select_dlg_test_commands{
    TEST_SELECT_EASING_FUNCTION = 1,
    TEST_SAVE_EASING_FUNCTION_SELECT,
    TEST_CANCEL_EASING_FUNCTION_SELECT
};

STRING_VAL_PAIR easing_function_list[] = {
    { _T("back ease in"), GX_ANIMATION_BACK_EASE_IN },
    { _T("back ease out"), GX_ANIMATION_BACK_EASE_OUT },
    { _T("back ease in out"), GX_ANIMATION_BACK_EASE_IN_OUT },
    { _T("bounce ease in"), GX_ANIMATION_BOUNCE_EASE_IN },
    { _T("bounce ease out"), GX_ANIMATION_BOUNCE_EASE_OUT },
    { _T("bounce ease in out"), GX_ANIMATION_BOUNCE_EASE_IN_OUT },
    { _T("circ ease in"), GX_ANIMATION_CIRC_EASE_IN },
    { _T("circ ease out"), GX_ANIMATION_CIRC_EASE_OUT },
    { _T("circ ease in out"), GX_ANIMATION_CIRC_EASE_IN_OUT },
    { _T("cubic ease in"), GX_ANIMATION_CUBIC_EASE_IN },
    { _T("cubic ease out"), GX_ANIMATION_CUBIC_EASE_OUT },
    { _T("cubic ease in out"), GX_ANIMATION_CUBIC_EASE_IN_OUT },
    { _T("elastic ease in"), GX_ANIMATION_ELASTIC_EASE_IN },
    { _T("elastic ease out"), GX_ANIMATION_ELASTIC_EASE_OUT },
    { _T("elastic ease in out"), GX_ANIMATION_ELASTIC_EASE_IN_OUT },
    { _T("expo ease in"), GX_ANIMATION_EXPO_EASE_IN },
    { _T("expo ease out"), GX_ANIMATION_EXPO_EASE_OUT },
    { _T("expo ease in out"), GX_ANIMATION_EXPO_EASE_IN_OUT },
    { _T("quad ease in"), GX_ANIMATION_QUAD_EASE_IN },
    { _T("quad ease out"), GX_ANIMATION_QUAD_EASE_OUT },
    { _T("quad ease in out"), GX_ANIMATION_QUAD_EASE_IN_OUT },
    { _T("quart ease in"), GX_ANIMATION_QUART_EASE_IN },
    { _T("quart ease out"), GX_ANIMATION_QUART_EASE_OUT },
    { _T("quart ease in out"), GX_ANIMATION_QUART_EASE_IN_OUT },
    { _T("quint ease in"), GX_ANIMATION_QUINT_EASE_IN },
    { _T("quint ease out"), GX_ANIMATION_QUINT_EASE_OUT },
    { _T("quint ease in out"), GX_ANIMATION_QUINT_EASE_IN_OUT },
    { _T("sine ease in"), GX_ANIMATION_SINE_EASE_IN },
    { _T("sine ease out"), GX_ANIMATION_SINE_EASE_OUT },
    { _T("sine ease in out"), GX_ANIMATION_SINE_EASE_IN_OUT },
    { _T(""), 0}
};

IMPLEMENT_DYNAMIC(easing_function_select_dlg, express_dialog)

BEGIN_MESSAGE_MAP(easing_function_select_dlg, express_dialog)
    ON_CBN_SELCHANGE(IDC_EASING_FUNC_COMBO, OnSelecteEasingFuncCombo)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_ANIMATION_START, &easing_function_select_dlg::OnAnimationStart)
    ON_MESSAGE(STUDIO_TEST, &easing_function_select_dlg::OnTestMessage)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
easing_function_select_dlg::easing_function_select_dlg(int easing_function_id, CWnd* pParent /*=NULL*/)
    : express_dialog(easing_function_select_dlg::IDD, pParent)
{
    SetTitleText("Select Easing Function");
    IconId = IDB_CONFIGURE_PROJECTS;
    mEasingFuncId = easing_function_id;

    mWaveRect.left = 0;
    mWaveRect.right = 0;
    mWaveRect.top = 0;
    mWaveRect.bottom = 0;
    mAnimationStep = 0;

    int dpi = GetSystemDPI();
    mAnimationBoxSize.cx = MulDiv(ANIMATION_BOX_WIDTH, dpi, DEFAULT_DPI_96);
    mAnimationBoxSize.cy = MulDiv(ANIMATION_BOX_HEIGHT, dpi, DEFAULT_DPI_96);

    mAnimationDotRadius = MulDiv(ANIMATION_DOT_RADIUS, dpi, DEFAULT_DPI_96);
}

///////////////////////////////////////////////////////////////////////////////
easing_function_select_dlg::~easing_function_select_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
int easing_function_select_dlg::GetEasingFuncId(CString type_name)
{
    return FindPairVal(easing_function_list, type_name);
}

///////////////////////////////////////////////////////////////////////////////
CString easing_function_select_dlg::GetEasingFuncIdName(int type)
{
    return FindPairString(easing_function_list, type);
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EASING_FUNC_COMBO, mEasingFuncCombo);
    DDX_Control(pDX, IDC_ANIMATION_STATUS, mAnimationStatus);

    if (!pDX->m_bSaveAndValidate)
    {
        mAnimationStatus.SetWindowTextW(L"");
        SetLiveRegion(mAnimationStatus.GetSafeHwnd());
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL easing_function_select_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();

    AddCancelButton();
    AddSaveButton();

    STRING_VAL_PAIR *current = easing_function_list;
    int index;
    int sel_index = -1;

    while (current->name.GetLength())
    {
        index = mEasingFuncCombo.AddString(current->name);
        mEasingFuncCombo.SetItemData(index, current->val);
        if (current->val == mEasingFuncId)
        {
            sel_index = index;
        }
        current++;
    }

    if (sel_index >= 0)
    {
        mEasingFuncCombo.SetCurSel(sel_index);
    }
    mEasingFuncCombo.InsertString(0, _T("None"));

    if (sel_index < 0)
    {
        mEasingFuncCombo.SetCurSel(0);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;

    GetDlgItem(IDC_ANIMATION_START)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    mWaveRect.top = rect.bottom + (mAnimationBoxSize.cy << 2);

    mAnimationStatus.GetWindowRect(&rect);
    ScreenToClient(&rect);
    mWaveRect.bottom = rect.top - (mAnimationBoxSize.cy << 2);
    mWaveRect.left = rect.left;
    mWaveRect.right = rect.right;

    mAnimationRect = mWaveRect;
    mAnimationRect.left -= (mAnimationBoxSize.cx * 3 / 2) + 1;
    mAnimationRect.right += (mAnimationDotRadius / 2) + 1;
    mAnimationRect.top -= (mAnimationBoxSize.cy << 2);
    mAnimationRect.bottom += (mAnimationBoxSize.cy << 2);

    CDC        dcMem;
    HBITMAP    hbmMem;
    HANDLE     hOld;

    GetClientRect(&rect);

    /* Flush canvas memory to off-screen DC. */
    dcMem.CreateCompatibleDC(&dc);
    hbmMem = CreateCompatibleBitmap(dc, rect.Width(), rect.Height());
    hOld = SelectObject(dcMem, hbmMem);

    dcMem.FillSolidRect(rect, GetSysColor(COLOR_3DFACE));

    Graphics       g(dcMem);

    Color color;
    color.SetFromCOLORREF(GetSysColor(COLOR_WINDOWTEXT));
    Pen pen(color);

    g.DrawLine(&pen, mWaveRect.left, mWaveRect.top, mWaveRect.left, mWaveRect.bottom);
    g.DrawLine(&pen, mWaveRect.left, mWaveRect.bottom, mWaveRect.right, mWaveRect.bottom);

    DrawEasingCurve(g);

    PaintTitleAndStatusBar(&dcMem);

    /* Transfer the off-screen DC to the screen. */
    BitBlt(dc, 0, 0, rect.Width(), rect.Height(), dcMem, 0, 0, SRCCOPY);

    /* Free-up the off-screen DC.  */
    SelectObject(dcMem, hOld);
    DeleteObject(hbmMem);
    DeleteDC(dcMem);
}

///////////////////////////////////////////////////////////////////////////////
BOOL easing_function_select_dlg::OnEraseBkgnd(CDC* pDC)
{
    return GX_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == ANIMATION_TIMER_ID)
    {
        mAnimationStep++;

        CString status("");
        if (mAnimationStep == 1)
        {
            if (mEasingFuncId)
            {
                status = GetEasingFuncIdName(mEasingFuncId);
            }
            else
            {
                status = L"linear";
            }
            
            status.Append(L" animation is started!");
        }
        else if (mAnimationStep == ANIMATION_TOTAL_STEP)
        {
            if (mEasingFuncId)
            {
                status = GetEasingFuncIdName(mEasingFuncId);
            }
            else
            {
                status = L"linear";
            }

            status.Append(L" animation is completed!");
            KillTimer(ANIMATION_TIMER_ID);
        }

        if (!status.IsEmpty())
        {
            mAnimationStatus.SetWindowTextW(status);

            // Raise a UIA LiveRegionChanged
            // event so that a screen reader is made aware of a change to the LiveRegion.
            // Make sure the updated text is set on the label before making this call.
            mAnimationStatus.NotifyWinEvent(
                EVENT_OBJECT_LIVEREGIONCHANGED,
                OBJID_CLIENT,
                CHILDID_SELF);
        }

        InvalidateRect(&mAnimationRect);
    }
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::OnAnimationStart()
{
    mAnimationStep = 0;
    SetTimer(ANIMATION_TIMER_ID, USER_TIMER_MINIMUM, GX_NULL);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BackEaseIn(float t, float d)
{
    t /= d;
    return (float)(t *t*(2.70158 * t - 1.70158));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BackEaseOut(float t, float d)
{
    t = t / d - 1;
    return (float)(t * t * (2.70158 * t + 1.70158) + 1);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BackEaseInOut(float t, float d)
{
    t = (2 * t / d);

    if (t < 1)
    {
        return (float)(t * t * (3.5949 * t - 2.5949) / 2);
    }
    else
    {
        t -= 2;
        return (float)(t * t * (3.5949 * t + 2.5949) / 2 + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BounceEaseIn(float t, float d)
{
    return (float)(1 - BounceEaseOut(d - t, d));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BounceEaseOut(float t, float d)
{
    t /= d;
    if (t < 0.363)
    {
        return (float)(7.5625 * t * t);
    }
    else if (t < 0.727)
    {
        t -= (float)0.545;
        return (float)(7.5625 * t * t + 0.75);
    }
    else if (t < 0.909)
    {
        t -= (float)0.818;
        return (float)(7.5625 * t * t + 0.9375);
    }
    else
    {
        t -= (float)0.9545;
        return (float)(7.5625 * t * t + 0.984375);
    }

}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::BounceEaseInOut(float t, float d)
{
    if (t * 2 < d)
    {
        return BounceEaseIn(t * 2, d) / 2;
    }
    else
    {
        return (BounceEaseOut(t * 2 - d, d) + 1) / 2;
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CircEaseIn(float t, float d)
{
    t /= d;
    return (float)(1 - sqrt(1 - t * t));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CircEaseOut(float t, float d)
{
    t = (t / d) - 1;
    return (float)(sqrt(1 - t * t));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CircEaseInOut(float t, float d)
{
    t = (2 * t / d);
    if (t < 1)
    {
        return (float)((1 - sqrt(1 - t * t)) / 2);
    }
    else
    {
        t-=2;
        return (float)((1 + sqrt(1 - t * t)) / 2);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CubicEaseIn(float t, float d)
{
    t /= d;

    return (t * t * t);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CubicEaseOut(float t, float d)
{
    t = t / d - 1;
    return (t * t * t + 1);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::CubicEaseInOut(float t, float d)
{
    t = 2 * t / d;

    if (t < 1)
    {
        return (t * t * t / 2);
    }
    else
    {
        t -= 2;
        return (t * t * t / 2 + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ElasticEaseIn(float t, float d)
{
    if (t == 0)
    {
        return 0;
    }

    if (t == d)
    {
        return 1;
    }

    t = (t / d) - 1;

    return (float)(-pow(2, 10 * t) * sin(20.93 * t -1.57));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ElasticEaseOut(float t, float d)
{
    if (t == 0)
    {
        return 0;
    }

    if (t == d)
    {
        return 1;
    }

    t /= d;
    return (float)(pow(2, -10 * t) * sin(20.93 * t - 1.57) + 1);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ElasticEaseInOut(float t, float d)
{
    if (t == 0)
    {
        return 0;
    }

    t = (2 * t) / d;
    if (t == 2)
    {
        return 1;
    }

    if (t < 1)
    {
        t -= 1;
        return (float)(-pow(2, 10 * t) * sin(13.96*t - 1.57) / 2);
    }
    else
    {
        t -= 1;
        return (float)(pow(2, -10 * t) * sin(13.96*t - 1.57) / 2 + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ExpoEaseIn(float t, float d)
{
    t /= d;
    return (float)pow(2, 10 * (t - 1));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ExpoEaseOut(float t, float d)
{
    t /= d;
    return (float)(1 - pow(2, -10 * t));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::ExpoEaseInOut(float t, float d)
{
    t = 2 * t / d;
    if (t < 1)
    {
        return (float)pow(2, 10 * (t - 1)) / 2;
    }
    else
    {
        t--;
        return (float)(1 - pow(2, -10 * t) / 2);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuadEaseInt(float t, float d)
{
    t /= d;
    return t * t;
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuadEaseOut(float t, float d)
{
    t /= d;
    return t * (2 - t);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuadEaseInOut(float t, float d)
{
    t = (2 * t / d);
    if (t < 1)
    {
        return (t * t / 2);
    }
    else
    {
        t--;
        return ((1 - t* (t - 2)) / 2);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuartEaseIn(float t, float d)
{
    t /= d;
    return t * t * t * t;
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuartEaseOut(float t, float d)
{
    t = t / d - 1;
    return (1 - t*t*t*t);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuartEaseInOut(float t, float d)
{
    t = (2 * t / d);
    if (t < 1)
    {
        return t * t * t * t / 2;
    }
    else
    {
        t -= 2;
        return (1 - t * t * t * t / 2);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuintEaseIn(float t, float d)
{
    t /= d;
    return t * t * t * t * t;
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuintEaseOut(float t, float d)
{
    t = (t / d - 1);
    return (t * t * t * t * t + 1);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::QuintEaseInOut(float t, float d)
{
    t = (2 * t / d);
    if (t < 1)
    {
        return t * t * t * t *t / 2;
    }
    else
    {
        t -= 2;
        return (t * t* t* t* t / 2 + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::SinEaseIn(float t, float d)
{
    return (float)(1 - cos(t / d * 1.57));
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::SinEaseOut(float t, float d)
{
    return (float)sin(t / d * 1.57);
}

///////////////////////////////////////////////////////////////////////////////
float easing_function_select_dlg::SinEaseInOut(float t, float d)
{
    return (float)((1 - cos(t / d * 3.14)) / 2);
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::DrawEasingCurve(Graphics &g)
{
    SolidBrush red_brush(Color(255, 200, 0, 0));
    REAL x;
    REAL x1, x2;
    REAL y1, y2;
    REAL width = (REAL)mWaveRect.Width();
    REAL height = (REAL)mWaveRect.Height();
    
    Color color;
    color.SetFromCOLORREF(GetSysColor(COLOR_WINDOWTEXT));
    Pen line_pen(color);

    g.SetSmoothingMode(SmoothingModeAntiAlias);

    switch (mEasingFuncId)
    {
    case GX_ANIMATION_BACK_EASE_IN:
        mpEasingFuncCallback = BackEaseIn;
        break;
    case GX_ANIMATION_BACK_EASE_OUT:
        mpEasingFuncCallback = BackEaseOut;
        break;
    case GX_ANIMATION_BACK_EASE_IN_OUT:
        mpEasingFuncCallback = BackEaseInOut;
        break;
    case GX_ANIMATION_BOUNCE_EASE_IN:
        mpEasingFuncCallback = BounceEaseIn;
        break;
    case GX_ANIMATION_BOUNCE_EASE_OUT:
        mpEasingFuncCallback = BounceEaseOut;
        break;
    case GX_ANIMATION_BOUNCE_EASE_IN_OUT:
        mpEasingFuncCallback = BounceEaseInOut;
        break;
    case GX_ANIMATION_CIRC_EASE_IN:
        mpEasingFuncCallback = CircEaseIn;
        break;
    case GX_ANIMATION_CIRC_EASE_OUT:
        mpEasingFuncCallback = CircEaseOut;
        break;
    case GX_ANIMATION_CIRC_EASE_IN_OUT:
        mpEasingFuncCallback = CircEaseInOut;
        break;
    case GX_ANIMATION_CUBIC_EASE_IN:
        mpEasingFuncCallback = CubicEaseIn;
        break;
    case GX_ANIMATION_CUBIC_EASE_OUT:
        mpEasingFuncCallback = CubicEaseOut;
        break;
    case GX_ANIMATION_CUBIC_EASE_IN_OUT:
        mpEasingFuncCallback = CubicEaseInOut;
        break;
    case GX_ANIMATION_ELASTIC_EASE_IN:
        mpEasingFuncCallback = ElasticEaseIn;
        break;
    case GX_ANIMATION_ELASTIC_EASE_OUT:
        mpEasingFuncCallback = ElasticEaseOut;
        break;
    case GX_ANIMATION_ELASTIC_EASE_IN_OUT:
        mpEasingFuncCallback = ElasticEaseInOut;
        break;
    case GX_ANIMATION_EXPO_EASE_IN:
        mpEasingFuncCallback = ExpoEaseIn;
        break;
    case GX_ANIMATION_EXPO_EASE_OUT:
        mpEasingFuncCallback = ExpoEaseOut;
        break;
    case GX_ANIMATION_EXPO_EASE_IN_OUT:
        mpEasingFuncCallback = ExpoEaseInOut;
        break;
    case GX_ANIMATION_QUAD_EASE_IN:
        mpEasingFuncCallback = QuadEaseInt;
        break;
    case GX_ANIMATION_QUAD_EASE_OUT:
        mpEasingFuncCallback = QuadEaseOut;
        break;
    case GX_ANIMATION_QUAD_EASE_IN_OUT:
        mpEasingFuncCallback = QuadEaseInOut;
        break;
    case GX_ANIMATION_QUART_EASE_IN:
        mpEasingFuncCallback = QuartEaseIn;
        break;
    case GX_ANIMATION_QUART_EASE_OUT:
        mpEasingFuncCallback = QuartEaseOut;
        break;
    case GX_ANIMATION_QUART_EASE_IN_OUT:
        mpEasingFuncCallback = QuartEaseInOut;
        break;
    case GX_ANIMATION_QUINT_EASE_IN:
        mpEasingFuncCallback = QuintEaseIn;
        break;
    case GX_ANIMATION_QUINT_EASE_OUT:
        mpEasingFuncCallback = QuintEaseOut;
        break;
    case GX_ANIMATION_QUINT_EASE_IN_OUT:
        mpEasingFuncCallback = QuintEaseInOut;
        break;
    case GX_ANIMATION_SINE_EASE_IN:
        mpEasingFuncCallback = SinEaseIn;
        break;
    case GX_ANIMATION_SINE_EASE_OUT:
        mpEasingFuncCallback = SinEaseOut;
        break;
    case GX_ANIMATION_SINE_EASE_IN_OUT:
        mpEasingFuncCallback = SinEaseInOut;
        break;
    default:
        mpEasingFuncCallback = GX_NULL;
        break;
    }

    // draw easing curve
    if (mpEasingFuncCallback)
    {
        x1 = (REAL)mWaveRect.left;
        y1 = (REAL)mWaveRect.bottom;
        for (x = 1; x <= width; x += 1)
        {
            x2 = (REAL)mWaveRect.left + x;
            y2 = (REAL)mWaveRect.bottom - height * mpEasingFuncCallback(x, width);
            g.DrawLine(&line_pen, x1, y1, x2, y2);

            x1 = x2;
            y1 = y2;
        }

        x = mAnimationStep * width / ANIMATION_TOTAL_STEP;
        x2 = (REAL)mWaveRect.left + x;
        y2 = (REAL)mWaveRect.bottom - height * mpEasingFuncCallback(x, width);
    }
    else
    {
        g.DrawLine(&line_pen, mWaveRect.left, mWaveRect.bottom, mWaveRect.right, mWaveRect.top);
        x = mAnimationStep * width / ANIMATION_TOTAL_STEP;
        x2 = (REAL)mWaveRect.left + x;
        y2 = (REAL)mWaveRect.bottom - (REAL)(height * x / width);
    }
    // draw dot in curve
    g.FillEllipse(&red_brush, RectF(x2 - ((REAL)mAnimationDotRadius/2), y2 - ((REAL)mAnimationDotRadius/2),
        (REAL)mAnimationDotRadius, (REAL)mAnimationDotRadius));

    // draw animation rectangle
    g.FillRectangle(&red_brush, RectF((REAL)(mWaveRect.left - (mAnimationBoxSize.cx * 3 / 2)),
            (REAL)(y2 - (mAnimationBoxSize.cy/2)), (REAL)mAnimationBoxSize.cx, (REAL)mAnimationBoxSize.cy));
}

///////////////////////////////////////////////////////////////////////////////
void easing_function_select_dlg::OnSelecteEasingFuncCombo()
{
    int index = mEasingFuncCombo.GetCurSel();
    mEasingFuncId = mEasingFuncCombo.GetItemData(index);
    InvalidateRect(&mAnimationRect);
    mAnimationStatus.SetWindowTextW(L"");
    mAnimationStep = 0;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT easing_function_select_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case TEST_SELECT_EASING_FUNCTION:
        mEasingFuncCombo.SelectString(0, GetTestingParam(0));
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EASING_FUNC_COMBO, CBN_SELCHANGE), (LPARAM)mEasingFuncCombo.m_hWnd);
        break;

    case TEST_SAVE_EASING_FUNCTION_SELECT:
        OnOK();
        break;

    case TEST_CANCEL_EASING_FUNCTION_SELECT:
        OnCancel();
        break;
    }

    return 0;
}
#include "studiox_includes.h"
#include "splash_screen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern     CString studiox_version_string;

BEGIN_MESSAGE_MAP(splash_screen, CDialog)
	ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDOK, OnDoneClicked)
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////////
splash_screen::splash_screen(BOOL AutoClose)
{
    mAutoClose = AutoClose;
    memset(date_built, 0, DATE_STRING_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
BOOL splash_screen::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


void splash_screen::ScreenReaderMessage()
{
    CString HelpString = _T("Azure RTOS GUIX Studio. ");
    CString version;

    version = "Version: ";
    version += studiox_version_string;
    version += _T(". Build Date: ");
    CString date(__DATE__);
    CString month = date.Left(3);
    CString day = date.Mid(4, 2);
    CString year = date.Right(4);

    version  += month + ' ' + day + ' ' + year;
    HelpString += version;

    SetWindowText(HelpString);
}


///////////////////////////////////////////////////////////////////////////////
int splash_screen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CWnd::OnCreate(lpCreateStruct);
    CenterWindow();
    ScreenReaderMessage();
    
    if (mAutoClose)
    {
        SetTimer(1, 2000, NULL);
    }
    else
    {
        CRect button_rect;
        GetClientRect(button_rect);
        button_rect.bottom -= 10;
        button_rect.top = button_rect.bottom - 20;
        button_rect.left += 220;
        button_rect.right -= 100;
        CloseButton.Create(_T("Close"), BS_PUSHBUTTON|BS_CENTER|BS_VCENTER|WS_CHILD|WS_VISIBLE, button_rect, this, IDOK);
        CloseButton.SetFocus();
    }

    return 0;
}

extern CFont TitleFont;
extern CFont TinyFont;

CString MonthToNumber(CString Month)
{
    if (Month == "Jan")
    {
        return CString("01");
    }
    if (Month == "Feb")
    {
        return CString("02");
    }
    if (Month == "Mar")
    {
        return CString("03");
    }
    if (Month == "Apr")
    {
        return CString("04");
    }
    if (Month == "May")
    {
        return CString("05");
    }
    if (Month == "Jun")
    {
        return CString("06");
    }
    if (Month == "Jul")
    {
        return CString("07");
    }
    if (Month == "Aug")
    {
        return CString("08");
    }
    if (Month == "Sep")
    {
        return CString("09");
    }
    if (Month == "Oct")
    {
        return CString("10");
    }
    if (Month == "Nov")
    {
        return CString("11");
    }
    if (Month == "Dec")
    {
        return CString("12");
    }
    return CString("xx");
}

///////////////////////////////////////////////////////////////////////////////
void splash_screen::OnPaint()
{
    CWnd::OnPaint();
    CDC *dc = GetDC();
    PaintBmp(dc, 0, 0, IDB_SPLASH_BACKGROUND);
    CFont *OldFont;

    CString temp;
    CRect rc;
    CRect client;

    int xpos = 200;
    int ypos = 120; 
    int line_height = 16;

    dc->SetTextColor(RGB(248, 208, 40));
    dc->SetBkColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    OldFont = dc->SelectObject(&TitleFont);

    dc->TextOut(xpos, ypos, _T("Azure RTOS GUIX Studio"));
    ypos += 30;

    temp = "Version ";
    temp += studiox_version_string;
    dc->TextOut(xpos, ypos, temp);
    ypos += 30;
    dc->SelectObject(&TinyFont);

    GetClientRect(&client);
    rc.SetRect(xpos, ypos, client.right - 10, ypos + line_height);
    rc.OffsetRect(0, line_height);

    CString date(__DATE__);
    CString month = date.Left(3);
    CString day = date.Mid(4, 2);
    CString year = date.Right(4);

    temp = "Studio Build Date: ";
    temp += month + '-' + day + '-' + year;
    dc->DrawText(temp, rc, DT_LEFT|DT_SINGLELINE);
    rc.OffsetRect(0, line_height);

    dc->SelectObject(OldFont);
    ReleaseDC(dc);
}

void splash_screen::OnTimer(UINT_PTR nIdEvent)
{
    EndModalLoop(0);
}

void splash_screen::OnDoneClicked()
{
    EndModalLoop(0);
}
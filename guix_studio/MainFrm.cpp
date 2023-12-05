// MainFrm.cpp : implementation of the CMainFrame class
//

#include "studiox_includes.h"
#include "config_languages_dlg.h"
#include "splash_screen.h"
#include "clone_repo_dialog.h"
#include "MainFrm.h"
#include "app_runner.h"
#include "gx_win32_studio_display_driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern INI_INFO StudioXIni;
extern CString target_class_name;
extern CBrush WinBackBrush;

#define MACRO_FILE_FLAG   _T("<macro head>")
#define MACRO_RECORD_SPEED  1
#define ID_SINGLEMACRO 1
#define ID_SUPERMACRO  2

int          WM_SET_LAYOUT;
HHOOK        mouse_hook = 0;
HHOOK        keybd_hook = 0;
CWinThread   *macro_play_thread = NULL;
CFile        *recorder;
ULONG        start_time;
WPARAM       premsg = 0;
POINT        prept = { 0, 0 };
CString      macro_name;
int          macro_filetype;
int          pre_key_state = -1;//record previous key state, pressed or released
int          pre_vkey = -1;//record previous virtual key value
CArray<int>  proj_tree_state;

enum main_frame_test_commands{
    TEST_ZOOM_IN = 1,
    TEST_TOOLBAR_CREATE,
    TEST_TOOLBAR_OPEN,
    TEST_TOOLBAR_SAVE,
    TEST_TOOLBAR_CUT,
    TEST_TOOLBAR_COPY,
    TEST_TOOLBAR_PASTE,
    TEST_TOOLBAR_ALIGN_LEFT,
    TEST_TOOLBAR_ALIGN_RIGHT,
    TEST_TOOLBAR_ALIGN_TOP,
    TEST_TOOLBAR_ALIGN_BOTTOM,
    TEST_TOOLBAR_VSPACE_EQUALLY,
    TEST_TOOLBAR_HSPACE_EQUALLY,
    TEST_TOOLBAR_EQUAL_WIDTH,
    TEST_TOOLBAR_EQUAL_HEIGHT,
    TEST_TOOLBAR_MOVE_TO_FRONT,
    TEST_TOOLBAR_MOVE_TO_BACK,
    TEST_TOOLBAR_SIZE_TO_FIT,
    TEST_TOOLBAR_ZOOM_IN,
    TEST_TOOLBAR_ZOOM_OUT,
    TEST_TOOLBAR_MACRO_RECORD,
    TEST_TOOLBAR_MACRO_PLAYBACK,
    TEST_TOOLBAR_ABOUT,
    TEST_UNDO,
    TEST_CONFIGURE_LANGUAGES,
    TEST_CONFIGURE_DISPLAYS,
    TEST_CONFIGURE_THEMES,
    TEST_CONFIGURE_SCREEN_FLOW,
    TEST_GENERATE_ALL,
    TEST_GENERATE_RESOURCES,
    TEST_GET_MENU_STATUS,
    TEST_GRID_SNAP_SETTING,
    TEST_FIND_DIALOG,
    TEST_LOCK_UNLOCK_WIDGET_POSITIONS,
    TEST_RUN_APPLICATION,
    TEST_IS_WAIT_DIALOG_RUNNING
};

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOVING()
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
    ON_COMMAND(ID_ADD_BUTTON, &CMainFrame::OnAddButton)
    ON_COMMAND(ID_ADD_PROMPT, &CMainFrame::OnAddPrompt)
    ON_COMMAND(ID_ADD_NUMERIC_PROMPT, &CMainFrame::OnAddNumericPrompt)
    ON_COMMAND(ID_ADD_ICON, &CMainFrame::OnAddIcon)
    ON_COMMAND(ID_ADD_TEXTBUTTON, &CMainFrame::OnAddTextButton)
    ON_COMMAND(ID_ADD_MLT_BUTTON, &CMainFrame::OnAddMltButton)
    ON_COMMAND(ID_ADD_CHECKBOX, &CMainFrame::OnAddCheckbox)
    ON_COMMAND(ID_ADD_RADIO_BUTTON, &CMainFrame::OnAddRadioButton)
    ON_COMMAND(ID_ADD_ICON_BUTTON, &CMainFrame::OnAddIconButton)
    ON_COMMAND(ID_ADD_PIXMAP_BUTTON, &CMainFrame::OnAddPixelmapButton)
    ON_COMMAND(ID_ADD_PIXMAP_PROMPT, &CMainFrame::OnAddPixelmapPrompt)
    ON_COMMAND(ID_ADD_NUMERIC_PIXELMAP_PROMPT, &CMainFrame::OnAddNumericPixelmapPrompt)
    ON_COMMAND(ID_ADD_SLIDER, &CMainFrame::OnAddSlider)
    ON_COMMAND(ID_ADD_PIXELMAP_SLIDER, &CMainFrame::OnAddPixelmapSlider)
    ON_COMMAND(ID_ADD_RADIAL_SLIDER, &CMainFrame::OnAddRadialSlider)
    ON_COMMAND(ID_ADD_PROGRESS_BAR, &CMainFrame::OnAddProgressBar)
    ON_COMMAND(ID_ADD_RADIAL_PROGRES_BAR, &CMainFrame::OnAddRadialProgressBar)
    ON_COMMAND(ID_ADD_SPRITE, &CMainFrame::OnAddSprite)

    ON_COMMAND(ID_ADD_HSCROLL, &CMainFrame::OnAddHScroll)
    ON_COMMAND(ID_ADD_VSCROLL, &CMainFrame::OnAddVScroll)

    ON_COMMAND(ID_ADD_STRING_SCROLL_WHEEL, &CMainFrame::OnAddStringScrollWheel)
    ON_COMMAND(ID_ADD_NUMERIC_SCROLL_WHEEL, &CMainFrame::OnAddNumericScrollWheel)
    ON_COMMAND(ID_ADD_GENERIC_SCROLL_WHEEL, &CMainFrame::OnAddGenericScrollWheel)

    ON_COMMAND(ID_ADD_WINDOW, &CMainFrame::OnAddWindow)
    ON_COMMAND(ID_ADD_VERTICAL_LIST, &CMainFrame::OnAddVerticalList)
    ON_COMMAND(ID_ADD_HORIZONTAL_LIST, &CMainFrame::OnAddHorizontalList)
    ON_COMMAND(ID_ADD_DROP_DOWN_LIST, &CMainFrame::OnAddDropDownList)
    ON_COMMAND(ID_INSERT_LINECHART, &CMainFrame::OnAddLineChart)

    ON_COMMAND(ID_ADD_TEXT_INPUT, &CMainFrame::OnAddTextInput)
    ON_COMMAND(ID_ADD_MULTI_LINE_VIEW, &CMainFrame::OnAddMLTextView)
    ON_COMMAND(ID_ADD_MULTI_LINE_INPUT, &CMainFrame::OnAddMLTextInput)
    ON_COMMAND(ID_ADD_RICH_TEXT_VIEW, &CMainFrame::OnAddRichTextView)

    ON_COMMAND(ID_ADD_CIRCULAR_GAUGE, &CMainFrame::OnAddCircularGauge)
    ON_COMMAND_RANGE(ID_ADD_TEMPLATE_BASE, ID_ADD_TEMPLATE_BASE + MAX_TEMPLATES, &CMainFrame::OnAddTemplate)

    ON_COMMAND(ID_ADD_MENU, &CMainFrame::OnAddMenu)
    ON_COMMAND(ID_ADD_ACCORDION_MENU, &CMainFrame::OnAddAccordionMenu)
    ON_COMMAND(ID_ADD_TREE_VIEW, &CMainFrame::OnAddTreeView)

    ON_COMMAND(ID_WIDGET_DELETE, &CMainFrame::OnDeleteWidget)
    ON_COMMAND(ID_WIDGET_CUT, &CMainFrame::OnCutWidget)
    ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnCutWidget)
    ON_COMMAND(ID_WIDGET_COPY, &CMainFrame::OnCopyWidget)
    ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnCopyWidget)
    ON_COMMAND(ID_WIDGET_PASTE, &CMainFrame::OnPasteWidget)
    ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnPasteWidget)
    ON_COMMAND(ID_INSERT_FOLDER, &CMainFrame::OnInsertFolder)

    ON_COMMAND(ID_ALIGN_LEFT, &CMainFrame::OnAlignLeft)
    ON_COMMAND(ID_ALIGN_TOP, &CMainFrame::OnAlignTop)
    ON_COMMAND(ID_ALIGN_RIGHT, &CMainFrame::OnAlignRight)
    ON_COMMAND(ID_ALIGN_BOTTOM, &CMainFrame::OnAlignBottom)
    ON_COMMAND(ID_HSPACE_EQUALLY, &CMainFrame::OnHSpaceEqually)
    ON_COMMAND(ID_VSPACE_EQUALLY, &CMainFrame::OnVSpaceEqually)
    ON_COMMAND(ID_EQUAL_WIDTH, &CMainFrame::OnEqualWidth)
    ON_COMMAND(ID_EQUAL_HEIGHT, &CMainFrame::OnEqualHeight)
    ON_COMMAND(ID_MOVE_TO_FRONT, &CMainFrame::OnMoveToFront)
    ON_COMMAND(ID_MOVE_TO_BACK, &CMainFrame::OnMoveToBack)
    ON_COMMAND(ID_SIZE_TO_FIT, &CMainFrame::OnSizeToFit)
    ON_COMMAND(ID_ZOOM_IN, &CMainFrame::OnZoom)
    ON_COMMAND(ID_ZOOM_OUT, &CMainFrame::OnZoom)
    ON_COMMAND(ID_ZOOM_100, &CMainFrame::OnZoom)
    ON_COMMAND(ID_ZOOM_200, &CMainFrame::OnZoom)
    ON_COMMAND(ID_ZOOM_300, &CMainFrame::OnZoom)
    ON_COMMAND(ID_ZOOM_400, &CMainFrame::OnZoom)
    ON_COMMAND(ID_TARGETVIEW_GRID_SNAP, &CMainFrame::OnGridSetting)

    ON_COMMAND(ID_CONFIGURE_DISPLAYS, &CMainFrame::OnConfigureDisplays)
    ON_COMMAND(ID_CONFIGURE_LANGUAGES, &CMainFrame::OnConfigureLanguages)
    ON_COMMAND(ID_CONFIGURE_THEMES, &CMainFrame::OnConfigureThemes)
    ON_COMMAND(ID_CONFIGURE_SCREENFLOW, &CMainFrame::OnConfigureScreenFlow)
    ON_COMMAND(ID_CONFIGURE_GUIX_REPOSITORY, &CMainFrame::OnConfigureGuixRepository)

    ON_COMMAND_RANGE(ID_FILE_MRU_FILE2, ID_FILE_MRU_FILE6, &CMainFrame::OnOpenHistoryProject)
    ON_COMMAND(ID_GENERATE_ALL, &CMainFrame::OnGenerateAll)
    ON_COMMAND(ID_GENERATE_RESOURCES, &CMainFrame::OnGenResources)
    ON_COMMAND(ID_GENERATE_APPLICATION, &CMainFrame::OnGenApplication)
    ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnUndo)

    ON_COMMAND(ID_RECORDMACRO, &CMainFrame::OnRecordmacro)
    ON_COMMAND(ID_PLAYBACKMACRO, &CMainFrame::OnPlaybackmacro)
    ON_COMMAND(ID_ENDMACRO, &CMainFrame::OnEndmacro)
    ON_COMMAND(ID_STOPMACRO, &CMainFrame::OnStopMacro)

    ON_COMMAND(ID_RUN_APPLICATION, &CMainFrame::OnRunApplication)
    ON_COMMAND(ID_RUN_STRING_FIT_CHECK, &CMainFrame::OnRunStringFitCheck)
    ON_COMMAND(ID_LOCK_WIDGET_POSITIONS, &CMainFrame::OnLockUlockWidgetPositions)

    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_REGISTERED_MESSAGE(WM_SET_LAYOUT, &CMainFrame::OnSetLayout)
    
    ON_WM_INITMENUPOPUP()
        ON_WM_SETTINGCHANGE()
        END_MESSAGE_MAP()

#define ID_COMPANY 1000
#define ID_SEATS   1001
static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL
};

BEGIN_MESSAGE_MAP(CustomToolBar, CToolBar)
    //ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    //ON_WM_PAINT()
END_MESSAGE_MAP()

CustomToolBar::CustomToolBar()
{
}


///////////////////////////////////////////////////////////////////////////////
BOOL CustomToolBar::OnEraseBkgnd(CDC* pDC)
{
    CRect boxrect;
    CDC dcMemory;
    CBitmap fillmap;
    fillmap.LoadBitmap(IDB_TOOLBAR_FILL);
    BITMAP bm;

    // go through hoops to get bitmap width:
    fillmap.GetObject(sizeof(BITMAP), &bm);
    int width = bm.bmWidth;
    int height = bm.bmHeight;

    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject(&fillmap);

    pDC->GetClipBox(&boxrect);     // Erase the area needed
    int xpos = boxrect.left;

    while(xpos < boxrect.right)
    {
        pDC->BitBlt(xpos, boxrect.top, width, height, &dcMemory, 0, 0, SRCCOPY);
        xpos += width;
    }
    return TRUE;
}
#if 0
void CustomToolBar::OnPaint()
{
    //CDC *dc = GetDC();
    //CWnd::OnPaint();

//    PaintHeader(dc, size);
    //ReleaseDC(dc);
   PAINTSTRUCT ps;
   CDC* pDC = BeginPaint(&ps);

   //pDC->Rectangle(CRect(0, 0, 100, 100));
    CToolBar::OnPaint();
   EndPaint(&ps);
}
#endif

#if 1

studiox_splitter::studiox_splitter()
{
    m_proj_width = m_res_width = 200;
}

void studiox_splitter::FixColumnWidths()
{
    CRect rect;
    GetWindowRect(rect);
    int target_width = 1;
    int total_width = rect.right - rect.left - 18;
//    int width_sum;

    //if (m_pColInfo[0].nIdealSize == 0)
    //{
    //    m_pColInfo[0].nIdealSize = 200;
    //    m_pColInfo[1].nIdealSize = total_width - 400;
    //}

    if (m_pColInfo[0].nIdealSize < 200)
    {
        m_pColInfo[0].nIdealSize = 200;
    }

    if(m_pColInfo[1].nIdealSize == 0)
    {
        m_pColInfo[1].nIdealSize = total_width - (m_pColInfo[0].nIdealSize + m_pColInfo[2].nIdealSize);
    }
 
    m_res_width = total_width - (m_pColInfo[0].nIdealSize + m_pColInfo[1].nIdealSize);

    if (m_res_width < 200)
    {
        m_res_width = 200;
        target_width = total_width - m_pColInfo[0].nIdealSize - m_res_width;

        if (target_width <= 0)
        {
            target_width = 1;
        }
        m_pColInfo[1].nIdealSize = target_width;
    }
    m_pColInfo[2].nIdealSize = m_res_width;
}

void studiox_splitter::RecalcLayout()
{
    FixColumnWidths();
    CSplitterWnd::RecalcLayout();
    //m_proj_width = m_pColInfo[0].nCurSize;
    //m_res_width = m_pColInfo[2].nCurSize;
}

#endif

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
    m_splitter_created = FALSE;
    mpTargetView = NULL;
    mpResourceView = NULL;
    mpProjectView = NULL;
    mpCmdInfo = new CCommandInfo();
    m_bAutoMenuEnable = FALSE;

    widget_factory::InitServiceProviders();
    mpUndoManager = new undo_manager();

    target_class_name = AfxRegisterWndClass(
      CS_VREDRAW | CS_HREDRAW,
      ::LoadCursor(NULL, IDC_ARROW),
      (HBRUSH) WinBackBrush,
      ::LoadIcon(NULL, IDI_APPLICATION));

    mpBaseMenu = new CMenu;
    mpBaseMenu->CreateMenu();

    mpRecentMenu = new CMenu;
    mpRecentMenu->CreateMenu();

    m_text_scaler = GetTextScaler();

    // Create accessibility property service
    CreateAccPropService();
}

CMainFrame::~CMainFrame()
{
    widget_factory::DeleteServiceProviders();
    delete mpUndoManager;
    delete mpCmdInfo;
    delete mpBaseMenu;
    delete mpRecentMenu;

    // Release accessibility property service
    ReleaseAccPropService();
}

void CMainFrame::OnAddButton()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_BUTTON);
}
void CMainFrame::OnAddTextButton()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_TEXT_BUTTON);
}
void CMainFrame::OnAddMltButton()
{
    if (project_lib_version() >= 50201)
    {
        mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_MULTI_LINE_TEXT_BUTTON);
    }
    else
    {
        ErrorMsg("This widget is not supported by your GUIX library release");
    }
}

void CMainFrame::OnAddCheckbox()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_CHECKBOX);
}
void CMainFrame::OnAddRadioButton()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_RADIO_BUTTON);
}

void CMainFrame::OnAddIconButton()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_ICON_BUTTON);
}

void CMainFrame::OnAddPixelmapButton()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_PIXELMAP_BUTTON);
}


void CMainFrame::OnAddPrompt()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_PROMPT);
}


void CMainFrame::OnAddNumericPrompt()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_NUMERIC_PROMPT);
}

void CMainFrame::OnAddPixelmapPrompt()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_PIXELMAP_PROMPT);
}

void CMainFrame::OnAddNumericPixelmapPrompt()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_NUMERIC_PIXELMAP_PROMPT);
}

void CMainFrame::OnAddIcon()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_ICON);
}


void CMainFrame::OnAddSlider()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_SLIDER);
}
void CMainFrame::OnAddPixelmapSlider()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_PIXELMAP_SLIDER);
}

void CMainFrame::OnAddRadialSlider()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_RADIAL_SLIDER);
}

void CMainFrame::OnAddProgressBar()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_PROGRESS_BAR);
}

void CMainFrame::OnAddRadialProgressBar()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_RADIAL_PROGRESS_BAR);
}

void CMainFrame::OnAddSprite()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_SPRITE);
}

void CMainFrame::OnAddHScroll()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_HORIZONTAL_SCROLL);
}

void CMainFrame::OnAddVScroll()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_VERTICAL_SCROLL);
}


void CMainFrame::OnAddStringScrollWheel()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_STRING_SCROLL_WHEEL);
}

void CMainFrame::OnAddNumericScrollWheel()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_NUMERIC_SCROLL_WHEEL);
}

void CMainFrame::OnAddGenericScrollWheel()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_GENERIC_SCROLL_WHEEL);
}


void CMainFrame::OnAddWindow()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_WINDOW);
}
void CMainFrame::OnAddVerticalList()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_VERTICAL_LIST);
}
void CMainFrame::OnAddHorizontalList()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_HORIZONTAL_LIST);
}
void CMainFrame::OnAddDropDownList()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_DROP_LIST);
}
void CMainFrame::OnAddLineChart()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_LINE_CHART);
}

void CMainFrame::OnAddTextInput()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_SINGLE_LINE_TEXT_INPUT);
}
void CMainFrame::OnAddMLTextView()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_MULTI_LINE_TEXT_VIEW);
}
void CMainFrame::OnAddMLTextInput()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_MULTI_LINE_TEXT_INPUT);
}

void CMainFrame::OnAddRichTextView()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_RICH_TEXT_VIEW);
}

void CMainFrame::OnAddCircularGauge()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_CIRCULAR_GAUGE);
}

void CMainFrame::OnAddTemplate(UINT nID)
{
    widget_info *info = GetProjectView()->GetTemplateInfo(nID - ID_ADD_TEMPLATE_BASE);

    if (info)
    {
        mpTargetView->GetTargetScreen()->OnAddTemplate(info);
    }
}

void CMainFrame::OnAddMenu()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_MENU);
}

void CMainFrame::OnAddAccordionMenu()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_ACCORDION_MENU);
}

void CMainFrame::OnAddTreeView()
{
    mpTargetView->GetTargetScreen()->OnAddWidget(GX_TYPE_TREE_VIEW);
}

void CMainFrame::OnCutWidget()
{
    GetProjectView()->OnCutWidget();
}
void CMainFrame::OnCopyWidget()
{
    GetProjectView()->OnCopyWidget();
}
void CMainFrame::OnPasteWidget()
{
    GetProjectView()->OnPasteWidget();
}
void CMainFrame::OnDeleteWidget()
{
    GetProjectView()->OnDeleteWidget();
}
void CMainFrame::OnInsertFolder()
{
    GetProjectView()->OnInsertFolder();
}

void CMainFrame::OnAlignLeft()
{
    mpTargetView->GetTargetScreen()->OnAlignLeft();
}

void CMainFrame::OnAlignTop()
{
    mpTargetView->GetTargetScreen()->OnAlignTop();
}

void CMainFrame::OnAlignRight()
{
    mpTargetView->GetTargetScreen()->OnAlignRight();
}

void CMainFrame::OnAlignBottom()
{
    mpTargetView->GetTargetScreen()->OnAlignBottom();

}

void CMainFrame::OnHSpaceEqually()
{
    mpTargetView->GetTargetScreen()->OnHSpaceEqually();
}

void CMainFrame::OnVSpaceEqually()
{
    mpTargetView->GetTargetScreen()->OnVSpaceEqually();
}

void CMainFrame::OnEqualWidth()
{
    mpTargetView->GetTargetScreen()->OnMakeEqualWidth();
}

void CMainFrame::OnEqualHeight()
{
    mpTargetView->GetTargetScreen()->OnMakeEqualHeight();
}

void CMainFrame::OnMoveToFront()
{
    GetProjectView()->MoveSelectedToFront();
}

void CMainFrame::OnMoveToBack()
{
    GetProjectView()->MoveSelectedToBack();
}

void CMainFrame::OnSizeToFit()
{
    mpTargetView->GetTargetScreen()->OnAutosize();
}

void CMainFrame::OnZoom()
{
    int scale = 100;

    UINT nID = (UINT)LOWORD(GetCurrentMessage()->wParam);

    switch (nID)
    {
    case ID_ZOOM_IN:
        scale = mpTargetView->GetTargetScreen()->GetZoomScale();
        scale += 100;

        if (scale > 400)
        {
            return;
        }
        break;
    case ID_ZOOM_OUT:
        scale = mpTargetView->GetTargetScreen()->GetZoomScale();
        scale -= 100;

        if (scale <= 0)
        {
            return;
        }
        break;
    case ID_ZOOM_100:
        scale = 100;
        break;
    case ID_ZOOM_200:
        scale = 200;
        break;
    case ID_ZOOM_300:
        scale = 300;
        break;
    case ID_ZOOM_400:
        scale = 400;
        break;
    }

    mpTargetView->GetTargetScreen()->ZoomIn(scale);
}

void CMainFrame::OnGridSetting()
{
    grid_setting_dlg dlg(this);

    dlg.DoModal();
}

void CMainFrame::OnConfigureDisplays()
{
    GetProjectView()->OnConfigureDisplays();
}

void CMainFrame::OnConfigureLanguages()
{
    if (GetOpenProject())
    {
        config_languages_dlg dlg;
        dlg.DoModal();
    }
}

void CMainFrame::OnConfigureThemes()
{
    if (GetOpenProject())
    {
        GetProjectView()->OnConfigureThemes();
    }
}

void CMainFrame::OnConfigureGuixRepository()
{
    clone_repo_dialog* dlg = new clone_repo_dialog(this);
    dlg->DoModal();
    delete(dlg);

    if (GetOpenProject() == NULL)
    {
        GetTargetView()->DisplayRecentProjects();
    }
}

void CMainFrame::OnUndo()
{
    if (mpUndoManager->CountEntries() > 0)
    {
        mpUndoManager->Undo();
    }
    else
    {
        ErrorMsg("Nothing to Undo");
    }
}

void CMainFrame::OnOpenHistoryProject(UINT nID)
{
    if (GetProjectView())
    {
        GetProjectView()->OpenHistoryProject(nID - ID_FILE_MRU_FILE2);
    }
}

void CMainFrame::OnGenerateAll()
{
    resource_export_dlg dlg(AfxGetMainWnd());
    studiox_project *project = GetOpenProject();

    if (project != GX_NULL)
    {
        if (dlg.DoModal() == IDOK)
        {
            GetProjectView()->OnGenerateAll();
        }
    }

}

void CMainFrame::OnGenResources()
{
    resource_export_dlg dlg(AfxGetMainWnd());
    studiox_project *project = GetOpenProject();

    if (project != GX_NULL)
    {
        if (dlg.DoModal() == IDOK)
        {
            if (project->mHeader.gen_binary)
            {
                GetProjectView()->OnGenBinary();
            }
            else
            {
                GetProjectView()->OnGenResources();
            }
        }
    }
}

void CMainFrame::OnGenApplication()
{
    if (GetOpenProject())
    {
        GetProjectView()->OnGenApplication();
    }
}

void CMainFrame::OnConfigureScreenFlow()
{
    //transition_dialog dlg(GetOpenProject()->mDisplays[0].first_widget, this);

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    if (project_lib_version() < 50400)
    {
        Notify("Not supported. Please update GUIX Library to at least 5.4.0.");
        return;
    }

    //dlg.DoModal();
    if (GetProjectView())
    {

        screen_flow_edit_dlg dlg(GetProjectView()->GetActiveDisplay(), this);
        if (dlg.DoModal() == IDOK)
        {
            project->SetModified();
        }
    }
}

void CMainFrame::OnRunApplication()
{
    project_view *project_view = GetProjectView();

    if (project_view)
    {
        project_view->RunApplication();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CMainFrame::TestOneWidgetStringsFit(int display_index, widget_info* info)
{
    project_view* pview = GetProjectView();

    if (!pview)
    {
        return;
    }

    studiox_project *project = GetOpenProject();

    GX_RECTANGLE newsize = widget_service_provider::CalculateTextFitSize(info, display_index);

    if (newsize.gx_rectangle_right > info->size.gx_rectangle_right ||
        newsize.gx_rectangle_bottom > info->size.gx_rectangle_bottom)
    {
        if (m_strings_fit_msg.IsEmpty())
        {
            m_strings_fit_msg.Append(_T("String(s) in following widgets do not fit within the widget:\n"));
        }

        m_strings_fit_msg.Append(_T("    "));

        if (project->mHeader.num_displays > 1)
        {
            m_strings_fit_msg.Append(project->mDisplays[display_index].name);
            m_strings_fit_msg.Append(_T("."));
        }

        widget_info* screen = pview->FindTopLevelWidget(info);
        if (screen && (screen != info))
        {
            m_strings_fit_msg.Append(screen->app_name);
            m_strings_fit_msg.Append(_T("."));
        }
        m_strings_fit_msg.Append(info->app_name);
        m_strings_fit_msg.Append(_T("\n"));
    }
}

///////////////////////////////////////////////////////////////////////////////
void CMainFrame::TestStringsFit(int display_index, widget_info* info)
{
    widget_info* child;

    while (info)
    {
        switch (info->basetype)
        {
        case GX_TYPE_TEXT_BUTTON:
        case GX_TYPE_RADIO_BUTTON:
        case GX_TYPE_CHECKBOX:
        case GX_TYPE_MULTI_LINE_TEXT_BUTTON:
        case GX_TYPE_PROMPT:
        case GX_TYPE_NUMERIC_PROMPT:
        case GX_TYPE_PIXELMAP_PROMPT:
        case GX_TYPE_NUMERIC_PIXELMAP_PROMPT:
        case GX_TYPE_STRING_SCROLL_WHEEL:
        case GX_TYPE_MENU:
            TestOneWidgetStringsFit(display_index, info);
            break;
        }

        child = info->GetChildWidgetInfo();

        if (child)
        {
            TestStringsFit(display_index, child);
        }
        info = info->GetNextWidgetInfo();
    }
}

///////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRunStringFitCheck()
{
    studiox_project *project = GetOpenProject();
    m_strings_fit_msg.Empty();

    display_info *display;
    folder_info *folder;
    widget_info *info;

    for (int index = 0; index < project->mHeader.num_displays; index++)
    {
        display = &project->mDisplays[index];
        folder = NULL;
        info = NULL;

        if (display)
        {
            folder = display->GetFirstChildFolder();
        }

        while (folder)
        {
            info = folder->GetFirstChildWidget();

            if (info)
            {
                TestStringsFit(index, info);
            }

            folder = folder->GetNextFolder();
        }
    }

    if (m_strings_fit_msg.IsEmpty())
    {
        m_strings_fit_msg = "String fit check passed!";
    }

    Notify(CT2A(m_strings_fit_msg.GetBuffer()));
}

void CMainFrame::OnLockUlockWidgetPositions()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        if (project->IsWidgetPositionLocked())
        {
            project->LockUlockWidgetPositions(FALSE);
        }
        else
        {
            project->LockUlockWidgetPositions(TRUE);
        }
    }
}

void CMainFrame::OnClose()
{
    CRect rect;

    CCommandInfo *pCmdInfo = GetCmdInfo();

    if(pCmdInfo->IsNoGui())
    {
        if (GetProjectView())
        {
            GetProjectView()->CloseProject(FALSE, FALSE);
        }
    }
    else
    {
        GetWindowRect(rect);
        StudioXIni.xpos = rect.left;
        StudioXIni.ypos = rect.top;

        int cxMin;

        m_splitter.GetColumnInfo(0, StudioXIni.proj_view_width, cxMin);
        m_splitter.GetColumnInfo(2, StudioXIni.resource_view_width, cxMin);
        m_splitter2.GetRowInfo(0, StudioXIni.proj_view_height, cxMin);

        WriteIniInfo();

        if (GetProjectView())
        {
            GetProjectView()->CloseProject(FALSE);
        }

        if (recorder)
        {
            OnEndmacro();
        }
    }

    CFrameWnd::OnClose();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext)
{
    m_splitter.CreateStatic(this, 1, 3);
    m_splitter.CreateView(0, 1, RUNTIME_CLASS(target_view), CSize(0, 0), pContext);
    m_splitter.CreateView(0, 2, RUNTIME_CLASS(right_panel_frame), CSize(200, 200), pContext);

    m_splitter.SetColumnInfo(0, StudioXIni.proj_view_width, 0);
    m_splitter.SetColumnInfo(1, 0, 0);
    m_splitter.SetColumnInfo(2, StudioXIni.resource_view_width, 0);

    // add the second splitter pane which is nested in the first splitter
    if (!m_splitter2.CreateStatic(
        &m_splitter, // parent window is the first splitter
        2, 1, //the new splitter is 2 rows, 1 column
        WS_CHILD | WS_VISIBLE | WS_BORDER, // style
        m_splitter.IdFromRowCol(0, 0) // new splitter is in the first row, first column of first splitter
        ))
    {
        TRACE0("Failed to created nested splitter\n");
        return FALSE;
    }

    if (!m_splitter2.CreateView(0, 0, RUNTIME_CLASS(left_top_panel_frame), CSize(0, 0), pContext))
    {
        TRACE0("Failed to create project view pane\n");
        return FALSE;
    }

    if (!m_splitter2.CreateView(1, 0, RUNTIME_CLASS(left_bottom_panel_frame), CSize(0, 0), pContext))
    {
        TRACE0("Failed to create properties view pane\n");
        return FALSE;
    }

    m_splitter2.SetRowInfo(0, StudioXIni.proj_view_height, 0);

    m_splitter_created = TRUE;
    return TRUE;
}

void CMainFrame::OnMoving(UINT nSide, LPRECT lpRect)
{
    CFrameWnd::OnMoving(nSide, lpRect);
    if (mpTargetView)
    {
        CDialog *dlg = mpTargetView->GetRecentDialog();
        if (dlg)
        {
            dlg->CenterWindow();
        }
    }
}

void CMainFrame::OnSize(UINT ntype, int cx, int cy)
{
    CFrameWnd::OnSize(ntype, cx, cy);

    CRect rect;
    GetWindowRect(rect);
        
    StudioXIni.width = rect.Width();
    StudioXIni.height = rect.Height();

    if (mpTargetView)
    {
        CDialog *dlg = mpTargetView->GetRecentDialog();
        if (dlg)
        {
            dlg->CenterWindow();
        }
    }
    if (m_splitter_created)
    {
        m_splitter.FixColumnWidths();
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_1BIT))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    SetControlAccessibleName(m_wndToolBar.GetSafeHwnd(), _T("mainframe_toolbar"));

    // TODO: Delete these three lines if you don't want the toolbar to be dockable
    HBITMAP hBitmap = (HBITMAP) ::LoadImage(AfxGetInstanceHandle(),
    MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_BITMAP,
    0,0, LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

    CBitmap bm;
    bm.Attach(hBitmap);

    m_imagelist.Create(30, 22, ILC_COLOR32, 4, 4);
    m_imagelist.Add(&bm, (CBitmap*) NULL);

    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imagelist);
    m_imagelist.Detach();
    bm.Detach();
 
    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    SetIcon(hIcon, TRUE);

    hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SMALL_ICON));
    SetIcon(hIcon, FALSE);

    // Load insert popup menu
    mInsertMenu.LoadMenu(IDR_INSERT_MENU);

    // Attach insert popup menu to mainframe menu
    CString string;
    int index;
    for (index = 0; index < GetMenu()->GetMenuItemCount(); index++)
    {
        if (GetMenu()->GetMenuItemID(index) == ID_INSERT)
        {
            GetMenu()->GetMenuString(ID_INSERT, string, 0);
            GetMenu()->RemoveMenu(index, MF_BYPOSITION);

            GetMenu()->InsertMenu(index, MF_POPUP | MF_BYPOSITION | MF_DISABLED, (UINT)mInsertMenu.GetSubMenu(0)->m_hMenu, string);
            break;
        }
    }

    index = GetSubMenuIndex(GetMenu(), _T("&Project"));
    CMenu *mpProjectMenu = GetMenu()->GetSubMenu(index);

    if (mpProjectMenu)
    {

        // Attach recent projects popup menu to mainframe menu
        for (index = 0; index < mpProjectMenu->GetMenuItemCount(); index++)
        {
            if (mpProjectMenu->GetMenuItemID(index) == ID_RECENT_PROJECTS)
            {
                mpProjectMenu->GetMenuString(ID_RECENT_PROJECTS, string, 0);
                mpProjectMenu->RemoveMenu(index, MF_BYPOSITION);

                mpProjectMenu->InsertMenu(index, MF_POPUP | MF_BYPOSITION, (UINT)mpRecentMenu->m_hMenu, string);
                break;
            }
        }

        UpdateRecentProjectsMenu();
    }

    mStatusMsg.Create(L"Saved", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
    SetLiveRegion(mStatusMsg.GetSafeHwnd());
    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.cx = StudioXIni.width;
    cs.cy = StudioXIni.height;
    cs.x = StudioXIni.xpos;
    cs.y = StudioXIni.ypos;

    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return TRUE;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CWnd::OnShowWindow(bShow, nStatus);
}


BOOL CMainFrame::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CFrameWnd::OnSettingChange(uFlags, lpszSection);

    int new_text_scaler = GetTextScaler();

    if (new_text_scaler != m_text_scaler)
    {
        theApp.DeleteSystemFonts();
        theApp.CreateSystemFonts();
        m_text_scaler = new_text_scaler;
    }
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers





void CMainFrame::HtmlHelp(DWORD_PTR dwData, UINT nCmd)
{
    // TODO: Add your specialized code here and/or call the base class

    CFrameWnd::HtmlHelp(dwData, nCmd);
}

LRESULT CALLBACK MouseHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
    {
        CallNextHookEx(NULL, nCode, wParam, lParam);
        return 0;
    }

    MOUSEHOOKSTRUCT *mhs = (MOUSEHOOKSTRUCT *)lParam;

    /* Retrive pointer to main window class. */

    CMainFrame *m_hwnd = (CMainFrame *)AfxGetApp()->m_pMainWnd;
    POINT pt = mhs->pt;

    CToolBar *m_wndToolbar = m_hwnd->GetToolBar();
    UINT nIndex = m_wndToolbar->CommandToIndex(ID_ENDMACRO);
    CRect toolbar_rect, macro_end_rect, macro_playback_rect;

    /* Calculate the rect of macro end tool button. */
    m_wndToolbar->GetWindowRect(&toolbar_rect);
    m_wndToolbar->GetItemRect(nIndex, &macro_end_rect);
    macro_end_rect.left += toolbar_rect.left;
    macro_end_rect.right += toolbar_rect.left;
    macro_end_rect.top += toolbar_rect.top;
    macro_end_rect.bottom += toolbar_rect.top;

    /* Calculate the rect of macro play tool button. */
    nIndex = m_wndToolbar->CommandToIndex(ID_PLAYBACKMACRO);
    m_wndToolbar->GetItemRect(nIndex, &macro_playback_rect);
    macro_playback_rect.left += toolbar_rect.left;
    macro_playback_rect.right += toolbar_rect.left;
    macro_playback_rect.top += toolbar_rect.top;
    macro_playback_rect.bottom += toolbar_rect.top;

    /* Skip 'macro end' and 'macro playback'click event. */
    if (!PtInRect(macro_end_rect, pt) && !PtInRect(macro_playback_rect, pt))
    {

        /* Converts the screen coordinates of a specified point on the screen to
           client-area coordinates. */
        m_hwnd->ScreenToClient(&pt);
        CString str;
        INPUT input;

        switch (wParam)
        {
        case WM_LBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDOWN:
        case WM_MOUSEMOVE:

            /* One button click action may generate several button down messages,
               unable to figure out the reason, just check the previous message with
               the current one to excude repeated messages. */
            if ((premsg == wParam) && (prept.x == pt.x) && (prept.y == pt.y))
            {
                break;
            }

            premsg = wParam;
            prept = pt;

            input.type = INPUT_MOUSE;
            input.mi.dx = pt.x;
            input.mi.dy = pt.y;
            input.mi.mouseData = 0;
            input.mi.dwFlags = wParam;
            input.mi.time = (GetTickCount() - start_time) / MACRO_RECORD_SPEED;
            input.mi.dwExtraInfo = 0;
            recorder->Write(&input, sizeof(INPUT));

            start_time = GetTickCount();
            break;

        case WM_MOUSEWHEEL:
        {
            MOUSEHOOKSTRUCTEX *hook_structex = (MOUSEHOOKSTRUCTEX *)lParam;
            pt = hook_structex->pt;
            m_hwnd->ScreenToClient(&pt);

            input.type = INPUT_MOUSE;
            input.mi.dx = pt.x;
            input.mi.dy = pt.y;
            input.mi.mouseData = ((signed int)hook_structex->mouseData >> 16);
            input.mi.dwFlags = wParam;
            input.mi.time = (GetTickCount() - start_time) / MACRO_RECORD_SPEED;
            input.mi.dwExtraInfo = 0;

            recorder->Write(&input, sizeof(INPUT));

            start_time = GetTickCount();
        }
        break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK KeybdHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode < 0)
    {
        CallNextHookEx(NULL, nCode, wParam, lParam);
        return 0;
    }

    INPUT input;

    int extended_key = (lParam & 0x01000000) >> 16;

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = wParam;
    input.ki.wScan = (lParam & 0x0000ff00)>>8;;
    input.ki.dwFlags = (lParam & 0x80000000) >> 31;

    if ((pre_key_state != input.ki.dwFlags) ||
        (pre_vkey != input.ki.wVk))
    {
        pre_key_state = input.ki.dwFlags;
        pre_vkey = input.ki.wVk;

        if (input.ki.dwFlags)
        {
            input.ki.dwFlags = KEYEVENTF_KEYUP;
        }

        if (extended_key)
        {
            input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
        }

        input.ki.time = (GetTickCount() - start_time) / MACRO_RECORD_SPEED;
        recorder->Write(&input, sizeof(INPUT));
    }
    start_time = GetTickCount();

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL PlaybackOnemacro(CMainFrame *m_hwnd, CString pathname)
{
    CFile *playback = new CFile();

    if (!playback->Open(pathname.GetBuffer(), CFile::modeRead))
    {
        return FALSE;
    }

    /* Read layout information. */
    TCHAR head_flag[50];
    playback->Read(&head_flag, sizeof(MACRO_FILE_FLAG));

    if (_tcscmp(head_flag, MACRO_FILE_FLAG) != 0)
    {
        playback->Close();
        delete playback;

        return FALSE;
    }

    /* Set layout. */
    MACRO_HEAD_INFO macro_info;
    playback->Read(&macro_info, sizeof(MACRO_HEAD_INFO));

    proj_tree_state.RemoveAll();
    int state;
    for (int i = 0; i < macro_info.proj_tree_item_count; i++)
    {
        playback->Read(&state, sizeof(int));
        proj_tree_state.Add(state);
    }

    m_hwnd->SendMessage(WM_SET_LAYOUT, (WPARAM)&macro_info, NULL);

    /* Retrieve desktop window class. */
    int screenx;
    int screeny;

    /* Retrieve screen resolution.  */
    screenx = GetSystemMetrics(SM_CXSCREEN);
    screeny = GetSystemMetrics(SM_CYSCREEN);

    INPUT input;
    int interval_time = 0;
    int ret = playback->Read(&input, sizeof(INPUT));

    /* Handle mouse events.  */
    while (ret == sizeof(INPUT))
    {
        if (input.type == INPUT_MOUSE)
        {
            POINT pt;

            pt.x = input.mi.dx;
            pt.y = input.mi.dy;

            /* Convert client coordinate to screen coordinate. */
            m_hwnd->ClientToScreen(&pt);

            input.mi.dx = (65536 * pt.x + screenx / 2) / screenx;
            input.mi.dy = (65536 * pt.y + screeny / 2) / screeny;

            switch (input.mi.dwFlags)
            {
            case WM_LBUTTONDBLCLK:
            case WM_LBUTTONDOWN:
            case WM_NCLBUTTONDOWN:
            case WM_NCLBUTTONDBLCLK:
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                break;
            case WM_LBUTTONUP:
            case WM_NCLBUTTONUP:
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                break;
            case WM_RBUTTONDOWN:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                break;
            case WM_RBUTTONUP:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                break;
            case WM_MOUSEWHEEL:
                input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                break;
            }

            input.mi.dwFlags |= MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            interval_time = input.mi.time;
            input.mi.time = 0;
        }
        else if (input.type == INPUT_KEYBOARD)
        {
            interval_time = input.ki.time;
            input.ki.time = 0;

            switch (input.ki.wVk)
            {
            case VK_LWIN:
            case VK_RWIN:
                /* Read next message. */
                ret = playback->Read(&input, sizeof(INPUT));
                continue;
            }
        }

        Sleep(interval_time);

        /* Post message.  */
        SendInput(1, &input, sizeof(INPUT));

        /* Read next message. */
        ret = playback->Read(&input, sizeof(INPUT));
    }

    playback->Close();
    delete playback;

    return TRUE;
}

DWORD MacroPlayThread(LPVOID lParam)
{
    CMainFrame *m_hwnd = (CMainFrame *)lParam;

    /* Sleep 500 tick count to initiating. */
    Sleep(500);

    CString str;

    str = macro_name;

    if (macro_filetype == ID_SUPERMACRO)
    {
        /* Play multi macro files.  */
        CStdioFile *pSuperMacro = new CStdioFile();
        if(pSuperMacro->Open(str, CStdioFile::modeRead))
        {

            int index = str.ReverseFind('/');
            if(index == -1)
            {
                index = str.ReverseFind('\\');
            }

            if(index)
            {
                str = str.Left(index + 1);
            }

            CString pathname;
            while(pSuperMacro->ReadString(pathname))
            {
                if (pathname.IsEmpty())
                {
                    continue;
                }

                str.Append(pathname);

                if (!PlaybackOnemacro(m_hwnd, str))
                {
                    m_hwnd->MessageBox(_T("Failed to open macro file."), _T("Error"), MB_ICONERROR | MB_OK);
                    break;
                }

                str = str.Left(index + 1);
            }

            pSuperMacro->Close();
            delete pSuperMacro;
        }
        else
        {
            m_hwnd->MessageBox(_T("Failed to open macro file."), _T("Error"), MB_ICONERROR | MB_OK);
        }
    }
    else
    {
        /* Play one macro file.  */
        PlaybackOnemacro(m_hwnd, str);
    }

    WPARAM wParam = MAKEWPARAM(ID_STOPMACRO, BN_CLICKED);

    BOOL ret = m_hwnd->PostMessage(WM_COMMAND, wParam, NULL);

    return 0;
}

void CMainFrame::OnRecordmacro()
{
    // TODO: Add your command handler code here
    MacroRecordDlg mrdlg(this);

    mrdlg.SetOwner(this);
    mrdlg.DoModal();

    if(!mrdlg.Getmacro_path().IsEmpty() && !mrdlg.Getmacro_name().IsEmpty())
    {
        macro_name.Format(_T("%s\\%s"), mrdlg.Getmacro_path(), mrdlg.Getmacro_name());

        recorder = new CFile();

        if(recorder == NULL)
        {
            return;
        }

        if(recorder->Open(macro_name.GetBuffer(), CFile::modeCreate | CFile::modeWrite))
        {
            UINT nIndex, nID, nStyle;
            int iImage;
            nIndex = m_wndToolBar.CommandToIndex(ID_RECORDMACRO);
            m_wndToolBar.GetButtonInfo(nIndex, nID, nStyle, iImage);
            m_wndToolBar.SetButtonInfo(nIndex, ID_ENDMACRO, nStyle, iImage);

            CImageList *pList = m_wndToolBar.GetToolBarCtrl().GetImageList();

            CBitmap bmImage;
            bmImage.LoadBitmap(IDB_MACRO_END);

            /* Change image of record button in toolbar. */
            pList->Replace(iImage, &bmImage, NULL);
            m_wndToolBar.Invalidate();

            CRect rect;
            m_splitter.GetWindowRect(rect);

            int total_width = rect.right - rect.left - 20;
            int cxMin;
        
            GetWindowRect(rect);
            StudioXIni.xpos = rect.left;
            StudioXIni.ypos = rect.top;
            StudioXIni.width = rect.Width();
            StudioXIni.height = rect.Height();
            m_splitter.GetColumnInfo(0, StudioXIni.proj_view_width, cxMin);
            m_splitter.GetColumnInfo(2, StudioXIni.resource_view_width, cxMin);

            /* Write a file flag to distinguish it from other files. */
            recorder->Write(MACRO_FILE_FLAG, sizeof(MACRO_FILE_FLAG));

            MACRO_HEAD_INFO info;
            info.ini_version = StudioXIni.ini_version;
            info.xpos = StudioXIni.xpos;
            info.ypos = StudioXIni.ypos;
            info.width = StudioXIni.width;
            info.height = StudioXIni.height;
            info.proj_view_width = StudioXIni.proj_view_width;
            info.resource_view_width = StudioXIni.resource_view_width;
            info.string_open = GetResourceView()->IsOpen(STRING_GROUP);
            info.color_open = GetResourceView()->IsOpen(COLOR_GROUP);
            info.font_open = GetResourceView()->IsOpen(FONT_GROUP);
            info.pixelmap_open = GetResourceView()->IsOpen(PIXELMAP_GROUP);
            info.proj_view_scrollpos = GetProjectView()->GetScrollPos(SB_VERT);
            info.res_view_scrollpos = GetResourceView()->GetScrollPos(SB_VERT);
            info.props_win_scrollpos = GetPropsWin()->GetScrollPos(SB_VERT);
            
            GetProjectView()->GetTreeState(&proj_tree_state);
            info.proj_tree_item_count = proj_tree_state.GetCount();

            /* Write layout information. */
            recorder->Write(&info, sizeof(MACRO_HEAD_INFO));
            
            /* Write project tree item state array.  */
            recorder->Write((int *)proj_tree_state.GetData(), sizeof(int) * proj_tree_state.GetCount());

            mouse_hook = SetWindowsHookEx(WH_MOUSE, MouseHook, NULL, GetCurrentThreadId());

            keybd_hook = SetWindowsHookEx(WH_KEYBOARD, KeybdHook, NULL, GetCurrentThreadId());

            start_time = GetTickCount();
        }
        else
        {
            CString msg;
            msg.Format(_T("Unable to open \"%d\"."), macro_name);
            ErrorMsg(msg);
        }
    }
}

void CMainFrame::OnPlaybackmacro()
{
    // TODO: Add your command handler code here
    MacroPlaybackDlg mpbdlg(this);

    mpbdlg.SetOwner(this);
    mpbdlg.DoModal();

    macro_name = mpbdlg.Getmacro_pathname();

    if(!macro_name.IsEmpty())
    {
    
        CStdioFile *pMacroFile = new CStdioFile();

        if (pMacroFile->Open(macro_name, CStdioFile::modeRead))
        {

            TCHAR head[50];

            /* Read file head. */
            pMacroFile->Read(head, sizeof(MACRO_FILE_FLAG));

            if (_tcscmp(head, MACRO_FILE_FLAG) == 0)
            {
                /* Single macro file. */
                macro_filetype = ID_SINGLEMACRO;

            }
            else
            {
                /* Super macro file. */
                macro_filetype = ID_SUPERMACRO;
            }

            pMacroFile->Close();
            delete pMacroFile;

            UINT nIndex, nID, nStyle;
            int iImage;
            nIndex = m_wndToolBar.CommandToIndex(ID_PLAYBACKMACRO);
            m_wndToolBar.GetButtonInfo(nIndex, nID, nStyle, iImage);
            m_wndToolBar.SetButtonInfo(nIndex, ID_STOPMACRO, nStyle, iImage);

            CImageList *pList = m_wndToolBar.GetToolBarCtrl().GetImageList();

            CBitmap bmImage;
            bmImage.LoadBitmap(IDB_MACRO_STOP);

            /* Change image of record button in toolbar. */
            pList->Replace(iImage, &bmImage, NULL);
            m_wndToolBar.Invalidate();

            /* Registr window message. */
            WM_SET_LAYOUT = RegisterWindowMessage(_T("MSG_SET_LAYOUT"));

            /* Start macro playback thread. */
            macro_play_thread = AfxBeginThread((AFX_THREADPROC)&MacroPlayThread, (LPVOID)this);
        }
        else
        {
            ErrorMsg("Failed to open macro file");
        }
    }
}

void CMainFrame::OnStopMacro()
{

    UINT nIndex, nID, nStyle;
    int iImage;
    nIndex = m_wndToolBar.CommandToIndex(ID_STOPMACRO);
    m_wndToolBar.GetButtonInfo(nIndex, nID, nStyle, iImage);
    m_wndToolBar.SetButtonInfo(nIndex, ID_PLAYBACKMACRO, nStyle, iImage);

    CImageList *pList = m_wndToolBar.GetToolBarCtrl().GetImageList();

    CBitmap bmImage;
    bmImage.LoadBitmap(IDB_MACRO_PLAYBACK);
    pList->Replace(iImage, &bmImage, NULL);
    m_wndToolBar.Invalidate();

    if (macro_play_thread)
    {
        /* Terminate macro playback thread. */
        TerminateThread(macro_play_thread, NULL);
        macro_play_thread = NULL;
    }
}

void CMainFrame::OnEndmacro()
{
    // TODO: Add your command handler code here

    UINT nIndex, nID, nStyle;
    int iImage;
    nIndex = m_wndToolBar.CommandToIndex(ID_ENDMACRO);
    m_wndToolBar.GetButtonInfo(nIndex, nID, nStyle, iImage);
    m_wndToolBar.SetButtonInfo(nIndex, ID_RECORDMACRO, nStyle, iImage);

    CImageList *pList = m_wndToolBar.GetToolBarCtrl().GetImageList();

    CBitmap bmImage;
    bmImage.LoadBitmap(IDB_MACRO_RECORD);
    pList->Replace(iImage, &bmImage, NULL);
    m_wndToolBar.Invalidate();

    if (mouse_hook)
    {
        UnhookWindowsHookEx(mouse_hook);
        mouse_hook = 0;
    }

    if (keybd_hook)
    {
        UnhookWindowsHookEx(keybd_hook);
        keybd_hook = 0;
    }

    if(recorder)
    {
        recorder->Close();

        delete recorder;
        recorder = NULL;
    }
}

BOOL CMainFrame::IsTestMode()
{
    if (macro_play_thread || recorder)
    {
        return TRUE;
    }

    return FALSE;
}

LRESULT CMainFrame::OnSetLayout(WPARAM wParam, LPARAM lParam)
{
    MACRO_HEAD_INFO *ini_info = (MACRO_HEAD_INFO *)wParam;

    /* Reset studio layout. */
    MoveWindow(ini_info->xpos, ini_info->ypos, ini_info->width, ini_info->height);

    m_splitter.SetColumnInfo(0, ini_info->proj_view_width, 200);
    m_splitter.SetColumnInfo(1, 0, 0);
    m_splitter.SetColumnInfo(2, ini_info->resource_view_width, 200);
    m_splitter.RecalcLayout();

    GetResourceView()->SetResFolderStatus(COLOR_GROUP, ini_info->color_open);
    GetResourceView()->SetResFolderStatus(FONT_GROUP, ini_info->font_open);
    GetResourceView()->SetResFolderStatus(PIXELMAP_GROUP, ini_info->pixelmap_open);
    GetResourceView()->SetResFolderStatus(STRING_GROUP, ini_info->string_open);

    /* Set project view state. */
    GetProjectView()->SetTreeState(&proj_tree_state);

    /* Set scrollbar position.  */
    GetProjectView()->SetScrollPos(SB_VERT, ini_info->proj_view_scrollpos);
    GetResourceView()->SetScrollPos(SB_VERT, ini_info->res_view_scrollpos);
    GetPropsWin()->SetScrollPos(SB_VERT, ini_info->props_win_scrollpos);

    return 0;
}


void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    if (mInsertMenu.GetSubMenu(0) == pPopupMenu)
    {
        int insert_folder_style;
        int insert_widget_style;
        project_view *view = GetProjectView();
        int selected_node_level = PROJECT_NODE_LEVEL;

        if (view)
        {
            selected_node_level = view->GetSelectedNodeLevel();
        }

        switch (selected_node_level)
        {
        case PROJECT_NODE_LEVEL:
            //all items are disabled
            insert_folder_style = MF_BYPOSITION | MF_DISABLED | MF_GRAYED;
            insert_widget_style = MF_BYPOSITION | MF_DISABLED | MF_GRAYED;
            break;
        case DISPLAY_NODE_LEVEL:
            //enabled insert folder item, disable insert widget items
            insert_folder_style = MF_BYPOSITION | MF_ENABLED;
            insert_widget_style = MF_BYPOSITION | MF_DISABLED | MF_GRAYED;
            break;
        default:
            //disabled insert folder item, enable insert widget items
            insert_folder_style = MF_BYPOSITION | MF_DISABLED | MF_GRAYED;
            insert_widget_style = MF_BYPOSITION | MF_ENABLED;
        }

        int count = pPopupMenu->GetMenuItemCount();
        int id;
        for (int pos = 0; pos < count; pos++)
        {
            id = pPopupMenu->GetMenuItemID(pos);
            if (id == ID_INSERT_FOLDER)
            {
                pPopupMenu->EnableMenuItem(pos, insert_folder_style);
            }
            else
            {
                pPopupMenu->EnableMenuItem(pos, insert_widget_style);
            }
        }

        ULONG flags = GetMenu()->GetMenuState(nIndex, MF_BYPOSITION);
        // If menu is disabled or grayed, Should handle nothing.
        if (!(flags & (MF_DISABLED | MF_GRAYED)))
        {
            GetProjectView()->UpdateBaseMenu(&mInsertMenu);
        }
    }
    else
    {
        int index = GetSubMenuIndex(GetMenu(), _T("&Project"));
        CMenu *p_project_menu = GetMenu()->GetSubMenu(index);

        index = GetSubMenuIndex(GetMenu(), _T("&Configure"));
        CMenu *p_configure_menu = GetMenu()->GetSubMenu(index);

        if (p_project_menu == pPopupMenu)
        {
            if (GetOpenProject())
            {
                EnableMenuItem(p_project_menu, ID_PROJECT_SAVE);
                EnableMenuItem(p_project_menu, ID_PROJECT_SAVE_AS);
                EnableMenuItem(p_project_menu, ID_PROJECT_CLOSE);
                EnableMenuItem(p_project_menu, ID_GENERATE_ALL);
                EnableMenuItem(p_project_menu, ID_GENERATE_RESOURCES);
                EnableMenuItem(p_project_menu, ID_GENERATE_APPLICATION);
                if (GetProjectView()->IsFolderSelected())
                {
                    // import project must specify the target folder. So only enable import button while folder selected.
                    EnableMenuItem(p_project_menu, ID_PROJECT_IMPORT);
                }
                else
                {
                    DisableMenuItem(p_project_menu, ID_PROJECT_IMPORT);
                }
            }
            else
            {
                DisableMenuItem(p_project_menu, ID_PROJECT_SAVE);
                DisableMenuItem(p_project_menu, ID_PROJECT_SAVE_AS);
                DisableMenuItem(p_project_menu, ID_PROJECT_CLOSE);
                DisableMenuItem(p_project_menu, ID_GENERATE_ALL);
                DisableMenuItem(p_project_menu, ID_GENERATE_RESOURCES);
                DisableMenuItem(p_project_menu, ID_GENERATE_APPLICATION);
                DisableMenuItem(p_project_menu, ID_PROJECT_IMPORT);
            }
        }
        else if (p_configure_menu == pPopupMenu)
        {
            studiox_project *project = GetOpenProject();

            if (project)
            {
                if (project->IsWidgetPositionLocked())
                {
                    p_configure_menu->CheckMenuItem(ID_LOCK_WIDGET_POSITIONS, MF_CHECKED);
                }
                else
                {
                    p_configure_menu->CheckMenuItem(ID_LOCK_WIDGET_POSITIONS, MF_UNCHECKED);
                }
            }
        }
    }

    // TODO: Add your message handler code here
}

void CMainFrame::DisableMenus()
{
    CMenu *p_menu = GetMenu();
    if (p_menu)
    {
        DisableMenuItem(p_menu, _T("&Insert"));
        DisableMenuItem(p_menu, _T("&Edit"));
//        DisableMenuItem(p_menu, _T("&Configure"));

        int index = GetSubMenuIndex(p_menu, _T("&Configure"));
        CMenu* p_configure_menu = p_menu->GetSubMenu(index);

        DisableMenuItem(p_configure_menu, _T("Project/Displays"));
        DisableMenuItem(p_configure_menu, _T("Languages"));
        DisableMenuItem(p_configure_menu, _T("Themes"));
        DisableMenuItem(p_configure_menu, _T("Target View"));
        DisableMenuItem(p_configure_menu, _T("Screen Flow"));
    }
}

void CMainFrame::EnableMenus()
{
    CMenu *p_menu = GetMenu();

    if (p_menu)
    {
        EnableMenuItem(p_menu, _T("&Insert"));
        EnableMenuItem(p_menu, _T("&Edit"));
        //EnableMenuItem(p_menu, _T("&Configure"));

        int index = GetSubMenuIndex(p_menu, _T("&Configure"));
        CMenu* p_configure_menu = p_menu->GetSubMenu(index);

        EnableMenuItem(p_configure_menu, _T("Project/Displays"));
        EnableMenuItem(p_configure_menu, _T("Languages"));
        EnableMenuItem(p_configure_menu, _T("Themes"));
        EnableMenuItem(p_configure_menu, _T("Target View"));
        EnableMenuItem(p_configure_menu, _T("Screen Flow"));
    }
}

void CMainFrame::DisableMenuItem(CMenu *menu, CString string)
{
    if (menu)
    {
        int index = GetSubMenuIndex(menu, string);
        menu->EnableMenuItem( index, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
    }
}

void CMainFrame::EnableMenuItem(CMenu *menu, CString string)
{
    if (menu)
    {
        int index = GetSubMenuIndex(menu, string);
        menu->EnableMenuItem(index, MF_BYPOSITION | MF_ENABLED);
        DrawMenuBar();
    }
}

void CMainFrame::DisableMenuItem(CMenu *menu, INT id)
{
    if (menu)
    {
        menu->EnableMenuItem(id, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
}

void CMainFrame::EnableMenuItem(CMenu *menu, INT id)
{
    if (menu)
    {
        menu->EnableMenuItem(id, MF_BYCOMMAND | MF_ENABLED);
        DrawMenuBar();
    }
}


void CMainFrame::UpdateRecentProjectsMenu()
{
    if (mpRecentMenu)
    {
        int index = mpRecentMenu->GetMenuItemCount();

        while (index)
        {
            mpRecentMenu->RemoveMenu(index - 1, MF_BYPOSITION);
            index--;
        }

        for (index = 0; index < MAX_RECENT_PROJECTS; index++)
        {
            if (!StudioXIni.recent_project_paths[index].IsEmpty())
            {
                mpRecentMenu->AppendMenu(MF_STRING, ID_FILE_MRU_FILE2 + index, StudioXIni.recent_project_paths[index]);
            }
        }
    }
}

LRESULT CMainFrame::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    CStringArray param;
    CString classname;
    CString title;
    CString parent_owner;
    CWnd *pWnd;
    studiox_project *project;

    switch (wParam)
    {
    case TEST_TOOLBAR_CREATE:
        GetProjectView()->CloseProject(FALSE, FALSE);
        theApp.OnProjectNew();
        break;

    case TEST_TOOLBAR_OPEN:
        break;

    case TEST_TOOLBAR_SAVE:
        theApp.OnProjectSave();
        break;

    case TEST_TOOLBAR_CUT:
        OnCutWidget();
        break;

    case TEST_TOOLBAR_COPY:
        OnCopyWidget();
        break;

    case TEST_TOOLBAR_PASTE:
        OnPasteWidget();
        break;

    case TEST_TOOLBAR_ALIGN_LEFT:
        OnAlignLeft();
        break;

    case TEST_TOOLBAR_ALIGN_RIGHT:
        OnAlignRight();
        break;

    case TEST_TOOLBAR_ALIGN_TOP:
        OnAlignTop();
        break;

    case TEST_TOOLBAR_ALIGN_BOTTOM:
        OnAlignBottom();
        break;

    case TEST_TOOLBAR_VSPACE_EQUALLY:
        OnVSpaceEqually();
        break;

    case TEST_TOOLBAR_HSPACE_EQUALLY:
        OnHSpaceEqually();
        break;

    case TEST_TOOLBAR_EQUAL_WIDTH:
        OnEqualWidth();
        break;

    case TEST_TOOLBAR_EQUAL_HEIGHT:
        OnEqualHeight();
        break;

    case TEST_TOOLBAR_MOVE_TO_FRONT:
        OnMoveToFront();
        break;

    case TEST_TOOLBAR_MOVE_TO_BACK:
        OnMoveToBack();
        break;

    case TEST_TOOLBAR_SIZE_TO_FIT:
        OnSizeToFit();
        break;

    case TEST_TOOLBAR_ZOOM_IN:
        SendMessage(WM_COMMAND, ID_ZOOM_IN, 0);
        break;

    case TEST_TOOLBAR_ZOOM_OUT:
        SendMessage(WM_COMMAND, ID_ZOOM_OUT, 0);
        break;

    case TEST_TOOLBAR_MACRO_RECORD:
        OnRecordmacro();
        break;

    case TEST_TOOLBAR_MACRO_PLAYBACK:
        OnPlaybackmacro();
        break;

    case TEST_TOOLBAR_ABOUT:
        theApp.OnAppAbout();
        break;

    case TEST_UNDO:
        OnUndo();
        break;

    case TEST_CONFIGURE_LANGUAGES:
        OnConfigureLanguages();
        break;

    case TEST_CONFIGURE_DISPLAYS:
        OnConfigureDisplays();
        break;

    case TEST_CONFIGURE_THEMES:
        OnConfigureThemes();
        break;

    case TEST_CONFIGURE_SCREEN_FLOW:
        OnConfigureScreenFlow();
        break;

    case TEST_GENERATE_ALL:
        OnGenerateAll();
        break;

    case TEST_GENERATE_RESOURCES:
        OnGenResources();
        break;

    case TEST_GET_MENU_STATUS:
    {
        int index = GetSubMenuIndex(GetMenu(), GetTestingParam(0));
        int status;

        //get menu status
        status = GetMenu()->GetMenuState(index, MF_BYPOSITION);

        if (status & MF_DISABLED)
        {
            //disabled
            return 1;
        }
        else
        {
            //enabled
            return 0;
        }
    }

    case TEST_GRID_SNAP_SETTING:
        OnGridSetting();
        break;

    case TEST_FIND_DIALOG:
        SplitString(GetTestingParam(0), ';', &param);
        classname = param.GetAt(0);
        title = param.GetAt(1);
        parent_owner = param.GetAt(2);

        // Retrieve a handle to the top-level window whose class name and window name match the specified strings.
        pWnd = FindWindow(classname, title);

        while (pWnd)
        {
            CWnd *owner = pWnd->GetOwner();

            if (classname == "GUIX")
            {
                GX_WIN32_DISPLAY_DRIVER_DATA *data = gx_win32_get_data_instance_head();
                for (int index = 0; index < GX_MAX_WIN32_DISPLAYS; index++)
                {
                    if ((data->win32_driver_winhandle == pWnd->GetSafeHwnd()) &&
                        (pWnd->IsWindowVisible()))
                    {
                        result = (LRESULT)pWnd->GetSafeHwnd();
                        break;
                    }
                    data++;
                }

                break;
            }
            else
            {
                HWND handle = 0;

                if (owner)
                {
                    handle = owner->GetSafeHwnd();
                }

                if (handle == (HWND)_tstoi(parent_owner))
                {
                    // The parent handle match, found the dialog
                    if (pWnd->IsWindowVisible())
                    {
                        result = (LRESULT)pWnd->GetSafeHwnd();
                    }
                    break;
                }

                // Retrieve a handle to a window whose class name and window name match the specified strings.
                pWnd = FindWindowEx(NULL, // Search among windows that are child windows of the desktop.
                                    handle,// A handle to a child window. The search begins with the next windnow in the Z order. If the parameter is NULL, the search begings with the first child window,
                                    classname, // The class name.
                                    title); // The window name.
            }
        }
        break;

    case TEST_LOCK_UNLOCK_WIDGET_POSITIONS:
        project = GetOpenProject();
        if (project)
        {
            project->LockUlockWidgetPositions(lParam);
        }
        break;

    case TEST_RUN_APPLICATION:
        OnRunApplication();
        break;

    case TEST_IS_WAIT_DIALOG_RUNNING:
        result = IsWaitDialogRunning();
        break;

    default:
        break;
    }
    return result;
}

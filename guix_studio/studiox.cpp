// studiox.cpp : Defines the class behaviors for the application.
//

#include "studiox_includes.h"
#include "studioxDoc.h"
#include "splash_screen.h"

#include "studiox_includes.h"
#include "studiox.h"
#include "clone_repo_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* some fonts used by this app */

CFont TitleFont;
CFont MediumFont;
CFont NormalFont;
CFont TinyFont;
CFont ViewHeaderFont;
CFont AsteriskFont;
UINT  StudioClipboardFormat = 0;

// CstudioxApp

BEGIN_MESSAGE_MAP(CstudioxApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CstudioxApp::OnAppAbout)
    ON_COMMAND(ID_HELP_USER_GUIDE, &CstudioxApp::OnUsersGuide)
	// Standard file based document commands
	ON_COMMAND(ID_PROJECT_NEW, &CstudioxApp::OnProjectNew)
    ON_COMMAND(ID_FILE_NEW, &CstudioxApp::OnProjectNew)
	ON_COMMAND(ID_PROJECT_OPEN, &CstudioxApp::OnProjectOpen)
    ON_COMMAND(ID_FILE_OPEN, &CstudioxApp::OnProjectOpen)
    ON_COMMAND(ID_PROJECT_SAVE, &CstudioxApp::OnProjectSave)
    ON_COMMAND(ID_FILE_SAVE, &CstudioxApp::OnProjectSave)
    ON_COMMAND(ID_PROJECT_SAVE_AS, &CstudioxApp::OnProjectSaveAs)
    ON_COMMAND(ID_PROJECT_CLOSE, &CstudioxApp::OnProjectClose)
    ON_COMMAND(ID_FILE_CLOSE, &CstudioxApp::OnProjectClose)
    ON_COMMAND(ID_PROJECT_IMPORT, &CstudioxApp::OnProjectMerge)
	// Standard print setup command
END_MESSAGE_MAP()



// CstudioxApp construction

CstudioxApp::CstudioxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
   #if defined (REPORT_MEMORY_LEAKS)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   #endif
}


// The one and only CstudioxApp object

CstudioxApp theApp;


CFrameWnd *CstudioxApp::CreateMainFrame()
{
    LoadIcon(IDR_MAINFRAME);

	CFrameWnd* pFrame = new CMainFrame();

	// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, NULL))
	{
		TRACE(traceAppMsg, 0, "Warning: couldn't create a frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

// CstudioxApp initialization
CString splash_class_name;
CString studiox_version_string;

void CstudioxApp::DeleteSystemFonts()
{
    TitleFont.DeleteObject();
    MediumFont.DeleteObject();
    NormalFont.DeleteObject();
    AsteriskFont.DeleteObject();
    TinyFont.DeleteObject();
    ViewHeaderFont.DeleteObject();
}

void CstudioxApp::CreateSystemFonts()
{
    /* Create the fonts used by app */

// this formula can be used to convert point size to logical units
// lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);

    HDC dc = ::GetDC(NULL);

    // Get number of pixels per logical inch along the screen height
    int dpi = GetDeviceCaps(dc, LOGPIXELSY);
    int text_scaler = GetTextScaler();

    TitleFont.CreateFont(TITLE_FONT_HEIGHT, 0, 0, 0,
        FW_BOLD, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Convert point size to logic unit
    int height = -MulDiv(MEDIUM_FONT_PT_SIZE, dpi, 72);
    height = MulDiv(height, text_scaler, DEFAULT_TEXT_SCALER);
    MediumFont.CreateFont(height, 0, 0, 0,
        FW_SEMIBOLD, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    height = -MulDiv(NORMAL_FONT_PT_SIZE, dpi, 72);
    height = MulDiv(height, text_scaler, DEFAULT_TEXT_SCALER);
    NormalFont.CreateFont(height, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    height = -MulDiv(ASTERISK_FONT_PT_SIZE, dpi, 72);
    height = MulDiv(height, text_scaler, DEFAULT_TEXT_SCALER);
    AsteriskFont.CreateFont(height, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    TinyFont.CreateFont(TYNY_FONT_HEIGHT, 0, 0, 0,
        FW_BOLD, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, NULL);

    height = -MulDiv(VIEW_HEADER_FONT_PT_SIZE, dpi, 72);
    height = MulDiv(height, text_scaler, DEFAULT_TEXT_SCALER);
    ViewHeaderFont.CreateFont(height, 0, 0, 0,
        FW_SEMIBOLD, FALSE, FALSE, 0,
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    ReleaseDC(NULL, dc);
}

BOOL CstudioxApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	
	//LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

    CreateSystemFonts();

    /* Initialize our INI_INFO structure */
    ReadIniInfo();

    /* Register out clipboard format */
    StudioClipboardFormat = RegisterClipboardFormat(_T("GUIX_Studio_Widget_Info"));

    /* Create the MainFrame, but don't show it yet */
    m_pMainWnd = CreateMainFrame();

    if (!m_pMainWnd)
    {
        return FALSE;
    }

    studiox_version_string.Format(_T("%d.%d.%d.%d"), 
            GUIX_MAJOR_VERSION,
            GUIX_MINOR_VERSION,
            GUIX_PATCH_VERSION,
            STUDIOX_VERSION_NUMBER);

#if defined(STUDIO_ENGINEERING_RELEASE)
    studiox_version_string += CString("-engineering");
#endif

    CString fulltitle = _T("Eclipse ThreadX GUIX Studio ");
    fulltitle += studiox_version_string;

    m_pMainWnd->SetWindowText(fulltitle);

    splash_class_name = AfxRegisterWndClass(
      CS_VREDRAW | CS_HREDRAW,
      ::LoadCursor(NULL, IDC_ARROW),
      CreateSolidBrush(RGB(0, 0, 0)),
      ::LoadIcon(NULL, IDI_APPLICATION));

    /* Read command info. */
    CMainFrame *pMain = (CMainFrame *) AfxGetMainWnd();
    CCommandInfo *pCmdInfo = pMain->GetCmdInfo();

    if(m_lpCmdLine[0])
    {
        pCmdInfo -> ReadCommandLineInfo(m_lpCmdLine);

        CString guix_repo_local_path = pCmdInfo->GetGuixRepoLocalPath();
        if (!guix_repo_local_path.IsEmpty())
        {
            if (guix_repo_local_path.GetLength() >= MAX_PATH)
            {
                return FALSE;
            }
            char local_path[MAX_PATH];
            memset(local_path, 0, MAX_PATH);
            HANDLE file_map_handle = pCmdInfo->GetGuixCloneWritePipeHandle();
            strcpy_s(local_path, MAX_PATH, CStringA(guix_repo_local_path).GetString());
            CloneGUIXRepo(local_path, file_map_handle);

            pMain->OnClose();

            return TRUE;
        }
    }

    /* Display the Splash screen */
#if !defined(_DEBUG)
//#if 1

    if(!pCmdInfo->IsNoGui())
    {
        int tag_status;

        CRect childrect(0, 0, 425, 283);
        splash_screen *splash = new splash_screen(TRUE);
        splash->CreateEx(0, splash_class_name, NULL,
            WS_POPUP|WS_VISIBLE|WS_BORDER, childrect, NULL, 0, NULL);
        tag_status = splash->RunModalLoop();
        splash->DestroyWindow();
        delete splash;

        if (tag_status != 0)
        {
            return FALSE;
        }
    }
#endif
	// The one and only window has been initialized, so show and update it

    if (!pCmdInfo->IsNoGui())
    {
        m_pMainWnd->ShowWindow(SW_SHOW);
        m_pMainWnd->UpdateWindow();
    }

    //Initialize GDI+ resources
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    if (m_lpCmdLine[0])
    {
        /* Open the log file. */
        if (pCmdInfo->GetLogFile())
        {
            fprintf(pCmdInfo->GetLogFile(), "\nProject File: %s\\%s\n", (char*)CW2A(pCmdInfo->GetProjectPath()), (char*)CW2A(pCmdInfo->GetProjectName()));
        }

        /* Open project. */
        if (GetProjectView()->OpenProject(pCmdInfo->GetProjectPath(), pCmdInfo->GetProjectName()))
        {

            CString output_path = pCmdInfo->GetOutputPath();
            if (!output_path.IsEmpty())
            {
                studiox_project *project = GetOpenProject();
                if (project)
                {
                    project->mHeader.resource_path = output_path;
                    project->mHeader.source_path = output_path;
                    project->mHeader.header_path = output_path;
                }
            }
            if (pCmdInfo->IsXmlMode())
            {
                /* Open the log file. */
                if (pCmdInfo->GetLogFile())
                {
                    fprintf(pCmdInfo->GetLogFile(), "Generating Resource File.\n");
                }

                if (pCmdInfo->IsBinaryMode())
                {
                    binary_resource_gen *generater = new binary_resource_gen(GetOpenProject(), BINARY_FILE_FORMAT_BIN_STANDALONE);
                    generater->GenerateBinaryFile(0);
                    delete generater;
                }
                else
                {
                    resource_gen* generater = new resource_gen(GetOpenProject());
                    generater->GenerateResourceFile(0);
                    delete generater;
                }

                if (pCmdInfo->GetLogFile())
                {
                    fprintf(pCmdInfo->GetLogFile(), "Resource File Generation Completed.\n");
                }
            }
            else
            {

                CString pathname = pCmdInfo->GetStringImportFileName();

                if (!pathname.IsEmpty())
                {
                    /* Open the log file. */
                    if (pCmdInfo->GetLogFile())
                    {
                        fprintf(pCmdInfo->GetLogFile(), "Improting String Data.\n");
                    }

                    //import string
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
                        GetOpenProject()->SetModified();
                        Notify("String Data Import Completed.");
                    }
                    else
                    {
                        ErrorMsg("String Data Import Failed!");
                    }
                }

                if (pCmdInfo->IsNoGui())
                {

                    /* Generate resource files. */
                    if (pCmdInfo->GenResource())
                    {
                        if (pCmdInfo->IsBinaryMode())
                        {
                            GetProjectView()->OnGenBinary();
                        }
                        else
                        {
                            GetProjectView()->OnGenResources();
                        }
                    }

                    /* Generate specification files. */
                    if (pCmdInfo->GenSpecification())
                    {
                        GetProjectView()->OnGenApplication();
                    }
                }
            }
        }

        /* Close the log file. */
        if(pCmdInfo ->GetLogFile())
        {
            fclose(pCmdInfo ->GetLogFile());
        }

        if(pCmdInfo->IsNoGui())
        {
            pMain->OnClose();
        }
    }
    else
    {
        ((CMainFrame *)AfxGetMainWnd())->DisableMenus();
        GetTargetView()->InitialDisplay();
    }

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CstudioxApp::ExitInstance()
{
    GdiplusShutdown(gdiplusToken);

    return CWinApp::ExitInstance();
}


// App command to run the dialog
void CstudioxApp::OnAppAbout()
{
    CRect childrect(0, 0, 425, 283);
    splash_screen *splash = new splash_screen(FALSE);
    splash->CreateEx(0, splash_class_name, NULL,
        WS_POPUP|WS_VISIBLE|WS_BORDER, childrect, m_pMainWnd, 0, NULL);
    splash->RunModalLoop();
    splash->DestroyWindow();
    delete splash;
}

void CstudioxApp::OnUsersGuide()
{
    ShellExecute(NULL, _T("open"), _T("https://github.com/eclipse-threadx/rtos-docs/blob/main/rtos-docs/guix/about-guix-studio.md"), NULL, NULL, SW_SHOW);
}

void CstudioxApp::OnProjectNew()
{
    UndoManager()->Reset();

    CMainFrame *pMain = (CMainFrame *) m_pMainWnd;
    pMain->GetProjectView()->NewProject();    
}

void CstudioxApp::OnProjectOpen()
{
    UndoManager()->Reset();
    CMainFrame *pMain = (CMainFrame *) m_pMainWnd;
    pMain->GetProjectView()->OpenProject();

}

void CstudioxApp::OnProjectMerge()
{
    UndoManager()->Reset();
    CMainFrame *pMain = (CMainFrame *) m_pMainWnd;
    pMain->GetProjectView()->MergeProject();
}

void CstudioxApp::OnProjectClose()
{
    UndoManager()->Reset();

    CMainFrame *pMain = (CMainFrame *) m_pMainWnd;
    pMain->GetProjectView()->CloseProject(TRUE);
}

void CstudioxApp::OnProjectSave()
{
    UndoManager()->Reset();

    if (GetOpenProject())
    {
        GetOpenProject()->Save();
    }
}

void CstudioxApp::OnProjectSaveAs()
{
    UndoManager()->Reset();
    studiox_project *project = GetOpenProject();
    if (project)
    {
       project->SaveAs();
    }
}
// CstudioxApp message handlers

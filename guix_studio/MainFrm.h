// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include <afxcview.h>
#include "CommandInfo.h"
#include "undo_manager.h"

class studiox_splitter : public CSplitterWnd
{
    public:
        studiox_splitter();
        virtual void RecalcLayout();
        //virtual void TrackColumnSize(int x, int col);
        void FixColumnWidths();

    private:
        int m_proj_width;
        int m_res_width;
};

class CustomToolBar : public CToolBar
{
    public:
        CustomToolBar();

    protected:
    afx_msg BOOL OnEraseBkgnd(CDC *pDc);
    //afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

class CMainFrame : public CFrameWnd
{
    
protected: // create from serialization only
//    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
    CMainFrame();

// Operations
public:

// Overrides
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext *pContext);
       CToolBar *GetToolBar() {return &m_wndToolBar;}


    void SetProjectView(project_view *view)
    {
        mpProjectView = view;
    }    
    void SetTargetView(target_view *view)
    {
        mpTargetView = view;
    }
    void SetResourceView(resource_view *view)
    {
        mpResourceView = view;
    }
    void SetPropsWin(properties_win *win)
    {
        mpPropsWin = win;
    }
    project_view *GetProjectView()
    {
        return mpProjectView;
    }    
    target_view *GetTargetView()
    {
        return mpTargetView;
    }
    resource_view *GetResourceView()
    {
        return mpResourceView;
    }
    properties_win *GetPropsWin()
    {
        return mpPropsWin;
    }
    undo_manager *GetUndoManager()
    {
        return mpUndoManager;
    }
    CCommandInfo *GetCmdInfo()
    {
        return mpCmdInfo;
    }
    CMenu *GetBaseMenu()
    {
        return mpBaseMenu;
    }
    CStatic *GetStatusMsgControl()
    {
        return &mStatusMsg;
    }


// Implementation
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
    CustomToolBar m_wndToolBar;
    studiox_splitter m_splitter;
    CSplitterWnd m_splitter2; // Embedded in the first
    CImageList m_imagelist;

    BOOL        m_splitter_created;
    project_view    *mpProjectView;
    target_view     *mpTargetView;
    resource_view   *mpResourceView;
    properties_win  *mpPropsWin;
    CCommandInfo    *mpCmdInfo;
    undo_manager    *mpUndoManager;
    CMenu           *mpBaseMenu;
    CMenu            mInsertMenu;
    CMenu           *mpRecentMenu;
    CStatic          mStatusMsg;

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC *pDc);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMoving(UINT nSide,  LPRECT lpRect);
    afx_msg void OnKeyDown(UINT nChar, UINT Rep, UINT flags);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

    afx_msg void OnAddButton();
    afx_msg void OnAddTextButton();
    afx_msg void OnAddMltButton();
    afx_msg void OnAddCheckbox();
    afx_msg void OnAddRadioButton();
    afx_msg void OnAddIconButton();
    afx_msg void OnAddPixelmapButton();

    afx_msg void OnAddPrompt();
    afx_msg void OnAddNumericPrompt();
    afx_msg void OnAddIcon();
    afx_msg void OnAddPixelmapPrompt();
    afx_msg void OnAddNumericPixelmapPrompt();
    afx_msg void OnAddSlider();
    afx_msg void OnAddPixelmapSlider();
    afx_msg void OnAddRadialSlider();
    afx_msg void OnAddProgressBar();
    afx_msg void OnAddRadialProgressBar();
    afx_msg void OnAddSprite();

    afx_msg void OnAddHScroll();
    afx_msg void OnAddVScroll();

    afx_msg void OnAddStringScrollWheel();
    afx_msg void OnAddNumericScrollWheel();
    afx_msg void OnAddGenericScrollWheel();

    afx_msg void OnAddWindow();
    afx_msg void OnAddVerticalList();
    afx_msg void OnAddHorizontalList();
    afx_msg void OnAddDropDownList();
    afx_msg void OnAddLineChart();

    afx_msg void OnAddTextInput();
    afx_msg void OnAddMLTextView();
    afx_msg void OnAddMLTextInput();
    afx_msg void OnAddRichTextView();
    afx_msg void OnAddCircularGauge();
    afx_msg void OnAddTemplate(UINT nID);

    afx_msg void OnAddMenu();
    afx_msg void OnAddAccordionMenu();
    afx_msg void OnAddTreeView();

    afx_msg void OnDeleteWidget();
    afx_msg void OnCutWidget();
    afx_msg void OnCopyWidget();
    afx_msg void OnPasteWidget();
    afx_msg void OnInsertFolder();

    afx_msg void OnAlignLeft();
    afx_msg void OnAlignTop();
    afx_msg void OnAlignRight();
    afx_msg void OnAlignBottom();
    afx_msg void OnHSpaceEqually();
    afx_msg void OnVSpaceEqually();
    afx_msg void OnEqualWidth();
    afx_msg void OnEqualHeight();

    afx_msg void OnMoveToFront();
    afx_msg void OnMoveToBack();

    afx_msg void OnSizeToFit();
    afx_msg void OnZoom();
    afx_msg void OnGridSetting();

    afx_msg void OnConfigureScreenFlow();
    afx_msg void OnConfigureDisplays();
    afx_msg void OnConfigureLanguages();
    afx_msg void OnConfigureThemes();
    afx_msg void OnConfigureGuixRepository();
    afx_msg void OnUndo();

    afx_msg void OnOpenHistoryProject(UINT nID);

    afx_msg void OnGenerateAll();
    afx_msg void OnGenResources();
    afx_msg void OnGenApplication();

    afx_msg void OnRunApplication();

    afx_msg void OnRunStringFitCheck();

    afx_msg void OnLockUlockWidgetPositions();

    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
public:
    virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
    afx_msg void OnRecordmacro();
    afx_msg void OnPlaybackmacro();
    afx_msg void OnEndmacro();
    afx_msg void OnStopMacro();
    afx_msg LRESULT OnSetLayout(WPARAM wParam, LPARAM lParam);
    BOOL IsTestMode();
    afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
    afx_msg void OnUnInitMenuPopup(CMenu *pPopupMenu, UINT nFlags);
    afx_msg void OnClose();
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);


public:
    void DisableMenus();
    void EnableMenus();
    void DisableMenuItem(CMenu *menu, CString string);
    void EnableMenuItem(CMenu *menu, CString string);
    void DisableMenuItem(CMenu *menu, INT id);
    void EnableMenuItem(CMenu *menu, INT id);
    void UpdateRecentProjectsMenu();

private:
    void TestOneWidgetStringsFit(int display_index, widget_info* info);
    void TestStringsFit(int display_index, widget_info* info);
    CString m_strings_fit_msg;
    int m_text_scaler;
};



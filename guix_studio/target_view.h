
#pragma once

#include <afxcview.h>

#include "recent_project_win.h"

class target_frame : public CScrollView
{
    public:
        target_frame();
        virtual void OnInitialUpdate();
	    virtual void OnDraw(CDC* pDC);

        void UpdateScrollSize();

    // Generated message map functions
    protected:
        BOOL PreCreateWindow(CREATESTRUCT &cs);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnKeyDown(UINT nChar, UINT Rep, UINT flags);
        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	    DECLARE_MESSAGE_MAP()

    private:
public:
    virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
};

class target_view : public CView
{
    protected:
        DECLARE_DYNCREATE(target_view)

    public:
        target_view();
        ~target_view();
        virtual void OnInitialUpdate();
	    virtual void OnDraw(CDC* pDC);

        target_screen *GetTargetScreen() {return mpTargetScreen;}
        void DisplayTarget();
        void DisplayRecentProjects();
        void InitialDisplay();
        CDialog *GetRecentDialog() { return mpRecentDialog;}

    // Generated message map functions
    protected:
        BOOL PreCreateWindow(CREATESTRUCT &cs);
        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnSetFocus(CWnd* pOldWnd);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
	    DECLARE_MESSAGE_MAP()

    private:

        target_frame *mpTargetFrame;

        express_dialog *mpRecentDialog;
        target_screen *mpTargetScreen;
        CToolBar *mpToolBar;
};
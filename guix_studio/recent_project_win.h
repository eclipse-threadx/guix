#pragma once
#include "express_dialog.h"

#define MAX_RECENT_PROJECTS 5
#define RECENT_WINDOW_WIDTH 700
#define RECENT_WINDOW_HEIGHT 220

class recent_list_frame : public CWnd
{
    public:
        recent_list_frame(CWnd* parent = NULL);
        ~recent_list_frame();
        void UpdateRecentList();

    private:
        DECLARE_MESSAGE_MAP()
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        afx_msg void OnSetFocus(CWnd* pOldWnd);
        afx_msg void OnKillFocus(CWnd* pNewWnd);
        int FindStatic(CPoint point);
        int GetRowHeight();

    private:
        CStatic mRecentList[MAX_RECENT_PROJECTS];
        int     mHighlightRow;
        CBrush  mHiBrush;
        CBrush  mBgBrush;
        CBrush  mGrayBrush; 
        int     mNumRecentProjects;
};

class recent_project_win : public express_dialog
{
    public:
        recent_project_win(int width, int height, CWnd* pParent = NULL);   // standard constructor
        virtual ~recent_project_win();
        void UpdateRecentList();
        static CRect GetCreateNewProjectButtonSize(CRect& parentSize);

    protected:
        DECLARE_MESSAGE_MAP()
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnShowWindow(BOOL, UINT);
        afx_msg void OnBnClickedCreateProject();
        virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
        recent_list_frame    mRecentListFrame;
        custom_bitmap_button new_button;
};

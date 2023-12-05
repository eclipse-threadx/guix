
#ifndef _LEFT_PANEL_FRAME_
#define _LEFT_PANEL_FRAME_

#include <afxcview.h>

class left_top_panel_frame : public CWnd
{
    public:
        left_top_panel_frame();
        ~left_top_panel_frame();
        void SetControlSize();
        virtual void PostNcDestroy();

    protected:
    	DECLARE_DYNCREATE(left_top_panel_frame);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    	DECLARE_MESSAGE_MAP()

        void PositionChildren();

    private:
        view_header *mp_project_header;
        project_view *mp_project_view;
        int m_header_height;
};

class left_bottom_panel_frame : public CWnd
{
public:
    left_bottom_panel_frame();
    ~left_bottom_panel_frame();
    void SetControlSize();
    virtual void PostNcDestroy();

protected:
    DECLARE_DYNCREATE(left_bottom_panel_frame);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    DECLARE_MESSAGE_MAP()

    void PositionChildren();

private:
    view_header* mp_props_header;
    properties_win* mp_properties_win;
    int m_header_height;
};

#endif

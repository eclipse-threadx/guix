
#ifndef _RIGHT_PANEL_FRAME_
#define _RIGHT_PANEL_FRAME_

#include <afxcview.h>

class right_panel_frame : public CWnd
{
    public:
        right_panel_frame();
        ~right_panel_frame();
        virtual void PostNcDestroy();

        //virtual void OnDraw(CDC *pDC);

    protected:
        DECLARE_DYNCREATE(right_panel_frame);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        DECLARE_MESSAGE_MAP()

    private:
        view_header *mp_resource_header;
        resource_view *mp_resource_view;
};

#endif

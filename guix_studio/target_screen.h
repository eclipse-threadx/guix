
#ifndef _TARGET_SCREEN_
#define _TARGET_SCREEN_

#include <afxcview.h>

///////////////////////////////////////////////////////////////////////////////
#define ID_ADD_TEMPLATE_BASE  20000
#define MAX_TEMPLATES           100

typedef struct {
    widget_info *info;
    void (*draw_func)(GX_WIDGET *);
} WIDGET_SELECT_ENTRY;

class target_screen : public CWnd
{
    public:
        target_screen();
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

        void UpdateWindowPos();

        void SwitchTopWidget(widget_info *info);
        BOOL ConfigureDisplay(int display_index, BOOL bForce = FALSE);
        void OnCloseProject(void);
        void MakeTopWindowVisible(widget_info *info);

        void OnAddWidget(int type);
        void OnAddTemplate(widget_info *base);

        BOOL SelectWidget(widget_info *select, BOOL echo_to_proj_view, BOOL multiple);
        BOOL SelectWidget(GX_POINT &point, BOOL multiple = FALSE);
        void WidgetWasMoved(widget_info *info);
        void CopySelectedWidgets();
        void ShiftSelectedWidgets(int x, int y);
        widget_info *GetSingleSelect(void);
        static BOOL SelectedWidgetsCanBeMoved();

        static USHORT GetWidgetParentType(widget_info *info);
        static BOOL IsWidgetResizable(USHORT parent_type, widget_info *info, GX_RECTANGLE &new_size);

        void RepositionListChildren(widget_info *info);

        void TestDropItem(resource_item *item, CPoint spot, BOOL shifted);
        void DeSelect();

        void OnAlignLeft();
        void OnAlignTop();
        void OnAlignRight();
        void OnAlignBottom();
        void OnHSpaceEqually();
        void OnVSpaceEqually();
        void OnMakeEqualWidth();
        void OnMakeEqualHeight();
        void OnAutosize();
        void ZoomIn(int scale);
        int GetZoomScale(){ return mZoomScale; };
        void SetZoomScale(int scale){ mZoomScale = scale; };
        BOOL OnAltKeyDown();

    private:
        void InvalidateCrosshairArea();
        void InvalidateSnapLineArea();
        void UpdateSnapLines(int delta_x, int delta_y);
        int  GetSnapLineVal(GX_RECTANGLE &size, int snap_line_type);
        BOOL IsVerticalSnapLineType(int snap_line_type);
        void TestAddSnapLine(SNAPLINE_INFO &snapline);
        void ClearSnapLine(int snapline_direction);
        BOOL Snap2Widget(GX_RECTANGLE &src_size, GX_RECTANGLE &target_size, int src_snap_line_type, int direction, int &snap_dist);
        int  CalSnap2GridDelta(int value, int &delta);
        BOOL CalSnap2WidgetDelta(GX_WIDGET *selected, int &delta, int direction, int snap_dist, int src_snap_line_type = -1);
        void CalSnapDelta(int &delta_x, int &delta_y, int snap_dist);
        int ConvertRadialProgressDragMode(int drag_mode, GX_RECTANGLE rect, GX_POINT point);
        void UpdateRadialProgressSize(GX_RECTANGLE *size, int mDragMode, int delta_x, int delta_y);
        void UpdateWidgetSize(GX_RECTANGLE *size, int mDrawMode, int delta_x, int delta_y);

    // Generated message map functions
    protected:

	    afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint pos);

        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);

        afx_msg LRESULT OnGuixReady(WPARAM wp, LPARAM lp);
        afx_msg LRESULT OnGuixDirty(WPARAM wp, LPARAM lp);
        afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

	    DECLARE_MESSAGE_MAP()

        BOOL IsWidgetSelected(GX_WIDGET *widget);
        GX_WIDGET *FindSelectedWidget(GX_POINT point);

        int CheckResizeCursor(CPoint point);
        int FindNearestSelected(int start_index, BOOL vertical);

        void PositionPopupList(widget_info *info, GX_DROP_LIST *list);

        void crc32_init(void);
        unsigned int calc_crc(unsigned char *data, int len);
        void canvas_to_bitmap(CString filename, GX_CANVAS *canvas);
        void target_screen_to_bitmap(CString filename, GX_CANVAS *canvas);

        GX_WIDGET *mpSelectedParent;
        int       mDragMode;
        int       m_display_index;
        BOOL      mFoldUndo;
        GX_POINT  mDragStart;
        GX_WIDGET *mpDragSelected;
        unsigned int crc_table[256];
        int       mZoomScale;
        GRID_SNAP_INFO mGridSnapInfo;

};

#endif

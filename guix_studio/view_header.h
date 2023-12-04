
#include <afxcview.h>

#define VIEW_HEADER_HEIGHT 28

class view_header : public CWnd
{
    public:
        view_header(char *title, int icon_id);

    protected:
        void FillHeader(CDC *dc, int y, int xstart, int xend);
        void PaintIcon(CDC *dc, int x, int y);
        void PaintRightIcon(CDC *dc, int icon_id, CRect &size);
        

        afx_msg void OnPaint();
    	DECLARE_MESSAGE_MAP()
        CString m_title;

        int m_icon_id;
        int m_icon_height;
        int m_icon_width;
        int m_icon_src_width;
        int m_icon_src_height;
        CBitmap m_left_bmp;
};

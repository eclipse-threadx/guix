
#ifndef _WAIT_DIALOG_
#define _WAIT_DIALOG_


class wait_dialog : public CDialog
{
    public:
        DECLARE_DYNAMIC(wait_dialog)
	    wait_dialog(int width, int height, const char *msg, CWnd *parent);   // standard constructor
	    virtual ~wait_dialog();

        enum { IDD = IDD_DIALOG_TEMPLATE };

    public:
        void SetWorkThread(HANDLE thread) { m_work_thread = thread; }

    protected:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnPaint();
        afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
        afx_msg LRESULT OnDialogVisible(WPARAM wParam, LPARAM lParam);
    	DECLARE_MESSAGE_MAP()

        void PaintTitleBar(CDC *dc);

        CStatic wait_msg;
        CString mMessage;
        int mIconId;
        int m_width;
        int m_height;
        HANDLE m_work_thread;
};

#endif

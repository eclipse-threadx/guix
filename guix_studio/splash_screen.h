
#ifndef _SPLASH_SCREEN_
#define _SPLASH_SCREEN_

#include <afxcview.h>

#define DATE_STRING_SIZE 128

class splash_screen : public CDialog
{
    public:
        splash_screen(BOOL AutoClose);
        virtual BOOL PreCreateWindow(CREATESTRUCT &cs);

    // Generated message map functions
    protected:

        char       date_built[DATE_STRING_SIZE];

        CButton CloseButton;

        void ScreenReaderMessage();
	    afx_msg void OnPaint();
        afx_msg void OnTimer(UINT_PTR nIdEvent);
        afx_msg void OnDoneClicked();
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    DECLARE_MESSAGE_MAP()
        BOOL mAutoClose;
};

#endif

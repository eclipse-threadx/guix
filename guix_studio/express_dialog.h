#ifndef _EXPRESS_DIALOG_
#define _EXPRESS_DIALOG_

#define TITLE_BAR_HEIGHT 28
#define STATUS_BAR_HEIGHT 41

#define CLOSE_BUTTON_WIDTH 25
#define CLOSE_BUTTON_HEIGHT 21

#define SAVE_BUTTON_WIDTH 80
#define SAVE_BUTTON_HEIGHT 36

#define CANCEL_BUTTON_WIDTH 75
#define CANCEL_BUTTON_HEIGHT 30

class custom_bitmap_button : public CBitmapButton
{
    public:
        custom_bitmap_button();
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    private:
        void custom_bitmap_button::DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

class express_dialog : public CDialog
{
    public:
        express_dialog(int width, int height);   // standard constructor
        express_dialog(int template_id, CWnd *parent);   // constructor if it will be modal
        virtual ~express_dialog();
        void SetControlSize();
        void AddSaveButton(CString title = _T("Save"));
        void AddCancelButton(CString title = _T("Cancel"));
        void SetSavedMsg(CString msg) { mSavedMsg = msg; }
        virtual INT_PTR DoModal();
        void ChangeDialogFontSize();
        void CreateDialogFont(int text_scaler);

    protected:
        DECLARE_MESSAGE_MAP()
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg LRESULT OnNcHitTest(CPoint point);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        virtual void OnOK();

        void PaintRequiredAsterisk(int control_id);
        void PaintTitleAndStatusBar(CDC *dc);
        void SetTitleText(CString &title);
        void SetTitleText(char *text);
        CStatic button_text;
        custom_bitmap_button close_button;
        custom_bitmap_button save_button;
        CButton cancel_button;
        
        int IconId;
        int m_width;
        int m_height;
        int m_title_bar_height;
        int m_status_bar_height;
        int m_close_button_width;
        int m_close_button_height;
        int m_save_button_width;
        int m_save_button_height;
        int m_cancel_button_width;
        int m_cancel_button_height;
        int m_template_id;
        int m_text_scaler;
        CFont m_dialog_font;

    protected:
        CString mTitleText;
        CString mSavedMsg;
};

#endif

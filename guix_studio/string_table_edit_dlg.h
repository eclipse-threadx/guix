#pragma once
#include "afxwin.h"

// string_table_edit_dlg dialog

class string_reference_row : public CWnd
{
public:
    string_reference_row(CWnd *parent, widget_info *info, int string_id);
    ~string_reference_row();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

public:
    GX_RECTANGLE GetWidgetPreviewSize(){ return m_preview_size; };
    CString GetInfoText(){ return mInfoText; };
    void MakeWidgetPreview();

protected:
    widget_info *mpInfo;
    int          mStringId;
    GX_BMP_INFO  m_bitmap;
    GX_WIDGET *mpWidget;
    GX_CANVAS   *mpCanvas;
    GX_WINDOW_ROOT *mpRoot;
    CString      mInfoText;

    int m_text_height;
    GX_RECTANGLE m_preview_size;
};

class string_reference_win : public CWnd
{
public:
    string_reference_win(CWnd *parent, string_table *table, int string_index);
    ~string_reference_win();

public:
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
    void CreateStringReferenceRows(widget_info *info, int string_id);
    void CreateStringReferenceRows();
    void PositionChildren();
    void CreateDisplay();
    void UpdateResourceTable();
    void SetActiveLanguage(int language);
    void StringSelected(int string_index);
    GX_DISPLAY *GetDisplay(){ return mpDisplay; };
    CFont *GetDialogFont();

protected:
    int mStringIndex;
    string_table *mpTable;
    CScrollHelper *m_scroll_helper;
    GX_DISPLAY  *mpDisplay;

    int m_text_height;
};

//class table_frame;  // forward reference

class table_frame : public CWnd
{
    public:
        table_frame(CWnd *parent, string_table *table);
        ~table_frame();

        void RowSelected(int column, int row_id, bool assign_focus=FALSE);
        void SelectedVisible();
        int GetSelectedId() { return mSelectedRow; }
        int GetRowCount() { return mRowCount; };

        void AddString();
        void DeleteString(int row_id);

        void ToggleThreeColumnMode();
        BOOL IncrementTransLanguage();
        BOOL DecrementTransLanguage();
        BOOL IsThreeColumnMode() { return mThreeColumnMode;}
        int GetTransLanguage();
        void SetSearchString(CString &search);
        void ImportString(CString pathname);
        CFont *GetUniFont();

        void SortStringTable();
        void SelectFirstRow();
        void InvalidateRow(int row_id);
        int GetStringIndex(int row_id);

    protected:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnDestroy(void);
        afx_msg void OnPaint();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnSetFocus(CWnd* pOldWnd);
        afx_msg void OnKillFocus(CWnd* pNewWnd);
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        void PositionChildren();
        void CreateTableRows();
        int CountFilterRows();
        void GetRowName(int row_id, CString& name);
        CWnd *GetRow(int row_id);

        BOOL IncludeInSearch(int RowId);
	    DECLARE_MESSAGE_MAP()

    private:
        CScrollHelper *m_scroll_helper;
        string_table *mpTable;
        int mSelectedRow;
        int mTransLanguage;
        int mRowHeight;
        int mRowCount;
        BOOL mThreeColumnMode;
        CString mSearchString;
};

class table_row : public CWnd
{
    public:
        table_row(CWnd *parent, int Id, int string_index, string_table *table);
        afx_msg void OnPaint();
        afx_msg void OnSetFocus(CWnd *Wnd);
        afx_msg void OnLButtonDown(UINT di, CPoint cp);
        void SetTransLanguage(int language_index);
        int GetRowHeight();
        int GetRowId() {return mRowId;}
        int GetStringIndex() { return mStringIndex; }
        
    private:
        void PaintHeaderRow(CDC *dc, CRect &size);
        void PaintStringRow(CDC *dc, CRect &size);

        int mRowId;
        int mStringIndex;
        int mRowHeight;
        string_table *mpTable;
        table_frame  *mpFrame;
	    DECLARE_MESSAGE_MAP()
};

class table_header : public CWnd
{
    public:
        table_header(table_frame *frame, string_table *table);
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT di, CPoint cp);
        void SetTransLanguage(int language_index);
        int GetIconWidth() { return m_icon_width; }
        
    private:
        void PaintHeader(CDC *dc, CRect &size);
        string_table *mpTable;
        table_frame *mpFrame;
        int m_icon_width;
        DECLARE_MESSAGE_MAP()
};

class button_frame : public CWnd
{
    public:
        button_frame(string_table *table);
        ~button_frame();

        void UpdateControlDimensions(int text_scaler);

    protected:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnSize(UINT nType, int cx, int cy);

        afx_msg void OnAddString();
        afx_msg void OnDeleteClicked();
        afx_msg void OnSearchButton();
        afx_msg void OnSearchEdit();
        afx_msg void OnImportString();
        afx_msg void OnExportString();
        afx_msg void OnSortStringTable();
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        virtual BOOL PreTranslateMessage(MSG* pMsg);

        DECLARE_MESSAGE_MAP()
    
    private:
        void PositionChildren();
        string_table *mpTable;
        CButton mAddButton;
        CButton mDeleteButton;
        //CButton mSortById;
        //CButton mSortByString;
        CStatic   mSortLabel;
        express_combobox mSortCombobox;
        CButton mImportButton;
        CButton mExportButton;
        CEdit mSearchString;
        CButton mSearchButton;
        CBitmap mSearchBitmap;

        int m_button_width;
        int m_button_height;
};

class string_table_edit_dlg : public express_dialog
{
	DECLARE_DYNAMIC(string_table_edit_dlg)

    public:
	    string_table_edit_dlg(string_table *table, CWnd* pParent = NULL);   // standard constructor
	    virtual ~string_table_edit_dlg();
        void SetControlDimensions(int text_scaler);
        BOOL StringSelected(int row_id, BOOL bIdColumn);
        BOOL SaveStringInfo();
        void UpdateEditFieldContent();
        void SetThreeColumnMode(BOOL bOnOff);
        void Exit(int exit_code);
        table_frame *GetTableFrame() {return mpTableFrame;}
        string_reference_win *GetStrReferenceWin(){ return mpStrReferenceWin; };
        CFont *GetUniFont(){ return &mUniFont; };
        CFont* GetDialogFont() { return &m_dialog_font; }
        afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);
        void InvalideTableHeader() { mpTableHeader->Invalidate(); }
        void UpdateSearchMessage();

    // Dialog Data
	    enum { IDD = IDD_STRING_TABLE_EDIT
        };

    protected:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
        afx_msg void OnStringEdit();
        afx_msg void OnIdEdit();

        afx_msg void OnChangeFont();
        afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
        afx_msg void OnOK();
        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        virtual INT_PTR DoModal();
        afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    private:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        void PositionChildren();
        void UpdateStringWidth(GX_RESOURCE_ID font_id, CString &str);
        void UpdateStringFields();

        void SetRichEditReadingDirection(CRichEditCtrl* edit, INT format);

        string_reference_win *mpStrReferenceWin;
        CStatic mStrReferenceLabel;

        button_frame *mpButtonFrame;
        table_header *mpTableHeader;
        table_frame *mpTableFrame;
        view_header *mpWinHeader;
        string_table *mpTable;
        int mRowId;
        BOOL    mThreeColumnMode;
        string_table_record m_record;

        CButton mOkButton;
        CStatic mIdLabel;
        CStatic mTopEditLabel;
        CStatic mNumRefLabel;
        CStatic mNumReferences;
        CStatic mWidthLabel;
        CStatic mWidth;
        CStatic mFontLabel;
        CStatic mNotesLabel;
        CEdit   mIdEdit;
        CBrush  *mNotesBrush;
        CBrush  *mViewEditBrush;
        express_combobox mFontCombo;
        CStatic mRequiredIndicationLable;

        CRichEditCtrl   mTopStringEdit;
        CRichEditCtrl   mBottomStringEdit;
        LOGFONT lf;
        CFont   mUniFont;
        CEdit   mNotesEdit;
        int m_button_bar_height;
        int m_table_header_height;
        int m_string_info_column_width;
        int m_info_field_width;
        int m_info_field_space;
        int m_min_string_dialog_width;
        int m_lable_height;
        int m_lable_width;
        int m_sys_dpi;
};

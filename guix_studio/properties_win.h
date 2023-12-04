
#ifndef _PROPERTIES_WIN_
#define _PROPERTIES_WIN_

#include <afxcview.h>

class properties_win : public CDialog
{
    public:
        properties_win();
        ~properties_win();
        void OnWidgetSelect(widget_info *info);
        virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
        void WidgetWasMoved(void);

        void WidgetWasModified(widget_info *info);
        void WidgetWasModified(GX_WIDGET *widget);

        void ResourcesChanged();

        // functions shared by properties window and Undo manager:
        static void SetChildAllocation(widget_info *info, int allocation_type);
        static void AssignProgressBarInfo(widget_info *wi);
        void SendEditFocusLoseMessage();
        void SetControlSize();

    protected:
        void AssignChildFont();
        void DestroyChildren(void);
        CButton *AddCheckBox(char *description, ULONG checked, int id);
        void AddEditControl(char *description, int value, int id);
        void AddEditControl(char *description, CString &name, int id);
        void AddRichEditControl(char *description, CString &name, int id);
        void AddPromptControl(char *description, CString &name);
        CButton *AddRadioButton(char *description, ULONG checked, int id);
        void AddPushButton(char *description, int id);

        CButton *CreateGroup(char *caption, int rows);
        void AddComboBox(char *label, STRING_VAL_PAIR *list, int id, ULONG current_val);
        void AddComboBox(char *label, int res_type, int id, ULONG current_val);

        void AddWidgetProps(void);
        void AddButtonStyles(widget_info *info);

        void AddTextWidgetProperties(widget_info *info);
        void AddTextCopyProperty(widget_info *info);
        void AddNumericPromptProps(widget_info *info);

        void AddPixelmapButtonProps(widget_info *info);
        void AddCheckboxProps(widget_info *info);
        void AddRadioButtonProps(widget_info *info);
        void AddIconButtonProps(widget_info *info);
        void AddIconProps(widget_info *info);
        void AddPixelmapPromptProps(widget_info *info);

        void AddTextInputProperties(widget_info *info);
        void AddMLTextViewProperties(widget_info *info);
        void AddMLTextInputProperties(widget_info *info);
        void AddRichTextViewProperties(widget_info *info);

        void AddSliderProps(widget_info *info);
        void AddPixelmapSliderProps(widget_info *info);
        void AddProgressBarProps(widget_info *info);
        void AddRadialProgressBarProps(widget_info *info);
        void AddSpriteProps(widget_info *info);

        void AddRadialSliderProps(widget_info *info);

        void AddWindowProps(widget_info *info);
        void AddVerticalListProps(widget_info *info);
        void AddHorizontalListProps(widget_info *info);
        void AddDropListProps(widget_info *info);
        void AddScrollbarProps(widget_info *info);

        void AddGaugeProps(widget_info *info);
        void AddChartProps(widget_info *info);

        void AddScrollWheelProps(widget_info *info);
        void AddTextScrollWheelProps(widget_info *info);
        void AddStringScrollWheelProps(widget_info *info);
        void AddNumericScrollWheelProps(widget_info *info);
        void AddGenericScrollWheelProps(widget_info *info);

        void AddMenuProps(widget_info *info);
        void AddTreeViewProps(widget_info *info);

        void AddStringSelect(int current_val);
        
        int m_top_pos;
        int m_group_pos;

        void OnTabKey();
        void OnPageUp();
        void OnPageDown();
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        afx_msg int OnCreate(LPCREATESTRUCT lpcs);
        afx_msg BOOL OnEraseBkgnd(CDC *pDC);
        afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);

        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnPaint();
        afx_msg void OnSysChar(UINT nChar, UINT RepCnt,UINT nFlags);

        afx_msg void OnChangeWidgetName();
        BOOL CheckChangeWidgetId(CString new_name);
        afx_msg void OnChangeWidgetId();
        afx_msg void OnChangeDrawFunction();
        afx_msg void OnChangeEventFunction();
        afx_msg void OnChangeWidgetUserData();

        afx_msg void OnChangeBorder();
        afx_msg void OnChangeColor();
        afx_msg void OnChangeWidgetPos();
        afx_msg void OnChangeTileWallpaper();

        afx_msg void OnChangeNumericPrompt();

        afx_msg void OnChangeDynamicWidget();
        afx_msg void OnChangeDynamicBuffer();


        afx_msg void OnChangeTextViewLineSpace();
        afx_msg void OnChangeTextViewWhitespace();
        afx_msg void OnChangeTextBufferSize();

        afx_msg void OnChangePixelmap0();
        afx_msg void OnChangePixelmap1();
        afx_msg void OnChangePixelmap2();
        afx_msg void OnChangePixelmap3();
        afx_msg void OnChangePixelmap4();
        afx_msg void OnChangePixelmap5();
        afx_msg void OnChangePixelmap6();
        afx_msg void OnChangePixelmap7();

        afx_msg void OnTextAlignmentChange();
        afx_msg void OnFontChange(int index);
        afx_msg void OnFontChange0();
        afx_msg void OnFontChange1();
        afx_msg void OnFontChange2();
        afx_msg void OnFontChange3();
        afx_msg void OnChangeNormalTextColor();
        afx_msg void OnChangeSelectedTextColor();
        afx_msg void OnChangeDisabledTextColor();
        afx_msg void OnChangeReadonlyFillColor();
        afx_msg void OnChangeReadonlyTextColor();
        afx_msg void OnChangePixmapAlign();
        afx_msg void OnButtonStyleChange();
        afx_msg void OnSliderStyleChange();
        afx_msg void OnDropListStyleChange();

        afx_msg void OnProgressInfoChange();
        afx_msg void OnChangeRadialProgressBarInfo();
        afx_msg void OnChangeChartInfo();

        afx_msg void OnChangeRadialSliderInfo();

        afx_msg void OnSpriteStyleChange();
        afx_msg void OnEditSpriteFrames();

        afx_msg void OnChangeSliderInfo();

        afx_msg void OnChangeListWrap();
        afx_msg void OnChangeListRepeatSelect();

        afx_msg void OnChangeListRows();
        afx_msg void OnChangeOpenHeight();

        afx_msg void OnScrollAppearanceChange();
        afx_msg void OnScrollStyleChange();
        
        afx_msg void OnChangeTransparent();
        afx_msg void OnChangeSelected();
        afx_msg void OnChangeEnabled();
        afx_msg void OnChangeTextCopy();
        afx_msg void OnChangeMLTextVCenter();
        afx_msg void OnChangeFocus();
        afx_msg void OnChangeReadonly();

        afx_msg void OnChangeStringId();
        afx_msg void OnChangeStringText();
        afx_msg void OnChangeCallbackName();

        afx_msg void OnChangeGaugeInfo();

        afx_msg void OnChangeScrollWheelProps();
        afx_msg void OnChangeTextScrollWheelProps();
        afx_msg void OnChangeStringScrollWheelProps();
        afx_msg void OnEditStringScrolWheelStrings();
        afx_msg void OnChangeNumericScrollWheelRange();

        afx_msg void OnChangeTemplate();
        afx_msg void OnChangeVisibleAtStartup();

        afx_msg void OnChangeMenuInfo();
        afx_msg void OnChangeTreeViewInfo();

        afx_msg void OnEditRichTextViewText();
        afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

        afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);// add to receive test message from py.

        DECLARE_MESSAGE_MAP()

    private:
        void OnPixelmapChange(int index);
        void ValidateTemplateName(CString &name);
        INT  ValidateAlignStyle(INT style);

        widget_info   *mpInfo;
        studiox_project *mpProject;
        CScrollHelper *m_scroll_helper;
        int            m_item_height;
        int            m_leftcol_width;
        int            m_row_height;
};

#endif

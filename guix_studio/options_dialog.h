#ifndef _OPTIONS_DIALOG_
#define _OPTIONS_DIALOG_

// options_dialog dialog

#define MAX_OPTION_COUNT 5

class options_dialog : public express_dialog
{
	DECLARE_DYNAMIC(options_dialog)

public:
    options_dialog(CString title, CString msg, STRING_VAL_PAIR *option_list, CWnd* pParent = NULL);   // standard constructor
    virtual ~options_dialog();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    int GetSelectedOption(){ return m_selected_option; };

// Dialog Data
    enum { IDD = IDD_OPTIONS_DLG };

private:
    STRING_VAL_PAIR *mp_option_list;
    CButton m_option_buttons[MAX_OPTION_COUNT];
    CStatic m_message_box;
    CString m_message;
    int m_option_count;
    int m_selected_option;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void PositionChildren();
    afx_msg void OnOptionClicked(UINT nID);
    LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};
#endif
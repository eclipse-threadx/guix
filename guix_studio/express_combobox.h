#ifndef _EXPRESS_COMBOBOX_
#define _EXPRESS_COMBOBOX_
class express_combobox : public CComboBox
{
public:
    express_combobox();
    ~express_combobox();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif

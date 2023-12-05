// sprite_edit_dialog.cpp : implementation file
//

#include "studiox_includes.h"
#include "stdafx.h"
#include "sprite_edit_dialog.h"
#include "afxdialogex.h"
#include "import_sprite_frames_dlg.h"

#define MAX_SPRITE_FRAMES     256
#define FRAME_INFO_NOT_EQUAL  65535

#define READ_FRAME_FIELD(id, str, val)     \
            GetDlgItemText(id, str);       \
            if (str.IsEmpty())             \
            {                              \
                val = FRAME_INFO_NOT_EQUAL;\
            }

#define SET_FRAME_FIELD(id, value)             \
            if (value != FRAME_INFO_NOT_EQUAL) \
            {                                  \
                SetDlgItemInt(id, value);      \
            }                                  \
            else                               \
            {                                  \
                GetDlgItem(id)->SetWindowText(L"");\
            }

#define SET_VAL(target, source)                \
            if (source != FRAME_INFO_NOT_EQUAL)\
            {                                  \
                target = source;               \
            }

#define CHECK_EQUAL(target, source)            \
            if (target != source)              \
            {                                  \
                target = FRAME_INFO_NOT_EQUAL; \
            }

enum sprite_edit_dlg_test_commands {
    TEST_EDIT_FRAME = 1,
    TEST_EDIT_NUM_FRAMES,
    TEST_SET_ALPHA,
    TEST_SET_XOFFSET,
    TEST_SET_YOFFSET,
    TEST_SET_DELAY,
    TEST_CHECK_APPLY_TO_ALL_FRAMES,
    TEST_IMPORT_FRAMES,
    TEST_SAVE,
    TEST_CANCEL
};


BEGIN_MESSAGE_MAP(sprite_edit_dialog, express_dialog)
    ON_WM_CREATE()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUM_FRAMES, &sprite_edit_dialog::OnDeltaposSpinNumFrames)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EDIT_FRAME, &sprite_edit_dialog::OnDeltaposSpinEditFrame)
    ON_EN_KILLFOCUS(IDC_NUM_FRAMES, &sprite_edit_dialog::OnEnKillfocusNumFrames)
    ON_EN_KILLFOCUS(IDC_EDIT_FRAME, &sprite_edit_dialog::OnEnKillfocusEditFrame)
    ON_EN_KILLFOCUS(IDC_ALPHA_VALUE, &sprite_edit_dialog::OnEnKillfocusAlphaValue)
    ON_EN_KILLFOCUS(IDC_X_OFFSET, &sprite_edit_dialog::OnEnKillfocusXOffset)
    ON_EN_KILLFOCUS(IDC_Y_OFFSET, &sprite_edit_dialog::OnEnKillfocusYOffset)
    ON_EN_KILLFOCUS(IDC_DELAY, &sprite_edit_dialog::OnEnKillfocusDelay)
    ON_BN_CLICKED(IDC_APPLY_TO_ALL_FRAMES, OnClickApplyToAllFrames)
    ON_BN_CLICKED(IDC_IMPORT_FRAMES, OnImportFrames)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
END_MESSAGE_MAP()


// sprite_edit_dialog dialog

///////////////////////////////////////////////////////////////////////////////
sprite_edit_dialog::sprite_edit_dialog(widget_info *info, CWnd* pParent /*=NULL*/)
	: express_dialog(sprite_edit_dialog::IDD, pParent)
{
    int index;

    IconId = IDB_PROPS_VIEW;
    SetTitleText("Edit Sprite Frames");

    mpInfo = info;
    mTempInfo = *info;

    // allcate MAX_FRAMES, so we don't have to keep resizing the array
    // every time the user adds or removes frames.
    GX_SPRITE_FRAME *old_frames = mTempInfo.ewi.sprite.framelist;
    mTempInfo.ewi.sprite.framelist = new GX_SPRITE_FRAME[MAX_SPRITE_FRAMES];
    memset(mTempInfo.ewi.sprite.framelist, 0, MAX_SPRITE_FRAMES * sizeof(GX_SPRITE_FRAME));
    
    for (index = 0; index < mTempInfo.ewi.sprite.frame_count; index++)
    {
        mTempInfo.ewi.sprite.framelist[index] = old_frames[index];
    }
    if (mTempInfo.ewi.sprite.frame_count == 0)
    {
        mTempInfo.ewi.sprite.frame_count = 1;
    }
    delete [] old_frames;
    mEditFrame = 0;
}

///////////////////////////////////////////////////////////////////////////////
sprite_edit_dialog::~sprite_edit_dialog()
{
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnOK()
{
    // copy my temp info to widget info
    ReadFrameFields();

    express_dialog::OnOK();

    GX_SPRITE_FRAME *old_frames = mpInfo->ewi.sprite.framelist;
    mpInfo->ewi.sprite.framelist = mTempInfo.ewi.sprite.framelist;
    mTempInfo.ewi.sprite.framelist = NULL;
    delete [] old_frames;
    mpInfo->ewi.sprite.frame_count = mTempInfo.ewi.sprite.frame_count;
    mTempInfo.ewi.sprite.frame_count = 0;
    mpInfo->ewi.sprite.apply_to_all_frames = mTempInfo.ewi.sprite.apply_to_all_frames;
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::SelectDropListItem(CComboBox* box, long val)
{
    int box_index;

    for (box_index = 0; box_index < box->GetCount(); box_index++)
    {
        if (box->GetItemData(box_index) == val)
        {
            box->SetCurSel(box_index);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::SetFrameFields()
{
    int alpha = mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_alpha;
    int xoffset = mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_x_offset;
    int yoffset = mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_y_offset;
    int delay = mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_delay;
    int bg_operation_sel = mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_background_operation;

    if (mTempInfo.ewi.sprite.apply_to_all_frames)
    {
        for (int index = 0; index < mTempInfo.ewi.sprite.frame_count; index++)
        {
            if (index == mEditFrame)
            {
                continue;
            }
            CHECK_EQUAL(alpha, mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_alpha);
            CHECK_EQUAL(xoffset, mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_x_offset);
            CHECK_EQUAL(yoffset, mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_y_offset);
            CHECK_EQUAL(delay, mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_delay);
            CHECK_EQUAL(bg_operation_sel, mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_background_operation);
        }
    }

    SET_FRAME_FIELD(IDC_ALPHA_VALUE, alpha);
    SET_FRAME_FIELD(IDC_X_OFFSET, xoffset);
    SET_FRAME_FIELD(IDC_X_OFFSET, xoffset);
    SET_FRAME_FIELD(IDC_Y_OFFSET, yoffset);
    SET_FRAME_FIELD(IDC_DELAY, delay);

    SelectDropListItem(&mPixelmapCombo, mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_pixelmap);

    CString diff_operation(L"<different operations>");
    int index = mBackgroundCombo.FindString(0, diff_operation);

    if (bg_operation_sel != FRAME_INFO_NOT_EQUAL)
    {
        if (index >= 0)
        {
            mBackgroundCombo.DeleteString(index);
        }
    }
    else
    {
        if (index < 0)
        {
            int sel = mBackgroundCombo.InsertString(-1, diff_operation);
            mBackgroundCombo.SetItemData(sel, FRAME_INFO_NOT_EQUAL);
        }
    }

    SelectDropListItem(&mBackgroundCombo, bg_operation_sel);
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::ReadFrameFields()
{
    if (mTempInfo.ewi.sprite.framelist)
    {
        int alpha = GetDlgItemInt(IDC_ALPHA_VALUE);
        int xoffset = GetDlgItemInt(IDC_X_OFFSET);
        int yoffset = GetDlgItemInt(IDC_Y_OFFSET);
        int delay = GetDlgItemInt(IDC_DELAY);
        int cursel = mBackgroundCombo.GetCurSel();
        int background_operation = mBackgroundCombo.GetItemData(cursel);
        CString str;
        int index;
        int frame_count;
       
        if (mTempInfo.ewi.sprite.apply_to_all_frames)
        {
            READ_FRAME_FIELD(IDC_ALPHA_VALUE, str, alpha);
            READ_FRAME_FIELD(IDC_X_OFFSET, str, xoffset);
            READ_FRAME_FIELD(IDC_Y_OFFSET, str, yoffset);
            READ_FRAME_FIELD(IDC_DELAY, str, delay);

            index = 0;
            frame_count = mTempInfo.ewi.sprite.frame_count;
        }
        else
        {
            cursel = mPixelmapCombo.GetCurSel();
            mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_pixelmap = mPixelmapCombo.GetItemData(cursel);

            index = mEditFrame;
            frame_count = index + 1;
        }

        while (index < frame_count)
        {
            SET_VAL(mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_alpha, alpha);
            SET_VAL(mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_x_offset, xoffset);
            SET_VAL(mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_y_offset, yoffset);
            SET_VAL(mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_delay, delay);
            SET_VAL(mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_background_operation, background_operation);

            index++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SPIN_NUM_FRAMES, mSpinNumFrames);
    DDX_Control(pDX, IDC_SPIN_EDIT_FRAME, mSpinEditFrame);
    DDX_Control(pDX, IDC_NUM_FRAMES, mNumFramesField);
    DDX_Control(pDX, IDC_EDIT_FRAME, mEditFrameField);
    DDX_Control(pDX, IDC_BACKGROUND_COMBO, mBackgroundCombo);
    DDX_Control(pDX, IDC_PIXELMAP_COMBO, mPixelmapCombo);
    DDX_Control(pDX, IDC_X_OFFSET, mXOffsetEdit);
    DDX_Control(pDX, IDC_Y_OFFSET, mYOffsetEdit);

    if (pDX->m_bSaveAndValidate)
    {
        ReadFrameFields();
    }
    else
    {
        mXOffsetEdit.EnableNumberCheck();
        mXOffsetEdit.EnableNegativeValue();

        mYOffsetEdit.EnableNumberCheck();
        mYOffsetEdit.EnableNegativeValue();

        mSpinNumFrames.SetRange(1, MAX_SPRITE_FRAMES);
        mSpinNumFrames.SetPos32(mTempInfo.ewi.sprite.frame_count);
        mSpinEditFrame.SetRange(1, mTempInfo.ewi.sprite.frame_count);
        mSpinEditFrame.SetPos32(1);
        SetDlgItemInt(IDC_NUM_FRAMES, mTempInfo.ewi.sprite.frame_count);
        SetDlgItemInt(IDC_EDIT_FRAME, 1);

        CString resname;

        int index;
        index = mBackgroundCombo.AddString(_T("No Operation"));
        mBackgroundCombo.SetItemData(index, 0);
        
        index = mBackgroundCombo.AddString(_T("Restore first pixelmap"));
        mBackgroundCombo.SetItemData(index, 1);

        index = mBackgroundCombo.AddString(_T("Solid Color Fill"));
        mBackgroundCombo.SetItemData(index, 2);


        mBackgroundCombo.ShowWindow(SW_SHOW);

        int pixelmap_id = 1;
        mPixelmapCombo.AddString(_T("None"));
        studiox_project* project = GetOpenProject();

        if (!project)
        {
            return;
        }

        int active_display = GetProjectView()->GetActiveDisplay();
        int active_theme = project->mDisplays[active_display].active_theme;

        for (pixelmap_id = 1; pixelmap_id < project->CountResources(active_display, RES_TYPE_PIXELMAP); pixelmap_id++)
        {
            project->GetResourceName(active_display, RES_TYPE_PIXELMAP, pixelmap_id, resname);

            if (!resname.IsEmpty())
            {
                index = mPixelmapCombo.AddString(resname);
                mPixelmapCombo.SetItemData(index, pixelmap_id);
            }
        }

        if (mpInfo->pixelmap_id[NORMAL_PIXELMAP_INDEX])
        {
        	// Disable frame count and pixelmap edit.
            GetDlgItem(IDC_NUM_FRAMES)->EnableWindow(FALSE);
            GetDlgItem(IDC_SPIN_NUM_FRAMES)->EnableWindow(FALSE);
            GetDlgItem(IDC_PIXELMAP_COMBO)->EnableWindow(FALSE);
            GetDlgItem(IDC_IMPORT_FRAMES)->EnableWindow(FALSE);
        }

        if (mTempInfo.ewi.sprite.apply_to_all_frames)
        {
            ((CButton*)GetDlgItem(IDC_APPLY_TO_ALL_FRAMES))->SetCheck(TRUE);
            GetDlgItem(IDC_EDIT_FRAME)->EnableWindow(FALSE);
            GetDlgItem(IDC_PIXELMAP_COMBO)->EnableWindow(FALSE);
        }

        SetFrameFields();
    }
}


// sprite_edit_dialog message handlers

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnDeltaposSpinNumFrames(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here

    int oldval = pNMUpDown ->iPos;
    int newval = oldval + pNMUpDown->iDelta;
    int current_val;

    if (oldval <= MAX_SPRITE_FRAMES && oldval >= 1)
    {
        if (newval <= 0)
        {
            newval = 1;
        }
        else
        {
            if (newval > MAX_SPRITE_FRAMES)
            {
                newval = MAX_SPRITE_FRAMES;
            }
        }
        SetDlgItemInt(IDC_NUM_FRAMES, newval);
        mTempInfo.ewi.sprite.frame_count = newval;

        if (newval < oldval)
        {
            current_val = mSpinEditFrame.GetPos32();

            if (mSpinEditFrame.GetPos32() > newval)
            {
                mSpinEditFrame.SetPos32(newval);
                SetDlgItemInt(IDC_EDIT_FRAME, newval);
                mEditFrame = newval - 1;
                SetFrameFields();
            }
        }
        else
        {
            // clear out the new entry
            memset(&mTempInfo.ewi.sprite.framelist[newval - 1], 0, sizeof(GX_SPRITE_FRAME));
            mTempInfo.ewi.sprite.framelist[newval - 1].gx_sprite_frame_alpha = 255;
            mTempInfo.ewi.sprite.framelist[newval - 1].gx_sprite_frame_delay = 5;
        }
        mSpinEditFrame.SetRange(1, newval);
        mSpinEditFrame.Invalidate();
    }

    *pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnDeltaposSpinEditFrame(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int upper;
    int lower;

    mSpinEditFrame.GetRange(lower, upper);

    int oldval = pNMUpDown ->iPos;
    int newval = oldval + pNMUpDown->iDelta;

    // save the current name, update to new name, if the value is within 
    // limits
    if (oldval <= upper && oldval >= lower &&
        newval <= upper && newval >= lower)
    {

        // Read and save current frame values
        ReadFrameFields();
        SetDlgItemInt(IDC_EDIT_FRAME, newval);

        // switch to new frame
        mEditFrame = newval - 1;
        SetFrameFields();
    }

    *pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusNumFrames()
{
    int newval = GetDlgItemInt(IDC_NUM_FRAMES);

    int oldval = mTempInfo.ewi.sprite.frame_count;

    if ((newval >= 1) && (newval <= MAX_SPRITE_FRAMES))
    {

        mTempInfo.ewi.sprite.frame_count = newval;

        if (newval < oldval)
        {
            if (mEditFrame > newval)
            {
                mSpinEditFrame.SetPos32(newval);
                SetDlgItemInt(IDC_EDIT_FRAME, newval);
                mEditFrame = newval - 1;
                SetFrameFields();
            }
        }
        else
        {
            // clear out the new entry
            for (int index = oldval; index < newval; index++)
            {
                memset(&mTempInfo.ewi.sprite.framelist[index], 0, sizeof(GX_SPRITE_FRAME));
                mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_alpha = 255;
                mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_delay = 5;
            }
        }
        mSpinEditFrame.SetRange(1, newval);
        mSpinEditFrame.Invalidate();
    }
    else
    {
        SetDlgItemInt(IDC_NUM_FRAMES, oldval);

        char msg[255];
        sprintf_s(msg, sizeof(msg), "Invalid value for Number of Frames. Valid range is from 1 to %d.", MAX_SPRITE_FRAMES);
        ErrorMsg(msg, this);
        GetDlgItem(IDC_NUM_FRAMES)->SetFocus();
    }
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusEditFrame()
{
    int newval = GetDlgItemInt(IDC_EDIT_FRAME);

    int max_frame_id = GetDlgItemInt(IDC_NUM_FRAMES);

    if ((newval >= 1) && (newval <= max_frame_id))
    {
        // Read and save current frame values
        ReadFrameFields();

        // switch to new frame
        mEditFrame = newval - 1;
        SetFrameFields();
    }
    else
    {
        SetDlgItemInt(IDC_EDIT_FRAME, mEditFrame + 1);
        ErrorMsg("Selected frame must be between 1 and the total number of frames", this);
        GetDlgItem(IDC_EDIT_FRAME)->SetFocus();
    }
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusAlphaValue()
{
    // TODO: Add your control notification handler code here
    int val = GetDlgItemInt(IDC_ALPHA_VALUE);
    if (val < 0 || val > 255)
    {
        SetDlgItemInt(IDC_ALPHA_VALUE, mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_alpha);
    }
}


///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusXOffset()
{
    // TODO: Add your control notification handler code here
    int val = GetDlgItemInt(IDC_X_OFFSET);
    if (val < -65535 || val > 65535)
    {
        SetDlgItemInt(IDC_X_OFFSET, mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_x_offset);
    }
}


///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusYOffset()
{
    // TODO: Add your control notification handler code here
    int val = GetDlgItemInt(IDC_Y_OFFSET);
    if (val < -65535 || val > 65535)
    {
        SetDlgItemInt(IDC_Y_OFFSET, mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_y_offset);
    }
}


///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnEnKillfocusDelay()
{
    // TODO: Add your control notification handler code here
    int val = GetDlgItemInt(IDC_DELAY);
    if (val < 0 || val > 999)
    {
        SetDlgItemInt(IDC_DELAY, mTempInfo.ewi.sprite.framelist[mEditFrame].gx_sprite_frame_delay);
    }
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnClickApplyToAllFrames()
{
    ReadFrameFields();

    if (((CButton*)GetDlgItem(IDC_APPLY_TO_ALL_FRAMES))->GetCheck() == BST_CHECKED)
    {
        mTempInfo.ewi.sprite.apply_to_all_frames = TRUE;
        GetDlgItem(IDC_EDIT_FRAME)->EnableWindow(FALSE);
        GetDlgItem(IDC_PIXELMAP_COMBO)->EnableWindow(FALSE);
    }
    else
    {
        mTempInfo.ewi.sprite.apply_to_all_frames = FALSE;
        GetDlgItem(IDC_EDIT_FRAME)->EnableWindow(TRUE);
        GetDlgItem(IDC_PIXELMAP_COMBO)->EnableWindow(TRUE);
    }

    SetFrameFields();
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::SetFrameCount(int frame_count)
{
    if (frame_count <= 0)
    {
        return;
    }

    if (frame_count > MAX_SPRITE_FRAMES)
    {
        frame_count = MAX_SPRITE_FRAMES;
    }

    int oldval = mTempInfo.ewi.sprite.frame_count;

    SetDlgItemInt(IDC_NUM_FRAMES, frame_count);
    mTempInfo.ewi.sprite.frame_count = frame_count;

    if (frame_count < oldval)
    {
        if (mSpinEditFrame.GetPos32() > frame_count)
        {
            mSpinEditFrame.SetPos32(frame_count);
            SetDlgItemInt(IDC_EDIT_FRAME, frame_count);
            mEditFrame = frame_count - 1;
            SetFrameFields();
        }
    }
    else
    {
        for (int index = oldval; index < frame_count; index++)
        {
            // clear out the new entry
            memset(&mTempInfo.ewi.sprite.framelist[index], 0, sizeof(GX_SPRITE_FRAME));
            mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_alpha = 255;
            mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_delay = 5;
        }
    }
    mSpinEditFrame.SetRange(1, frame_count);
    mSpinEditFrame.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
void sprite_edit_dialog::OnImportFrames()
{
    import_sprite_frames_dlg dlg(this);

    if (dlg.DoModal() == IDOK)
    {
        CArray<CString> *name_list = dlg.GetFrameNameList();
        if (!name_list || name_list->IsEmpty())
        {
            return;
        }

        int count = name_list->GetCount();

        if (count)
        {
            if (count > MAX_SPRITE_FRAMES)
            {
                count = MAX_SPRITE_FRAMES;
            }

            SetFrameCount(count);

            studiox_project* project = GetOpenProject();
            int active_display = GetProjectView()->GetActiveDisplay();

            CString name;

            for(int index = 0; index < count; index++)
            {
                name = name_list->GetAt(index);

                mTempInfo.ewi.sprite.framelist[index].gx_sprite_frame_pixelmap = project->GetResourceId(active_display, RES_TYPE_PIXELMAP, name);

                if (index == mEditFrame)
                {
                    mPixelmapCombo.SelectString(0, name);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL sprite_edit_dialog::OnInitDialog()
{
    express_dialog::OnInitDialog();

    // TODO:  Add extra initialization here
    AddCancelButton();
    AddSaveButton();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL sprite_edit_dialog::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    CWnd* focus_owner = GetFocus();

    if (focus_owner)
    {
        int ctrl_id = focus_owner->GetDlgCtrlID();

        switch (ctrl_id)
        {
        case IDC_NUM_FRAMES:
        case IDC_EDIT_FRAME:
        case IDC_ALPHA_VALUE:
        case IDC_DELAY:
            if ((pMsg->message == WM_CHAR) && isprint(pMsg->wParam))
            {
                if (pMsg->wParam < '0' || pMsg->wParam > '9')
                {
                    ErrorMsg("Unacceptable Character. You can only type a number here.", this);
                    return TRUE;
                }
            }
            break;
        }
    }

    return express_dialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////////
LRESULT sprite_edit_dialog::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd* pWnd;
    CString string;

    switch (wParam)
    {
    case TEST_EDIT_FRAME:
        pWnd = GetDlgItem(IDC_EDIT_FRAME);
        if (pWnd)
        {
            CStringArray param;
            CString id;


            SplitString(GetTestingParam(0), ',', &param);
            id = param.GetAt(0);
            string = param.GetAt(1);

            pWnd->SetWindowText(id);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_EDIT_FRAME, EN_KILLFOCUS), (LPARAM)((CEdit*)pWnd)->m_hWnd);

            
            pWnd = GetDlgItem(IDC_PIXELMAP_COMBO);
            ((CComboBox*)pWnd)->SelectString(-1, string);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_PIXELMAP_COMBO, CBN_SELCHANGE), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        }
        break;

    case TEST_EDIT_NUM_FRAMES:
        pWnd = GetDlgItem(IDC_NUM_FRAMES);
        string.Format(_T("%d"), lParam);
        pWnd->SetWindowText(string);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_NUM_FRAMES, EN_KILLFOCUS), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        break;

    case TEST_SET_ALPHA:
        pWnd = GetDlgItem(IDC_ALPHA_VALUE);
        string.Format(_T("%d"), lParam);
        pWnd->SetWindowText(string);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ALPHA_VALUE, EN_KILLFOCUS), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        break;

    case TEST_SET_XOFFSET:
        pWnd = GetDlgItem(IDC_X_OFFSET);
        string.Format(_T("%d"), lParam);
        pWnd->SetWindowText(string);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_X_OFFSET, EN_KILLFOCUS), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        break;

    case TEST_SET_YOFFSET:
        pWnd = GetDlgItem(IDC_Y_OFFSET);
        string.Format(_T("%d"), lParam);
        pWnd->SetWindowText(string);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_Y_OFFSET, EN_KILLFOCUS), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        break;

    case TEST_SET_DELAY:
        pWnd = GetDlgItem(IDC_DELAY);
        string.Format(_T("%d"), lParam);
        pWnd->SetWindowText(string);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DELAY, EN_KILLFOCUS), (LPARAM)((CComboBox*)pWnd)->m_hWnd);
        break;

    case TEST_CHECK_APPLY_TO_ALL_FRAMES:
    {
        CButton *button = (CButton *)GetDlgItem(IDC_APPLY_TO_ALL_FRAMES);
        button->SetCheck(lParam);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_APPLY_TO_ALL_FRAMES, BN_CLICKED), (LPARAM)button->m_hWnd);
    }
        break;

    case TEST_IMPORT_FRAMES:
        OnImportFrames();
        break;

    case TEST_SAVE:
        OnOK();
        break;

    case TEST_CANCEL:
        OnCancel();
        break;
    }

    return 0;
}


#pragma once

// color_edit_dialog dialog
#include "studiox_includes.h"
#include "afxwin.h"
#include "afxcmn.h"

class color_edit_dialog;

class color_gradient_win : public CWnd
{
public:
    color_gradient_win(CWnd* pParent = NULL);
    ~color_gradient_win() {};

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

    void PaintColorGradient(CDC *pDC);
    void PaintColorGradientHighlight (CDC* pDC);
    void PaintColorPalette(CDC *pDC);
    void PaintColorPaletteHighlight(CDC* pDC);

protected:
    void GetColorGradientDimension(int *rows, int *cols);
    void GetSelectedPos(int *row, int *col);
    void SetSelectedPos(int row, int col);
    void InvalidateSelection(int row, int col);

protected:
    int m_swatch_size;
    studiox_project *mpProject;
    color_edit_dialog *mpParent;
};

class color_edit_dialog : public express_dialog
{

public:
	color_edit_dialog(resource_item *res, int display_index, CWnd* pParent = NULL);   // standard constructor
    color_edit_dialog(int display_index, CWnd *pParent = NULL);

	virtual ~color_edit_dialog();

    virtual void OnOK();

    GX_COLOR GetColor();
    CString GetName();

    static void RgbToHsl(int red, int green, int blue, int &hue, int &saturation, int &luminance);
    static void HslToRgb(double hue, double sat, double lum, int &red, int &green, int &blue);
    palette_info *GetPalette() { return &mPalette; }
    void GetHSLColor(int *hue, int *sat, int *lum);
    void SetHSLColor(int hue, int sat);
    void SetPaletteColor(int index);
    int  GetPaletteColor() { return mColorIndex; }

// Dialog Data
	enum { IDD = IDD_COLOR_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
    void UpdateHSLFields();
    void UpdateRGBVals();
    void UpdateRGBFields();

    void PaintColorPreview();
    BOOL IsAlphaFormat(int color_format);
    void AnnounceColorUpdate(CString msg);

public:
    afx_msg void OnEnChangeEditRed();
    afx_msg void OnEnChangeEditGreen();
    afx_msg void OnEnChangeEditBlue();
    afx_msg void OnEnChangeEditAlpha();

    afx_msg void OnEnChangeEditHue();
    afx_msg void OnEnChangeEditSaturation();
    afx_msg void OnEnChangeEditLuminance();

    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL OnInitDialog();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *bar);
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);


    virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
    CString mName;

    int     mRed;
    int     mGreen;
    int     mBlue;
    int     mAlpha;
    int     mHue;
    int     mSat;
    int     mLum;
    int     mDisplayIndex;
    int     mResId;
    int     mColorIndex;
    palette_info mPalette;
    studiox_project *mpProject;
    res_info *mpRes;

protected:
    CEdit mRedEdit;
    CEdit mGreenEdit;
    CEdit mBlueEdit;
    CEdit mAlphaEdit;
    CEdit mHueEdit;
    CEdit mSaturationEdit;
    CEdit mLuminanceEdit;
    CSliderCtrl mRedSlider;
    CSliderCtrl mGreenSlider;
    CSliderCtrl mBlueSlider;
    CSliderCtrl mAlphaSlider;
    CSliderCtrl mHueSlider;
    CSliderCtrl mSaturationSlider;
    CSliderCtrl mLuminanceSlider;
    color_gradient_win *mpColorGradientWin;
    CStatic mColorUpdateMsg;
};

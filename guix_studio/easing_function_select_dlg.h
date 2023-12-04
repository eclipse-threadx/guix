#ifndef _EASING_FUNCTION_SELECT_DLG_
#define _EASING_FUNCTION_SELECT_DLG_
#include "afxwin.h"
// easing_function_select_dlg dialog

class easing_function_select_dlg : public express_dialog
{
    DECLARE_DYNAMIC(easing_function_select_dlg)

public:
    easing_function_select_dlg(int easing_function_id, CWnd* pParent = NULL);   // standard constructor
    virtual ~easing_function_select_dlg();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    static int GetEasingFuncId(CString type_name);
    static CString GetEasingFuncIdName(int type);

    int GetEasingFuncId(){ return mEasingFuncId; };

    enum { IDD = IDD_EASING_FUNCTION_SELECT_DLG };

public:
    afx_msg void OnSelecteEasingFuncCombo();
    afx_msg void OnAnimationStart();
    afx_msg LRESULT OnTestMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    static float BackEaseIn(float t, float d);
    static float BackEaseOut(float t, float d);
    static float BackEaseInOut(float t, float d);
    static float BounceEaseIn(float t, float d);
    static float BounceEaseOut(float t, float d);
    static float BounceEaseInOut(float t, float d);
    static float CircEaseIn(float t, float d);
    static float CircEaseOut(float t, float d);
    static float CircEaseInOut(float t, float d);
    static float CubicEaseIn(float t, float d);
    static float CubicEaseOut(float t, float d);
    static float CubicEaseInOut(float t, float d);
    static float ElasticEaseIn(float t, float d);
    static float ElasticEaseOut(float t, float d);
    static float ElasticEaseInOut(float t, float d);
    static float ExpoEaseIn(float t, float d);
    static float ExpoEaseOut(float t, float d);
    static float ExpoEaseInOut(float t, float d);
    static float QuadEaseInt(float t, float d);
    static float QuadEaseOut(float t, float d);
    static float QuadEaseInOut(float t, float d);
    static float QuartEaseIn(float t, float d);
    static float QuartEaseOut(float t, float d);
    static float QuartEaseInOut(float t, float d);
    static float QuintEaseIn(float t, float d);
    static float QuintEaseOut(float t, float d);
    static float QuintEaseInOut(float t, float d);
    static float SinEaseIn(float t, float d);
    static float SinEaseOut(float t, float d);
    static float SinEaseInOut(float t, float d);
    float(*mpEasingFuncCallback)(float time, float duration);
    void DrawEasingCurve(Graphics &g);

protected:
    express_combobox mEasingFuncCombo;
    int       mEasingFuncId;
    CRect     mWaveRect;
    CRect     mAnimationRect;
    int       mAnimationStep;
    CStatic   mAnimationStatus;
    CSize     mAnimationBoxSize;
    int       mAnimationDotRadius;
};
#endif

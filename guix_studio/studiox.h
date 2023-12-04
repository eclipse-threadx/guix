// studiox.h : main header file for the studiox application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CstudioxApp:
// See studiox.cpp for the implementation of this class
//

class CstudioxApp : public CWinApp
{
public:
	CstudioxApp();


// Overrides
public:
    void DeleteSystemFonts();
    void CreateSystemFonts();
	virtual BOOL InitInstance();
    virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
    afx_msg void OnUsersGuide();
    afx_msg void OnProjectNew();
    afx_msg void OnProjectOpen();
    afx_msg void OnProjectSave();
    afx_msg void OnProjectSaveAs();
    afx_msg void OnProjectClose();
    afx_msg void OnProjectMerge();
	DECLARE_MESSAGE_MAP()

private:
    CFrameWnd *CreateMainFrame();
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};

extern CstudioxApp theApp;
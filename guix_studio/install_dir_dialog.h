#pragma once

// clone repo dialog
#include "studiox_includes.h"
#include "afxwin.h"
#include "afxcmn.h"


class install_dir_dialog : public express_dialog
{
	DECLARE_DYNAMIC(install_dir_dialog)

public:
	install_dir_dialog(CWnd *parent);
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SELECT_INSTALL_DIR };

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOK();
	afx_msg void OnSetDirectory();



private:
	void ConfigureDefaultSamplesDir(void);
	void CopyInstalledObjects();

};

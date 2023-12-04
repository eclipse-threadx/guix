
#ifndef _ACCESSIBILITY_PROPS_SERVICER
#define _ACCESSIBILITY_PROPS_SERVICER

// Create accessibility property service
void CreateAccPropService();

// Release accessibility property service
void ReleaseAccPropService();

// Run when the UI is created.
void SetControlAccessibleName(HWND ctr_handle, LPCWSTR name);

// Annotate role property
void SetControlAccessibleRole(HWND ctr_handle, LONG role);

// Set accessible help string for a control
void SetAccessibleHelpString(HWND ctr_handle, LPCWSTR help_string);

// Set a lable to be a LiveRegion
void SetLiveRegion(HWND ctr_handle);

// Set accessible full description for a control
void SetAccessibleFullDescription(HWND ctr_handle, LPCWSTR string);

// Set accessible description string for a control
void SetAccessibleDescription(HWND ctr_handle, LPCWSTR help_string);

#endif
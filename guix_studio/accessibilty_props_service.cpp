
#include "resource.h"
#include <initguid.h> 
#include "objbase.h"
#include "uiautomation.h" 
#include "accessibility_props_service.h"

static IAccPropServices* _pAccPropServices = NULL;

// Create accessibility property service
void CreateAccPropService()
{
   CoCreateInstance(
        CLSID_AccPropServices,
        nullptr,
        CLSCTX_INPROC,
        IID_PPV_ARGS(&_pAccPropServices));
}

// Release accessibility property service
void ReleaseAccPropService()
{
    if (_pAccPropServices != nullptr)
    {
        _pAccPropServices->Release();
        _pAccPropServices = NULL;
    }
}


// Run when the UI is created.
void SetControlAccessibleName(HWND ctr_handle, LPCWSTR name)
{
    if (_pAccPropServices != nullptr)
    {
        // Now set the name on the edit control. This gets exposed through UIA as the 
        // element's Name property.
        _pAccPropServices->SetHwndPropStr(
            ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            Name_Property_GUID,
            name);
    }
}

// Annotate role property
void SetControlAccessibleRole(HWND ctr_handle, LONG role)
{
    if (_pAccPropServices != nullptr)
    {
        // Annotating the Role of this object to be STATICTEXT
        VARIANT var;
        var.vt = VT_I4;
        var.lVal = role;

        _pAccPropServices->SetHwndProp(ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            PROPID_ACC_ROLE,
            var);
    }
}

// Set accessible help string for a control
void SetAccessibleHelpString(HWND ctr_handle, LPCWSTR help_string)
{
    if (_pAccPropServices != nullptr)
    {
        // Expose the help string through UIA as the element's HelpText property.
        _pAccPropServices->SetHwndPropStr(
            ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            PROPID_ACC_HELP,
            help_string);
    }
}

// Set a lable to be a LiveRegion
void SetLiveRegion(HWND ctr_handle)
{
    if (_pAccPropServices != nullptr)
    {
        // Set up the status label to be an assertive LiveRegion. The assertive property 
            // is used to request that the screen reader announces the update immediately. 
            // And alternative setting of polite requests that the screen reader completes 
            // any in-progress announcement before announcing the LiveRegion update.

        VARIANT var;
        var.vt = VT_I4;
        var.lVal = Assertive;

       _pAccPropServices->SetHwndProp(
            ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            LiveSetting_Property_GUID,
            var);
    }
}

// Set accessible full description for a control
void SetAccessibleFullDescription(HWND ctr_handle, LPCWSTR string)
{
    if (_pAccPropServices != nullptr)
    {
        // Expose a localized string which can contain extended description text for an element.
        // FullDescription can contain a more complete description of an element than maybe
        // appropriate for the element name.
        _pAccPropServices->SetHwndPropStr(
            ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            FullDescription_Property_GUID,
            string);
    }
}

// Set accessible description string for a control
void SetAccessibleDescription(HWND ctr_handle, LPCWSTR help_string)
{
    if (_pAccPropServices != nullptr)
    {
        // Expose the descriotion through UIA as the element's Description property.
        _pAccPropServices->SetHwndPropStr(
            ctr_handle,
            OBJID_CLIENT,
            CHILDID_SELF,
            PROPID_ACC_DESCRIPTION,
            help_string);
    }
}
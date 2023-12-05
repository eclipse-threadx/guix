
#include "studiox_includes.h"
#include "uiautomation.h"
#include "resource_item_provider.h"
#include "resource_view_provider.h"

///////////////////////////////////////////////////////////////////////////////
resource_view_provider::resource_view_provider(resource_view* pControl)
{
    m_refCount = 1;
    m_pResViewControl = pControl;
    m_resViewControlHwnd = pControl->GetSafeHwnd();
}

///////////////////////////////////////////////////////////////////////////////
resource_view_provider::~resource_view_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
// IUnknown implementation.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP_(ULONG) resource_view_provider::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP_(ULONG) resource_view_provider::Release()
{
    long val = InterlockedDecrement(&m_refCount);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::QueryInterface(REFIID riid, void** ppInterface)
{
    if (riid == __uuidof(IUnknown))                              *ppInterface = static_cast<IRawElementProviderSimple*>(this);
    else if (riid == __uuidof(IRawElementProviderSimple))        *ppInterface = static_cast<IRawElementProviderSimple*>(this);
    else if (riid == __uuidof(IRawElementProviderFragment))      *ppInterface = static_cast<IRawElementProviderFragment*>(this);
    else if (riid == __uuidof(IRawElementProviderFragmentRoot))  *ppInterface = static_cast<IRawElementProviderFragmentRoot*>(this);
    else if (riid == __uuidof(ISelectionProvider))               *ppInterface = static_cast<ISelectionProvider*>(this);
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    (static_cast<IUnknown*>(*ppInterface))->AddRef();
    return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// IRawElementProviderSimple implementation
//
// Implementation of IRawElementProviderSimple::get_ProviderOptions.
// Gets UI Automation provider options.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_ProviderOptions(ProviderOptions* pRetVal)
{
    *pRetVal = ProviderOptions_ServerSideProvider;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::get_PatternProvider.
// Gets the object that supports ISelectionPattern.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
    *pRetVal = NULL;
    if (patternId == UIA_SelectionPatternId)
    {
        *pRetVal = static_cast<IRawElementProviderSimple*>(this);
        AddRef();
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::get_PropertyValue.
// Gets custom properties.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
    if (propertyId == UIA_LocalizedControlTypePropertyId)
    {
        pRetVal->vt = VT_BSTR;
        pRetVal->bstrVal = SysAllocString(L"resource tree");
    }
    else if (propertyId == UIA_ControlTypePropertyId)
    {
        pRetVal->vt = VT_I4;
        pRetVal->lVal = UIA_TreeControlTypeId;
    }
    else if (propertyId == UIA_IsKeyboardFocusablePropertyId)
    {
        pRetVal->vt = VT_BOOL;
        pRetVal->boolVal = VARIANT_TRUE;
    }
    else
    {
        pRetVal->vt = VT_EMPTY;
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::get_HostRawElementProvider.
// Gets the default UI Automation provider for the host window. This provider 
// supplies many properties.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
{
    if (m_resViewControlHwnd == NULL)
    {
        return UIA_E_ELEMENTNOTAVAILABLE;
    }
    HRESULT hr = UiaHostProviderFromHwnd(m_resViewControlHwnd, pRetVal);
    return hr;
}

///////////////////////////////////////////////////////////////////////////////
// IRawElementProviderFragment implementation
//
// Implementation of IRawElementProviderFragment::Navigate.
// Enables UI Automation to locate the element in the tree.
// Navigation to the parent is handled by the host window provider.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal)
{
    resource_item* pDest = NULL;
    IRawElementProviderFragment* pFrag = NULL;

    switch (direction)
    {
    case NavigateDirection_FirstChild:
        pDest = m_pResViewControl->GetResTree()->GetRoot();

        if (pDest)
        {
            pDest = pDest->NavigateFirst();
        }
        break;

    case NavigateDirection_LastChild:
        pDest = m_pResViewControl->GetResTree()->GetRoot();

        if (pDest)
        {
            pDest = pDest->NavigateLast();
        }
        break;
    }

    if (pDest)
    {
        pFrag = pDest->GetResItemProvider();
    }

    if (pFrag != NULL)
    {
        pFrag->AddRef();
    }
    *pRetVal = pFrag;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::GetRuntimeId.
// UI Automation gets this value from the host window provider, so supply NULL here.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetRuntimeId(SAFEARRAY** pRetVal)
{
    *pRetVal = NULL;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::get_BoundingRectangle.
//
// Retrieves the screen location and size of the control. Controls hosted in
// Win32 windows can return an empty rectangle; UI Automation will
// retrieve the rectangle from the HWND provider. However, the method is
// implemented here so that it can be used by the list items to calculate
// their own bounding rectangles.
//
// UI Spy uses the bounding rectangle to draw a red border around the element.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_BoundingRectangle(UiaRect* pRetVal)
{
    RECT rect;
    m_pResViewControl->GetClientRect(&rect);
    m_pResViewControl->ClientToScreen(&rect);

    pRetVal->left = rect.left;
    pRetVal->top = rect.top;
    pRetVal->width = rect.right - rect.left;
    pRetVal->height = rect.bottom - rect.top;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::GetEmbeddedFragmentRoots.
// Retrieves other fragment roots that may be hosted in this one.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal)
{
    *pRetVal = NULL;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::SetFocus.
// Responds to the control receiving focus through a UI Automation request.
// For HWND-based controls, this is handled by the host window provider.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::SetFocus()
{
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::get_FragmentRoot.
// Retrieves the root element of this fragment.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal)
{
    *pRetVal = static_cast<IRawElementProviderFragmentRoot*>(this);
    AddRef();
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// IRawElementProviderFragmentRoot implementation
//
// Implementation of IRawElementProviderFragmentRoot::ElementProviderFromPoint.
// Retrieves the IRawElementProviderFragment interface for the item at the specified 
// point (in client coordinates).
// UI Spy uses this to determine what element is under the cursor when Ctrl is pressed.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::ElementProviderFromPoint(double x, double y, IRawElementProviderFragment** pRetVal)
{
    POINT pt;
    pt.x = (LONG)x;
    pt.y = (LONG)y;

    m_pResViewControl->ScreenToClient(&pt);
    resource_tree* pResTree = m_pResViewControl->GetResTree();
    resource_item* pRoot = pResTree->GetRoot();

    if (!pRoot)
    {
        return NULL;
    }

    resource_item* pItem = pResTree->FindItem(pRoot, pt);

    if (pItem == NULL)
    {
        return NULL;
    }

    resource_item_provider *pItemProvider = pItem->GetResItemProvider();

    if (pItemProvider != NULL)
    {
        *pRetVal = static_cast<IRawElementProviderFragment*>(pItemProvider);
        pItemProvider->AddRef();
    }
    else
    {
        *pRetVal = NULL;
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragmentRoot::GetFocus.
// Retrieves the provider for the list item that is selected when the control gets focus.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetFocus(IRawElementProviderFragment** pRetVal)
{
    *pRetVal = NULL;
    resource_item* pSelectItem = m_pResViewControl->GetCurrentItem();
    if (pSelectItem)
    {
        resource_item_provider* pItemProvider = pSelectItem->GetResItemProvider();

        if (pItemProvider != NULL)
        {
            pItemProvider->AddRef();
            *pRetVal = pItemProvider;
        }
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// ISelectionProvider implementation
//
// Implementation of ISelectionProvider::GetSelection.
// Gets the provider(s) for the items(s) selected in the list box. 
// In this case, only a single item can be selected.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::GetSelection(SAFEARRAY** pRetVal)
{
    SAFEARRAY* psa = SafeArrayCreateVector(VT_UNKNOWN, 0, 1);
    resource_item* pSelectItem = m_pResViewControl->GetCurrentItem();

    if (pSelectItem)
    {
        resource_item_provider* pItemProvider = pSelectItem->GetResItemProvider();
        if (pItemProvider != NULL)
        {
            LONG i = 0;
            SafeArrayPutElement(psa, &i, pItemProvider);
        }
    }
    *pRetVal = psa;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionProvider::get_CanSelectMultiple.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_CanSelectMultiple(BOOL* pRetVal)
{
    *pRetVal = FALSE;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionProvider::get_IsSelectionRequired.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_view_provider::get_IsSelectionRequired(BOOL* pRetVal)
{
    *pRetVal = TRUE;
    return S_OK;
}


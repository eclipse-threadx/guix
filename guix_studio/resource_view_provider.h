
#pragma once
#include "uiautomation.h" 
class resource_view_provider : public IRawElementProviderSimple,
    public IRawElementProviderFragment,
    public IRawElementProviderFragmentRoot,
    public ISelectionProvider
{
public:

    // Constructor/destructor.
    resource_view_provider(resource_view* pControl);

    // IUnknown methods
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppInterface);

    // IRawElementProviderSimple methods
    IFACEMETHODIMP get_ProviderOptions(ProviderOptions* pRetVal);
    IFACEMETHODIMP GetPatternProvider(PATTERNID iid, IUnknown** pRetVal);
    IFACEMETHODIMP GetPropertyValue(PROPERTYID idProp, VARIANT* pRetVal);
    IFACEMETHODIMP get_HostRawElementProvider(IRawElementProviderSimple** pRetVal);

    // IRawElementProviderFragment methods
    IFACEMETHODIMP Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal);
    IFACEMETHODIMP GetRuntimeId(SAFEARRAY** pRetVal);
    IFACEMETHODIMP get_BoundingRectangle(UiaRect* pRetVal);
    IFACEMETHODIMP GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal);
    IFACEMETHODIMP SetFocus();
    IFACEMETHODIMP get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal);

    // IRawElementProviderFragmenRoot methods
    IFACEMETHODIMP ElementProviderFromPoint(double x, double y, IRawElementProviderFragment** pRetVal);
    IFACEMETHODIMP GetFocus(IRawElementProviderFragment** pRetVal);

    // ISelectionProvider methods
    IFACEMETHODIMP GetSelection(SAFEARRAY** pRetVal);
    IFACEMETHODIMP get_CanSelectMultiple(BOOL* pRetVal);
    IFACEMETHODIMP get_IsSelectionRequired(BOOL* pRetVal);

private:
    virtual ~resource_view_provider();

    // Ref counter for this COM object.
    ULONG m_refCount;

    // Parent control.
    HWND m_resViewControlHwnd;
    resource_view* m_pResViewControl;
};
#pragma once
#include "uiautomation.h" 

class resource_item_provider : public IRawElementProviderSimple,
    public IRawElementProviderFragment,
    public ISelectionItemProvider,
    public IExpandCollapseProvider,
    public IScrollItemProvider
{
public:

    // Constructor / destructor
    resource_item_provider(resource_item *pControl);

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

    // ISelectionItemProvider methods
    IFACEMETHODIMP Select();
    IFACEMETHODIMP AddToSelection();
    IFACEMETHODIMP RemoveFromSelection();
    IFACEMETHODIMP get_IsSelected(BOOL* pRetVal);
    IFACEMETHODIMP get_SelectionContainer(IRawElementProviderSimple** pRetVal);

    // IExpandCollapseProvider methods
    IFACEMETHODIMP Expand();
    IFACEMETHODIMP Collapse();
    IFACEMETHODIMP get_ExpandCollapseState(ExpandCollapseState* pRetVal);

    // IScrollItemProvider methods
    IFACEMETHODIMP ScrollIntoView();

    // Various methods
    void RaiseSelectElementEvent();
    void RaiseChangeFocusEvent();
    void RaiseExpandCollapseAutomationEvent(BOOL oldValue, BOOL newValue);

    int GetCurrentPositionInSet();
    int GetCurrentSizeOfSet();
    int GetCurrentLevel();

private:
    virtual ~resource_item_provider();

    // Ref Counter for this COM object
    ULONG m_refCount;

    // Pointers to the owning item control and list control.
    resource_item *m_pResItemControl;
    resource_view *m_pResViewControl;
};

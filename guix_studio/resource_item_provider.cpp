
#include "studiox_includes.h"
#include "uiautomation.h" 
#include "resource_view_provider.h"
#include "resource_item_provider.h"

///////////////////////////////////////////////////////////////////////////////
resource_item_provider::resource_item_provider(resource_item *pControl)
{
    m_refCount = 1;
    m_pResItemControl = pControl;

    resource_tree *pTree = m_pResItemControl->GetResTree();
    m_pResViewControl = pTree->GetParent();
}

///////////////////////////////////////////////////////////////////////////////
resource_item_provider::~resource_item_provider()
{
}

///////////////////////////////////////////////////////////////////////////////
// IUnknown implementation.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP_(ULONG) resource_item_provider::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP_(ULONG) resource_item_provider::Release()
{
    long val = InterlockedDecrement(&m_refCount);
    if (val == 0)
    {
        delete this;
    }
    return val;
}

///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::QueryInterface(REFIID riid, void** ppInterface)
{
    if (riid == __uuidof(IUnknown))                              *ppInterface = static_cast<IRawElementProviderSimple*>(this);
    else if (riid == __uuidof(IRawElementProviderSimple))        *ppInterface = static_cast<IRawElementProviderSimple*>(this);
    else if (riid == __uuidof(IRawElementProviderFragment))      *ppInterface = static_cast<IRawElementProviderFragment*>(this);
    else if (riid == __uuidof(ISelectionItemProvider))           *ppInterface = static_cast<ISelectionItemProvider*>(this);
    else if (riid == __uuidof(IExpandCollapseProvider))          *ppInterface = static_cast<IExpandCollapseProvider*>(this);
    else if (riid == __uuidof(IScrollItemProvider))              *ppInterface = static_cast<IScrollItemProvider*>(this);
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
// Implementation of IRawElementProviderSimple::GetProviderOptions.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_ProviderOptions(ProviderOptions* pRetVal)
{

    *pRetVal = ProviderOptions_ServerSideProvider;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::GetPatternProvider.
// Gets the object that supports the specified pattern.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::GetPatternProvider(PATTERNID patternId, IUnknown** pRetVal)
{
    if ((patternId == UIA_SelectionItemPatternId) ||
        (patternId == UIA_ExpandCollapsePatternId) ||
        (patternId == UIA_ScrollItemPatternId))
    {
        *pRetVal = static_cast<IRawElementProviderSimple*>(this);
        AddRef();
    }
    else
    {
        *pRetVal = NULL;
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::GetPropertyValue.
// Gets custom properties.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::GetPropertyValue(PROPERTYID propertyId, VARIANT* pRetVal)
{
    switch (propertyId)
    {
    case UIA_SizeOfSetPropertyId:
        pRetVal->vt = VT_I4;
        pRetVal->lVal = GetCurrentSizeOfSet();
        break;

    case UIA_PositionInSetPropertyId:
        pRetVal->vt = VT_I4;
        pRetVal->lVal = GetCurrentPositionInSet();;
        break;

    case UIA_LevelPropertyId:
        pRetVal->vt = VT_I4;
        pRetVal->lVal = GetCurrentLevel();
        break;

    case UIA_AutomationIdPropertyId:
    {
        pRetVal->vt = VT_BSTR;

        // Convert integer to string.
        CString str;
        str.Format(L"%d", m_pResItemControl->GetId());
        pRetVal->bstrVal = SysAllocString(str);
        break;
    }

    case UIA_NamePropertyId:
        pRetVal->vt = VT_BSTR;
        pRetVal->bstrVal = SysAllocString(m_pResItemControl->GetName());
        break;

    case UIA_ControlTypePropertyId:
        pRetVal->vt = VT_I4;
        pRetVal->lVal = UIA_TreeItemControlTypeId;
        break;

    case UIA_HasKeyboardFocusPropertyId:
    {
        // HasKeyboardFocus is true if the list has focus, and this item is selected.
        resource_item* selected = m_pResViewControl->GetCurrentItem();
        BOOL hasFocus = (selected == m_pResItemControl);
        pRetVal->vt = VT_BOOL;
        pRetVal->boolVal = hasFocus ? VARIANT_TRUE : VARIANT_FALSE;
    }
    break;

    case UIA_IsExpandCollapsePatternAvailablePropertyId:
    case UIA_IsControlElementPropertyId:
    case UIA_IsContentElementPropertyId:
    case UIA_IsKeyboardFocusablePropertyId:
        pRetVal->vt = VT_BOOL;
        pRetVal->boolVal = VARIANT_TRUE;
        break;

    case UIA_ExpandCollapseExpandCollapseStatePropertyId:
    {
        ExpandCollapseState state;
        get_ExpandCollapseState(&state);
        pRetVal->vt = VT_I4;
        pRetVal->lVal = state;
    }
        break;

    case UIA_IsOffscreenPropertyId:
        pRetVal->vt = VT_BOOL;

        if (m_pResItemControl->IsVisible())
        {
            pRetVal->boolVal = VARIANT_FALSE;
        }
        else
        {
            pRetVal->boolVal = VARIANT_TRUE;
        }

        if (m_pResItemControl->mpRes)
        {
            if (m_pResItemControl->mpRes->type == RES_TYPE_FOLDER)
            {
                switch (m_pResItemControl->mpRes->folder_id)
                {
                case DEFAULT_COLOR_FOLDER:
                case CUSTOM_COLOR_FOLDER:
                case DEFAULT_FONT_FOLDER:
                case CUSTOM_FONT_FOLDER:
                    pRetVal->boolVal = VARIANT_TRUE;
                    break;
                }
            }
            else if (!m_pResItemControl->Parent())
            {
                pRetVal->boolVal = VARIANT_TRUE;
            }
        }
        break;

    case UIA_FullDescriptionPropertyId:
    case UIA_ProviderDescriptionPropertyId:
        pRetVal->vt = VT_BSTR;
        pRetVal->bstrVal = SysAllocString(m_pResItemControl->GetDescription());
        break;

    default:
        pRetVal->vt = VT_EMPTY;
        break;
    }

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderSimple::get_HostRawElementProvider.
// Gets the UI Automation provider for the host window. 
// Return NULL. because the tree view items are not directly hosted in a window.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_HostRawElementProvider(IRawElementProviderSimple** pRetVal)
{
    *pRetVal = NULL;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// IRawElementProviderFragment implementation.
//
// Implementation of IRawElementProviderFragment::Navigate.
// Enables UI Automation to locate the element in the tree.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::Navigate(NavigateDirection direction, IRawElementProviderFragment** pRetVal)
{
    IRawElementProviderFragment* pFrag = NULL;
    resource_item* pItem = NULL;

    switch (direction)
    {
    case NavigateDirection_Parent:
        pItem = m_pResItemControl->NavigateParent();

        if (pItem)
        {
            pFrag = pItem->GetResItemProvider();
        }
        else
        {
            pFrag = m_pResViewControl->GetResViewProvider();
        }
        break;

    case NavigateDirection_NextSibling:
        pItem = m_pResItemControl->NavigateNext();

        if (pItem)
        {
            pFrag = pItem->GetResItemProvider();
        }
        break;

    case NavigateDirection_FirstChild:
        pItem = m_pResItemControl->NavigateFirst();
 
        if (pItem)
        {
            pFrag = pItem->GetResItemProvider();
        }
        break;

    case NavigateDirection_LastChild:
        pItem = m_pResItemControl->NavigateLast();

        if (pItem)
        {
            pFrag = pItem->GetResItemProvider();
        }
        break;
    }
    *pRetVal = pFrag;
    if (pFrag != NULL)
    {
        pFrag->AddRef();
    }
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::GetRuntimeId.
// Gets the runtime identifier. This is an array consisting of UiaAppendRuntimeId, 
// which makes the ID unique among instances of the control, and the Automation Id.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::GetRuntimeId(SAFEARRAY** pRetVal)
{
    int id = m_pResItemControl->GetId();
    int rId[] = { UiaAppendRuntimeId, id };

    SAFEARRAY* psa = SafeArrayCreateVector(VT_I4, 0, 2);
    for (LONG i = 0; i < 2; i++)
    {
        SafeArrayPutElement(psa, &i, &(rId[i]));
    }
    *pRetVal = psa;

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::get_BoundingRectangle.
// Gets the bounding rectangle of the item, in screen coordinates.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_BoundingRectangle(UiaRect* pRetVal)
{
    CRect rect = m_pResItemControl->GetPos();

    m_pResViewControl->ClientToScreen(&rect);
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;
    SendMessage(m_pResViewControl->GetSafeHwnd(), SBM_GETSCROLLINFO, 0, (LPARAM)&si);

    int height = rect.Height();
    if ((m_pResItemControl->GetResType() == RES_TYPE_GROUP) && m_pResItemControl->IsOpen())
    {
        height -= m_pResItemControl->GetItemHeight();
    }

    pRetVal->left = rect.left;
    pRetVal->top = rect.top - si.nPos;
    pRetVal->width = rect.Width();
    pRetVal->height = height;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::GetEmbeddedFragmentRoots.
// Retrieves any fragment roots that may be hosted in this element.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::GetEmbeddedFragmentRoots(SAFEARRAY** pRetVal)
{
    *pRetVal = NULL;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::SetFocus.
// Responds to the control receiving focus through a UI Automation request.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::SetFocus()
{
    Select();
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IRawElementProviderFragment::get_FragmentRoot.
// Retrieves the root element of this fragment.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_FragmentRoot(IRawElementProviderFragmentRoot** pRetVal)
{
    IRawElementProviderFragmentRoot* pRoot = this->m_pResViewControl->GetResViewProvider();
    if (pRoot == NULL)
    {
        return E_FAIL;
    }
    pRoot->AddRef();
    *pRetVal = pRoot;
    return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// ISelectionItemProvider implementation.
//
// Implementation of ISelectionItemProvider::Select.
// Responds to a request by UI Automation to select the item.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::Select()
{
    m_pResViewControl->SetCurrentItem(m_pResItemControl);

    // Force refresh even when app doesn't have focus.
    InvalidateRect(m_pResViewControl->GetSafeHwnd(), NULL, false);
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionItemProvider::AddToSelection.
// Responds to a request by UI Automation to add the item to the selection.
// Because this is a single-selection list box, the call is equivalent to Select().
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::AddToSelection()
{
    Select();
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionItemProvider::RemoveFromSelection.
// Responds to a request by UI Automation to remove the item from the selection.
// One and only one item must always be selected, so this is not implemented.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::RemoveFromSelection()
{
    return UIA_E_INVALIDOPERATION;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionItemProvider::get_IsSelected.
// Advises whether the item is selected.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_IsSelected(BOOL* pRetVal)
{
    *pRetVal = (m_pResItemControl == m_pResViewControl->GetCurrentItem());
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of ISelectionItemProvider::get_SelectionContainer.
// Returns the UI Automation provider for the tree view control.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_SelectionContainer(
    IRawElementProviderSimple** pRetVal)
{
    IRawElementProviderSimple* pParent =
        static_cast<IRawElementProviderSimple*>(m_pResViewControl->GetResViewProvider());
    pParent->AddRef();
    *pRetVal = pParent;
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IExpandCollapseProvider::Expand.
// Hides all nodes, controls, or content that are descendants of the control.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::Expand()
{
    switch (m_pResItemControl->GetResType())
    {
    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        m_pResItemControl->Open();
        break;
    }

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IExpandCollapseProvider::Collapse.
// Displays all child nodes, controls, or content of the control.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::Collapse()
{
    switch (m_pResItemControl->GetResType())
    {
    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        m_pResItemControl->Close();
        break;
    }

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IExpandCollapseProvider::get_ExpandCollapseState.
// Gets the state, expanded or collapsed, of the control.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::get_ExpandCollapseState(ExpandCollapseState* pRetVal)
{
    switch (m_pResItemControl->GetResType())
    {
    case RES_TYPE_GROUP:
    case RES_TYPE_FOLDER:
        if (m_pResItemControl->IsOpen())
        {
            *pRetVal = ExpandCollapseState_Expanded;
        }
        else
        {
            *pRetVal = ExpandCollapseState_Collapsed;
        }
        break;

    default:
        *pRetVal = ExpandCollapseState_LeafNode;
        break;
    }

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of IScrollItemProvider::ScrollIntoView.
// Scrolls the content area of a container object in order to display the
// control within the visible region of the container.
//
///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP resource_item_provider::ScrollIntoView()
{
    m_pResViewControl->ScrollIntoView(m_pResItemControl);

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Raises an event when an element is selected.
//
///////////////////////////////////////////////////////////////////////////////
void resource_item_provider::RaiseSelectElementEvent()
{
    if (UiaClientsAreListening())
    {
        UiaRaiseAutomationEvent(this, UIA_SelectionItem_ElementSelectedEventId);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Raises an event when the focus has changed from one element to another.
//
///////////////////////////////////////////////////////////////////////////////
void resource_item_provider::RaiseChangeFocusEvent()
{
    if (UiaClientsAreListening())
    {
        UiaRaiseAutomationEvent(this, UIA_AutomationFocusChangedEventId);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Raises an event when a property is changed.
//
///////////////////////////////////////////////////////////////////////////////
void resource_item_provider::RaiseExpandCollapseAutomationEvent(BOOL oldValue, BOOL newValue)
{
    if (UiaClientsAreListening())
    {
        VARIANT oldVar;
        VARIANT newVar;
        oldVar.vt = VT_I4;
        oldVar.lVal = oldValue ? ExpandCollapseState_Expanded : ExpandCollapseState_Collapsed;

        newVar.vt = VT_I4;
        newVar.lVal = newValue ? ExpandCollapseState_Expanded : ExpandCollapseState_Collapsed;

        UiaRaiseAutomationPropertyChangedEvent(this, UIA_ExpandCollapseExpandCollapseStatePropertyId, oldVar, newVar);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Calcualte the position in the set for the element.
//
///////////////////////////////////////////////////////////////////////////////
int resource_item_provider::GetCurrentPositionInSet()
{
    int pos = 1;
    resource_item* parent;

    parent = m_pResItemControl->NavigateParent();

    if (parent)
    {
        resource_item* child;
        child = parent->NavigateFirst();

        while (child)
        {
            if (child == m_pResItemControl)
            {
                break;
            }
            child = child->NavigateNext();
            pos++;
        }
    }

    return pos;
}

///////////////////////////////////////////////////////////////////////////////
// Calculate the size of the set where the element is located.
//
///////////////////////////////////////////////////////////////////////////////
int resource_item_provider::GetCurrentSizeOfSet()
{
    int size = 0;
    resource_item* parent;

    parent = m_pResItemControl->NavigateParent();

    if (parent)
    {
        resource_item* child = parent->NavigateFirst();
        while (child)
        {
            child = child->NavigateNext();
            size++;
        }
    }

    return size;
}

///////////////////////////////////////////////////////////////////////////////
// Calculate current level for the element.
//
///////////////////////////////////////////////////////////////////////////////
int resource_item_provider::GetCurrentLevel()
{
    int level = 1;
    resource_item* parent = m_pResItemControl->NavigateParent();

    while (parent)
    {
        parent = parent->NavigateParent();
        level++;
    }

    return level;
}


 #include "studiox_includes.h"
#include "resource_tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
resource_tree::resource_tree(resource_view *parent)
{
    mpRoot = NULL;
    mParentView = parent;

    m_res_info.name = _T("Resources");
    m_res_info.type = RES_TYPE_HEADER;
    mpRoot = new resource_item(this, &m_res_info);
    mpRoot->Open();
    mpRoot->SetVisible(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
resource_tree::~resource_tree()
{
    if (mpRoot)
    {
        delete mpRoot;
    }
}



///////////////////////////////////////////////////////////////////////////////
void resource_tree::Paint(CDC *dc)
{
    mpRoot->Paint(dc);
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::InsertItem(resource_item *parent,
    res_info *info)
{
    resource_item *item = new resource_item(this, info);

    if (!parent)
    {
        parent = mpRoot;
    }

    parent->Attach(item);
    return item;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::InsertItem(resource_item *parent,
    CString &string_id, CString &string_val)
{
    resource_item *item = new resource_item(this, string_id, string_val);
    parent->Attach(item);
    return item;
}

///////////////////////////////////////////////////////////////////////////////
INT resource_tree::GetMaxWidth(resource_item* start, CDC* dc)
{
    int width;
    int max = 0;
    bool is_folder = FALSE;

    while (start)
    {
        width = start->GetWidth(dc);

        if (width > max)
        {
            max = width;
        }

        if (start->IsOpen() && start->First())
        {
            width = GetMaxWidth(start->First(), dc);

            if (width > max)
            {
                max = width;
            }
        }

        start = start->Next();
    }

    return max;
}

///////////////////////////////////////////////////////////////////////////////
int resource_tree::PositionItems(resource_item *start, int top, int indent, CDC *dc)
{
    CRect size;
 
    size.top = top;
    size.left = 0;
   
    while(start)
    {
        size.bottom = size.top + start->GetHeight() - 1;

        if (size.bottom > m_height)
        {
            m_height = size.bottom;
        }
    
        size.right = size.left + m_width - 1;

        start->SetPos(size);
        start->SetIndent(indent);
        top = size.bottom + 1;
        BOOL IsFolder = FALSE;

        if (start->mpRes)
        {
            if (start->mpRes->type == RES_TYPE_FOLDER)
            {
                IsFolder = TRUE;
            }
        }

        if (IsFolder)
        {
            if (start->IsOpen())
            {
                top = start->LayoutFolderChildren(dc, top);
            }
        }
        else
        {
            if (start->IsOpen() && start->First())
            {
                top = PositionItems(start->First(), top, indent + 1, dc);
            }
        }
        size.top = top;
        start = start->Next();        
    }
    
    return top;
}

///////////////////////////////////////////////////////////////////////////////
void resource_tree::DeleteAllItems()
{
    resource_item *start = mpRoot;
    resource_item *next;

    SetCurrentItem(NULL);

    start = start->First();

    while(start)
    {
        next = start->Next();
        start->Detach();
        delete start;
        start = next;
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_tree::DeleteItem(resource_item *item, BOOL reposition)
{
    item->Detach();
    delete item;

    if (reposition)
    {
        PositionItems();
    }
}

///////////////////////////////////////////////////////////////////////////////
int resource_tree::GetViewWidth()
{
    CRect size;
    mParentView->GetClientRect(&size);
    return size.Width();
}

///////////////////////////////////////////////////////////////////////////////
int resource_tree::PositionItems()
{
    CRect size;
    CDC *dc = mParentView->GetDC();

    resource_item *start = mpRoot;
    start = mpRoot;
    m_height = 0;
    m_width = GetMaxWidth(start, dc);

    mParentView->GetClientRect(&size);
    m_parent_right = size.right;

    if (m_width <= m_parent_right)
    {
        m_width = m_parent_right;
    }
    else
    {
        m_width += 10;
    }

    PositionItems(start, 0, 0, dc);
    mParentView->ReleaseDC(dc);

    size.right = size.left + m_width;
    mParentView->OnTreeSizeChange();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int resource_tree::GetHeight(resource_item *start)
{
    return m_height;
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_tree::DoesFolderExist(CString &find_name, int resource_id)
{
    res_info *info;

    resource_item *start = FindFolder(GetRoot(), RES_TYPE_FOLDER, resource_id);

    while (start)
    {
        info = start->mpRes;

        if ((find_name == info->name))
        {
            return TRUE;
        }

        start = start->Next();
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void resource_tree::CreateUniqueFolderName(CString &seed, int folder_id)
{
    CString out;

    int underscore = seed.ReverseFind('_');

    if (underscore > 0 && underscore > (seed.GetLength() - 3))
    {
        if (seed.GetAt(underscore + 1) >= '0' &&
            seed.GetAt(underscore + 1) <= '9')
        {
            seed = seed.Left(underscore);
        }
    }

    if (mpRoot)
    {
        if (!DoesFolderExist(seed, folder_id))
        {
            out = seed;
        }
        else
        {
            int index = 1;
            out.Format(_T("%s_%d"), seed, index);

            while (DoesFolderExist(out, folder_id))
            {
                index++;
                out.Format(_T("%s_%d"), seed, index);
            }
        }
    }

    seed = out;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::FindItem(resource_item *start, CPoint cp)
{
    resource_item *test;
    CRect rect;

    while(start)
    {
        if (start->IsVisible())
        {
            //PtInRect: a point on the right or bottom side is considered outside the rectangle.
            //increate one pixel in bottom and right side so tath a point on right and bottom side
            //can be considered inside the rectangle
            rect = start->GetPos();
            rect.InflateRect(0, 0, 1, 1);
            if (rect.PtInRect(cp))
            {
                return start;
            }

            // special check for the theme and language switch icon within folder:
            if ((start->mpRes) && (start->IsOpen()))
            {
                if ((start->mpRes->type == RES_TYPE_FOLDER) ||
                     start->mpRes->type == RES_TYPE_GROUP)
                {
                    if (start->CheckClickCommand(cp))
                    {
                        return start;
                    }
                }
            }

            if (start->First() && start->IsOpen())
            {
                test = FindItem(start->First(), cp);
                if (test)
                {   
                    return test;
                }
            }
        }
        start = start->Next();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::FindFolder(resource_item *start, int folder_type, int folder_id)
{
    resource_item *test;

    while(start)
    {
        if (start->mpRes && start->mpRes->type == folder_type && start->mpRes->folder_id == folder_id)
        {
            return start;
        }
        if (start ->First())
        {
            test = FindFolder(start->First(), folder_type, folder_id);
            if (test)
            {
                return test;
            }
        }
        start = start->Next();
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::FindItem(resource_item *start, int res_type, CString &res_name)
{
    resource_item *test;

    while(start)
    {
        if (start->mpRes && start->mpRes->type == res_type && start->mpRes->name == res_name)
        {
            return start;
        }
        if (start ->First())
        {
            test = FindItem(start->First(), res_type, res_name);
            if (test)
            {
                return test;
            }
        }
        start = start->Next();
    }
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
resource_item *resource_tree::FindFolderNamed(resource_item *start, int folder_type, int folder_id, CString &folder_name)
{
    resource_item *test;

    while (start)
    {
        if ((start->mpRes) &&
            (start->mpRes->type == folder_type) &&
            (start->mpRes->folder_id == folder_id) &&
            (start->mpRes->name == folder_name))
        {
            return start;
        }
        if (start->First())
        {
            test = FindFolderNamed(start->First(), folder_type, folder_id, folder_name);
            if (test)
            {
                return test;
            }
        }
        start = start->Next();
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_tree:: GetCurrentItem()
{
    return mParentView->GetCurrentItem();
}

///////////////////////////////////////////////////////////////////////////////
void resource_tree::SetCurrentItem(resource_item* item)
{
    mParentView->SetCurrentItem(item);
}


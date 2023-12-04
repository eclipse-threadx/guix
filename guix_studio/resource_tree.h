
#ifndef _RESOURCE_TREE_
#define _RESOURCE_TREE_

#include <afxcview.h>

class resource_view;    // forward reference

class resource_tree
{
    public:
        resource_tree(resource_view *parent);
        ~resource_tree();
        resource_item *InsertItem(resource_item *parent, res_info *info);
        resource_item *InsertItem(resource_item *parent, CString &name, CString &val);

        void DeleteAllItems();
        void DeleteItem(resource_item *item, BOOL reposition = TRUE);
        int GetHeight(resource_item *item = NULL);
        int GetWidth() { return m_width;}
        int GetViewWidth();
        INT GetMaxWidth(resource_item *start, CDC *dc);
        int PositionItems(resource_item *start, int top, int indent, CDC *dc);
        int PositionItems();

        void Paint(CDC *dc);

        BOOL DoesFolderExist(CString &find_name, int resource_id);
        void CreateUniqueFolderName(CString &seed, int resource_id);

        resource_item *FindItem(resource_item *start, CPoint cp);
        resource_item *FindItem(resource_item *start, int res_type, CString &res_name);
        resource_item *FindFolder(resource_item *start, int folder_type, int folder_id);
        resource_item *FindFolderNamed(resource_item *start, int folder_type, int folder_id, CString &folder_name);
        resource_item *GetCurrentItem();
        void SetCurrentItem(resource_item* item);

        //resource_item *GetSelectedItem() {return mpCurrentItem;}
        resource_item *GetRoot() { return mpRoot;}
        resource_view* GetParent() { return mParentView; }
        
    private:
        void PopupMenu(int MenuId, CPoint &pos);

        resource_view *mParentView;
        resource_item *mpRoot;
        res_info       m_res_info;
        int m_height;
        int m_width;
        int m_parent_right;
};

#endif


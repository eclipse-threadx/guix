

#ifndef _RESOURCE_ITEM_
#define _RESOURCE_ITEM_

#include <afxcview.h>

class resource_tree;    // forward reference
class resource_item_provider;

class resource_item
{
    public:
        resource_item(resource_tree *tree, res_info *get);
        resource_item(resource_tree *tree, CString &name, CString &string_val);

        ~resource_item();

        void Attach(resource_item *child);
        void Detach();

        BOOL IsOpen() { return m_open;}
        BOOL IsVisible() { return m_visible;}
        void Open();
        void Close();

        CRect &GetPos() {return m_size;}

        resource_item *First() {return m_first;}
        resource_item* Last();
        resource_item *Next() {return m_next;}
        resource_item *Previous();
        resource_item *SortFirst();
        resource_item *SortNext(resource_item *current);

        resource_item *Parent() {return m_parent;}

        resource_item* NavigateFirst();
        resource_item* NavigateLast();
        resource_item* NavigateNext();
        resource_item* NavigateParent();

        res_info *mpRes;
        
        void Paint(CDC *dc);
        void SetIndent(int Indent) {m_indent = Indent;}
        void SetVisible(BOOL bVisible);
        void SetIcon(int IconId, int IconIdHighlight = 0);
        void SetPos(CRect &size) {m_size = size;}
        void SetColorValue(GX_COLOR color);
        void SetColorValue(BYTE red, BYTE green, BYTE blue);
        void SetStringVal(CString &name, CString &val);

        void RebuildPreview();

        int GetItemHeight();
        int GetHeight();
        int GetWidth(CDC *dc);
        int LayoutFolderChildren(CDC *dc, int top);
        int CheckClickCommand(CPoint &click);

        CString &GetStringId() {return string_id;}
        static COLORREF MakeColorRef(GX_COLOR val);
        static int GetWin32CompatiblePixelWidth(int pixel_width, int color_format);
        COLORREF GetColorRef() {return m_boxcolor;}
        GX_COLOR GetRGBColor(int display_index = -1);
        int GetResType();

        resource_item_provider* GetResItemProvider();
        CString GetName();
        CString GetDescription();
        resource_tree* GetResTree() { return mpTree; }
        int GetId() { return m_Id; };
        resource_item* FindAddNewItem();

    private:
        void Init();

        void MakeFontPreview();
        void MakePixelmapPreview();

        //int LayoutColorGrid(int top);

        void PaintThemeHeader(CDC *dc);

        void PaintIcon(CDC *dc, int icon_id, int x, int y);
        void PaintGroupHeader(CDC *dc);
        void PaintFolderHeader(CDC *dc);

        void PaintColorGroupColumnHeader(CDC *dc);
        void PaintColorItem(CDC *dc);
        void PaintColorItemLines(CDC *dc, int offset);

        void PaintFontGroupColumnHeader(CDC *dc);
        void PaintFontItem(CDC *dc);
        void PaintFontItemLines(CDC *dc, int offset);

        void PaintPixelmapGroupColumnHeader(CDC *dc);
        void PaintPixelmapItem(CDC *dc);
        void PaintPixelmapItemLines(CDC *dc, int offset);

        void PaintStringGroupColumnHeader(CDC *dc);
        void PaintStringItem(CDC *dc);
        void PaintStringItemLines(CDC *dc, int offset);

        void PaintAddNewItem(CDC* dc);

        void PaintHighlightRect(CDC* dc, CRect *rect);
        BOOL IsSelected();
        BOOL IsAlphaFormat(int color_format);

        BOOL m_selected;
        BOOL m_visible;
        BOOL m_open;

        resource_tree *mpTree;
        resource_item *m_next;
        resource_item *m_first;
        resource_item *m_parent;

        UCHAR         *m_preview_mem;
        GX_BMP_INFO    m_bitmap;
        COLORREF       m_boxcolor;

        CString       string_id;
        CString       display_id;
        CString       string_val;

        CRect m_size;
        int m_indent;
        int m_icon_id;
        int m_icon_id_highlight;
        int m_icon_height;
        int m_icon_width;
        int m_string_arrow_icon_width;

        CRect m_up_icon_rect;
        CRect m_down_icon_rect;

        int m_sys_dpi;
        int m_item_height;
        int m_header_height;

        int m_font_preview_width;
        int m_font_pixel_width;
        int m_font_storage_width;
        int m_pixelmap_dimension_width;
        int m_pixelmap_storage_width;
        int m_thumbnail_size;
        int m_string_id_width;

        resource_item_provider* m_pResItemProvider;
        int m_Id;
};

#endif
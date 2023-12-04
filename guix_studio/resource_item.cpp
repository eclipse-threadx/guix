
#include "studiox_includes.h"
#include "resource_tree.h"
#include "resource_item_provider.h"
#include "resource_view_provider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ITEM_HEIGHT   22
#define COLOR_BOX_SPACE   4
#define COLOR_TEXT_SPACE  8
#define ICON_HORZ_SPACE 8
#define ITEM_VERT_SPACE 6
#define TREE_INDENT 16
#define FONT_PREVIEW_COLUMN_WIDTH 120
#define FONT_PIXEL_COLUMN_WIDTH 30
#define FONT_STORAGE_COLUMN_WIDTH 50

#define PIXELMAP_DIMENSION_COLUMN_WIDTH 60
#define PIXELMAP_STORAGE_COLUMN_WIDTH 60

#define STRING_ID_COLUMN_WIDTH 120

extern CFont TitleFont;
extern CFont MediumFont;
extern CFont NormalFont;
extern CFont ViewHeaderFont;

extern CString target_class_name;

///////////////////////////////////////////////////////////////////////////////
// this constructor is used for most items:
resource_item::resource_item(resource_tree *tree, res_info *get)
{
    Init();
    mpTree = tree;
    mpRes = get;

    if (mpRes->type == RES_TYPE_FOLDER)
    {
        if ((mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER) ||
            (mpRes->folder_id == DEFAULT_PIXELMAP_FOLDER))
        {
            m_open = FALSE;
        }
        else
        {
            m_open = TRUE;
        }
    }
    else
    {
        switch (mpRes->type)
        {
        case RES_TYPE_ADD_COLOR:
        case RES_TYPE_ADD_FONT:
        case RES_TYPE_ADD_PIXELMAP:
        case RES_TYPE_ADD_STRING:
            SetIcon(IDB_ADD_RESOURCE, IDB_ADD_RESOURCE_HIGHLIGHT);
            break;

        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// this constructor is used for String Table items
resource_item::resource_item(resource_tree *tree, CString &name, CString &val)
{
    Init();
    mpTree = tree;
    mpRes = NULL;
    string_id = name;
    display_id = name;
    string_val = val;
}


///////////////////////////////////////////////////////////////////////////////
resource_item::~resource_item()
{
    // must delete all my children as well as myself
    resource_item *test;

    while(m_first)
    {
        test = m_first->m_next;
        delete m_first;
        m_first = test;
    }

    if (m_preview_mem)
    {
        delete [] m_preview_mem;
    }

    if (m_pResItemProvider)
    {
        UiaDisconnectProvider(m_pResItemProvider);
        m_pResItemProvider->Release();
        m_pResItemProvider = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Init()
{
    m_first = m_next = m_parent = NULL;
    m_indent = 0;
    m_icon_id = 0;
    m_icon_id_highlight = 0;
    m_icon_height = 0;
    m_icon_width = 0;
    m_string_arrow_icon_width = 0;
    m_boxcolor = MakeColorRef(0);
    m_open = FALSE;
    m_visible = FALSE;
    m_selected = FALSE;
    m_preview_mem = NULL;
    m_size.SetRect(0, 0, 0, 0);

    m_sys_dpi = GetSystemDPI();
    int text_scaler = GetTextScaler();
    m_item_height = GetScaledValue(ITEM_HEIGHT, m_sys_dpi, text_scaler);
    m_header_height = GetScaledValue(VIEW_HEADER_HEIGHT, m_sys_dpi, text_scaler);

    m_font_preview_width = GetScaledValue(FONT_PREVIEW_COLUMN_WIDTH, m_sys_dpi, text_scaler);
    m_font_pixel_width = GetScaledValue(FONT_PIXEL_COLUMN_WIDTH, m_sys_dpi, text_scaler);
    m_font_storage_width = GetScaledValue(FONT_STORAGE_COLUMN_WIDTH, m_sys_dpi, text_scaler);
    m_pixelmap_dimension_width = GetScaledValue(PIXELMAP_DIMENSION_COLUMN_WIDTH, m_sys_dpi, text_scaler);
    m_pixelmap_storage_width = GetScaledValue(PIXELMAP_STORAGE_COLUMN_WIDTH, m_sys_dpi, text_scaler);
    m_thumbnail_size = GetScaledValue(THUMBNAIL_SIZE, m_sys_dpi, text_scaler);
    m_string_id_width = GetScaledValue(STRING_ID_COLUMN_WIDTH, m_sys_dpi, text_scaler);

    m_pResItemProvider = NULL;

    m_Id = resource_view::CreateUniqueId();
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Attach(resource_item *child)
{
    child->m_next = NULL;

    if (m_first)
    {
        resource_item *test = m_first;
        while(test->Next())
        {
            test = test->Next();
        }
        test->m_next = child;
    }
    else
    {
        m_first = child;
    }
    child ->m_parent = this;

    if (m_open && m_visible)
    {
        child->SetVisible(TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Detach()
{
    resource_item *previous;

    if (m_parent)
    {
        if (m_parent->m_first == this)
        {
            // the easy case, first child:
            m_parent->m_first = m_next;
        }
        else
        {
            previous = m_parent->m_first;
            while(previous->m_next != this)
            {
                previous = previous->m_next;
            }
            previous->m_next = m_next;
        }
    }
    m_next = NULL;
    m_parent = NULL;
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::SetVisible(BOOL bVisible)
{
    m_visible = bVisible;

    resource_item* child = First();

    if ((GetResType() >= RES_TYPE_FOLDER) && IsOpen())
    {
        while (child)
        {
            child->SetVisible(TRUE);
            child = child->Next();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Open()
{
    if (!m_open)
    {
        m_open = TRUE;

        resource_item* child = First();

        while (child)
        {
            child->SetVisible(TRUE);
            child = child->Next();
        }
        mpTree->PositionItems();

        GetResItemProvider()->RaiseExpandCollapseAutomationEvent(FALSE, TRUE);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Close()
{
    if ((mpRes->type <= RES_TYPE_FOLDER) && m_open)
    {
        m_open = FALSE;
        resource_item *child = First();

        while(child)
        {
            child->SetVisible(FALSE);
            child = child->Next();
        }
        mpTree->PositionItems();

        GetResItemProvider()->RaiseExpandCollapseAutomationEvent(TRUE, FALSE);
    }
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_item::Previous()
{
    resource_item* previous = NULL;

    if (m_parent->m_first != this)
    {
        previous = m_parent->m_first;
        while (previous->m_next != this)
        {
            previous = previous->m_next;
        }
    }

    return previous;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_item::Last()
{
    resource_item* child = First();

    while (child->Next())
    {
        child = child->Next();
    }

    return child;
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::GetWin32CompatiblePixelWidth(int pixel_width, int color_format)
{
    switch(color_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
        pixel_width += 7;   
        pixel_width /= 8;   // get bytes
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        return pixel_width * 8;

    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
        pixel_width += 3;
        pixel_width /= 4;   // get bytes
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        return pixel_width * 4;

    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
        pixel_width++;
        pixel_width /= 2;   // bytes
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        return pixel_width * 2;
        
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        break;

    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
    case GX_COLOR_FORMAT_565BGR:
        pixel_width *= 2;  // bytes
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        return pixel_width / 2;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
        pixel_width *= 3;
        pixel_width += 3;
        pixel_width &= 0xfffc;  // multiple of 4 bytes
        return pixel_width / 3;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32BGRA:
    case GX_COLOR_FORMAT_32ARGB:
        break;
    }
    return pixel_width;
}

///////////////////////////////////////////////////////////////////////////////
// grayscale palette
GX_COLOR gray_palette[16] = {
    0x00000000,
    0x00111111,
    0x00222222,
    0x00333333,
    0x00444444,
    0x00555555,
    0x00666666,
    0x00777777,
    0x00888888,
    0x00999999,
    0x00aaaaaa,
    0x00bbbbbb,
    0x00cccccc,
    0x00dddddd,
    0x00eeeeee,
    0x00ffffff
};

///////////////////////////////////////////////////////////////////////////////
void resource_item::MakeFontPreview()
{
    GX_VALUE width;
    int height;
    GX_BRUSH *brush;
    GX_RECTANGLE rect;
    GX_CANVAS temp_canvas;
    GX_DISPLAY temp_display;
    char *prev_string = NULL;
    studiox_project *project = GetOpenProject();

    if (!project || !mpRes)
    {
        return;
    }
    if (mpRes->font)
    {
#ifdef _UNICODE
        int char_count = mpRes->name.GetLength() * 6 + 1;
        char *utf8buf = new char[char_count];

        // convert Unicode to UTF8
        WideCharToMultiByte(CP_UTF8, 0, mpRes->name.GetString(), -1, utf8buf, char_count, NULL, NULL);

        prev_string = utf8buf;
#else
        prev_string = mpRes->name.GetBuffer();
#endif

        GX_STRING string;
        string.gx_string_ptr = prev_string;
        string.gx_string_length = strlen(prev_string);

        gx_system_string_width_get_ext(mpRes->font, &string, &width);

        if (!width)
        {
            // maybe this font only has the numeric characters?
            prev_string = "0123456789";
            string.gx_string_ptr = prev_string;
            string.gx_string_length = strlen(prev_string);
            gx_system_string_width_get_ext(mpRes->font, &string, &width);
        }

        height = mpRes->font->gx_font_line_height;

        // if we don't have anything to 
        if (!width || !height)
        {
            delete[] utf8buf;
            return;
        }

        // create a temporary screen and temporary
        // canvas to make a bitmap:
        int display_index = project->GetDisplayIndex(mpRes);

        if (display_index >= 0)
        {
            int color_format = project->mDisplays[display_index].colorformat;
            width = GetWin32CompatiblePixelWidth(width, color_format);

            int memsize = guix_studio_create_display(&temp_display, "preview display",
                width, height, color_format, project->mHeader.target_cpu,
                IsRenesasDave2D(project), IsDave2dFontFormat(project, display_index), gray_palette, 16, project->mHeader.palette_mode_aa_text_colors);

            if (memsize)
            {
                m_preview_mem = new UCHAR[memsize];

                gx_canvas_create(&temp_canvas, "preview",
                    &temp_display, GX_CANVAS_SIMPLE,
                    width, height, (GX_COLOR*)m_preview_mem, memsize);

                gx_utility_rectangle_define(&rect, 0, 0, width - 1, height - 1);

                gx_canvas_drawing_initiate(&temp_canvas, NULL, &rect);
                gx_context_raw_brush_define(COLOR_BLACK, COLOR_WHITE, GX_BRUSH_SOLID_FILL);
                gx_context_brush_width_set(0);
                gx_context_brush_get(&brush);
                brush->gx_brush_font = mpRes->font;

                gx_canvas_rectangle_draw(&rect);
                gx_canvas_text_draw_ext(0, 0, &string);
                gx_canvas_drawing_complete(&temp_canvas, FALSE);

                // destroy the canvas, but keep the memory pointer:
                gx_canvas_delete(&temp_canvas);

                // copy the screen bitmap info locally:
                GX_WIN32_DISPLAY_DRIVER_DATA* data = (GX_WIN32_DISPLAY_DRIVER_DATA*)temp_display.gx_display_driver_data;
                m_bitmap = data->win32_driver_bmpinfo;

                // delete the temporary screen
                guix_studio_delete_display(&temp_display);
            }
        }
#ifdef _UNICODE
        delete[] utf8buf;
#endif
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::MakePixelmapPreview()
{
    UINT width;
    int height;
    GX_RECTANGLE rect;
    GX_DISPLAY temp_display;
    GX_CANVAS  temp_canvas;
    studiox_project *project = GetOpenProject();

    GX_PIXELMAP *map;

    if (!mpRes || !project)
    {
        return;
    }

    if (mpRes->thumbnail)
    {
        map = mpRes->thumbnail;
    }
    else
    {
        map = mpRes->GetPixelmap();
    }

    if (map)
    {
        width = map->gx_pixelmap_width;
        height = map->gx_pixelmap_height;

        int display_index = project->GetDisplayIndex(mpRes);
        int color_format = project->mDisplays[display_index].colorformat;
        int active_theme = project->mDisplays[display_index].active_theme;
 
        width = GetWin32CompatiblePixelWidth(width, color_format);

        int memsize = guix_studio_create_display(&temp_display, "preview display",
                width, height, color_format,
                project->mHeader.target_cpu, IsRenesasDave2D(project), IsDave2dFontFormat(project, display_index),
                project->mDisplays[display_index].themes[active_theme].palette,
                project->mDisplays[display_index].themes[active_theme].palette_total_size,
                project->mHeader.palette_mode_aa_text_colors);

        if (memsize)
        {
            m_preview_mem = new UCHAR[memsize];

            gx_canvas_create(&temp_canvas, "preview",
                &temp_display, GX_CANVAS_SIMPLE,
                width, height, (GX_COLOR *) m_preview_mem, memsize);

            gx_utility_rectangle_define(&rect, 0, 0, width -1, height - 1);
        
            gx_canvas_drawing_initiate(&temp_canvas, NULL, &rect);
            GX_COLOR fill_color = ColorRefToGxColor(GetSysColor(COLOR_WINDOW));
            gx_context_raw_brush_define(fill_color, fill_color, GX_BRUSH_SOLID_FILL);
            gx_context_brush_width_set(0);
            gx_canvas_rectangle_draw(&rect);

            // set the fill color to medium gray, so that alphamap will be drawn to preview canvas in visible manner:
            fill_color = 0xff808080;
			gx_context_raw_brush_define(fill_color, fill_color, GX_BRUSH_SOLID_FILL);
			gx_canvas_pixelmap_draw(0, 0, map);
            gx_canvas_drawing_complete(&temp_canvas, FALSE);

            // destroy the canvas, but keep the memory pointer:
            gx_canvas_delete(&temp_canvas);

            // copy the screen bitmap info locally:
            GX_WIN32_DISPLAY_DRIVER_DATA *data = (GX_WIN32_DISPLAY_DRIVER_DATA *) temp_display.gx_display_driver_data;
            m_bitmap = data->win32_driver_bmpinfo;

            // delete the temporary screen
            guix_studio_delete_display(&temp_display);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::RebuildPreview()
{
    if (m_preview_mem)
    {
        delete [] m_preview_mem;
        m_preview_mem = NULL;
    }

    int type = RES_TYPE_STRING;

    if (mpRes)
    {
        type = mpRes->type;
    }
    switch(type)
    {
    case RES_TYPE_FONT:
        MakeFontPreview();
        break;
    
    case RES_TYPE_PIXELMAP:
        MakePixelmapPreview();
        break;

    case RES_TYPE_COLOR:
        m_boxcolor = MakeColorRef(GetRGBColor());
        break;

     default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintThemeHeader(CDC *dc)
{
    CRect boxrect;
    CFont *old_font;
    CDC dcMemory;
    CBitmap fillmap;
    CBitmap rightmap;
    fillmap.LoadBitmap(IDB_HEADER_BACKGROUND);
    BITMAP bm;
    int vspace;
    int icon_id = m_icon_id;
    int width, height;

    // go through hoops to get bitmap width:
    fillmap.GetObject(sizeof(BITMAP), &bm);
    width = bm.bmWidth;

    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&fillmap);

    int xpos = m_size.left;

    dc->SetBkColor(RGB(128, 128, 128));
    while (xpos < m_size.right)
    {
        dc->StretchBlt(xpos, m_size.top, width, m_header_height, &dcMemory, 0, 0, width, bm.bmHeight, SRCCOPY);
        xpos += width;
    }

    if (IsSelected())
    {
        //highlight selected item
        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.top += 1;
        rect.bottom -= 2;

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);

        if (m_icon_id_highlight)
        {
            icon_id = m_icon_id_highlight;
        }
    }

    boxrect = m_size;
    boxrect.bottom = boxrect.top + m_header_height;
    boxrect.left += 8;

    if (icon_id)
    {
        vspace = boxrect.bottom - boxrect.top;
        vspace -= m_icon_height;
        vspace /= 2;
        PaintIcon(dc, icon_id, boxrect.left, boxrect.top + vspace);
        boxrect.left += m_icon_width + ICON_HORZ_SPACE;
    }

    dc->SetTextColor(RGB(255, 255, 255));
    dc->SetBkColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    CString title;
    title.Format(_T("Theme: \"%s\""), mpRes->name);

    old_font = dc->SelectObject(&ViewHeaderFont);
    dc->DrawText(title, boxrect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    dc->SelectObject(old_font);

    //paint up and down icons
    CBitmap map;
    BITMAP bitmap;

    map.LoadBitmap(IDB_UP_ARROW_GRAY);
    map.GetBitmap(&bitmap);
    width = MulDiv(bitmap.bmWidth, m_sys_dpi, DEFAULT_DPI_96);
    height = MulDiv(bitmap.bmHeight, m_sys_dpi, DEFAULT_DPI_96);

    m_up_icon_rect.left = m_size.right - width - 7;
    m_up_icon_rect.top = m_size.top + 2;
    m_up_icon_rect.right = m_up_icon_rect.left + width;
    m_up_icon_rect.bottom = m_up_icon_rect.top + height;

    dcMemory.SelectObject(&map);
    dc->StretchBlt(m_up_icon_rect.left, m_up_icon_rect.top, width, height, &dcMemory, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    map.DeleteObject();
    map.LoadBitmap(IDB_DOWN_ARROW_GRAY);
    map.GetBitmap(&bitmap);

    m_down_icon_rect.left = m_up_icon_rect.left;
    m_down_icon_rect.top = m_up_icon_rect.bottom;
    m_down_icon_rect.right = m_down_icon_rect.left + width;
    m_down_icon_rect.bottom = m_down_icon_rect.top + height;

    dcMemory.SelectObject(&map);
    dc->StretchBlt(m_down_icon_rect.left, m_down_icon_rect.top, width, height, &dcMemory, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintIcon(CDC *dc, int icon_id, int x, int y)
{
    CDC dcMemory;
    CBitmap icon;
    BITMAP bmp;
    icon.LoadBitmap(icon_id);
    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(icon);
    icon.GetObject(sizeof(BITMAP), &bmp);

    dc->StretchBlt(x, y, m_icon_width, m_icon_height, &dcMemory, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintGroupHeader(CDC *dc)
{
    CRect boxrect;
    CFont *old_font;
    CDC dcMemory;
    CBitmap fillmap;
    CBitmap rightmap;
    fillmap.LoadBitmap(IDB_HEADER_BACKGROUND);
    BITMAP bm;
    int vspace;
    int icon_id = m_icon_id;
    int width, height;

    // go through hoops to get bitmap width:
    fillmap.GetObject(sizeof(BITMAP), &bm);
    width = bm.bmWidth;

    dcMemory.CreateCompatibleDC(dc);
    dcMemory.SelectObject(&fillmap);

    int xpos = m_size.left;

    while(xpos < m_size.right)
    {
        dc->StretchBlt(xpos, m_size.top, width, m_header_height, &dcMemory, 0, 0, width, bm.bmHeight, SRCCOPY);
        xpos += width;
    }

    if (IsSelected())
    {
        //highlight selected item
        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.bottom = rect.top + m_header_height - 3;
        rect.top += 1;


        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);

        if (m_icon_id_highlight)
        {
            icon_id = m_icon_id_highlight;
        }
    }

    boxrect = m_size;
    boxrect.bottom = boxrect.top + m_header_height;
    boxrect.left += 8;

    if (icon_id)
    {
        vspace = boxrect.bottom - boxrect.top;
        vspace -= m_icon_height;
        vspace /= 2;
        PaintIcon(dc, icon_id, boxrect.left, boxrect.top + vspace);
        boxrect.left += m_icon_width + ICON_HORZ_SPACE;
    }

    if (IsOpen())
    {
        rightmap.LoadBitmap(IDB_MINUS);
    }
    else
    {
        rightmap.LoadBitmap(IDB_PLUS);
    }

    rightmap.GetObject(sizeof(BITMAP), &bm);

    width = MulDiv(bm.bmWidth, m_sys_dpi, DEFAULT_DPI_96);
    height = MulDiv(bm.bmHeight, m_sys_dpi, DEFAULT_DPI_96);

    vspace = boxrect.bottom - boxrect.top;
    vspace -= height;
    vspace /= 2;

    dcMemory.SelectObject(&rightmap);
    dc->StretchBlt(boxrect.right - width - 8, boxrect.top + vspace, width, height,
                   &dcMemory, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    dc->SetTextColor(RGB(255, 255, 255));
    dc->SetBkColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    old_font = dc->SelectObject(&ViewHeaderFont);
    dc->DrawText(mpRes->name, boxrect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
    dc->SelectObject(old_font);

    // paint the gray background for the column header
    if (m_open)
    {
        boxrect = m_size;
        boxrect.top += m_header_height;
        //CBrush *old_brush;
        CBrush fillbrush;
        fillbrush.CreateSolidBrush(RGB(229, 229, 229));
        dc->FillRect(&boxrect, &fillbrush);
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintFolderHeader(CDC *dc)
{
    CRect boxrect;
    CFont *old_font;
    int vspace;

    // Draw folder icon
    boxrect = m_size;
    boxrect.bottom = boxrect.top + m_header_height;
    boxrect.left += 8;

    vspace = boxrect.bottom - boxrect.top;
    vspace -= m_icon_height;
    vspace /= 2;

    if (IsOpen())
    {
        PaintIcon(dc, IDB_FOLDER_OPEN, boxrect.left, boxrect.top + vspace);
    }
    else
    {
        PaintIcon(dc, IDB_FOLDER_CLOSE, boxrect.left, boxrect.top + vspace);
    }

    boxrect.left += m_icon_width + ICON_HORZ_SPACE;

    // Draw folder name
    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.left = boxrect.left - 3;

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    }

    dc->SetBkColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    old_font = dc->SelectObject(&ViewHeaderFont);
    dc->DrawText(mpRes->name, boxrect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    dc->SelectObject(old_font);


    CPen *old_pen;
    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    old_pen = dc->SelectObject(&line_pen);

    // Draw line across the bottom
    dc->MoveTo(m_size.left, m_size.bottom);
    dc->LineTo(m_size.right, m_size.bottom);
    dc->SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintColorItemLines(CDC *dc, int offset)
{
    CPen *old_pen;

    // draw vertical line
    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    old_pen = dc->SelectObject(&line_pen);
    dc->MoveTo(m_size.left + m_item_height + (COLOR_BOX_SPACE << 1),
               m_size.top + offset);
    dc->LineTo(m_size.left + m_item_height + (COLOR_BOX_SPACE << 1),
               m_size.bottom);

    // draw line across the bottom
    dc->MoveTo(m_size.left, m_size.bottom);
    dc->LineTo(m_size.right, m_size.bottom);
    dc->SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintColorGroupColumnHeader(CDC *dc)
{
    PaintColorItemLines(dc, m_header_height - 1);

    // draw the text:
    // column headers are always black on gray, no matter system color settings
    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    CFont *old_font = dc->SelectObject(&MediumFont);
    dc->TextOut(m_size.left + m_item_height + (COLOR_BOX_SPACE << 1) + COLOR_TEXT_SPACE,
               m_size.top + m_header_height + 2,
               _T("Name"));
    dc->SelectObject(old_font);    
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintColorItem(CDC *dc)
{
    CBrush boxbrush;
    CBrush *old_brush;
    CFont *old_font;

    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    UINT nDrawFormat = DT_LEFT|DT_TOP|DT_SINGLELINE|DT_VCENTER;
    
    PaintColorItemLines(dc, 0);

    CRect boxrect = m_size;
    boxrect.left += COLOR_BOX_SPACE;
    boxrect.right = boxrect.left + m_item_height - 1;
    boxrect.DeflateRect(2, 3);

    boxbrush.CreateSolidBrush(m_boxcolor);

    old_brush = dc->SelectObject(&boxbrush);

    dc->Rectangle(boxrect);

    boxrect.left = m_size.left + m_item_height + (COLOR_BOX_SPACE << 1) + COLOR_TEXT_SPACE;
    boxrect.right = m_size.left + GetWidth(dc);

    old_font = dc->SelectObject(&NormalFont);

    dc->SetBkColor(GetSysColor(COLOR_WINDOW));

    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)); 

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.left = m_size.left + m_item_height + (COLOR_BOX_SPACE << 1) + 1;

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));  
    }

    dc->DrawText(mpRes->name, boxrect, nDrawFormat);

    dc->SelectObject(old_font);
    dc->SelectObject(old_brush);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintFontItemLines(CDC *dc, int offset)
{
    CPen *old_pen;

    // draw vertical lines
    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    old_pen = dc->SelectObject(&line_pen);
    dc->MoveTo(m_size.left + m_font_preview_width,
               m_size.top + offset);
    dc->LineTo(m_size.left + m_font_preview_width,
               m_size.bottom);
    dc->MoveTo(m_size.left + m_font_preview_width + m_font_pixel_width,
               m_size.top + offset);
    dc->LineTo(m_size.left + m_font_preview_width + m_font_pixel_width,
               m_size.bottom);
    dc->MoveTo(m_size.right - m_font_storage_width,
               m_size.top + offset);
    dc->LineTo(m_size.right - m_font_storage_width,
               m_size.bottom);

    // draw line across the bottom
    dc->MoveTo(m_size.left, m_size.bottom);
    dc->LineTo(m_size.right, m_size.bottom);
    dc->SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintFontGroupColumnHeader(CDC *dc)
{
    PaintFontItemLines(dc, m_header_height - 1);

    // draw the text:
    // column headers are always black on gray, no matter system color settings
    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    CFont *old_font = dc->SelectObject(&MediumFont);
    RECT   rect = m_size;

    rect.top += m_header_height + 2;
    rect.left += 4;
    rect.right = rect.left + m_font_preview_width - 8;
    dc->DrawText(_T("View"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = rect.left + m_font_pixel_width - 8;
    dc->DrawText(_T("Pix"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = m_size.right - m_font_storage_width - 4;
    dc->DrawText(_T("Name"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = m_size.right - 4;
    dc->DrawText(_T("Size"), &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

    dc->SelectObject(old_font);    
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintFontItem(CDC *dc)
{
    CRect boxrect;
    CFont *old_font;
    TCHAR pix_size[20];

    //Draw font preview
    boxrect = m_size;
    boxrect.left += 4;
    boxrect.right = m_size.left + m_font_preview_width - 4;

    if (m_preview_mem)
    {
        int offset = (boxrect.Height() - m_bitmap.gx_bmp_header.biHeight) / 2;
        guix_bitmap_flush(dc->GetSafeHdc(), boxrect.left, boxrect.top + offset,
                          boxrect.right - boxrect.left + 1, m_preview_mem, &m_bitmap);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
        dc->SetBkColor(GetSysColor(COLOR_WINDOW));
        dc->DrawText(mpRes->name, boxrect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_VCENTER);
    }

    //Draw pix size
    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.left = m_size.left + m_font_preview_width + 1;

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    }

    PaintFontItemLines(dc, 0);

    dc->SetBkColor(GetSysColor(COLOR_WINDOW));
    old_font = dc->SelectObject(&NormalFont);
    boxrect.left = boxrect.right + 8;
    boxrect.right = boxrect.left + m_font_pixel_width - 8;
    _itot(mpRes->font_height, pix_size, 10);
    dc->DrawText(pix_size, boxrect, DT_RIGHT | DT_TOP | DT_SINGLELINE | DT_VCENTER);

    //Draw font name
    boxrect.left = boxrect.right + 8;
    boxrect.right = m_size.right - m_font_storage_width - 4;
    dc->DrawText(mpRes->name, boxrect, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_VCENTER);

    // Display resource storage.
    boxrect.left = boxrect.right + 8;
    boxrect.right = m_size.right - 4;
    int storage = (mpRes->storage_size + 1023) >> 10;
    CString str = NumberFormatWithCommas(storage);
    str.Append(_T("KB"));
    dc->DrawText(str, &boxrect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

    dc->SelectObject(old_font);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintPixelmapItemLines(CDC *dc, int offset)
{
    CPen *old_pen;
    int xpos;

    // draw vertical line
    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    old_pen = dc->SelectObject(&line_pen);

    xpos = m_size.left + m_thumbnail_size + (ITEM_VERT_SPACE * 2);
    dc->MoveTo(xpos, m_size.top + offset);
    dc->LineTo(xpos, m_size.bottom);

    xpos += m_pixelmap_dimension_width;
    dc->MoveTo(xpos, m_size.top + offset);
    dc->LineTo(xpos, m_size.bottom);

    xpos = m_size.right - m_pixelmap_storage_width;
    dc->MoveTo(xpos, m_size.top + offset);
    dc->LineTo(xpos, m_size.bottom);

    // draw line across the bottom
    dc->MoveTo(m_size.left, m_size.bottom);
    dc->LineTo(m_size.right, m_size.bottom);
    dc->SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintPixelmapGroupColumnHeader(CDC *dc)
{
    PaintPixelmapItemLines(dc, m_header_height - 1);

    // draw the text:
    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);

    CFont *old_font = dc->SelectObject(&MediumFont);
    RECT   rect = m_size;

    rect.top += m_header_height + 2;
    rect.left += 4;
    rect.right = rect.left + m_thumbnail_size + (ITEM_VERT_SPACE * 2) - 8;
    dc->DrawText(_T("View"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = rect.left + m_pixelmap_dimension_width - 8;
    dc->DrawText(_T("Dims"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = m_size.right - m_pixelmap_storage_width - 4;
    dc->DrawText(_T("Name"), &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    rect.left = rect.right + 8;
    rect.right = m_size.right - 4;
    dc->DrawText(_T("Size"), &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

    dc->SelectObject(old_font);   
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintPixelmapItem(CDC *dc)
{
    CRect boxrect = m_size;
    boxrect.left += 4;

    boxrect.right = boxrect.left + m_thumbnail_size + (ITEM_VERT_SPACE * 2) - 8;

    // Draw pixelmap preview
    if (m_preview_mem)
    {
        int vspace = boxrect.bottom - boxrect.top + 1;
        vspace -= m_bitmap.gx_bmp_header.biHeight;
        vspace /= 2;

        int hspace = m_thumbnail_size;
        hspace -= m_bitmap.gx_bmp_header.biWidth;
        hspace /= 2;

        if (hspace < 0)
        {
            hspace = 0;
        }

        guix_bitmap_flush(dc->GetSafeHdc(),
            boxrect.left + hspace,
            boxrect.top + vspace,
            boxrect.right - boxrect.left + 1,
            m_preview_mem, &m_bitmap);
    }

    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));
        CRect rect = m_size;
        rect.left = m_size.left + m_thumbnail_size + (ITEM_VERT_SPACE * 2) + 1;

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&rect, &brush);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    }

    PaintPixelmapItemLines(dc, 0);

    dc->SetBkMode(TRANSPARENT);

    CFont *old_font = dc->SelectObject(&NormalFont);

    CString str;

    // Draw pixelmap size. 
    boxrect.left = boxrect.right + 8;
    boxrect.right = boxrect.left + m_pixelmap_dimension_width - 8;
    GX_PIXELMAP* map = mpRes->GetPixelmap();
    if (map)
    {
        str.Format(_T("%d x %d"), map->gx_pixelmap_width, map->gx_pixelmap_height);
        dc->DrawText(str, &boxrect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    }

    // Draw pixelmap name.
    boxrect.left = boxrect.right + 8;
    boxrect.right = m_size.right - m_pixelmap_storage_width - 4;
    dc->DrawText(mpRes->name, &boxrect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

    // Draw resource storage size.
    boxrect.left = boxrect.right + 8;
    boxrect.right = m_size.right - 8;
    int storage = (mpRes->storage_size + 1023) >> 10;
    str = NumberFormatWithCommas(storage);
    str.Append(_T("KB"));
    dc->DrawText(str, &boxrect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
    dc->SelectObject(old_font);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintStringItemLines(CDC *dc, int offset)
{
    CPen *old_pen;

    // draw vertical lines
    CPen line_pen(PS_SOLID, 1, RGB(192, 192, 192));
    old_pen = dc->SelectObject(&line_pen);
    dc->MoveTo(m_size.left + m_string_id_width,
               m_size.top + offset);
    dc->LineTo(m_size.left + m_string_id_width,
               m_size.bottom);

    // draw line across the bottom
    dc->MoveTo(m_size.left, m_size.bottom);
    dc->LineTo(m_size.right, m_size.bottom);
    dc->SelectObject(old_pen);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintStringGroupColumnHeader(CDC *dc)
{
    PaintStringItemLines(dc, m_header_height - 1);
    CRect textrect;
    int xpos;

    // draw the text:
    // column headers are always white on gray, no matter the system color settings.
    dc->SetTextColor(RGB(0, 0, 0));
    dc->SetBkMode(TRANSPARENT);
    
    CFont *old_font = dc->SelectObject(&MediumFont);
    dc->TextOut(m_size.left + 4,
               m_size.top + m_header_height + 2,
               _T("String ID"));

    studiox_project *project = GetOpenProject();
    if (project && mpRes)
    {
        int display = project->GetDisplayIndex(mpRes);
        if (display < 0)
        {
            return;
        }
        string_table *table = project->mDisplays[display].stable;
        if (!table)
        {
            return;
        }

        int language = table->GetActiveLanguage();

        xpos = m_size.left + m_string_id_width + 4;
        textrect.SetRect(xpos,  m_size.top + m_header_height + 2,
                         m_size.right - 32, m_size.bottom);
            
        dc->DrawText(project->mHeader.languages[language].name, &textrect, DT_LEFT);

        CBitmap map;
        BITMAP  bmp;

        map.LoadBitmap(IDB_RIGHT_ARROW);
        map.GetBitmap(&bmp);

        int dpi = GetSystemDPI();
        m_string_arrow_icon_width = MulDiv(bmp.bmWidth, dpi, DEFAULT_DPI_96);

        xpos = m_size.right - m_string_arrow_icon_width;
        PaintBmp(dc, xpos, m_size.top + m_header_height + 6, IDB_RIGHT_ARROW);
        xpos -= m_string_arrow_icon_width;
        PaintBmp(dc, xpos,  m_size.top + m_header_height + 6, IDB_LEFT_ARROW);
    }
    dc->SelectObject(old_font);    
}


///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintStringItem(CDC *dc)
{
    
    int left = m_size.left + 4;
    CRect textrect = m_size;
    textrect.left += 4;
    textrect.right = m_size.left + m_string_id_width - 4;
   

    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));

        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&m_size, &brush);
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    }

    PaintStringItemLines(dc, 0);

    CFont *old_font = dc->SelectObject(&NormalFont);

    dc->DrawText(display_id, &textrect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

    textrect.left = m_size.left + m_string_id_width + 4;
    textrect.right = m_size.right - 4;

#ifdef _UNICODE
    DrawTextW(*dc, string_val.GetString(), -1, &textrect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
#else
    wchar_t *uni_text = CStringToWideChar(string_val);

    if (uni_text)
    {
        DrawTextW(*dc, uni_text, -1, &textrect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
        delete [] uni_text;
    }
#endif
    dc->SelectObject(old_font);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintAddNewItem(CDC* dc)
{
    CFont *old_font = dc->SelectObject(&NormalFont);
    int icon_id = m_icon_id;

    if (IsSelected())
    {
        //highlight selected item
        dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));

        CBrush brush(GetSysColor(COLOR_HIGHLIGHT));


        //FillRect does not include the rectangle's right and bottom sides
        dc->FillRect(&m_size, &brush);

        if (m_icon_id_highlight)
        {
            icon_id = m_icon_id_highlight;
        }
    }
    else
    {
        dc->SetTextColor(GetSysColor(COLOR_MENUTEXT));
    }

    if (icon_id)
    {
        int offset = (m_size.Height() - m_icon_height) >> 1;
        PaintIcon(dc, icon_id, m_size.left + 8, m_size.top + offset);
    }

    CRect boxrect = m_size;
    boxrect.left += m_icon_width + 16;

    dc->SetBkColor(GetSysColor(COLOR_WINDOW));
    dc->DrawText(mpRes->name, &boxrect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
    dc->SelectObject(old_font);
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::Paint(CDC *dc)
{
    if (m_visible)
    {
        int type = GetResType();

        switch(type)
        {
        case RES_TYPE_HEADER:
            if (mpRes->folder_id == THEME_HEADER)
            {
                PaintThemeHeader(dc);
            }
            break;

        case RES_TYPE_GROUP:
            PaintGroupHeader(dc);
            if (m_open)
            {
                switch(mpRes->folder_id)
                {
                case COLOR_GROUP:
                    PaintColorGroupColumnHeader(dc);
                    break;

                case FONT_GROUP:
                    PaintFontGroupColumnHeader(dc);
                    break;

                case PIXELMAP_GROUP:
                    PaintPixelmapGroupColumnHeader(dc);
                    break;

                case STRING_GROUP:
                    PaintStringGroupColumnHeader(dc);
                    break;

                default:
                    break;
                }
            }
            break;

        case RES_TYPE_FOLDER:
            if ((mpRes->folder_id == DEFAULT_PIXELMAP_FOLDER) ||
                (mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER))
            {
                PaintFolderHeader(dc);
            }
            break;

        case RES_TYPE_COLOR:
            PaintColorItem(dc);
            break;

        case RES_TYPE_FONT:
            PaintFontItem(dc);
            break;

        case RES_TYPE_PIXELMAP:
            PaintPixelmapItem(dc);
            break;

        case RES_TYPE_STRING:
            PaintStringItem(dc);
            break;

        case RES_TYPE_ADD_COLOR:
        case RES_TYPE_ADD_FONT:
        case RES_TYPE_ADD_PIXELMAP:
        case RES_TYPE_ADD_STRING:
            PaintAddNewItem(dc);
            break;

        default:
            break;
        }

        if (m_open && m_first)
        {
            m_first->Paint(dc);
        }
    }
    if (m_next)
    {
        m_next->Paint(dc);
    }
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::GetItemHeight()
{
    return m_item_height;
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::GetHeight()
{
    int height = 0;
    int type = RES_TYPE_STRING;

    if (mpRes)
    {
        type = mpRes->type;
    }
    switch(type)
    {
    case RES_TYPE_HEADER:
        if (mpRes->folder_id == THEME_HEADER)
        {
            return m_header_height;
        }
        else
        {
            return 0;
        }
        break;

    case RES_TYPE_GROUP:
        if (m_open)
        {
            return m_header_height + m_item_height;
        }
        else
        {
            return m_header_height;
        }
        break;

    case RES_TYPE_STRING:
    case RES_TYPE_COLOR:
    case RES_TYPE_ADD_COLOR:
    case RES_TYPE_ADD_FONT:
    case RES_TYPE_ADD_PIXELMAP:
    case RES_TYPE_ADD_STRING:
        height = m_item_height;
        break;

    case RES_TYPE_FOLDER:
        if ((mpRes->folder_id == DEFAULT_PIXELMAP_FOLDER) ||
            (mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER))
        {
            return m_header_height;
        }

        return 0;
    
    case RES_TYPE_FONT:
        if (!m_preview_mem)
        {
            MakeFontPreview();
        }
        if (m_preview_mem)
        {
            height = MulDiv(m_bitmap.gx_bmp_header.biHeight, m_sys_dpi, DEFAULT_DPI_96) + 4;
        }
        else
        {
            height = m_item_height;
        }
        break;

    case RES_TYPE_PIXELMAP:
        if (!m_preview_mem)
        {
            MakePixelmapPreview();
        }
        height = m_item_height;
        if (m_preview_mem)
        {
            if (m_bitmap.gx_bmp_header.biHeight > height)
            {
                height = m_bitmap.gx_bmp_header.biHeight;
            }
        }
        height += ITEM_VERT_SPACE * 2;
        break;

    default:
        break;
    }
    return height;
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::GetWidth(CDC *dc)
{
    CFont *old_font;
    int width = 0;
    int type = RES_TYPE_STRING;

    if (mpRes)
    {
        type = mpRes->type;
    }
    switch(type)
    {
    case RES_TYPE_HEADER:
        break;

    case RES_TYPE_GROUP:
        width = 8 + (m_indent * TREE_INDENT);
        old_font = dc->SelectObject(&MediumFont);
        width += dc->GetTextExtent(mpRes->name).cx;
        dc->SelectObject(old_font);
        break;

    case RES_TYPE_COLOR:
        width = m_item_height + (COLOR_BOX_SPACE << 1);
        width += dc->GetTextExtent(mpRes->name).cx + (COLOR_TEXT_SPACE << 1);
        if (width < mpTree->GetViewWidth())
        {
            width = mpTree->GetViewWidth();
        }
        break;

    case RES_TYPE_FONT:
        if (!m_preview_mem)
        {
            MakeFontPreview();
        }
        if (m_preview_mem)
        {
            width = m_bitmap.gx_bmp_header.biWidth + 8;
        }
        else
        { 
            old_font = dc->SelectObject(&NormalFont);
            width = dc->GetTextExtent(mpRes->name).cx;
            dc->SelectObject(old_font);
        }
        if (width < mpTree->GetViewWidth())
        {
            width = mpTree->GetViewWidth();
        }
        break;

    case RES_TYPE_PIXELMAP:
        width = mpTree->GetViewWidth();
        
        break;

    case RES_TYPE_STRING:
        width = 8 + (m_indent * TREE_INDENT);
        width += dc->GetTextExtent(display_id).cx;
        width += dc->GetTextExtent(string_val).cx;
        break;

    default:
        width = 8 + (m_indent * TREE_INDENT);
        old_font = dc->SelectObject(&NormalFont);
        width += dc->GetTextExtent(mpRes->name).cx;
        dc->SelectObject(old_font);
        break;
    }
    return width;   
}

#if 0
int resource_item::LayoutColorGrid(int top)
{
    CRect childsize;
    int step = COLOR_BOX_SIZE + COLOR_BOX_SPACE;
    top += COLOR_BOX_SPACE;

    childsize.SetRect(m_size.left + COLOR_BOX_SPACE, 
        top,
        m_size.left + step - 1,
        top + COLOR_BOX_SIZE - 1);

    resource_item *child = First();

    while(child)
    {
        child->SetPos(childsize);

        if (childsize.right + step > m_size.right)
        {
            top += step;
            childsize.SetRect(m_size.left + COLOR_BOX_SPACE,
                top,
                m_size.left + step - 1,
                top + COLOR_BOX_SIZE - 1);
        }
        else
        {
            childsize.OffsetRect(step, 0);
        }
        child = child->Next();      
    }

    if (mpRes->resource_id == CUSTOM_COLOR_FOLDER)
    {
        if (childsize.right + step > m_size.right)
        {
            top += step;
            m_add_icon_y = top;
            m_add_icon_x = m_size.left + COLOR_BOX_SPACE;
        }
        else
        {
            m_add_icon_y = top;
            m_add_icon_x = childsize.left;
        }
    }
    return top + step;
}
#endif

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_item::SortFirst()
{
    resource_item *winner = NULL;
    resource_item *child = First();

    while(child)
    {
        switch(child->GetResType())
        {
        case RES_TYPE_ADD_COLOR:
        case RES_TYPE_ADD_FONT:
        case RES_TYPE_ADD_PIXELMAP:
        case RES_TYPE_ADD_STRING:
            break;

        default:
            if (!winner)
            {
                winner = child;
            }
            else
            {
                if (_tcscmp(child->mpRes->name, winner->mpRes->name) < 0)
                {
                    winner = child;
                }
            }
            break;
        }

        child = child->Next();
    }

    return winner;
}

///////////////////////////////////////////////////////////////////////////////
resource_item *resource_item::SortNext(resource_item *current)
{
    resource_item *winner = NULL;
    resource_item *child = First();

    while(child)
    {
        switch (child->GetResType())
        {
        case RES_TYPE_ADD_COLOR:
        case RES_TYPE_ADD_FONT:
        case RES_TYPE_ADD_PIXELMAP:
        case RES_TYPE_ADD_STRING:
            break;

        default:
            if (_tcscmp(child->mpRes->name, current->mpRes->name) > 0)
            {
                if (!winner)
                {
                    winner = child;
                }
                else
                {
                    if (_tcscmp(child->mpRes->name, winner->mpRes->name) < 0)
                    {
                        winner = child;
                    }
                }
            }
            break;
        }

        child = child->Next();
    }
    return winner;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_item::FindAddNewItem()
{
    resource_item* child = First();

    while (child)
    {
        switch (child->GetResType())
        {
        case RES_TYPE_ADD_COLOR:
        case RES_TYPE_ADD_FONT:
        case RES_TYPE_ADD_PIXELMAP:
        case RES_TYPE_ADD_STRING:
            return child;

        default:
            break;
        }

        child = child->Next();
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_item::NavigateFirst()
{
    resource_item* first = NULL;

    switch (GetResType())
    {
    case RES_TYPE_FOLDER:
        first = SortFirst();
        break;

    case RES_TYPE_GROUP:
        first = First();

        if (first->GetResType() == RES_TYPE_FOLDER)
        {
            switch (first->mpRes->folder_id)
            {
            case DEFAULT_COLOR_FOLDER:
            case CUSTOM_COLOR_FOLDER:
            case DEFAULT_FONT_FOLDER:
            case CUSTOM_FONT_FOLDER:
                // Skip folders which is invisible.
                first = first->SortFirst();
                break;
            }
        }
        break;

    default:
        first = First();
        break;
    }

    return first;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_item::NavigateLast()
{
    resource_item* last = NULL;
    resource_item* next;

    switch (GetResType())
    {
    case RES_TYPE_FOLDER:
        if (mpRes->folder_id == CUSTOM_PIXELMAP_FOLDER)
        {
            last = FindAddNewItem();
        }
        else
        {
            next = SortFirst();
            do
            {
                last = next;
                next = SortNext(last);
            } while (next);
        }
        break;

    case RES_TYPE_GROUP:
        last = Last();

        if (last->GetResType() == RES_TYPE_FOLDER)
        {
            switch (last->mpRes->folder_id)
            {
            case DEFAULT_COLOR_FOLDER:
            case CUSTOM_COLOR_FOLDER:
            case DEFAULT_FONT_FOLDER:
            case CUSTOM_FONT_FOLDER:
                // Skip folders which is invisible.
                last = last->FindAddNewItem();
                break;
            }
        }
        break;

    default:
        last = Last();
        break;
    }

    return last;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_item::NavigateNext()
{
    resource_item* next = NULL;
    resource_item* parent;
    switch (GetResType())
    {
    case RES_TYPE_FONT:
    case RES_TYPE_COLOR:
    case RES_TYPE_PIXELMAP:
        // Pick parent item.
        parent = Parent();
        if (parent->GetResType() == RES_TYPE_FOLDER)
        {
            // Sort next item.
            next = parent->SortNext(this);

            if (next == NULL)
            {
                switch (parent->mpRes->folder_id)
                {
                case DEFAULT_COLOR_FOLDER:
                case DEFAULT_FONT_FOLDER:
                    // Set the first custom color/font item to be the next sibling of the last default color/font item.
                    parent = parent->Next();

                    if (parent)
                    {
                        next = parent->SortFirst();
                    }
                    break;
                }

                if (next == NULL)
                {
                    next = parent->FindAddNewItem();
                }
            }
        }
        break;

    case RES_TYPE_ADD_COLOR:
    case RES_TYPE_ADD_FONT:
    case RES_TYPE_ADD_PIXELMAP:
    case RES_TYPE_ADD_STRING:
        break;

    default:
        next = Next();
        break;
    }

    return next;
}

///////////////////////////////////////////////////////////////////////////////
resource_item* resource_item::NavigateParent()
{
    int restype = GetResType();

    if (restype == RES_TYPE_HEADER ||
        restype == RES_TYPE_GROUP)
    {
        return NULL;
    }

    resource_item* parent = Parent();

    if (parent && parent->GetResType() == RES_TYPE_FOLDER)
    {
        switch (parent->mpRes->folder_id)
        {
        case DEFAULT_COLOR_FOLDER:
        case CUSTOM_COLOR_FOLDER:
        case DEFAULT_FONT_FOLDER:
        case CUSTOM_FONT_FOLDER:
            // Skip folders which is invisible
            parent = parent->Parent();
            break;
        }
    }

    return parent;
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::LayoutFolderChildren(CDC *dc, int top)
{
    resource_item *child;
    CRect size;
    int height;
    int width;

    child = SortFirst();
    while(child)
    {   
        height = child->GetHeight();
        width = child->GetWidth(dc);
        size.SetRect(0, top, width, top + height - 1);
        child->SetPos(size);
        top += height;

        child = SortNext(child);
    }

    resource_item* pAdd = FindAddNewItem();
    if (pAdd)
    {
        //position "Add New" icon
        height = pAdd->GetHeight();
        width = mpTree->GetViewWidth();
        size.SetRect(0, top, width, top + height);
        pAdd->SetPos(size);
        top += height;
    }

    return top;
}


///////////////////////////////////////////////////////////////////////////////
int resource_item::CheckClickCommand(CPoint &click)
{
    CRect size;

    if (!mpRes)
    {
        return CMD_NO_COMMAND;
    }

    if ((mpRes->type == RES_TYPE_HEADER) &&
        (mpRes->folder_id == THEME_HEADER))
    {
        if (m_up_icon_rect.PtInRect(click))
        {
            return CMD_INC_THEME;
        }
        else if (m_down_icon_rect.PtInRect(click))
        {
            return CMD_DEC_THEME;
        }
    }
    else if (mpRes->type == RES_TYPE_GROUP &&
             mpRes->folder_id == STRING_GROUP)
    {
        // test here for inc/dec language commands:

        size.right = m_size.right;
        size.left = size.right - m_string_arrow_icon_width;
        size.top = m_size.top + m_header_height + 6;
        size.bottom = size.top + m_string_arrow_icon_width;
            
        if (size.PtInRect(click))
        {
            return CMD_INC_LANGUAGE;
        }
        size.OffsetRect(-12, 0);
        if (size.PtInRect(click))
        {
            return CMD_DEC_LANGUAGE;
        }
    }
 
    return CMD_NO_COMMAND;       
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::SetColorValue(GX_COLOR val)
{
    if (mpRes)
    {
        mpRes->colorval = val;
        RebuildPreview();
    }
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::SetIcon(int IconId, int IconIdHighlight)
{
    m_icon_id = IconId;
    m_icon_id_highlight = IconIdHighlight;
    CBitmap icon;

    BITMAP bmp;

    if (IconId)
    {
        if (icon.LoadBitmap(IconId))
        {
            icon.GetBitmap(&bmp);
            m_icon_width = MulDiv(bmp.bmWidth, m_sys_dpi, DEFAULT_DPI_96);
            m_icon_height = MulDiv(bmp.bmHeight, m_sys_dpi, DEFAULT_DPI_96);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
COLORREF resource_item::MakeColorRef(GX_COLOR val)
{
    UCHAR Red = (UCHAR) (val >> 16);
    UCHAR Green = (UCHAR) ((val >> 8) & 0xff);
    UCHAR Blue = (UCHAR) (val & 0xff);
    return(RGB(Red, Green, Blue));
}

///////////////////////////////////////////////////////////////////////////////
GX_COLOR resource_item::GetRGBColor(int display_index)
{
    GX_COLOR *palette;
    int palsize;
    GX_COLOR color = 0;
    studiox_project *project = GetOpenProject();

    if (mpRes == NULL || project == NULL)
    {
        return color;
    }
    if (mpRes->type != RES_TYPE_COLOR)
    {
        return color;
    }

    if (display_index == -1)
    {
        display_index = project->GetDisplayIndex(mpRes);
    }

    /* only 8bit driver need get rgb color from palette table now. 
       4bpp driver contains palette table but we can calculate its gray value directly. */
    if (project->mDisplays[display_index].colorformat == GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        int active_theme = project->mDisplays[display_index].active_theme;

        // colorval is a palette index, lookup rgb and convert to COLORREF
        palette = project->mDisplays[display_index].themes[active_theme].palette;
        palsize = project->mDisplays[display_index].themes[active_theme].palette_predefined;

        if ((ULONG) mpRes->colorval >= (ULONG) palsize)
        {
            /* this should not be allowed to happen, but just as a safety check
               if the palette size has been changed and we didn't update the color
               indexes correctly, do a final safety check here
            */
            mpRes->colorval = 0;
        }

        if (palette && ((int) mpRes->colorval >= 0 && (int) mpRes->colorval < palsize))
        {
            color = palette[mpRes->colorval];
        } 
    }
    else
    {
        color = mpRes->colorval;
    }

    if (!IsAlphaFormat(project->mDisplays[display_index].colorformat))
    {
        color |= 0xff000000;
    }
    return color;
}

///////////////////////////////////////////////////////////////////////////////
int resource_item::GetResType()
{
    if (mpRes)
    {
        return mpRes->type;
    }

    return RES_TYPE_STRING;
}

///////////////////////////////////////////////////////////////////////////////
resource_item_provider* resource_item::GetResItemProvider()
{
    if (m_pResItemProvider == NULL)
    {
        m_pResItemProvider = new resource_item_provider(this);
    }
    return m_pResItemProvider;
}

///////////////////////////////////////////////////////////////////////////////
CString resource_item::GetName()
{
    int type = GetResType();

    switch(type)
    {
    case RES_TYPE_STRING:
        return string_val;
    }

    if (mpRes)
    {
        return mpRes->name;
    }

    return _T("");
}

///////////////////////////////////////////////////////////////////////////////
CString resource_item::GetDescription()
{
    int type = GetResType();
    CString description(L"");

    switch (type)
    {
    case RES_TYPE_FONT:
        if (mpRes)
        {
            description.Format(L"Pix: %d, Name: %s, Size: %d kilobyte", mpRes->font_height, mpRes->name, (mpRes->storage_size + 1023) >> 10);
        }
        break;

    case RES_TYPE_PIXELMAP:
        if(mpRes)
        {   
            GX_PIXELMAP *map = mpRes->GetPixelmap();
            if (map)
            {
                description.Format(L"Dims: %d by %d, Name: %s, Size: %d kilobyte", map->gx_pixelmap_width, map->gx_pixelmap_height, mpRes->name,
                                    (mpRes->storage_size + 1023) >> 10);
            }
        }
        break;

    case RES_TYPE_STRING:
        description.Format(L"String ID: %s, String value: %s", string_id, string_val);
        break;
    }

    return description;
}

///////////////////////////////////////////////////////////////////////////////
void resource_item::PaintHighlightRect(CDC* dc, CRect *rect)
{
    //Draw a blue rectangle to highlight the item
    CPen penBlue(PS_SOLID, 2, GetSysColor(COLOR_HIGHLIGHT));
    CPen *pOldPen = dc->SelectObject(&penBlue);
    CBrush *pOldBrush = (CBrush *)dc->SelectStockObject(NULL_BRUSH);

    dc->Rectangle(*rect);
    dc->SelectObject(pOldPen);
    dc->SelectObject(pOldBrush);
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_item::IsSelected()
{
    resource_view* view = GetResourceView();

    if (view && (GetFocus() == view->GetSafeHwnd()) && (mpTree->GetCurrentItem() == this))
    {
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL resource_item::IsAlphaFormat(int color_format)
{
    if (color_format == GX_COLOR_FORMAT_32ARGB ||
        color_format == GX_COLOR_FORMAT_32BGRA ||
        color_format == GX_COLOR_FORMAT_4444ARGB ||
        color_format == GX_COLOR_FORMAT_4444BGRA)
    {
        return TRUE;
    }
    return FALSE;
}
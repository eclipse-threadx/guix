/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Menu Management (Menu)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_menu.h"
#include "gx_pixelmap_prompt.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_menu_create                                     PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates an menu widget.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to the menu control   */
/*                                            block                       */
/*    name                                  Name of the menu              */
/*    parent                                Parent control block          */
/*    text_id                               String id of menu text        */
/*    fill_id                               Pixelmap id for fill area     */
/*    style                                 Style of the widget           */
/*    menu_id                               Application-defined ID of     */
/*                                          the menu                      */
/*    size                                  Menu size                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_prompt_create            Create a pixelmap prompt      */
/*    _gx_widget_create                     Create a widget               */
/*    _gx_widget_link                       Link a child widget to its    */
/*                                            parent                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            set menu event process,     */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_menu_create(GX_MENU *menu, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                      ULONG style, USHORT menu_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_pixelmap_prompt_create((GX_PIXELMAP_PROMPT *)menu, name, GX_NULL, text_id, fill_id, style, menu_id, size);

    /* Populate the rest of menu control block - overriding as necessary.  */
    menu -> gx_widget_type = GX_TYPE_MENU;
    menu -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_menu_draw;
    menu -> gx_widget_event_process_function = (UINT(*)(GX_WIDGET*, GX_EVENT*))_gx_menu_event_process;
    menu -> gx_widget_style |= GX_STYLE_ENABLED;
    menu -> gx_menu_text_x_offset = 10;
    menu -> gx_menu_text_y_offset = 0;
    menu -> gx_menu_list_total_count = 0;

    if (!(menu -> gx_widget_style & GX_STYLE_TEXT_ALIGNMENT_MASK))
    {
        menu -> gx_widget_style |= GX_STYLE_TEXT_LEFT;
    }

    /* create the menu list */
    _gx_widget_create((GX_WIDGET *)&menu -> gx_menu_list, "menu_list", GX_NULL, GX_STYLE_TRANSPARENT, menu_id, size);

    menu -> gx_menu_list.gx_widget_type = GX_TYPE_MENU_LIST;
    menu -> gx_menu_list.gx_menu_list_owner = (GX_WIDGET *)menu;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)menu);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}


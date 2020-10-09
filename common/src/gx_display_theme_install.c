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
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_theme_install                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service installs themes to the specified display.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    theme_table                           Theme table to be installed   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_canvas_dirty              Mark all root windows as dirty*/
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_display_theme_install(GX_DISPLAY *display, GX_CONST GX_THEME *theme_ptr)
{
    /* Install color table.  */
    _gx_display_color_table_set(display, theme_ptr->theme_color_table, theme_ptr->theme_color_table_size);

    /* install the color palette if required.  */
    if (display->gx_display_driver_palette_set &&
        theme_ptr->theme_palette != NULL)
    {
        display->gx_display_driver_palette_set(display, theme_ptr->theme_palette, theme_ptr->theme_palette_size);
    }

    /* install font table.  */
    _gx_display_font_table_set(display, theme_ptr->theme_font_table, theme_ptr->theme_font_table_size);

    /* install pixelmap table.  */
    _gx_display_pixelmap_table_set(display, theme_ptr->theme_pixelmap_table, theme_ptr->theme_pixelmap_table_size);

    _gx_system_scroll_appearance_set(theme_ptr->theme_vertical_scroll_style, (GX_SCROLLBAR_APPEARANCE *)&theme_ptr->theme_vertical_scrollbar_appearance);
    _gx_system_scroll_appearance_set(theme_ptr->theme_horizontal_scroll_style, (GX_SCROLLBAR_APPEARANCE *)&theme_ptr->theme_horizontal_scrollbar_appearance);

    /* mark all visible canvases as dirty when there is a resource change */
    _gx_display_canvas_dirty(display);

    /* Return success.  */
    return(GX_SUCCESS);
}


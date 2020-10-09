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
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_menu_text_draw                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the text of a menu widget.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    menu                                  Pointer to menu control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_text_draw                  Draw text to a widget         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
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
VOID  _gx_menu_text_draw(GX_MENU *menu)
{
GX_WIDGET     *widget = (GX_WIDGET *)menu;
GX_RESOURCE_ID color;
GX_STRING      pText;

    /* Draw text.  */

    if ((widget -> gx_widget_style & GX_STYLE_DRAW_SELECTED) &&
        (menu -> gx_prompt_selected_text_color != 0))
    {
        color = menu -> gx_prompt_selected_text_color;
    }
    else
    {
        color = menu -> gx_prompt_normal_text_color;
    }

    memset((GX_STRING *)&pText, 0, sizeof(GX_STRING));
    menu -> gx_prompt_text_get_function((GX_PROMPT *)menu, &pText);

    if (pText.gx_string_ptr)
    {
        _gx_widget_text_draw_ext((GX_WIDGET *)menu, color,
                                 menu -> gx_prompt_font_id,
                                 &pText,
                                 menu -> gx_menu_text_x_offset,
                                 menu -> gx_menu_text_y_offset);
    }
}


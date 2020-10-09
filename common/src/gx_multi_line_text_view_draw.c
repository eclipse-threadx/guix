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
/**   Multi Line Text View Management (Multi Line Text View)              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_multi_line_text_view.h"
#include "gx_window.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_draw                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws a multi-line-text-view widget.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                              Multi-line_text_view widget  */
/*                                           control block                */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_border_draw                Draw window background with   */
/*                                            specified fill color        */
/*    _gx_multi_line_text_view_text_draw    Draw text                     */
/*    _gx_widget_children_draw              Draw children widgets         */
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
VOID  _gx_multi_line_text_view_draw(GX_MULTI_LINE_TEXT_VIEW *text_view)
{
GX_RESOURCE_ID text_color;

    /* Set the brush color, font, width. */
    if (text_view -> gx_widget_style & GX_STYLE_ENABLED)
    {
        if (text_view -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            text_color = text_view -> gx_multi_line_text_view_selected_text_color;
        }
        else
        {
            text_color = text_view -> gx_multi_line_text_view_normal_text_color;
        }
    }
    else
    {
        text_color = text_view -> gx_multi_line_text_view_disabled_text_color;
    }

    /* Draw window background. */
    _gx_window_background_draw((GX_WINDOW *)text_view);

    /* Draw text. */
    _gx_multi_line_text_view_text_draw(text_view, text_color);

    /* Draw widget's children.  */
    _gx_widget_children_draw((GX_WIDGET *)text_view);
}


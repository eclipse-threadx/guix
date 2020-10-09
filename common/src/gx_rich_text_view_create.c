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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_multi_line_text_view.h"
#include "gx_rich_text_view.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_create                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a rich text view widget.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Rich text view widget control */
/*                                            block                       */
/*    name                                  Name of the widget            */
/*    parent                                Pointer to parent widget      */
/*    text_id                               Resource ID of the text string*/
/*    fonts                                 Font list                     */
/*    style                                 Style of text view widget     */
/*    id                                    Application-defined ID of text*/
/*                                            view                        */
/*    size                                  Dimension of the widget       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_create       Create multi line text view   */
/*    _gx_widget_link                       Link the widget to its parent */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT    _gx_rich_text_view_create(GX_RICH_TEXT_VIEW *text_view,
                                  GX_CONST GX_CHAR *name,
                                  GX_WIDGET *parent,
                                  GX_RESOURCE_ID text_id,
                                  GX_RICH_TEXT_FONTS *fonts,
                                  ULONG style,
                                  USHORT id,
                                  GX_CONST GX_RECTANGLE *size)
{

    /* Call the multi line text view create function.  */
    _gx_multi_line_text_view_create((GX_MULTI_LINE_TEXT_VIEW *)text_view, name, GX_NULL, text_id, style, id, size);

    text_view -> gx_widget_type = GX_TYPE_RICH_TEXT_VIEW;
    text_view -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_rich_text_view_draw;
    text_view -> gx_window_scroll_info_get = (VOID (*)(struct GX_WINDOW_STRUCT *, ULONG, GX_SCROLL_INFO *))(void (*)(void))_gx_rich_text_view_scroll_info_get;
    text_view -> gx_rich_text_view_fonts = *fonts;
    text_view -> gx_rich_text_view_text_total_height = 0;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)text_view);
    }

    /* Return the error status from window create.  */
    return(GX_SUCCESS);
}


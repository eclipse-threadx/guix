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
#include "gx_system.h"
#include "gx_window.h"
#include "gx_multi_line_text_view.h"
#include "gx_widget.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_visible_rows_compute       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function computes the number of visible rows.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Multi line text view          */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Retrieve font                 */
/*    _gx_window_client_height_get          Get the height of client      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_event_process                              */
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
UINT _gx_multi_line_text_view_visible_rows_compute(GX_MULTI_LINE_TEXT_VIEW *view)
{
GX_FONT *font;
GX_VALUE widget_height;
INT      line_height;

    _gx_widget_font_get((GX_WIDGET *)view, view -> gx_multi_line_text_view_font_id, &font);

    if (!font)
    {
        return GX_FAILURE;
    }

    /* Pickup text height. */
    line_height = font -> gx_font_line_height + view -> gx_multi_line_text_view_line_space;

    /* Pickup widget height. */
    _gx_window_client_height_get((GX_WINDOW *)view, &widget_height);

    if (line_height)
    {
        widget_height = (GX_VALUE)(widget_height - (view -> gx_multi_line_text_view_whitespace << 1));

        /* Compute the total rows number of the widget. */
        view -> gx_multi_line_text_view_text_visible_rows = (UINT)((widget_height + line_height - 1) / line_height);
    }

    return GX_SUCCESS;
}


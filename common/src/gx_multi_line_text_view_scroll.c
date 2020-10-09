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
#include "gx_multi_line_text_view.h"
#include "gx_window.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_scroll                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs the scroll action                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Multi-line text view widget   */
/*                                             control block              */
/*    amount_to_scroll                      Shifting value                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_scrollbar_find             Find scrollbar for a window   */
/*    _gx_scrollbar_reset                   Reset scrollbar information   */
/*    _gx_utility_rectangle_resize          Offset rectangle by specified */
/*                                            value                       */
/*    _gx_multi_line_text_view_line_cache_update                          */
/*                                          Update line cache             */
/*    _gx_widget_block_move                 Move a block of pixels        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_event_process                              */
/*                                         Multi line text view           */
/*                                           event process routine        */
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
UINT    _gx_multi_line_text_view_scroll(GX_MULTI_LINE_TEXT_VIEW *view, GX_VALUE amount_to_scroll)
{
GX_RECTANGLE  block;
GX_SCROLLBAR *scroll;

    if (!amount_to_scroll)
    {
        return GX_SUCCESS;
    }

    /* Calculate the shift value. */
    view -> gx_multi_line_text_view_text_scroll_shift += amount_to_scroll;

    _gx_window_scrollbar_find((GX_WINDOW *)view, GX_TYPE_VERTICAL_SCROLL, &scroll);

    if (scroll)
    {
        _gx_scrollbar_reset((GX_SCROLLBAR *)scroll, GX_NULL);
    }
    else
    {
        if (view -> gx_multi_line_text_view_text_total_rows >
            view -> gx_multi_line_text_view_cache_size)
        {
            /* Update line index. */
            _gx_multi_line_text_view_line_cache_update(view);
        }
    }

    block = view -> gx_window_client;

    /* Offset client area by the size of whitespace.  */
    _gx_utility_rectangle_resize(&block, (GX_VALUE)(-view -> gx_multi_line_text_view_whitespace));

    block.gx_rectangle_left = (GX_VALUE)(block.gx_rectangle_left + 1);

    /* If the text view has a thin (rounded) border with no 
       whitespace between border and text, we cannot use block
       move to scroll because we will capture pixels from the
       rounded corner. In that case just use dirty_mark, otherwise
       use block_move
    */
    if ((view -> gx_widget_style & GX_STYLE_BORDER_THIN) &&
        !(view -> gx_widget_style & GX_STYLE_TRANSPARENT) &&
        (view -> gx_multi_line_text_view_whitespace == 0))
    {
        _gx_system_dirty_mark((GX_WIDGET *)view);
    }
    else
    {
        _gx_widget_block_move((GX_WIDGET *)view, &block, 0, amount_to_scroll);
    }

    return(GX_SUCCESS);
}


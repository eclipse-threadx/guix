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
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_window.h"
#include "gx_multi_line_text_view.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_index_find            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to find the first byte index of the        */
/*    sepcified line from the specified position.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi line text view          */
/*                                            control block               */
/*    start_line                            Line index to search from     */
/*    start_index                           Byte index to search from     */
/*    search_line                           Index of the line to search   */
/*    return_index                          Retrieved byte index of the   */
/*                                            line                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_client_width_get           Get window client width       */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Get the number of character   */
/*                                            bytes a line can display    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_line_start_cache_update                    */
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
static VOID _gx_multi_line_text_view_line_index_find(GX_MULTI_LINE_TEXT_VIEW *view, UINT start_line, UINT start_index, UINT search_line, UINT *return_index)
{
GX_VALUE                client_width;
GX_MULTI_LINE_TEXT_INFO text_info;

    /* Pickup widget width. */
    _gx_window_client_width_get((GX_WINDOW *)view, &client_width);

    /* Calcualte client width. */
    client_width = (GX_VALUE)(client_width - (view -> gx_multi_line_text_view_whitespace << 1));
    client_width = (GX_VALUE)(client_width - 2);

    /* Calcualte from the start of the text. */
    while (start_line < search_line)
    {
        /* Caculate maximum byte number a line can display in forward direction. */
        _gx_multi_line_text_view_display_info_get(view, start_index, view -> gx_multi_line_text_view_text.gx_string_length, &text_info, client_width);

        start_index += text_info.gx_text_display_number;
        start_line++;
    }

    *return_index = start_index;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_start_cache_create    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to create line index cache for specified   */
/*    line range.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi line text view          */
/*                                            control block               */
/*    first_line                            The line to start cache       */
/*    updated_size                          The numer of lines to cache   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_client_width_get           Get window client width       */
/*    _gx_multi_line_text_view_line_index_find                            */
/*                                          Get index of the line         */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Get the number of character   */
/*                                            bytes a line can display    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_line_start_cache_update                    */
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
static VOID _gx_multi_line_text_view_line_start_cache_create(GX_MULTI_LINE_TEXT_VIEW *view, UINT first_line, UINT updated_size)
{
GX_VALUE                client_width;
GX_MULTI_LINE_TEXT_INFO text_info;
UINT                    index;
UINT                    loop;

    /* Pickup widget width. */
    _gx_window_client_width_get((GX_WINDOW *)view, &client_width);

    /* Calcualte client width. */
    client_width = (GX_VALUE)(client_width - (view -> gx_multi_line_text_view_whitespace << 1));
    client_width = (GX_VALUE)(client_width - 2);

    /* Find index of first line. */
    _gx_multi_line_text_view_line_index_find(view, 0, 0, first_line, &index);

    view -> gx_multi_line_text_view_line_index[0] = index;

    for (loop = 1; loop < updated_size; loop++)
    {
        /* Caculate maximum byte number a line can display in forward direction. */
        _gx_multi_line_text_view_display_info_get(view, index, view -> gx_multi_line_text_view_text.gx_string_length, &text_info, client_width);

        index += text_info.gx_text_display_number;
        view -> gx_multi_line_text_view_line_index[loop] = index;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_tail_cache_create     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to create line index cache for specified   */
/*    line range with specified byte index and line index to start        */
/*    calculating needed line index.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*   view                                   Multi line text view          */
/*                                            control block               */
/*   start_index                            The first byte index of the   */
/*                                            line to search from         */
/*   start_line                             The line to search from       */
/*   first_line                             The line to start cache       */
/*   updated_size                           The number of lines to cache  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_client_width_get           Get window client width       */
/*    _gx_multi_line_text_view_line_index_find                            */
/*                                          Search byte index of specified*/
/*                                            line                        */
/*    _gx_multi_line_text_view_display_info_get                           */
/*                                          Calculate the number of bytes */
/*                                            that a line can display     */
/*    _gx_system_string_get                 Retrieve string by ID         */
/*    _gx_system_private_string_get         Retrieve string pointer in a  */
/*                                            dynamically copied string   */
/*                                            buffer                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_line_tail_cache_update                     */
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
static VOID _gx_multi_line_text_view_line_tail_cache_create(GX_MULTI_LINE_TEXT_VIEW *view, UINT start_index, UINT start_line, UINT first_line, UINT updated_size)
{
GX_VALUE                client_width;
GX_MULTI_LINE_TEXT_INFO text_info;
UINT                    index;
UINT                    loop;

    /* Pickup widget width. */
    _gx_window_client_width_get((GX_WINDOW *)view, &client_width);

    /* Calcualte client width. */
    client_width = (GX_VALUE)(client_width - (view -> gx_multi_line_text_view_whitespace << 1));
    client_width = (GX_VALUE)(client_width - 2);

    /* Find index of first line. */
    _gx_multi_line_text_view_line_index_find(view, start_line, start_index, first_line, &index);

    view -> gx_multi_line_text_view_line_index[GX_MULTI_LINE_INDEX_CACHE_SIZE - updated_size] = index;

    for (loop = GX_MULTI_LINE_INDEX_CACHE_SIZE - updated_size + 1; loop < GX_MULTI_LINE_INDEX_CACHE_SIZE; loop++)
    {
        /* Calculate maximum byte number a line can display in forward direction. */
        _gx_multi_line_text_view_display_info_get(view, (UINT)index, view -> gx_multi_line_text_view_text.gx_string_length, &text_info, client_width);

        index += text_info.gx_text_display_number;
        view -> gx_multi_line_text_view_line_index[loop] = index;
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_start_cache_update    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to update line index cache to cover        */
/*    previous lines.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Multi line text view          */
/*                                            control block               */
/*    new_first_cache_line                  The new line index that cache */
/*                                            start                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_line_start_cache_create                    */
/*                                          Create line index cache for   */
/*                                            specified range             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_line_cache_update                          */
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
static VOID _gx_multi_line_text_view_line_start_cache_update(GX_MULTI_LINE_TEXT_VIEW *view, UINT new_first_cache_line)
{
UINT first_cache_line;
UINT updated_size;

    /* Pickup first cache line. */
    first_cache_line = view -> gx_multi_line_text_view_first_cache_line;

    updated_size = first_cache_line - new_first_cache_line;

    if (updated_size < GX_MULTI_LINE_INDEX_CACHE_SIZE)
    {
        /* Move line index value in [0, index_cache_size - updated_size] to [updated_size, index_cache_size]. */
        memmove(view -> gx_multi_line_text_view_line_index + updated_size, view -> gx_multi_line_text_view_line_index,
                sizeof(UINT) * (GX_MULTI_LINE_INDEX_CACHE_SIZE - updated_size));
    }
    else
    {
        updated_size = GX_MULTI_LINE_INDEX_CACHE_SIZE;
    }

    _gx_multi_line_text_view_line_start_cache_create(view, new_first_cache_line, updated_size);

    /* Update cache size. */
    view -> gx_multi_line_text_view_cache_size = (GX_UBYTE)(view -> gx_multi_line_text_view_cache_size + updated_size);

    if (view -> gx_multi_line_text_view_cache_size > GX_MULTI_LINE_INDEX_CACHE_SIZE)
    {
        view -> gx_multi_line_text_view_cache_size = GX_MULTI_LINE_INDEX_CACHE_SIZE;
    }

    /* Update first cache line. */
    view -> gx_multi_line_text_view_first_cache_line = new_first_cache_line;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_tail_cache_update     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to update line index cache to cover        */
/*    following lines.                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    view                                  Multi line text view          */
/*                                            control block               */
/*    new_first_cache_line                  The new line index that cache */
/*                                            start                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_multi_line_text_view_line_tail_cache_create                     */
/*                                          Create line index cache for   */
/*                                            specified range             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_multi_line_text_view_line_cache_update                          */
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
static VOID _gx_multi_line_text_view_line_tail_cache_update(GX_MULTI_LINE_TEXT_VIEW *view, UINT new_first_cache_line)
{
UINT first_cache_line;
UINT updated_size;
UINT last_cache_index;
UINT last_cache_line;

    /* Pickup first cache line. */
    first_cache_line = view -> gx_multi_line_text_view_first_cache_line;

    updated_size = new_first_cache_line - first_cache_line;

    last_cache_index = view -> gx_multi_line_text_view_line_index[GX_MULTI_LINE_INDEX_CACHE_SIZE - 1];
    last_cache_line = view -> gx_multi_line_text_view_first_cache_line + GX_MULTI_LINE_INDEX_CACHE_SIZE - 1;

    if (updated_size < GX_MULTI_LINE_INDEX_CACHE_SIZE)
    {
        /* Move line index value in [updated_size, index_cache_size - 1] to [0, index_cache_size - updated_size]. */
        memmove(view -> gx_multi_line_text_view_line_index, view -> gx_multi_line_text_view_line_index + updated_size,
                sizeof(UINT) * (GX_MULTI_LINE_INDEX_CACHE_SIZE - updated_size));
    }
    else
    {
        updated_size = GX_MULTI_LINE_INDEX_CACHE_SIZE;
    }

    _gx_multi_line_text_view_line_tail_cache_create(view, last_cache_index, last_cache_line,
                                                    new_first_cache_line + GX_MULTI_LINE_INDEX_CACHE_SIZE - updated_size, updated_size);

    /* Update cache size. */
    view -> gx_multi_line_text_view_cache_size = GX_MULTI_LINE_INDEX_CACHE_SIZE;

    /* Update first cache line. */
    view -> gx_multi_line_text_view_first_cache_line = new_first_cache_line;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_view_line_cache_update          PORTABLE C      */
/*                                                           6.1.5        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function updates the line index cache of the displayed text.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Multi line text view          */
/*                                            control block               */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_font_get                   Get font by resource ID       */
/*    _gx_multi_line_text_view_line_start_cache_update                    */
/*                                          Update line index cache to    */
/*                                            cover previous lines        */
/*    _gx_multi_line_text_view_line_tail_cache_update                     */
/*                                          Update line index cache to    */
/*                                            cover following lines       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added logic to disable line */
/*                                            cache process while dynamic */
/*                                            bidi text is enabled,       */
/*                                            resulting in version 6.1    */
/*  03-02-2021     Ting Zhu                 Modified comment(s), fixed    */
/*                                            line height calculation,    */
/*                                            resulting in version 6.1.5  */
/*                                                                        */
/**************************************************************************/
UINT _gx_multi_line_text_view_line_cache_update(GX_MULTI_LINE_TEXT_VIEW *view)
{
UINT     first_visible_line;
UINT     last_visible_line;
UINT     first_cache_line;
INT      new_first_cache_line;
INT      line_height;
GX_FONT *font;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    if (_gx_system_bidi_text_enabled)
    {
        /* No need to update line index cache when dynamic bidi text option is on. */
        return GX_SUCCESS;
    }
#endif

    _gx_widget_font_get((GX_WIDGET *)view, view -> gx_multi_line_text_view_font_id, &font);

    if (font)
    {
        /* Pickup line height. */
        line_height = font -> gx_font_line_height + view -> gx_multi_line_text_view_line_space;

        if (line_height <= 0)
        {
            return GX_FAILURE;
        }

        /* Calcualte first visible line. */
        if (view -> gx_multi_line_text_view_text_scroll_shift < 0)
        {
            first_visible_line = ((UINT)-view -> gx_multi_line_text_view_text_scroll_shift) / (UINT)line_height;
        }
        else
        {
            first_visible_line = 0;
        }

        /* Calculate last visible line. */
        last_visible_line = first_visible_line + view -> gx_multi_line_text_view_text_visible_rows;

        /* Pickuo first cache line. */
        first_cache_line = view -> gx_multi_line_text_view_first_cache_line;

        /* Calculat new first cache line. */
        new_first_cache_line = GX_MULTI_LINE_INDEX_CACHE_SIZE;
        new_first_cache_line -= (INT)(last_visible_line - first_visible_line + 1);
        new_first_cache_line /= 2;
        new_first_cache_line = (INT)first_visible_line - new_first_cache_line;

        if ((UINT)new_first_cache_line + GX_MULTI_LINE_INDEX_CACHE_SIZE > view -> gx_multi_line_text_view_text_total_rows)
        {
            new_first_cache_line = (INT)(view -> gx_multi_line_text_view_text_total_rows - GX_MULTI_LINE_INDEX_CACHE_SIZE);
        }

        if (first_visible_line <= first_cache_line)
        {
            if (new_first_cache_line < 0)
            {
                new_first_cache_line = 0;
            }

            if (new_first_cache_line < (INT)first_cache_line)
            {
                /* Update line start cache. */
                _gx_multi_line_text_view_line_start_cache_update(view, (UINT)new_first_cache_line);
            }
        }
        else if (last_visible_line >= first_cache_line + view -> gx_multi_line_text_view_cache_size - 1)
        {
            if (new_first_cache_line > (INT)first_cache_line)
            {
                /* Update line tail cache. */
                _gx_multi_line_text_view_line_tail_cache_update(view, (UINT)new_first_cache_line);
            }
        }
    }
    return GX_SUCCESS;
}


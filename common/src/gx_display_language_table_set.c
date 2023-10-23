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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_language_table_set                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION  (Deprecated)                                             */
/*                                                                        */
/*    This service sets the language table.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    language_table                        The language table to be set  */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*    number_of_strings                     Number of strings in each     */
/*                                            language                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_all_canvas_dirty           Mark canvas dirty             */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_display_language_table_set(GX_DISPLAY *display, GX_CHAR ***language_table,
                                    GX_UBYTE num_languages, UINT number_of_strings)
{
GX_EVENT            language_event;
GX_WINDOW_ROOT     *root;
GX_CONST GX_CHAR ***old_table = display -> gx_display_language_table_deprecated;

     /* Setup the system string table information.  */
    display -> gx_display_language_table_deprecated = (GX_CONST GX_CHAR ***)language_table;
    display -> gx_display_language_table = GX_NULL;

    display -> gx_display_language_table_size = num_languages;
    display -> gx_display_string_table_size = number_of_strings;

    if (old_table)
    {
        memset(&language_event, 0, sizeof(GX_EVENT));
        language_event.gx_event_type = GX_EVENT_LANGUAGE_CHANGE;

        root = (GX_WINDOW_ROOT *)_gx_system_root_window_created_list;

        while (root)
        {
            if (root -> gx_window_root_canvas)
            {
                if (root -> gx_window_root_canvas -> gx_canvas_display == display)
                {
                    language_event.gx_event_target = (GX_WIDGET *) root;
                    _gx_system_event_send(&language_event);
                }
            }

            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
        }

        /* mark all visible canvases as dirty when there is a resource change */
        _gx_system_all_canvas_dirty();
    }

    /* Return success.  */
    return(GX_SUCCESS);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_language_table_set_ext                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the language table.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    language_table                        The language table to be set  */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*    number_of_strings                     Number of strings in each     */
/*                                            language                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_all_canvas_dirty           Mark canvas dirty             */
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
UINT  _gx_display_language_table_set_ext(GX_DISPLAY *display, GX_CONST GX_STRING **language_table,
                                    GX_UBYTE num_languages, UINT number_of_strings)
{
GX_EVENT   language_event;
GX_WINDOW_ROOT *root;
GX_CONST GX_STRING **old_table = display -> gx_display_language_table;

    /* Setup the system string table information.  */
    display -> gx_display_language_table = language_table;

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    display -> gx_display_language_table_deprecated = GX_NULL;
#endif

    display -> gx_display_language_table_size = num_languages;
    display -> gx_display_string_table_size = number_of_strings;

    if (old_table)
    {
        memset(&language_event, 0, sizeof(GX_EVENT));
        language_event.gx_event_type = GX_EVENT_LANGUAGE_CHANGE;

        root = (GX_WINDOW_ROOT *)_gx_system_root_window_created_list;

        while (root)
        {
            if (root -> gx_window_root_canvas)
            {
                if (root -> gx_window_root_canvas -> gx_canvas_display == display)
                {
                    language_event.gx_event_target = (GX_WIDGET *) root;
                    _gx_system_event_send(&language_event);
                }
            }

            root = (GX_WINDOW_ROOT *)root -> gx_widget_next;
        }

        /* mark all visible canvases as dirty when there is a resource change */
        _gx_system_all_canvas_dirty();
    }

    /* Return success.  */
    return(GX_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_language_direction_table_set_ext        PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the language direction table.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to display            */
/*    language_direction_table              The language direction table  */
/*                                            to be set                   */
/*    num_languages                         Number of languages in the    */
/*                                            table                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT  _gx_display_language_direction_table_set(GX_DISPLAY *display, GX_CONST GX_UBYTE *language_direction_table, GX_UBYTE num_languages)
{

    if (num_languages != display->gx_display_language_table_size)
    {
        return(GX_INVALID_VALUE);
    }

    /* Setup the system string table information.  */
    display -> gx_display_language_direction_table = language_direction_table;

    /* Return success.  */
    return(GX_SUCCESS);
}
#endif

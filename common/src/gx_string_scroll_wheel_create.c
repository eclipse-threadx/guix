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
/**   String Scroll Wheel Management (Scroll Wheel)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_string_scroll_wheel_create                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function creates a string scroll wheel selector widget.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    total_rows                            Total rows of the scroll wheel*/
/*    string_list                           String list for display       */
/*    callback                              Callback for retrieved row    */
/*                                            string                      */
/*    style                                 Style of widget               */
/*    Id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_create          Create a text scroll wheel    */
/*    _gx_widget_link                       Link a widget to parent       */
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
UINT  _gx_string_scroll_wheel_create(GX_STRING_SCROLL_WHEEL *wheel,
                                     GX_CONST GX_CHAR *name,
                                     GX_WIDGET *parent, INT total_rows,
                                     GX_CONST GX_CHAR **string_list,
                                     ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{
UINT status;

    status = _gx_string_scroll_wheel_create_ext(wheel, name, parent, total_rows, GX_NULL, style, Id, size);

    wheel -> gx_string_scroll_wheel_string_list_deprecated = string_list;

    /* Return successful status.  */
    return(status);
}
#endif


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_string_scroll_wheel_create_ext                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a string scroll wheel selector widget.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    total_rows                            Total rows of the scroll wheel*/
/*    string_list                           String list for display       */
/*    callback                              Callback for retrieved row    */
/*                                            string                      */
/*    style                                 Style of widget               */
/*    Id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_create          Create a text scroll wheel    */
/*    _gx_widget_link                       Link a widget to parent       */
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
/*                                            set new event process,      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_string_scroll_wheel_create_ext(GX_STRING_SCROLL_WHEEL *wheel,
                                        GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent, INT total_rows,
                                        GX_CONST GX_STRING *string_list,
                                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{

    /* Call text scroll wheel create. */
    _gx_text_scroll_wheel_create((GX_TEXT_SCROLL_WHEEL *)wheel, name, GX_NULL, total_rows, style, Id, size);

    wheel -> gx_widget_type = GX_TYPE_STRING_SCROLL_WHEEL;
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    wheel -> gx_string_scroll_wheel_string_list_deprecated = GX_NULL;
#endif
    wheel -> gx_string_scroll_wheel_string_list = string_list;
    wheel -> gx_string_scroll_wheel_string_list_buffer_size = 0;
    wheel -> gx_string_scroll_wheel_string_id_list = GX_NULL;
    wheel -> gx_widget_event_process_function = (UINT(*)(GX_WIDGET*, GX_EVENT*))_gx_string_scroll_wheel_event_process;
    wheel -> gx_text_scroll_wheel_text_get = (UINT (*)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *))_gx_string_scroll_wheel_text_get;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)wheel);
    }

    /* Return successful status.  */
    return(GX_SUCCESS);
}


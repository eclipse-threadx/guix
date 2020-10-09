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
#include "gx_scroll_wheel.h"
#include "gx_utility.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_string_scroll_wheel_create                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in string scroll wheel create call. */
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
/*    control_block_size                    Size of the scroll wheel      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_create          Actual text scroll wheel      */
/*                                            create call                 */
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
UINT  _gxe_string_scroll_wheel_create(GX_STRING_SCROLL_WHEEL *wheel,
                                      GX_CONST GX_CHAR *name,
                                      GX_WIDGET *parent, INT total_rows,
                                      GX_CONST GX_CHAR **string_list,
                                      ULONG style, USHORT Id,
                                      GX_CONST GX_RECTANGLE *size, UINT control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer. */
    if ((wheel == GX_NULL) || (size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    /* Check for invalid value. */
    if (control_block_size != sizeof(GX_STRING_SCROLL_WHEEL))
    {
        return GX_INVALID_SIZE;
    }

    /* Check for widget already created.  */
    if (wheel -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for invalid widget. */
    if (parent && (parent -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    status = _gx_string_scroll_wheel_create(wheel, name, parent, total_rows, string_list, style, Id, size);
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_string_scroll_wheel_create_ext                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in string scroll wheel create call. */
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
/*    control_block_size                    Size of the scroll wheel      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_scroll_wheel_create_ext      Actual text scroll wheel      */
/*                                            create ext call             */
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
UINT _gxe_string_scroll_wheel_create_ext(GX_STRING_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name, GX_WIDGET *parent, INT total_rows,
                                         GX_CONST GX_STRING *string_list,
                                         ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT control_block_size)
{
UINT                status;
INT                 string_id;
GX_CONST GX_STRING *string;
UINT                string_length;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer. */
    if ((wheel == GX_NULL) || (size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    /* Check for invalid value. */
    if (control_block_size != sizeof(GX_STRING_SCROLL_WHEEL))
    {
        return GX_INVALID_SIZE;
    }

    /* Check for widget already created.  */
    if (wheel -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for invalid widget. */
    if (parent && (parent -> gx_widget_type == 0))
    {
        return(GX_INVALID_WIDGET);
    }

    if (string_list)
    {
        /* Test string length. */
        for (string_id = 0; string_id < total_rows; string_id++)
        {
            string = &string_list[string_id];
            if (string -> gx_string_ptr)
            {
                status = _gx_utility_string_length_check(string -> gx_string_ptr, &string_length, string -> gx_string_length);

                if (status != GX_SUCCESS)
                {
                    return status;
                }
            }
            else
            {
                string_length = 0;
            }

            if (string_length != string -> gx_string_length)
            {
                return GX_INVALID_STRING_LENGTH;
            }
        }
    }

    status = _gx_string_scroll_wheel_create_ext(wheel, name, parent, total_rows, string_list, style, Id, size);
    return status;
}


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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_generic_scroll_wheel_create                    PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the generic scroll wheel create  */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Scroll wheel control block    */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    total_rows                            Total rows of the scroll wheel*/
/*    callback                              Callback function to create a */
/*                                            widget row                  */
/*    style                                 Style of widget               */
/*    id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*    control_block_size                   Size of the widget scroll      */
/*                                            wheel control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gxe_generic_scroll_wheel_create      Actual generic scroll wheel   */
/*                                            create function             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
UINT  _gxe_generic_scroll_wheel_create(GX_GENERIC_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *name,
                                       GX_WIDGET *parent, INT total_rows,
                                       VOID (*callback)(GX_GENERIC_SCROLL_WHEEL *, GX_WIDGET *, INT),
                                       ULONG style, USHORT id, GX_CONST GX_RECTANGLE *size,
                                       UINT control_block_size)
{

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((wheel == GX_NULL) || (size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (control_block_size != sizeof(GX_GENERIC_SCROLL_WHEEL))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for id is created.  */
    if (wheel -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Check for valid number of rows .  */
    if (total_rows <= 0)
    {
        return GX_INVALID_VALUE;
    }

    /* Check for invalid widget. */
    if (parent && (parent -> gx_widget_type == 0))
    {
        return GX_INVALID_WIDGET;
    }

    /* Call the actual generic scroll wheel create function.  */
    return _gx_generic_scroll_wheel_create(wheel, name, parent, total_rows, callback, style, id, size);
}


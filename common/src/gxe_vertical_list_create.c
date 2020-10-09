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
/**   Vertical List (List)                                                */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_window.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_vertical_list_create                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the vertical list create         */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*    name                                  Name of vertical list         */
/*    parent                                Pointer to parent widget      */
/*    total_rows                            Total number of rows in       */
/*                                            vertical list               */
/*    callback                              User-specified Callback       */
/*                                            function                    */
/*    style                                 Style of scrollbar widget     */
/*    vertical_list_id                      Application-defined ID of     */
/*                                            vertical list               */
/*    size                                  Dimensions of vertical list   */
/*    vertical_list_control_block_size      Size of the vertical list     */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_vertical_list_create              Actual vertical list create   */
/*                                          function                      */
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
UINT  _gxe_vertical_list_create(GX_VERTICAL_LIST *vertical_list, GX_CONST GX_CHAR *name,
                                GX_WIDGET *parent, INT total_rows,
                                VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                                ULONG style, USHORT vertical_list_id, GX_CONST GX_RECTANGLE *size,
                                UINT vertical_list_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((vertical_list == GX_NULL) || (size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (vertical_list_control_block_size != sizeof(GX_VERTICAL_LIST))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for id is created.  */
    if (vertical_list -> gx_widget_type != 0)
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

    /* Call the actual vertical list create function.  */
    status = _gx_vertical_list_create(vertical_list, name, parent, total_rows, callback, style, vertical_list_id, size);

    /* Return completion status.  */
    return status;
}


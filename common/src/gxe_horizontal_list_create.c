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
/**   Horizontal List (List)                                              */
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
/*    _gxe_horizontal_list_create                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the horizontal list create       */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    horizontal_list                       Horizontal list widget control*/
/*                                          block                         */
/*    name                                  Name of horizontal list       */
/*    parent                                Pointer to parent widget      */
/*    total_columns                         Total number of columsn in    */
/*                                          horizontal list               */
/*    style                                 Style of scrollbar widget     */
/*    horizontal_list_id                      Application-defined ID of   */
/*                                          horizontal list               */
/*    size                                  Dimensions of horizontal list */
/*    horizontal_list_control_block_size      Size of the horizontal list */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_horizontal_list_create            Actual horizontal list create */
/*                                            function                    */
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
UINT  _gxe_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list,
                                  GX_CONST GX_CHAR *name,
                                  GX_WIDGET *parent, INT total_columns,
                                  VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                  ULONG style, USHORT horizontal_list_id,
                                  GX_CONST GX_RECTANGLE *size,
                                  UINT horizontal_list_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((horizontal_list == GX_NULL) || (size == GX_NULL))
    {
        return GX_PTR_ERROR;
    }

    if (horizontal_list_control_block_size != sizeof(GX_HORIZONTAL_LIST))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for id is created.  */
    if (horizontal_list -> gx_widget_type != 0)
    {
        return GX_ALREADY_CREATED;
    }

    /* Check for valid number of rows .  */
    if (total_columns <= 0)
    {
        return GX_INVALID_VALUE;
    }

    /* Call the actual horizontal list create function.  */
    status = _gx_horizontal_list_create(horizontal_list, name, parent, total_columns, callback, style, horizontal_list_id, size);

    /* Return completion status.  */
    return status;
}


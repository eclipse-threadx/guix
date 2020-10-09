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
/**   Drop List (List)                                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_drop_list.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_drop_list_create                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the drop list create function    */
/*    call.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Drop list control block       */
/*    name                                  Name of drop list             */
/*    parent                                Pointer to parent widget      */
/*    total_rows                            Total number of rows in       */
/*                                            drop list                   */
/*    open_height                           Height of the vertical list   */
/*    callback                              Callback function             */
/*    style                                 Style of scrollbar widget     */
/*    drop_list_id                          Application-defined ID of     */
/*                                            the drop list               */
/*    size                                  Dimensions of the drop list   */
/*    drop_list_control_block_size          Control block size            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_drop_list_create                  Actual drop list create call  */
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
UINT  _gxe_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name,
                            GX_WIDGET *parent, INT total_rows, INT open_height,
                            VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                            ULONG style, USHORT drop_list_id, GX_CONST GX_RECTANGLE *size, UINT drop_list_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if (!drop_list || !size)
    {
        return(GX_PTR_ERROR);
    }
    
    /* Check for control block size. */
    if (drop_list_control_block_size != sizeof(GX_DROP_LIST))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (drop_list -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call actual widget hide function.  */
    status = _gx_drop_list_create(drop_list, name, parent, total_rows, open_height,
                                  callback, style, drop_list_id, size);

    return(status);
}


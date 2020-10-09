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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_window_root_create                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in window root create function call.*/
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_window                           Root window control block     */
/*    name                                  Name of window                */
/*    canvas                                Canvas this root window       */
/*                                            belongs to                  */
/*    style                                 Style of window               */
/*    Id                                    User-specified root window ID */
/*    size                                  Window size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_root_create                Actual root window create call*/
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
UINT  _gxe_window_root_create(GX_WINDOW_ROOT *root_window,
                              GX_CONST GX_CHAR *name, GX_CANVAS *canvas,
                              ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size, UINT root_window_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((root_window == GX_NULL) || (size == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid input pointers.  */
    if (canvas == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget control block size.  */
    if (root_window_control_block_size != sizeof(GX_WINDOW_ROOT))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (root_window -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call actual root window create function. */
    status = _gx_window_root_create(root_window, name, canvas, style, Id, size);

    return(status);
}


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
/**   Scrollbar Management (Scrollbar)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_scrollbar.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_horizontal_scrollbar_create                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the horizontal scrollbar create  */
/*    function call.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             scroll control block          */
/*    name                                  Name of scroll                */
/*    parent                                parent window                 */
/*    appearance                            style of scroll bar           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_horizontal_scrollbar_create       Actual horizontal scrollbar   */
/*                                          create function               */
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
UINT  _gxe_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name,
                                       GX_WINDOW *parent, GX_SCROLLBAR_APPEARANCE *appearance,
                                       ULONG style, UINT scrollbar_control_block_size)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if (scrollbar == GX_NULL)
    {
        return(GX_PTR_ERROR);
    }

    if (scrollbar_control_block_size != sizeof(GX_SCROLLBAR))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for id is created.  */
    if (scrollbar -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call the actual horizontal scroll*/
    status = _gx_horizontal_scrollbar_create(scrollbar, name, parent, appearance, style);

    /* Return completion status.  */
    return status;
}


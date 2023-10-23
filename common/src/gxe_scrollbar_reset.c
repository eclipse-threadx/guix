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
#include "gx_scrollbar.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_scrollbar_reset                                PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service checks errors in the scrollbar reset function call.    */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scrollbar                             Scrollbar control block       */
/*    info                                  Pointer to GX_SCROLL          */
/*                                            structure that defines the  */
/*                                            scrollbar limits, current   */
/*                                            value, and step/increment.  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status               Completion status                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_scrollbar_reset     The actual function                         */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT _gxe_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info)
{
UINT status;

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid input pointers.  */
    if ((scrollbar == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid widget.  */
    if (scrollbar -> gx_widget_type == 0)
    {
        return(GX_INVALID_WIDGET);
    }

    /* Check for valid scroll info.  */
    if ((info != GX_NULL) && ((info -> gx_scroll_value > info -> gx_scroll_maximum) ||
                              (info -> gx_scroll_value < info -> gx_scroll_minimum)))
    {
        return(GX_INVALID_VALUE);
    }

    /* Call the actual function.  */
    status = _gx_scrollbar_reset(scrollbar, info);

    /* Return successful completion.  */
    return(status);
}


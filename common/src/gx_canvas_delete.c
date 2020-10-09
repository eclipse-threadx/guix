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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_delete                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a canvas.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block and canvas  */
/*                                            memory to zero              */
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
UINT  _gx_canvas_delete(GX_CANVAS *canvas)
{
    /* lock access to gx_system */

    GX_ENTER_CRITICAL

    if (_gx_system_canvas_created_count > 0)
    {
        if (canvas -> gx_canvas_created_previous)
        {
            canvas -> gx_canvas_created_previous -> gx_canvas_created_next = canvas -> gx_canvas_created_next;
            if (canvas -> gx_canvas_created_next)
            {
                canvas -> gx_canvas_created_next -> gx_canvas_created_previous = canvas -> gx_canvas_created_previous;
            }
        }
        else
        {
            _gx_system_canvas_created_list = canvas -> gx_canvas_created_next;

            if (_gx_system_canvas_created_list)
            {
                _gx_system_canvas_created_list -> gx_canvas_created_previous = GX_NULL;
            }
        }

        _gx_system_canvas_created_count--;

        /* Only for Win32, we sometimes have to create module 4 aligned scratch memory buffer */
        #ifdef GX_TARGET_WIN32
        if (canvas -> gx_canvas_padded_memory != GX_NULL)
        {
            free(canvas -> gx_canvas_padded_memory);
        }
        #endif

        /* Clear the screen control block.  */
        memset(canvas, 0, sizeof(GX_CANVAS));
    }
    /* unlock gx_system.  */
    GX_EXIT_CRITICAL

    /* Return successful status.  */
    return(GX_SUCCESS);
}


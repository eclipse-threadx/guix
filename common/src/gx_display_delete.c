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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_delete                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a previous-created display                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    display_driver_cleanup                Display driver cleanup routine*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [display_driver_cleanup]              Call the user-supplied        */
/*                                            display driver cleanup      */
/*                                            routine                     */
/*    memset                                Cleanup the memory            */
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
UINT  _gx_display_delete(GX_DISPLAY *display, VOID (*display_driver_cleanup)(GX_DISPLAY *))
{
GX_DISPLAY *previous_display;

    if (_gx_system_display_created_count > 0)
    {
        display_driver_cleanup(display);
        _gx_system_display_created_count--;

        if (display -> gx_display_created_previous)
        {
            /* this is not first display, unlink it: */

            previous_display = display -> gx_display_created_previous;
            previous_display -> gx_display_created_next = display -> gx_display_created_next;

            if (display -> gx_display_created_next)
            {
                display -> gx_display_created_next -> gx_display_created_previous = previous_display;
            }
        }
        else
        {
            /* this is the first display, unlink it */
            _gx_system_display_created_list = display -> gx_display_created_next;

            if (_gx_system_display_created_list)
            {
                _gx_system_display_created_list -> gx_display_created_previous = GX_NULL;
            }
        }
        memset(display, 0, sizeof(GX_DISPLAY));

        /* Return successful status.  */
        return(GX_SUCCESS);
    }
    return GX_FAILURE;
}


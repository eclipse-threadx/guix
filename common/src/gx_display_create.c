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
/*    _gx_display_create                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates the display and calls the display driver      */
/*    setup function.                                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*    name                                  Name of display               */
/*    display_driver_setup                  Display driver setup function */
/*    width                                 Display width in pixels       */
/*    height                                Display height in pixels      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block to zero     */
/*    _gx_system_error_process              System error handler          */
/*    [dispaly_driver_setup]                Call display driver setup     */
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
UINT  _gx_display_create(GX_DISPLAY *display, GX_CONST GX_CHAR *name,
                         UINT (*display_driver_setup)(GX_DISPLAY *),
                         GX_VALUE width, GX_VALUE height)
{

UINT status;

    /* Clear the display control block.  */
    memset(display, 0, sizeof(GX_DISPLAY));
    display -> gx_display_width = width;
    display -> gx_display_height = height;

    /* Call the display driver setup function. This function initializes the underlying
       hardware and sets up all the primitive drawing function pointers.  */

    status =  display_driver_setup(display);

    /* Determine if the display driver setup was successful.  */
    if (status)
    {
        /* Error setting up display driver - call system error handler.  */
        _gx_system_error_process(GX_SYSTEM_DRIVER_SETUP_ERROR);

        /* Return to system error.  */
        return(GX_SYSTEM_ERROR);
    }

    /* Load the display ID field in the display control block.  */
    display -> gx_display_id =  GX_DISPLAY_ID;

    /* Save the display name.  */
    display -> gx_display_name =  name;

    /* Place the display on the list of created displays.
       First, check for an empty list.  */

    _gx_system_display_created_count++;

    if (_gx_system_display_created_list)
    {
        _gx_system_display_created_list -> gx_display_created_previous = display;
        display -> gx_display_created_next = _gx_system_display_created_list;
    }

    _gx_system_display_created_list =      display;

    /* Return successful status.  */
    return(GX_SUCCESS);
}


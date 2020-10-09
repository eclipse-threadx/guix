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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_unlock                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function unlocks the GUIX system mutex                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    tx_mutex_put                          Release the mutex             */
/*     _gx_system_error_process             Process system error          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
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
VOID  _gx_system_unlock(VOID)
{
#ifdef GX_THREADX_BINDING
UINT protection_status;
#endif

    if (_gx_system_lock_nesting > 0)
    {
        _gx_system_lock_nesting--;

        if (_gx_system_lock_nesting == 0)
        {
            /* no thread has GUIX locked */
            _gx_system_lock_thread = GX_NULL;

            /* Release the protection prior to blocking on the event queue.  */

#ifdef GX_THREADX_BINDING
            protection_status =  tx_mutex_put(&_gx_system_protect);
            /* Determine if the protection was successfully released.  */
            if (protection_status)
            {

                /* Error releasing protection - call system error handler.  */
                _gx_system_error_process(GX_SYSTEM_PROTECTION_ERROR);

                /* Return to exit the system thread.  */
                return;
            }
#else
            GX_SYSTEM_MUTEX_UNLOCK;
#endif
        }
    }
}


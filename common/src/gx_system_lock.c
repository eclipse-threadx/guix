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
/*    _gx_system_lock                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function locks the GUIX system mutex                           */
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
/*    tx_mutex_get                      Grab a mutex                      */
/*    _tx_thread_identify               Identify current thread           */
/*    _gx_system_error_process          Process an error                  */
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
VOID _gx_system_lock(VOID)
{
#ifdef GX_THREADX_BINDING
UINT protection_status;
#endif

    /* if the current thread already owns the GUIX system protection
       then we don't need to lock again, just increment our nesting counter.
       If no thread has yet locked GUIX or if the caller is not the owning
       thread, then we need to request the gx_system_protect mutex.
     */

    if (_gx_system_lock_thread != GX_CURRENT_THREAD)
    {
#ifdef GX_THREADX_BINDING
        protection_status =  tx_mutex_get(&_gx_system_protect, TX_WAIT_FOREVER);
        /* Determine if we successfully obtained the protection.  */
        if (protection_status)
        {
            /* Error obtaining protection - call system error handler.  */
            _gx_system_error_process(GX_SYSTEM_PROTECTION_ERROR);

            /* Return to exit the system thread.  */
            return;
        }
#else
        GX_SYSTEM_MUTEX_LOCK;
#endif



        /* save which thread owns the GUIX locked */
        _gx_system_lock_thread = GX_CURRENT_THREAD;
    }
    _gx_system_lock_nesting++;
}


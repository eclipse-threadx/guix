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
#include "gx_widget.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_event_remove                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes events targetted to indicated widget          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    remove                                Widget to be removed          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_child_detect               Detect a child widget         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_widget_delete                     Delete a widget               */
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
VOID  _gx_system_event_remove(GX_WIDGET *widget)
{
#ifdef GX_THREADX_BINDING

TX_INTERRUPT_SAVE_AREA

GX_EVENT *pEvent;
ULONG    *pSrc;
TX_QUEUE *pQueue;
GX_BOOL   Purge;

    TX_DISABLE

    pQueue = &_gx_system_event_queue;

    if (pQueue -> tx_queue_enqueued)
    {
        pSrc =  pQueue -> tx_queue_read;

        do
        {
            pEvent = (GX_EVENT *)pSrc;
            Purge = GX_FALSE;

            if (pEvent -> gx_event_target)
            {
                if (pEvent -> gx_event_target == widget)
                {
                    Purge = GX_TRUE;
                }
                else
                {
                    _gx_widget_child_detect(widget, pEvent -> gx_event_target, &Purge);
                }
                if (Purge)
                {
                    pEvent -> gx_event_target = GX_NULL;
                    pEvent -> gx_event_type = 0;
                }
            }

            pSrc += pQueue -> tx_queue_message_size;

            if (pSrc >= pQueue -> tx_queue_end)
            {
                pSrc = pQueue -> tx_queue_start;
            }
        } while (pSrc != pQueue->tx_queue_write);
    }
    TX_RESTORE
#else
    GX_EVENT_PURGE(widget);
#endif
}


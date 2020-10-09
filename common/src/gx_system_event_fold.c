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
/*    _gx_system_event_fold                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service folds GUIX events.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    event                                 Pointer to event              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    TX_DISABLE                                                          */
/*    TX_RESTORE                                                          */
/*    _gx_system_pen_speed_update                                         */
/*    _gx_system_event_send                 Send GUIX events              */
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
UINT  _gx_system_event_fold(GX_EVENT *in_event)
{
#ifdef GX_THREADX_BINDING
TX_INTERRUPT_SAVE_AREA

GX_EVENT *pEvent;
ULONG    *pSrc;
TX_QUEUE *pQueue;

    TX_DISABLE

    pQueue = &_gx_system_event_queue;

    if (pQueue -> tx_queue_enqueued)
    {
        pSrc =  pQueue -> tx_queue_read;

        do
        {
            pEvent = (GX_EVENT *)pSrc;

            if (pEvent -> gx_event_type == in_event -> gx_event_type &&
                pEvent -> gx_event_target == in_event -> gx_event_target)
            {
                /* we found a matching event, just update the existing event data
                   rather than posting a new event
                 */

                /* for timer event, update tick count */
                if (pEvent -> gx_event_type == GX_EVENT_TIMER)
                {
                    pEvent -> gx_event_payload.gx_event_ulongdata++;
                }
                else
                {
                    /* for all other event types, just copy payload */
                    pEvent -> gx_event_payload.gx_event_ulongdata = in_event -> gx_event_payload.gx_event_ulongdata;
                }
                TX_RESTORE

                if (pEvent -> gx_event_type == GX_EVENT_PEN_DRAG)
                {
                    _gx_system_pen_speed_update(&pEvent -> gx_event_payload.gx_event_pointdata);
                }
                return GX_SUCCESS;
            }

            pSrc += pQueue -> tx_queue_message_size;

            if (pSrc >= pQueue -> tx_queue_end)
            {
                pSrc = pQueue -> tx_queue_start;
            }
        } while (pSrc != pQueue->tx_queue_write);
    }

    TX_RESTORE

    /* we didn't find a matching event, so post a new event */
    return _gx_system_event_send(in_event);
#else
    return(GX_EVENT_FOLD(in_event));
#endif
}


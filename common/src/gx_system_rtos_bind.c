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

//#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_system_rtos_bind.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_rtos_bind                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains a small set of shell functions used to bind GUIX */
/*    to a generic RTOS. GUIX is by default configured to run with the    */
/*    ThreadX RTOS, but you can port to another RTOS by defining the      */
/*    pre-processor directive GX_DISABLE_THREADX_BINDING and completing   */
/*    the implementation of the functions found in this file.             */
/*    Refer to the GUIX User Guide for more information.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    Refer to GUIX User Guide                                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX system serives                                                 */
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

#ifndef GX_THREADX_BINDING

/* This is an example of a generic RTOS binding. We have #ifdefed this code out
   of the GUIX library build, so that the user attempting to port to a new RTOS
   will intentionally observe a series of linker errors. These functions need to
   be implemented in a way that works correctly with the target RTOS.
*/

#if defined(GUIX_BINDING_UCOS_III)
 
#include "os.h"

#ifndef OS_CFG_TICK_RATE_HZ
#define OS_CFG_TICK_RATE_HZ 100
#endif

#define GX_TIMER_TASK_PRIORITY   (GX_SYSTEM_THREAD_PRIORITY + 1)
#define GX_TIMER_TASK_STACK_SIZE 512
void guix_timer_task_entry(void *);

/* a few global status variables */
GX_BOOL guix_timer_event_pending;

/* define stack space for GUIX task */
CPU_STK guix_task_stack[GX_THREAD_STACK_SIZE];

/* define stack space for timer task */
CPU_STK guix_timer_task_stack[GX_TIMER_TASK_STACK_SIZE];

/* define semaphore for lock/unlock macros */
OS_MUTEX guix_system_lock_mutex;

OS_ERR uc_error;
OS_TCB guix_tcb;
OS_TCB guix_timer_tcb;

/* a custom event structure, to hold event and linked list */

typedef struct guix_linked_event_struct
{
    GX_EVENT                         event_data;        /* the GUIX event structure */
    struct guix_linked_event_struct *next;              /* add link member          */
} GUIX_LINKED_EVENT;


/* a custom fifo event queue structure */

typedef struct guix_event_queue_struct
{
    GUIX_LINKED_EVENT *first;           /* first (oldest) event in fifo */
    GUIX_LINKED_EVENT *last;            /* last (newest) event in fifo  */
    GUIX_LINKED_EVENT *free;            /* linked list of free events   */
    GUIX_LINKED_EVENT *free_end;        /* end of the free list         */
    OS_SEM             count_sem;       /* number of queued events      */
    OS_MUTEX           lock;            /* lock to protect queue update */
} GUIX_EVENT_QUEUE;

/* an array of GX_EVENTs used to implement fifo queue */

GUIX_LINKED_EVENT guix_event_array[GX_MAX_QUEUE_EVENTS];
GUIX_EVENT_QUEUE  guix_event_queue;

/* rtos initialize: perform any setup that needs to be done before the GUIX task runs here */
VOID   gx_generic_rtos_initialize(VOID)
{
int Loop;
GUIX_LINKED_EVENT *current;

    guix_timer_event_pending = GX_FALSE;
    OSMutexCreate(&guix_system_lock_mutex, "gx_system_lock", &uc_error);

    /* initialize a custom fifo queue structure */

    guix_event_queue.first = GX_NULL;
    guix_event_queue.last = GX_NULL;
    guix_event_queue.free = guix_event_array;

    current = guix_event_queue.free;

    /* link all the structures together */
    for (Loop = 0; Loop < GX_MAX_QUEUE_EVENTS - 1; Loop++)
    {
        current -> next = (current + 1);
        current = current -> next;
    }
    current -> next = GX_NULL;                /* terminate the list */
    guix_event_queue.free_end = current;

    /* create mutex for lock access to this queue */
    OSMutexCreate(&guix_event_queue.lock, "gx_queue_lock", &uc_error);

    /* create counting semaphore to track queue entries */
    OSSemCreate(&guix_event_queue.count_sem, "gx_queue_count", 0, &uc_error);
}

VOID (*gx_system_thread_entry)(ULONG);

// A small shell function to convert the void * arg expected by uC/OS to
// a ULONG parameter expected by GUIX thread entry
void gx_system_thread_entry_shell(void *parg)
{
    gx_system_thread_entry((ULONG) parg);
}

/* thread_start: start the GUIX thread running. */
UINT   gx_generic_thread_start(VOID(*guix_thread_entry)(ULONG))
{

    /* save the GUIX system thread entry function pointer */
    gx_system_thread_entry = guix_thread_entry;

    /* create the main GUIX task */

    OSTaskCreate(&guix_tcb, "guix_task",
                 gx_system_thread_entry_shell,
                 GX_NULL,
                 GX_SYSTEM_THREAD_PRIORITY,
                 &guix_task_stack[0],
                 GX_THREAD_STACK_SIZE - 1,
                 GX_THREAD_STACK_SIZE,
                 0, 0, GX_NULL, OS_OPT_TASK_NONE, &uc_error);

    /* create a simple task to generate timer events into GUIX */
    OSTaskCreate(&guix_timer_tcb, "guix_timer_task",
                 guix_timer_task_entry,
                 GX_NULL,
                 GX_TIMER_TASK_PRIORITY,
                 &guix_timer_task_stack[0],
                 GX_TIMER_TASK_STACK_SIZE -1,
                 GX_TIMER_TASK_STACK_SIZE,
                 0, 0, GX_NULL, OS_OPT_TASK_NONE, &uc_error);

    /* suspend the timer task until needed */
    OSTaskSuspend(&guix_timer_tcb, &uc_error);
    return GX_SUCCESS;
}

/* event_post: push an event into the fifo event queue */
UINT   gx_generic_event_post(GX_EVENT *event_ptr)
{
GUIX_LINKED_EVENT *linked_event;

    /* lock down the guix event queue */
    OSMutexPend(&guix_event_queue.lock, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

    /* grab a free event slot */
    if (!guix_event_queue.free)
    {
        /* there are no free events, return failure */
        OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);
        return GX_FAILURE;
    }

    linked_event = guix_event_queue.free;
    guix_event_queue.free = guix_event_queue.free->next;

    if (!guix_event_queue.free)
    {
        guix_event_queue.free_end = GX_NULL;
    }

    /* copy the event data into this slot */
    linked_event -> event_data = *event_ptr;
    linked_event -> next = GX_NULL;

    /* insert this event into fifo queue */
    if (guix_event_queue.last)
    {
        guix_event_queue.last -> next = linked_event;
    }
    else
    {
        guix_event_queue.first = linked_event;
    }
    guix_event_queue.last = linked_event;

    /* Unlock the guix queue */
    OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);

    /* increment event count */
    OSSemPost(&guix_event_queue.count_sem, OS_OPT_POST_ALL, &uc_error);
    return GX_SUCCESS;
}

/* event_fold: update existing matching event, otherwise post new event */
UINT   gx_generic_event_fold(GX_EVENT *event_ptr)
{
GUIX_LINKED_EVENT *pTest;

    /* Lock down the guix queue */

    OSMutexPend(&guix_event_queue.lock, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

    // see if the same event is already in the queue:
    pTest = guix_event_queue.first;

    while (pTest)
    {
        if (pTest -> event_data.gx_event_type == event_ptr -> gx_event_type)
        {
            /* found matching event, update it and return */
            pTest -> event_data.gx_event_payload.gx_event_ulongdata = event_ptr -> gx_event_payload.gx_event_ulongdata;
            OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);
            return GX_SUCCESS;
        }
        pTest = pTest -> next;
    }

    /* did not find a match, push new event */

    OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);
    gx_generic_event_post(event_ptr);
    return GX_SUCCESS;
}


/* event_pop: pop oldest event from fifo queue, block if wait and no events exist */
UINT   gx_generic_event_pop(GX_EVENT *put_event, GX_BOOL wait)
{
    if (!wait)
    {
        if (guix_event_queue.first == GX_NULL)
        {
            /* the queue is empty, just return */
            return GX_FAILURE;
        }
    }

    /* wait for an event to arrive in queue */
    OSSemPend(&guix_event_queue.count_sem, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

    /* lock down the queue */
    OSMutexPend(&guix_event_queue.lock, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

    /* copy the event into destination */
    *put_event = guix_event_queue.first -> event_data;

    /* link this event holder back into free list */
    if (guix_event_queue.free_end)
    {
        guix_event_queue.free_end -> next = guix_event_queue.first;
    }
    else
    {
        guix_event_queue.free = guix_event_queue.first;
    }

    guix_event_queue.free_end = guix_event_queue.first;
    guix_event_queue.first = guix_event_queue.first -> next;
    guix_event_queue.free_end -> next = GX_NULL;

    if (!guix_event_queue.first)
    {
        guix_event_queue.last = GX_NULL;
    }

    /* check for popping out a timer event */
    if (put_event -> gx_event_type == GX_EVENT_TIMER)
    {
        guix_timer_event_pending = GX_FALSE;
    }

    /* unlock the queue */
    OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);
    return GX_SUCCESS;
}

/* event_purge: delete events targetted to particular widget */
VOID   gx_generic_event_purge(GX_WIDGET *target)
{
GX_BOOL Purge;
GUIX_LINKED_EVENT *pTest;

    /* Lock down the guix queue */

    OSMutexPend(&guix_event_queue.lock, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

    // look for events targetted to widget or widget children:
    pTest = guix_event_queue.first;

    while (pTest)
    {
        Purge = GX_FALSE;

        if (pTest ->event_data.gx_event_target)
        {
            if (pTest -> event_data.gx_event_target == target)
            {
                Purge = GX_TRUE;
            }
            else
            {
                gx_widget_child_detect(target, pTest->event_data.gx_event_target, &Purge);
            }
            if (Purge)
            {
                pTest ->event_data.gx_event_target = GX_NULL;
                pTest->event_data.gx_event_type = 0;
            }
        }
        pTest = pTest -> next;
    }

    OSMutexPost(&guix_event_queue.lock, OS_OPT_POST_NONE, &uc_error);
}

/* start the RTOS timer */
VOID   gx_generic_timer_start(VOID)
{
    OSTaskResume(&guix_timer_tcb, &uc_error);
}

/* stop the RTOS timer */
VOID   gx_generic_timer_stop(VOID)
{
    OSTaskSuspend(&guix_timer_tcb, &uc_error);
}

/* lock the system protection mutex */
VOID   gx_generic_system_mutex_lock(VOID)
{
    OSMutexPend(&guix_system_lock_mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &uc_error);

}

/* unlock system protection mutex */
VOID   gx_generic_system_mutex_unlock(VOID)
{
    OSMutexPost(&guix_system_lock_mutex, OS_OPT_POST_NONE, &uc_error);
}

/* return number of low-level system timer ticks. Used for pen speed caculations */
ULONG  gx_generic_system_time_get(VOID)
{
    return((ULONG)OSTimeGet(&uc_error));
}

/* thread_identify: return current thread identifier, cast as VOID * */
VOID  *gx_generic_thread_identify(VOID)
{
    return((VOID *)OSTCBCurPtr);
}

/* a simple task to drive the GUIX timer mechanism */
void guix_timer_task_entry(void *unused)
{
int TickCount = OS_CFG_TICK_RATE_HZ / GX_TICKS_SECOND;

    if (TickCount <= 0)
    {
        TickCount = 1;
    }

    while (1)
    {
        OSTimeDly(TickCount, OS_OPT_TIME_DLY, &uc_error);

        /* prevent sending timer events faster than they can be processed */
        if (!guix_timer_event_pending)
        {
            guix_timer_event_pending = GX_TRUE;
            _gx_system_timer_expiration(0);
        }
    }
}

VOID gx_generic_time_delay(int ticks)
{
    OSTimeDly(ticks, OS_OPT_TIME_DLY, &uc_error); 
}


#endif  /* Binding for UCOS-III enabled */

#if defined(GUIX_BINDING_UCOS_II)

#include "ucos_ii.h"

#define TIMER_TASK_PRIORITY   (GX_SYSTEM_THREAD_PRIORITY + 1)
#define TIMER_TASK_STACK_SIZE 512
void guix_timer_task_entry(void *);

/* a few global status variables */
GX_BOOL guix_timer_event_pending;
INT8U   guix_uc_err = 0;

/* define stack space for GUIX task */
OS_STK guix_task_stack[GX_THREAD_STACK_SIZE];
OS_STK guix_task_stack[GX_THREAD_STACK_SIZE];

/* define stack space for timer task */
OS_STK guix_timer_task_stack[TIMER_TASK_STACK_SIZE];

/* define semaphore for lock/unlock macros */
OS_EVENT *guix_system_lock_semaphore;

/* a custom event structure, to hold event and linked list */

typedef struct guix_linked_event_struct
{
    GX_EVENT                         event_data;        /* the GUIX event structure */
    struct guix_linked_event_struct *next;              /* add link member          */
} GUIX_LINKED_EVENT;


/* a custom fifo event queue structure */

typedef struct guix_event_queue_struct
{
    GUIX_LINKED_EVENT *first;           /* first (oldest) event in fifo */
    GUIX_LINKED_EVENT *last;            /* last (newest) event in fifo  */
    GUIX_LINKED_EVENT *free;            /* linked list of free events   */
    GUIX_LINKED_EVENT *free_end;        /* end of the free list         */
    OS_EVENT          *count_sem;       /* number of queued events      */
    OS_EVENT          *lock_sem;        /* lock to protect queue update */
} GUIX_EVENT_QUEUE;

/* an array of GX_EVENTs used to implement fifo queue */

GUIX_LINKED_EVENT guix_event_array[GX_MAX_QUEUE_EVENTS];
GUIX_EVENT_QUEUE  guix_event_queue;

/* rtos initialize: perform any setup that needs to be done before the GUIX task runs here */
VOID   gx_generic_rtos_initialize(VOID)
{
int Loop;
GUIX_LINKED_EVENT *current;

    guix_timer_event_pending = GX_FALSE;
    guix_system_lock_semaphore = OSSemCreate(1);

    /* initialize a custom fifo queue structure */

    guix_event_queue.first = GX_NULL;
    guix_event_queue.last = GX_NULL;
    guix_event_queue.free = guix_event_array;

    current = guix_event_queue.free;

    /* link all the structures together */
    for (Loop = 0; Loop < GX_MAX_QUEUE_EVENTS - 1; Loop++)
    {
        current -> next = (current + 1);
        current = current -> next;
    }
    current -> next = GX_NULL;                /* terminate the list */
    guix_event_queue.free_end = current;

    /* create mutex for lock access to this queue */
    guix_event_queue.lock_sem = OSSemCreate(1);

    /* create counting semaphore to track queue entries */
    guix_event_queue.count_sem = OSSemCreate(0);
}

/* A variable to store GUIX thread entry function pointer */
VOID (*gx_thread_entry)(ULONG);

/* a simple shell function to convert the UCOS (void *)
   parameter to the GUIX ULONG param type. */
void guix_thread_entry_shell(void *ptr)
{
    gx_thread_entry((ULONG) ptr);
}

/* thread_start: start the GUIX thread running. */
UINT   gx_generic_thread_start(VOID(*guix_thread_entry)(ULONG))
{
    /* save pointer to GUIX thread entry function */
    gx_thread_entry = guix_thread_entry;

    /* create the main GUIX task */
    OSTaskCreate(guix_thread_entry_shell, (void *)0,
                 &guix_task_stack[GX_THREAD_STACK_SIZE - 1], GX_SYSTEM_THREAD_PRIORITY);

    /* create a simple task to generate timer events into GUIX */
    OSTaskCreate(guix_timer_task_entry, (void *)0,
                 &guix_timer_task_stack[TIMER_TASK_STACK_SIZE - 1], TIMER_TASK_PRIORITY);

    /* suspend the timer task until needed */
    OSTaskSuspend(TIMER_TASK_PRIORITY);
    return GX_SUCCESS;
}

/* event_post: push an event into the fifo event queue */
UINT   gx_generic_event_post(GX_EVENT *event_ptr)
{
GUIX_LINKED_EVENT *linked_event;

    /* lock down the guix event queue */
    OSSemPend(guix_event_queue.lock_sem, 0, &guix_uc_err);

    /* grab a free event slot */
    if (!guix_event_queue.free)
    {
        /* there are no free events, return failure */
        OSSemPost(guix_event_queue.lock_sem);
        return GX_FAILURE;
    }

    linked_event = guix_event_queue.free;
    guix_event_queue.free = guix_event_queue.free->next;

    if (!guix_event_queue.free)
    {
        guix_event_queue.free_end = GX_NULL;
    }

    /* copy the event data into this slot */
    linked_event -> event_data = *event_ptr;
    linked_event -> next = GX_NULL;

    /* insert this event into fifo queue */
    if (guix_event_queue.last)
    {
        guix_event_queue.last -> next = linked_event;
    }
    else
    {
        guix_event_queue.first = linked_event;
    }
    guix_event_queue.last = linked_event;

    /* Unlock the guix queue */
    OSSemPost(guix_event_queue.lock_sem);

    /* increment event count */
    OSSemPost(guix_event_queue.count_sem);
    return GX_SUCCESS;
}

/* event_fold: update existing matching event, otherwise post new event */
UINT   gx_generic_event_fold(GX_EVENT *event_ptr)
{
GUIX_LINKED_EVENT *pTest;

    /* Lock down the guix queue */

    OSSemPend(guix_event_queue.lock_sem, 0, &guix_uc_err);

    // see if the same event is already in the queue:
    pTest = guix_event_queue.first;

    while (pTest)
    {
        if (pTest -> event_data.gx_event_type == event_ptr -> gx_event_type)
        {
            /* found matching event, update it and return */
            pTest -> event_data.gx_event_payload.gx_event_ulongdata = event_ptr -> gx_event_payload.gx_event_ulongdata;
            OSSemPost(guix_event_queue.lock_sem);
            return GX_SUCCESS;
        }
        pTest = pTest -> next;
    }

    /* did not find a match, push new event */

    OSSemPost(guix_event_queue.lock_sem);
    gx_generic_event_post(event_ptr);
    return GX_SUCCESS;
}


/* event_pop: pop oldest event from fifo queue, block if wait and no events exist */
UINT   gx_generic_event_pop(GX_EVENT *put_event, GX_BOOL wait)
{
    if (!wait)
    {
        if (guix_event_queue.first == GX_NULL)
        {
            /* the queue is empty, just return */
            OSSemPost(guix_event_queue.lock_sem);
            return GX_FAILURE;
        }
    }

    /* wait for an event to arrive in queue */
    OSSemPend(guix_event_queue.count_sem, 0, &guix_uc_err);

    /* lock down the queue */
    OSSemPend(guix_event_queue.lock_sem, 0, &guix_uc_err);

    /* copy the event into destination */
    *put_event = guix_event_queue.first -> event_data;

    /* link this event holder back into free list */
    if (guix_event_queue.free_end)
    {
        guix_event_queue.free_end -> next = guix_event_queue.first;
    }
    else
    {
        guix_event_queue.free = guix_event_queue.first;
    }

    guix_event_queue.free_end = guix_event_queue.first;
    guix_event_queue.first = guix_event_queue.first -> next;
    guix_event_queue.free_end -> next = GX_NULL;

    if (!guix_event_queue.first)
    {
        guix_event_queue.last = GX_NULL;
    }

    /* check for popping out a timer event */
    if (put_event -> gx_event_type == GX_EVENT_TIMER)
    {
        guix_timer_event_pending = GX_FALSE;
    }

    /* unlock the queue */
    OSSemPost(guix_event_queue.lock_sem);
    return GX_SUCCESS;
}

/* event_purge: delete events targetted to particular widget */
VOID   gx_generic_event_purge(GX_WIDGET *target)
{
GX_BOOL Purge;
GUIX_LINKED_EVENT *pTest;

    /* Lock down the guix queue */

    OSSemPend(guix_event_queue.lock_sem, 0, &guix_uc_err);

    // look for events targetted to widget or widget children:
    pTest = guix_event_queue.first;

    while (pTest)
    {
        Purge = GX_FALSE;

        if (pTest ->event_data.gx_event_target)
        {
            if (pTest -> event_data.gx_event_target == target)
            {
                Purge = GX_TRUE;
            }
            else
            {
                gx_widget_child_detect(target, pTest->event_data.gx_event_target, &Purge);
            }
            if (Purge)
            {
                pTest ->event_data.gx_event_target = GX_NULL;
                pTest->event_data.gx_event_type = 0;
            }
        }
        pTest = pTest -> next;
    }

    OSSemPost(guix_event_queue.lock_sem);
}

/* start the RTOS timer */
VOID   gx_generic_timer_start(VOID)
{
    OSTaskResume(TIMER_TASK_PRIORITY);
}

/* stop the RTOS timer */
VOID   gx_generic_timer_stop(VOID)
{
    OSTaskSuspend(TIMER_TASK_PRIORITY);
}

/* lock the system protection mutex */
VOID   gx_generic_system_mutex_lock(VOID)
{
    OSSemPend(guix_system_lock_semaphore, 0, &guix_uc_err);
}

/* unlock system protection mutex */
VOID   gx_generic_system_mutex_unlock(VOID)
{
    OSSemPost(guix_system_lock_semaphore);
}

/* return number of low-level system timer ticks. Used for pen speed caculations */
ULONG  gx_generic_system_time_get(VOID)
{
    return((ULONG)OSTimeGet());
}

/* thread_identify: return current thread identifier, cast as VOID * */
VOID  *gx_generic_thread_identify(VOID)
{
    return((VOID *)OSTCBCur);
}

/* a simple task to drive the GUIX timer mechanism */
VOID guix_timer_task_entry(VOID *unused)
{
int TickCount = OS_TICKS_PER_SEC / GX_TICKS_SECOND;

    if (TickCount <= 0)
    {
        TickCount = 1;
    }

    while (1)
    {
        OSTimeDly(TickCount);

        /* prevent sending timer events faster than they can be processed */
        if (!guix_timer_event_pending)
        {
            guix_timer_event_pending = GX_TRUE;
            _gx_system_timer_expiration(0);
        }
    }
}

VOID gx_generic_time_delay(int ticks)
{
    OSTimeDly(ticks); 
}


#endif /* Binding for UCOS-II enabled */

#endif  /* if !ThreadX */



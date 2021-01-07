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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/
#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_resolved_text_info_delete          PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a reordered bidi text information link.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    reordered_text                        The head pointer of reordered */
/*                                            bidi text information link  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT _gx_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head)
{
GX_BIDI_RESOLVED_TEXT_INFO *info;
GX_BIDI_RESOLVED_TEXT_INFO *next;

    if (!_gx_system_memory_free)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    info = *resolved_info_head;

    while (info)
    {
        next = info -> gx_bidi_resolved_text_info_next;
        _gx_system_memory_free(info);
        info = next;
    }

    *resolved_info_head = GX_NULL;

    return GX_SUCCESS;
}
#endif


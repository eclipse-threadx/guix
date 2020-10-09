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
/**   Numeric Prompt Management (Prompt)                                  */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_numeric_prompt.h                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX numeric prompt management component,     */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
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

#ifndef GX_NUMERIC_PROMPT_H
#define GX_NUMERIC_PROMPT_H


/* Define prompt management function prototypes.  */

UINT _gx_numeric_prompt_create(GX_NUMERIC_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                               GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size);
VOID _gx_numeric_prompt_format(GX_NUMERIC_PROMPT *prompt, INT value);
UINT _gx_numeric_prompt_format_function_set(GX_NUMERIC_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PROMPT *, INT));
UINT _gx_numeric_prompt_value_set(GX_NUMERIC_PROMPT *prompt, INT value);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_numeric_prompt_create(GX_NUMERIC_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id, ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size,
                                UINT control_block_size);
UINT _gxe_numeric_prompt_format_function_set(GX_NUMERIC_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PROMPT *, INT));
UINT _gxe_numeric_prompt_value_set(GX_NUMERIC_PROMPT *prompt, INT value);
#endif


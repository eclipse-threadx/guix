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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_prompt.h                                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX prompt management component,             */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new prototypes,       */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/

#ifndef GX_PROMPT_H
#define GX_PROMPT_H


/* Define prompt management function prototypes.  */

UINT _gx_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                       GX_RESOURCE_ID text_id,
                       ULONG style, USHORT prompt_id,
                       GX_CONST GX_RECTANGLE *size);
VOID _gx_prompt_draw(GX_PROMPT *prompt);
UINT _gx_prompt_event_process(GX_PROMPT *prompt, GX_EVENT *event_ptr);
UINT _gx_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
VOID _gx_prompt_text_draw(GX_PROMPT *prompt);
UINT _gx_prompt_text_color_set(GX_PROMPT *prompt,
                               GX_RESOURCE_ID normal_text_color_id,
                               GX_RESOURCE_ID selected_text_color_id,
                               GX_RESOURCE_ID disabled_text_color_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_prompt_text_get(GX_PROMPT *prompt, GX_CONST GX_CHAR **return_text);
UINT _gx_prompt_text_set(GX_PROMPT *prompt, GX_CONST GX_CHAR *text);
#endif
UINT _gx_prompt_text_get_ext(GX_PROMPT *prompt, GX_STRING *return_text);
UINT _gx_prompt_text_set_ext(GX_PROMPT *prompt, GX_CONST GX_STRING *text);

UINT _gx_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_prompt_create(GX_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent, GX_RESOURCE_ID text_id,
                        ULONG style, USHORT prompt_id, GX_CONST GX_RECTANGLE *size, UINT prompt_control_block_size);
UINT _gxe_prompt_event_process(GX_PROMPT *prompt, GX_EVENT *event_ptr);
UINT _gxe_prompt_font_set(GX_PROMPT *prompt, GX_RESOURCE_ID fontid);
UINT _gxe_prompt_text_color_set(GX_PROMPT *prompt,
                                GX_RESOURCE_ID normal_text_color_id,
                                GX_RESOURCE_ID selected_text_color_id,
                                GX_RESOURCE_ID disabled_text_color_id);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gxe_prompt_text_get(GX_PROMPT *prompt, GX_CONST GX_CHAR **return_text);
UINT _gxe_prompt_text_set(GX_PROMPT *prompt, GX_CONST GX_CHAR *text);
#endif
UINT _gxe_prompt_text_get_ext(GX_PROMPT *prompt, GX_STRING *return_text);
UINT _gxe_prompt_text_set_ext(GX_PROMPT *prompt, GX_CONST GX_STRING *text);

UINT _gxe_prompt_text_id_set(GX_PROMPT *prompt, GX_RESOURCE_ID string_id);
#endif


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

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)

#define GX_LINK_RESOLVED_BIDI_TEXT_INFO                         \
    if (pre)                                                    \
    {                                                           \
        pre -> gx_bidi_resolved_text_info_next = resolved_info; \
    }                                                           \
    pre = resolved_info;                                        \
    if (!head)                                                  \
    {                                                           \
        head = pre;                                             \
    }

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_buffer_allocate                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate character count to be handled */
/*    and allocate a buffer for following processing.                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            update with bidi context    */
/*                                            structure change,           */
/*                                            moved a pointer check to    */
/*                                            its caller function,        */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_buffer_allocate(GX_BIDI_CONTEXT *context)
{
INT                  character_count = 0;
GX_STRING            text;
ULONG                unicode;
INT                  max_depth = 0;
INT                  depth = 0;
INT                  level_run_count = 1;
INT                  bracket_pair_count = 0;
INT                  max_bracket_pair = 0;
INT                  run_stack_size;
INT                  explicite_stack_size;
GX_BIDI_BRACKET_PAIR bracket_pair;
GX_BIDI_TEXT_INFO   *input_info = context -> gx_bidi_context_input_info;

    text = input_info -> gx_bidi_text_info_text;

    while (text.gx_string_length > 0)
    {
        if ((text.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN) ||
            (text.gx_string_ptr[0] == GX_KEY_LINE_FEED))
        {
            break;
        }

        /* Convert utf8 to unicode. */
        unicode = 0;
        _gx_utility_utf8_string_character_get(&text, (GX_CHAR_CODE *)&unicode, GX_NULL);

        switch (unicode)
        {
        case GX_BIDI_DIRECTION_FORMAT_RLE:
        case GX_BIDI_DIRECTION_FORMAT_LRE:
        case GX_BIDI_DIRECTION_FORMAT_RLO:
        case GX_BIDI_DIRECTION_FORMAT_LRO:
        case GX_BIDI_DIRECTION_FORMAT_RLI:
        case GX_BIDI_DIRECTION_FORMAT_LRI:
        case GX_BIDI_DIRECTION_FORMAT_FSI:
            depth++;
            level_run_count++;
            break;

        case GX_BIDI_DIRECTION_FORMAT_PDF:
        case GX_BIDI_DIRECTION_FORMAT_PDI:
            depth--;
            level_run_count++;
            break;
        default:
            if (max_bracket_pair < GX_BIDI_MAX_BRACKET_DEPTH)
            {

                _gx_utility_bidi_bracket_pair_get(unicode, &bracket_pair);
                if (bracket_pair.gx_bidi_bracket_pair_open)
                {
                    if (bracket_pair.gx_bidi_bracket_pair_open == unicode)
                    {
                        bracket_pair_count++;
                        if (bracket_pair_count > max_bracket_pair)
                        {
                            max_bracket_pair = bracket_pair_count;
                        }
                    }
                }
            }
            break;
        }

        if (depth > max_depth)
        {
            max_depth = depth;
        }

        character_count++;
    }

    if (max_depth > GX_BIDI_MAX_EXPLICIT_DEPTH + 1)
    {
        max_depth = GX_BIDI_MAX_EXPLICIT_DEPTH + 1;
    }

    context -> gx_bidi_context_processced_size = input_info -> gx_bidi_text_info_text.gx_string_length - text.gx_string_length;
    context -> gx_bidi_context_reordered_utf8_size = context -> gx_bidi_context_processced_size;
    context -> gx_bidi_context_unit_count = character_count;

    context -> gx_bidi_context_buffer_size = sizeof(GX_BIDI_UNIT) * (UINT)(character_count);         /* unit list size. */

    if (input_info -> gx_bidi_text_info_font && input_info -> gx_bidi_text_info_display_width > 0)
    {
        context -> gx_bidi_context_buffer_size += sizeof(ULONG) * (UINT)character_count; /* line index cache size. */
    }

    /* Size of buffer used for loading explicit level data
       Will be released before level run buffer is used. */
    explicite_stack_size = (INT)sizeof(GX_BIDI_EXPLICIT_ENTRY) * (max_depth + 1);

    /* Size of buffer used for loading level run data. */
    run_stack_size = (INT)sizeof(GX_BIDI_LEVEL_RUN) * character_count;

    /* Size of buffer used for loading isolate run data. */
    run_stack_size += (INT)sizeof(GX_BIDI_ISOLATE_RUN) * level_run_count + (INT)sizeof(ULONG) * character_count;

    /* Size of buffer used for loading bracket pair data. */
    context -> gx_bidi_context_bracket_pair_size = sizeof(ULONG) * (UINT)(max_bracket_pair * 2);
    run_stack_size += (INT)context -> gx_bidi_context_bracket_pair_size;
    run_stack_size += (INT)sizeof(ULONG) * (INT)max_bracket_pair;

    if (run_stack_size < explicite_stack_size)
    {
        run_stack_size = explicite_stack_size;
    }

    context -> gx_bidi_context_buffer_size += (UINT)run_stack_size;

    context -> gx_bidi_context_buffer = (GX_UBYTE *)_gx_system_memory_allocator(context -> gx_bidi_context_buffer_size);
    if (context -> gx_bidi_context_buffer == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(context -> gx_bidi_context_buffer, 0, (size_t)context -> gx_bidi_context_buffer_size);

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_initiate                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to convert utf8 text to unicode, initiate  */
/*    initiate character types and calculate the total lines.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    font                                  Text font, if not set, the    */
/*                                            text is handled as a single */
/*                                            line                        */
/*    display_width                         The max width of text in one  */
/*                                            line , if not set, the text */
/*                                            is handled as a single line */
/*    total_lines                           Total lines of handled text   */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            removed line breaking logic,*/
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_initiate(GX_BIDI_CONTEXT *context)
{
GX_BIDI_TEXT_INFO *input_info = context -> gx_bidi_context_input_info;
INT                index = 0;
GX_STRING          string;
GX_CHAR_CODE       code;
GX_BIDI_UNIT      *unit;

    context -> gx_bidi_context_unit_list = (GX_BIDI_UNIT *)context -> gx_bidi_context_buffer;
    context -> gx_bidi_context_buffer_index += sizeof(GX_BIDI_UNIT) * (UINT)context -> gx_bidi_context_unit_count;

    string = input_info -> gx_bidi_text_info_text;

    while (index < context -> gx_bidi_context_unit_count)
    {
        unit = &context -> gx_bidi_context_unit_list[index];

        /* Convert utf8 to unicode. */
        _gx_utility_utf8_string_character_get(&string, &code, GX_NULL);
        unit -> gx_bidi_unit_code = code;

        _gx_utility_bidi_character_type_get(unit -> gx_bidi_unit_code, &unit -> gx_bidi_unit_type);
        unit -> gx_bidi_unit_org_type = unit -> gx_bidi_unit_type;

        index++;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_block_level_determine              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate the level of the specified    */
/*    text block                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    start_index                           The start index of the text   */
/*                                            block                       */
/*    end_index                             The end index of the text     */
/*                                            block                       */
/*    level                                 Buffer for level value        */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*    _gx_utility_bidi_explicit_levels_determine                          */
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
static UINT _gx_utility_bidi_block_level_determine(GX_BIDI_CONTEXT *context, UINT start_index, UINT end_index, GX_UBYTE *level)
{
INT           isolate_count = 0;
GX_BIDI_UNIT *unit;

    unit = &context -> gx_bidi_context_unit_list[start_index];

    while (start_index <= end_index)
    {
        /* Find the first character of type L, AL, or R
           while skipping over any hcaracters between an isolate initiator and its matching PDI. */
        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_AL:
        case GX_BIDI_CHARACTER_TYPE_R:
            if (isolate_count == 0)
            {
                *level = 1;
                return GX_SUCCESS;
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_L:
            if (isolate_count == 0)
            {
                *level = 0;
                return GX_SUCCESS;
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_RLI:
        case GX_BIDI_CHARACTER_TYPE_LRI:
            isolate_count++;
            break;

        case GX_BIDI_CHARACTER_TYPE_PDI:
            if (isolate_count)
            {
                isolate_count--;
            }
            break;
        }

        start_index++;
        unit++;
    }

    /* Otherwise, set paragraph embedding level to 0. */
    *level = 0;
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_explicit_levels_determine          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate explicite level of every      */
/*    character.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*                                            format.                     */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_explicit_levels_determine(GX_BIDI_CONTEXT *context)
{
GX_BIDI_EXPLICIT_ENTRY *stack;
GX_BIDI_EXPLICIT_ENTRY *entry;
GX_BIDI_EXPLICIT_ENTRY *last_entry;
INT                     stack_top = 0;
INT                     overflow_isolate_count = 0;
INT                     overflow_embedding_count = 0;
INT                     valid_isolate_count = 0;
INT                     index = 0;
INT                     end_index;
GX_UBYTE                level;
ULONG                   type;
GX_BIDI_UNIT           *unit;

    stack = (GX_BIDI_EXPLICIT_ENTRY *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);

    if (context -> gx_bidi_context_buffer_index > context -> gx_bidi_context_buffer_size)
    {
        return GX_INVALID_MEMORY_SIZE;
    }
    entry = stack;

    /* Push onto the stack an entry consisting of the paragraph embedding level,
       a neutral directional override status, and a false directional isolate status. */
    entry -> gx_bidi_explicit_level = context -> gx_bidi_context_base_level;
    entry -> gx_bidi_explicit_override_status = GX_BIDI_OVERRIDE_STATUS_N;
    entry -> gx_bidi_explicit_isolate_status = GX_FALSE;
    last_entry = entry;
    entry++;
    stack_top++;

    unit = &context -> gx_bidi_context_unit_list[index];

    while (index < context -> gx_bidi_context_unit_count)
    {
        type = unit -> gx_bidi_unit_type;

        switch (type)
        {
        case GX_BIDI_CHARACTER_TYPE_RLE:
        case GX_BIDI_CHARACTER_TYPE_LRE:
        case GX_BIDI_CHARACTER_TYPE_RLO:
        case GX_BIDI_CHARACTER_TYPE_LRO:
            /* Handle explicit embeddings/overrides. */
            entry -> gx_bidi_explicit_level = (GX_UBYTE)(last_entry -> gx_bidi_explicit_level + 1);
            unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_BN;

            if (type == GX_BIDI_CHARACTER_TYPE_RLE ||
                type == GX_BIDI_CHARACTER_TYPE_RLO)
            {
                /* Compute the least odd embedding level greater than the embedding level of the last entry. */
                if ((entry -> gx_bidi_explicit_level & 0x01) == 0)
                {
                    entry -> gx_bidi_explicit_level = (GX_UBYTE)(entry -> gx_bidi_explicit_level + 1);
                }
            }
            else
            {
                /* Compute the least even embedding level greater than the embedding level of the last entry. */
                if ((entry -> gx_bidi_explicit_level & 0x01) != 0)
                {
                    entry -> gx_bidi_explicit_level = (GX_UBYTE)(entry -> gx_bidi_explicit_level + 1);
                }
            }

            if ((entry -> gx_bidi_explicit_level <= GX_BIDI_MAX_EXPLICIT_DEPTH) &&
                (overflow_isolate_count == 0) &&
                (overflow_embedding_count == 0))
            {
                /* Push an entry to stack. */

                if (type == GX_BIDI_CHARACTER_TYPE_RLE ||
                    type == GX_BIDI_CHARACTER_TYPE_LRE)
                {
                    entry -> gx_bidi_explicit_override_status = GX_BIDI_OVERRIDE_STATUS_N;
                }
                else if (type == GX_BIDI_CHARACTER_TYPE_RLO)
                {
                    entry -> gx_bidi_explicit_override_status = GX_BIDI_OVERRIDE_STATUS_R;
                }
                else
                {
                    entry -> gx_bidi_explicit_override_status = GX_BIDI_OVERRIDE_STATUS_L;
                }

                entry -> gx_bidi_explicit_isolate_status = GX_FALSE;
                last_entry = entry;
                entry++;
                stack_top++;
            }
            else if (overflow_isolate_count == 0)
            {
                overflow_embedding_count++;
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_RLI:
        case GX_BIDI_CHARACTER_TYPE_LRI:
        case GX_BIDI_CHARACTER_TYPE_FSI:
            if (type == GX_BIDI_CHARACTER_TYPE_FSI)
            {
                for (end_index = index + 1; end_index < context -> gx_bidi_context_unit_count; end_index++)
                {
                    if (context -> gx_bidi_context_unit_list[end_index].gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_PDI)
                    {
                        break;
                    }
                }

                _gx_utility_bidi_block_level_determine(context, (UINT)index, (UINT)end_index, &level);

                if (level == 1)
                {
                    type = GX_BIDI_CHARACTER_TYPE_RLI;
                }
                else
                {
                    type = GX_BIDI_CHARACTER_TYPE_LRI;
                }
            }

            /* Set the RLI's embedding level to the embedding level of the last entry. */
            unit -> gx_bidi_unit_level = last_entry -> gx_bidi_explicit_level;
            entry -> gx_bidi_explicit_level = (GX_UBYTE)(last_entry -> gx_bidi_explicit_level + 1);

            if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_L)
            {
                /* Reset the current character type from RLI to L if last override status is left to right. */
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_L;
            }
            else if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_R)
            {
                /* Reset the current character type from RLI to R if last overried status is right to left. */
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_R;
            }

            if ((type == GX_BIDI_CHARACTER_TYPE_RLI && (entry -> gx_bidi_explicit_level & 0x01) == 0) ||
                (type == GX_BIDI_CHARACTER_TYPE_LRI && (entry -> gx_bidi_explicit_level & 0x01) != 0))
            {
                entry -> gx_bidi_explicit_level = (GX_UBYTE)(entry -> gx_bidi_explicit_level + 1);
            }

            if (entry -> gx_bidi_explicit_level <= GX_BIDI_MAX_EXPLICIT_DEPTH &&
                overflow_isolate_count == 0 &&
                overflow_embedding_count == 0)
            {
                valid_isolate_count++;
                entry -> gx_bidi_explicit_override_status = GX_BIDI_OVERRIDE_STATUS_N;
                entry -> gx_bidi_explicit_isolate_status = GX_TRUE;
                last_entry = entry;
                entry++;
                stack_top++;
            }
            else
            {
                /* This is an overflow RLI/LRI. */
                overflow_isolate_count++;
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_PDF:
            unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_BN;
            if (overflow_isolate_count == 0)
            {
                if (overflow_embedding_count)
                {
                    overflow_embedding_count--;
                }
                else if (last_entry -> gx_bidi_explicit_isolate_status == GX_FALSE &&
                         stack_top >= 2)
                {
                    last_entry--;
                    entry--;
                    stack_top--;
                }
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_PDI:
            if (overflow_isolate_count > 0)
            {
                /* This PDI matches an overflow isolate initiator. */
                overflow_isolate_count--;
            }
            else if (valid_isolate_count != 0)
            {
                overflow_embedding_count = 0;

                while ((stack_top > 1) &&
                       (last_entry -> gx_bidi_explicit_isolate_status == GX_FALSE))
                {
                    /* Pop the last entry. */
                    last_entry--;
                    entry--;
                    stack_top--;
                }

                last_entry--;
                entry--;
                stack_top--;
                valid_isolate_count--;
            }

            unit -> gx_bidi_unit_level = last_entry -> gx_bidi_explicit_level;

            if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_L)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_L;
            }
            else if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_R)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_R;
            }
            break;

        case GX_BIDI_CHARACTER_TYPE_BN:
            break;

        default:
            /* Set current character's embedding level to the embedding level of the last entry. */
            unit -> gx_bidi_unit_level = last_entry -> gx_bidi_explicit_level;

            if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_L)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_L;
            }
            else if (last_entry -> gx_bidi_explicit_override_status == GX_BIDI_OVERRIDE_STATUS_R)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_R;
            }
            break;
        }

        index++;
        unit++;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_level_runs_compute                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate the level runs of the         */
/*    specified text block                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    start_index                           The start index of the text   */
/*                                            block                       */
/*    end_index                             The end index of the text     */
/*                                            block                       */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_level_runs_compute(GX_BIDI_CONTEXT *context, INT start_index, INT end_index)
{
INT                run_start_index = -1;
INT                run_end_index;
INT                index = start_index;
GX_BIDI_LEVEL_RUN *entry;
GX_BIDI_LEVEL_RUN *last_entry = GX_NULL;
GX_UBYTE           run_level;
GX_BIDI_UNIT      *unit;

    if (context -> gx_bidi_context_level_runs == GX_NULL)
    {
        context -> gx_bidi_context_level_runs = (GX_BIDI_LEVEL_RUN *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);
        context -> gx_bidi_context_buffer_index += sizeof(GX_BIDI_LEVEL_RUN) * (UINT)context -> gx_bidi_context_unit_count;

        if (context -> gx_bidi_context_buffer_index > context -> gx_bidi_context_buffer_size)
        {
            return GX_INVALID_MEMORY_SIZE;
        }
    }

    entry = context -> gx_bidi_context_level_runs;

    unit = &context -> gx_bidi_context_unit_list[index];
    while (index <= end_index)
    {
        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_BN:
            break;

        default:
            if (run_start_index == -1)
            {
                run_start_index = index;
                run_level = unit -> gx_bidi_unit_level;
            }
            else if (run_level != unit -> gx_bidi_unit_level)
            {
                entry -> gx_bidi_level_run_start_index = run_start_index;
                entry -> gx_bidi_level_run_end_index = run_end_index;
                entry -> gx_bidi_level_run_level = run_level;
                entry -> gx_bidi_level_run_next = GX_NULL;

                if (last_entry)
                {
                    last_entry -> gx_bidi_level_run_next = entry;
                }
                last_entry = entry;
                entry++;
                run_start_index = index;
                run_level = unit -> gx_bidi_unit_level;
            }
            run_end_index = index;
            break;
        }

        index++;
        unit++;
    }

    entry -> gx_bidi_level_run_start_index = run_start_index;
    entry -> gx_bidi_level_run_end_index = run_end_index;
    entry -> gx_bidi_level_run_level = run_level;
    entry -> gx_bidi_level_run_next = GX_NULL;

    if (last_entry)
    {
        last_entry -> gx_bidi_level_run_next = entry;
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utiltiy_isolate_run_sequence_append             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to append a sequence to the specified      */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    start_index                           Start position of the append  */
/*                                            sequence                    */
/*    end_index                             End position of the append    */
/*                                            sequence                    */
/*    siolate_run                           Pointer to isolate run        */
/*                                            structure                   */
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
/*    _gx_utility_bidi_one_isolate_run_sequences_get                      */
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
static UINT _gx_utiltiy_isolate_run_sequence_append(GX_BIDI_CONTEXT *context, INT start_index, INT end_index,
                                                    GX_BIDI_ISOLATE_RUN *isolate_run)
{
INT           index;
GX_BIDI_UNIT *unit;

    unit = &context -> gx_bidi_context_unit_list[start_index];
    for (index = start_index; index <= end_index; index++)
    {
        if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_BN)
        {
            unit++;
            continue;
        }

        isolate_run -> gx_bidi_isolate_run_index_list[isolate_run -> gx_bidi_isolate_run_index_count++] = index;
        context -> gx_bidi_context_buffer_index += sizeof(ULONG);

        unit++;
    }

    if (context -> gx_bidi_context_buffer_index > context -> gx_bidi_context_buffer_size)
    {
        return GX_INVALID_MEMORY_SIZE;
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_one_isolate_run_sequence_get       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate one isolate run sequence      */
/*    start from the specified level run.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    pre_level                             Embedding level of the        */
/*                                            previous level run          */
/*    level_run                             Pointer to level run structure*/
/*    isolate_run                           Buffer to load an isolate run */
/*                                            sequence                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utiltiy_isolate_run_sequence_append                             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_bidi_isolate_run_sequence_get                           */
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
static UINT _gx_utility_bidi_one_isolate_run_sequence_get(GX_BIDI_CONTEXT *context, GX_UBYTE  pre_level,
                                                          GX_BIDI_LEVEL_RUN *level_run, GX_BIDI_ISOLATE_RUN *isolate_run)
{
UINT          status;
GX_UBYTE      next_level;
GX_UBYTE      run_level;
GX_UBYTE      higher_level;
INT           start_index;
INT           end_index;
UINT          isolate_initiator_count;
GX_BIDI_UNIT *unit;
ULONG         last_character;

    run_level = level_run -> gx_bidi_level_run_level;
    start_index = level_run -> gx_bidi_level_run_start_index;
    end_index = level_run -> gx_bidi_level_run_end_index;

    isolate_run -> gx_bidi_isolate_run_index_count = 0;

    status = _gx_utiltiy_isolate_run_sequence_append(context, start_index, end_index, isolate_run);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    isolate_run -> gx_bidi_isolate_run_next = GX_NULL;

    last_character = context -> gx_bidi_context_unit_list[end_index].gx_bidi_unit_code;

    level_run = level_run -> gx_bidi_level_run_next;
    isolate_initiator_count = 1;

    next_level = (level_run == GX_NULL) ? context -> gx_bidi_context_base_level : level_run -> gx_bidi_level_run_level;

    while (level_run)
    {
        /* while the level run last in the sequence ends with an isolate initiator that has a matching PDI,
           append the level run containing the matching PDI to the isolate run sequence. */
        if ((last_character != GX_BIDI_DIRECTION_FORMAT_RLI) &&
            (last_character != GX_BIDI_DIRECTION_FORMAT_LRI) &&
            (last_character != GX_BIDI_DIRECTION_FORMAT_FSI))
        {
            break;
        }

        start_index = level_run -> gx_bidi_level_run_start_index;
        end_index = level_run -> gx_bidi_level_run_end_index;

        unit = &context -> gx_bidi_context_unit_list[start_index];
        if (unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_PDI)
        {
            isolate_initiator_count--;
            if (isolate_initiator_count == 0)
            {
                /* This is a matching PDI. */
                status = _gx_utiltiy_isolate_run_sequence_append(context, start_index, end_index, isolate_run);

                if (status != GX_SUCCESS)
                {
                    return status;
                }

                last_character = context -> gx_bidi_context_unit_list[end_index].gx_bidi_unit_code;

                if (level_run -> gx_bidi_level_run_next)
                {
                    next_level = level_run -> gx_bidi_level_run_next -> gx_bidi_level_run_level;
                }
                else
                {
                    next_level = context -> gx_bidi_context_base_level;
                }
            }
        }

        unit = &context -> gx_bidi_context_unit_list[end_index];

        if (unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_LRI ||
            unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_RLI ||
            unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_FSI)
        {
            isolate_initiator_count++;
        }

        level_run = level_run -> gx_bidi_level_run_next;
    }

    /* Determine sos. */
    higher_level = (pre_level > run_level) ? pre_level : run_level;
    isolate_run -> gx_bidi_isolate_run_sos = ((higher_level & 0x01) == 1) ? GX_BIDI_CHARACTER_TYPE_R : GX_BIDI_CHARACTER_TYPE_L;

    /* Determine eos. */
    higher_level = (next_level > run_level) ? next_level : run_level;
    isolate_run -> gx_bidi_isolate_run_eos = ((higher_level & 0x01) == 1) ? GX_BIDI_CHARACTER_TYPE_R : GX_BIDI_CHARACTER_TYPE_L;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_isolate_run_sequences_get          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate the isolate run sequences.    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_isolate_run_sequences_get(GX_BIDI_CONTEXT *context)
{
UINT                 status;
INT                  level_run_start_index;
INT                  level_run_end_index;
INT                  isolate_initiator_count = 0;
GX_BIDI_LEVEL_RUN   *level_run_entry;
GX_BIDI_ISOLATE_RUN *isolate_run_entry;
GX_BIDI_ISOLATE_RUN *last_isolate_run_entry = GX_NULL;
GX_UBYTE             pre_level = context -> gx_bidi_context_base_level;
GX_BIDI_UNIT        *unit;

    /* Compute level runs. */
    status = _gx_utility_bidi_level_runs_compute(context, 0, context -> gx_bidi_context_unit_count - 1);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    /* Compute isolate runs. */
    context -> gx_bidi_context_isolate_runs = (GX_BIDI_ISOLATE_RUN *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);

    level_run_entry = context -> gx_bidi_context_level_runs;
    while (level_run_entry)
    {
        /* For each level run in the paragraph whose first characer is not a PDI,
           or is a PDI that does not match any isolate initiator,
           create a new isolate run sequence and initiate it to the level run. */
        level_run_start_index = level_run_entry -> gx_bidi_level_run_start_index;
        level_run_end_index = level_run_entry -> gx_bidi_level_run_end_index;

        unit = &context -> gx_bidi_context_unit_list[level_run_start_index];

        if ((unit -> gx_bidi_unit_code != GX_BIDI_DIRECTION_FORMAT_PDI) ||
            (isolate_initiator_count == 0))
        {
            isolate_run_entry = (GX_BIDI_ISOLATE_RUN *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);
            context -> gx_bidi_context_buffer_index += (INT)(sizeof(GX_BIDI_ISOLATE_RUN));
            isolate_run_entry -> gx_bidi_isolate_run_index_list = (INT *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);

            if (context -> gx_bidi_context_buffer_index > context -> gx_bidi_context_buffer_size)
            {
                return GX_INVALID_MEMORY_SIZE;
            }

            /* Get one isolate run sequence. */
            _gx_utility_bidi_one_isolate_run_sequence_get(context, pre_level, level_run_entry, isolate_run_entry);

            if (last_isolate_run_entry)
            {
                last_isolate_run_entry -> gx_bidi_isolate_run_next = isolate_run_entry;
            }

            last_isolate_run_entry = isolate_run_entry;
        }

        if (unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_PDI)
        {
            isolate_initiator_count--;
        }

        unit = &context -> gx_bidi_context_unit_list[level_run_end_index];

        if (unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_LRI ||
            unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_RLI ||
            unit -> gx_bidi_unit_code == GX_BIDI_DIRECTION_FORMAT_FSI)
        {
            isolate_initiator_count++;
        }

        pre_level = level_run_entry -> gx_bidi_level_run_level;
        level_run_entry = level_run_entry -> gx_bidi_level_run_next;
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_1                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 1 to weak characters in an   */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_1(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index;
INT           text_index;
GX_UBYTE      pre_type = 0;
GX_BIDI_UNIT *unit;

    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_NSM)
        {
            if (pre_type == 0)
            {
                /* If the NSM is at the start of the isolating run sequence, it will get the type of sos. */
                unit -> gx_bidi_unit_type = entry -> gx_bidi_isolate_run_sos;
            }
            else
            {
                switch (pre_type)
                {
                case GX_BIDI_CHARACTER_TYPE_RLI:
                case GX_BIDI_CHARACTER_TYPE_LRI:
                case GX_BIDI_CHARACTER_TYPE_PDI:
                    /* If the previous character type is an isolate initiator or PDI, change type to ON. */
                    unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_ON;
                    break;

                default:
                    /* Otherwise, change type to the type of previous character. */
                    unit -> gx_bidi_unit_type = pre_type;
                    break;
                }
            }
        }

        pre_type = unit -> gx_bidi_unit_type;
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_2_3              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 2 and 3 to weak characters in*/
/*    an isolate run sequence.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_2_3(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index = 0;
INT           text_index;
GX_BOOL       do_change = GX_FALSE;
GX_BIDI_UNIT *unit;

    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        /* Search backward from each instance of a EN until the first strong type is found,
           If AL is found, replacing following EN to AN. */

        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_R:
        case GX_BIDI_CHARACTER_TYPE_L:
            do_change = GX_FALSE;
            break;

        case GX_BIDI_CHARACTER_TYPE_AL:
            do_change = GX_TRUE;

            /* Change all ALs to R. */
            unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_R;
            break;

        case GX_BIDI_CHARACTER_TYPE_EN:
            if (do_change)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_AN;
            }
            break;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_4                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 4 to weak characters in an   */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_4(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index;
INT           text_index;
GX_UBYTE      pre_type = 0;
GX_UBYTE      follow_type = 0;
GX_BIDI_UNIT *unit;

    /* A single ES between two EN, change to EN,
       a single CS between two EN, change to EN,
       a single CS between two AN, change to AN. */

    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        if (run_index + 1 < entry -> gx_bidi_isolate_run_index_count)
        {
            text_index = entry -> gx_bidi_isolate_run_index_list[run_index + 1];
            follow_type = context -> gx_bidi_context_unit_list[text_index].gx_bidi_unit_type;
        }

        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if (pre_type == follow_type)
        {
            if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_ES &&
                pre_type == GX_BIDI_CHARACTER_TYPE_EN)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_EN;
            }
            else if ((unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_CS) &&
                     (pre_type == GX_BIDI_CHARACTER_TYPE_EN || pre_type == GX_BIDI_CHARACTER_TYPE_AN))
            {
                unit -> gx_bidi_unit_type = pre_type;
            }
            run_index++;
        }
        else
        {
            pre_type = unit -> gx_bidi_unit_type;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_5                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 5 to weak characters in an   */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_5(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{

INT           run_index;
INT           pre_run_index;
INT           text_index;
GX_BOOL       do_change = GX_FALSE;
GX_BIDI_UNIT *unit;

    /* A sequence of ETs adjacent to EN, change to all ENs. */
    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_EN)
        {
            do_change = GX_TRUE;

            pre_run_index = run_index - 1;

            while (pre_run_index >= 0)
            {
                text_index = entry -> gx_bidi_isolate_run_index_list[pre_run_index--];
                unit = &context -> gx_bidi_context_unit_list[text_index];

                if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_ET)
                {
                    unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_EN;
                }
                else
                {
                    break;
                }
            }
        }
        else if (do_change && unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_ET)
        {
            unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_EN;
        }
        else
        {
            do_change = GX_FALSE;
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_6                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 6 to weak characters in an   */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_6(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index;
INT           text_index;
GX_BIDI_UNIT *unit;

    /* ES, CS, ET change to ON. */
    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_ES:
        case GX_BIDI_CHARACTER_TYPE_CS:
        case GX_BIDI_CHARACTER_TYPE_ET:
            unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_ON;
            break;
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_weak_type_resolve_7                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 7 to weak characters in an   */
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_weak_type_resolve_7(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index;
INT           text_index;
GX_BOOL       do_change = GX_FALSE;
GX_BIDI_UNIT *unit;

    /* Search backward from each EN until first strong type (R, L, or SOS) is found, if
       L is found, change type to L.*/
    if (entry -> gx_bidi_isolate_run_sos == GX_BIDI_CHARACTER_TYPE_L)
    {
        do_change = GX_TRUE;
    }
    else
    {
        do_change = GX_FALSE;
    }

    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_R:
            do_change = GX_FALSE;
            break;

        case GX_BIDI_CHARACTER_TYPE_L:
            do_change = GX_TRUE;
            break;

        default:
            if (do_change && unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_EN)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_L;
            }
            break;
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_bracket_pair_search                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to calculate bracket pairs in an isolate   */
/*    run sequence.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
/*    return_bracket_pair                   Buffer to load bracket pair   */
/*                                            list                        */
/*    return_pair_count                     Buffer to load the number of  */
/*                                            bracket pair characters     */
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
/*    _gx_utility_bidi_neutral_type_resolve_0                             */
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
static UINT _gx_utility_bidi_bracket_pair_search(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry, INT **return_bracket_pair, INT *return_pair_count)
{
ULONG               *stack;
ULONG               *bracket_entry;
ULONG               *last_bracket_entry = GX_NULL;
INT                  top = 0;
INT                  run_index;
INT                  text_index;
ULONG                character;
INT                 *bracket_pair = *return_bracket_pair;
INT                  bracket_pair_count = 0;
GX_BIDI_BRACKET_PAIR pair;
UINT                 buffer_index;
INT                  depth;
INT                  temp = 0;
GX_BIDI_UNIT        *unit;

    stack = (ULONG *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);
    buffer_index = context -> gx_bidi_context_buffer_index;
    bracket_entry = stack;

    for (run_index = 0; run_index < (INT)entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if (unit -> gx_bidi_unit_type != GX_BIDI_CHARACTER_TYPE_ON)
        {
            continue;
        }

        character = unit -> gx_bidi_unit_code;
        _gx_utility_bidi_bracket_pair_get(character, &pair);

        if (character == pair.gx_bidi_bracket_pair_open)
        {
            if (top >= 63)
            {
                /* Stop processing for the remainder to the isolating run sequence. */
                break;
            }

            last_bracket_entry = bracket_entry;
            *bracket_entry = (ULONG)run_index;
            bracket_entry++;
            top++;
            buffer_index += (INT)(sizeof(ULONG));
            if (buffer_index > context -> gx_bidi_context_buffer_size)
            {
                return GX_INVALID_MEMORY_SIZE;
            }
        }
        else if (last_bracket_entry && (character == pair.gx_bidi_bracket_pair_close))
        {
            depth = 0;

            while (top - depth > 0)
            {
                text_index = entry -> gx_bidi_isolate_run_index_list[*(last_bracket_entry - depth)];
                unit = &context -> gx_bidi_context_unit_list[text_index];

                if (unit -> gx_bidi_unit_code == pair.gx_bidi_bracket_pair_open ||
                    (unit -> gx_bidi_unit_code == 0x2329 && pair.gx_bidi_bracket_pair_open == 0x3008) ||
                    (unit -> gx_bidi_unit_code == 0x3008 && pair.gx_bidi_bracket_pair_open == 0x2329))
                {
                    bracket_pair[bracket_pair_count++] = (INT)(*(last_bracket_entry - depth));
                    bracket_pair[bracket_pair_count++] = run_index;
                    depth++;

                    last_bracket_entry -= depth;
                    bracket_entry -= depth;
                    top -= depth;
                    buffer_index -= sizeof(ULONG) * (UINT)depth;
                    break;
                }
                depth++;
            }
        }
    }

    /* Sort list of pairs of text position in ascending order
       based on the text position of the openging paired bracket. */
    for (run_index = 0; run_index < bracket_pair_count - 2; run_index += 2)
    {
        for (text_index = run_index + 2; text_index < bracket_pair_count; text_index += 2)
        {
            if (bracket_pair[run_index] > bracket_pair[text_index])
            {
                temp = bracket_pair[run_index];
                bracket_pair[run_index] = bracket_pair[text_index];
                bracket_pair[text_index] = temp;

                temp = bracket_pair[run_index + 1];
                bracket_pair[run_index + 1] = bracket_pair[text_index + 1];
                bracket_pair[text_index + 1] = temp;
            }
        }
    }

    *return_pair_count = bracket_pair_count;
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_neutral_type_resolve_0             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 0 to neutral characters in an*/
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_neutral_type_resolve_0(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
UINT          status;
INT           index = 0;
INT           start_index;
INT           end_index;
INT           text_index;
INT          *bracket_pair;
INT           bracket_pair_count = 0;
GX_UBYTE      embedding_type;
GX_UBYTE      oppsite_type;
GX_UBYTE      type;
GX_BOOL       bracket_type_changed;
GX_BOOL       strong_type_found;
GX_BIDI_UNIT *unit;

    if (!context -> gx_bidi_context_bracket_pair_size)
    {
        return GX_SUCCESS;
    }

    bracket_pair = (INT *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);
    context -> gx_bidi_context_buffer_index += context -> gx_bidi_context_bracket_pair_size;

    if (context -> gx_bidi_context_buffer_index > context -> gx_bidi_context_buffer_size)
    {
        return GX_INVALID_MEMORY_SIZE;
    }

    status = _gx_utility_bidi_bracket_pair_search(context, entry, &bracket_pair, &bracket_pair_count);

    if (status != GX_SUCCESS)
    {
        return GX_SUCCESS;
    }

    if (bracket_pair_count > 0)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[bracket_pair[0]];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if ((unit -> gx_bidi_unit_level & 0x01) == 0)
        {
            embedding_type = GX_BIDI_CHARACTER_TYPE_L;
            oppsite_type = GX_BIDI_CHARACTER_TYPE_R;
        }
        else
        {
            embedding_type = GX_BIDI_CHARACTER_TYPE_R;
            oppsite_type = GX_BIDI_CHARACTER_TYPE_L;
        }
    }

    for (index = 0; index < bracket_pair_count; index += 2)
    {
        strong_type_found = GX_FALSE;
        bracket_type_changed = GX_FALSE;
        for (start_index = bracket_pair[index] + 1; start_index < bracket_pair[index + 1]; start_index++)
        {
            text_index = entry -> gx_bidi_isolate_run_index_list[start_index];
            unit = &context -> gx_bidi_context_unit_list[text_index];

            type = unit -> gx_bidi_unit_type;

            if (type == GX_BIDI_CHARACTER_TYPE_EN ||
                type == GX_BIDI_CHARACTER_TYPE_AN)
            {
                /* Within this scope, bidirectional types EN and AN are treated as R. */
                type = GX_BIDI_CHARACTER_TYPE_R;
            }

            if (type == GX_BIDI_CHARACTER_TYPE_L ||
                type == GX_BIDI_CHARACTER_TYPE_R)
            {
                /* If any strong type (either L or R) matching the embedding direction is found,
                   set the type for both brackets in the pair to match the embedding direction. */
                strong_type_found = GX_TRUE;

                if (type == embedding_type)
                {
                    bracket_type_changed = GX_TRUE;

                    text_index = entry -> gx_bidi_isolate_run_index_list[bracket_pair[index]];
                    unit = &context -> gx_bidi_context_unit_list[text_index];
                    unit -> gx_bidi_unit_type = embedding_type;

                    text_index = entry -> gx_bidi_isolate_run_index_list[bracket_pair[index + 1]];
                    unit = &context -> gx_bidi_context_unit_list[text_index];
                    unit -> gx_bidi_unit_type = embedding_type;
                    break;
                }
            }
        }

        if (!bracket_type_changed)
        {
            if (strong_type_found)
            {
                bracket_type_changed = GX_TRUE;

                end_index = bracket_pair[index] - 1;
                strong_type_found = GX_FALSE;
                while (end_index >= 0)
                {
                    text_index = entry -> gx_bidi_isolate_run_index_list[end_index];
                    unit = &context -> gx_bidi_context_unit_list[text_index];

                    /* Search baackward until first strong type is found. */
                    type = unit -> gx_bidi_unit_type;

                    if (type == GX_BIDI_CHARACTER_TYPE_EN ||
                        type == GX_BIDI_CHARACTER_TYPE_AN)
                    {
                        /* Within this scope, bidirectional types EN and AN are treated as R. */
                        type = GX_BIDI_CHARACTER_TYPE_R;
                    }

                    if (type == GX_BIDI_CHARACTER_TYPE_R ||
                        type == GX_BIDI_CHARACTER_TYPE_L)
                    {
                        strong_type_found = GX_TRUE;
                        break;
                    }

                    end_index--;
                }

                if (!strong_type_found)
                {
                    type = entry -> gx_bidi_isolate_run_sos;
                }

                if (type != oppsite_type)
                {
                    type = embedding_type;
                }

                text_index = entry -> gx_bidi_isolate_run_index_list[bracket_pair[index]];
                unit = &context -> gx_bidi_context_unit_list[text_index];
                unit -> gx_bidi_unit_type = type;

                text_index = entry -> gx_bidi_isolate_run_index_list[bracket_pair[index + 1]];
                unit = &context -> gx_bidi_context_unit_list[text_index];
                unit -> gx_bidi_unit_type = type;
            }
        }

        if (bracket_type_changed)
        {
            /* Any number of characters that had original bidirectional character type NSM that immediately follow a
               paired bracket which changed to L or R under N0 should change to match the type of their preceding bracket.*/
            start_index = bracket_pair[index] + 1;
            while (start_index < bracket_pair[index + 1])
            {
                end_index = entry -> gx_bidi_isolate_run_index_list[start_index];
                unit = &context -> gx_bidi_context_unit_list[end_index];

                if (unit -> gx_bidi_unit_org_type == GX_BIDI_CHARACTER_TYPE_NSM)
                {
                    unit -> gx_bidi_unit_type = type;
                }
                else
                {
                    break;
                }
                start_index++;
            }

            start_index = bracket_pair[index + 1] + 1;
            while (start_index < (INT)entry -> gx_bidi_isolate_run_index_count)
            {
                text_index = entry -> gx_bidi_isolate_run_index_list[start_index];
                unit = &context -> gx_bidi_context_unit_list[text_index];

                if (unit -> gx_bidi_unit_org_type == GX_BIDI_CHARACTER_TYPE_NSM)
                {
                    unit -> gx_bidi_unit_type = type;
                }
                else
                {
                    break;
                }
                start_index++;
            }
        }
    }

    context -> gx_bidi_context_buffer_index -= context -> gx_bidi_context_bracket_pair_size;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_neutral_type_resolve_1             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 1 to neutral characters in an*/
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_neutral_type_resolve_1(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT      run_index;
INT      start_index = 0;
INT      text_index;
GX_UBYTE pre_type = entry -> gx_bidi_isolate_run_sos;
GX_UBYTE follow_type;
GX_UBYTE type;

    /* A sequence of NIs takes the direction of the surrounding strong text if the text
       on both sides has the same direction. */
    for (run_index = 0; run_index <= (INT)entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        if (run_index < (INT)entry -> gx_bidi_isolate_run_index_count)
        {
            text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
            type = context -> gx_bidi_context_unit_list[text_index].gx_bidi_unit_type;
        }
        else
        {
            type = entry -> gx_bidi_isolate_run_eos;
        }

        switch (type)
        {
        case GX_BIDI_CHARACTER_TYPE_B:
        case GX_BIDI_CHARACTER_TYPE_S:
        case GX_BIDI_CHARACTER_TYPE_WS:
        case GX_BIDI_CHARACTER_TYPE_ON:
        case GX_BIDI_CHARACTER_TYPE_FSI:
        case GX_BIDI_CHARACTER_TYPE_LRI:
        case GX_BIDI_CHARACTER_TYPE_RLI:
        case GX_BIDI_CHARACTER_TYPE_PDI:
            break;

        default:
            follow_type = type;

            if (follow_type == GX_BIDI_CHARACTER_TYPE_EN ||
                follow_type == GX_BIDI_CHARACTER_TYPE_AN)
            {
                follow_type = GX_BIDI_CHARACTER_TYPE_R;
            }

            if (pre_type == follow_type)
            {
                while (start_index < run_index)
                {
                    text_index = entry -> gx_bidi_isolate_run_index_list[start_index];

                    context -> gx_bidi_context_unit_list[text_index].gx_bidi_unit_type = pre_type;
                    start_index++;
                }
            }
            pre_type = follow_type;
            start_index = run_index + 1;
            break;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_neutral_type_resolve_2             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to apply rule 2 to neutral characters in an*/
/*    isolate run sequence.                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_neutral_type_resolve_2(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index = 0;
INT           text_index;
GX_BIDI_UNIT *unit;

    /* Any remaining NIs take the embedding direction. */
    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        switch (unit -> gx_bidi_unit_type)
        {
        case GX_BIDI_CHARACTER_TYPE_B:
        case GX_BIDI_CHARACTER_TYPE_S:
        case GX_BIDI_CHARACTER_TYPE_WS:
        case GX_BIDI_CHARACTER_TYPE_ON:
        case GX_BIDI_CHARACTER_TYPE_FSI:
        case GX_BIDI_CHARACTER_TYPE_LRI:
        case GX_BIDI_CHARACTER_TYPE_RLI:
        case GX_BIDI_CHARACTER_TYPE_PDI:
            if ((unit -> gx_bidi_unit_level & 0x01) == 0)
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_L;
            }
            else
            {
                unit -> gx_bidi_unit_type = GX_BIDI_CHARACTER_TYPE_R;
            }
            break;
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_implicit_level_resolve             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to increase the embedding level of text    */
/*    based on the resolved character type.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    entry                                 Pointer to an isolate run     */
/*                                            sequence                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_implicit_level_resolve(GX_BIDI_CONTEXT *context, GX_BIDI_ISOLATE_RUN *entry)
{
INT           run_index;
INT           text_index;
GX_BIDI_UNIT *unit;

    /* Any remaining NIs take the embedding direction. */
    for (run_index = 0; run_index < entry -> gx_bidi_isolate_run_index_count; run_index++)
    {
        text_index = entry -> gx_bidi_isolate_run_index_list[run_index];
        unit = &context -> gx_bidi_context_unit_list[text_index];

        if ((unit -> gx_bidi_unit_level & 0x01) == 0)
        {
            /* For all characers with an even embedding level,
                those of type R go up ove level and those of type AN or EN go up two levels. */
            switch (unit -> gx_bidi_unit_type)
            {
            case GX_BIDI_CHARACTER_TYPE_R:
                unit -> gx_bidi_unit_level++;
                break;

            case GX_BIDI_CHARACTER_TYPE_AN:
            case GX_BIDI_CHARACTER_TYPE_EN:
                unit -> gx_bidi_unit_level = (GX_UBYTE)(unit -> gx_bidi_unit_level + 2);
                break;
            }
        }

        if ((unit -> gx_bidi_unit_level & 0x01) == 1)
        {
            /* For all characers with an odd embedding level,
                those of type L, EN and AN go up ove level. */
            switch (unit -> gx_bidi_unit_type)
            {
            case GX_BIDI_CHARACTER_TYPE_L:
            case GX_BIDI_CHARACTER_TYPE_EN:
            case GX_BIDI_CHARACTER_TYPE_AN:
                unit -> gx_bidi_unit_level++;
                break;
            }
        }
    }

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_isolate_run_sequences_resolve      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to determine the embedding levels of the   */
/*    text.                                                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_isolate_run_sequences_resolve(GX_BIDI_CONTEXT *context)
{
UINT                 status = GX_SUCCESS;

GX_BIDI_ISOLATE_RUN *entry = context -> gx_bidi_context_isolate_runs;

    while (entry)
    {
        status = _gx_utility_bidi_weak_type_resolve_1(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        status = _gx_utility_bidi_weak_type_resolve_2_3(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }
        status = _gx_utility_bidi_weak_type_resolve_4(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }
        status = _gx_utility_bidi_weak_type_resolve_5(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }
        status = _gx_utility_bidi_weak_type_resolve_6(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        status = _gx_utility_bidi_weak_type_resolve_7(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        /* Process bracket pairs. */
        status = _gx_utility_bidi_neutral_type_resolve_0(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        status = _gx_utility_bidi_neutral_type_resolve_1(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        status = _gx_utility_bidi_neutral_type_resolve_2(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        status = _gx_utility_bidi_implicit_level_resolve(context, entry);

        if (status != GX_SUCCESS)
        {
            break;
        }

        entry = entry -> gx_bidi_isolate_run_next;
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_reordering_resolve_1               PORTABLE C      */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to reset the embedding level of some       */
/*    characters in specified text block.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*    start_index                           Start index of the text block */
/*    end_index                             End index of the text block   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  10-15-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            corrected logic,            */
/*                                            resulting in version 6.1.9  */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_reordering_resolve_1(GX_BIDI_CONTEXT *context, INT start_index, INT end_index)
{
INT           index;
INT           pre_index;
GX_BOOL       check_before;
GX_BIDI_UNIT *unit;
GX_BIDI_UNIT *pre_unit;
USHORT        mirror;

    for (index = start_index; index <= end_index; index++)
    {
        check_before = GX_FALSE;
        unit = &context -> gx_bidi_context_unit_list[index];

        switch (unit -> gx_bidi_unit_org_type)
        {
        case GX_BIDI_CHARACTER_TYPE_S:
        case GX_BIDI_CHARACTER_TYPE_B:
            /* Reset segment separators and paragraph separators to the paragraph embedding level. */
            unit -> gx_bidi_unit_level = context -> gx_bidi_context_base_level;
            check_before = GX_TRUE;
            break;

        default:
            if (index == context -> gx_bidi_context_unit_count - 1)
            {
                index = context -> gx_bidi_context_unit_count;
                check_before = GX_TRUE;
            }
            break;
        }

        if (check_before)
        {
            /* Any sequence of whitespace or isolate formatting characters preceding a
               segment/paragraph separator or at the end of the text, reset to paragraph level. */
            pre_index = index - 1;
            pre_unit = &context -> gx_bidi_context_unit_list[pre_index];
            while (pre_index >= 0)
            {
                if (pre_unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_BN)
                {
                    pre_index--;
                    pre_unit--;
                    continue;
                }

                switch (pre_unit -> gx_bidi_unit_org_type)
                {
                case GX_BIDI_CHARACTER_TYPE_WS:
                case GX_BIDI_CHARACTER_TYPE_FSI:
                case GX_BIDI_CHARACTER_TYPE_LRI:
                case GX_BIDI_CHARACTER_TYPE_RLI:
                case GX_BIDI_CHARACTER_TYPE_PDI:
                    pre_unit -> gx_bidi_unit_level = context -> gx_bidi_context_base_level;
                    break;

                default:
                    pre_index = -1;
                    break;
                }
                pre_index--;
                pre_unit--;
            }
        }

        if (unit -> gx_bidi_unit_type == GX_BIDI_CHARACTER_TYPE_R &&
            unit -> gx_bidi_unit_code < 0xffff)
        {
            /* Mirror */
            _gx_utility_bidi_mirroring_get((USHORT)unit -> gx_bidi_unit_code, &mirror);

            if (mirror)
            {
                unit -> gx_bidi_unit_code = mirror;
            }
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_reordering_resolve_2               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to reverse text for the specified text     */
/*    block.                                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*    start_index                           Start index of the text block */
/*    end_index                             End index of the text block   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_bidi_paragraph_reorder                                  */
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
static UINT _gx_utility_bidi_reordering_resolve_2(GX_BIDI_CONTEXT *context, INT start_index, INT end_index)
{
INT                max_level;
INT                min_level;
INT                level;
INT                index;
INT                run_start_index;
INT                run_end_index;
GX_BIDI_LEVEL_RUN *entry;
GX_BIDI_LEVEL_RUN *next_entry;
GX_BIDI_UNIT      *unit;
GX_BIDI_UNIT      *tail_unit;
GX_BIDI_UNIT       temp;
INT                count;

    _gx_utility_bidi_level_runs_compute(context, start_index, end_index);

    /* Find max level. */
    entry = context -> gx_bidi_context_level_runs;
    if (entry)
    {
        unit = &context -> gx_bidi_context_unit_list[entry -> gx_bidi_level_run_start_index];
        max_level = unit -> gx_bidi_unit_level;
        min_level = max_level;
        entry = entry -> gx_bidi_level_run_next;
    }

    while (entry)
    {
        unit = &context -> gx_bidi_context_unit_list[entry -> gx_bidi_level_run_start_index];

        if (unit -> gx_bidi_unit_level > max_level)
        {
            max_level = unit -> gx_bidi_unit_level;
        }
        else if (unit -> gx_bidi_unit_level < min_level)
        {
            min_level = unit -> gx_bidi_unit_level;
        }
        entry = entry -> gx_bidi_level_run_next;
    }

    if ((min_level & 0x01) == 0)
    {
        min_level += 1;
    }

    /* From highest level, reverse any contiguous sequence of characters that are at that level or higher. */
    for (level = max_level; level >= min_level; level--)
    {
        entry = context -> gx_bidi_context_level_runs;

        while (entry)
        {
            run_start_index = entry -> gx_bidi_level_run_start_index;
            run_end_index = entry -> gx_bidi_level_run_end_index;

            next_entry = entry -> gx_bidi_level_run_next;
            unit = &context -> gx_bidi_context_unit_list[run_start_index];

            if (unit -> gx_bidi_unit_level >= level)
            {
                while (next_entry &&
                       context -> gx_bidi_context_unit_list[next_entry -> gx_bidi_level_run_start_index].gx_bidi_unit_level >= level)
                {
                    run_end_index = next_entry -> gx_bidi_level_run_end_index;
                    next_entry = next_entry -> gx_bidi_level_run_next;
                }

                count = (run_end_index - run_start_index + 1) >> 1;

                /* Reverse characters. */
                unit = &context -> gx_bidi_context_unit_list[run_start_index];
                tail_unit = &context -> gx_bidi_context_unit_list[run_end_index];

                for (index = 0; index < count; index++)
                {
                    temp = *unit;
                    *unit = *tail_unit;
                    *tail_unit = temp;

                    unit++;
                    tail_unit--;
                }
            }
            entry = next_entry;
        }
    }

    context -> gx_bidi_context_buffer_index -= sizeof(GX_BIDI_LEVEL_RUN) * (UINT)context -> gx_bidi_context_unit_count;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_line_break                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to break text into lines.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_line_break(GX_BIDI_CONTEXT *context)
{
GX_BIDI_TEXT_INFO *input_info = context -> gx_bidi_context_input_info;
INT                index = 0;
GX_STRING          ch;
UINT               glyph_len;
GX_VALUE           glyph_width;
UINT               line = 0;
GX_BIDI_UNIT      *unit;
GX_UBYTE           utf8[6];
INT                display_number = 0;
INT                display_width = 0;
INT                line_break_display_number = 0;
INT                line_break_display_width = 0;
INT                line_index = -1;

    unit = context -> gx_bidi_context_unit_list;

    if ((!input_info -> gx_bidi_text_info_font) || (input_info -> gx_bidi_text_info_display_width <= 0))
    {
        context -> gx_bidi_context_total_lines = 1;
        return GX_SUCCESS;
    }

    context -> gx_bidi_context_line_index_cache = (INT *)(context -> gx_bidi_context_buffer + context -> gx_bidi_context_buffer_index);
    context -> gx_bidi_context_line_index_cache[0] = 0;

    for (index = 0; index < context -> gx_bidi_context_unit_count; index++)
    {
        _gx_utility_unicode_to_utf8(unit -> gx_bidi_unit_code, utf8, &glyph_len);

        ch.gx_string_ptr = (GX_CHAR *)utf8;
        ch.gx_string_length = glyph_len;
        _gx_system_string_width_get_ext(input_info -> gx_bidi_text_info_font, &ch, &glyph_width);

        if ((display_width + glyph_width > input_info -> gx_bidi_text_info_display_width) &&
            (display_number > 0) &&
            (ch.gx_string_ptr[0] != ' '))
        {
            /* Breadk line. */
            if (line_break_display_number)
            {
                line_index += line_break_display_number;
                display_number -= line_break_display_number;
                display_width -= line_break_display_width;
            }
            else
            {
                line_index += display_number;
                display_number = 0;
                display_width = 0;
            }

            context -> gx_bidi_context_line_index_cache[line++] = line_index;
            line_break_display_number = 0;
            line_break_display_width = 0;
        }

        display_width += glyph_width;
        display_number++;

        if ((ch.gx_string_ptr[0] == ' ') ||
            (ch.gx_string_ptr[0] == ',') ||
            (ch.gx_string_ptr[0] == ';'))
        {
            line_break_display_number = display_number;
            line_break_display_width = display_width;
        }

        unit++;
    }

    context -> gx_bidi_context_line_index_cache[line++] = context -> gx_bidi_context_unit_count - 1;
    context -> gx_bidi_context_total_lines = line;

    context -> gx_bidi_context_buffer_index += sizeof(INT *) * line;

    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_reordering_resolve                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function to reorder text based on resolved embedding*/
/*    levels or each line.                                                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
/*    total_lines                           Total lines of the text       */
/*    returned_utf8_text                    Pointer to the utf8 string of */
/*                                            the first line, following   */
/*                                            lines are linked one after  */
/*                                            another.                    */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            update with bidi context    */
/*                                            structure change,           */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_reordering_resolve(GX_BIDI_CONTEXT *context, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info)
{
UINT                        status = GX_SUCCESS;
INT                         start_index = 0;
INT                         end_index;
UINT                        line_index;
GX_STRING                  *out_text_list = GX_NULL;
GX_CHAR                    *line_text;
ULONG                       byte_size;
INT                         index;
UINT                        glyph_count;
GX_BIDI_UNIT               *unit;
GX_BIDI_RESOLVED_TEXT_INFO *bidi_text;

    byte_size = context -> gx_bidi_context_total_lines * (sizeof(GX_BIDI_RESOLVED_TEXT_INFO) + sizeof(GX_STRING));
    byte_size += context -> gx_bidi_context_reordered_utf8_size + context -> gx_bidi_context_total_lines;

    bidi_text = (GX_BIDI_RESOLVED_TEXT_INFO *)_gx_system_memory_allocator(byte_size);

    if (!bidi_text)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    memset(bidi_text, 0, (size_t)byte_size);

    bidi_text -> gx_bidi_resolved_text_info_text = (GX_STRING *)(bidi_text + 1);
    bidi_text -> gx_bidi_resolved_text_info_total_lines = context -> gx_bidi_context_total_lines;

    *resolved_info = bidi_text;

    out_text_list = bidi_text -> gx_bidi_resolved_text_info_text;
    line_text = (GX_CHAR *)(out_text_list + context -> gx_bidi_context_total_lines);


    for (line_index = 0; line_index < context -> gx_bidi_context_total_lines; line_index++)
    {
        if (context -> gx_bidi_context_total_lines == 1)
        {
            end_index = context -> gx_bidi_context_unit_count - 1;
        }
        else
        {
            end_index = context -> gx_bidi_context_line_index_cache[line_index];
        }

        status = _gx_utility_bidi_reordering_resolve_1(context, start_index, end_index);

        if (status == GX_SUCCESS)
        {
            status = _gx_utility_bidi_reordering_resolve_2(context, start_index, end_index);
        }

        if (status == GX_SUCCESS)
        {
            unit = &context -> gx_bidi_context_unit_list[start_index];

            out_text_list -> gx_string_ptr = line_text;

            /* Convert unicode to utf8 text. */
            for (index = start_index; index <= end_index; index++)
            {
                switch (unit -> gx_bidi_unit_type)
                {
                case GX_BIDI_CHARACTER_TYPE_BN:
                    /* Skip above characters. */
                    break;

                default:
                    if (unit -> gx_bidi_unit_code)
                    {
                        if (unit -> gx_bidi_unit_code < 0x80)
                        {
                            *(GX_UBYTE *)(line_text) = (GX_UBYTE)(unit -> gx_bidi_unit_code);
                            glyph_count = 1;
                        }
                        else
                        {

                            _gx_utility_unicode_to_utf8(unit -> gx_bidi_unit_code, (GX_UBYTE *)line_text, &glyph_count);
                        }
                        line_text += glyph_count;
                        out_text_list -> gx_string_length += glyph_count;
                    }
                    break;
                }

                unit++;
            }

            line_text++;
            out_text_list++;
        }

        start_index = end_index + 1;
    }

    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_one_paragraph_reorder              PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reorders a bidi text for displaying.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_info                             Pointer to bidi text          */
/*    reordered_text                        Reordered text, each line is  */
/*                                            ended a with string         */
/*                                            terminator '\0', multi line */
/*                                            strings are linked one after*/
/*                                            another                     */
/*    processed_count                       The processed text size in    */
/*                                            byte                        */
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s), modified */
/*                                            line breaking logic,        */
/*                                            supported Arabic shaping,   */
/*                                            updated with resolved text  */
/*                                            info structure change,      */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Kenneth Maxwell          Modified comment(s), modified */
/*                                            base level set logic,       */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_one_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info, UINT *processed_size)
{
UINT            status;
GX_BIDI_CONTEXT context;

    memset(&context, 0, sizeof(GX_BIDI_CONTEXT));
    context.gx_bidi_context_input_info = input_info;

    /* Allocate buffer needed for bidi text reordering. */
    status = _gx_utility_bidi_buffer_allocate(&context);

    if (status == GX_SUCCESS)
    {
        status = _gx_utility_bidi_initiate(&context);
    }

    if (status == GX_SUCCESS)
    {
        switch (input_info -> gx_bidi_text_info_direction)
        {
        case GX_LANGUAGE_DIRECTION_LTR:
            context.gx_bidi_context_base_level = 0;
            break;

        case GX_LANGUAGE_DIRECTION_RTL:
            context.gx_bidi_context_base_level = 1;
            break;

        default:
            /* Compute paragraph embedding_level. */
            status = _gx_utility_bidi_block_level_determine(&context, 0,
                                                            (UINT)(context.gx_bidi_context_unit_count - 1),
                                                            &context.gx_bidi_context_base_level);
            break;
        }
    }

    if (status == GX_SUCCESS)
    {
        /* Compute explicie level for every character. */
        status = _gx_utility_bidi_explicit_levels_determine(&context);
    }

    if (status == GX_SUCCESS)
    {
        /* Compute isolate run sequences. */
        status = _gx_utility_bidi_isolate_run_sequences_get(&context);
    }

    if (status == GX_SUCCESS)
    {
        /* Apply rules to each isolate run sequence. */
        status = _gx_utility_bidi_isolate_run_sequences_resolve(&context);
    }

#if defined(GX_DYNAMIC_ARABIC_SHAPING_SUPPORT)
    if (status == GX_SUCCESS)
    {
        status = _gx_utility_bidi_arabic_shaping(&context);
    }
#endif

    if (status == GX_SUCCESS)
    {
        /* Broke paragraph text into lines. */
        status = _gx_utility_bidi_line_break(&context);
    }

    if (status == GX_SUCCESS)
    {
        /* Reorder text of each line for display. */
        status = _gx_utility_bidi_reordering_resolve(&context, resolved_info);
    }

    if (status == GX_SUCCESS)
    {
        if (processed_size)
        {
            *processed_size = context.gx_bidi_context_processced_size;
        }

        _gx_system_memory_free(context.gx_bidi_context_buffer);
    }

    return status;
}



/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_paragraph_reorder                  PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reorders a bidi text for displaying.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input_info                            Pointer to bidi text info     */
/*    reordered_text                        Reordered text information    */
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
/*                                            made this function a public */
/*                                            api,                        */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_bidi_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head)
{
    input_info->gx_bidi_text_info_direction = -1;

    return _gx_utility_bidi_paragraph_reorder_ext(input_info, resolved_info_head);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_paragraph_reorder_ext              PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function reorders a bidi text for displaying.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    input_info                            Pointer to bidi text info     */
/*    reordered_text                        Reordered text information    */
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
/*  01-31-2022     Ting Zhu                 Initial Version 6.1.10        */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_bidi_paragraph_reorder_ext(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head)
{
UINT                        status = GX_SUCCESS;
GX_BIDI_TEXT_INFO           text_info;
GX_BIDI_RESOLVED_TEXT_INFO *resolved_info;
GX_BIDI_RESOLVED_TEXT_INFO *head = GX_NULL;
GX_BIDI_RESOLVED_TEXT_INFO *pre = GX_NULL;
UINT                        line_size;
UINT                        line_break_counts = 0;
GX_STRING                   string = input_info -> gx_bidi_text_info_text;

    if (!_gx_system_memory_allocator)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    text_info = *input_info;

    while (string.gx_string_length > 0)
    {
        if (string.gx_string_ptr[0] == GX_KEY_CARRIAGE_RETURN)
        {
            if ((string.gx_string_length > 1) && (string.gx_string_ptr[1] == GX_KEY_LINE_FEED))
            {
                line_size = 2;
            }
            else
            {
                line_size = 1;
            }
            line_break_counts++;
        }
        else if (string.gx_string_ptr[0] == GX_KEY_LINE_FEED)
        {
            line_size = 1;
            line_break_counts++;
        }
        else
        {
            line_size = 0;
        }

        if (((line_break_counts > 1) && (!line_size)) ||
            ((line_break_counts > 0) && (line_size == string.gx_string_length)))
        {
            /* Collect blank lines as one bidi resolved text info instance. */
            resolved_info = (GX_BIDI_RESOLVED_TEXT_INFO *)_gx_system_memory_allocator(sizeof(GX_BIDI_RESOLVED_TEXT_INFO));
            if (!resolved_info)
            {
                status = GX_SYSTEM_MEMORY_ERROR;
                break;
            }

            memset(resolved_info, 0, sizeof(GX_BIDI_RESOLVED_TEXT_INFO));

            if (!line_size)
            {
                resolved_info -> gx_bidi_resolved_text_info_total_lines = (UINT)(line_break_counts - 1);
            }
            else
            {
                resolved_info -> gx_bidi_resolved_text_info_total_lines = line_break_counts;
            }

            GX_LINK_RESOLVED_BIDI_TEXT_INFO
        }

        if (!line_size)
        {
            /* Start bidi text reorderding for one paragraph. */
            text_info.gx_bidi_text_info_text = string;
            status = _gx_utility_bidi_one_paragraph_reorder(&text_info, &resolved_info, &line_size);

            if (status != GX_SUCCESS)
            {
                break;
            }

            GX_LINK_RESOLVED_BIDI_TEXT_INFO

            line_break_counts = 0;
        }

        /* Increment string pointer and decrement string length. */
        string.gx_string_ptr += line_size;
        string.gx_string_length -= line_size;
    }

    if (status == GX_SUCCESS)
    {
        *resolved_info_head = head;
    }
    else if (head)
    {
        /* Clean generated resolved bidi text link. */
        _gx_utility_bidi_resolved_text_info_delete(&head);
    }

    return status;
}
#endif


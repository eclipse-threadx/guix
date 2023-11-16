/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "all_widgets_synergy_24xrgb_rotated_cw_binary_resources.h"
#include "all_widgets_synergy_24xrgb_rotated_cw_binary_specifications.h"

#define DROP_LIST_VISIBLE_ROWS 5
#define NUMBER_ANIMATION_TIMER 2
#define NESTED_WINDOW_WIDTH  94
#define NESTED_WINDOW_HEIGHT  73

#define MAX_PATH_LENGTH 255

#define DYNAMIC_MEMORY_SIZE 2000 * 1024
GX_UBYTE dynamic_memory_buffer[DYNAMIC_MEMORY_SIZE];

GX_SCROLLBAR list_scroll;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;

GX_THEME *theme = GX_NULL;
GX_STRING **language_table = GX_NULL;
GX_UBYTE *binres_root_address = GX_NULL;
GX_CHAR binres_pathname[] = "all_widgets_synergy_24xrgb_rotated_cw_binary_resources.bin";

UINT load_binary_resource_data_to_ram();
UINT load_binres_theme(GX_UBYTE* root_address, INT theme_id);

typedef struct {
    GX_PROMPT prompt;
    CHAR text[80];
} DROP_LIST_WIDGET;

DROP_LIST_WIDGET drop_list_widgets[DROP_LIST_VISIBLE_ROWS + 1];

/* Define prototypes.   */
VOID  PopulateDropList();
VOID  demo_thread_entry(ULONG thread_input);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *dynamic_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID dynamic_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}


VOID  demo_thread_entry(ULONG thread_input)
{
    tx_byte_pool_create(&memory_pool, "dynamic memory pool", dynamic_memory_buffer, DYNAMIC_MEMORY_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_system_memory_allocator_set(dynamic_memory_allocate, dynamic_memory_free);

    /* Configure display.  */
    gx_studio_display_configure(MAIN_DISPLAY, _gx_synergy_display_driver_setup,
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    load_binary_resource_data_to_ram();
    load_binres_theme(binres_root_address, 1);

    /* Create the button screen.  */
    gx_studio_named_widget_create("button_screen", (GX_WIDGET *)root, GX_NULL);

    /* Create the text widget screen.  */
    gx_studio_named_widget_create("text_screen", GX_NULL, GX_NULL);

    /* Create the windows screen.  */
    gx_studio_named_widget_create("window_screen", GX_NULL, GX_NULL);
    PopulateDropList();

    /* Create the indicator screen.  */
    gx_studio_named_widget_create("indicator_screen", GX_NULL, GX_NULL);

    /* Create the shapes screen.  */
    gx_studio_named_widget_create("shapes_screen", GX_NULL, GX_NULL);

    /* Create the rotate screen.  */
    gx_studio_named_widget_create("rotate_screen", GX_NULL, GX_NULL);

    /* Create scroll wheel screen.  */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, GX_NULL);

    /* Create resize screen.  */
    gx_studio_named_widget_create("resize_screen", GX_NULL, GX_NULL);

    /* Create menu screen.  */
    gx_studio_named_widget_create("menu_screen", GX_NULL, GX_NULL);

    /* Create lines screen.  */
    gx_studio_named_widget_create("lines_screen", GX_NULL, GX_NULL);

    /* Create circular gauge screen.  */
    gx_studio_named_widget_create("gauge_screen", GX_NULL, GX_NULL);

    /* Create pixelmap gauge screen.  */
    gx_studio_named_widget_create("pixelmap_screen", GX_NULL, GX_NULL);

    /* Create glyph screen.  */
    gx_studio_named_widget_create("glyph_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run.  */
    gx_system_start();
}

void ToggleScreen(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
    if (!new_win->gx_widget_parent)
    {
        gx_widget_attach(root, (GX_WIDGET *) new_win);
    }
    else
    {
        gx_widget_show((GX_WIDGET *) new_win);
    }
    gx_widget_hide((GX_WIDGET *) old_win);
}


UINT glyph_screen_event_process(GX_WINDOW* widget, GX_EVENT* event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_8BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_FONT);
        break;

    case GX_SIGNAL(ID_8BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_8BPP);

        break;

    case GX_SIGNAL(ID_4BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_4BPP);
        break;

    case GX_SIGNAL(ID_4BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_4BPP);
        break;

    case GX_SIGNAL(ID_1BPP_RAW, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_NORMAL_1BPP);

        break;

    case GX_SIGNAL(ID_1BPP_COMPRESS, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_font_set(&glyph_screen.glyph_screen_view, GX_FONT_ID_COMPRESS_1BPP);
        break;

    default:
        return next_button_handler(widget, event_ptr);
    }

    return 0;
}

UINT next_button_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(IDB_NEXT, GX_EVENT_CLICKED):
        switch(window ->gx_widget_id)
        {
        case ID_BUTTON_SCREEN:
            /* we are on button window, switch to window */
            ToggleScreen((GX_WINDOW *) &window_screen, window);
            break;

        case ID_WINDOW_SCREEN:
            ToggleScreen((GX_WINDOW *) &text_screen, window);
            gx_system_timer_start(&text_screen, NUMBER_ANIMATION_TIMER, 20, 20);
            break; 

        case ID_TEXT_SCREEN:
            ToggleScreen((GX_WINDOW *) &indicator_screen, window);
            break; 

        case ID_INDICATOR_SCREEN:
            ToggleScreen((GX_WINDOW *) &rotate_screen, window);
            break; 
            
        case ID_ROTATE_SCREEN:
            ToggleScreen((GX_WINDOW *) &resize_screen, window);
            break;

        case ID_RESIZE_SCREEN:
            ToggleScreen((GX_WINDOW *) &scroll_wheel_screen, window);
            break;

        case ID_SCROLL_WHEEL_SCREEN:
            ToggleScreen((GX_WINDOW *) &menu_screen, window);
            break;

        case ID_MENU_SCREEN:
            ToggleScreen((GX_WINDOW *) &lines_screen, window);
            break;

        case ID_LINES_SCREEN:
            ToggleScreen((GX_WINDOW *) &gauge_screen, window);
            break;

        case ID_GAUGE_SCREEN:
            ToggleScreen((GX_WINDOW *) &shapes_screen, window);
            break;

        case ID_SHAPES_SCREEN:
            ToggleScreen((GX_WINDOW *) &pixelmap_screen, window);
            break;

        case ID_PIXELMAP_SCREEN:
            ToggleScreen((GX_WINDOW *) &glyph_screen, window);
            break;

        case ID_GLYPH_SCREEN:
            ToggleScreen((GX_WINDOW *) &button_screen, window);
            break;
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }
    return 0;
}

VOID drop_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_BOOL created;
GX_RECTANGLE size;
GX_CHAR temp[10];

    DROP_LIST_WIDGET *entry = (DROP_LIST_WIDGET *) widget;

    GX_STRCPY(entry->text, "List Entry #");
    gx_utility_ltoa(index + 1, temp, 10);
    GX_STRCAT(entry->text, (char*)temp);

    gx_widget_created_test(widget, &created);

    if (!created)
    {
        gx_utility_rectangle_define(&size, 0, 0, 100, 30);
        gx_prompt_create(&entry->prompt, entry->text, list, 0, GX_STYLE_ENABLED|GX_STYLE_TEXT_LEFT|GX_STYLE_BORDER_NONE, 0, &size);
        gx_widget_fill_color_set(&entry->prompt, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLUE, GX_COLOR_ID_BLACK);
        gx_prompt_text_color_set(&entry->prompt, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
    }
  
    gx_prompt_text_set(&entry->prompt, (GX_CHAR*)entry->text);
}

VOID PopulateDropList(void)
{
    int index;
    GX_VERTICAL_LIST *list;

    WINDOW_SCREEN_CONTROL_BLOCK *control = (WINDOW_SCREEN_CONTROL_BLOCK *) &window_screen;
    GX_DROP_LIST *drop = &control->window_screen_drop_list;
    gx_drop_list_popup_get(drop, &list);

    for (index = 0; index < DROP_LIST_VISIBLE_ROWS; index++)
    {
        drop_list_row_create(list, (GX_WIDGET *) &drop_list_widgets[index], index);
    }

    gx_vertical_scrollbar_create(&list_scroll, "list_scroll", list, GX_NULL,
        GX_SCROLLBAR_VERTICAL | GX_SCROLLBAR_RELATIVE_THUMB | GX_SCROLLBAR_END_BUTTONS | GX_STYLE_ENABLED);
}

//////////////////////////////////////////////////////////////////////////////////////
static UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

//////////////////////////////////////////////////////////////////////////////////////
static int extract_path(char* pathname, char* path)
{
    int len = string_length_get(pathname, MAX_PATH_LENGTH) - 1;

    while (len >= 0)
    {
        if ((pathname[len] == '/') || pathname[len] == '\\')
        {
            memcpy(path, pathname, len + 1);
            len++;
            path[len] = '\0';
            break;
        }
        len--;
    }

    return len;
}


//////////////////////////////////////////////////////////////////////////////////////
UINT load_binary_resource_data_to_ram()
{
    FILE* p_file;
    size_t total_length;
    size_t result;
    char pathname[MAX_PATH_LENGTH + 1];
    int len;

    len = extract_path(__FILE__, pathname);

    memcpy(pathname + len, binres_pathname, sizeof(binres_pathname));
    p_file = fopen(pathname, "rb");

    if (!p_file)
    {
        printf("Cannot open file: %s\n", pathname);
        return GX_FAILURE;
    }

    fseek(p_file, 0, SEEK_END);
    total_length = ftell(p_file);
    fseek(p_file, SEEK_SET, SEEK_SET);

    binres_root_address = dynamic_memory_allocate(total_length);
    result = fread(binres_root_address, 1, total_length, p_file);

    fclose(p_file);

    if (result != total_length)
    {
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////
UINT load_binres_theme(GX_UBYTE* root_address, INT theme_id)
{
    UINT      status = GX_SUCCESS;
    GX_THEME* new_theme = GX_NULL;
    GX_STRING** new_table = GX_NULL;

    status = gx_binres_theme_load(root_address, theme_id, &new_theme);

    if (status == GX_SUCCESS)
    {
        if (theme)
        {
            dynamic_memory_free(theme);
        }

        gx_display_theme_install(root->gx_window_root_canvas->gx_canvas_display, new_theme);

        theme = new_theme;
    }

    if (status == GX_SUCCESS)
    {
        status = gx_binres_language_table_load_ext(root_address, &new_table);
    }

    if (status == GX_SUCCESS)
    {
        if (language_table)
        {
            dynamic_memory_free(language_table);
        }

        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING**)new_table, 1, MAIN_DISPLAY_STRING_TABLE_SIZE);
        gx_system_active_language_set(0);
        language_table = new_table;
    }

    return status;
}
/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "screenshot_to_bitmap_16bpp_resources.h"
#include "screenshot_to_bitmap_16bpp_specifications.h"

#define MAX_PATH_LENGTH 255

//#define USE_FILEX_API

#ifdef USE_FILEX_API
#include "fx_api.h"

#define DEMO_STACK_SIZE 2048

FX_MEDIA        ram_disk;
FX_FILE         my_file;
CHAR            *ram_disk_memory;

/* Define ThreadX global data structures.  */

TX_THREAD       thread_0;
/* Define ThreadX global data structures.  */
VOID start_filex(void* first_unused_memory);
void    thread_0_entry(ULONG thread_input);
VOID _fx_ram_driver(FX_MEDIA *media_ptr);

unsigned char media_memory[512];

#else
#define USE_ANSI_FILEIO
#ifdef REGRESSION_TEST_MODE
GX_CHAR bmp_file[] = "test_screenshot.bmp";
GX_CHAR golden_bmp_file[] = "screenshot.bmp";
GX_CHAR scroll_wheel_bmp_file[] = "test_scroll_wheel_screenshot.bmp";
GX_CHAR scroll_wheel_golden_bmp_file[] = "scroll_wheel_screenshot.bmp";
#else
GX_CHAR bmp_file[] = "screenshot.bmp";
GX_CHAR scroll_wheel_bmp_file[] = "scroll_wheel_screenshot.bmp";
#endif
#endif // USE_FILEX_API

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

TX_BYTE_POOL       rotate_pool;

#define            SCRATCHPAD_PIXELS (PRIMARY_X_RESOLUTION * PRIMARY_X_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
VOID generate_screen_shot();

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *rotate_memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&rotate_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID rotate_memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */

    /* create byte pool for rotate to use */
    tx_byte_pool_create(&rotate_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();

#ifdef USE_FILEX_API
    start_filex(first_unused_memory);
#endif
}

#ifdef USE_FILEX_API
VOID start_filex(void* first_unused_memory)
{
    CHAR *pointer;

    /* Put first available memory address into a character pointer.  */
    pointer = (CHAR *)first_unused_memory;

    /* Put system definition stuff in here, e.g. thread creates and other assorted
    create information.  */

    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,
        pointer, DEMO_STACK_SIZE,
        1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer = pointer + DEMO_STACK_SIZE;

    /* Save the memory pointer for the RAM disk.  */
    ram_disk_memory = pointer;

    /* Initialize FileX.  */
    fx_system_initialize();
}

void    thread_0_entry(ULONG thread_input)
{
    UINT status = 0;
    /* Format the RAM disk - the memory for the RAM disk was setup in
    tx_application_define above.  */
    fx_media_format(&ram_disk,
        _fx_ram_driver,               // Driver entry
        ram_disk_memory,              // RAM disk memory pointer
        media_memory,                 // Media buffer pointer
        sizeof(media_memory),         // Media buffer size
        "MY_RAM_DISK",                // Volume Name
        1,                            // Number of FATs
        32,                           // Directory Entries
        0,                            // Hidden sectors
        256,                          // Total sectors
        512,                          // Sector size
        8,                            // Sectors per cluster
        1,                            // Heads
        1);                           // Sectors per track


        /* Open the RAM disk.  */
    status = fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, media_memory, sizeof(media_memory));

    /* Create a file called TEST.TXT in the root directory.  */
    fx_file_create(&ram_disk, "screenshot.bmp");

    /* If we get here the FileX test failed!  */
    return;
}
#endif

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_565rgb,
                                LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);
    /* create the scroll_wheel_screen */
    gx_studio_named_widget_create("scroll_wheel_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}

void scroll_wheel_value_set(GX_PROMPT *prompt, INT selected_row)
{
    GX_CHAR text_buffer[10];
    GX_STRING text;

    gx_utility_ltoa(selected_row, text_buffer, 10);
    text.gx_string_ptr = text_buffer;
    text.gx_string_length = strnlen(text_buffer, sizeof(text_buffer));
    gx_prompt_text_set_ext(prompt, &text);
}


/* Override default event processing of scroll wheel screen. */
UINT scroll_wheel_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_CHECKBOX, GX_EVENT_TOGGLE_ON):
    case GX_SIGNAL(ID_CHECKBOX, GX_EVENT_TOGGLE_OFF):
        generate_screen_shot();
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

#ifdef USE_FILEX_API
UINT write_data_callback(GX_UBYTE *byte_data, UINT data_count)
{
    /* Write a string to the test file.  */
    fx_file_write(&my_file, byte_data, data_count);

    return GX_SUCCESS;
}

VOID generate_screen_shot()
{
    GX_RECTANGLE size = { 0,0,640,480 };

    /* open file */
    fx_file_open(&ram_disk, &my_file, "screenshot.bmp", FX_OPEN_FOR_WRITE);

    gx_utility_canvas_to_bmp(root->gx_window_root_canvas, &size, write_data_callback);
    /* Close file */

    /* Close the test file.  */
    fx_file_close(&my_file);
}
#else
    
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

FILE *fp = GX_NULL;
UINT write_data_callback(GX_UBYTE *byte_data, UINT data_count)
{
    if (fp)
    {
        fwrite(byte_data, 1, data_count, fp);
    }
    return GX_SUCCESS;
}

VOID generate_screen_shot()
{
    GX_RECTANGLE size;
    GX_CHAR pathname[MAX_PATH_LENGTH + 1];
    INT len;

    size = scroll_wheel_screen.gx_widget_size;
    len = extract_path(__FILE__, pathname);
    memcpy(pathname + len, bmp_file, sizeof(bmp_file));
    fp = fopen(pathname, "wb");
    if (!fp)
    {
        printf("Cannot open file: %s\n", pathname);
    }
    gx_utility_canvas_to_bmp(root->gx_window_root_canvas, &size, write_data_callback);
    fclose(fp);

    size = scroll_wheel_screen.scroll_wheel_screen_month_wheel_1.gx_widget_size;
    memcpy(pathname + len, scroll_wheel_bmp_file, sizeof(scroll_wheel_bmp_file));
    fp = fopen(pathname, "wb");
    if (!fp)
    {
        printf("Cannot open file: %s\n", pathname);
    }
    gx_utility_canvas_to_bmp(root->gx_window_root_canvas, &size, write_data_callback);
    fclose(fp);
}
#endif

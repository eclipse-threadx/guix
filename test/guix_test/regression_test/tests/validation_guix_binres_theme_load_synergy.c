/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_binres_theme_load_synergy", /* Test name */
    5, 5, 630, 470  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
#ifdef GX_UTF8_SUPPORT
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);
#else
    printf("guix_binres_theme_load:Skip Testing!\n");
#endif
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_dave2d_graphics_driver_setup_565rgb  
#undef win32_dave2d_graphics_driver_setup_565rgb  
#endif
#define win32_dave2d_graphics_driver_setup_565rgb  gx_validation_dave2d_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#define REGRESSION_TEST_MODE

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_themes_16bpp_synergy.c"

char test_comment[256];

typedef struct TEST_TRUCT{
char *bin_file_path;
char *comment;
}TEST;

TEST test_list[]={
{"..//..//..//example_internal//multi_themes_16bpp_synergy//binary_files_for_regression//theme_4_invalid_compressed_glyph_magic_number.bin", "invalid compressed glyph magic number"},
{GX_NULL, GX_NULL}
};

UINT test_load_binres_to_ram(char *pathname)
{
    FILE *p_file;
    size_t total_length;
    size_t result;

    p_file = fopen(pathname, "rb");

    if (!p_file)
    {
        printf("Cannot open file: %s\n", pathname);
        return GX_FAILURE;
    }

    fseek(p_file, 0, SEEK_END);
    total_length = ftell(p_file);
    fseek(p_file, SEEK_SET, SEEK_SET);

    if(binres_root_address)
    {
        memory_free((void *)binres_root_address);
    }

    binres_root_address = memory_allocate(total_length);
    result = fread(binres_root_address, 1, total_length, p_file);

    fclose(p_file);

    if (result != total_length)
    {
        return GX_FAILURE;
    }
    return GX_SUCCESS;
}

UINT test_load_binres_theme(GX_UBYTE *root_address, INT theme_id)
{
UINT  status = GX_SUCCESS;

    if (theme)
    {
        memory_free(theme);
        theme = GX_NULL;
    }

    status = gx_binres_theme_load(root_address, theme_id, &theme);

    if (theme &&
        theme->theme_color_table &&
        theme->theme_font_table &&
        theme->theme_pixelmap_table)
    {
        gx_display_theme_install(multi_themes_16bpp_synergy_display_table[MAIN_DISPLAY].display, theme);
    }

    return status;
}

static VOID control_thread_entry(ULONG input)
{
#ifdef GX_UTF8_SUPPORT
INT   frame_id = 1;
TEST *test = test_list;
char  pathname[255];
int   pathlen;

    gx_display_theme_install(multi_themes_16bpp_synergy_display_table[MAIN_DISPLAY].display, main_display_theme_table[MAIN_DISPLAY_THEME_2]);

    gx_validation_extract_path(__FILE__, pathname, &pathlen);

    while(test->bin_file_path)
    {
        sprintf(pathname + pathlen, "%s", test->bin_file_path);
        test_load_binres_to_ram(pathname);
        test_load_binres_theme(binres_root_address, 0);

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test->comment);
        gx_validation_screen_refresh();

        gx_display_theme_install(multi_themes_16bpp_synergy_display_table[MAIN_DISPLAY].display, main_display_theme_table[MAIN_DISPLAY_THEME_2]);
        test++;
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();
#endif
    exit(0);
}

/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_binres_language_table_load", /* Test name */
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
    printf("guix_binres_language_table_load:Skip Testing!\n");
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
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#define REGRESSION_TEST_MODE

#include "gx_validation_wrapper.h"

#include "demo_guix_multi_themes_16bpp.c"

#define EXPECT_EQ(expected, actual) \
    if((expected) != (actual))          \
    {                               \
        printf("\nERROR! File: %s Line: %d\n", __FILE__, __LINE__); \
        printf("Expected: 0x%x, (%d) Got: 0x%x (%d)\n", (UINT)(expected), (INT)(expected), (UINT)(actual), (INT)(actual)); \
        failed_tests++; \
    }

char test_comment[256];

extern SIMPLE_WINDOW_CONTROL_BLOCK simple_window;

enum ids{
INVALID_LANGUAGE_RESOURCE_HEADER = 0,
INVALID_LANGUAGE_HEADER,
INVALID_LANGUAGE_STRING_COUNT,
INVALID_LANGUAGE_STRING_HEADER,
INVALID_LANGUAGE_AND_STRING_COUNT,
INVALID_LANGUAGE_COUNT_0,
VALID_LANGUAGE_BINRES
};

int binres_ids[] ={
INVALID_LANGUAGE_RESOURCE_HEADER,
INVALID_LANGUAGE_HEADER,
INVALID_LANGUAGE_STRING_COUNT,
INVALID_LANGUAGE_STRING_HEADER,
INVALID_LANGUAGE_AND_STRING_COUNT,
INVALID_LANGUAGE_COUNT_0,
VALID_LANGUAGE_BINRES
};

GX_UBYTE *test_binres_root_address = GX_NULL;
static UINT load_test_binary_files(GX_CONST GX_CHAR *path)
{
FILE  *p_file;
size_t total_length;
size_t result;
    
    if(Japanese_root_address)
    {
        memory_free(test_binres_root_address);
        test_binres_root_address = GX_NULL;
    }
    
    p_file = fopen(path, "rb");

    if (!p_file)
    {
        printf("Cannot open file: %s\n", path);
        return GX_FAILURE;
    }

    fseek(p_file, 0, SEEK_END);
    total_length = ftell(p_file);
    fseek(p_file, SEEK_SET, SEEK_SET);

    test_binres_root_address = memory_allocate(total_length);
    result = fread(test_binres_root_address, 1, total_length, p_file);

    fclose(p_file);

    if (result != total_length)
    {
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_CHAR ***language_table_old = GX_NULL;
UINT load_binres_language_table_old(GX_UBYTE *root_address)
{
UINT       status = GX_SUCCESS;
GX_CHAR ***new_table = GX_NULL;


    status = gx_binres_language_table_load(root_address, (GX_UBYTE ****)&new_table);

    if (status == GX_SUCCESS)
    {
        if (language_table_old)
        {
            memory_free(language_table_old);
        }
       
        gx_display_language_table_set(root->gx_window_root_canvas->gx_canvas_display, new_table, 1, MAIN_DISPLAY_STRING_TABLE_SIZE);
        gx_system_active_language_set(0);

        language_table_old = new_table;
    }
    return status;
}
#endif

void *test_memory_allocator(ULONG size){return GX_NULL;}
void  test_memory_free(void *address){}

static VOID control_thread_entry(ULONG input)
{
INT     frame_id = 1;
UINT    index;
INT     res_id;
GX_CHAR binres_file[255];
INT     pathlen;
UINT    status;
UINT    expect_status;
UINT    old_api_expect_status;
INT     failed_tests = 0;

    gx_validation_extract_path(__FILE__, binres_file, &pathlen);

    for(index = 0; index < sizeof(binres_ids)/sizeof(int); index++)
    {
        res_id = binres_ids[index];
        binres_file[pathlen]='\0';

        switch(res_id)
        {
        case INVALID_LANGUAGE_RESOURCE_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_resource_header.bin");
            sprintf(test_comment, "Load invalid language resource header binary file");
            expect_status = GX_INVALID_FORMAT;
            old_api_expect_status = expect_status;
            break;

        case INVALID_LANGUAGE_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_header.bin");
            sprintf(test_comment, "Load invalid language header binary file");
            expect_status = GX_INVALID_FORMAT;
            old_api_expect_status = expect_status;
            break;

        case INVALID_LANGUAGE_STRING_COUNT:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_string_count.bin");
            sprintf(test_comment, "Load invalid string count binary file");
            expect_status = GX_MATH_OVERFLOW;
            old_api_expect_status = expect_status;
            break;

        case INVALID_LANGUAGE_STRING_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_string_header.bin");
            sprintf(test_comment, "Load invalid string header binary file");
            expect_status = GX_INVALID_FORMAT;
            old_api_expect_status = expect_status;
            break;

        case INVALID_LANGUAGE_AND_STRING_COUNT:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_and_string_count.bin");
            sprintf(test_comment, "Load invalid language and string count binary file");
            expect_status = GX_MATH_OVERFLOW;
            old_api_expect_status = GX_SYSTEM_MEMORY_ERROR;
            break;

        case INVALID_LANGUAGE_COUNT_0:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_count_0.bin");
            sprintf(test_comment, "Load invalid language count (0) binary file");
            expect_status = GX_INVALID_FORMAT;
            old_api_expect_status = expect_status;
            break;

        case VALID_LANGUAGE_BINRES:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//Japanese.bin");
            gx_system_memory_allocator_set(test_memory_allocator, test_memory_free);
            sprintf(test_comment, "Set valid binres, set memory allocator that returns null.");
            expect_status = GX_SYSTEM_MEMORY_ERROR;
            old_api_expect_status = expect_status;
            break;
        }

        status = load_test_binary_files(binres_file);
        EXPECT_EQ(status, GX_SUCCESS);

        status = load_binres_language_table(test_binres_root_address);
        EXPECT_EQ(status, expect_status);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
        status = load_binres_language_table_old(test_binres_root_address);
        EXPECT_EQ(status, old_api_expect_status);
#endif
    }

    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Load language table with new API. */
    status = load_binres_language_table_old(Japanese_root_address);
    EXPECT_EQ(status, GX_SUCCESS);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Load latest version Japanese binres with new API");
    gx_validation_screen_refresh();

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    /* Load language table with old API. */
    status = load_binres_language_table_old(Japanese_root_address);
    EXPECT_EQ(status, GX_SUCCESS);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Load latest version Japanese binres with old API");
    gx_validation_screen_refresh();

    /* Read binres file to memory. */
    binres_file[pathlen] = '\0';
    strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//Japanese_5_5_1.bin");
    status = load_test_binary_files(binres_file);
    EXPECT_EQ(status, GX_SUCCESS);

    /* Load 5.5.1 version language table with old API. */
    status = load_binres_language_table_old(test_binres_root_address);
    EXPECT_EQ(status, GX_SUCCESS);

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Load 5.5.1 version Japanese binres with old API");
    gx_validation_screen_refresh();
#endif
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    if(failed_tests == 0)
    {
        exit(0);
    }
    else
    {
        exit(1);
    }
}

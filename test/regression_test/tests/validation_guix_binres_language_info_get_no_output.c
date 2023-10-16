/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {0};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static TX_THREAD               Guix_API_Test;
static VOID      control_thread_entry(ULONG);
static ULONG     control_thread_stack[1024];

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    tx_thread_create(&Guix_API_Test, "GUIX API Test", control_thread_entry,
                     0, control_thread_stack, sizeof(control_thread_stack), 
                     GX_SYSTEM_THREAD_PRIORITY + 1, GX_SYSTEM_THREAD_PRIORITY + 1,
                     TX_NO_TIME_SLICE, TX_AUTO_START);
}

/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_multi_themes_16bpp.c"

enum ids{
BINRES_VALID = 0,
BINRES_INVALID_RESOURCE_HEADER,
BINRES_INVALID_STRING_HEADER,
BINRES_INVALID_LANGUAGE_HEADER,
BINRES_TEST_SIZE
};

GX_UBYTE *test_binres_root_address = GX_NULL;
static UINT load_test_binary_files(GX_CONST GX_CHAR *path)
{
FILE  *p_file;
size_t total_length;
size_t result;
    
    if(test_binres_root_address)
    {
        memory_free(test_binres_root_address);
        test_binres_root_address = GX_NULL;
    }
    
    p_file = fopen(path, "rb");

    if (!p_file)
    {
        printf("Cannot open file:%s\n", path);
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

static VOID control_thread_entry(ULONG input)
{
int                failed_tests = 0;
UINT               status;
UINT               expected_status;
GX_VALUE           count;
GX_LANGUAGE_HEADER info;
UINT               index;
GX_CHAR            binres_file[255];
INT                pathlen;

    gx_validation_extract_path(__FILE__, binres_file, &pathlen);

    status = gx_binres_language_count_get(GX_NULL, &count);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_language_count_get((GX_UBYTE *)0x60000, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_language_info_load(GX_NULL, &info);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_language_info_load((GX_UBYTE *)0x60000, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    for(index = 0; index < BINRES_TEST_SIZE; index++)
    {
        binres_file[pathlen] = '\0';

        switch(index)
        {
        case BINRES_VALID:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//Japanese.bin");
            expected_status = GX_SUCCESS;
            break;

        case BINRES_INVALID_RESOURCE_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_resource_header.bin");
            expected_status = GX_INVALID_FORMAT;
            break;

        case BINRES_INVALID_STRING_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_string_header.bin");
            expected_status = GX_INVALID_FORMAT;
            break;

        case BINRES_INVALID_LANGUAGE_HEADER:
            strcat(binres_file, "..//..//..//example_internal//multi_themes_16bpp//binary_files_for_regression//English_invalid_language_header.bin");
            expected_status = GX_INVALID_FORMAT;
            break;
        }

        status = load_test_binary_files(binres_file);
        EXPECT_EQ(status, GX_SUCCESS);

        if(index != BINRES_INVALID_LANGUAGE_HEADER)
        {
            status = gx_binres_language_count_get(test_binres_root_address, &count);
            EXPECT_EQ(expected_status, status);
        }

        status = gx_binres_language_info_load(test_binres_root_address, &info);
        EXPECT_EQ(expected_status, status);
    }
    
    if(failed_tests == 0) 
    {
        printf("Guix Test:   guix_binres_language_info_get_no_output.........................................Success!\n");
        exit(0);
    }
    else 
    {
        printf("Guix Test:   guix_binres_language_info_get_no_output..........................................Failed!\n");
        exit(1);
    }
}

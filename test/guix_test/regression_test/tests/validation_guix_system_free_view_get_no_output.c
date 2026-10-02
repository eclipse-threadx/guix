/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* Portions of this file were generated with AI assistance. */

/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_system_free_view_get_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area. */
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

/* Exhaust and restore the available view pool while drawing is locked. */
static VOID control_thread_entry(ULONG input)
{
INT      failed_tests = 0;
UINT     views_used = 0;
GX_VIEW *saved_views;
GX_VIEW *view;

    GX_ENTER_CRITICAL
    saved_views = _gx_system_free_views;

    while ((_gx_system_free_views != GX_NULL) && (views_used < GX_MAX_VIEWS))
    {
        view = _gx_system_free_view_get();
        EXPECT_EQ(GX_TRUE, view != GX_NULL);
        views_used++;
    }

    EXPECT_EQ(GX_TRUE, views_used > 0);
    EXPECT_EQ(GX_NULL, _gx_system_free_views);
    EXPECT_EQ(GX_NULL, _gx_system_free_view_get());
    EXPECT_EQ(GX_SYSTEM_OUT_OF_VIEWS, _gx_system_last_error);
    _gx_system_free_views = saved_views;
    GX_EXIT_CRITICAL

    if (!failed_tests)
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}

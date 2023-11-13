
#include "demo_jpeg_decode_24xrgb_rotated_ccw_resources.h"
#include "demo_jpeg_decode_24xrgb_rotated_ccw_specifications.h"
#include "common_code.h"
#include "golden_results.c"

GX_WINDOW_ROOT *root;

int main()
{
		  tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
GX_CANVAS *canvas;
	
	  __enable_interrupts();
	
    memory_pool_create();

    test_start(0, /* Generate golden results */
               1, /* Run test */
               0, /* Generate screen shot. */
               golden_results, /* Golden results table */
               sizeof(golden_results)/sizeof(UINT) /* Test count */
               );

    /* Initialize GUIX. */
    gx_system_initialize();

    /* Install memory allocator and de-allocator.  */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, graphics_driver_24xrgb_rotated_setup, LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make "main_screen" screen visible.  */
    gx_widget_show(root);

    /* Start GUIX thread */
    gx_system_start();
    
    canvas = root->gx_window_root_canvas;
    canvas->gx_canvas_status = GX_CANVAS_SIMPLE | GX_CANVAS_VISIBLE;
    gx_canvas_drawing_initiate(canvas, root, &root->gx_widget_size);
    gx_window_draw((GX_WINDOW *)root);
    
    test_end();
}

VOID main_screen_draw(GX_WINDOW *window)
{
GX_RECTANGLE *size = &window->gx_window_client;
INT frame_id = 1;
MAP_INFO map_info_list[]={
      {GX_PIXELMAP_ID_RED_APPLE, "RED_APPLE", size->gx_rectangle_left, size->gx_rectangle_top + 105},
      {GX_PIXELMAP_ID_FISH_SMALL_411, "FISH_SMALL_411", size->gx_rectangle_left - 100, size->gx_rectangle_top - 100},
      {GX_PIXELMAP_ID_FISH_SMALL_444, "FISH_SMALL_444", size->gx_rectangle_right - 100, size->gx_rectangle_top - 101},
      {GX_PIXELMAP_ID_FISH_SMALL_420, "FISH_SMALL_420",size->gx_rectangle_left - 101, size->gx_rectangle_bottom - 100},
      {GX_PIXELMAP_ID_FISH_SMALL_422, "FISH_SMALL_422",size->gx_rectangle_right - 101, size->gx_rectangle_bottom - 101},
      {0, "", 0, 0}
    };

  /* Test decode JPEG to canvas. */
  test_jpeg_to_canvas_draw(root->gx_window_root_canvas, window, map_info_list, frame_id++);

  /* Test decode JPEG to pixelmap. */
  test_jpeg_to_pixelmap_draw(root->gx_window_root_canvas, window, map_info_list, DISPLAY_1_COLOR_FORMAT, GX_IMAGE_READER_MODE_ROTATE_CCW, frame_id++);
}
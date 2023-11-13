
#include "common_code.h"

#define QUOTIENT 0x04c11db7
static GX_BOOL generate = GX_FALSE;
static GX_BOOL test = GX_FALSE;
static GX_BOOL screenshot = GX_FALSE;
static UINT *golden_results = GX_NULL;
static UINT  golden_results_size = 0;
static UINT  test_count = 0;
static UINT  failed_count = 0;
static UINT  crc_table[256];
static UINT  tick_counter = 0;
static UINT  start_time = 0;
static FILE *canvas_fp = GX_NULL;
static FILE *golden_fp = GX_NULL;
static FILE *output_fp = GX_NULL;

/******************************************************************************************/
/* Define memory allocator function.                                                     */
/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
  return malloc(size);
}

/******************************************************************************************/
/* Define memory de-allocator function.                                                   */
/******************************************************************************************/
void memory_free(VOID *mem)
{
  free(mem);
}

void SysTick_Handler(void)
{
  tick_counter++;
}

VOID display_buffer_toggle(GX_CANVAS *canvas, GX_RECTANGLE *dirty)
{
    
}

VOID graphics_driver_8bit_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
    display -> gx_display_palette = palette;
    display -> gx_display_palette_size = count;
}

UINT graphics_driver_565rgb_setup(GX_DISPLAY *display)
{
    _gx_display_driver_565rgb_setup(display, GX_NULL, display_buffer_toggle);
    
    return GX_SUCCESS;
}

UINT graphics_driver_1555xrgb_setup(GX_DISPLAY *display)
{
    _gx_display_driver_1555xrgb_setup(display, GX_NULL, display_buffer_toggle);
    
    return GX_SUCCESS;
}

UINT graphics_driver_24xrgb_setup(GX_DISPLAY *display)
{
    _gx_display_driver_24xrgb_setup(display, GX_NULL, display_buffer_toggle);
    
    return GX_SUCCESS;
}

UINT graphics_driver_8bit_palette_setup(GX_DISPLAY *display)
{
    _gx_display_driver_8bit_palette_setup(display, GX_NULL, display_buffer_toggle);
    display -> gx_display_driver_palette_set = graphics_driver_8bit_palette_set;
    
    return GX_SUCCESS;
}

UINT graphics_driver_565rgb_rotated_setup(GX_DISPLAY *display)
{
    _gx_display_driver_565rgb_rotated_setup(display, GX_NULL, display_buffer_toggle);
    
    return GX_SUCCESS;
}

UINT graphics_driver_24xrgb_rotated_setup(GX_DISPLAY *display)
{
    _gx_display_driver_24xrgb_rotated_setup(display, GX_NULL, display_buffer_toggle);
    
    return GX_SUCCESS;
}

UINT graphics_driver_8bit_palette_rotated_setup(GX_DISPLAY *display)
{
    _gx_display_driver_8bit_palette_rotated_setup(display, GX_NULL, display_buffer_toggle);
    display -> gx_display_driver_palette_set = graphics_driver_8bit_palette_set;
    
    return GX_SUCCESS;
}

UINT write_data_callback(GX_UBYTE *byte_data, UINT data_count)
{
    if(canvas_fp)
    {
        fwrite(byte_data, 1, data_count, canvas_fp);
    }

    return GX_SUCCESS;
}

VOID test_start(GX_BOOL g, GX_BOOL t, GX_BOOL s, UINT *results, UINT size)
{
    generate = g;
    test = t;
    screenshot = s;
    golden_results = results;
    golden_results_size = size;
}

VOID test_verify_canvas(GX_CANVAS *canvas, INT frame_id)
{
UINT new_crc;
GX_RECTANGLE size;
GX_CHAR string[50];

    if(screenshot)
    {
        sprintf(string, "frame_%d.bmp", frame_id);
        canvas_fp  = fopen (string, "w+");
        gx_utility_rectangle_define(&size, 0, 0, canvas->gx_canvas_x_resolution - 1, canvas->gx_canvas_y_resolution - 1);
        gx_utility_canvas_to_bmp(canvas, &size, write_data_callback);
        fclose(canvas_fp);
        canvas_fp = GX_NULL;
    }
    
    new_crc = crc_calculate((UCHAR *)canvas->gx_canvas_memory, canvas->gx_canvas_memory_size);
  
    if(test)
    {
        if(!golden_results)
        {
           printf("golden results is missing!\n");
           failed_count++;
        }
        else if(frame_id <= golden_results_size)
        {
           if(new_crc != golden_results[frame_id - 1])
           {
               printf("frame %d: failed\n", frame_id);
               failed_count++;
           }
           else
           {
               printf("frame %d: passed\n", frame_id);
           }
        }
        else
        {
          printf("frame %d: failed\n", frame_id);
          failed_count++;
        }
          
        test_count++;
    }
    
    if(generate || test)
    {
        if(frame_id == 1)
        {
          if(generate)
          {
              golden_fp = fopen("golden_results.c", "w+");
          }
          if(test)
          {
              output_fp = fopen("output_results.c", "w+");
          }
          sprintf(string, "UINT golden_results[] ={\n    0x%x", new_crc);
        }
        else
        {
          sprintf(string, ",\n    0x%x", new_crc);
        }
        
        if(golden_fp)
        {
            fwrite(string, 1, strnlen(string, sizeof(string)), golden_fp);
        }
        
        if(output_fp)
        {
            fwrite(string, 1, strnlen(string, sizeof(string)), output_fp);
        }
    }
}

UINT test_end()
{
    if(golden_fp)
    {
       fwrite("\n};", 1, 3, golden_fp);
       fclose(golden_fp);
       golden_fp = GX_NULL;
    }
    
    if(output_fp)
    {
       fwrite("\n};", 1, 3, output_fp);
       fclose(output_fp);
       output_fp = GX_NULL;
    }
    
    if(test)
    {
        printf("\nTests Passed: %d\n", test_count - failed_count);
        printf("Tests Failed: %d\n", failed_count);
          
        return failed_count;
    }
 
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void crc32_init(UINT *crc_table, INT size)
{
INT  i;
INT  j;
UINT crc;

    for (i = 0; i < size; i++)
    {
        crc = i << 24;
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80000000)
                crc = (crc << 1) ^ QUOTIENT;
            else
                crc = crc << 1;
        }
        crc_table[i] = crc;
    }
}

///////////////////////////////////////////////////////////////////////////////
UINT crc_calculate(UCHAR *data, INT len)
{
UINT result;
INT  i;
    
    crc32_init(crc_table, sizeof(crc_table)/sizeof(UINT));

    result = *data++ << 24;
    result |= *data++ << 16;
    result |= *data++ << 8;
    result |= *data++;
    result = ~ result;
    len -=4;
    
    for (i=0; i<len; i++)
    {
        result = (result << 8 | *data++) ^ crc_table[result >> 24];
    }
    
    return ~result;
}

VOID test_jpeg_to_canvas_draw(GX_CANVAS *canvas, GX_WINDOW *window, MAP_INFO *test, INT frame_id)
{
GX_PIXELMAP *map;
ULONG cycle_count;

  /* Test decode JPEG to canvas. */

  gx_window_background_draw(window);
  while(test->map_id)
  {
      gx_context_pixelmap_get(test->map_id, &map);
      start_time = tick_counter;
      gx_canvas_pixelmap_draw(test->xpos, test->ypos, map);
      cycle_count = tick_counter - start_time;
      printf("JPEG to canvas. ID Name: %s, Dimension: %dx%d, cycle count = %ld.%ld(Million Cycles)\n\n", test->name, map->gx_pixelmap_width, map->gx_pixelmap_height, cycle_count / 10, cycle_count % 10);
      test++;
  }
  test_verify_canvas(canvas, frame_id);
}

VOID test_jpeg_to_pixelmap_draw(GX_CANVAS *canvas, GX_WINDOW *window, MAP_INFO *test, GX_UBYTE outformat, GX_UBYTE flags, INT frame_id)
{
GX_PIXELMAP *map;
GX_IMAGE_READER image_reader;
GX_PIXELMAP outmap;
ULONG cycle_count;

  /* Test decode JPEG to pixelmap. */

  gx_window_background_draw(window);
  while(test->map_id)
  {

      gx_context_pixelmap_get(test->map_id, &map);
      
      gx_image_reader_create(&image_reader, map->gx_pixelmap_data, map->gx_pixelmap_data_size, outformat, flags);
      
      if(outformat == GX_COLOR_FORMAT_8BIT_PALETTE)
      {
          gx_image_reader_palette_set(&image_reader, canvas->gx_canvas_display->gx_display_palette, canvas->gx_canvas_display->gx_display_palette_size);
      }
      
      start_time = tick_counter;
      if(gx_image_reader_start(&image_reader, &outmap) == GX_SUCCESS)
      {
          cycle_count = tick_counter - start_time;
          printf("JPGE to pixelmap. ID Name: %s, Dimension: %dx%d, cycle count = %ld.%ld(Million Cycles)\n\n", test->name, map->gx_pixelmap_width, map->gx_pixelmap_height, cycle_count / 10, cycle_count % 10);
          
          gx_canvas_pixelmap_draw(test->xpos, test->ypos, &outmap);
          memory_free((void *)outmap.gx_pixelmap_data);    
      }
      else
      {
          printf("JPEG decode failure!\n");
      }
      test++;
  }
  test_verify_canvas(canvas, frame_id);
}
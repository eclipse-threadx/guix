
#include "common_code.h"

#define QUOTIENT 0x04c11db7
#define MEMORY_POOL_SIZE (640 * 480 * 4)

#define SYSTEM_CLOCK 6000000
#define SYSTICK_CYCLES ((SYSTEM_CLOCK / 100) -1)
#define ONE_MILLION 1000000

static GX_BOOL generate = GX_FALSE;
static GX_BOOL test = GX_FALSE;
static GX_BOOL screenshot = GX_FALSE;
static UINT *golden_results = GX_NULL;
static UINT  golden_results_size = 0;
static UINT  test_count = 0;
static UINT  failed_count = 0;
static UINT  crc_table[256];
static FILE *canvas_fp = GX_NULL;
static FILE *golden_fp = GX_NULL;
static FILE *output_fp = GX_NULL;
static TX_BYTE_POOL memory_pool;
static GX_UBYTE memory_pool_buffer[MEMORY_POOL_SIZE];

extern int stdout_init (void);

/******************************************************************************************/
/* Define memory allocator function.                                                     */
/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
/* Define memory de-allocator function.                                                   */
/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID memory_pool_create()
{
	    /* create byte pool for decode to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", memory_pool_buffer, MEMORY_POOL_SIZE);

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);
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
	
	  stdout_init();
}

static UINT test_string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
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
           printf("golden results is missing!\r\n");
           failed_count++;
        }
        else if(frame_id <= golden_results_size)
        {
           if(new_crc != golden_results[frame_id - 1])
           {
               printf("frame %d: failed\r\n", frame_id);
               failed_count++;
           }
           else
           {
               printf("frame %d: passed\r\n", frame_id);
           }
        }
        else
        {
          printf("frame %d: failed\r\n", frame_id);
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
          sprintf(string, "UINT golden_results[] ={\r\n    0x%x", new_crc);
        }
        else
        {
          sprintf(string, ",\r\n    0x%x", new_crc);
        }
        
        if(golden_fp)
        {
            fwrite(string, 1, test_string_length_get(string, 256), golden_fp);
        }
        
        if(output_fp)
        {
            fwrite(string, 1, test_string_length_get(string, 256), output_fp);
        }
    }
}

UINT test_end()
{
    if(golden_fp)
    {
       fwrite("\r\n};", 1, 3, golden_fp);
       fclose(golden_fp);
       golden_fp = GX_NULL;
    }
    
    if(output_fp)
    {
       fwrite("\r\n};", 1, 3, output_fp);
       fclose(output_fp);
       output_fp = GX_NULL;
    }
    
    if(test)
    {
        printf("\r\nTests Passed: %d\r\n", test_count - failed_count);
        printf("Tests Failed: %d\r\n", failed_count);
          
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
ULONG start_time;
ULONG ticks;

  /* Test decode JPEG to canvas. */

  gx_window_background_draw(window);
  while(test->map_id)
  {
      gx_context_pixelmap_get(test->map_id, &map);
      start_time = tx_time_get();
      gx_canvas_pixelmap_draw(test->xpos, test->ypos, map);
      ticks = tx_time_get() - start_time;
      printf("JPEG to canvas. ID Name: %s, Dimension: %dx%d, cycle count = %ld.%ld(Million Cycles)\r\n\r\n", test->name, map->gx_pixelmap_width, map->gx_pixelmap_height, ticks * SYSTICK_CYCLES / ONE_MILLION, ticks * SYSTICK_CYCLES % ONE_MILLION);
      test++;
  }
  test_verify_canvas(canvas, frame_id);
}

VOID test_jpeg_to_pixelmap_draw(GX_CANVAS *canvas, GX_WINDOW *window, MAP_INFO *test, GX_UBYTE outformat, GX_UBYTE flags, INT frame_id)
{
GX_PIXELMAP *map;
GX_IMAGE_READER image_reader;
GX_PIXELMAP outmap;
ULONG start_time;
ULONG ticks;

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
      
      start_time = tx_time_get();
      if(gx_image_reader_start(&image_reader, &outmap) == GX_SUCCESS)
      {
          ticks = tx_time_get() - start_time;
          printf("JPGE to pixelmap. ID Name: %s, Dimension: %dx%d, cycle count = %ld.%ld(Million Cycles)\r\n\r\n", test->name, map->gx_pixelmap_width, map->gx_pixelmap_height, ticks * SYSTICK_CYCLES / ONE_MILLION, ticks * SYSTICK_CYCLES % ONE_MILLION);
          
          gx_canvas_pixelmap_draw(test->xpos, test->ypos, &outmap);
          memory_free((void *)outmap.gx_pixelmap_data);    
      }
      else
      {
          printf("JPEG decode failure!\r\n");
      }
      test++;
  }
  test_verify_canvas(canvas, frame_id);
}
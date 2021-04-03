/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "demo_guix_binres_resources.h"
#include "demo_guix_binres_specifications.h"

/* This demo can be built and run on evaluation boards such as the STM32F439, in which
   case the binary resource data is programmed to FLASH prior to running this demo.
*/

#ifdef STM32F439xx
#include "stm32f4xx_hal.h"
#endif

#define GUIX_POOL_SIZE        (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)
#define DEFAULT_CANVAS_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)

GX_WINDOW_ROOT               *root;
TX_BYTE_POOL                  memory_pool;
TX_THREAD                     demo_thread;
ULONG                         demo_thread_stack[1024];

GX_VALUE language_count;


GX_LANGUAGE_HEADER language_info[8];

/* reference the display table, theme table, and language table
   produced by GUIX Studio
*/
extern GX_STUDIO_DISPLAY_INFO demo_guix_binres_display_table[];
extern GX_CONST GX_THEME     *main_display_theme_table[];
extern GX_CONST GX_UBYTE    **main_display_language_table[];

/* pointers which will be initialized when the binary resources
   are installed
*/
GX_THEME   *theme = GX_NULL;
GX_STRING **language_table = GX_NULL;
GX_UBYTE   *binres_root_address = GX_NULL;
GX_CHAR     binres_pathname[] = "..//..//demo_guix_binres_resources.bin";

/* Define prototypes.   */
VOID demo_thread_entry(ULONG thread_input);

#ifdef GX_TARGET_WIN32
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
GX_COLOR       default_canvas_memory[DEFAULT_CANVAS_PIXELS];
UCHAR          guix_pool_memory[GUIX_POOL_SIZE];
#endif

#ifdef STM32F439xx
extern UINT  stm32f429_graphics_driver_setup_565rgb(GX_DISPLAY *graphic);
extern VOID  touch_thread_entry(ULONG thread_input);

#define SDRAM _Pragma("location=\"SD_RAM\"")
SDRAM GX_COLOR default_canvas_memory[DEFAULT_CANVAS_PIXELS];
SDRAM UCHAR    guix_pool_memory[GUIX_POOL_SIZE];
TX_THREAD      touch_thread;
ULONG          touch_thread_stack[1024];
#endif

/* local application function prototypes */

VOID *load_binary_resource_data_to_ram();
UINT load_theme(GX_UBYTE *root_address, INT theme_id);
UINT load_language_table(GX_UBYTE *root_address);

UINT load_language_info(GX_UBYTE *root_address, GX_LANGUAGE_HEADER *put_info);
UINT load_language_count(GX_UBYTE *root_address, GX_VALUE *put_count);


#ifdef STM32F439xx
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
    clocked below the maximum system frequency, to update the voltage scaling value 
    regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();
    
    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
#endif

/************************************************************************************/
/*  User defined memory allocate function.                                          */
/************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/************************************************************************************/
/*  User defined memory free function.                                              */
/************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/************************************************************************************/
/*  Program entry.                                                                  */
/************************************************************************************/
int main(int argc, char ** argv)
{
#ifdef STM32F439xx
    /* STM32F4xx HAL library initialization */
    HAL_Init();
    
    /* Configure the system clock to 180 MHz */
    SystemClock_Config(); 
    
    BSP_NOR_Init();
#endif

    /* Enter the ThreadX kernel. */
    tx_kernel_enter();
    return(0);
}

/************************************************************************************/
/*  Define system initialization.                                                   */
/************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
      tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,  
                       demo_thread_stack, sizeof(demo_thread_stack), 
                       GX_SYSTEM_THREAD_PRIORITY + 1,
                       GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
     
#ifdef STM32F439xx
      tx_thread_create(&touch_thread, "Touch Thread", touch_thread_entry, 0,  
                       touch_thread_stack, sizeof(touch_thread_stack), 
                       GX_SYSTEM_THREAD_PRIORITY + 1,
                       GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
#endif
}

VOID demo_thread_entry(ULONG thread_input)
{
      /* Initialize GUIX.  */
    gx_system_initialize();

    /* Set memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Create a display driver based on the target platform */
#if defined(GX_TARGET_WIN32)
    /* Configure display. */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);
#elif defined(STM32F439xx)
    gx_studio_display_configure(MAIN_DISPLAY, stm32f429_graphics_driver_setup_565rgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);
#endif

    /* initialize the canvas memory pointer (not allocated by Studio) */

#if defined(GX_TARGET_WIN32) || defined(STM_32F439xx)
    root -> gx_window_root_canvas->gx_canvas_memory = default_canvas_memory;

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);
#endif

#ifdef GX_TARGET_WIN32
    /* Load binary resource file into ram. */
    /* This suit the case that we generated a .bin format resource file. */
    binres_root_address = load_binary_resource_data_to_ram();
#endif

#ifdef STM32F439xx
    /* Specify address that binary resource data is loated. */
    /* This suit the case that we generate a .srec format resource file. */
    /* .srec file should be programed to the target device memory first. */
    binres_root_address = (GX_UBYTE *)0x60000000;
#endif
    
    /* Load language table. */
    load_language_table(binres_root_address);

    /* Load theme table. */
    load_theme(binres_root_address, 0);

    /* Create the "simple_window" screen. */
    gx_studio_named_widget_create("simple_window", (GX_WIDGET *)root, GX_NULL);

    gx_binres_language_count_get(binres_root_address, &language_count);
    gx_binres_language_info_load(binres_root_address, language_info);

    /* Show the root window to make "simple_window" screen visible.  */
    gx_widget_show(root);

    /* Start GUIX thread */
    gx_system_start();
}

/************************************************************************************/
/*  Override default event processing of "simple_window" to handle signals from     */
/*  child widgets.                                                                  */
/************************************************************************************/
UINT simple_window_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_THEME_1, GX_EVENT_RADIO_SELECT):
        /* Load first theme from generated binary resource data. */
        load_theme(binres_root_address, 0);
        break;

    case GX_SIGNAL(ID_THEME_2, GX_EVENT_RADIO_SELECT):
        /* Load second theme from generated binary resource data. */
        load_theme(binres_root_address, 1);
        break;

    case GX_SIGNAL(ID_ENGLISH, GX_EVENT_RADIO_SELECT):
        /* Set action language to English. */
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;

    case GX_SIGNAL(ID_SPANISH, GX_EVENT_RADIO_SELECT):
        /* Set active language to Spanish. */
        gx_system_active_language_set(LANGUAGE_SPANISH);
        break;

    default:
        /* for all other events, allow the default event
           processor to run
        */
        return gx_window_event_process(widget, event_ptr);
    }

    return 0;
}

/************************************************************************************/
/*  Load binary resource data into ram.                                             */
/*  This is an example of how an application might load a binary resource data
    file to RAM (or FLASH) from a local filesystem. In this case we are using generic ANSI
    file I/O to read the resource data file.

    Note that for a typical embedded target, this step is not required. For most
    targets, the binary resource data has been directly programmed to FLASH rather
    than saved in a filesytem.
*/
/************************************************************************************/
VOID *load_binary_resource_data_to_ram()
{
UCHAR *address = GX_NULL;

    /* If generated resource is stored in a FAT filesystem, it must be 
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply 
       read the file to RAM. */
    FILE *p_file;
    size_t total_length;

    p_file = fopen(binres_pathname, "rb");

    if (p_file)
    {
        fseek(p_file, 0, SEEK_END);
        total_length = ftell(p_file);
        fseek(p_file, SEEK_SET, SEEK_SET);

        address = memory_allocate(total_length);
        fread(address, 1, total_length, p_file);

        fclose(p_file);
    }

    return address;
}

/************************************************************************************/
/*  Load a theme.                                                                   */
/************************************************************************************/
UINT load_theme(GX_UBYTE *root_address, INT theme_id)
{
UINT  status = GX_SUCCESS;

    if (theme)
    {
        memory_free(theme);
        theme = GX_NULL;
    }

    /* Load a theme from binary data memory. */
    status = gx_binres_theme_load(root_address,/* Address of binary resource data. */
                                  theme_id,    /* Theme identification, 0, 1, 2: 1th, 2nd, 3rd theme in the binary resource data. */
                                  &theme);     /* Loaded theme. */

    if (status == GX_SUCCESS)
    {
        /* Install themes to specified display. */
        gx_display_theme_install(demo_guix_binres_display_table[MAIN_DISPLAY].display, theme);
    }

    return status;
}

/************************************************************************************/
/*  Load language table.                                                            */
/************************************************************************************/
UINT load_language_table(GX_UBYTE *root_address)
{
UINT  status = GX_SUCCESS;

    if (language_table)
    {
        memory_free(language_table);
        language_table = GX_NULL;
    }

    /* Load language table from binary data memory. */
    status = gx_binres_language_table_load_ext(root_address,     /* Address of binary resource data. */
                                              &language_table); /* Loaded language table that contains all languages in the specified binary resource data. */

    if (language_table)
    {
        /* Set language table. */
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING **)language_table, MAIN_DISPLAY_LANGUAGE_TABLE_SIZE, MAIN_DISPLAY_STRING_TABLE_SIZE);

        /* Set active language. */
        gx_system_active_language_set(LANGUAGE_ENGLISH);
    }
    return status;
}

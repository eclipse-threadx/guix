#define _POSIX_SOURCE
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"
#include "gx_validation_verify.h"
#include "stdio.h"

/* Check version definitions. */
#ifndef __PRODUCT_GUIX__
#error "__PRODUCT_GUIX__ is not defined."
#endif /* __PRODUCT_GUIX__ */

#if defined(EXPECTED_MAJOR_VERSION) && ( !defined(__GUIX_MAJOR_VERSION) || (__GUIX_MAJOR_VERSION != EXPECTED_MAJOR_VERSION))
#error "__GUIX_MAJOR_VERSION"
#endif /* EXPECTED_MAJOR_VERSION */

#if defined(EXPECTED_MINOR_VERSION) && ( !defined(__GUIX_MINOR_VERSION) || (__GUIX_MINOR_VERSION != EXPECTED_MINOR_VERSION))
#error "__GUIX_MINOR_VERSION"
#endif /* EXPECTED_MINOR_VERSION */

#if defined(__linux) && defined(RUNTIME_LIMITATION)
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static pid_t child_pid = -1;

void kill_child(int sig)
{
    kill(child_pid, SIGKILL);
    printf("%s: RUNNING OUT OF TIME!\n", test_parameter.test_name);
}
#endif

static char output_failures_file_string[256];
static char golden_checksum_file_string[256];
static char output_checksum_file_string[256];
static char golden_file_string[256];
static char output_file_string[256];
static char golden_file_path[128];
static char output_file_path[128];
static char golden_file_name[128];
static char output_file_name[128];
static FILE *output_file = GX_NULL;
static FILE *output_checksum_file = GX_NULL;
static FILE *output_failures_file = GX_NULL;
static int  do_compare = 1;
static int  checksum = 0;
static int  generate = 0;

static int total_frames = 0;
static int total_failures = 0;
static ULONG gx_validation_frame_capture_timeout = 0;
static int gx_validation_frame_capture_max_frame = 0;
static char **gx_validation_frame_capture_comments = NULL;
static int gx_validation_frame_capture_comments_max = 0;
static int gx_validation_frame_capture_current = 0;
static GX_BOOL gx_validation_no_output = GX_TRUE;
extern TEST_PARAM test_parameter;

char *gx_validation_frame_buffer = NULL;
int   gx_validation_frame_size = 0;
int   gx_validation_frame_id = 0;
char *gx_validation_frame_comment = NULL;
int   gx_validation_frame_width = 0;
int   gx_validation_frame_height = 0;
char *gx_validation_frame_color_depth = NULL;
int   gx_validation_record_frame = 0;

TX_SEMAPHORE *gx_validation_semaphore = NULL;
TX_SEMAPHORE gx_validation_timer_semaphore;

#define GX_VALIDATION_MAX_QUEUE_TIMERS  48
#define QUOTIENT 0x04c11db7

static unsigned int crc_table[256];
static void gx_validation_crc32_init(void)
{
    int i, j;

    unsigned int crc;

    for (i = 0; i < 256; i++)
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

static unsigned int gx_validation_calc_crc(unsigned char *data, int len)
{
    unsigned int        result;
    int                 i;

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

void gx_validation_setup(int argc, char **argv)
{
#if defined(__linux) && defined(RUNTIME_LIMITATION)
    int status;
#endif
    int i;

    memset(golden_checksum_file_string, 0, sizeof(golden_checksum_file_string));
    memset(output_checksum_file_string, 0, sizeof(output_checksum_file_string));
    memset(golden_file_string, 0, sizeof(golden_file_string));
    memset(output_file_string, 0, sizeof(output_file_string));
    memset(golden_file_path, 0, sizeof(golden_file_path));
    memset(golden_file_name, 0, sizeof(golden_file_name));
    memset(output_file_path, 0, sizeof(output_file_path));
    memset(output_file_name, 0, sizeof(output_file_name));
    
    
    /* Set the default golden file path and output file path */
    memcpy(golden_file_path, DEFAULT_GOLDEN_FILE_PATH, sizeof(DEFAULT_GOLDEN_FILE_PATH));
    memcpy(output_file_path, DEFAULT_OUTPUT_FILE_PATH, sizeof(DEFAULT_OUTPUT_FILE_PATH));

    sprintf(golden_file_name, "%s"DATAFILE_EXTENSION, test_parameter.test_name);
    sprintf(output_file_name, "%s"DATAFILE_EXTENSION, test_parameter.test_name);

    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], GOLDEN_FILE_PATH_FLAG) == 0)
        {
            i++;
            strncpy(golden_file_path, argv[i], sizeof(golden_file_path) - 1);
        }
        else if(strcmp(argv[i], GOLDEN_FILE_NAME_FLAG) == 0)
        {
            i++;
            strncpy(golden_file_name, argv[i], sizeof(golden_file_name) - 1);
        }

        if(strcmp(argv[i], OUTPUT_FILE_PATH_FLAG) == 0)
        {
            i++;
            strncpy(output_file_path, argv[i], sizeof(output_file_path) - 1);
        }
        else if(strcmp(argv[i], OUTPUT_FILE_NAME_FLAG) == 0)
        {
            i++;
            strncpy(output_file_name, argv[i], sizeof(output_file_name) - 1);
        }
        else if(strcmp(argv[i], RUN_ONLY_FLAG) == 0)
        {
            do_compare = 0;
        }
        else if(strcmp(argv[i], CHECKSUM_FLAG) == 0)
        {
            /* Compare with checksum. */
            checksum = 1;
        }
        else if(strcmp(argv[i], GENERATE_FLAG) == 0)
        {
            /* Generate golden file. */
            generate = 1;
        }
    }

    gx_validation_no_output = GX_FALSE;

    sprintf(output_failures_file_string, "%s%s_failures.bin", output_file_path, test_parameter.test_name);

    sprintf(golden_checksum_file_string, "%s%s.checksum", golden_file_path, test_parameter.test_name);
    sprintf(output_checksum_file_string, "%s%s.checksum", output_file_path, test_parameter.test_name);
    
    strncpy(golden_file_string, golden_file_path, sizeof(golden_file_string));
    strncat(golden_file_string, golden_file_name, sizeof(golden_file_string));
    strncpy(output_file_string, output_file_path, sizeof(output_file_string));
    strncat(output_file_string, output_file_name, sizeof(output_file_string));

    if(generate || checksum)
    {
        gx_validation_crc32_init();
    }

    if(do_compare)
    {
        if(checksum)
        {
            gx_validation_verify_start(test_parameter.test_name, golden_checksum_file_string, checksum);
        }
        else
        {
            gx_validation_verify_start(test_parameter.test_name, golden_file_string, checksum);
        }
        
    }

#if defined(__linux) && defined(RUNTIME_LIMITATION)
    signal(SIGALRM, (void (*)(int))kill_child);
    child_pid = fork();
    if (child_pid > 0)
    {
        alarm(RUNTIME_LIMITATION);
        wait(&status);
        exit(WEXITSTATUS(status));
    }
#endif
}

void gx_validation_frame_info_set(char *color_depth, int width, int height)
{
    gx_validation_frame_width = width;
    gx_validation_frame_height = height;
    gx_validation_frame_color_depth = color_depth;
}

void gx_validation_write_output_file_header(FILE *output_file)
{
    fputs(TOTAL_FRAMES"          \n", output_file);
    fprintf(output_file, FILE_FORMAT"%d\n", FILE_FORMAT_VERSION_CURRENT);
    fprintf(output_file, GX_VERSION_MAJOR"%d\n", __GUIX_MAJOR_VERSION);
    fprintf(output_file, GX_VERSION_MINOR"%d\n", __GUIX_MINOR_VERSION);
    fprintf(output_file, FILE_CREATE_DATE"%s %s\n", __DATE__, __TIME__);
    fprintf(output_file, COLOR_DEPTH"%s\n", gx_validation_frame_color_depth);
    fprintf(output_file, WIDTH"%d\n", gx_validation_frame_width);
    fprintf(output_file, HEIGHT"%d\n", gx_validation_frame_height);
}

int gx_validation_create_output_file(char *color_depth, int width, int height)
{
    gx_validation_frame_info_set(color_depth, width, height);

    /* Open output file for write */
    if(generate)
    {
        output_file = fopen(output_file_string, "wb");
        output_checksum_file = fopen(output_checksum_file_string, "wb");

        if((output_file == 0) || (output_checksum_file == 0))
        {
            return -1;
        }
        gx_validation_write_output_file_header(output_file);
        gx_validation_write_output_file_header(output_checksum_file);
    }
    return 0;
}

int gx_validation_create_output_failures_file()
{
    output_failures_file = fopen(output_failures_file_string, "wb");

    if(output_failures_file == 0)
    {
        return -1;
    }

    gx_validation_write_output_file_header(output_failures_file);

    return 0;
}

int gx_validation_create_frame_buffer(int frame_size)
{
    /* Allocate memory for frame buffer. */
    if(gx_validation_frame_buffer)
    {
        free(gx_validation_frame_buffer);
        gx_validation_frame_buffer = NULL;
    }

    gx_validation_frame_size = frame_size;

    gx_validation_frame_buffer = (char *)malloc(frame_size);
    memset(gx_validation_frame_buffer, 0, frame_size);

    if(!gx_validation_no_output)
    {
        tx_semaphore_create(&gx_validation_timer_semaphore, "timer_semaphore", 0);
        _gx_system_timer.tx_timer_internal.tx_timer_internal_timeout_function = gx_validation_system_timer_expiration;
    }

    return(0);
}

void gx_validation_write_one_frame(FILE *file)
{
    /* Write the frame number */
    fprintf(file, "\n\n"FRAME_ID"%d\n", gx_validation_frame_id);
    if(gx_validation_frame_comment)
    {
        fprintf(file, FRAME_COMMENT"%s\n", gx_validation_frame_comment);
    }
    fprintf(file, FRAME_DATA"\n");

    /* Write the frame data */
    fwrite((char *)gx_validation_frame_buffer, sizeof(char), gx_validation_frame_size, file);

}

void gx_validation_write_frame_buffer(void)
{
unsigned int result = 0;
int status;
TX_INTERRUPT_SAVE_AREA

    TX_DISABLE

    gx_validation_record_frame = 0;

    if(generate || checksum)
    {
        /* Calculate checksum for canvas data. */
        result = gx_validation_calc_crc((unsigned char *)gx_validation_frame_buffer, gx_validation_frame_size);
    }

    if(generate)
    {
        if(output_file)
        {
            gx_validation_write_one_frame(output_file);
        }

        if(output_checksum_file)
        {
            /* Write the frame number */
            fprintf(output_checksum_file, "\n\n"FRAME_ID"%d\n", gx_validation_frame_id);
            fprintf(output_checksum_file, FRAME_DATA"\n");

            /* Write checksum value. */
            fwrite((void *)&result, sizeof(result), sizeof(result), output_checksum_file);
        }
    }

    if(do_compare)
    {
        if(checksum)
        {
            /* Verify frame with checksum, */
            status = gx_validation_verify_one_frame((void *)&result, sizeof(result), gx_validation_frame_id);
        }
        else
        {
            /* Verify frame with canvas data. */
            status = gx_validation_verify_one_frame(gx_validation_frame_buffer,
                                                    gx_validation_frame_size,
                                                    gx_validation_frame_id);
        }

        if(status != GX_VALIDATION_SUCCESS)
        {
            if(total_failures == 0)
            {
                if (gx_validation_create_output_failures_file() == 0)
                {
                    gx_validation_write_one_frame(output_failures_file);
                }
            }
           
            total_failures++;
        }
    }

    gx_validation_frame_comment = NULL;

    TX_RESTORE
    total_frames++;

    /* If we are in the frame catpure mode, set up information for the next frame. */
    if((gx_validation_frame_capture_timeout >= tx_time_get()) &&
       (gx_validation_frame_capture_current != gx_validation_frame_capture_max_frame))
    {
        gx_validation_frame_id ++;
        
        gx_validation_frame_capture_current++;
            
        if(gx_validation_frame_capture_current < gx_validation_frame_capture_comments_max)
        {
            gx_validation_frame_comment = gx_validation_frame_capture_comments[gx_validation_frame_capture_current];
        }

        if(gx_validation_frame_capture_current != gx_validation_frame_capture_max_frame)
        {
            gx_validation_record_frame = 1;
        }
    }

    if(gx_validation_frame_capture_current == gx_validation_frame_capture_max_frame)
    {
        if(gx_validation_semaphore)
        {
            tx_semaphore_put(gx_validation_semaphore);
        }
    }
}

int gx_validation_validate_data_frame(char *golden_file, char *test_output)
{
    return(0);

}

void gx_validation_close_output_file(void)
{
    if(output_file)
    {
        fseek(output_file, 0, SEEK_SET);
        /* skip the first line, which is the file format info */

        fprintf(output_file, TOTAL_FRAMES"%d\n", total_frames);
        fclose(output_file);
    }

    if(output_checksum_file)
    {
        fseek(output_checksum_file, 0, SEEK_SET);
        /* skip the first line, which is the file format info */

        fprintf(output_checksum_file, TOTAL_FRAMES"%d\n", total_frames);
        fclose(output_checksum_file);

    }

    if(output_failures_file)
    {
       fseek(output_failures_file, 0, SEEK_SET);
        
       /* skip the first line, which is the file format info */
       fprintf(output_failures_file, TOTAL_FRAMES"%d\n", total_failures);
       fclose(output_failures_file);
    }
}

void gx_validation_end(void)
{
    GX_ENTER_CRITICAL
    gx_validation_close_output_file();

    if(gx_validation_frame_buffer)
    {
        free(gx_validation_frame_buffer);
        gx_validation_frame_buffer = NULL;
        gx_validation_frame_size = 0;
    }

    if(gx_validation_semaphore)
    {
        tx_semaphore_delete(gx_validation_semaphore);
        gx_validation_semaphore = NULL;
    }

    if(do_compare)
    {
        gx_validation_verify_end(total_frames);
    }
    GX_EXIT_CRITICAL
}

void gx_validation_watchdog_create(int tick)
{

}

void gx_validation_set_frame_comment(char *comment)
{
    gx_validation_frame_comment = comment;
}

void gx_validation_set_frame_id(int frame_id)
{
    gx_validation_frame_id = frame_id;
    gx_validation_record_frame = 1;

    /* Clear any capture operation started early on */
    if(gx_validation_frame_capture_timeout)
    {
        gx_validation_frame_capture_timeout = 0;
        gx_validation_frame_capture_max_frame = 0;
        gx_validation_frame_capture_comments = NULL;
        gx_validation_frame_capture_comments_max = 0;
        gx_validation_frame_capture_current = 0;
    }
}

void gx_validation_capture_frames(int start_id, int expected_frames, char **comments, int num_comments, int max_time)
{
    gx_validation_frame_capture_timeout = tx_time_get() + max_time;
    gx_validation_frame_id = start_id;
    gx_validation_frame_capture_max_frame = expected_frames;
    gx_validation_frame_capture_comments = comments;
    gx_validation_frame_capture_comments_max = num_comments;
    gx_validation_frame_capture_current = 0;

    if(num_comments && comments)
        gx_validation_frame_comment = comments[0];

    if(!gx_validation_semaphore)
    {
        gx_validation_semaphore = (TX_SEMAPHORE *)malloc(sizeof(TX_SEMAPHORE));
        tx_semaphore_create(gx_validation_semaphore, "capture_semaphore", 0);
    }
    gx_validation_record_frame = 1;
}

void gx_validation_capture_frames_wait()
{
    tx_semaphore_get(gx_validation_semaphore, 1000);
}

void gx_validation_current_frame_id_get(int *current_frame)
{
    *current_frame = gx_validation_frame_id;

    return;

}


void gx_validation_screen_refresh(void)
{
GX_EVENT my_event;
   
    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_type = GX_EVENT_REDRAW;
    gx_system_event_send(&my_event);
}


static TX_THREAD control_thread;
static ULONG     control_thread_stack[CONTROL_STACK_SIZE / sizeof(ULONG)];
static TX_THREAD timer_control_thread;
static ULONG     timer_control_thread_stack[CONTROL_STACK_SIZE / sizeof(ULONG)];

void timer_control_thread_entry()
{
TX_INTERRUPT_SAVE_AREA

    while(1)
    {
        tx_semaphore_get(&gx_validation_timer_semaphore, TX_WAIT_FOREVER);
        while(_gx_system_event_queue.tx_queue_enqueued != 0)
        {
            tx_thread_sleep(1);
        }

        TX_DISABLE
        _gx_system_timer_expiration(0);
        TX_RESTORE
  }
}

void gx_validation_control_thread_create(VOID (*func)(ULONG))
{
    tx_thread_create(&control_thread, "GUIX Validation Control Thread", func,
                     0, control_thread_stack, sizeof(control_thread_stack), 
                     CONTROL_THREAD_PRIORITY, CONTROL_THREAD_PRIORITY,
                     TX_NO_TIME_SLICE, TX_AUTO_START);

    if(!gx_validation_no_output)
    {
        tx_thread_create(&timer_control_thread, "GUIX Validation Timer Control Thread", timer_control_thread_entry,
                         0, timer_control_thread_stack, sizeof(timer_control_thread_stack), 
                         CONTROL_THREAD_PRIORITY, CONTROL_THREAD_PRIORITY,
                         TX_NO_TIME_SLICE, TX_AUTO_START);
    }
}


void gx_validation_write_palette(ULONG *palette, int size)
{
int i;

    if(generate && output_file)
    {
        GX_ENTER_CRITICAL
        fprintf(output_file, "\nPALETTE DATA SIZE: %d\n", size);
        fprintf(output_file, "PALETTE DATA:\n");
        for(i = 0; i < size; i++)
        {
            fprintf(output_file, "%lu ", palette[i]);
        }            
        fprintf(output_file, "\n");
        GX_EXIT_CRITICAL
    }
}

void gx_validation_system_timer_expiration(ULONG val)
{
    tx_semaphore_put(&gx_validation_timer_semaphore);
}

void gx_validation_print_test_result(int result)
{
char *test_padder = "................................................................................";

    if(result == TEST_SUCCESS)
    {
        printf("Guix Test:   %s%sSuccess!\n", test_parameter.test_name, test_padder + strlen(test_parameter.test_name));
    }
    else
    {
        printf("Guix Test:   %s%s.Failed!\n", test_parameter.test_name, test_padder + strlen(test_parameter.test_name));
    }
}

void gx_validation_extract_path(char *pathname, char *path, int *pathlen)
{
int index = strlen(pathname) - 1;

    while(index >= 0)
    {
        if(pathname[index] == '/')
        {
            memcpy(path, pathname, index+1);
            path[index + 1]='\0';
            break;
        }
        index--;
    }

   *pathlen = index + 1;
}

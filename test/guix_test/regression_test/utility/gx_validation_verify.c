#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gx_validation_strings.h"
#include "gx_validation_verify.h"

#ifdef __linux__
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#endif

extern void gx_validation_print_test_result(int result);

static char line[256];
static FILE *golden_file = NULL;
static int   golden_file_frame_size = 0;
static int   golden_file_total_frames = 0;
static int   num_mismatch = 0;

#ifdef __linux__
static pthread_t frame_read_thread;
static sem_t     frame_read_semaphore;
static sem_t     frame_verify_semaphore;
#endif

#define MAX_NUM_FRAMES 3

typedef struct frame_info_struct{
    char                     *frame_buffer;
    int                       frame_id;
    struct frame_info_struct *next;
}frame_info;

static frame_info  frame_list[MAX_NUM_FRAMES];
static frame_info *frame_list_head = NULL;
static frame_info *frame_list_tail = NULL;
static int         frame_list_read_count = 0;

static int read_line(FILE *file)
{
    char ch;
    int count = 0;

    memset(line, 0, sizeof(line));

    ch = getc(file);

    while((ch != '\n') && (ch != EOF))
    {
        if(count == sizeof(line))
            count = 0;
        line[count++] = ch;

        ch = getc(file);
    }
    
    if(ch == EOF)
    {
        return(-1);
    }

    line[count] = 0;

    return(count);

}

void get_frame_id(FILE *pFile, int *frame_id)
{
    *frame_id = -1;

    while(*frame_id == -1)
    {
        read_line(pFile);

        if(strncmp(line, FRAME_ID, strlen(FRAME_ID)) == 0)
            *frame_id = atoi(&line[strlen(FRAME_ID)]);

    }
    return;


}
int read_file_header(FILE *pFile, int *file_format, int *total_frame, char **color_format, int *bits_per_pixel, int *width, int *height)
{
int len;

    *file_format = *total_frame = *bits_per_pixel = *width = *height = 0;
    while(!(*total_frame) || !(*bits_per_pixel) || !(*width) || !(*height))
    {
        len = read_line(pFile);
    
        if(len < 0)
        {
            *total_frame = 0;
            return (-1);
        }
        
        if(strncmp(line, FILE_FORMAT, strlen(FILE_FORMAT)) == 0)
            *file_format = atoi(&line[strlen(FILE_FORMAT)]);

        else if(strncmp(line, TOTAL_FRAMES, strlen(TOTAL_FRAMES)) == 0)
            *total_frame = atoi(&line[strlen(TOTAL_FRAMES)]);

        else if(strncmp(line, COLOR_DEPTH, strlen(COLOR_DEPTH)) == 0)
        {

            if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_24XRGB, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_24XRGB;
                }
                *bits_per_pixel = 32;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_565RGB, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_565RGB;
                }
                *bits_per_pixel = 16;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_565BGR, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_565BGR;
                }
                *bits_per_pixel = 16;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_1555XRGB, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_1555XRGB;
                }
                *bits_per_pixel = 16;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_4444ARGB, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_4444ARGB;
                }
                *bits_per_pixel = 16;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_8BPP, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_8BPP;
                }
                *bits_per_pixel = 8;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_332RGB, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if(color_format)
                {
                    *color_format = COLOR_FORMAT_332RGB;
                }
                *bits_per_pixel = 8;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_4BPP, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                     *color_format = COLOR_FORMAT_4BPP;
                }
                *bits_per_pixel = 4;
            }
            else if (strncmp(line + strlen(COLOR_DEPTH), COLOR_FORMAT_1BPP, strlen(line) - strlen(COLOR_DEPTH)) == 0)
            {
                if (color_format)
                {
                    *color_format = COLOR_FORMAT_1BPP;
                }
                *bits_per_pixel = 1;
            }
        }

        else if(strncmp(line, WIDTH, strlen(WIDTH)) == 0)
            *width = atoi(&line[strlen(WIDTH)]);
        
        else if(strncmp(line, HEIGHT, strlen(HEIGHT)) == 0)
            *height = atoi(&line[strlen(HEIGHT)]);

    }

    if(*file_format > FILE_FORMAT_VERSION_CURRENT)
    {
        printf("Incorrect file format: %d, current version is %d\n", *file_format, FILE_FORMAT_VERSION_CURRENT);
        return(-1);
    }

    return(0);
}

void get_frame_comment(FILE *fp, char *buffer, int buf_size)
{
int copy_size = 0;
int end = 0;
    memset(buffer, 0, buf_size);

    do
    {

        read_line(fp);
        
        if(strncmp(line, FRAME_COMMENT, strlen(FRAME_COMMENT)) == 0)
        {
            if((int)(strlen(line) - strlen(FRAME_COMMENT)) <= (buf_size - 1))
                copy_size = strlen(line) - strlen(FRAME_COMMENT);
            else
                copy_size = buf_size - 1;
            memcpy(buffer, &line[strlen(FRAME_COMMENT)], copy_size);

        }
        if(strncmp(line, FRAME_DATA, strlen(FRAME_DATA)) == 0)
            end = 1;
    }while(!end);

}

void find_frame_data(FILE *fp)
{


    do
    {
        read_line(fp);
        
        if(strncmp(line, FRAME_DATA, strlen(FRAME_DATA)) == 0)
            return;
    }while(1);
        
}

void cleanup(FILE *fp, char *buffer)
{
    if(fp)
        fclose(fp);

    if(buffer)
        free(buffer);
}

#ifdef __linux__
static void *frame_read(void *args)
{
int result;

    while(1)
    {
        sem_wait(&frame_read_semaphore);

        get_frame_id(golden_file, &frame_list_tail->frame_id);

        find_frame_data(golden_file);

        result = fread(frame_list_tail->frame_buffer, 1, golden_file_frame_size, golden_file);
        if(result != golden_file_frame_size)
        {
            gx_validation_print_test_result(TEST_FAIL);
            printf("Reading error\n");
            exit(1);
        }

        frame_list_tail = frame_list_tail->next;
        sem_post(&frame_verify_semaphore);
        frame_list_read_count++;
        if(frame_list_read_count == golden_file_total_frames)
        {
            break;
        } 
    }

    return NULL;
}

int gx_validation_verify_start(char *test_name, char *golden_file_name, int checksum)
{
int color_format;
int color_depth;
int width;
int height;
int loop;

    /* Open golden file. */
    golden_file = fopen(golden_file_name, "rb");
    if(!golden_file)
    {
        gx_validation_print_test_result(TEST_FAIL);
        printf("Golden file is missing, unable to compare output\n");
        exit(1);
    }
    
    /* Read golden file header. */
    read_file_header(golden_file, &color_format, &golden_file_total_frames, NULL,
                     &color_depth, &width, &height);


    if(checksum)
    {
        golden_file_frame_size = sizeof(unsigned int);
    }
    else
    {
        golden_file_frame_size = (width * color_depth >> 3) * height;
    }

    frame_list_head = frame_list;
    frame_list_tail = frame_list;
    for(loop = 0; loop < MAX_NUM_FRAMES; loop++)
    {
        if(loop < MAX_NUM_FRAMES - 1)
        {
            frame_list[loop].next = &frame_list[loop + 1];
        }
        else
        {
            frame_list[loop].next = frame_list_head;
        }

        frame_list[loop].frame_buffer = (char*)malloc(golden_file_frame_size);

        if(frame_list[loop].frame_buffer == NULL)
        {
            gx_validation_print_test_result(TEST_FAIL);
            printf("Failed to allocate requested memory\n");
            exit(1);
        }
    }

    /* Create semaphores to synchornize between threads*/
    if((sem_init(&frame_read_semaphore, 0, MAX_NUM_FRAMES) != 0) ||
       (sem_init(&frame_verify_semaphore, 0, 0) != 0))
    {
         gx_validation_print_test_result(TEST_FAIL);
         printf("Semaphore initialization failed!");
         exit(1);
    }

    frame_list_read_count = 0;

    /* Create thread to read frame buffer. */
    if(pthread_create(&frame_read_thread, NULL, frame_read, NULL) != 0)
    {
        gx_validation_print_test_result(TEST_FAIL);
        printf("Thread creation failed!\n");
        exit(1);
    }

    return 0;
}

int gx_validation_verify_end(int total_frames)
{
int loop;

    /* Destroy semaphores. */
    sem_destroy(&frame_read_semaphore);
    sem_destroy(&frame_verify_semaphore);

    if(golden_file)
    {
        fclose(golden_file);
        golden_file = NULL;
    }

    for(loop = 0; loop < MAX_NUM_FRAMES; loop++)
    {
        free(frame_list[loop].frame_buffer);
        frame_list[loop].frame_buffer = NULL;
    }

    if(total_frames != golden_file_total_frames)
    {
        /* frame number dismatch. */
        if(num_mismatch == 0)
        {
            gx_validation_print_test_result(TEST_FAIL);
        }

        printf("Total frame number is not match\n");
        exit(1);
    }

    if(num_mismatch)
    {
        printf("Total %d, Failed %d\n", total_frames, num_mismatch);
        exit(1);
    }

    gx_validation_print_test_result(TEST_SUCCESS);
    return(GX_VALIDATION_SUCCESS);
}

int gx_validation_verify_one_frame(void *frame_data, int frame_data_size, int frame_id)
{
int result = -1;
int status = GX_VALIDATION_SUCCESS;

    while(1)
    {
        result = sem_wait(&frame_verify_semaphore);

        if(result == 0)
        {
            /* Success. */
            break;
        }
        else
        {
            if(errno == EINTR)
            {
                /* The call was interrupted by a signal handler, continue wait. */
                continue;
            }
            else
            {
                /* Unexpected error. */
                printf("sem_wait error!\n");
                exit(1);
            }
        }
    }

    if(frame_id != frame_list_head->frame_id)
    {
        /* frame id dismatch. */
        if(num_mismatch == 0)
        {
            gx_validation_print_test_result(TEST_FAIL);
        }

        printf("golden_file_frame_id = %d, test_frame_id = %d.\n", frame_list_head->frame_id, frame_id);
        exit(1);
    }
 
    if(memcmp(frame_data, frame_list_head->frame_buffer, frame_data_size))
    {
        /* frame data dismatch. */
        if(num_mismatch == 0)
        {
            gx_validation_print_test_result(TEST_FAIL);
        }

        num_mismatch++;
        printf("Frame %d is different.\n", frame_id);
        status = GX_VALIDATION_DISMATCH;
    }
    frame_list_head = frame_list_head->next;
    sem_post(&frame_read_semaphore);

    return(status);
}
#endif // __linux__

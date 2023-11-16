
#include "gx_validation_strings.h"

typedef struct TEST_PARAM_S 
{

    char *test_name;          /* Must be set */


    /* The following parameters defines the screen area to capture.
       If 0, capture the whole screen. */
       
    int x_start;   
    int y_start;
    int x_end;
    int y_end;

} TEST_PARAM;

/* Define test exit codes */
#define VALIDATION_SUCCESS 0
#define VALIDATION_NA      233

#define CONTROL_STACK_SIZE 4096

#define CONTROL_THREAD_PRIORITY (GX_SYSTEM_THREAD_PRIORITY + 1)

VOID gx_validation_application_define(void *);

/* gx_val_start_test is called in main() to parse argc/argc, and set up file names. */

void gx_validation_setup(int argc, char **argv);

/* gx_validation_create_output_file is called from the validation driver toggle routine 
   to create the output file, and to record the color depth, width, height */
int  gx_validation_create_output_file(char *color_depth, int width, int height);

/* gx_validation_create_frame_buffer is called from the validation driver toggle routine
   to create a output frame buffer. */
int  gx_validation_create_frame_buffer(int frame_size);

/* gx_validation_write_frame_write is called from the validation driver toggle routine */
void gx_validation_write_frame_buffer(void);

/* gx_validation_validate_data_frame is called from the frame work to validate the output.  
   It also indicates the beginning of a frame. */
int  gx_validation_validate_data_frame(char *golden_file, char *test_output);

/* gx_validation_close_output_file is called from the framework to close the output file */
void gx_validation_close_output_file(void);

void gx_validation_watchdog_create(int);

void gx_validation_end(void);

void gx_validation_set_frame_id(int frame_id);

void gx_validation_control_thread_create(void (*func)(ULONG));

void gx_validation_screen_refresh(void);

void gx_validation_set_frame_comment(char *comment);

void gx_validation_capture_frames(int, int, char**, int, int);

void gx_validation_capture_frames_wait();

void gx_validation_current_frame_id_get(int *current_frame);

void gx_validation_write_palette(ULONG *palette, int size);

void gx_validation_system_timer_expiration(ULONG val);

void gx_validation_print_test_result(int result);

void gx_validation_extract_path(char *pathname, char *path, int *pathlen);

extern char *gx_validation_frame_buffer;
extern int gx_validation_frame_id;
extern int gx_validation_record_frame;
extern TEST_PARAM test_parameter;

#define EXPECT_EQ(expected, actual) \
    if((expected) != (actual))          \
    {                               \
        printf("\nERROR! File: %s Line: %d\n", __FILE__, __LINE__); \
        printf("Expected: 0x%x, (%d) Got: 0x%x (%d)\n", (UINT)(expected), (INT)(expected), (UINT)(actual), (INT)(actual)); \
        failed_tests++; \
    }

#define PRINT_ERROR(msg) \
    printf("%s\nERROR! File: %s Line: %d\n", msg, __FILE__, __LINE__);

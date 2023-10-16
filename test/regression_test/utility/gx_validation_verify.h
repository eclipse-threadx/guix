#include "stdio.h"

#define GX_VALIDATION_SUCCESS              0
#define GX_VALIDATION_DISMATCH             1
#define GX_VALIDATION_GOLDEN_FILE_MISSING  2
#define GX_VALIDATION_OUTPUT_FILE_MISSING  3
#define GX_VALIDATION_FRAME_NUM_DISMATCH   4
#define GX_VALIDATION_FRAME_ID_DISMATCH    5
#define GX_VALIDATION_WIDTH_DISMATCH       6
#define GX_VALIDATION_HEIGHT_DISMATCH      7
#define GX_VALIDATION_COLOR_DEPTH_DISMATCH 8
#define GX_VALIDATION_MEMORY_REQUEST_FAIL  9
#define GX_VALIDATION_READING_ERROR        10

int  read_file_header(FILE *pFile, int *file_format, int *total_frame, char **color_format, int *bits_per_pixel, int *width, int *height);
void cleanup(FILE *fp, char *buffer);
void find_frame_data(FILE *fp);
void get_next_frame_id(FILE *pFile, int *frame_id);

int gx_validation_verify_output(char *golden_file_name, char *output_file_name, int *total_frames, int *mismatch_frames);

int gx_validation_verify_start(char *test_name, char *golden_file, int checksum);
int gx_validation_verify_end(int total_frames);
int gx_validation_verify_one_frame(void *frame_data, int frame_data_size, int frame_id);

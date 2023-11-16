#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gx_validation_compare_file_data.h>

#define GX_SUCCESS 0
#define GX_FAILURE 1

static void cleanup_file(FILE *file, char * buffer)
{
    if(file)
        fclose(file);

    if(buffer)
        free(buffer);
}

int  gx_validation_compare_file_data(char *golden_file_name, char *output_file_name)
{
    FILE *golden_file, *output_file;
    int size1, size2;
    char *output_frame_buffer=NULL, *golden_frame_buffer=NULL;

    golden_file = fopen(golden_file_name, "rb");
    output_file = fopen(output_file_name, "rb");
    if(!golden_file)
    {
        printf("Cannot open file: %s\n", golden_file_name);
        return(GX_FAILURE);
    }
    
    if(!output_file)
    {
        printf("Cannot open file: %s\n", output_file_name);
        fclose(golden_file);
        return(GX_FAILURE);
    }
    
    fseek(golden_file, 0, SEEK_END);  
    size1 = ftell(golden_file);
    
    
    fseek(output_file, 0, SEEK_END);  
    size2 = ftell(output_file);
    
    if(size1 != size2)
    {
        cleanup_file(golden_file, golden_frame_buffer );
        cleanup_file(output_file, output_frame_buffer );
        return GX_FAILURE;
    }
    
    output_frame_buffer = (char*)malloc(sizeof(char)*size1);
    if(output_frame_buffer == NULL)
    {
        cleanup_file(output_file, output_frame_buffer );
        return GX_FAILURE;
    }
    
    golden_frame_buffer = (char*)malloc(sizeof(char)*size2);

    if(golden_frame_buffer == NULL)
    {
        cleanup_file(golden_file, golden_frame_buffer );
        return GX_FAILURE;
    }      
    
    rewind(golden_file);
    rewind(output_file);    
    
    size2 = fread(golden_frame_buffer, 1,size1,golden_file);
    if(size1 != size2)
    {
        cleanup_file(golden_file, golden_frame_buffer );
        cleanup_file(output_file, output_frame_buffer );
        return GX_FAILURE;
    }
    
    size2 = fread(output_frame_buffer, 1,size1,output_file);
    if(size1 != size2)
    {
        cleanup_file(golden_file, golden_frame_buffer );
        cleanup_file(output_file, output_frame_buffer );
        return GX_FAILURE;
    }
    
    if(memcmp(output_frame_buffer, golden_frame_buffer, size1))
    {
        cleanup_file(golden_file, golden_frame_buffer );
        cleanup_file(output_file, output_frame_buffer );
        return GX_FAILURE;
    }
    else
    {
        cleanup_file(golden_file, golden_frame_buffer );
        cleanup_file(output_file, output_frame_buffer );
        return GX_SUCCESS;
    }    
}

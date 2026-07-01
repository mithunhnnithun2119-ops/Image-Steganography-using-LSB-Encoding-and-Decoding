#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include <string.h>
#include "types.h"

typedef struct Decode_Info
{
    /* Source Image info */
    char *src_image_fname;// Name of the stego image file
    FILE *fptr_src_image;  // File pointer for source image

    /* Secret File Info */
    char *output_fname; //Name of output (decoded) file    
    FILE *fptr_output; // File pointer for output file    
    char extn_secret_file[10];   // increased size
    long size_output_file;   // Size of decoded secret file
    long size_extn_file;  // Size of extension


} DecodeInfo;

/* Function Prototypes */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);// Validate command line arguments for decoding
Status do_decoding(DecodeInfo *decInfo);// Main decoding function
Status open_dfiles(DecodeInfo *decInfo);// Open required files for decoding
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);// Decode and verify magic string from image
Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo);// Decode size of secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo);// Decode secret file extension
Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo);// Decode secret file size
Status decode_secret_file_data(DecodeInfo *decInfo);// Decode secret file data from image
Status decode_byte_to_lsb(char buffer[], char *);// Convert LSB bits to a single byte
int decode_size_to_lsb(char *buffer);// Convert LSB bits to integer size

#endif


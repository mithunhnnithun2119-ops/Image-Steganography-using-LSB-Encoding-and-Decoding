#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "decode.h"

#define MAGIC_STRING "#*"// Magic string used to verify hidden data

/* Validate arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)// 
{
    if (argv[2] && strstr(argv[2], ".bmp") != NULL)// Check whether input file is BMP
    {
        decInfo->src_image_fname = argv[2];// Store image filename
        return e_success; // Validation success
    }
    return e_failure;// Validation failed
}

/* Open input file */
Status open_dfiles(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");// Open stego image in read binary mode

    if (decInfo->fptr_src_image == NULL)// Check if file opened successfully
    {
        perror("fopen");// Print system error
        return e_failure;// Return failure
    }

    return e_success;// File opened successfully
}

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_dfiles(decInfo) == e_failure)// Open required files
        return e_failure;

    /* Skip BMP header */
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    if (decode_magic_string(MAGIC_STRING, decInfo) == e_failure)// Verify magic string
        return e_failure;

    int extn_size;

    if (decode_secret_file_extn_size(&extn_size, decInfo) == e_failure)// Decode extension size
        return e_failure;

    decInfo->size_extn_file = extn_size;// Store extension size

    if (decode_secret_file_extn(decInfo) == e_failure)// Decode extension
        return e_failure;

    long file_size;

    if (decode_secret_file_size(&file_size, decInfo) == e_failure)// Decode secret file size
        return e_failure;

    decInfo->size_output_file = file_size;// Store output file size

    /* Create output file name */
    char output_name[50] = "decoded";
    strcat(output_name, decInfo->extn_secret_file); // Append extension

    decInfo->output_fname = strdup(output_name);// Allocate and store name

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");// Open output file in write binary mode

    if (decInfo->fptr_output == NULL)// Check output file opened or not
        return e_failure;

    if (decode_secret_file_data(decInfo) == e_failure)
        return e_failure;

    fclose(decInfo->fptr_src_image);// Close files
    fclose(decInfo->fptr_output);

    printf("Decoding Successful\n");// Success message
    return e_success;
}

/* Decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    char buffer[8];// Buffer to hold 8 bytes
    char ch;// Decoded character

    for (int i = 0; i < strlen(magic_string); i++) // Loop through each character of magic string
    {
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)// Read 8 bytes from image
            return e_failure;

        decode_byte_to_lsb(buffer, &ch);// Decode one byte from LSB

        if (ch != magic_string[i])// Compare with expected magic string
            return e_failure;
    }

    return e_success; // Magic string matched
}

/* Decode 1 byte from LSB */
Status decode_byte_to_lsb(char *buffer, char *c)
{
    *c = 0;// Initialize output character

    for (int i = 0; i < 8; i++)// Extract LSB from each byte
    {
        *c = (*c << 1) | (buffer[i] & 1);// Shift and add LSB
    }

    return e_success;
}

/* Decode extension size */
Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo)
{
    char buffer[32]; // Buffer for 32 bits

    if (fread(buffer, 1, 32, decInfo->fptr_src_image) != 32)// Read 32 bytes from image
        return e_failure;

    *size = decode_size_to_lsb(buffer);// Convert LSB to integer

    return e_success;
}

/* Decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];// Buffer for one byte
    char ch;// Decoded character

    for (int i = 0; i < decInfo->size_extn_file; i++)// Loop through extension size
    {
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)// Read 8 bytes
            return e_failure;

        decode_byte_to_lsb(buffer, &ch);// Decode one character

        decInfo->extn_secret_file[i] = ch;// Store extension character
    }

    decInfo->extn_secret_file[decInfo->size_extn_file] = '\0';// Null terminate extension string

    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(long *file_size, DecodeInfo *decInfo)
{
    char buffer[32];// Buffer for 32 bits

    if (fread(buffer, 1, 32, decInfo->fptr_src_image) != 32)
        return e_failure;

    *file_size = decode_size_to_lsb(buffer);// Convert to size

    return e_success;
}

/* Decode file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];// Buffer for one byte
    char ch;// Decoded character

    for (long i = 0; i < decInfo->size_output_file; i++)// Loop through secret file size
    {
        if (fread(buffer, 1, 8, decInfo->fptr_src_image) != 8)// Read 8 bytes
            return e_failure;

        decode_byte_to_lsb(buffer, &ch);// Decode one character

        fwrite(&ch, 1, 1, decInfo->fptr_output);// Write decoded byte to output file
    }

    return e_success;
}

/* Decode 32-bit size */
int decode_size_to_lsb(char *buffer)
{
    int size = 0;// Initialize size

    for (int i = 0; i < 32; i++)// Extract 32 bits from LSB
    {
        size = (size << 1) | (buffer[i] & 1);// Shift and add bit
    }

    return size;// Return decoded size

}




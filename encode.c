#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"



/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
 
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}


/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
   
    //step 1 = check source file having .bmp extension or not
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
       // return e_success;
    }

    else
    {   
        printf("ERROR: Source file should be of .bmp\n");
        return e_failure;
    }

    //step 2 = check secret file having extn or not
    if(strstr(argv[3],".txt") != NULL)
    {
        encInfo ->secret_fname = argv[3];
    }

    else
    {   
        printf("ERROR: SOURCE FILE NOT  .bmp\n");
        return e_failure;
    }

    //step 3 = option file passed or not
      if (argv[4] != NULL)
    {
        if (strstr(argv[4], ".bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("ERROR: Output file must be .bmp\n");
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    return e_success;
}
    //step1 -> check source file name having .bmp present or not
            // no -> return e_failure
            // yes -> store source file name into encInfo->src_image_fname
    //step2 -> check secret file having extn or not
            // no -> return e_failure
            // yes -> store secret file name into encInfo->src_image_fname
    // step3 -> check optional file is passed or not
            // yes -> check the file having .bmp or not
                    // no -> return e_failure
                    //yes -> store the file name into encInfo->stego_image_fname
     // no -> store default name to encInfo->stego_image_fname = "stego.bmp";
    //step4 -> return e_success


Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* Check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity =
        get_image_size_for_bmp(encInfo->fptr_src_image);// Get total image capacity in bytes

    encInfo->size_secret_file =
        get_file_size(encInfo->fptr_secret); // Get size of secret file

    uint extn_len = strlen(strstr(encInfo->secret_fname, "."));// Get extension length (e.g., .txt)
     // Calculate total required bytes for encoding
    uint required =
        (strlen(MAGIC_STRING) * 8) +
        32 +
        (extn_len * 8) +
        32 +
        (encInfo->size_secret_file * 8);
        printf("check Image capacity success: %u bytes\n", encInfo->image_capacity);// Print image capacity

    if (encInfo->image_capacity > required) // Check if image has enough capacity
    {
        

        return e_success;
    }

    printf("ERROR: Image capacity is not sufficient\n"); // Error if capacity is insufficient
    return e_failure;
}

/* Copy BMP header */
Status copy_bmp_header(FILE *src, FILE *dest)
{
    unsigned char buffer[54];// BMP header size

    rewind(src);// Move to start of source image
    fread(buffer, 1, 54, src);// Read header
    fwrite(buffer, 1, 54, dest);// Write header to stego image
    printf("BMP header copied successfully\n");

    return e_success;
}

/* Encode magic string */
Status encode_magic_string(const char *magic_string,
                           EncodeInfo *encInfo)
{
    unsigned char buffer[8];// buffer for 8 bytes
    for (int i = 0; magic_string[i] != '\0'; i++)// Loop through each character of magic string
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);// read image bytes
        encode_byte_to_lsb(magic_string[i], (char *)buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);// write to output
    }
    printf("Magic string encoded successfully\n");
    return e_success;
}

/* Encode extension size */
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    unsigned char buffer[32];// buffer for 32 bits

    fread(buffer, 1, 32, encInfo->fptr_src_image);// read image data
    encode_size_to_lsb(size, (char *)buffer);// encode extension size
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
printf("Encoding  secret file extension size : successfully \n");// write output
    return e_success;
}

/* Encode extension */
Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{
    unsigned char buffer[8];
    char *extn = strstr(file_extn, ".");

    for (int i = 0; extn[i] != '\0'; i++)// Encode each extension character
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);// read image bytes
        encode_byte_to_lsb(extn[i], (char *)buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image); // write output
    }
printf("Encoding secret file extension : Done \n");
    return e_success;
    
}

/* Encode secret file size */
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    unsigned char buffer[32];//32 bit buffer

    fread(buffer, 1, 32, encInfo->fptr_src_image);// read bytes
    encode_size_to_lsb(file_size, (char *)buffer);// encode size
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
printf("Encoding  secret file size : successfully done \n");// all condition is true printf the done messge
    return e_success;
}

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    unsigned char buffer[8];
    int ch;
    rewind(encInfo->fptr_secret);// start from beginning char

    while ((ch = fgetc(encInfo->fptr_secret)) != EOF) // read each char by chr
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);// read the image bytes
        encode_byte_to_lsb((char)ch, (char *)buffer);// encode data
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
 printf("Secret file data encoded successfully\n");// print the success message
    return e_success;
}

/* Copy remaining image data */
Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    int ch;
    while ((ch = fgetc(src)) != EOF)//copy the remaining the till end
    {
        fputc(ch, dest);
    }
    printf("Copying remaining data : successfully done \n");
    return e_success;
}

/* Encode one byte into LSB */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] &= 0xFE;// clear lsb
        image_buffer[i] |= ((data >> (7 - i)) & 1);// insert bit
    }
    return e_success;
}

/* Encode size into LSB */
Status encode_size_to_lsb(int size, char *image_buffer)
{
    for (int i = 0; i < 32; i++)// process 32 bits
    {
        image_buffer[i] &= 0xFE;//clear lsb side
        image_buffer[i] |= ((size >> (31 - i)) & 1);// insert the bit
    }
    return e_success;
}

/* Main encoding controller */
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) != e_success)// open the all file
    {
        printf("Failed to open files\n");
        return e_failure;
    }

    if (check_capacity(encInfo) != e_success)// check capacity
    {
        printf("Insufficient capacity\n");
        return e_failure;
    }

    copy_bmp_header(encInfo->fptr_src_image,
                    encInfo->fptr_stego_image);// calling the purpose of the copy the image header

    encode_magic_string(MAGIC_STRING, encInfo);// calling the function encode the magic string

    char *extn = strstr(encInfo->secret_fname, ".");// get the extension
    int extn_size = strlen(extn);// extension length

    encode_secret_file_extn_size(extn_size, encInfo);// calling the function encode the extn size
    encode_secret_file_extn(encInfo->secret_fname, encInfo);// calling the function encode extn
    encode_secret_file_size(encInfo->size_secret_file, encInfo);// calling the function encode the file size
    encode_secret_file_data(encInfo);// encode secret data

    if (copy_remaining_img_data(encInfo->fptr_src_image,
                                encInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }

    return e_success;
}

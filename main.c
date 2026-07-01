/*
NAME: MITHUN HN
DATE : 17/02/2026
Description : Steganography
*/


#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h" 
#include "types.h"
#include"common.h"
OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    //step1 -> check_operation_type(argv[1])

    if(argc < 3)//check the number of commentline argument pass
    {
        printf("Error : not enough CLA passed\n");// condition is fail print this statment
        return 1;
    }


    OperationType ret = check_operation_type(argv[1]);// Determine whether user wants encode or decode

    if(ret == e_encode)
    {
       EncodeInfo encInfo;// Structure to hold encoding info
       if(read_and_validate_encode_args(argv,&encInfo) == e_success)
       {
            
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding Successfull\n");// Encoding success message
            }

            else
            {
                    printf("Encoding Failed\n");// Encoding failure message
            }
       }

       else
       {
         printf("The number of arguments passed are invalid\n");// Invalid args
         return 1;
       }

    }

      if(ret == e_decode)
    {
       DecodeInfo decInfo; // Structure to hold decoding info
       if(read_and_validate_decode_args(argv,&decInfo) == e_success)// Validate decoding arguments
       {
            
            if(do_decoding(&decInfo) == e_success)// Perform decoding
            {
                printf("decoding Successfull\n");// Decoding failure message
            }

            else
            {
                    printf("decoding Failed\n");// decode fail
            }
       }

       else
       {
         printf("The number of arguments passed are invalid\n");// Invalid args
         return 1;
       }

    }
 //step2 -> check the return value == e_encode
//declare structure variable EncodeInfo encInfo
//--> read_and_validate_encode_args(pass command line arg, &encInfo) == e_success or e_failure
// e_failure -> print error msg and stop the program
// e_success -> next step.
// call do_encoding(&encInfo);
//e_failure -> print error msg and stop the program
//step3 -> return value == e_decode
//step3 -> return value == e_unsupported
// --> print invalid arg
// -e -> encode
// -d  -> decode
}

OperationType check_operation_type(char *symbol)
{
    if(strcmp(symbol,"-e") == 0 ) //check first argument 
    {
        return e_encode; //if it matches return e_encode to know it is encoding
    }

    else if (strcmp(symbol,"-d") == 0) // check first argument
    {
        return e_decode;    //if it matches return e_decode to know it is decoding
    }

    else
    {
            return e_unsupported;
    }
//step1 -> check it is -e or -d 
// if it is -e return e_encode
// else if it is -d rteun e_decode
// else return e_unsuported
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[] , DecodeInfo *decInfo)
{
    /*  Check the source file is .bmp or not , if .bmp
        then store the filename to pointer (encoded_image_fname ,in decocde.h) 
    
        check next is  NULL or not, if Null
        then store the new output filename to pointer
        if not NULL store the filename (what user as passed ) to the pointer and dont give extension
    */
   
   //for encoded file
    if(argv[2] == NULL)
    {
        printf("Error : Unable to perform Operations , Arguments should be > 3.\n");
        printf("For encoding arguments should be :\n");
        printf("./a.out -e <sourcefile.bmp> <secretfile.txt> <outputfile.bmp> (output file is optional).\n\n");
        printf("For decoding arguments should be :\n");
        printf("./a.out -d <sourcefile.bmp> <outputfile.txt> (output file is optional).\n");
        return e_failure ;
    }
    if (strstr(argv[2],".bmp") != NULL)
    {
        decInfo-> encoded_img_fname = argv[2];
        //printf("%s\n",decInfo-> encoded_img_fname);
    }
    else
    {
        printf("Decoding started.\n");
        printf("Checking arguments is started.\n");
        printf("Error :: Difficulty in checking source file extension in .bmp.\n");
        printf("Difficulty in checking file extensions.\n\n");
        return e_failure ;

    }
    //for output file
    
    if (argv[3] == NULL)
    {
        strcpy(decInfo->output_secret_fname ,"output");
        //printf("%s\n",decInfo->output_secret_fname);
        printf("Decoding started.\n");
        printf("Checking arguments is started.\n");
        printf("Arguments verified successfully.\n");
        return e_success ;
    }
    else if (argv[3] != NULL)
    {
        strtok(argv[3],".");
        strcpy(decInfo->output_secret_fname ,argv[3]);
        //printf("%s\n",decInfo->output_secret_fname);
        printf("Decoding started.\n");
        printf("Checking arguments is started.\n");
        printf("Arguments verified successfully.\n");
        return e_success ;
    }
    else
    {
        return e_failure ;
    }
}
Status open_files_for_decode(DecodeInfo *decInfo)
{
    //Open encoded image file
    decInfo->fptr_encoded_img = fopen(decInfo->encoded_img_fname , "r");
    // Do Error handling
    if (decInfo->fptr_encoded_img == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->encoded_img_fname);

    	return e_failure;
    }
    
    printf("Decoding file :: %s\n",decInfo->encoded_img_fname);
    // No failure return e_success
    return e_success;
}
char decode_int_from_lsb(char *buffer)
{
    //decode int from lsb and store in ch , return ch
    int i , get ;
    char ch = 0;
    for(i = 0 ; i < 32 ; i++)
    {
        get = (buffer[i] & 1 ) << (31 - i) ;
        ch = ch | get ;
    }
    return ch ;
}
char decode_byte_from_lsb(char *buffer)
{
    //decode byte from lsb and store in ch , and return ch
    int i , get;
    char ch = 0 ;
    for(i = 0 ; i < 8 ; i++)
    {
        get = (buffer[i] & 1 ) << (7 - i) ;
        ch = ch | get ;
    }
    return ch ;
}
Status decode_magic_string(int size , DecodeInfo *decInfo)
{
    int i ;
    char buffer[8];
    char magic[size + 1] ;
    fseek(decInfo->fptr_encoded_img ,54 ,SEEK_SET); //skip header files
    //printf("skipped header file %ld\n",ftell(decInfo->fptr_encoded_img));
    printf("Skipped Header file\n");

    //decode bytes from lsb and store into magic
    for(i = 0 ; i < size ; i++)
    {
        fread(buffer , 8 , 1 , decInfo->fptr_encoded_img);
        magic[i] = decode_byte_from_lsb(buffer);
    }
    magic[i] = '\0';
    //printf("magic string = %s\n",magic);
    //if decoded string is magic string , return e_success
    if(strcmp(magic , MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        printf("Error :: Magic String cannot be Decoded\n");
        printf("Error :: in Decoding\n");
        return e_failure ;
    }
    
}

Status decode_extn_size(DecodeInfo *decInfo)
{
    //decode the secret file extn size

    char buffer[32] ;
    fread(buffer , 32 , 1 , decInfo->fptr_encoded_img);
    decInfo->extn_size = decode_int_from_lsb(buffer);
    //printf("%d\n",decInfo->extn_size);
    return e_success ;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    //decode secret file extn

    int i ;
    char buffer[8];
    char extn[(decInfo->extn_size) + 1];
    for(i = 0 ; i < decInfo->extn_size ; i++)
    {
        fread(buffer , 8 , 1 , decInfo->fptr_encoded_img);
        extn[i] = decode_byte_from_lsb(buffer);
    }
    extn[i] = '\0';

    //printf("Extn = %s\n",extn);
    //concat extn to the output filename
    strcat(decInfo->output_secret_fname , extn);
    //printf("Output name = %s\n",decInfo -> output_secret_fname);
    return e_success ;
    
}
Status open_output_file(DecodeInfo *decInfo)
{
    // open output file
    decInfo->fptr_output_secret = fopen(decInfo->output_secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_secret_fname);
        return e_failure;
    }
    printf("Decoded data will store into :: %s\n",decInfo->output_secret_fname);
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    //decode secret file size
    char buffer[32] ;
    fread(buffer , 32 , 1 , decInfo->fptr_encoded_img);
    decInfo->sec_size = decode_int_from_lsb(buffer);
    //printf("%d\n",decInfo->sec_size);
    return e_success ;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    //decode secret file data
    int i ;
    char buffer[8];
    char sec_data[(decInfo->sec_size) + 1];
    for(i = 0 ; i < decInfo->sec_size ; i++)
    {
        fread(buffer , 8 , 1 , decInfo->fptr_encoded_img);
        sec_data[i] = decode_byte_from_lsb(buffer);
    }
    sec_data[i] = '\0';
    //copy data into structure member sec data
    strcpy(decInfo->sec_data , sec_data);
    //printf("secret data = %s\n",decInfo->sec_data);
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    printf("Opening files for decoding.\n");
    if(open_files_for_decode(decInfo) == e_success)
    {
        printf("Files Opened Successfully.\n");
        printf("Decoding Magic String.\n");
        if(decode_magic_string(strlen(MAGIC_STRING) ,decInfo ) == e_success)
        {
            printf("Magic String Decoded Successfully.\n");
            printf("Decoding Secret file Extension size.\n");
            if(decode_extn_size(decInfo) == e_success)
            {
                printf("Extension size Decoded Successfully.\n");
                printf("Decoding Secret file Extension.\n");
                if(decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("Secret file Extension Decoded Successfully.\n");
                    printf("Opening File to store the Secret Data (Output File).\n");
                    if(open_output_file(decInfo) == e_success)
                    {
                        printf("Output file opened Successfully.\n");
                        printf("Decoding Secret file size.\n");
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                            printf("Secret file size Decoded Successfully.\n");
                            printf("Decoding Secret Data.\n");
                            if(decode_secret_file_data(decInfo) == e_success)
                            {
                                printf("Secret Data Decoded Successfully.\n");
                                printf("Storing Data into Output file.\n");
                                fwrite(decInfo->sec_data, decInfo->sec_size , 1 ,decInfo->fptr_output_secret);
                                printf("Data Stored Successfully.\n");
                                return e_success;
                            }
                        }
                    }

                }
            }
        }
    }
       
}
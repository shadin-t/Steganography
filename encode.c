#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
OperationType check_operation_type(char *argv[])
{
    /*  Compare 2 strings using strcmp
        if '-e', return e_encode 
        if '-d', return e_decode
        if another option , return e_unsupported  
    */
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode ;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode ;
    }
    else
    {
        return e_unsupported ;
    }
}
Status read_and_validate_encode_args(char *argv[] , EncodeInfo *encInfo)
{
    /*  Check the source file is .bmp or not , if .bmp
        then store the filename to pointer (src_image_fname ,in enocde.h) 
        
        check the sec data file (.txt , .sh , .c) other than this print error , if true 
        then store the sec filename to pointer (secert_fname)

        check is it NULL or not, if true or (if user passed output filename then this condtion is false )
        then store the new output filename to pointer
        if false store the filename (what user as passed ) to the pointer
    */
   
   //for source file
    if (argv[2] == NULL)
    {
        printf("Error : Unable to perform Operations , Arguments should be > 3.\n");
        printf("For encoding arguments should be :\n/a.out -e <sourcefile.bmp> <secretfile.txt> <outputfile.bmp> (output file is optional).\n\n");
        printf("For decoding arguments should be :\n");
        printf("./a.out -d <sourcefile.bmp> <outputfile.txt> (output file is optional).\n");
        return e_failure ;

    }
    else if (strstr(argv[2],".bmp") != NULL)
    {
            encInfo->src_image_fname = argv[2];
            
            //printf("%s\n",encInfo->src_image_fname);
    }
    else
    {
        printf("Encoding started.\n");
        printf("Checking arguments is started.\n");
        printf("Error :: Difficulty in checking source file extension in .bmp.\n");
        printf("Difficulty in checking file extensions.\n\n");
        return e_failure;
    }
   //for secret file
    if(argv[3] == NULL)
    {           
        printf("Error : Unable to perform Operations , Arguments should be > 3.\n");
        printf("For encoding arguments should be :\n/a.out -e <sourcefile.bmp> <secretfile.txt> <outputfile.bmp> (output file is optional).\n\n");
        printf("For decoding arguments should be :\n");
        printf("./a.out -d <sourcefile.bmp> <outputfile.txt> (output file is optional).\n");
        return e_failure;
    }
    else if (strstr(argv[3],".txt") != NULL)
    {
        encInfo->secret_fname = argv[3];
        //printf("%s\n",encInfo->secret_fname);
        strcpy(encInfo->extn_secret_file, ".txt");
        //printf("%s\n",encInfo->extn_secret_file);
        printf("Encoding started.\n");
        printf("Checking arguments is started.\n");
    }
    else if(strstr(argv[3],".sh") != NULL)
    {
        encInfo->secret_fname = argv[3];
        //printf("%s\n",encInfo->secret_fname);
        strcpy(encInfo->extn_secret_file, ".sh");
        //printf("%s\n",encInfo->extn_secret_file);
        printf("Encoding started.\n");
        printf("Checking arguments is started.\n");
    }
    else if (strstr(argv[3],".c") != NULL)
    {
        encInfo->secret_fname = argv[3];
        //printf("%s\n",encInfo->secret_fname);
        strcpy(encInfo->extn_secret_file, ".c");
        //printf("%s\n",encInfo->extn_secret_file);
        printf("Encoding started.\n");
        printf("Checking arguments is started.\n");
    }
    else
    {
        printf("Encoding started.\n");
        printf("Checking arguments is started.\n");
        printf("Error :: Difficulty in checking source file extension in .txt or .sh or .c.\n");
        printf("Difficulty in checking file extensions.\n");
        return e_failure ;
    }
    //for output file
    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "stego.bmp";
        //printf("%s\n",encInfo->stego_image_fname);
        printf("Arguments Verified Successfully.\n");

        return e_success ;
    }
    else if (strstr(argv[4],".bmp") != NULL)
    {
        encInfo->stego_image_fname = argv[4];
        //printf("%s\n",encInfo->stego_image_fname);
        printf("Arguments Verified Successfully.\n");
        return e_success ;
    }
    else
    {
        printf("Error :: Provide Output File with .bmp extension.\n");
        printf("Difficulty in checking file extensions.\n\n");
        return e_failure ;
    }
}

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
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
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
    printf("Encoding file :: %s.\n",encInfo->stego_image_fname);
    // No failure return e_success
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    // get secret file size
    // return where the filepointer is (which is end of secret file, that is its size)
    fseek(fptr ,0 ,SEEK_END);
    return ftell(fptr);
    
}

Status check_capacity(EncodeInfo *encInfo)
{
    //get source image size
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    //printf("Image capacity = %u\n",encInfo->image_capacity);

    //get secret file size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    //printf("size of secret file = %lu\n",encInfo->size_secret_file);

    //printf("len of extn of secfile = %ld\n",strlen(encInfo->extn_secret_file));
    //printf("magic string = %ld\n",strlen(MAGIC_STRING));
    
    //get total bytes to change in the output file
    uint total_bytes = (((strlen(MAGIC_STRING)) + sizeof(int) + strlen(encInfo->extn_secret_file) + sizeof(int) +  (encInfo->size_secret_file)) * 8) + 54 ;
    //printf("Total bytes = %u\n",total_bytes);

    /* if image capacity > total bytes = 366 ((2+4+4+4+25)*8 + 54) is true return e_success*/
    if(encInfo->image_capacity > total_bytes)
    {
        return e_success ;
    }
    else
    {
        return e_failure ;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*  make filepointer into 0 th position (rewind(src filepointer))
        from 0 to 54 copy from src to dest using,
        create a string , char buffer[54];
        fread(&buffer (dest), 54 , 1 ,src filepointer (src) );
        fwrite(buffer(src) , 54 , 1 , dest filepointer (dest));
    */
    char buffer[54];
    rewind(fptr_src_image);
    fread(&buffer , 54 , 1 ,fptr_src_image);
    fwrite(buffer , 54 , 1 ,fptr_dest_image);
    return e_success ;
    
}
Status encode_int_to_lsb(long data , char*image_buffer)
{
    /*  run a loop to change the lsb bit
        get msb bit of data
        clear lsb bit of image buffer
        set lsb bit of image buffer */
    int i , get  ;
    for(i = 31 ; i >= 0 ; i--)
    {
        //get msb bit
        get = (data & (1 << i)) >> i ; 
        image_buffer[31 - i] = image_buffer[31 - i] &(~1) ;
        image_buffer[31 - i] = image_buffer[31 - i] | get ;
    }
    return e_success ;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*  run a loop to change the lsb bit
        get msb bit of data
        clear lsb bit of image buffer
        set lsb bit of image buffer */
    int i , get  ;
    for(i = 7 ; i >= 0 ; i--)
    {
        //get msb bit
        get = (data & (1 << i)) >> i ; 
        image_buffer[7 - i] = image_buffer[7 - i] &(~1) ;
        image_buffer[7 - i] = image_buffer[7 - i] | get ;
    }
    return e_success ;
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /*  create a buffer of size 8
        run a loop until size times 
        fread the 8 bytes from src image to buffer
        then call encode_byte_to_lsb function to change the lsb bit of buffer 
        fwrite changed 8 bytes of buffer to output image file  */
    char buffer[8];
    for (int i = 0 ; i < size ; i++)
    {
        fread(&buffer, 8 , 1 , fptr_src_image);
        encode_byte_to_lsb(data[i], buffer);
        fwrite(buffer, 8 , 1 , fptr_stego_image);
    }
    return e_success ;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //call encode_data_to_image function
    if(encode_data_to_image(MAGIC_STRING , strlen(MAGIC_STRING) , encInfo->fptr_src_image , encInfo -> fptr_stego_image) == e_success)
    {
        return e_success ;
    }
}
Status encode_extn_size(long extn_size, EncodeInfo *encInfo)
{
    /*  read 32 bytes from src image to buffer
        call encode int to lsb function
        write 32 bytes of changed buffer to output image */
    char buffer[32];
    fread(&buffer , 32 , 1 , encInfo->fptr_src_image);
    encode_int_to_lsb(extn_size,buffer);
    fwrite(buffer , 32 , 1 , encInfo->fptr_stego_image);
    return e_success ;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    //call encode data to image , where sec file extn is encode to output image
    if(encode_data_to_image((char *)file_extn ,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image , encInfo->fptr_stego_image) == e_success)
    {
        return e_success;
    }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*  read 32 bytes from src image to buffer
        call encode int to lsb function
        write 32 bytes of changed buffer to output image */
    char buffer[32];
    fread(&buffer , 32 , 1 , encInfo->fptr_src_image);
    encode_int_to_lsb(file_size , buffer);
    fwrite(buffer , 32 , 1 , encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*  rewind filepointer to 0th byte
        read 25 bytes of data from secret file to new variable
        call data to image function , where the data is encoded to output image
    */
    char secret_data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fread(&secret_data , encInfo->size_secret_file , 1 , encInfo->fptr_secret);
    //printf("%s\n",secret_data);
    encode_data_to_image(secret_data, encInfo->size_secret_file , encInfo ->fptr_src_image , encInfo->fptr_stego_image);
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*  copy remaining bytes one by one until end of file
        read 1 byte from src image , if true
        write 1 byte to output image , if false end of file is reached */
    char buffer;
    while(fread(&buffer , 1 , 1 , fptr_src))
    {
        fwrite(&buffer , 1 , 1 , fptr_dest);
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    printf("Open files process Started.\n");
    if(open_files(encInfo) == e_success)
    {
        printf("Files Opened Successfully.\n");
        printf("Checking Capacity to Encode Message.\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("Capacity of source file is more than the Secret File.\n");
            printf("Header is Copying.\n");
            if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
            {
                printf("Header Copied Successfully.\n");
                printf("Encoding Magic String.\n");
                //printf("header file copied = %ld\n",ftell(encInfo->fptr_stego_image));
                if (encode_magic_string(MAGIC_STRING , encInfo) == e_success)
                {
                    printf("Magic String Encoded Successfully.\n");
                    printf("Encoding the File Extension Size.\n");
                    //printf("magic string copied = %ld\n",ftell(encInfo->fptr_stego_image));
                    if(encode_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                    {
                        printf("File Extension Size Encoded Successfully.\n");
                        printf("Encoding the File Extension.\n");
                        //printf("extn size copied = %ld\n",ftell(encInfo->fptr_stego_image));
                        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("File Extension Encoded Successfully(Ex : .txt).\n");
                            printf("Encoding Secret File Size (Data Size).\n");
                            //printf("extn file copied = %ld\n",ftell(encInfo->fptr_stego_image));
                            if(encode_secret_file_size(encInfo->size_secret_file , encInfo) == e_success)
                            {
                                printf("File Size Encoded Successfully.\n");
                                printf("Encoding Secret Data\n");
                                //printf("secret file size copied = %ld\n",ftell(encInfo->fptr_stego_image));
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Secret Data Encoded Successfully.\n");
                                    printf("Encoding Remaining Data.\n");
                                    //printf("secret file data copied = %ld\n",ftell(encInfo->fptr_stego_image));
                                    if(copy_remaining_img_data(encInfo->fptr_src_image , encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("Remaining Data Encoded Successfully.\n");
                                        return e_success ;
                                    }
                                    else
                                    {
                                        printf("Error : Remaining data copying\n");
                                    }
                                }
                                else
                                {
                                    printf("Error : Secret file data copying\n");
                                }
                            }
                            else
                            {
                                printf("Error : Secret file size copying\n");
                            }
                        }
                        else
                        {
                            printf("Error : Secret file extn copying\n");
                        }
                    }
                    else
                    {
                        printf("Error : Secret file extn size copying\n");
                    }
                }
                else
                {
                    printf("Error : Magic string copying\n");
                }
           }
           else
           {
                printf("Error : Header file copying\n");
           }
        }
        else
        {
            printf("Error : Image capacity\n");
        }
    }
    else
    {
        printf("Error : Opening files\n");
    }
}

#include "types.h"

typedef struct _DecodeInfo
{
    char *encoded_img_fname ;
    FILE *fptr_encoded_img ;

    char output_secret_fname[20] ;
    FILE *fptr_output_secret ; 

    int extn_size ;
    int sec_size ;
    char sec_data[25] ;
    
    
}DecodeInfo ;

/* Read and validate Deccode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_for_decode(DecodeInfo *decInfo);

Status open_output_file(DecodeInfo *decInfo);

//skip bmp header using fseek

/* Store Magic String */
Status decode_magic_string(int size , DecodeInfo *decInfo);

/* Decode extenstion size*/
Status decode_extn_size( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);


/* Decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *buffer);

char decode_int_from_lsb(char *buffer);

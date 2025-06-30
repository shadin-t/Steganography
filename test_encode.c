/*------Documentation------
    Name  : MOHAMMAD SHADIN
    Date  : 21-11-24
    Batch : ECEP 24021-C 
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    //if arguments is less than 1 , then error 
    if(argc > 1)
    {
        //check if -e is passed
        if(check_operation_type(argv) == e_encode)
        {
            
            EncodeInfo encInfo ;
            //read and validate arguments, if success do encoding
            if(read_and_validate_encode_args(argv ,&encInfo) == e_success)
            {
                
                if( do_encoding(&encInfo) == e_success)
                {
                    printf("\n----------ENCODING COMPLETED--------\n\n");
                }
            }
            
        }
        //check if -d is passed
        else if(check_operation_type(argv) == e_decode)
        {
            DecodeInfo decInfo ;
            //read and validate arguments , if success do decoding
            if(read_and_validate_decode_args(argv , &decInfo) == e_success)
            {
                if(do_decoding(&decInfo) == e_success)
                {
                    printf("\n-----------DECODING COMPLETED---------\n\n");
                }
            }
            
            
        }
        //if other than -e or -d is passed , then error
        else
        {
            printf("Error : Unable to proceed operation provide valid type (-e or -d).\n");
        }
        

    }
    //if only one argument is passed , then print this
    else
    {
        printf("Please pass the proper arguments\n");
    }
    
    return 0;
}

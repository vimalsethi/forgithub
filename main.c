#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define MAX_PROG_SIZE  128*1024   // 128K data max


int main(int argc, char* argv[])
{
    int error;
    error = 0;  // no error
    FILE* ipfile;

    FILE* opfile;
    //unsigned int addr;

    binData_t binData[MAX_PROG_SIZE];

    unsigned int x,y;
    unsigned int segment, i, j;
    char strdata[3];
    unsigned char data[8];
    unsigned int lastAddress = 0x00;
    unsigned int dataSize = 0x00;
    unsigned int initialAddress;
    unsigned char filedone;
    //unsigned char buffer[4];
    filedone = 0;

    union{
    unsigned int f;
    unsigned char d[4];
    }addr;

	char test[5]
	test[6] = 10;
	
    //char ipfilename[1];
    //char opfilename[1];
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
        return -1;
        exit(-1);
    }
    else
    {
        // We assume argv[1] is a filename to open
        //FILE *file = fopen( argv[1], "r" );
        x = strlen(argv[1]);
        if(x<128)
            {
                strncpy(ipfilename,argv[1],x);
                printf("\nInput file name is: %s\n",ipfilename);
                strncpy(opfilename,argv[1],x);
                for(i=0;i<x;i++){
                    if(opfilename[i] == '.')
                    {
                        strcpy(&opfilename[++i],"bin");
                        opfilename[i+4] = 0x00;
                        printf("\noutput file name is: %s\n",opfilename);
                        break;

                    }
                }


            }else{
            printf("\nFile name is too long. Use smaller name <128\n");
            printf(" Press enter to continue... \n");
            fflush(stdin);
            getchar();
            return 0;
            exit(0);



            }

    }


    strdata[2] = 0x00;  //terminate
    segment = 0;
    addr.f = 0x3f0000;



    memset(binData,0xFF,sizeof(binData));



    intelhex2bin(ipfilename,(binData_t *)(binData),(unsigned int)(MAX_PROG_SIZE),&lastAddress,&dataSize );

    //printf("\nLast Address: %08X    Size: %04X\n",lastAddress,dataSize);

    FILE *binFile;

    binFile = fopen(argv[1],"wb+");
    if(binFile == 0x00)
    {
        return -2;
    }

    initialAddress = 0x3F0000;
    j = lastAddress - initialAddress;
    j = j * 2;

    printf("J= %u",j);

    unsigned int sz;
    unsigned int k;
    k = 0;
    while(k < j)
    {
     if((j-k) > 1024)
        sz = 1024;
     else
        sz = j-k;


     for(i = 0; i<sz;i++,k++){

        buffer[i] = binData[k].buffer;
        //printf("SZ: %u  K: %u  Data:   %02X\n",sz,k,buffer[i]);
     }

      fwrite((void *)(buffer),1,sz,binFile);

    }

    fclose(binFile);


    return error;
exit(0);
}




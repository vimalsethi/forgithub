#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.h"


int intelhex2bin(char * hexfile, binData_t* binData, unsigned int bufferSize, unsigned int *lastAddress, unsigned int * dataSize)
{

   /* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
    char line[INI_MAX_LINE];
#else
    char* line;
#endif

#if !INI_USE_STACK
    line = (char*)malloc(INI_MAX_LINE);
    if (!line) {
        return -2;
    }
#endif

    FILE * file;
    char *startline;
    hexRecord_t record;
    unsigned int initialAddress = 0;
    unsigned char firstRecord = 0;

    int i =0;
    int j = 0;
    int lineno = 0;


   if(bufferSize == 0)
        return -1;

    file = fopen(hexfile, "r");
    if (!file)
        return -3;

    *dataSize = 0;
    *lastAddress = 0;

    while (fgets(line, INI_MAX_LINE, file) != NULL) {
        lineno++;

   printf("line Number: %u\n",lineno);
   startline = line;

   record.startInd = *startline++;  // start indicator ':'

   record.byteCount = convertToDecimal(startline,2);    // Byte count location 1,2
   startline = startline + 2;

   record.addr = convertToDecimal(startline,4); // address location 3,4,5,6
   startline = startline + 4;


   record.dataType = convertToDecimal(startline,2); // data type location 7,8
   startline = startline + 2;

   switch(record.dataType)
   {
   case END_OF_FILE:
        printf("\rEnd of Hex Records  Last Addr: %08X  DataSize: %04X\r",*lastAddress, *dataSize);
    break;

   case EXT_LIN_ADDR:

            record.extAddr = convertToDecimal(startline,4)*256*256; // Address location 9,10,11,12
            startline = startline + 4;
            record.addr = 0x00;
       break;

   case DATA_TYPE:

            record.finalAddr = record.extAddr + record.addr;
            if(firstRecord == 0)
            {
              initialAddress = record.finalAddr;
              firstRecord = 1;
            }

            j = 2 * (record.finalAddr - initialAddress);


           *dataSize = *dataSize + record.byteCount;
            if(*dataSize > bufferSize)
            {
                printf("\nError, Data Size is bigger than buffer Size\n");
                printf(" Press enter to continue... \n");
                fflush(stdin);
                getchar();
             }
             i = 1;
             //for(i=0;i<record.byteCount;i++,j++)
             //printf("byte count:  %u",record.byteCount);
             while(i<record.byteCount)
             {
                //printf("byte count:  %u\n",record.byteCount);
                binData[j].buffer = convertToDecimal(startline,2);
                binData[j].status = 1;
                startline += 2;
              //  printf("line no:  %u, Final Addr:  %08X    LAddr:  %u,   data:  %02X\n",lineno,record.finalAddr,i,binData[j].buffer);
                i++;
                j++;
                binData[j].buffer = convertToDecimal(startline,2);
                binData[j].status = 1;
                startline += 2;
               // printf("line no:  %u, Final Addr:  %08X    LAddr:  %u,   data:  %02X\n",lineno,record.finalAddr,i,binData[j].buffer);
                i++;
                j++;
                record.finalAddr++;
                if((record.finalAddr) > *lastAddress)
                *lastAddress = record.finalAddr;



              //  if(*lastAddress > 0x3FFFFF)
                  //  printf("Max Addr: %08X\n",*lastAddress);


              printf("Address: %08X   Data: %02X   LastAddress: %08X\n",(record.finalAddr + j),binData[j].buffer,*lastAddress);
             }

        break;

   default:
        break;

   }

    }
    return 0;

}


unsigned long convertToDecimal(char hex[],int length)
{
    char *hexString;
 //   int length = 0;
    const int base = 16; // Base of Hexadecimal Number
    unsigned long decimalNumber = 0;
    int i;
    // Find length of Hexadecimal Number

//    for (hexString = hex; *hexString != '\0'; hexString++)
//    {
//        length++;
//    }
    // Find Hexadecimal Number
    hexString = hex;
    for (i = 0; *hexString != '\0' && i < length; i++, hexString++)
    {
        // Compare *hexString with ASCII values
        if (*hexString >= 48 && *hexString <= 57)   // is *hexString Between 0-9
        {
            decimalNumber += (((int)(*hexString)) - 48) * pow(base, length - i - 1);
        }
        else if ((*hexString >= 65 && *hexString <= 70))   // is *hexString Between A-F
        {
            decimalNumber += (((int)(*hexString)) - 55) * pow(base, length - i - 1);
        }
        else if (*hexString >= 97 && *hexString <= 102)   // is *hexString Between a-f
        {
            decimalNumber += (((int)(*hexString)) - 87) * pow(base, length - i - 1);
        }
        else
        {
            printf(" Invalid Hexadecimal Number \n");

            printf(" Press enter to continue... \n");
            fflush(stdin);
            getchar();
            return 0;
            exit(0);
        }
    }
    return decimalNumber;
}



#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define INI_MAX_LINE 128


typedef struct HEXRECORD
   {
   unsigned char startInd;
   unsigned int byteCount;        // number of data bytes in this record
   unsigned int finalAddr;    // segment address in hex file
   unsigned int addr;           // record address in hex file
   unsigned int extAddr;

   unsigned char dataType;
   unsigned char data[128];      // max 128 bytes per record
   unsigned int cksm;
   }hexRecord_t;

typedef struct BIN_DATA
   {
   unsigned char buffer;
   unsigned char status;
   }binData_t;


    enum {
    DATA_TYPE = 0,
    END_OF_FILE = 1,
    EXT_SEG_ADDR = 2,
    EXT_LIN_ADDR = 4,
    };


/*************************************** function prototypes ******************/

unsigned long convertToDecimal(char hex[], int length);
int intelhex2bin(char * hexfile, binData_t* binData, unsigned int bufferSize, unsigned int * lastAddress, unsigned int * dataSize);

#endif // UTIL_H_INCLUDED

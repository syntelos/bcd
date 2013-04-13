/*
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "BCD.h"



bool test(float vectorInValue,
          BCDFormatSign vectorInFormatSign,
          unsigned int vectorInPrecision,
          float vectorInAccuracy,
          char* vectorOut)
{
    fprintf(stderr,"\ntest input value: %f, sign: %d, prec: %d, acc: %f\n",vectorInValue,vectorInFormatSign,vectorInPrecision,vectorInAccuracy);

    BCD* dst = BCDCreate(9,9);
    if (dst){

        if (0 == BCDSetFloat(dst,vectorInValue)){

            BCDDebugPrint(stderr,dst);

            char* string = BCDToString(dst,
                                       vectorInFormatSign,
                                       vectorInPrecision,
                                       vectorInAccuracy);
            if (string){

                if (0 == strcmp(string,vectorOut)){

                    fprintf(stderr,"test success for %f -> %s\n",vectorInValue,string);

                    free(string);
                    BCDDestroy(dst);
                    return true;
                }
                else {

                    fprintf(stderr,"test failure for %f -> %s\n",vectorInValue,string);

                    free(string);
                    BCDDestroy(dst);
                    return false;
                }
            }
            else {
                BCDDestroy(dst);
                fprintf(stderr,"test error from BCDToString\n");
                return false;
            }
        }
        else {
            BCDDestroy(dst);
            fprintf(stderr,"test error from BCDSetFloat\n");
            return false;
        }
    }
    else {
        fprintf(stderr,"test error from BCDCreate\n");
        return false;
    }
}

int main(int argc, char** argv){

    FILE* fin = fopen("test.in","r");
    if (fin){
        int rc;
        /*
         * Test vector file format
         * 
         * Space separated line of
         */
        float vectorInValue; 
        BCDFormatSign vectorInFormatSign; 
        unsigned int vectorInPrecision; 
        float vectorInAccuracy;
        char vectorOut[128];
        /*
         * Process test vectors 
         */
        unsigned int vector = 0, success = 0, failure = 0;

        while (EOF != (rc = fscanf(fin,"%f %u %u %f %128s\n",&vectorInValue,&vectorInFormatSign,&vectorInPrecision,&vectorInAccuracy,vectorOut))){
            if (5 == rc){
                vector += 1;
                if (test(vectorInValue,
                         vectorInFormatSign,
                         vectorInPrecision,
                         vectorInAccuracy,
                         vectorOut))
                {
                    success += 1;
                }
                else {

                    failure += 1;
                }
            }
            else {
                fscanf(fin,"%128s\n",vectorOut);
            }
        }

        return failure;
    }
    else {
        fprintf(stderr,"%s: error: opening test vector 'test.in'\n",argv[0]);
        return 1;
    }
}

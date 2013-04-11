/*
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BCD.h"


int test_copy(){

    BCD* src = BCDCreate(9,7);

    BCD* dst = BCDCreate(9,7);

    unsigned int rc = BCDCopy(dst,src);

    BCDDestroy(src);

    BCDDestroy(dst);

    return rc;
}
int test_int(int value){

    BCD* dst = BCDCreate(9,0);
    if (dst){
        if (0 == BCDSetWord(dst,value)){

            char* string = BCDToString(dst,BCDFormatSignOpt,3,BCDFormatSignalAny,0.0);
            if (string){
                printf("%d -> %s\n",value,string);

                free(string);
                BCDDestroy(dst);
                return 0;
                
            }
            BCDDestroy(dst);
            return 1;
        }
        else {
            BCDDestroy(dst);
            return 1;
        }
    }
    else
        return 1;
}
int test_float(float value){

    BCD* dst = BCDCreate(9,9);
    if (dst){

        if (0 == BCDSetFloat(dst,value)){

            BCDDebugPrint(stderr,dst);

            char* string = BCDToString(dst,BCDFormatSignOpt,3,BCDFormatSignalFloor,0.001);
            if (string){
                printf("%f -> %s\n",value,string);

                free(string);
                BCDDestroy(dst);
                return 0;
                
            }
            BCDDestroy(dst);
            return 1;
        }
        else {
            BCDDestroy(dst);
            return 1;
        }
    }
    else
        return 1;
}

int main(int argc, char** argv){


    return test_float(0.121);
}

/*
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BCD.h"


int test_copy(){

    BCD* src = BCDCreate(8);

    BCD* dst = BCDCreate(8);

    unsigned int rc = BCDCopy(dst,src);

    BCDDestroy(src);

    BCDDestroy(dst);

    return rc;
}

int main(int argc, char** argv){


    return test_copy();
}

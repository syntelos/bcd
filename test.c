/*
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "BCD.h"



bool test(unsigned int lno,
          float vectorInValue,
          BCDFormatSign vectorInFormatSign,
          unsigned int vectorInPrecision,
          float vectorInAccuracy,
          char* vectorOut)
{


    BCD* dst = BCDCreate(9,9);
    if (dst){

        if (0 == BCDSetFloat(dst,vectorInValue)){

            char* string = BCDToString(dst,
                                       vectorInFormatSign,
                                       vectorInPrecision,
                                       vectorInAccuracy);
            if (string){

                if (0 == strcmp(string,vectorOut)){

                    free(string);
                    BCDDestroy(dst);
                    return true;
                }
                else {
                    fprintf(stderr,"%d\tFAILURE value: %f, sign: %d, prec: %d, acc: %f, expected: %s, result: %s\n\n",lno,vectorInValue,vectorInFormatSign,vectorInPrecision,vectorInAccuracy,vectorOut,string);

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

/*
 * Read input test vector file, "test.in".
 */
int main(int argc, char** argv){

    FILE* fin = fopen("test.in","r");
    if (fin){

        /*
         * Test Vector File Format
         * 
         * Line terminal (CRLF) or (LF).
         * Line comment starts with '#'.
         * 
         * Space separated line of "%f %u %u %f %s" as...
         */
        float vectorInValue; 
        BCDFormatSign vectorInFormatSign; 
        unsigned int vectorInPrecision; 
        float vectorInAccuracy;
        char vectorOut[128];

        /*
         * Process test vectors 
         */
        char linin[128];
        bool comment = false;
        unsigned int rc, lno = 0, vector = 0, success = 0, failure = 0;

        do {
            /*
             * Read test vector
             */
            lno += 1;
            comment = false;
            {
                char ch;
                unsigned int cc = 0;
                memset(linin,0,128);
                while (0 < (rc = fread(&ch,1,1,fin))){

                    if ('\r' == ch)
                        continue;
                    else if ('\n' == ch)
                        break;
                    else if ('#' == ch){
                        linin[cc++] = ch;
                        comment = true;
                    }
                    else if (128 <= cc){

                        fprintf(stderr,"%s: error: file 'test.in' input line #%d too long.\n",argv[0],lno);
                        return 1;
                    }
                    else
                        linin[cc++] = ch;
                }
            }
            /*
             * Evaluate test vector
             */
            if (*linin){
                if (comment){

                    fprintf(stderr,"%d\t%s\n",lno,linin);
                }
                else {
                    fprintf(stderr,"%d\t%s\n",lno,linin);

                    if (5 == sscanf(linin,"%f %u %u %f %128s",&vectorInValue,&vectorInFormatSign,&vectorInPrecision,&vectorInAccuracy,vectorOut)){

                        if (test(lno, 
                                 vectorInValue,
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
                }
            }
        }
        while (rc);

        return failure;
    }
    else {
        fprintf(stderr,"%s: error: opening test vector 'test.in'\n",argv[0]);
        return 1;
    }
}

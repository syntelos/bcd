/*
 * BCD
 * Copyright (C) 2013 John Pritchard (jdp@syntelos.org)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#define _BCD_C

#include <math.h>

#include "BCD.h"


/*
 */
BCD* BCDCreate(unsigned char integer, unsigned char fraction){
    BCD* bcd = malloc(sizeof(BCD));
    BCDOpen(bcd,integer,fraction);
    return bcd;
}
/*
 */
BCD* BCDCreateWord(){
    return BCDCreate(BCD_LEN_WORD,0);
}
/*
 */
BCD* BCDCreateLong(){
    return BCDCreate(BCD_LEN_LONG,0);
}
/*
 */
void BCDDestroy(BCD* bcd){
    if (bcd && BCDLength(bcd)){
        BCDClose(bcd);
        free(bcd);
    }
}
/*
 */
unsigned int BCDCopy(BCD* dst, BCD* src){
    if (src && dst && 
        src->integer <= dst->integer && 
        src->fraction <= dst->fraction)
    {
        BCDClear(dst);

        BCDDigit *sp, *dp;
        unsigned int sx, dx;

        /*
         * Copy integer part
         */
        sx = BCDInteger(src);
        dx = BCDInteger(dst);

        for (; BCDIntegerValid(src,sx) && BCDIntegerValid(dst,dx); sx--, dx--){

            sp = &(src->list[sx]);
            dp = &(dst->list[dx]);

            dp->bin = sp->bin;
        }

        /*
         * Copy fraction part
         */
        sx = BCDFraction(src);
        dx = BCDFraction(dst);

        for (; BCDFractionValid(src,sx) && BCDFractionValid(dst,dx); sx++, dx++){

            sp = &(src->list[sx]);
            dp = &(dst->list[dx]);

            dp->bin = sp->bin;
        }
        return 0;
    }
    else
        return 1;
}
/*
 */
unsigned int BCDSetWord(BCD* dst, int value){

}
/*
 */
unsigned int BCDSetLong(BCD* dst, long value){

}

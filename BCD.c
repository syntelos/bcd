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
BCD* BCDCreate(unsigned char length){
    BCD* bcd = malloc(sizeof(BCD));
    BCDOpen(bcd,length);
    return bcd;
}
/*
 */
BCD* BCDCreateWord(){
    return BCDCreate(BCD_LEN_WORD);
}
/*
 */
BCD* BCDCreateLong(){
    return BCDCreate(BCD_LEN_LONG);
}
/*
 */
void BCDDestroy(BCD* bcd){
    if (bcd && bcd->length){
        BCDClose(bcd);
        free(bcd);
    }
}
/*
 */
unsigned int BCDCopy(BCD* dst, BCD* src){
    if (src && dst && src->length <= dst->length){
        BCDClear(dst);

        BCDDigit* sp = src->list;
        const BCDDigit* sz = &(src->list[src->length-1]);
        BCDDigit* dp = dst->list;

        for (; sp < sz; sp++, dp++){

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

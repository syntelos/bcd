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
#ifndef _BCD_H
#define _BCD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    /*
     * Decimal digit value
     */
    unsigned char bin;
    /*
     * Shift register of modification records
     */
    unsigned char changes;

} BCDDigit;

typedef struct {

    unsigned char length;

    BCDDigit* list;

} BCD;

#define BCDClear(bp)                            \
    memset( (bp)->list,0,(bp)->length);

#define BCDOpen(bp,len)                         \
    (bp)->length = len;                         \
    (bp)->list = malloc(sizeof(BCDDigit)*len);  \
    BCDClear(bp);

#define BCDClose(bp)                        \
    free((bp)->list);                       \
    (bp)->length = 0;

#ifdef _BCD_C

const unsigned char BCD_LEN_WORD = (unsigned char)log10f(powf(2,(8*sizeof(int))-1));

const unsigned char BCD_LEN_LONG = (unsigned char)log10f(powf(2,(8*sizeof(long))-1));

#else
extern const unsigned char BCD_LEN_WORD;

extern const unsigned char BCD_LEN_LONG;
#endif

BCD* BCDCreate(unsigned char length);

BCD* BCDCreateWord();

BCD* BCDCreateLong();

void BCDDestroy(BCD* bcd);

unsigned int BCDCopy(BCD* dst, BCD* src);

unsigned int BCDSetWord(BCD* dst, int value);

unsigned int BCDSetLong(BCD* dst, long value);

#endif

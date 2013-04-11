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

/*
 * This digit includes a change register.  The change register is left
 * - shifted on each update to the BCD (number).  The least
 * significant bit of the change register is set to one when this
 * digit changes value.
 */
typedef struct {
    /*
     * Digit value
     */
    unsigned char bin;
    /*
     * Shift register of modification records
     */
    unsigned char changes;

} BCDDigit;
/*
 * The digit list is an array having length integer plus fraction (sum
 * total number of digits in memory).
 * 
 * The "list[integer]" element is the first element of the fraction
 * part, which is "fraction" number of digit elements in length.
 * 
 * The "list[integer-1]" element is the least significant digit of the
 * integer part, which is "integer" number of digit elements in
 * length.  
 * 
 * The most significant digits of the integer part, and the least
 * significant digits of the fraction part, may have value zero.
 */
typedef struct {
    /*
     * Number of digits allocated to integer part of BCD number
     */
    unsigned char integer;
    /*
     * Number of digits allocated to fraction part of BCD number
     */
    unsigned char fraction;

    BCDDigit* list;

} BCD;

#define BCDLength(bp) ((bp)->integer+(bp)->fraction)

#define BCDClear(bp)                            \
    memset( (bp)->list,0,BCDLength(bp));

#define BCDOpen(bp,ii,ff)                           \
    (bp)->integer = ii;                             \
    (bp)->fraction = ff;                            \
    (bp)->list = malloc(sizeof(BCDDigit)*(ii+ff));  \
    BCDClear(bp);

#define BCDClose(bp)                        \
    free((bp)->list);                       \
    (bp)->integer = 0;                      \
    (bp)->fraction = 0;
/*
 * List index for least signficant integer digit
 */
#define BCDInteger(bp) (((bp)->integer)-1)
/*
 * Boolean test of index for integer digit
 */
#define BCDIntegerValid(bp,ix) ((0 <= (ix))&&((ix) <= BCDInteger(bp)))
/*
 * List index for most signficant fraction digit
 */
#define BCDFraction(bp) ((bp)->integer)
/*
 * Boolean test of index for fraction digit
 */
#define BCDFractionValid(bp,ix) ((BCDFraction(bp) <= (ix))&&((ix) <= BCDLength(bp)))

#ifdef _BCD_C

const unsigned char BCD_LEN_WORD = (unsigned char)log10f(powf(2,(8*sizeof(int))-1));

const unsigned char BCD_LEN_LONG = (unsigned char)log10f(powf(2,(8*sizeof(long))-1));

#else
extern const unsigned char BCD_LEN_WORD;

extern const unsigned char BCD_LEN_LONG;
#endif

BCD* BCDCreate(unsigned char integer, unsigned char fraction);

BCD* BCDCreateWord();

BCD* BCDCreateLong();

void BCDDestroy(BCD* bcd);

unsigned int BCDCopy(BCD* dst, BCD* src);

unsigned int BCDSetWord(BCD* dst, int value);

unsigned int BCDSetLong(BCD* dst, long value);

#endif

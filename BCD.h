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
#include <stdbool.h>
#include <string.h>

/*
 * This digit includes a change register.  The change register is left
 * - shifted on each update to the BCD (number).  The least
 * significant bit of the change register is set to one when this
 * digit changes value.
 * 
 * Note that sign changes are not recorded.
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
     * True when the number is negative
     */
    bool sign;
    /*
     * Number of digits allocated to integer part of BCD number
     */
    unsigned char integer;
    /*
     * Number of digits allocated to fraction part of BCD number
     */
    unsigned char fraction;
    /*
     * List of digits including integer and fraction subsets in a
     * fixed point layout.
     */
    BCDDigit* list;

} BCD;

#define BCDLength(bp) ((bp)->integer+(bp)->fraction)
/*
 * BCD length plus three for sign, decimal point, and string
 * terminator.
 */
#define BCDStringLen(bp) (BCDLength(bp)+3)

#define BCDClear(bp)                                        \
    memset( (bp)->list,0,(sizeof(BCDDigit)*BCDLength(bp)));

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
#define BCDFractionValid(bp,ix) ((BCDFraction(bp) <= (ix))&&((ix) < BCDLength(bp)))

/*
 * Create a BCD having "integer" and "fraction" number of base ten digits 
 */
BCD* BCDCreate(unsigned char integer, unsigned char fraction);
/*
 * Create a BCD having "integer" and "fraction" number of digits
 * copied from 'src'
 */
BCD* BCDCreateFrom(const BCD* src);
/*
 * Free all memory held via pointer
 */
void BCDDestroy(BCD* bcd);
/*
 * Return true for non-null and not-empty data structure (may have
 * value zero)
 */
bool BCDIsOpen(const BCD* src);
/*
 * Return true for null or empty data structure
 */
bool BCDIsClosed(const BCD* src);
/*
 * Return true for zero valued number, or a closed data structure.
 */
bool BCDIsZero(const BCD* src);
/*
 * Copy 'src' to 'dst', first clearing 'dst', and then ignoring sizing
 * differences.  
 * 
 * This function safely performs the requested copy operation for
 * accessible digits and changes.  Each part of the number is copied
 * for digits that exist in both 'src' and 'dst'.
 * 
 * Because the 'dst' is first cleared, this operation will be
 * consistent subject to the sizes of 'src' and 'dst'.
 * 
 * Return zero on success and one on failure.  For failure cases,
 * refer to the source.
 */
unsigned int BCDCopy(BCD* dst, BCD* src);
/*
 * Update digits and their change registers from a signed integer
 * value.  
 * 
 * Accepts any arguments.  A valid "BCD dst" has been correctly
 * allocated and initialized for any integer and fraction lengths.  A
 * valid "int value"
 * 
 * Return zero on success and one on failure.  For failure cases,
 * refer to the source.
 */
unsigned int BCDSetWord(BCD* dst, int value);
/*
 * Update digits and their change registers from a floating point
 * value.
 * 
 * Accepts any arguments.  A valid "BCD dst" has been correctly
 * allocated and initialized for any integer and fraction lengths.  A
 * valid "float value" is not a NaN or infinity (value == value).
 * 
 * Return zero on success and one on failure.  For failure cases,
 * refer to the source.
 */
unsigned int BCDSetFloat(BCD* dst, float value);

typedef enum {
    /*
     * No sign (absolute value)
     */
    BCDFormatSignNone,
    /*
     * Optional minus sign
     */
    BCDFormatSignOpt, 
    /*
     * Require sign: one of plus or minus
     */
    BCDFormatSignReq

} BCDFormatSign;

typedef enum {
    /*
     * Ignore signal floor value
     */
    BCDFormatSignalAny,
    /*
     * Represent values on or above the floor value
     */
    BCDFormatSignalFloor

} BCDFormatSignal;

/*
 * Allocate and populate a character array representing the BCD number.
 * 
 * Return NULL on failure, otherwise a newly malloc'ed string buffer.
 * 
 * This "precision" is related to representational accuracy, and this
 * "signal" is an expression of representational precision.  Need to
 * review what's going on in the broader context between signal
 * processing and representation.  For the moment this is useful, but
 * it might be more hackery than it should be.
 */
char* BCDToString(const BCD* src, 
                  const BCDFormatSign signFormat, 
                  const unsigned int precision, 
                  const BCDFormatSignal signalFormat, const float signalValue);

/*
 * Print data structure numeric (not changes) content to output
 */
void BCDDebugPrint(FILE* out, const BCD* src);

#endif

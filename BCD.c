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
#include <float.h>

#include "BCD.h"

#define BCD_EPS (FLT_EPSILON*2.0)

#define BCD_EEQ(a,b)  (fabsf(a - b) < BCD_EPS)

#define BCD_ENE(a,b)  (fabsf(a - b) > BCD_EPS)

#define BCD_EGT(a,b)  ((a - b) > BCD_EPS)

#define BCD_ELT(a,b)  ((b - a) > BCD_EPS)

/*
 */
BCD* BCDCreate(unsigned char integer, unsigned char fraction){
    BCD* bcd = malloc(sizeof(BCD));
    BCDOpen(bcd,integer,fraction);
    return bcd;
}
/*
 */
BCD* BCDCreateFrom(const BCD* src){
    if (src && BCDLength(src)){

        return BCDCreate(src->integer,src->fraction);
    }
    else
        return NULL;
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
bool BCDIsOpen(const BCD* src){
    return (src && BCDLength(src));
}
/*
 */
bool BCDIsClosed(const BCD* src){
    return (NULL == src || 0 == BCDLength(src));
}
/*
 */
bool BCDIsZero(const BCD* src){
    if (src){
        const int bcd_len = BCDLength(src);
        if (bcd_len){

            BCDDigit *sp;
            unsigned int sx;

            for (sx = 0; sx < bcd_len; sx++){

                sp = &(src->list[sx]);

                if (0 != sp->bin){

                    return false;
                }
            }
        }
    }
    return true;
}
/*
 */
unsigned int BCDCopy(BCD* dst, BCD* src){
    if (src && dst && BCDLength(src) && BCDLength(dst))
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
            dp->changes = sp->changes;
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
            dp->changes = sp->changes;
        }
        return 0;
    }
    else
        return 1;
}
/*
 */
unsigned int BCDSetWord(BCD* dst, int value){
    if (dst && BCDLength(dst)){

        dst->sign = (0 > value);
        float v = (float)abs(value), r;
        unsigned int ix = BCDInteger(dst);
        unsigned int fx = BCDFraction(dst);
        unsigned int dx;
        unsigned char bin, changes;
        BCDDigit *dp;

        /*
         * Update the integer part
         */
        for (dx = ix; BCDIntegerValid(dst,dx); dx--){

            dp = &(dst->list[dx]);

            changes = ((dp->changes)<<1);

            r = fmodf(v,10);
            if (r == r){

                if (BCD_EEQ(0.0,r)){

                    bin = 0;
                }
                else {

                    bin = (unsigned char)r;
                }

                if (bin != dp->bin){

                    changes ^= 1;
                }
                dp->bin = bin;
                dp->changes = changes;

                v /= 10;
                if (BCD_EGT(1.0,v)){

                    break;
                }
            }
            else
                break;
        }

        /*
         * Update the fraction part
         */
        for (dx = fx; BCDFractionValid(dst,dx); dx++){

            dp = &(dst->list[dx]);

            changes = ((dp->changes)<<1);

            if (0 != dp->bin){

                changes ^= 1;
            }
            dp->bin = 0;
            dp->changes = changes;
        }

        return 0;
    }
    else
        return 1;
}
/*
 */
unsigned int BCDSetFloat(BCD* dst, float value){
    if (dst && BCDLength(dst) && value == value){

        dst->sign = (0 > value);
        double v = fabs(value), r;
        unsigned int ix = BCDInteger(dst);
        unsigned int fx = BCDFraction(dst);
        unsigned int dx;
        unsigned char bin, changes;
        BCDDigit *dp;

        /*
         * Update the integer part
         */
        for (dx = ix; BCDIntegerValid(dst,dx); dx--){

            dp = &(dst->list[dx]);

            changes = ((dp->changes)<<1);

            r = fmodf(v,10);
            if (r == r){

                if (BCD_EEQ(0.0,r)){

                    bin = 0;
                }
                else {

                    bin = (unsigned char)r;
                }

                if (bin != dp->bin){

                    changes ^= 1;
                }
                dp->bin = bin;
                dp->changes = changes;

                v -= bin;

                if (BCD_EGT(1.0,v)){

                    break;
                }
                else
                    v /= 10;
            }
            else {
                return 1;
            }
        }

        /*
         * Update the fraction part
         */
        for (dx = fx; BCDFractionValid(dst,dx); dx++){

            dp = &(dst->list[dx]);

            changes = ((dp->changes)<<1);

            v *= 10;

            r = fmod(v,10);
            if (r == r){

                if (BCD_EEQ(0.0,r)){

                    bin = 0;
                }
                else {

                    bin = (unsigned char)r;
                }

                if (bin != dp->bin){

                    changes ^= 1;
                }
                dp->bin = bin;
                dp->changes = changes;

                v -= bin;
            }
            else {
                if (0 != dp->bin){

                    changes ^= 1;
                }
                dp->bin = 0;
                dp->changes = changes;
            }
        }

        return 0;
    }
    else
        return 1;
}
char* BCDToString(const BCD* src, const BCDFormatSign signFormat, const unsigned int precision, 
                  const BCDFormatSignal signalFormat, const float signalValue)
{
    if (src && BCDLength(src)){

        const unsigned int string_len = BCDStringLen(src);

        char* string = malloc(string_len);
        if (string){
            memset(string,0,string_len);

            /*
             * Signal floor
             */
            BCD* signalFloor;

            if (BCDFormatSignalFloor == signalFormat){

                signalFloor = BCDCreateFrom(src);

                BCDSetFloat(signalFloor,signalValue);
            }
            else {
                signalFloor = NULL;
            }
            
            unsigned int stringx = 0;

            bool significand = false;
            unsigned int sigdigits = 0;

            /*
             * Copy sign part
             */
            switch(signFormat){

            case BCDFormatSignOpt:
                if (src->sign){
                    string[stringx++] = '-';
                }
                break;
            case BCDFormatSignReq:
                if (src->sign){
                    string[stringx++] = '-';
                }
                else {
                    string[stringx++] = '+';
                }
                break;
            default:
                break;
            }

            BCDDigit *sp, *ss;
            unsigned int sx;
            bool aboveSignalFloor = true;

            /*
             * Copy integer part
             */
            for (sx = 0; BCDIntegerValid(src,sx); sx++){

                sp = &(src->list[sx]);


                if (0 == sp->bin){
                    /*
                     * Skip leading zeros in integer string
                     */
                    if (significand){
                        sigdigits += 1;
                        string[stringx++] = '0';
                    }
                }
                else {
                    significand = true;

                    if (aboveSignalFloor && (0 == precision || sigdigits < precision)){

                        string[stringx++] = '0'+(sp->bin);

                        sigdigits += 1;

                    }
                    else {
                        string[stringx++] = '0';
                    }
                }

                /*
                 * Check signal floor constraint 
                 */
                if (aboveSignalFloor && signalFloor){

                    ss = &(signalFloor->list[sx]);

                    if (0 != ss->bin){

                        aboveSignalFloor = false;
                    }
                }
            }

            /*
             * Copy fraction part
             */
            if (aboveSignalFloor && (0 == precision || sigdigits < precision)){

                sx = BCDFraction(src);
                /*
                 * Representation prefix
                 */
                if (BCDFractionValid(src,sx)){

                    if (!significand){

                        string[stringx++] = '0';
                    }

                    string[stringx++] = '.';
                }

                for (; BCDFractionValid(src,sx) && aboveSignalFloor && (0 == precision || sigdigits < precision); sx++){

                    sp = &(src->list[sx]);

                    if (0 == sp->bin){

                        string[stringx++] = '0';

                        if (significand){

                            sigdigits += 1;
                        }
                    }
                    else {
                        significand = true;

                        string[stringx++] = '0'+(sp->bin);

                        sigdigits += 1;
                    }

                    /*
                     * Check signal floor constraint 
                     */
                    if (aboveSignalFloor && signalFloor){

                        ss = &(signalFloor->list[sx]);

                        if (0 != ss->bin){

                            aboveSignalFloor = false;
                        }
                    }
                }
            }

            /*
             * Truncate zero to representation of precision (in zero)
             */
            if (!significand){

                if (0 < precision){
                    if (2 < precision){
                        string[0] = '0';
                        string[1] = '.';
                        for (sx = 2; sx < precision; sx++){
                            string[sx] = '0';
                        }
                        string[3] = 0;
                    }
                    else {
                        string[0] = '0';
                        string[1] = 0;
                    }
                }
                else {
                    string[0] = '0';
                    string[1] = 0;
                }
            }

            BCDDestroy(signalFloor);

            return string;
        }
    }
    return NULL;
}
void BCDDebugPrint(FILE* out, const BCD* src){
    if (out && src){
        const int bcd_len = BCDLength(src);
        if (bcd_len){

            fprintf(out,"[%s] sign %c\n","si",(src->sign)?('-'):('+'));

            BCDDigit *sp;
            unsigned int sx;

            for (sx = 0; sx < bcd_len; sx++){

                if (BCDIntegerValid(src,sx)){

                    sp = &(src->list[sx]);

                    fprintf(out,"[%02d] intg %d\n",sx,sp->bin);
                }
                else if (BCDFractionValid(src,sx)){

                    sp = &(src->list[sx]);

                    fprintf(out,"[%02d] frac %d\n",sx,sp->bin);
                }
            }
        }
    }
}

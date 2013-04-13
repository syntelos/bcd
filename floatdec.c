#include <stdio.h>
#include <stdbool.h>
#include <math.h>


int main(int argc, char** argv){

    printf("static const float floatdec[256] = {\n    ");

    unsigned int fraction ;
    float dec, ceil = 0;
    bool bounds = true;

    for (fraction = 0; fraction < 256; fraction++){

        dec = pow(10.0,fraction);

        if (0 < fraction && 0 == fraction % 11){
            printf("\n    ");
        }

        if (bounds && dec == dec && 
            (0 == fraction || 0 == fmodf(dec,10)))
        {

            printf("%1.0f",dec);
            ceil = dec;
        }
        else {
            bounds = false;

            printf("%1.0f",ceil);
        }

        if (fraction < 255)
            printf(", ");
        else
            printf("\n");

    }

    printf("};\n");
}

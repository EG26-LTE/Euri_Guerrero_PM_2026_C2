#include <stdio.h>
#include <stdlib.h>

int main()
{
    float x = 0.0;
    float b = 0.0;
    unsigned int cnt = 0;

    printf("\nIngresa el numero:");
    scanf("%f", &x) ;

    b = x;

    while (!(b== (x/b)))
    {
        printf("\nValor de b[%i]:%f",cnt,b) ;
        b = 0.5 * ((x/b) +b) ;
        cnt++;
    }

    printf("\nRaiz de %f es:%f", x,b);
}


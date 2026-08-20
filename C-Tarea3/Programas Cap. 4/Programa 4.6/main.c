#include <stdio.h>
#include <stdlib.h>

void f1(int *);

void main(void)
{
    int I, K = 4;
    for (I = 1; I <= 3; I++)
    {
        printf("\n\nValor de K antes de llamar a la funcion: %d", ++K);
        f1(&K);  /*Llame la funcion fuera del printf, contrario a como estaba
        en el ejercicio because que me estaba dando error*/
        printf("\nValor de K despues de llamar a la funcion: %d", K);
    }
}

void f1(int *R)
{
    *R += *R;
}

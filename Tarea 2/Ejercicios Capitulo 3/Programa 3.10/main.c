#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(void)
{
    int I, N, NUM, SPA = 0, SIM = 0, CIM = 0;
    printf("Ingrese el numero de datos que se van a procesar:\t");
    scanf("%d", &N);
    if (N > 0)
    {
        for (I=1; I <= N; I++)
        {
            printf("\nIngrese el numero %d:", I);
            scanf("%d", &NUM);
            if (NUM)
                if (pow(-1, NUM) > 0)
                SPA = SPA + NUM;
                else
                {
                    SIM = SIM + NUM;
                    CIM++;
                }
        }
        printf("\nLa Suma de los numeros pares es: %d", SPA);
        printf("\nEl promedio de los numeros impares es: %5.2f", (float)(SIM / CIM));
    }
    else
        printf("\nEL valor de N es incorrecto");
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cantidaddeJugadas = 1;

void jugar(void)
{
    int nGanador, numero, I;
    float cantidad, totalGanado = 0;

    srand(time(NULL));
    nGanador = (rand() % 40) + 1;

    printf("\nJUGANDO LOTERIA\n");
    printf("|nCantidad de jugadas: %d", cantidaddeJugadas);

    for (I = 1; I <= cantidaddeJugadas; I++)
        {
        printf("\nJugada %d ", I);

        do
        {
            printf("\nIngrese un numero entre 1 y 40: ");
            scanf("%d", &numero);
            if (numero < 1 || numero > 40)
                printf("\nFUAP Numero Erroneo");
        }
        while (numero < 1 || numero > 40);

        do
        {
            printf("Ingrese la cantidad a jugar:");
            scanf("%f", &cantidad);
        }
        while (cantidad <= 0);

        printf("Jugaste el numero %d con $%.2f\n\n", numero, cantidad);

        if (numero == nGanador)
            {
            totalGanado += cantidad * 1000;
            }
    }
    printf("Numero ganador: %d\n", nGanador);
    if (totalGanado > 0)
    {
        printf("FELICIDADES CACON Ganaste: $%.2f", totalGanado);
    }
    else
    {
        printf("Usted acaba de perder su dinero Jaja\n\n");
    }
}

void elegirCantidad()
{
    int n;
    printf("\nIngrese la cantidad de jugadas:");
    scanf("%d", &n);
    if (n > 0)
    {
        cantidaddeJugadas = n;
        printf("\nLa cantidad de jugadas son: %d", cantidaddeJugadas);
    }
    else
    {
        printf("Cacon tu no puedes jugar 0 veces");
    }
}

int main()
{
    int opcion;

    do
        {
        printf("\nLoteria La Bendición\n");
        printf("\n1. Jugar");
        printf("\n2. Elegir cantidad de jugadas", cantidaddeJugadas);
        printf("\n3. Salir");
        printf("\nOpcion: ");
        scanf("%d", &opcion);
        printf("\n");

        switch(opcion)
        {
        case 1:
            jugar();
            break;
        case 2:
            elegirCantidad();
            break;
        case 3:
            printf("Byee");
            break;
        }
    } while (opcion != 3);

    return 0;
}

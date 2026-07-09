#include <stdio.h>
#include <stdlib.h>

void hanoi(int n, int *origen, int *toporigen, char nomorigen, int *auxiliar, int *topauxiliar, char nomauxiliar, int *destino, int *topdestino, char nomdestino, FILE *archivo);

int main()
{
    int ndis;
    int mov = 1;
    int *p1, *p2, *p3;
    int top1, top2, top3;

    FILE *archivo;

    archivo = fopen("movimientos.txt", "w");

    do
    {
        printf("----Bienvenido a la Torre de Hanoi----");
        printf("\nIngrese un numero de discos mayor a 0 y menor a 11(no somos locos):");
        scanf("%d", &ndis);

        if (ndis > 0 && ndis <= 11)
        {
            printf("\nEl numero de discos ingresados es: %d \n", ndis);
        }
        else
        {
            printf("\nError el numero de discos no puede ser menor o igual a cero ni mayor a 30 cacon.\n\n");
        }
    }
    while (ndis <= 0 || ndis > 11);
    {
       int *dis = (int *)malloc(ndis * sizeof(int));

       if(dis == NULL)
       {
           printf("La memoria no pudo ser reservada para %d discos", ndis);
       }

       for(int i = 0; i < ndis; i++)
       {
           dis[i] = i + 1;
       }

       for(int i = 0; i < ndis; i++)
       {
        mov *= 2;
       }

        mov -= 1;

        //printf para probar que la funcion matematiqueishon (2^n) - 1 funcione
        printf("\nSe necesitaran %d movimientos para resolver la torre\n", mov);

        p1 = malloc(ndis * sizeof(int));
        top1 = ndis -1;

        for(int i = 0; i < ndis; i++)
        {
            p1[i] = ndis - i;

           // printf("\np1: %d \n", i);

        }

        p2 = malloc(ndis * sizeof(int));
        top2 = -1;


        p3 = malloc(ndis * sizeof(int));
        top3 = -1;
    }
    hanoi(ndis,
          p1, &top1, 'A',
          p2, &top2, 'B',
          p3, &top3, 'C',
          archivo);

    fclose(archivo);
    free(p1);
    free(p2);
    free(p3);

    printf("\n\n ------------La TORRE DE HANOI HA TERMINADO CON EXITO------------ \n\n");
    printf("Verifique el archivo de texto ''movimientos'' para ver los movimientos de la ultima operacion\n");
}

void hanoi(int n, int *origen, int *toporigen, char nomorigen, int *auxiliar, int *topauxiliar, char nomauxiliar, int *destino, int *topdestino, char nomdestino, FILE *archivo)
{
    if (n == 1)
    {
        int temp = origen[*toporigen];

        (*toporigen)--;

        (*topdestino)++;

        destino[*topdestino] = temp;

    fprintf(archivo, "\nMover disco %d de %c a %c\n", temp, nomorigen, nomdestino);
    //printf("\ntopOrigen=%d topDestino=%d\n", *toporigen, *topdestino);

        return;
    }

    hanoi(n - 1,
          origen, toporigen, nomorigen,
          destino, topdestino, nomdestino,
          auxiliar, topauxiliar, nomauxiliar,
          archivo);

    int temp = origen[*toporigen];

    (*toporigen)--;

    (*topdestino)++;

    destino[*topdestino] = temp;

    fprintf(archivo, "\nMover disco %d de %c a %c\n", temp, nomorigen, nomdestino);
    //printf("\ntopOrigen=%d topDestino=%d\n", *toporigen, *topdestino);

    hanoi(n - 1,
          auxiliar, topauxiliar, nomauxiliar,
          origen, toporigen, nomorigen,
          destino, topdestino, nomdestino,
          archivo);
}

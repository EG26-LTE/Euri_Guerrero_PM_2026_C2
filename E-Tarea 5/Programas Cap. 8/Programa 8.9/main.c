#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char zona[20];
    char calle[20];
    char colo[20];
} ubicacion;

typedef struct
{
    char clave[6];
    float scu;
    float ste;
    char car[50];
    ubicacion ubi;
    float precio;
    char dispo;
} propiedades;

void Lectura(propiedades *, int);
void F1(propiedades *, int);
void F2(propiedades *, int);

int main(void)
{

    propiedades PROPIE[100];
    int TAM;
    do
    {
        printf("Ingrese el numero de propiedades:");
        scanf("%d", &TAM);
    }
    while (TAM > 100 || TAM < 1);
    Lectura(PROPIE, TAM);
    F1(PROPIE, TAM);
    F2(PROPIE, TAM);

    return 0;
}

void Lectura(propiedades A[], int T)
{

    int I;
    for (I = 0; I < T; I++)
    {

        printf("\n\tIngrese datos de la propiedad %d\n", I + 1);
        getchar();

        printf("Clave: ");
        fgets(A[I].clave, sizeof(A[I].clave), stdin);
        A[I].clave[strcspn(A[I].clave, "\n")] = '\0';

        printf("Superficie cubierta:");
        scanf("%f", &A[I].scu);
        printf("Superficie terreno:");
        scanf("%f", &A[I].ste);
        getchar();

        printf("Caracteristicas:");
        fgets(A[I].car, sizeof(A[I].car), stdin);
        A[I].car[strcspn(A[I].car, "\n")] = '\0';

        printf("\tZona:");
        fgets(A[I].ubi.zona, sizeof(A[I].ubi.zona), stdin);
        A[I].ubi.zona[strcspn(A[I].ubi.zona, "\n")] = '\0';

        printf("\tCalle:");
        fgets(A[I].ubi.calle, sizeof(A[I].ubi.calle), stdin);
        A[I].ubi.calle[strcspn(A[I].ubi.calle, "\n")] = '\0';

        printf("\tColonia:");
        fgets(A[I].ubi.colo, sizeof(A[I].ubi.colo), stdin);
        A[I].ubi.colo[strcspn(A[I].ubi.colo, "\n")] = '\0';

        printf("Precio:");
        scanf("%f", &A[I].precio);
        getchar();

        printf("Disponibilidad (Venta-V / Renta-R):");
        scanf("%c", &A[I].dispo);
    }
}

void F1(propiedades A[], int T)
{

    int I;
    printf("\n\t\tListado de Propiedades para Venta en Ocoa\n");
    for (I = 0; I < T; I++)
    {

        if ((A[I].dispo == 'V' || A[I].dispo == 'v') &&
                strcmp(A[I].ubi.zona, "Miraflores") == 0 &&
                A[I].precio >= 450000 && A[I].precio <= 650000)
        {


            printf("\nClave de la propiedad: %s", A[I].clave);
            printf("\nSuperficie cubierta: %.2f", A[I].scu);
            printf("\nSuperficie terreno: %.2f", A[I].ste);
            printf("\nCaracteristicas: %s", A[I].car);
            printf("\nCalle: %s", A[I].ubi.calle);
            printf("\nColonia: %s", A[I].ubi.colo);
            printf("\nPrecio: %.2f\n", A[I].precio);
        }
    }
}

void F2(propiedades A[], int T)
{

    int I;
    float li, ls;
    char zon[20];

    printf("\n\t\tListado de Propiedades para Renta\n");
    getchar();
    printf("Ingrese zona geografica: ");
    fgets(zon, sizeof(zon), stdin);
    zon[strcspn(zon, "\n")] = '\0';

    printf("Ingrese el limite inferior del precio:");
    scanf("%f", &li);
    printf("Ingrese el limite superior del precio:");
    scanf("%f", &ls);

    for (I = 0; I < T; I++)
    {

        if ((A[I].dispo == 'R' || A[I].dispo == 'r') &&
                strcmp(A[I].ubi.zona, zon) == 0 &&
                A[I].precio >= li && A[I].precio <= ls)
        {


            printf("\nClave de la propiedad: %s", A[I].clave);
            printf("\nSuperficie cubierta: %.2f", A[I].scu);
            printf("\nSuperficie terreno: %.2f", A[I].ste);
            printf("\nCaracteristicas: %s", A[I].car);
            printf("\nCalle: %s", A[I].ubi.calle);
            printf("\nColonia: %s", A[I].ubi.colo);
            printf("\nPrecio: %.2f\n", A[I].precio);
        }
    }
}

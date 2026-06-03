#include <stdio.h>
#include <stdlib.h>
#define SALIR 0
#define SUMAR 1
#define RESTAR 2
#define MULTIPLICAR 3
#define DIVIDIR 4

void entrada(int oper);
double Sumar(double sumando1, double sumando2);
//ambito de variables
//variables globales
    double n1 = 0.0;//decimal de 64 bits
    double n2 = 0.0;
    double buf = 0.0;

int main()
{
    int menu= -1;
    printf("\nCALCULADORA BASICA");

do
    {
        printf("\n\n0-Salir\n1-Sumar\n2-Restar\n3-Multiplicar\n4-Dividir\n");
        scanf("%i",&menu);
        if(menu == SUMAR)
        {
            entrada(SUMAR);
            buf = Sumar(n1,n2);//pasamos por valor
            printf("\nResultado:%lf",buf);
        }
        else if(menu == RESTAR)
        {

        }
        else if(menu == MULTIPLICAR)
        {

        }
        else if(menu == DIVIDIR)
        {

        }

    }
    while(menu != SALIR);

    return 0;
}
void entrada(int oper)
{
    if(oper == SUMAR)
    {
        printf("\nIngrese el primer operando:");
        scanf("%lf", &n1);
        printf("\nIngrese el segundo operando:");
        scanf ("%lf", &n2);
        return;
    }
}
double Sumar(double sumando1, double sumando2)
{
    double tmp = 0.0;
    tmp = sumando1 + sumando2;
    return tmp;
}

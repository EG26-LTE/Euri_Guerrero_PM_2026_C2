#include <stdio.h>
#include <stdlib.h>
#define SALIR 0
#define MULTIPLICAR 3
#define DIVIDIR 4
#define SUMAR 1
#define RESTAR 2
#define ERR_OK 0
#define ERR_DivByZero 504



void entrada(int oper, double * num1, double *num2);//declaracion de la funcion
void salida(double numero);
int Sumar(double sumando1,double sumando2, double *result);
int Mul(double numero1, double numero2, double *result);
int Dividir(double numerador, double denominador, double *result);
int Restar(double numerador, double denominador, double *result);
//ambito de variables
//variables globales





int main()
{
    int menu = -1;
    int cod_err = ERR_OK;
    double n1 = 0.0;//decimal de 64 bits
    double n2 = 0.0;
    double buf = 0.0;
    double *pd;
    // * operador de indireccion
    // & operador de direccion
    printf("\nCALCULADORA BASICA");



    do
    {
        printf("\n\n0-Salir\n1-Sumar\n2-Restar\n3-Multiplicar\n4-Dividir\n");
        scanf("%i",&menu);
        if(menu == SUMAR)
        {
            entrada(SUMAR,&n1,&n2);
            Sumar(n1,n2, &buf);//pasamos por valor
            printf("\nResultado:%lf",buf);
        }
        else if(menu == RESTAR)
        {
           entrada(RESTAR,&n1,&n2);
           Restar(n1,n2, &buf);//pasamos por valor
           printf("\nResultado:%lf",buf);
        }
        else if(menu == MULTIPLICAR)
        {
            entrada(MULTIPLICAR,&n1,&n2);
            Mul(n1,n2,&buf);
            printf("\nResultado:%lf",buf);
        }
        else if(menu == DIVIDIR)
        {
            entrada(DIVIDIR,&n1,&n2);
            cod_err = Dividir(n1,n2, &buf);
            if(cod_err != ERR_OK)
            {
               if(cod_err == ERR_DivByZero)
               {
                   printf("\nERROR Division By Zero");
               }
            }else
            {
              printf("\nResultado:%lf",buf);
            }



        }else
        {
            printf("\nEntrada Menu invalida\nValores validos 0 - 4\n");
        }



    }
    while(menu != SALIR);





    return 0;
}
void salida(double numero)
{
   printf("\nResultado:%lf",numero);
}
void entrada(int oper, double *num1, double *num2)
{
    if(oper == SUMAR || oper == MULTIPLICAR)
    {
        printf("\nIngrese el primer operando:");
        scanf("%lf",num1);
        printf("\nIngrese el segundo operando:");
        scanf("%lf",num2);
        return;



    }
    else if(oper == DIVIDIR)
    {
        printf("\nIngrese el numerador:");
        scanf("%lf",num1);
        printf("\nIngrese el denominador:");
        scanf("%lf",num2);
        return;
    }
    else if(oper == RESTAR)
    {
        printf("\nIngrese el minuendo:");
        scanf("%lf",num1);
        printf("\nIngrese el sustraendo:");
        scanf("%lf",num2);
        return;
    }
}
//recivimos por valor
int Sumar(double sumando1,double sumando2,double *result)
{
    double tmp = 0.0;
    tmp = sumando1 + sumando2;
    *result = tmp;
    return ERR_OK;
}



int Mul(double numero1, double numero2,double *result)
{
    *result = numero1 * numero2;
   return ERR_OK;
}



int Dividir(double numerador, double denominador, double *result)
{
    if(denominador == 0.0)
    {
        return ERR_DivByZero;



    }else{
        *result = numerador / denominador;
        return ERR_OK;
    }



}



int Restar(double minuendo, double sustraendo, double *result)
{
    *result = minuendo - sustraendo;
   return ERR_OK;
}



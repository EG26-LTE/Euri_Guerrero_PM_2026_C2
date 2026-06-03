#include
#include

int main ()
{
    int dato = -1;
    printf ("\nIngresa un numero del 1 al 10");
    scanf ("%i",&dato);
    if (dato >=1 && dato >=10)
    {
        if(dato == 5)
        {
            printf("\nAdivinaste el numero 5");
        }else
        {
            printf("\nPerdiste!");
        }
    }else
    return 0;
}

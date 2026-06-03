#include <stdio.h>
#include <stdlib.h>

int main()
{
  int a = 10;
  int b = 5;
  int *pi;
  pi = &a;
  printf("\nValor de a: %i\n",a);
  printf("\nValor de a con puntero: %i\n", *pi);
  pi = &b;
  printf("\nValor de b: %i\n");
  printf("\nValor de b con puntero: %i\n");
  *pi = 50;
  printf("\nValor de b: %i\n", b);
  printf("\nValor de b con puntero: %i\n", *pi);
  printf("\nValor de a: %i\n");
}

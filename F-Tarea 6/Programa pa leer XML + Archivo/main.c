#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

int main()
{
    FILE *archivo;
    char linea[200];
    int cantidad = 0;
    int i = 0;
    Alumno *alumnos;

    archivo = fopen("alumnos.xml", "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo alumnos.xml\n");
        return 1;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {

        if (strstr(linea, "<alumno>") != NULL) {
            cantidad++;
        }
    }
    fclose(archivo);

    printf("Cantidad de alumnos encontrados: %d\n\n", cantidad);

    alumnos = (Alumno *) malloc(cantidad * sizeof(Alumno));
    if (alumnos == NULL)
    {
        printf("Error al reservar memoria\n");
        return 1;
    }

    archivo = fopen("alumnos.xml", "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo XML\n");
        free(alumnos);
        return 1;
    }

    i = -1;

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {

        if (strstr(linea, "<alumno>") != NULL)
        {
            i++;
        }
        else if (strstr(linea, "<nombre>") != NULL)
        {
            sscanf(linea, " <nombre>%[^<]", alumnos[i].nombre);
        }
        else if (strstr(linea, "<apellido>") != NULL)
        {
            sscanf(linea, " <apellido>%[^<]", alumnos[i].apellido);
        }
        else if (strstr(linea, "<promedio>") != NULL)
        {
            sscanf(linea, " <promedio>%f", &alumnos[i].promedio);
        }
        else if (strstr(linea, "<materia>") != NULL)
        {
            sscanf(linea, " <materia>%[^<]", alumnos[i].materia);
        }
    }
    fclose(archivo);

    printf("Lista de alumnos:\n");
    printf("-----------------------------------------\n");
    for (i = 0; i < cantidad; i++) {
        printf("Nombre:   %s\n", alumnos[i].nombre);
        printf("Apellido: %s\n", alumnos[i].apellido);
        printf("Promedio: %.2f\n", alumnos[i].promedio);
        printf("Materia:  %s\n", alumnos[i].materia);
        printf("-----------------------------------------\n");
    }

    free(alumnos);

    return 0;
}

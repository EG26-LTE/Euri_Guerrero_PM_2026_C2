#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char apellido[50];
    float promedio;
    char materia[50];
} Alumno;

int main() {
    FILE *archivo;
    char linea[200];
    int cantidad = 0;
    int i = 0;
    Alumno *alumnos;
    char primeraLinea = 1;
    archivo = fopen("alumnos.csv", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo alumnos.csv\n");
        return 1;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (primeraLinea)
        {
            primeraLinea = 0;
            continue;
        }
        cantidad++;
    }
    fclose(archivo);

    printf("Cantidad de alumnos encontrados: %d\n\n", cantidad);

    alumnos = (Alumno *) malloc(cantidad * sizeof(Alumno));
    if (alumnos == NULL)
    {
        printf("Error al reservar memoria\n");
        return 1;
    }

    archivo = fopen("alumnos.csv", "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo alumnos.csv\n");
        free(alumnos);
        return 1;
    }

    primeraLinea = 1;
    i = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL)
        {
        char *token;

        if (primeraLinea) {
            /* Saltamos el encabezado */
            primeraLinea = 0;
            continue;
        }

        linea[strcspn(linea, "\n")] = '\0';

        token = strtok(linea, ",");
        strcpy(alumnos[i].nombre, token);

        token = strtok(NULL, ",");
        strcpy(alumnos[i].apellido, token);

        token = strtok(NULL, ",");
        alumnos[i].promedio = atof(token);

        token = strtok(NULL, ",");
        strcpy(alumnos[i].materia, token);

        i++;
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

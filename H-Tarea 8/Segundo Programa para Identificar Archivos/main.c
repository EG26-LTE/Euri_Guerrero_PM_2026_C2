#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH 512
#define MAX_FILES 100

void mostrarMenu();
int listarTodosLosArchivos(char nombresArchivos[MAX_FILES][MAX_PATH]);
void verificarArchivo(const char *nombreArchivo);

int main() {
    char listaArchivos[MAX_FILES][MAX_PATH];
    int cantidadArchivos = 0;
    int seleccion = 0;
    int opcionMenu = 0;

    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        scanf("%d", &opcionMenu);

        if (opcionMenu == 1) {
            cantidadArchivos = listarTodosLosArchivos(listaArchivos);

            if (cantidadArchivos == 0) {
                printf("\nNo se encontraron archivos en la carpeta 'imagenes/'.\n\n");
                continue;
            }

            printf("\nSeleccione el numero del archivo a verificar (1-%d): ", cantidadArchivos);
            scanf("%d", &seleccion);

            if (seleccion < 1 || seleccion > cantidadArchivos) {
                printf("Seleccion invalida.\n\n");
                continue;
            }

            verificarArchivo(listaArchivos[seleccion - 1]);

        } else if (opcionMenu != 2) {
            printf("Opcion no valida.\n\n");
        }

    } while (opcionMenu != 2);

    printf("Saliendo del verificador. ¡Adios!\n");
    return 0;
}

void mostrarMenu() {
    printf("=========================================\n");
    printf("     VERIFICADOR DE EXTENSIONES          \n");
    printf("=========================================\n");
    printf("1. Listar Archivos y Verificar Contenido\n");
    printf("2. Salir\n");
    printf("=========================================\n");
}

int listarTodosLosArchivos(char nombresArchivos[MAX_FILES][MAX_PATH]) {
    DIR *carpeta;
    struct dirent *entrada;
    int contador = 0;

    carpeta = opendir("imagenes");
    if (carpeta == NULL) {
        printf("La carpeta 'imagenes' no existe. Intenta ejecutar primero el Programa 1.\n");
        return 0;
    }

    printf("\n--- ARCHIVOS EN LA CARPETA ---\n");
    while ((entrada = readdir(carpeta)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        strcpy(nombresArchivos[contador], entrada->d_name);
        printf("%d. %s\n", contador + 1, entrada->d_name);
        contador++;

        if (contador >= MAX_FILES) {
            break;
        }
    }

    closedir(carpeta);
    return contador;
}

void verificarArchivo(const char *nombreArchivo) {
    char rutaCompleta[MAX_PATH];
    sprintf(rutaCompleta, "imagenes/%s", nombreArchivo);

    FILE *archivo = fopen(rutaCompleta, "rb");
    if (archivo == NULL) {
        printf("\nError: No se pudo abrir el archivo %s.\n\n", nombreArchivo);
        return;
    }

    const char *punto = strrchr(nombreArchivo, '.');
    if (!punto) {
        printf("\nResultado: [Formato desconocido] (El archivo no tiene extension en su nombre).\n\n");
        fclose(archivo);
        return;
    }

    unsigned char cabecera[8];
    size_t bytesLeidos = fread(cabecera, 1, 8, archivo);

    if (bytesLeidos < 3) {
        printf("\nResultado: [Archivo corrupto] (Demasiado chico para analizar o vacio).\n\n");
        fclose(archivo);
        return;
    }

    int esBMP = 0;
    int esPNG = 0;
    int esJPG = 0;

    if (bytesLeidos >= 2 && cabecera[0] == 0x42 && cabecera[1] == 0x4D) {
        esBMP = 1;
    }

    if (bytesLeidos >= 8 &&
        cabecera[0] == 0x89 && cabecera[1] == 0x50 && cabecera[2] == 0x4E && cabecera[3] == 0x47 &&
        cabecera[4] == 0x0D && cabecera[5] == 0x0A && cabecera[6] == 0x1A && cabecera[7] == 0x0A) {
        esPNG = 1;
    }

    if (bytesLeidos >= 3 && cabecera[0] == 0xFF && cabecera[1] == 0xD8 && cabecera[2] == 0xFF) {
        esJPG = 1;
    }

    printf("\n--- Analizando: %s ---\n", nombreArchivo);
    printf("Primeros bytes leidos (Hexadecimal): ");
    for (size_t i = 0; i < bytesLeidos; i++) {
        printf("%02X ", cabecera[i]);
    }
    printf("\n");

    if (esJPG) {
        fseek(archivo, -2, SEEK_END);
        unsigned char finDeArchivo[2];
        fread(finDeArchivo, 1, 2, archivo);

        if (finDeArchivo[0] == 0xFF && finDeArchivo[1] == 0xD9) {
            if (strcasecmp(punto, ".jpg") == 0 || strcasecmp(punto, ".jpeg") == 0) {
                printf("Resultado: Archivo valido. Es un JPG real y su extension coincide.\n\n");
            } else {
                printf("Resultado: La extension no coincide con el contenido. El archivo interno es JPG, pero su extension dice ser %s.\n\n", punto);
            }
        } else {
            printf("Resultado: Archivo corrupto. Tiene inicio de JPG pero le falta la firma de cierre FF D9.\n\n");
        }
    }
    else if (esPNG) {
        if (strcasecmp(punto, ".png") == 0) {
            printf("Resultado: Archivo valido. Es un PNG real y su extension coincide.\n\n");
        } else {
            printf("Resultado: La extension no coincide con el contenido. El archivo interno es PNG, pero su extension dice ser %s.\n\n", punto);
        }
    }
    else if (esBMP) {
        if (strcasecmp(punto, ".bmp") == 0) {
            printf("Resultado: Archivo valido. Es un BMP real y su extension coincide.\n\n");
        } else {
            printf("Resultado: La extension no coincide con el contenido. El archivo interno es BMP, pero su extension dice ser %s.\n\n", punto);
        }
    }
    else {
        if (strcasecmp(punto, ".png") == 0 || strcasecmp(punto, ".jpg") == 0 ||
            strcasecmp(punto, ".jpeg") == 0 || strcasecmp(punto, ".bmp") == 0) {
            printf("Resultado: La extension no coincide con el contenido / Corrupto. El nombre dice que es %s pero sus bytes internos no corresponden con ningun formato valido conocido..\n\n", punto);
        } else {
            printf("Resultado: Formato desconocido. Los bytes internos no pertenecen a BMP, PNG o JPG.\n\n");
        }
    }

    fclose(archivo);
}

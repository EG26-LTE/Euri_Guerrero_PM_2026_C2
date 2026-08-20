#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_PATH 512
#define MAX_FILES 100

void mostrarMenu();
int listarImagenes(char nombresArchivos[MAX_FILES][MAX_PATH]);
int tieneExtensionValida(const char *nombreArchivo);
void obtenerNombreSalida(const char *origen, char *destino);
void convertirABlancoYNegro(const char *rutaEntrada, const char *rutaSalida);

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

            cantidadArchivos = listarImagenes(listaArchivos);

            if (cantidadArchivos == 0) {
                printf("\nNo se encontraron imagenes validas en la carpeta 'imagenes/'.\n");
                printf("Asegurate de crear la carpeta y meter archivos .jpg, .png o .bmp\n\n");
                continue;
            }

            printf("\nSeleccione el numero de la imagen a convertir (1-%d): ", cantidadArchivos);
            scanf("%d", &seleccion);

            if (seleccion < 1 || seleccion > cantidadArchivos) {
                printf("Seleccion invalida.\n\n");
                continue;
            }

            char *archivoSeleccionado = listaArchivos[seleccion - 1];
            printf("\nHas seleccionado: %s\n", archivoSeleccionado);

            char rutaEntrada[MAX_PATH];
            char rutaSalida[MAX_PATH];
            char nombreSalida[MAX_PATH];

            sprintf(rutaEntrada, "imagenes/%s", archivoSeleccionado);

            obtenerNombreSalida(archivoSeleccionado, nombreSalida);

            sprintf(rutaSalida, "imagenes/%s", nombreSalida);

            convertirABlancoYNegro(rutaEntrada, rutaSalida);

        } else if (opcionMenu != 2) {
            printf("Opcion no valida.\n\n");
        }

    } while (opcionMenu != 2);

    printf("Saliendo del programa. ¡Adios!\n");
    return 0;
}

void mostrarMenu() {
    printf("=========================================\n");
    printf("     CONVERSOR A BLANCO Y NEGRO          \n");
    printf("=========================================\n");
    printf("1. Ver imagenes disponibles\n");
    printf("2. Salir\n");
    printf("=========================================\n");
}


int tieneExtensionValida(const char *nombreArchivo) {

    const char *punto = strrchr(nombreArchivo, '.');
    if (!punto) {
        return 0;
    }

    if (strcasecmp(punto, ".png") == 0 ||
        strcasecmp(punto, ".jpg") == 0 ||
        strcasecmp(punto, ".jpeg") == 0 ||
        strcasecmp(punto, ".bmp") == 0) {
        return 1;
    }

    return 0;
}

int listarImagenes(char nombresArchivos[MAX_FILES][MAX_PATH]) {
    DIR *carpeta;
    struct dirent *entrada;
    int contador = 0;

    carpeta = opendir("imagenes");
    if (carpeta == NULL) {
        printf("La carpeta 'imagenes' no existe. Creando carpeta 'imagenes/'...\n");
        system("mkdir imagenes");
        carpeta = opendir("imagenes");
        if (carpeta == NULL) {
            printf("Error al abrir o crear la carpeta 'imagenes'.\n");
            return 0;
        }
    }

    printf("\n--- IMAGENES DISPONIBLES ---\n");
    while ((entrada = readdir(carpeta)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        if (tieneExtensionValida(entrada->d_name)) {
            strcpy(nombresArchivos[contador], entrada->d_name);
            printf("%d. %s\n", contador + 1, entrada->d_name);
            contador++;

            if (contador >= MAX_FILES) {
                break;
            }
        }
    }

    closedir(carpeta);
    return contador;
}


void obtenerNombreSalida(const char *origen, char *destino) {
    const char *punto = strrchr(origen, '.');
    if (!punto) {
        strcpy(destino, origen);
        return;
    }

    int longitudNombreBase = punto - origen;

    strncpy(destino, origen, longitudNombreBase);
    destino[longitudNombreBase] = '\0';

    strcat(destino, "_BN");
    strcat(destino, punto);
}

void convertirABlancoYNegro(const char *rutaEntrada, const char *rutaSalida) {
    int ancho, alto, canales;

    printf("Cargando imagen: %s...\n", rutaEntrada);
    unsigned char *pixeles = stbi_load(rutaEntrada, &ancho, &alto, &canales, 3);

    if (pixeles == NULL) {
        printf("Error: No se pudo cargar o procesar la imagen.\n\n");
        return;
    }

    printf("Dimensiones: %dx%d pixeles. Canales originales: %d.\n", ancho, alto, canales);
    printf("Procesando conversion...\n");

    int totalPixeles = ancho * alto;
    for (int i = 0; i < totalPixeles; i++) {
        int r = pixeles[i * 3];
        int g = pixeles[i * 3 + 1];
        int b = pixeles[i * 3 + 2];

        int gris = (r + g + b) / 3;

        pixeles[i * 3] = (unsigned char)gris;     // R
        pixeles[i * 3 + 1] = (unsigned char)gris; // G
        pixeles[i * 3 + 2] = (unsigned char)gris; // B
    }

    const char *ext = strrchr(rutaSalida, '.');
    int exito = 0;

    if (strcasecmp(ext, ".png") == 0) {
        exito = stbi_write_png(rutaSalida, ancho, alto, 3, pixeles, ancho * 3);
    } else if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) {
        exito = stbi_write_jpg(rutaSalida, ancho, alto, 3, pixeles, 90);
    } else if (strcasecmp(ext, ".bmp") == 0) {
        exito = stbi_write_bmp(rutaSalida, ancho, alto, 3, pixeles);
    }

    if (exito) {
        printf("¡Exito! Imagen convertida y guardada en: %s\n\n", rutaSalida);
    } else {
        printf("Error: No se pudo escribir/guardar la imagen de salida.\n\n");
    }
    stbi_image_free(pixeles);
}

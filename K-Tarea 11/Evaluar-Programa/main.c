#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "cJSON.h"

/* =========================================================================
 * ESTRUCTURAS DE DATOS
 * ========================================================================= */

typedef struct {
    char *nombre;
    char *descripcion;
    int funcion;        /* F: 1 a 5 */
    int sustitucion;    /* S: 1 a 5 */
    int profundidad;    /* P: 1 a 5 */
    int extension;      /* E: 1 a 5 */
    int agresion;       /* A: 1 a 5 */
    int vulnerabilidad; /* V: 1 a 5 */
} Riesgo;

typedef struct {
    char *nombre;
    char *industria;
    char *descripcion;
    char *archivoJSON;
    Riesgo *riesgos;
    int cantidadRiesgos;
} Empresa;

/* =========================================================================
 * DECLARACIÓN DE FUNCIONES (PROTOTIPOS)
 * ========================================================================= */

/* Entrada y Validación */
void limpiarBufferEntrada(void);
char *leerCadenaDinamica(const char *mensaje);
int leerEnteroRango(const char *mensaje, int min, int max);
int solicitarCriterio(const char *titulo, const char *explicacion, const char *opciones[5]);

/* Memoria y Limpieza */
char *duplicarCadena(const char *src);
void liberarRiesgoContenido(Riesgo *r);
void liberarEmpresaContenido(Empresa *emp);
void liberarListaEmpresas(Empresa **empresas, int *cantidad);
void liberarListaRiesgos(Riesgo **riesgos, int *cantidad);

/* Método Mosler */
int calcularImportancia(int f, int s);
int calcularDanios(int p, int e);
int calcularCaracter(int i, int d);
int calcularProbabilidad(int a, int v);
int calcularCuantificacionER(int c, int pb);
const char *obtenerClasificacionER(int er);
void mostrarCalculoMosler(const Riesgo *r);

/* Archivos y cJSON */
int guardarEmpresaJSON(const Empresa *emp);
Empresa *cargarEmpresaJSON(const char *nombreArchivo);
char *generarNombreArchivo(const char *nombreEmpresa);
int estaEmpresaCargada(Empresa *empresas, int cantEmpresas, const char *archivoJSON);
void cargarEmpresasDelDirectorio(Empresa **empresas, int *cantEmpresas);

/* Gestión de Empresas y Riesgos */
int agregarEmpresaColeccion(Empresa **empresas, int *cantidad, Empresa nuevaEmpresa);
int eliminarEmpresaColeccion(Empresa **empresas, int *cantidad, int indice);
int agregarRiesgoAEmpresa(Empresa *emp, Riesgo r);
int eliminarRiesgoDeEmpresa(Empresa *emp, int indice);
int agregarRiesgoAIndiv(Riesgo **riesgos, int *cantidad, Riesgo r);
int eliminarRiesgoDeIndiv(Riesgo **riesgos, int *cantidad, int indice);

/* Formularios y Menús */
Riesgo capturarDatosRiesgo(void);
void modificarRiesgoSecuencial(Riesgo *r);
void menuCrearRiesgo(Empresa **empresas, int *cantEmpresas, Riesgo **indiv, int *cantIndiv);
void menuModificarRiesgo(Empresa *empresas, int cantEmpresas, Riesgo *indiv, int cantIndiv);
void menuBorrarRiesgo(Empresa **empresas, int *cantEmpresas, Riesgo **indiv, int *cantIndiv);
void menuCalcularRiesgos(Empresa *empresas, int cantEmpresas, Riesgo *indiv, int cantIndiv);
void menuCrearEmpresa(Empresa **empresas, int *cantEmpresas);
void menuVerEmpresas(Empresa **empresas, int *cantEmpresas);
void menuBorrarEmpresa(Empresa **empresas, int *cantEmpresas);

/* =========================================================================
 * ENTRADA Y VALIDACIÓN
 * ========================================================================= */

void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char *leerCadenaDinamica(const char *mensaje) {
    char buffer[1024];
    if (mensaje != NULL && strlen(mensaje) > 0) {
        printf("%s", mensaje);
    }
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }

    size_t len = strlen(buffer);
    if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        buffer[len - 1] = '\0';
        len--;
    }
    if (len > 0 && buffer[len - 1] == '\r') {
        buffer[len - 1] = '\0';
        len--;
    }

    char *cadena = (char *)malloc(len + 1);
    if (cadena == NULL) {
        printf("Error: Fallo de asignacion de memoria para la cadena.\n");
        return NULL;
    }
    strcpy(cadena, buffer);
    return cadena;
}

int leerEnteroRango(const char *mensaje, int min, int max) {
    char buffer[256];
    long valor;
    char *endptr;

    while (1) {
        if (mensaje != NULL && strlen(mensaje) > 0) {
            printf("%s", mensaje);
        }
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;

        if (strlen(buffer) == 0) {
            printf(">> Entrada vacia. Por favor ingrese un numero entre %d y %d.\n", min, max);
            continue;
        }

        valor = strtol(buffer, &endptr, 10);

        if (*endptr != '\0') {
            printf(">> Entrada invalida. Ingrese unicamente numeros enteros sin caracteres adicionales.\n");
            continue;
        }

        if (valor < min || valor > max) {
            printf(">> El numero debe estar dentro del rango permitido [%d - %d].\n", min, max);
            continue;
        }

        return (int)valor;
    }
}

int solicitarCriterio(const char *titulo, const char *explicacion, const char *opciones[5]) {
    printf("\n========================================\n");
    printf(" %s\n", titulo);
    printf("========================================\n");
    printf("%s\n\n", explicacion);
    printf("Seleccione un numero de la escala:\n\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, opciones[i]);
    }
    return leerEnteroRango("\nSeleccion: ", 1, 5);
}

/* =========================================================================
 * MEMORIA Y LIMPIEZA
 * ========================================================================= */

char *duplicarCadena(const char *src) {
    if (src == NULL) return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (dest != NULL) {
        strcpy(dest, src);
    }
    return dest;
}

void liberarRiesgoContenido(Riesgo *r) {
    if (r == NULL) return;
    if (r->nombre != NULL) {
        free(r->nombre);
        r->nombre = NULL;
    }
    if (r->descripcion != NULL) {
        free(r->descripcion);
        r->descripcion = NULL;
    }
}

void liberarEmpresaContenido(Empresa *emp) {
    if (emp == NULL) return;
    if (emp->nombre) free(emp->nombre);
    if (emp->industria) free(emp->industria);
    if (emp->descripcion) free(emp->descripcion);
    if (emp->archivoJSON) free(emp->archivoJSON);

    if (emp->riesgos != NULL) {
        for (int i = 0; i < emp->cantidadRiesgos; i++) {
            liberarRiesgoContenido(&emp->riesgos[i]);
        }
        free(emp->riesgos);
        emp->riesgos = NULL;
    }
    emp->cantidadRiesgos = 0;
}

void liberarListaEmpresas(Empresa **empresas, int *cantidad) {
    if (empresas == NULL || *empresas == NULL) return;
    for (int i = 0; i < *cantidad; i++) {
        liberarEmpresaContenido(&((*empresas)[i]));
    }
    free(*empresas);
    *empresas = NULL;
    *cantidad = 0;
}

void liberarListaRiesgos(Riesgo **riesgos, int *cantidad) {
    if (riesgos == NULL || *riesgos == NULL) return;
    for (int i = 0; i < *cantidad; i++) {
        liberarRiesgoContenido(&((*riesgos)[i]));
    }
    free(*riesgos);
    *riesgos = NULL;
    *cantidad = 0;
}

/* =========================================================================
 * MÉTODO MOSLER
 * ========================================================================= */

int calcularImportancia(int f, int s) { return f * s; }
int calcularDanios(int p, int e) { return p * e; }
int calcularCaracter(int i, int d) { return i + d; }
int calcularProbabilidad(int a, int v) { return a * v; }
int calcularCuantificacionER(int c, int pb) { return c * pb; }

const char *obtenerClasificacionER(int er) {
    if (er >= 1 && er <= 250) {
        return "Existen muy bajas probabilidades de que su riesgo se concrete,\ny de concretarse, representa un daño casi nulo para usted o su empresa.";
    } else if (er >= 251 && er <= 500) {
        return "Existen bajas probabilidades de que su riesgo se concrete,\ny de concretarse, representa un daño no muy significante para usted o su empresa.";
    } else if (er >= 501 && er <= 750) {
        return "Existen probabilidades de que su riesgo se concrete,\nde concretarse, representa un daño perceptible para usted o su empresa.";
    } else if (er >= 751 && er <= 1000) {
        return "Existen altas probabilidades de que su riesgo se concrete,\ny de concretarse, representa un daño significativo para usted o su empresa.";
    } else if (er >= 1001 && er <= 1250) {
        return "Existen altisimas probabilidades de que su riesgo se concrete,\ny de concretarse, representa un daño muy grave para usted o su empresa.";
    }
    return "Valor de cuantificacion fuera del rango del Método Mosler.";
}

void mostrarCalculoMosler(const Riesgo *r) {
    int I = calcularImportancia(r->funcion, r->sustitucion);
    int D = calcularDanios(r->profundidad, r->extension);
    int C = calcularCaracter(I, D);
    int Pb = calcularProbabilidad(r->agresion, r->vulnerabilidad);
    int ER = calcularCuantificacionER(C, Pb);

    printf("\n========================================\n");
    printf("         RESULTADO DEL RIESGO\n");
    printf("========================================\n\n");
    printf("Riesgo: %s\n", r->nombre ? r->nombre : "N/A");
    printf("Descripcion: %s\n\n", r->descripcion ? r->descripcion : "N/A");
    printf("Importancia del suceso (I = F x S): %d\n", I);
    printf("Daños ocasionados por el suceso (D = P x E): %d\n", D);
    printf("Caracter del riesgo (C = I + D): %d\n", C);
    printf("Probabilidad del riesgo (Pb = A x V): %d\n", Pb);
    printf("Cuantificacion del riesgo (ER = C x Pb): %d\n\n", ER);
    printf("Clasificacion:\n%s\n", obtenerClasificacionER(ER));
    printf("========================================\n");
}

/* =========================================================================
 * ARCHIVOS Y cJSON
 * ========================================================================= */

char *generarNombreArchivo(const char *nombreEmpresa) {
    if (nombreEmpresa == NULL) return duplicarCadena("empresa.json");

    size_t len = strlen(nombreEmpresa);
    char *buffer = (char *)malloc(len + 10);
    if (buffer == NULL) return duplicarCadena("empresa.json");

    int j = 0;
    for (size_t i = 0; i < len; i++) {
        char c = nombreEmpresa[i];
        if (isalnum((unsigned char)c) || c == '_' || c == '-') {
            buffer[j++] = c;
        } else if (c == ' ') {
            buffer[j++] = '_';
        }
    }
    buffer[j] = '\0';

    if (strlen(buffer) == 0) {
        strcpy(buffer, "empresa");
    }

    strcat(buffer, ".json");
    return buffer;
}

int guardarEmpresaJSON(const Empresa *emp) {
    if (emp == NULL || emp->archivoJSON == NULL) return 0;

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) return 0;

    cJSON_AddStringToObject(root, "nombre", emp->nombre ? emp->nombre : "");
    cJSON_AddStringToObject(root, "industria", emp->industria ? emp->industria : "");
    cJSON_AddStringToObject(root, "descripcion", emp->descripcion ? emp->descripcion : "");

    cJSON *riesgosArray = cJSON_CreateArray();
    if (riesgosArray == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    for (int i = 0; i < emp->cantidadRiesgos; i++) {
        cJSON *rObj = cJSON_CreateObject();
        cJSON_AddStringToObject(rObj, "nombre", emp->riesgos[i].nombre ? emp->riesgos[i].nombre : "");
        cJSON_AddStringToObject(rObj, "descripcion", emp->riesgos[i].descripcion ? emp->riesgos[i].descripcion : "");
        cJSON_AddNumberToObject(rObj, "funcion", emp->riesgos[i].funcion);
        cJSON_AddNumberToObject(rObj, "sustitucion", emp->riesgos[i].sustitucion);
        cJSON_AddNumberToObject(rObj, "profundidad", emp->riesgos[i].profundidad);
        cJSON_AddNumberToObject(rObj, "extension", emp->riesgos[i].extension);
        cJSON_AddNumberToObject(rObj, "agresion", emp->riesgos[i].agresion);
        cJSON_AddNumberToObject(rObj, "vulnerabilidad", emp->riesgos[i].vulnerabilidad);

        int I = calcularImportancia(emp->riesgos[i].funcion, emp->riesgos[i].sustitucion);
        int D = calcularDanios(emp->riesgos[i].profundidad, emp->riesgos[i].extension);
        int C = calcularCaracter(I, D);
        int Pb = calcularProbabilidad(emp->riesgos[i].agresion, emp->riesgos[i].vulnerabilidad);
        int ER = calcularCuantificacionER(C, Pb);

        cJSON_AddNumberToObject(rObj, "importancia", I);
        cJSON_AddNumberToObject(rObj, "danios", D);
        cJSON_AddNumberToObject(rObj, "caracter", C);
        cJSON_AddNumberToObject(rObj, "probabilidad", Pb);
        cJSON_AddNumberToObject(rObj, "cuantificacion", ER);

        cJSON_AddItemToArray(riesgosArray, rObj);
    }

    cJSON_AddItemToObject(root, "riesgos", riesgosArray);

    char *jsonStr = cJSON_Print(root);
    if (jsonStr == NULL) {
        cJSON_Delete(root);
        return 0;
    }

    FILE *f = fopen(emp->archivoJSON, "w");
    if (f == NULL) {
        printf(">> Error: No se pudo abrir el archivo '%s' para escribir.\n", emp->archivoJSON);
        free(jsonStr);
        cJSON_Delete(root);
        return 0;
    }

    fputs(jsonStr, f);
    fclose(f);

    free(jsonStr);
    cJSON_Delete(root);
    return 1;
}

Empresa *cargarEmpresaJSON(const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "rb");
    if (f == NULL) return NULL;

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (length <= 0) {
        fclose(f);
        return NULL;
    }

    char *data = (char *)malloc(length + 1);
    if (data == NULL) {
        fclose(f);
        return NULL;
    }

    size_t leidos = fread(data, 1, length, f);
    data[leidos] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(data);
    free(data);

    if (root == NULL) {
        return NULL;
    }

    Empresa *emp = (Empresa *)malloc(sizeof(Empresa));
    if (emp == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    emp->nombre = NULL;
    emp->industria = NULL;
    emp->descripcion = NULL;
    emp->archivoJSON = duplicarCadena(nombreArchivo);
    emp->riesgos = NULL;
    emp->cantidadRiesgos = 0;

    cJSON *jNombre = cJSON_GetObjectItemCaseSensitive(root, "nombre");
    cJSON *jIndustria = cJSON_GetObjectItemCaseSensitive(root, "industria");
    cJSON *jDesc = cJSON_GetObjectItemCaseSensitive(root, "descripcion");

    emp->nombre = (cJSON_IsString(jNombre) && jNombre->valuestring) ? duplicarCadena(jNombre->valuestring) : duplicarCadena("Empresa Sin Nombre");
    emp->industria = (cJSON_IsString(jIndustria) && jIndustria->valuestring) ? duplicarCadena(jIndustria->valuestring) : duplicarCadena("General");
    emp->descripcion = (cJSON_IsString(jDesc) && jDesc->valuestring) ? duplicarCadena(jDesc->valuestring) : duplicarCadena("Sin descripción");

    cJSON *jRiesgos = cJSON_GetObjectItemCaseSensitive(root, "riesgos");
    if (cJSON_IsArray(jRiesgos)) {
        int count = cJSON_GetArraySize(jRiesgos);
        for (int i = 0; i < count; i++) {
            cJSON *item = cJSON_GetArrayItem(jRiesgos, i);
            if (cJSON_IsObject(item)) {
                Riesgo r;
                cJSON *rNom = cJSON_GetObjectItemCaseSensitive(item, "nombre");
                cJSON *rDesc = cJSON_GetObjectItemCaseSensitive(item, "descripcion");
                cJSON *rF = cJSON_GetObjectItemCaseSensitive(item, "funcion");
                cJSON *rS = cJSON_GetObjectItemCaseSensitive(item, "sustitucion");
                cJSON *rP = cJSON_GetObjectItemCaseSensitive(item, "profundidad");
                cJSON *rE = cJSON_GetObjectItemCaseSensitive(item, "extension");
                cJSON *rA = cJSON_GetObjectItemCaseSensitive(item, "agresion");
                cJSON *rV = cJSON_GetObjectItemCaseSensitive(item, "vulnerabilidad");

                r.nombre = (cJSON_IsString(rNom) && rNom->valuestring) ? duplicarCadena(rNom->valuestring) : duplicarCadena("Riesgo");
                r.descripcion = (cJSON_IsString(rDesc) && rDesc->valuestring) ? duplicarCadena(rDesc->valuestring) : duplicarCadena("");
                r.funcion = cJSON_IsNumber(rF) ? rF->valueint : 1;
                r.sustitucion = cJSON_IsNumber(rS) ? rS->valueint : 1;
                r.profundidad = cJSON_IsNumber(rP) ? rP->valueint : 1;
                r.extension = cJSON_IsNumber(rE) ? rE->valueint : 1;
                r.agresion = cJSON_IsNumber(rA) ? rA->valueint : 1;
                r.vulnerabilidad = cJSON_IsNumber(rV) ? rV->valueint : 1;

                agregarRiesgoAEmpresa(emp, r);
            }
        }
    }

    cJSON_Delete(root);
    return emp;
}

int estaEmpresaCargada(Empresa *empresas, int cantEmpresas, const char *archivoJSON) {
    for (int i = 0; i < cantEmpresas; i++) {
        if (empresas[i].archivoJSON != NULL && strcmp(empresas[i].archivoJSON, archivoJSON) == 0) {
            return 1;
        }
    }
    return 0;
}

void cargarEmpresasDelDirectorio(Empresa **empresas, int *cantEmpresas) {
    DIR *dir = opendir(".");
    if (dir == NULL) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        size_t len = strlen(entry->d_name);
        if (len > 5 && strcmp(entry->d_name + len - 5, ".json") == 0) {
            if (!estaEmpresaCargada(*empresas, *cantEmpresas, entry->d_name)) {
                Empresa *emp = cargarEmpresaJSON(entry->d_name);
                if (emp != NULL) {
                    agregarEmpresaColeccion(empresas, cantEmpresas, *emp);
                    free(emp);
                }
            }
        }
    }
    closedir(dir);
}

/* =========================================================================
 * GESTIÓN DINÁMICA DE EMPRESAS Y RIESGOS
 * ========================================================================= */

int agregarEmpresaColeccion(Empresa **empresas, int *cantidad, Empresa nuevaEmpresa) {
    Empresa *temp = (Empresa *)realloc(*empresas, (*cantidad + 1) * sizeof(Empresa));
    if (temp == NULL) {
        printf(">> Error: No se pudo reasignar memoria para la lista de empresas.\n");
        return 0;
    }
    *empresas = temp;
    (*empresas)[*cantidad] = nuevaEmpresa;
    (*cantidad)++;
    return 1;
}

int eliminarEmpresaColeccion(Empresa **empresas, int *cantidad, int indice) {
    if (indice < 0 || indice >= *cantidad) return 0;

    liberarEmpresaContenido(&((*empresas)[indice]));

    for (int i = indice; i < *cantidad - 1; i++) {
        (*empresas)[i] = (*empresas)[i + 1];
    }

    (*cantidad)--;

    if (*cantidad == 0) {
        free(*empresas);
        *empresas = NULL;
    } else {
        Empresa *temp = (Empresa *)realloc(*empresas, (*cantidad) * sizeof(Empresa));
        if (temp != NULL) {
            *empresas = temp;
        }
    }
    return 1;
}

int agregarRiesgoAEmpresa(Empresa *emp, Riesgo r) {
    Riesgo *temp = (Riesgo *)realloc(emp->riesgos, (emp->cantidadRiesgos + 1) * sizeof(Riesgo));
    if (temp == NULL) {
        printf(">> Error: No se pudo reasignar memoria para la lista de riesgos.\n");
        return 0;
    }
    emp->riesgos = temp;
    emp->riesgos[emp->cantidadRiesgos] = r;
    emp->cantidadRiesgos++;
    return 1;
}

int eliminarRiesgoDeEmpresa(Empresa *emp, int indice) {
    if (indice < 0 || indice >= emp->cantidadRiesgos) return 0;

    liberarRiesgoContenido(&(emp->riesgos[indice]));

    for (int i = indice; i < emp->cantidadRiesgos - 1; i++) {
        emp->riesgos[i] = emp->riesgos[i + 1];
    }

    emp->cantidadRiesgos--;

    if (emp->cantidadRiesgos == 0) {
        free(emp->riesgos);
        emp->riesgos = NULL;
    } else {
        Riesgo *temp = (Riesgo *)realloc(emp->riesgos, emp->cantidadRiesgos * sizeof(Riesgo));
        if (temp != NULL) {
            emp->riesgos = temp;
        }
    }
    return 1;
}

int agregarRiesgoAIndiv(Riesgo **riesgos, int *cantidad, Riesgo r) {
    Riesgo *temp = (Riesgo *)realloc(*riesgos, (*cantidad + 1) * sizeof(Riesgo));
    if (temp == NULL) {
        printf(">> Error: No se pudo asignar memoria para el riesgo individual.\n");
        return 0;
    }
    *riesgos = temp;
    (*riesgos)[*cantidad] = r;
    (*cantidad)++;
    return 1;
}

int eliminarRiesgoDeIndiv(Riesgo **riesgos, int *cantidad, int indice) {
    if (indice < 0 || indice >= *cantidad) return 0;

    liberarRiesgoContenido(&((*riesgos)[indice]));

    for (int i = indice; i < *cantidad - 1; i++) {
        (*riesgos)[i] = (*riesgos)[i + 1];
    }

    (*cantidad)--;

    if (*cantidad == 0) {
        free(*riesgos);
        *riesgos = NULL;
    } else {
        Riesgo *temp = (Riesgo *)realloc(*riesgos, (*cantidad) * sizeof(Riesgo));
        if (temp != NULL) {
            *riesgos = temp;
        }
    }
    return 1;
}

/* =========================================================================
 * FORMULARIOS Y MENÚS
 * ========================================================================= */

Riesgo capturarDatosRiesgo(void) {
    Riesgo r;
    printf("\n--- DATOS BASICOS DEL RIESGO ---\n");
    r.nombre = leerCadenaDinamica("Nombre del riesgo: ");
    r.descripcion = leerCadenaDinamica("Descripción del riesgo: ");

    printf("\nA continuacion evaluaremos el riesgo...\n");

    const char *optF[5] = {
        "Muy Levemente", "Levemente",
        "Medianamente", "Gravemente", "Muy Gravemente"
    };
    r.funcion = solicitarCriterio(
        "Primer Criterio – Funcion del riesgo (F)",
        "El riesgo puede tener consecuencias o danos sobre las actividades principales.",
        optF
    );

    const char *optS[5] = {
        "Muy Facilmente", "Facilmente",
        "Sin mucha dificultad", "Dificilmente", "Muy Dificilmente"
    };
    r.sustitucion = solicitarCriterio(
        "Segundo Criterio – Sustitucion de los danos (S)",
        "Los bienes danados en caso de concretarse el riesgo, ¿pueden ser reemplazados?\nTOME EN CONSIDERACION EL COSTO Y EL TIEMPO.",
        optS
    );

    const char *optP[5] = {
        "Perturbaciones Muy leves", "Perturbaciones Leves",
        "Perturbaciones Limitadas", "Perturbaciones Graves", "Perturbaciones Muy Graves"
    };
    r.profundidad = solicitarCriterio(
        "Tercer Criterio – Profundidad (P)",
        "Valore las perturbaciones o efectos psicologicos que pueden ocasionar el riesgo en caso de producirse:",
        optP
    );

    const char *optE[5] = {
        "Danos Individuales", "Danos Locales",
        "Danos Regionales", "Danos Nacionales", "Danos Internacionales"
    };
    r.extension = solicitarCriterio(
        "Cuarto Criterio – Extension de los danos (E)",
        "Identifique que tanto pueden alcanzar los danos de concretarse el riesgo:",
        optE
    );

    const char *optA[5] = {
        "Muy Baja", "Baja", "Normal", "Alta", "Muy Alta"
    };
    r.agresion = solicitarCriterio(
        "Quinto Criterio – Agresion (A)",
        "A continuacion, evalue la probabilidad de que el riesgo se materialice:",
        optA
    );

    const char *optV[5] = {
        "Muy Baja", "Baja", "Normal", "Alta", "Muy Alta"
    };
    r.vulnerabilidad = solicitarCriterio(
        "Sexto Criterio – Vulnerabilidad (V)",
        "Establezca la PROBABILIDAD de que se produzcan perdidas o danos.\nTome en cuenta el costo de las perdidas o danos:",
        optV
    );

    return r;
}

void modificarRiesgoSecuencial(Riesgo *r) {
    if (r == NULL) return;

    printf("\n========================================\n");
    printf("         MODIFICAR - Nombre del riesgo\n");
    printf("========================================\n");
    printf("Actual: %s\n", r->nombre ? r->nombre : "N/A");
    printf("Ingrese el nuevo nombre (o 0 para NO MODIFICAR): ");
    char *nuevoNom = leerCadenaDinamica("");
    if (nuevoNom != NULL) {
        if (strcmp(nuevoNom, "0") != 0 && strlen(nuevoNom) > 0) {
            free(r->nombre);
            r->nombre = nuevoNom;
        } else {
            free(nuevoNom);
        }
    }

    printf("\n========================================\n");
    printf("      MODIFICAR - Descripcion del riesgo\n");
    printf("========================================\n");
    printf("Actual: %s\n", r->descripcion ? r->descripcion : "N/A");
    printf("Ingrese la nueva descripcion (o 0 para NO MODIFICAR): ");
    char *nuevaDesc = leerCadenaDinamica("");
    if (nuevaDesc != NULL) {
        if (strcmp(nuevaDesc, "0") != 0 && strlen(nuevaDesc) > 0) {
            free(r->descripcion);
            r->descripcion = nuevaDesc;
        } else {
            free(nuevaDesc);
        }
    }

    const char *optF[5] = {"Muy Levemente graves", "Levemente Grave", "Medianamente Grave", "Grave", "Muy Grave"};
    printf("\nMODIFICAR - Primer Criterio – Funcion (Actual: %d)\n", r->funcion);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optF[i]);
    printf("0. NO MODIFICAR\n");
    int valF = leerEnteroRango("Opción: ", 0, 5);
    if (valF != 0) r->funcion = valF;

    const char *optS[5] = {"Muy Facilmente", "Facilmente", "Sin mucha dificultad", "Dificilmente", "Muy Dificilmente"};
    printf("\nMODIFICAR - Segundo Criterio – Sustitucion (Actual: %d)\n", r->sustitucion);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optS[i]);
    printf("0. NO MODIFICAR\n");
    int valS = leerEnteroRango("Opción: ", 0, 5);
    if (valS != 0) r->sustitucion = valS;

    const char *optP[5] = {"Perturbaciones Muy leves", "Perturbaciones Leves", "Perturbaciones Limitadas", "Perturbaciones Graves", "Perturbaciones Muy Graves"};
    printf("\nMODIFICAR - Tercer Criterio – Profundidad (Actual: %d)\n", r->profundidad);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optP[i]);
    printf("0. NO MODIFICAR\n");
    int valP = leerEnteroRango("Opción: ", 0, 5);
    if (valP != 0) r->profundidad = valP;

    const char *optE[5] = {"Danos Individuales", "Danos Locales", "Danos Regionales", "Danos Nacionales", "Danos Internacionales"};
    printf("\nMODIFICAR - Cuarto Criterio – Extension (Actual: %d)\n", r->extension);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optE[i]);
    printf("0. NO MODIFICAR\n");
    int valE = leerEnteroRango("Opción: ", 0, 5);
    if (valE != 0) r->extension = valE;

    const char *optA[5] = {"Muy Baja", "Baja", "Normal", "Alta", "Muy Alta"};
    printf("\nMODIFICAR - Quinto Criterio – Agresion (Actual: %d)\n", r->agresion);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optA[i]);
    printf("0. NO MODIFICAR\n");
    int valA = leerEnteroRango("Opción: ", 0, 5);
    if (valA != 0) r->agresion = valA;

    const char *optV[5] = {"Muy Baja", "Baja", "Normal", "Alta", "Muy Alta"};
    printf("\nMODIFICAR - Sexto Criterio – Vulnerabilidad (Actual: %d)\n", r->vulnerabilidad);
    for (int i = 0; i < 5; i++) printf("%d. %s\n", i + 1, optV[i]);
    printf("0. NO MODIFICAR\n");
    int valV = leerEnteroRango("Opcion: ", 0, 5);
    if (valV != 0) r->vulnerabilidad = valV;

    printf("\n>> Modificación completada exitosamente.\n");
}

void menuCrearRiesgo(Empresa **empresas, int *cantEmpresas, Riesgo **indiv, int *cantIndiv) {
    printf("\n========================================\n");
    printf("              CREAR RIESGO\n");
    printf("========================================\n");
    printf("1. Riesgo Individual\n");
    printf("2. Crear Riesgo Para una Empresa\n");
    printf("3. Crear Riesgo Para un Archivo\n");
    printf("0. Volver\n");

    int opc = leerEnteroRango("Seleccione una opcion: ", 0, 3);
    if (opc == 0) return;

    if (opc == 1) {
        Riesgo r = capturarDatosRiesgo();
        if (agregarRiesgoAIndiv(indiv, cantIndiv, r)) {
            printf("\n>> Riesgo individual creado en memoria con exito.\n");
        }
    } else if (opc == 2) {
        if (*cantEmpresas == 0) {
            printf("\n>> No hay empresas cargadas. Cree una primero (Opcion 5).\n");
            return;
        }
        printf("\n--- Seleccione la empresa ---\n");
        for (int i = 0; i < *cantEmpresas; i++) {
            printf("%d. %s (%s)\n", i + 1, (*empresas)[i].nombre, (*empresas)[i].archivoJSON);
        }
        int idx = leerEnteroRango("Empresa #: ", 1, *cantEmpresas) - 1;
        Riesgo r = capturarDatosRiesgo();
        if (agregarRiesgoAEmpresa(&((*empresas)[idx]), r)) {
            if (guardarEmpresaJSON(&((*empresas)[idx]))) {
                printf("\n>> Riesgo agregado a la empresa y JSON actualizado.\n");
            }
        }
    } else if (opc == 3) {
        char *nomArch = leerCadenaDinamica("Nombre del archivo JSON (ej: riesgos_mantenimiento.json): ");
        if (nomArch == NULL || strlen(nomArch) == 0) {
            free(nomArch);
            printf(">> Nombre de archivo inválido.\n");
            return;
        }

        Empresa *empExistente = cargarEmpresaJSON(nomArch);
        if (empExistente == NULL) {
            Empresa nuevaEmp;
            nuevaEmp.nombre = duplicarCadena("Archivo Independiente");
            nuevaEmp.industria = duplicarCadena("Varios");
            nuevaEmp.descripcion = duplicarCadena("Contenedor de riesgos individuales por archivo");
            nuevaEmp.archivoJSON = nomArch;
            nuevaEmp.riesgos = NULL;
            nuevaEmp.cantidadRiesgos = 0;

            Riesgo r = capturarDatosRiesgo();
            agregarRiesgoAEmpresa(&nuevaEmp, r);
            if (guardarEmpresaJSON(&nuevaEmp)) {
                agregarEmpresaColeccion(empresas, cantEmpresas, nuevaEmp);
                printf("\n>> Archivo creado y riesgo guardado correctamente.\n");
            } else {
                liberarEmpresaContenido(&nuevaEmp);
            }
        } else {
            free(nomArch);
            Riesgo r = capturarDatosRiesgo();
            agregarRiesgoAEmpresa(empExistente, r);
            if (guardarEmpresaJSON(empExistente)) {
                agregarEmpresaColeccion(empresas, cantEmpresas, *empExistente);
                free(empExistente);
                printf("\n>> Riesgo agregado al archivo existente y guardado.\n");
            } else {
                liberarEmpresaContenido(empExistente);
                free(empExistente);
            }
        }
    }
}

void menuModificarRiesgo(Empresa *empresas, int cantEmpresas, Riesgo *indiv, int cantIndiv) {
    printf("\n========================================\n");
    printf("             MODIFICAR RIESGO\n");
    printf("========================================\n");
    printf("1. Modificar un riesgo individual\n");
    printf("2. Modificar el riesgo para una empresa\n");
    printf("0. Volver\n");

    int opc = leerEnteroRango("Seleccione una opcion: ", 0, 2);
    if (opc == 0) return;

    if (opc == 1) {
        if (cantIndiv == 0) {
            printf("\n>> No hay riesgos individuales guardados.\n");
            return;
        }
        printf("\n--- Riesgos Individuales ---\n");
        for (int i = 0; i < cantIndiv; i++) {
            printf("%d. %s\n", i + 1, indiv[i].nombre);
        }
        int idx = leerEnteroRango("Seleccione el riesgo: ", 1, cantIndiv) - 1;
        modificarRiesgoSecuencial(&(indiv[idx]));
    } else if (opc == 2) {
        if (cantEmpresas == 0) {
            printf("\n>> No hay empresas cargadas.\n");
            return;
        }
        printf("\n--- Seleccione la Empresa ---\n");
        for (int i = 0; i < cantEmpresas; i++) {
            printf("%d. %s\n", i + 1, empresas[i].nombre);
        }
        int idxE = leerEnteroRango("Empresa #: ", 1, cantEmpresas) - 1;

        if (empresas[idxE].cantidadRiesgos == 0) {
            printf("\n>> La empresa seleccionada no posee riesgos registrados.\n");
            return;
        }

        printf("\n--- Seleccione el Riesgo a Modificar ---\n");
        for (int i = 0; i < empresas[idxE].cantidadRiesgos; i++) {
            printf("%d. %s\n", i + 1, empresas[idxE].riesgos[i].nombre);
        }
        int idxR = leerEnteroRango("Riesgo #: ", 1, empresas[idxE].cantidadRiesgos) - 1;

        modificarRiesgoSecuencial(&(empresas[idxE].riesgos[idxR]));
        if (guardarEmpresaJSON(&(empresas[idxE]))) {
            printf("\n>> Archivo JSON actualizado con los cambios del riesgo.\n");
        }
    }
}

void menuBorrarRiesgo(Empresa **empresas, int *cantEmpresas, Riesgo **indiv, int *cantIndiv) {
    printf("\n========================================\n");
    printf("              BORRAR RIESGO\n");
    printf("========================================\n");
    printf("1. Riesgo Individual\n");
    printf("2. Riesgo Agrupado (Empresa/Archivo)\n");
    printf("0. Volver\n");

    int opc = leerEnteroRango("Seleccione una opcion: ", 0, 2);
    if (opc == 0) return;

    if (opc == 1) {
        if (*cantIndiv == 0) {
            printf("\n>> No hay riesgos individuales registrados.\n");
            return;
        }
        for (int i = 0; i < *cantIndiv; i++) {
            printf("%d. %s\n", i + 1, (*indiv)[i].nombre);
        }
        int idx = leerEnteroRango("Seleccione riesgo a borrar: ", 1, *cantIndiv) - 1;

        printf("\n¿Esta seguro de que desea borrar este riesgo?\n1. Si\n2. No\n");
        if (leerEnteroRango("Confirmación: ", 1, 2) == 1) {
            eliminarRiesgoDeIndiv(indiv, cantIndiv, idx);
            printf(">> Riesgo individual eliminado exitosamente.\n");
        } else {
            printf(">> Operacion cancelada.\n");
        }
    } else if (opc == 2) {
        if (*cantEmpresas == 0) {
            printf("\n>> No hay empresas cargadas.\n");
            return;
        }
        printf("\n--- Seleccione la Empresa ---\n");
        for (int i = 0; i < *cantEmpresas; i++) {
            printf("%d. %s\n", i + 1, (*empresas)[i].nombre);
        }
        int idxE = leerEnteroRango("Empresa #: ", 1, *cantEmpresas) - 1;

        if ((*empresas)[idxE].cantidadRiesgos == 0) {
            printf("\n>> La empresa seleccionada no posee riesgos registrados.\n");
            return;
        }

        printf("\n--- Seleccione el Riesgo a Eliminar ---\n");
        for (int i = 0; i < (*empresas)[idxE].cantidadRiesgos; i++) {
            printf("%d. %s\n", i + 1, (*empresas)[idxE].riesgos[i].nombre);
        }
        int idxR = leerEnteroRango("Riesgo #: ", 1, (*empresas)[idxE].cantidadRiesgos) - 1;

        printf("\n¿Esta seguro de que desea borrar este riesgo?\n1. Si\n2. No\n");
        if (leerEnteroRango("Confirmación: ", 1, 2) == 1) {
            eliminarRiesgoDeEmpresa(&((*empresas)[idxE]), idxR);
            guardarEmpresaJSON(&((*empresas)[idxE]));
            printf(">> Riesgo eliminado de la empresa y JSON actualizado.\n");
        } else {
            printf(">> Operacion cancelada.\n");
        }
    }
}

void menuCalcularRiesgos(Empresa *empresas, int cantEmpresas, Riesgo *indiv, int cantIndiv) {
    printf("\n========================================\n");
    printf("         CALCULAR / EVALUAR RIESGOS\n");
    printf("========================================\n");
    printf("1. Evaluar Riesgos Individuales\n");
    printf("2. Evaluar Riesgos de una Empresa\n");
    printf("0. Volver\n");

    int opc = leerEnteroRango("Seleccione una opcion: ", 0, 2);
    if (opc == 0) return;

    if (opc == 1) {
        if (cantIndiv == 0) {
            printf("\n>> No hay riesgos individuales para evaluar.\n");
            return;
        }
        for (int i = 0; i < cantIndiv; i++) {
            mostrarCalculoMosler(&indiv[i]);
        }
    } else if (opc == 2) {
        if (cantEmpresas == 0) {
            printf("\n>> No hay empresas cargadas.\n");
            return;
        }
        printf("\n--- Seleccione la Empresa ---\n");
        for (int i = 0; i < cantEmpresas; i++) {
            printf("%d. %s\n", i + 1, empresas[i].nombre);
        }
        int idxE = leerEnteroRango("Empresa #: ", 1, cantEmpresas) - 1;

        if (empresas[idxE].cantidadRiesgos == 0) {
            printf("\n>> La empresa seleccionada no posee riesgos registrados.\n");
            return;
        }

        printf("\n========================================\n");
        printf(" EVALUACIÓN DE RIESGOS: %s\n", empresas[idxE].nombre);
        printf("========================================\n");
        for (int i = 0; i < empresas[idxE].cantidadRiesgos; i++) {
            mostrarCalculoMosler(&(empresas[idxE].riesgos[i]));
        }
    }
}

void menuCrearEmpresa(Empresa **empresas, int *cantEmpresas) {
    printf("\n========================================\n");
    printf("             CREAR EMPRESA\n");
    printf("========================================\n");

    Empresa nuevaEmp;
    nuevaEmp.nombre = leerCadenaDinamica("Nombre de la Empresa: ");
    nuevaEmp.industria = leerCadenaDinamica("Industria / Sector: ");
    nuevaEmp.descripcion = leerCadenaDinamica("Descripción general: ");
    nuevaEmp.archivoJSON = generarNombreArchivo(nuevaEmp.nombre);
    nuevaEmp.riesgos = NULL;
    nuevaEmp.cantidadRiesgos = 0;

    if (guardarEmpresaJSON(&nuevaEmp)) {
        agregarEmpresaColeccion(empresas, cantEmpresas, nuevaEmp);
        printf("\n>> Empresa '%s' creada exitosamente y archivo '%s' generado.\n", nuevaEmp.nombre, nuevaEmp.archivoJSON);
    } else {
        printf("\n>> Error al crear el archivo de la empresa.\n");
        liberarEmpresaContenido(&nuevaEmp);
    }
}

void menuVerEmpresas(Empresa **empresas, int *cantEmpresas) {
    cargarEmpresasDelDirectorio(empresas, cantEmpresas);

    printf("\n========================================\n");
    printf("          LISTA DE EMPRESAS\n");
    printf("========================================\n");

    if (*cantEmpresas == 0) {
        printf(">> No hay empresas registradas ni archivos JSON en la carpeta.\n");
        return;
    }

    for (int i = 0; i < *cantEmpresas; i++) {
        printf("\n[%d] Empresa: %s\n", i + 1, (*empresas)[i].nombre ? (*empresas)[i].nombre : "Sin Nombre");
        printf("    Industria: %s\n", (*empresas)[i].industria ? (*empresas)[i].industria : "N/A");
        printf("    Descripcion: %s\n", (*empresas)[i].descripcion ? (*empresas)[i].descripcion : "N/A");
        printf("    Archivo JSON: %s\n", (*empresas)[i].archivoJSON ? (*empresas)[i].archivoJSON : "N/A");
        printf("    Riesgos registrados: %d\n", (*empresas)[i].cantidadRiesgos);
        if ((*empresas)[i].cantidadRiesgos > 0) {
            printf("    Riesgos:\n");
            for (int j = 0; j < (*empresas)[i].cantidadRiesgos; j++) {
                printf("      - %s\n", (*empresas)[i].riesgos[j].nombre);
            }
        }
    }
}

void menuBorrarEmpresa(Empresa **empresas, int *cantEmpresas) {
    printf("\n========================================\n");
    printf("            BORRAR EMPRESA\n");
    printf("========================================\n");

    if (*cantEmpresas == 0) {
        printf(">> No hay empresas registradas para borrar.\n");
        return;
    }

    for (int i = 0; i < *cantEmpresas; i++) {
        printf("%d. %s (%s)\n", i + 1, (*empresas)[i].nombre, (*empresas)[i].archivoJSON);
    }

    int idx = leerEnteroRango("Seleccione empresa a borrar: ", 1, *cantEmpresas) - 1;

    printf("\n¿Esta seguro de eliminar '%s'? (No borra el archivo JSON del disco)\n1. Si\n2. No\n", (*empresas)[idx].nombre);
    if (leerEnteroRango("Confirmación: ", 1, 2) == 1) {
        eliminarEmpresaColeccion(empresas, cantEmpresas, idx);
        printf(">> Empresa eliminada de la memoria exitosamente.\n");
    } else {
        printf(">> Operacion cancelada.\n");
    }
}

/* =========================================================================
 * FUNCIÓN PRINCIPAL (main)
 * ========================================================================= */

int main(void) {
    Empresa *empresas = NULL;
    int cantEmpresas = 0;

    Riesgo *indiv = NULL;
    int cantIndiv = 0;

    /* Cargar automáticamente todos los JSON existentes en el directorio al arrancar */
    cargarEmpresasDelDirectorio(&empresas, &cantEmpresas);

    int opcion;

    do {
        printf("\n========================================\n");
        printf("    SISTEMA DE EVALUACION DE RIESGOS\n");
        printf("             METODO MOSLER\n");
        printf("========================================\n");
        printf("1. Crear Riesgo\n");
        printf("2. Modificar Riesgo\n");
        printf("3. Borrar Riesgo\n");
        printf("4. Calcular / Evaluar Riesgos\n");
        printf("5. Crear Empresa\n");
        printf("6. Ver Empresas Cargadas\n");
        printf("7. Borrar Empresa\n");
        printf("0. Salir\n");

        opcion = leerEnteroRango("Seleccione una opcion: ", 0, 7);

        switch (opcion) {
            case 1:
                menuCrearRiesgo(&empresas, &cantEmpresas, &indiv, &cantIndiv);
                break;
            case 2:
                menuModificarRiesgo(empresas, cantEmpresas, indiv, cantIndiv);
                break;
            case 3:
                menuBorrarRiesgo(&empresas, &cantEmpresas, &indiv, &cantIndiv);
                break;
            case 4:
                menuCalcularRiesgos(empresas, cantEmpresas, indiv, cantIndiv);
                break;
            case 5:
                menuCrearEmpresa(&empresas, &cantEmpresas);
                break;
            case 6:
                menuVerEmpresas(&empresas, &cantEmpresas);
                break;
            case 7:
                menuBorrarEmpresa(&empresas, &cantEmpresas);
                break;
            case 0:
                printf("\n>> Saliendo del programa. ¡Hasta luego!\n");
                break;
        }

    } while (opcion != 0);

    /* Limpieza total de memoria asignada dinámicamente */
    liberarListaEmpresas(&empresas, &cantEmpresas);
    liberarListaRiesgos(&indiv, &cantIndiv);

    return 0;
}

// erozvalue.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_VEHICULOS 100

// ==========================================
// ESTRUCTURAS DE DATOS
// ==========================================

typedef struct {
    float gas_premium;
    float gas_regular;
    float diesel_optimo;
    float diesel_regular;
    float aceite_sintetico;
    float aceite_semisintetico;
    float aceite_mineral;
    float precio_neumaticos;
    float precio_aceite_trans;
    float precio_aceite_dif;
    float precio_bandas_freno;
} Productos;

typedef struct {
    char marca[50];
    char modelo[50];
    char cilindrada[30];
    float potencia;
    char torque[50];
    char transmision[50];
    char traccion[30];
    float cap_arrastre;
    float precio_actual;
    char estado[20];
    char tipo_seguro[30];
    float costo_seguro;
    float vida_util;

    char tipo_combustible[30];
    float consumo_ciudad_kml;
    float consumo_autopista_kml;
    float consumo_offroad_kml;

    char tipo_aceite[30];
    float km_aceite_ciudad;
    float km_aceite_offroad;
    float precio_aceite_trans;
    float km_aceite_trans;
    float precio_aceite_dif;
    float km_aceite_dif;
    float km_bandas_freno;
    float km_refrigerante;
    float km_neumaticos_carretera;
    float km_neumaticos_offroad;
    float km_bujias;

    float km_acumulados;
    float total_combustible;
    float total_mantenimiento;
    float total_amortizacion;
    float total_imprevistos; // NUEVO: Gastos acumulados por imprevistos
    int cantidad_viajes;
    float total_invertido;
} Vehiculo;

// ==========================================
// PROTOTIPOS DE FUNCIONES
// ==========================================
void limpiar_buffer();
void leer_cadena(const char* mensaje, char* buffer, int max_len);
float leer_float_positivo(const char* mensaje);
int leer_entero_positivo(const char* mensaje);

void guardar_productos(Productos p);
Productos cargar_productos();
void crear_productos_por_defecto();

void guardar_vehiculos(Vehiculo vehiculos[], int contador);
void cargar_vehiculos(Vehiculo vehiculos[], int *contador);
void crear_vehiculo_defecto(Vehiculo vehiculos[], int *contador);

void registrar_vehiculo(Vehiculo vehiculos[], int *contador);
void borrar_vehiculo(Vehiculo vehiculos[], int *contador);
void modificar_vehiculo(Vehiculo vehiculos[], int contador);
void calcular_viaje(Vehiculo vehiculos[], int contador);
void listar_vehiculos(Vehiculo vehiculos[], int contador);
void menu_modificar_combustibles();
void menu_modificar_mantenimiento();

// ==========================================
// FUNCIÓN PRINCIPAL (MAIN)
// ==========================================
int main() {
    Vehiculo vehiculos[MAX_VEHICULOS];
    int total_vehiculos = 0;
    int opcion;

    // Carga inicial: Si los archivos no existen, el programa los crea automáticamente.
    cargar_productos();
    cargar_vehiculos(vehiculos, &total_vehiculos);

    do {
        printf("\n=== Bienvenido a ErozValue ===\n");
        printf("1. Registrar nuevo vehiculo\n");
        printf("2. Borrar vehiculo\n");
        printf("3. Modificar parametros de un vehiculo\n");
        printf("4. Calcular viaje\n");
        printf("5. Modificar precios del combustible\n");
        printf("6. Modificar precios de mantenimiento\n");
        printf("7. Salir\n");

        opcion = leer_entero_positivo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrar_vehiculo(vehiculos, &total_vehiculos); break;
            case 2: borrar_vehiculo(vehiculos, &total_vehiculos); break;
            case 3: modificar_vehiculo(vehiculos, total_vehiculos); break;
            case 4: calcular_viaje(vehiculos, total_vehiculos); break;
            case 5: menu_modificar_combustibles(); break;
            case 6: menu_modificar_mantenimiento(); break;
            case 7: printf("Saliendo de ErozValue... ¡Nos vemos!\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while (opcion != 7);

    return 0;
}

// ==========================================
// UTILIDADES Y VALIDACIONES
// ==========================================
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void leer_cadena(const char* mensaje, char* buffer, int max_len) {
    while (1) {
        printf("%s", mensaje);
        if (fgets(buffer, max_len, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) break;
            else printf("[Error] La entrada no puede estar vacia.\n");
        }
    }
}

float leer_float_positivo(const char* mensaje) {
    float valor;
    int leidos;
    while (1) {
        printf("%s", mensaje);
        leidos = scanf("%f", &valor);
        limpiar_buffer();
        if (leidos == 1 && valor >= 0.0f) return valor;
        printf("[Error] Debe ingresar un numero positivo valido.\n");
    }
}

int leer_entero_positivo(const char* mensaje) {
    int valor;
    int leidos;
    while (1) {
        printf("%s", mensaje);
        leidos = scanf("%d", &valor);
        limpiar_buffer();
        if (leidos == 1 && valor >= 0) return valor;
        printf("[Error] Debe ingresar un numero entero positivo valido.\n");
    }
}

// ==========================================
// GESTIÓN DE PRODUCTOS (JSON)
// ==========================================
void guardar_productos(Productos p) {
    FILE *file = fopen("Productos.json", "w");
    if (!file) {
        printf("Error al guardar Productos.json\n");
        return;
    }
    fprintf(file, "{\n");
    fprintf(file, "  \"gas_premium\": %.2f,\n", p.gas_premium);
    fprintf(file, "  \"gas_regular\": %.2f,\n", p.gas_regular);
    fprintf(file, "  \"diesel_optimo\": %.2f,\n", p.diesel_optimo);
    fprintf(file, "  \"diesel_regular\": %.2f,\n", p.diesel_regular);
    fprintf(file, "  \"aceite_sintetico\": %.2f,\n", p.aceite_sintetico);
    fprintf(file, "  \"aceite_semisintetico\": %.2f,\n", p.aceite_semisintetico);
    fprintf(file, "  \"aceite_mineral\": %.2f,\n", p.aceite_mineral);
    fprintf(file, "  \"precio_neumaticos\": %.2f,\n", p.precio_neumaticos);
    fprintf(file, "  \"precio_aceite_trans\": %.2f,\n", p.precio_aceite_trans);
    fprintf(file, "  \"precio_aceite_dif\": %.2f,\n", p.precio_aceite_dif);
    fprintf(file, "  \"precio_bandas_freno\": %.2f\n", p.precio_bandas_freno);
    fprintf(file, "}\n");
    fclose(file);
}

void crear_productos_por_defecto() {
    Productos p = {
        290.10, 274.50, 239.10, 221.60,
        3500.0, 2500.0, 1500.0,
        8500.0, 4000.0, 3000.0, 2500.0
    };
    guardar_productos(p);
}

Productos cargar_productos() {
    Productos p = {0};
    FILE *file = fopen("Productos.json", "r");
    if (!file) {
        crear_productos_por_defecto();
        file = fopen("Productos.json", "r");
    }

    char linea[256], clave[100];
    float valor;

    while (fgets(linea, sizeof(linea), file)) {
        if (sscanf(linea, " \"%[^\"]\": %f", clave, &valor) == 2) {
            if (strcmp(clave, "gas_premium") == 0) p.gas_premium = valor;
            else if (strcmp(clave, "gas_regular") == 0) p.gas_regular = valor;
            else if (strcmp(clave, "diesel_optimo") == 0) p.diesel_optimo = valor;
            else if (strcmp(clave, "diesel_regular") == 0) p.diesel_regular = valor;
            else if (strcmp(clave, "aceite_sintetico") == 0) p.aceite_sintetico = valor;
            else if (strcmp(clave, "aceite_semisintetico") == 0) p.aceite_semisintetico = valor;
            else if (strcmp(clave, "aceite_mineral") == 0) p.aceite_mineral = valor;
            else if (strcmp(clave, "precio_neumaticos") == 0) p.precio_neumaticos = valor;
            else if (strcmp(clave, "precio_aceite_trans") == 0) p.precio_aceite_trans = valor;
            else if (strcmp(clave, "precio_aceite_dif") == 0) p.precio_aceite_dif = valor;
            else if (strcmp(clave, "precio_bandas_freno") == 0) p.precio_bandas_freno = valor;
        }
    }
    fclose(file);
    return p;
}

void menu_modificar_combustibles() {
    Productos p = cargar_productos();
    printf("\n--- Modificar Precios de Combustible (DOP) ---\n");
    p.gas_premium = leer_float_positivo("Precio Gasolina Premium: ");
    p.gas_regular = leer_float_positivo("Precio Gasolina Regular: ");
    p.diesel_optimo = leer_float_positivo("Precio Diesel Optimo: ");
    p.diesel_regular = leer_float_positivo("Precio Diesel Regular: ");
    guardar_productos(p);
    printf("¡Precios actualizados exitosamente!\n");
}

void menu_modificar_mantenimiento() {
    Productos p = cargar_productos();
    printf("\n--- Modificar Precios de Mantenimiento (DOP) ---\n");
    p.precio_neumaticos = leer_float_positivo("Precio promedio neumaticos: ");
    p.precio_aceite_trans = leer_float_positivo("Precio promedio aceite transmision: ");
    p.precio_aceite_dif = leer_float_positivo("Precio promedio aceite diferencial: ");
    p.precio_bandas_freno = leer_float_positivo("Precio promedio bandas de freno: ");
    p.aceite_sintetico = leer_float_positivo("Precio aceite sintetico: ");
    p.aceite_semisintetico = leer_float_positivo("Precio aceite semisintetico: ");
    p.aceite_mineral = leer_float_positivo("Precio aceite mineral: ");
    guardar_productos(p);
    printf("¡Precios actualizados exitosamente!\n");
}

// ==========================================
// GESTIÓN DE VEHÍCULOS (JSON)
// ==========================================
void guardar_vehiculos(Vehiculo vehiculos[], int contador) {
    FILE *file = fopen("Vehiculos.json", "w");
    if (!file) return;

    fprintf(file, "[\n");
    for (int i = 0; i < contador; i++) {
        fprintf(file, "  {\n");
        fprintf(file, "    \"marca\": \"%s\",\n", vehiculos[i].marca);
        fprintf(file, "    \"modelo\": \"%s\",\n", vehiculos[i].modelo);
        fprintf(file, "    \"cilindrada\": \"%s\",\n", vehiculos[i].cilindrada);
        fprintf(file, "    \"potencia\": %.2f,\n", vehiculos[i].potencia);
        fprintf(file, "    \"torque\": \"%s\",\n", vehiculos[i].torque);
        fprintf(file, "    \"transmision\": \"%s\",\n", vehiculos[i].transmision);
        fprintf(file, "    \"traccion\": \"%s\",\n", vehiculos[i].traccion);
        fprintf(file, "    \"cap_arrastre\": %.2f,\n", vehiculos[i].cap_arrastre);
        fprintf(file, "    \"precio_actual\": %.2f,\n", vehiculos[i].precio_actual);
        fprintf(file, "    \"estado\": \"%s\",\n", vehiculos[i].estado);
        fprintf(file, "    \"tipo_seguro\": \"%s\",\n", vehiculos[i].tipo_seguro);
        fprintf(file, "    \"costo_seguro\": %.2f,\n", vehiculos[i].costo_seguro);
        fprintf(file, "    \"vida_util\": %.2f,\n", vehiculos[i].vida_util);

        fprintf(file, "    \"tipo_combustible\": \"%s\",\n", vehiculos[i].tipo_combustible);
        fprintf(file, "    \"consumo_ciudad_kml\": %.2f,\n", vehiculos[i].consumo_ciudad_kml);
        fprintf(file, "    \"consumo_autopista_kml\": %.2f,\n", vehiculos[i].consumo_autopista_kml);
        fprintf(file, "    \"consumo_offroad_kml\": %.2f,\n", vehiculos[i].consumo_offroad_kml);

        fprintf(file, "    \"tipo_aceite\": \"%s\",\n", vehiculos[i].tipo_aceite);
        fprintf(file, "    \"km_aceite_ciudad\": %.2f,\n", vehiculos[i].km_aceite_ciudad);
        fprintf(file, "    \"km_aceite_offroad\": %.2f,\n", vehiculos[i].km_aceite_offroad);
        fprintf(file, "    \"precio_aceite_trans\": %.2f,\n", vehiculos[i].precio_aceite_trans);
        fprintf(file, "    \"km_aceite_trans\": %.2f,\n", vehiculos[i].km_aceite_trans);
        fprintf(file, "    \"precio_aceite_dif\": %.2f,\n", vehiculos[i].precio_aceite_dif);
        fprintf(file, "    \"km_aceite_dif\": %.2f,\n", vehiculos[i].km_aceite_dif);
        fprintf(file, "    \"km_bandas_freno\": %.2f,\n", vehiculos[i].km_bandas_freno);
        fprintf(file, "    \"km_refrigerante\": %.2f,\n", vehiculos[i].km_refrigerante);
        fprintf(file, "    \"km_neumaticos_carretera\": %.2f,\n", vehiculos[i].km_neumaticos_carretera);
        fprintf(file, "    \"km_neumaticos_offroad\": %.2f,\n", vehiculos[i].km_neumaticos_offroad);
        fprintf(file, "    \"km_bujias\": %.2f,\n", vehiculos[i].km_bujias);

        fprintf(file, "    \"km_acumulados\": %.2f,\n", vehiculos[i].km_acumulados);
        fprintf(file, "    \"total_combustible\": %.2f,\n", vehiculos[i].total_combustible);
        fprintf(file, "    \"total_mantenimiento\": %.2f,\n", vehiculos[i].total_mantenimiento);
        fprintf(file, "    \"total_amortizacion\": %.2f,\n", vehiculos[i].total_amortizacion);
        fprintf(file, "    \"total_imprevistos\": %.2f,\n", vehiculos[i].total_imprevistos); // NUEVO
        fprintf(file, "    \"cantidad_viajes\": %d,\n", vehiculos[i].cantidad_viajes);
        fprintf(file, "    \"total_invertido\": %.2f\n", vehiculos[i].total_invertido);

        fprintf(file, "  }%s\n", (i == contador - 1) ? "" : ",");
    }
    fprintf(file, "]\n");
    fclose(file);
}

void crear_vehiculo_defecto(Vehiculo vehiculos[], int *contador) {
    Vehiculo v = {0};
    strcpy(v.marca, "Nissan");
    strcpy(v.modelo, "Frontier 2024");
    strcpy(v.cilindrada, "2.5L Turbo Diesel");
    v.potencia = 188.0;
    strcpy(v.torque, "450 Nm");
    strcpy(v.transmision, "Manual de 6 velocidades o automatica de 7");
    strcpy(v.traccion, "4x4");
    v.cap_arrastre = 3500.0;
    v.precio_actual = 2500000.0;
    strcpy(v.estado, "Nuevo");
    strcpy(v.tipo_seguro, "Full");
    v.costo_seguro = 5500.0;
    v.vida_util = 300000.0;

    strcpy(v.tipo_combustible, "Diesel Optimo");
    v.consumo_ciudad_kml = 9.5;
    v.consumo_autopista_kml = 13.0;
    v.consumo_offroad_kml = 6.0;

    strcpy(v.tipo_aceite, "Sintetico");
    v.km_aceite_ciudad = 5000.0;
    v.km_aceite_offroad = 3000.0;
    v.precio_aceite_trans = 4500.0;
    v.km_aceite_trans = 40000.0;
    v.precio_aceite_dif = 3000.0;
    v.km_aceite_dif = 40000.0;
    v.km_bandas_freno = 20000.0;
    v.km_refrigerante = 50000.0;
    v.km_neumaticos_carretera = 60000.0;
    v.km_neumaticos_offroad = 30000.0;
    v.km_bujias = 80000.0;

    vehiculos[*contador] = v;
    (*contador)++;
    guardar_vehiculos(vehiculos, *contador);
}

void cargar_vehiculos(Vehiculo vehiculos[], int *contador) {
    *contador = 0;
    FILE *file = fopen("Vehiculos.json", "r");
    if (!file) {
        crear_vehiculo_defecto(vehiculos, contador);
        return;
    }

    char linea[512];
    Vehiculo v = {0};
    int en_objeto = 0;

    while (fgets(linea, sizeof(linea), file)) {
        if (strstr(linea, "{")) {
            en_objeto = 1;
            memset(&v, 0, sizeof(Vehiculo));
        } else if (strstr(linea, "}")) {
            if (en_objeto) {
                vehiculos[*contador] = v;
                (*contador)++;
                en_objeto = 0;
            }
        } else if (en_objeto) {
            char clave[100], valor_str[100];
            float valor_f;

            if (sscanf(linea, " \"%[^\"]\": \"%[^\"]\"", clave, valor_str) == 2) {
                if (strcmp(clave, "marca") == 0) strcpy(v.marca, valor_str);
                else if (strcmp(clave, "modelo") == 0) strcpy(v.modelo, valor_str);
                else if (strcmp(clave, "cilindrada") == 0) strcpy(v.cilindrada, valor_str);
                else if (strcmp(clave, "torque") == 0) strcpy(v.torque, valor_str);
                else if (strcmp(clave, "transmision") == 0) strcpy(v.transmision, valor_str);
                else if (strcmp(clave, "traccion") == 0) strcpy(v.traccion, valor_str);
                else if (strcmp(clave, "estado") == 0) strcpy(v.estado, valor_str);
                else if (strcmp(clave, "tipo_seguro") == 0) strcpy(v.tipo_seguro, valor_str);
                else if (strcmp(clave, "tipo_combustible") == 0) strcpy(v.tipo_combustible, valor_str);
                else if (strcmp(clave, "tipo_aceite") == 0) strcpy(v.tipo_aceite, valor_str);
            } else if (sscanf(linea, " \"%[^\"]\": %f", clave, &valor_f) == 2) {
                if (strcmp(clave, "potencia") == 0) v.potencia = valor_f;
                else if (strcmp(clave, "cap_arrastre") == 0) v.cap_arrastre = valor_f;
                else if (strcmp(clave, "precio_actual") == 0) v.precio_actual = valor_f;
                else if (strcmp(clave, "costo_seguro") == 0) v.costo_seguro = valor_f;
                else if (strcmp(clave, "vida_util") == 0) v.vida_util = valor_f;
                else if (strcmp(clave, "consumo_ciudad_kml") == 0) v.consumo_ciudad_kml = valor_f;
                else if (strcmp(clave, "consumo_autopista_kml") == 0) v.consumo_autopista_kml = valor_f;
                else if (strcmp(clave, "consumo_offroad_kml") == 0) v.consumo_offroad_kml = valor_f;
                else if (strcmp(clave, "km_aceite_ciudad") == 0) v.km_aceite_ciudad = valor_f;
                else if (strcmp(clave, "km_aceite_offroad") == 0) v.km_aceite_offroad = valor_f;
                else if (strcmp(clave, "precio_aceite_trans") == 0) v.precio_aceite_trans = valor_f;
                else if (strcmp(clave, "km_aceite_trans") == 0) v.km_aceite_trans = valor_f;
                else if (strcmp(clave, "precio_aceite_dif") == 0) v.precio_aceite_dif = valor_f;
                else if (strcmp(clave, "km_aceite_dif") == 0) v.km_aceite_dif = valor_f;
                else if (strcmp(clave, "km_bandas_freno") == 0) v.km_bandas_freno = valor_f;
                else if (strcmp(clave, "km_refrigerante") == 0) v.km_refrigerante = valor_f;
                else if (strcmp(clave, "km_neumaticos_carretera") == 0) v.km_neumaticos_carretera = valor_f;
                else if (strcmp(clave, "km_neumaticos_offroad") == 0) v.km_neumaticos_offroad = valor_f;
                else if (strcmp(clave, "km_bujias") == 0) v.km_bujias = valor_f;
                else if (strcmp(clave, "km_acumulados") == 0) v.km_acumulados = valor_f;
                else if (strcmp(clave, "total_combustible") == 0) v.total_combustible = valor_f;
                else if (strcmp(clave, "total_mantenimiento") == 0) v.total_mantenimiento = valor_f;
                else if (strcmp(clave, "total_amortizacion") == 0) v.total_amortizacion = valor_f;
                else if (strcmp(clave, "total_imprevistos") == 0) v.total_imprevistos = valor_f; // NUEVO
                else if (strcmp(clave, "cantidad_viajes") == 0) v.cantidad_viajes = (int)valor_f;
                else if (strcmp(clave, "total_invertido") == 0) v.total_invertido = valor_f;
            }
        }
    }
    fclose(file);
    if (*contador == 0) crear_vehiculo_defecto(vehiculos, contador);
}

// ==========================================
// FUNCIONES DEL MENÚ
// ==========================================
void registrar_vehiculo(Vehiculo vehiculos[], int *contador) {
    if (*contador >= MAX_VEHICULOS) {
        printf("Limite de vehiculos alcanzado.\n");
        return;
    }
    Vehiculo v = {0};

    printf("\n--- Primera Impresion (Informacion basica) ---\n");
    leer_cadena("Marca: ", v.marca, 50);
    leer_cadena("Modelo: ", v.modelo, 50);
    leer_cadena("Cilindrada: ", v.cilindrada, 30);
    v.potencia = leer_float_positivo("Potencia (HP): ");
    leer_cadena("Torque (Nm y RPM): ", v.torque, 50);
    leer_cadena("Transmision: ", v.transmision, 50);
    leer_cadena("Traccion: ", v.traccion, 30);
    v.cap_arrastre = leer_float_positivo("Capacidad de arrastre (Kg): ");
    v.precio_actual = leer_float_positivo("Precio actual del vehiculo: ");
    leer_cadena("Estado (Nuevo o Usado): ", v.estado, 20);
    leer_cadena("Tipo de seguro: ", v.tipo_seguro, 30);
    v.costo_seguro = leer_float_positivo("Costo mensual del seguro: ");

    do {
        v.vida_util = leer_float_positivo("Vida util del vehiculo (Km): ");
        if (v.vida_util == 0) printf("La vida util no puede ser cero.\n");
    } while (v.vida_util == 0);

    leer_cadena("Tipo de combustible (Gasolina Premium/Regular, Diesel Optimo/Regular): ", v.tipo_combustible, 30);
    v.consumo_ciudad_kml = leer_float_positivo("Consumo en ciudad (Km/L o Km/Galon): ");
    v.consumo_autopista_kml = leer_float_positivo("Consumo en autopista (Km/L o Km/Galon): ");
    v.consumo_offroad_kml = leer_float_positivo("Consumo Off-Road (Km/L o Km/Galon): ");

    printf("\n--- Segunda Impresion (Informacion tecnica) ---\n");
    leer_cadena("Tipo de aceite: ", v.tipo_aceite, 30);
    v.km_aceite_ciudad = leer_float_positivo("Km para cambio de aceite en ciudad: ");
    v.km_aceite_offroad = leer_float_positivo("Km para cambio de aceite Off-Road: ");
    v.precio_aceite_trans = leer_float_positivo("Precio del aceite de transmision: ");
    v.km_aceite_trans = leer_float_positivo("Km para cambio de aceite transmision: ");
    v.precio_aceite_dif = leer_float_positivo("Precio del aceite del diferencial: ");
    v.km_aceite_dif = leer_float_positivo("Km para cambio de aceite diferencial: ");
    v.km_bandas_freno = leer_float_positivo("Km para cambio de bandas de freno: ");
    v.km_refrigerante = leer_float_positivo("Km para cambio de refrigerante: ");
    v.km_neumaticos_carretera = leer_float_positivo("Km para cambio de neumaticos en carretera: ");
    v.km_neumaticos_offroad = leer_float_positivo("Km para cambio de neumaticos Off-Road: ");
    v.km_bujias = leer_float_positivo("Km para cambio de bujias: ");

    vehiculos[*contador] = v;
    (*contador)++;
    guardar_vehiculos(vehiculos, *contador);
    printf("¡Vehiculo registrado exitosamente!\n");
}

void listar_vehiculos(Vehiculo vehiculos[], int contador) {
    for (int i = 0; i < contador; i++) {
        printf("%d. %s %s\n", i + 1, vehiculos[i].marca, vehiculos[i].modelo);
    }
}

void borrar_vehiculo(Vehiculo vehiculos[], int *contador) {
    if (*contador == 0) {
        printf("No hay vehiculos para eliminar.\n");
        return;
    }
    printf("\nSeleccione el vehiculo a eliminar:\n");
    listar_vehiculos(vehiculos, *contador);
    int seleccion = leer_entero_positivo("Opcion: ") - 1;

    if (seleccion >= 0 && seleccion < *contador) {
        char confirmacion[10];
        leer_cadena("¿Esta seguro? (Si/No): ", confirmacion, 10);
        if (strcmp(confirmacion, "Si") == 0 || strcmp(confirmacion, "si") == 0) {
            for (int i = seleccion; i < *contador - 1; i++) {
                vehiculos[i] = vehiculos[i + 1];
            }
            (*contador)--;
            guardar_vehiculos(vehiculos, *contador);
            printf("Vehiculo eliminado.\n");
        } else {
            printf("Operacion cancelada.\n");
        }
    } else {
        printf("Seleccion invalida.\n");
    }
}

void modificar_vehiculo(Vehiculo vehiculos[], int contador) {
    if (contador == 0) {
        printf("No hay vehiculos registrados.\n");
        return;
    }
    printf("\nSeleccione vehiculo a modificar:\n");
    listar_vehiculos(vehiculos, contador);
    int s = leer_entero_positivo("Opcion: ") - 1;

    if (s >= 0 && s < contador) {
        printf("Modificando %s %s...\n", vehiculos[s].marca, vehiculos[s].modelo);
        vehiculos[s].precio_actual = leer_float_positivo("Nuevo precio actual del vehiculo: ");
        leer_cadena("Nuevo tipo de seguro: ", vehiculos[s].tipo_seguro, 30);
        vehiculos[s].costo_seguro = leer_float_positivo("Nuevo costo mensual del seguro: ");

        guardar_vehiculos(vehiculos, contador);
        printf("Vehiculo actualizado automaticamente en Vehiculos.json.\n");
    } else {
        printf("Seleccion invalida.\n");
    }
}

void calcular_viaje(Vehiculo vehiculos[], int contador) {
    if (contador == 0) {
        printf("Debe registrar un vehiculo primero.\n");
        return;
    }

    printf("\nSeleccione vehiculo para el viaje:\n");
    listar_vehiculos(vehiculos, contador);
    int s = leer_entero_positivo("Opcion: ") - 1;

    if (s < 0 || s >= contador) {
        printf("Seleccion invalida.\n");
        return;
    }

    Vehiculo *v = &vehiculos[s];
    Productos p = cargar_productos();

    float km_ciudad = leer_float_positivo("Kilometros en ciudad: ");
    float km_autopista = leer_float_positivo("Kilometros en autopista: ");
    float km_offroad = leer_float_positivo("Kilometros Off-Road: ");

    // NUEVO: Pidiendo presupuesto de imprevistos
    float imprevistos = leer_float_positivo("Presupuesto para gastos imprevistos (peajes, parqueos, pinchaduras): ");

    float km_total = km_ciudad + km_autopista + km_offroad;
    if (km_total == 0) {
        printf("El viaje debe tener una distancia mayor a 0.\n");
        return;
    }

    float precio_comb = p.gas_regular;
    if (strstr(v->tipo_combustible, "Premium") != NULL || strstr(v->tipo_combustible, "premium") != NULL) precio_comb = p.gas_premium;
    else if (strstr(v->tipo_combustible, "Optimo") != NULL || strstr(v->tipo_combustible, "optimo") != NULL) precio_comb = p.diesel_optimo;
    else if (strstr(v->tipo_combustible, "Diesel") != NULL || strstr(v->tipo_combustible, "diesel") != NULL) precio_comb = p.diesel_regular;

    float gasto_comb_ciudad = (km_ciudad / v->consumo_ciudad_kml) * precio_comb;
    float gasto_comb_autopista = (km_autopista / v->consumo_autopista_kml) * precio_comb;
    float gasto_comb_offroad = (km_offroad / v->consumo_offroad_kml) * precio_comb;
    float total_combustible_viaje = gasto_comb_ciudad + gasto_comb_autopista + gasto_comb_offroad;

    float amortizacion_viaje = (v->precio_actual / v->vida_util) * km_total;

    float mant_aceite = (km_ciudad / v->km_aceite_ciudad) * p.aceite_sintetico +
                        (km_offroad / v->km_aceite_offroad) * p.aceite_sintetico;
    float mant_trans = (km_total / v->km_aceite_trans) * v->precio_aceite_trans;
    float mant_dif = (km_total / v->km_aceite_dif) * v->precio_aceite_dif;
    float mant_frenos = (km_total / v->km_bandas_freno) * p.precio_bandas_freno;
    float mant_llantas = (km_ciudad + km_autopista) / v->km_neumaticos_carretera * p.precio_neumaticos +
                         (km_offroad / v->km_neumaticos_offroad) * p.precio_neumaticos;

    float total_mant_viaje = mant_aceite + mant_trans + mant_dif + mant_frenos + mant_llantas;

    // NUEVO: Sumando los imprevistos al costo real del viaje
    float costo_real_viaje = total_combustible_viaje + amortizacion_viaje + total_mant_viaje + imprevistos;

    v->km_acumulados += km_total;
    v->total_combustible += total_combustible_viaje;
    v->total_mantenimiento += total_mant_viaje;
    v->total_amortizacion += amortizacion_viaje;
    v->total_imprevistos += imprevistos; // NUEVO: Acumulando imprevistos
    v->cantidad_viajes += 1;
    v->total_invertido = v->total_combustible + v->total_mantenimiento + v->total_amortizacion + v->total_imprevistos;

    guardar_vehiculos(vehiculos, contador);

    printf("\n=== RESULTADOS DEL VIAJE ===\n");
    printf("Precio actual del combustible: $%.2f\n", precio_comb);
    printf("Kilometros recorridos en ciudad: %.2f Km\n", km_ciudad);
    printf("Kilometros recorridos en autopista: %.2f Km\n", km_autopista);
    printf("Kilometros recorridos en Off-Road: %.2f Km\n", km_offroad);

    printf("\nGasto de combustible en ciudad: $%.2f\n", gasto_comb_ciudad);
    printf("Gasto de combustible en autopista: $%.2f\n", gasto_comb_autopista);
    printf("Gasto de combustible Off-Road: $%.2f\n", gasto_comb_offroad);
    printf("Presupuesto de imprevistos utilizado: $%.2f\n", imprevistos); // NUEVO

    printf("\nTotal de gastos acumulados anteriores: $%.2f\n", (v->total_invertido - costo_real_viaje));
    printf("Total acumulado en combustible: $%.2f\n", v->total_combustible);
    printf("Total acumulado en mantenimiento: $%.2f\n", v->total_mantenimiento);
    printf("Total acumulado en imprevistos: $%.2f\n", v->total_imprevistos); // NUEVO

    printf("\n*** Costo real del viaje actual: $%.2f ***\n", costo_real_viaje);
    printf("--------------------------------------------------\n");
    printf("Los costos pueden variar ligeramente. Utilice estos resultados unicamente como referencia.\n");
}

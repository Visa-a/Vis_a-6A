#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constantes
#define MAX_USUARIOS 100
#define ARCHIVO_CSV "usuarios.csv"

// Códigos de color ANSI
#define COLOR_RESET "\033[0m"
#define COLOR_ROJO "\033[31m"
#define COLOR_VERDE "\033[32m"
#define COLOR_AZUL "\033[34m"
#define COLOR_AMARILLO "\033[33m"
#define COLOR_CIAN "\033[36m"

// Estructura para representar un usuario
typedef struct {
    char nombre[50];
    char apellidoPaterno[50];
    char apellidoMaterno[50];
    int anioNacimiento;
    char carrera[50];
    char correo[100];
    char folio[20];
    int aceptaPublicidad; // 1: Acepta, 0: No acepta
} Usuario;

// Variables globales
Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0; // Contador de usuarios registrados
int anioRegistro = 2024; // Año de registro

// Prototipos de funciones
void limpiarPantalla();
void cargarUsuarios();
void guardarUsuarios();
void registroUsuario();
void validarIngreso();
void consultarUsuario();
void generarFolio(char *nombre, char *apellidoPaterno, char *apellidoMaterno, int anioNacimiento, char *carrera, char *folio);
void imprimirLinea(int longitud, char caracter);
void imprimirTitulo(const char *titulo);

// Función principal
int main() {
    int opcion;
    char continuar;

    // Cargar datos del archivo CSV al iniciar el programa
    cargarUsuarios();

    do {
        limpiarPantalla();
        imprimirTitulo("Sistema de Control de Acceso");
        printf(COLOR_AZUL "1. Registrar Usuario\n" COLOR_RESET);
        printf(COLOR_AZUL "2. Validar Ingreso\n" COLOR_RESET);
        printf(COLOR_AZUL "3. Consultar Datos de Usuario\n" COLOR_RESET);
        printf(COLOR_ROJO "4. Salir\n" COLOR_RESET);
        imprimirLinea(40, '-');
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer

        switch (opcion) {
            case 1:
                registroUsuario();
                break;
            case 2:
                validarIngreso();
                break;
            case 3:
                consultarUsuario();
                break;
            case 4:
                printf(COLOR_ROJO "Saliendo del sistema...\n" COLOR_RESET);
                guardarUsuarios();
                exit(0);
            default:
                printf(COLOR_ROJO "Opcion no valida. Intente de nuevo.\n" COLOR_RESET);
        }

        printf("\n¿Desea volver al menu principal? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    // Guardar datos antes de cerrar
    guardarUsuarios();
    return 0;
}

// Función para limpiar la pantalla
void limpiarPantalla() {
    system("cls || clear");
}

// Función para cargar usuarios desde un archivo CSV
void cargarUsuarios() {
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        printf(COLOR_AMARILLO "No se encontro el archivo de usuarios. Se creara uno nuevo.\n" COLOR_RESET);
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo); // Ignorar encabezados

    while (fgets(linea, sizeof(linea), archivo)) {
        Usuario nuevoUsuario;
        sscanf(linea, "%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%d",
               nuevoUsuario.nombre,
               nuevoUsuario.apellidoPaterno,
               nuevoUsuario.apellidoMaterno,
               &nuevoUsuario.anioNacimiento,
               nuevoUsuario.carrera,
               nuevoUsuario.correo,
               nuevoUsuario.folio,
               &nuevoUsuario.aceptaPublicidad);
        usuarios[totalUsuarios++] = nuevoUsuario;
    }

    fclose(archivo);
}

// Función para guardar usuarios en un archivo CSV
void guardarUsuarios() {
    FILE *archivo = fopen(ARCHIVO_CSV, "w");
    if (archivo == NULL) {
        printf(COLOR_ROJO "Error al guardar los datos.\n" COLOR_RESET);
        return;
    }

    fprintf(archivo, "Nombre,Apellido Paterno,Apellido Materno,Anio Nacimiento,Carrera,Correo,Folio,Acepta Publicidad\n");

    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(archivo, "%s,%s,%s,%d,%s,%s,%s,%d\n",
                usuarios[i].nombre,
                usuarios[i].apellidoPaterno,
                usuarios[i].apellidoMaterno,
                usuarios[i].anioNacimiento,
                usuarios[i].carrera,
                usuarios[i].correo,
                usuarios[i].folio,
                usuarios[i].aceptaPublicidad);
    }

    fclose(archivo);
}

// Función para registrar usuarios
void registroUsuario() {
    if (totalUsuarios >= MAX_USUARIOS) {
        printf(COLOR_ROJO "No se pueden registrar mas usuarios. Capacidad maxima alcanzada.\n" COLOR_RESET);
        return;
    }

    Usuario nuevoUsuario;
    imprimirTitulo("Registro de Usuario");

    printf("Ingrese su nombre: ");
    fgets(nuevoUsuario.nombre, sizeof(nuevoUsuario.nombre), stdin);
    nuevoUsuario.nombre[strcspn(nuevoUsuario.nombre, "\n")] = '\0';
    printf("Ingrese su apellido paterno: ");
    fgets(nuevoUsuario.apellidoPaterno, sizeof(nuevoUsuario.apellidoPaterno), stdin);
    nuevoUsuario.apellidoPaterno[strcspn(nuevoUsuario.apellidoPaterno, "\n")] = '\0';
    printf("Ingrese su apellido materno: ");
    fgets(nuevoUsuario.apellidoMaterno, sizeof(nuevoUsuario.apellidoMaterno), stdin);
    nuevoUsuario.apellidoMaterno[strcspn(nuevoUsuario.apellidoMaterno, "\n")] = '\0';
    printf("Ingrese su anio de nacimiento: ");
    scanf("%d", &nuevoUsuario.anioNacimiento);
    getchar();
    printf("Ingrese su carrera: ");
    fgets(nuevoUsuario.carrera, sizeof(nuevoUsuario.carrera), stdin);
    nuevoUsuario.carrera[strcspn(nuevoUsuario.carrera, "\n")] = '\0';
    printf("Ingrese su correo electronico: ");
    fgets(nuevoUsuario.correo, sizeof(nuevoUsuario.correo), stdin);
    nuevoUsuario.correo[strcspn(nuevoUsuario.correo, "\n")] = '\0';

    printf("Acepta recibir recomendaciones y publicidad? (1: Si, 0: No): ");
    scanf("%d", &nuevoUsuario.aceptaPublicidad);

    generarFolio(nuevoUsuario.nombre, nuevoUsuario.apellidoPaterno, nuevoUsuario.apellidoMaterno, nuevoUsuario.anioNacimiento, nuevoUsuario.carrera, nuevoUsuario.folio);

    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].folio, nuevoUsuario.folio) == 0) {
            printf(COLOR_ROJO "El usuario ya esta registrado.\n" COLOR_RESET);
            return;
        }
    }

    usuarios[totalUsuarios++] = nuevoUsuario;
    printf(COLOR_VERDE "Registro exitoso. Su folio es: %s\n" COLOR_RESET, nuevoUsuario.folio);
}

// Función para validar el ingreso
void validarIngreso() {
    char folio[20];
    imprimirTitulo("Validacion de Ingreso");
    printf("Ingrese su folio: ");
    fgets(folio, sizeof(folio), stdin);
    folio[strcspn(folio, "\n")] = '\0';

    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].folio, folio) == 0) {
            printf(COLOR_VERDE "Ingreso permitido. Bienvenido, %s %s.\n" COLOR_RESET, usuarios[i].nombre, usuarios[i].apellidoPaterno);
            return;
        }
    }

    printf(COLOR_ROJO "Folio no valido o no registrado.\n" COLOR_RESET);
}

// Función para consultar datos de un usuario
void consultarUsuario() {
    char folio[20];
    imprimirTitulo("Consulta de Datos");
    printf("Ingrese el folio: ");
    fgets(folio, sizeof(folio), stdin);
    folio[strcspn(folio, "\n")] = '\0';

    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].folio, folio) == 0) {
            printf(COLOR_VERDE "Nombre: %s\n" COLOR_RESET, usuarios[i].nombre);
            printf(COLOR_VERDE "Apellido Paterno: %s\n" COLOR_RESET, usuarios[i].apellidoPaterno);
            printf(COLOR_VERDE "Apellido Materno: %s\n" COLOR_RESET, usuarios[i].apellidoMaterno);
            printf(COLOR_VERDE "Anio de Nacimiento: %d\n" COLOR_RESET, usuarios[i].anioNacimiento);
            printf(COLOR_VERDE "Carrera: %s\n" COLOR_RESET, usuarios[i].carrera);
            printf(COLOR_VERDE "Correo: %s\n" COLOR_RESET, usuarios[i].correo);
            printf(COLOR_VERDE "Acepta Publicidad: %s\n" COLOR_RESET, usuarios[i].aceptaPublicidad ? "Si" : "No");
            return;
        }
    }

    printf(COLOR_ROJO "Usuario no encontrado.\n" COLOR_RESET);
}

// Función para generar folio único
void generarFolio(char *nombre, char *apellidoPaterno, char *apellidoMaterno, int anioNacimiento, char *carrera, char *folio) {
    sprintf(folio, "%c%c%c%d%d%c", nombre[0], apellidoPaterno[0], apellidoMaterno[0], anioNacimiento, anioRegistro, carrera[0]);
}

// Función para imprimir una línea decorativa
void imprimirLinea(int longitud, char caracter) {
    for (int i = 0; i < longitud; i++) {
        printf("%c", caracter);
    }
    printf("\n");
}

// Función para imprimir un título con bordes
void imprimirTitulo(const char *titulo) {
    printf(COLOR_CIAN);
    imprimirLinea(40, '=');
    printf("%s\n", titulo);
    imprimirLinea(40, '=');
    printf(COLOR_RESET);
}

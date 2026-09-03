#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Un programa que gestione las notas de una clase de 20 alumnos de los cuales sabemos el nombre y la nota.
El programa debe ser capaz de:
1- Buscar un alumno
2- Modificar su nota
3. Realizar la media de todas las notas
4. Realizar la media de las notas menores de 5
5. Mostrar el alumno que mejores notas ha sacado
6. Mostrar el alumno que peores notas ha sacado

*/

#define TAM 20

typedef struct {
    char nombre[50];
    double nota;
} Alumno;

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double pedirNota() {
    double n;
    int ok;

    do {
        printf("Ingrese nota (0 - 10): ");
        ok = scanf("%lf", &n);

        if (ok != 1 || n < 0 || n > 10) {
            printf("Error: nota inválida.\n");
            limpiarBuffer();
        } else {
            limpiarBuffer();
            return n;
        }
    } while (1);
}

void pedirNombre(char *dest) {
    printf("Ingrese nombre: ");
    fgets(dest, 50, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

int buscarAlumno(Alumno clase[], const char *nombre) {
    for (int i = 0; i < TAM; i++) {
        if (strcmp(clase[i].nombre, nombre) == 0)
            return i;
    }
    return -1;
}

double mediaGeneral(Alumno clase[]) {
    double suma = 0;
    for (int i = 0; i < TAM; i++)
        suma += clase[i].nota;
    return suma / TAM;
}

double mediaSuspensos(Alumno clase[]) {
    double suma = 0;
    int cont = 0;

    for (int i = 0; i < TAM; i++) {
        if (clase[i].nota < 5) {
            suma += clase[i].nota;
            cont++;
        }
    }

    return (cont == 0) ? 0 : suma / cont;
}

int mejorAlumno(Alumno clase[]) {
    int pos = 0;
    for (int i = 1; i < TAM; i++)
        if (clase[i].nota > clase[pos].nota)
            pos = i;
    return pos;
}

int peorAlumno(Alumno clase[]) {
    int pos = 0;
    for (int i = 1; i < TAM; i++)
        if (clase[i].nota < clase[pos].nota)
            pos = i;
    return pos;
}

void guardarTabla(Alumno clase[]) {
    FILE *f = fopen("alumnos.txt", "w");

    if (f == NULL) {
        printf("Error al crear el archivo.\n");
        return;
    }

    // Encabezado de la tabla
    fprintf(f, "=============================================\n");
    fprintf(f, "| %-25s | %-10s |\n", "NOMBRE", "NOTA");
    fprintf(f, "=============================================\n");

    // Filas de la tabla
    for (int i = 0; i < TAM; i++) {
        fprintf(f, "| %-25s | %-10.2lf |\n", clase[i].nombre, clase[i].nota);
    }

    fprintf(f, "=============================================\n");

    fclose(f);
    printf("Datos guardados en formato tabla.\n");
}


int main() {
    Alumno clase[TAM];

    // Cargar datos iniciales
    for (int i = 0; i < TAM; i++) {
        printf("Alumno %d:\n", i + 1);
        pedirNombre(clase[i].nombre);
        clase[i].nota = pedirNota();
    }

    int opcion;
    char nombreBuscado[50];
    int pos;

    do {
        printf("\n=== MENU ===\n");
        printf("1. Buscar alumno\n");
        printf("2. Modificar nota\n");
        printf("3. Media general\n");
        printf("4. Media suspensos\n");
        printf("5. Mejor alumno\n");
        printf("6. Peor alumno\n");
        printf("7. Guardar en un archivo");
        printf("8. Salir\n");
        printf("Seleccione: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
        case 1:
            pedirNombre(nombreBuscado);
            pos = buscarAlumno(clase, nombreBuscado);
            if (pos == -1)
                printf("No encontrado.\n");
            else
                printf("Alumno: %s - Nota: %.2lf\n", clase[pos].nombre, clase[pos].nota);
            break;

        case 2:
            pedirNombre(nombreBuscado);
            pos = buscarAlumno(clase, nombreBuscado);
            if (pos == -1)
                printf("No encontrado.\n");
            else
                clase[pos].nota = pedirNota();
            break;

        case 3:
            printf("Media general: %.2lf\n", mediaGeneral(clase));
            break;

        case 4:
            printf("Media suspensos: %.2lf\n", mediaSuspensos(clase));
            break;

        case 5:
            pos = mejorAlumno(clase);
            printf("Mejor alumno: %s (%.2lf)\n", clase[pos].nombre, clase[pos].nota);
            break;

        case 6:
            pos = peorAlumno(clase);
            printf("Peor alumno: %s (%.2lf)\n", clase[pos].nombre, clase[pos].nota);
            break;

        case 7:
            guardarTabla(clase);
            break;
        case 8:
            printf("Saliendo...\n");
            break;

        default:
            printf("Opción inválida.\n");
        }

    } while (opcion != 7);

    return 0;
}


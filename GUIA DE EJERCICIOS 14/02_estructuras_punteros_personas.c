/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 2: Estructuras y Punteros (Gestion de Personas)
 * ============================================================================
 * Algoritmo: Define la estructura Persona (nombre, edad, ciudad) y gestiona
 *            un arreglo de estructuras mediante un menu interactivo completo:
 *              1. Agregar una nueva persona.
 *              2. Listar todas las personas registradas.
 *              3. Buscar una persona por nombre.
 *              4. Actualizar la informacion de una persona mediante punteros.
 *              5. Eliminar una persona del arreglo.
 * Funciones:
 *   - actualizar_persona_ptr(): Modifica una estructura Persona via puntero.
 *   - agregar_persona(), eliminar_persona(), buscar_persona(), listar_personas()
 * Punteros y Recursividad:
 *   - Punteros a estructuras (Persona *) y operador flecha (->).
 * Tipos de datos: struct Persona, int, char[].
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PERSONAS 50

typedef struct {
    char nombre[60];
    int edad;
    char ciudad[60];
} Persona;

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Ingrese un numero entero valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] Rango permitido: [%d, %d].\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

void leer_texto(const char *prompt, char *dest, size_t tam) {
    while (1) {
        printf("%s", prompt);
        if (!fgets(dest, (int)tam, stdin)) {
            strcpy(dest, "Desconocido");
            return;
        }
        dest[strcspn(dest, "\r\n")] = '\0';
        int valido = 0;
        for (size_t i = 0; dest[i] != '\0'; i++) {
            if (!isspace((unsigned char)dest[i])) {
                valido = 1;
                break;
            }
        }
        if (valido) return;
        printf("  [Error] El texto no puede estar vacio.\n");
    }
}

void actualizar_persona_ptr(Persona *p) {
    printf("\n  --- Actualizando datos de '%s' (via puntero Persona *) ---\n", p->nombre);
    leer_texto("  Nuevo nombre: ", p->nombre, sizeof(p->nombre));
    p->edad = leer_entero("  Nueva edad (0 - 130): ", 0, 130);
    leer_texto("  Nueva ciudad: ", p->ciudad, sizeof(p->ciudad));
    printf("  [OK] Datos actualizados correctamente.\n");
}

void listar_personas(const Persona *lista, int total) {
    if (total == 0) {
        printf("\n  [Aviso] No hay personas registradas.\n");
        return;
    }
    printf("\n  --- LISTADO DE PERSONAS (%d) ---\n", total);
    for (int i = 0; i < total; i++) {
        const Persona *p = lista + i;
        printf("  #%2d | Nombre: %-22s | Edad: %3d | Ciudad: %-18s\n",
               i + 1, p->nombre, p->edad, p->ciudad);
    }
}

int buscar_por_nombre(const Persona *lista, int total, const char *nombre) {
    for (int i = 0; i < total; i++) {
        if (strcmp((lista + i)->nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    Persona personas[MAX_PERSONAS];
    int total = 0;

    printf("=================================================================\n");
    printf("   GESTION DE PERSONAS CON ESTRUCTURAS Y PUNTEROS\n");
    printf("=================================================================\n");

    int opcion = 0;
    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  1. Agregar persona\n");
        printf("  2. Listar personas\n");
        printf("  3. Buscar persona por nombre\n");
        printf("  4. Actualizar informacion de una persona (con punteros)\n");
        printf("  5. Eliminar persona\n");
        printf("  6. Salir\n");
        opcion = leer_entero("Seleccione una opcion (1 - 6): ", 1, 6);

        if (opcion == 1) {
            if (total >= MAX_PERSONAS) {
                printf("  [Error] Capacidad maxima alcanzada.\n");
            } else {
                Persona *nueva = &personas[total];
                leer_texto("  Nombre: ", nueva->nombre, sizeof(nueva->nombre));
                nueva->edad = leer_entero("  Edad (0 - 130): ", 0, 130);
                leer_texto("  Ciudad: ", nueva->ciudad, sizeof(nueva->ciudad));
                total++;
                printf("  [OK] Persona agregada exitosamente.\n");
            }
        } else if (opcion == 2) {
            listar_personas(personas, total);
        } else if (opcion == 3) {
            if (total == 0) {
                printf("  [Aviso] No hay registros para buscar.\n");
            } else {
                char buscado[60];
                leer_texto("  Ingrese el nombre exacto a buscar: ", buscado, sizeof(buscado));
                int idx = buscar_por_nombre(personas, total, buscado);
                if (idx != -1) {
                    Persona *p = &personas[idx];
                    printf("  [ENCONTRADO en #%d] Nombre: %s | Edad: %d | Ciudad: %s\n",
                           idx + 1, p->nombre, p->edad, p->ciudad);
                } else {
                    printf("  [No encontrado] No existe '%s' en la lista.\n", buscado);
                }
            }
        } else if (opcion == 4) {
            if (total == 0) {
                printf("  [Aviso] No hay registros para actualizar.\n");
            } else {
                listar_personas(personas, total);
                int num = leer_entero("  Seleccione el numero (#) de persona a actualizar: ", 1, total);
                actualizar_persona_ptr(&personas[num - 1]);
            }
        } else if (opcion == 5) {
            if (total == 0) {
                printf("  [Aviso] No hay registros para eliminar.\n");
            } else {
                listar_personas(personas, total);
                int num = leer_entero("  Seleccione el numero (#) de persona a eliminar: ", 1, total);
                for (int i = num - 1; i < total - 1; i++) {
                    personas[i] = personas[i + 1];
                }
                total--;
                printf("  [OK] Registro eliminado correctamente.\n");
            }
        }
    } while (opcion != 6);

    printf("\nPrograma finalizado.\n");
    system("pause");
    return 0;
}

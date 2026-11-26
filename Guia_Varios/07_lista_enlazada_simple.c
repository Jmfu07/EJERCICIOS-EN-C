/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 7: Lista Enlazada Simple
 * ============================================================================
 * Algoritmo: Implementa una Lista Enlazada Simple dinamica con nodos
 *            autorreferenciados (valor entero y puntero al siguiente nodo).
 *            Ofrece un menu completo para:
 *              1. Agregar nodo al inicio de la lista.
 *              2. Agregar nodo al final de la lista.
 *              3. Eliminar un nodo por valor.
 *              4. Buscar un nodo especifico y reportar su posicion.
 *              5. Mostrar todos los nodos de la lista enlazada.
 * Funciones:
 *   - insertar_inicio(), insertar_final()
 *   - eliminar_nodo(), buscar_nodo(), mostrar_lista(), liberar_lista()
 * Punteros y Recursividad:
 *   - Punteros simples (Nodo *) y dobles (Nodo **) para modificar la cabeza.
 * Tipos de datos: struct Nodo, int.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
    int dato;
    struct Nodo *siguiente;
} Nodo;

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

Nodo *crear_nodo(int valor) {
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo) {
        nuevo->dato = valor;
        nuevo->siguiente = NULL;
    }
    return nuevo;
}

void insertar_inicio(Nodo **cabeza, int valor) {
    Nodo *nuevo = crear_nodo(valor);
    if (!nuevo) return;
    nuevo->siguiente = *cabeza;
    *cabeza = nuevo;
    printf("  [OK] Nodo [%d] insertado al inicio.\n", valor);
}

void insertar_final(Nodo **cabeza, int valor) {
    Nodo *nuevo = crear_nodo(valor);
    if (!nuevo) return;
    if (*cabeza == NULL) {
        *cabeza = nuevo;
    } else {
        Nodo *actual = *cabeza;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    printf("  [OK] Nodo [%d] insertado al final.\n", valor);
}

int eliminar_nodo(Nodo **cabeza, int valor) {
    Nodo *actual = *cabeza;
    Nodo *anterior = NULL;

    while (actual != NULL && actual->dato != valor) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        return 0;
    }

    if (anterior == NULL) {
        *cabeza = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    free(actual);
    return 1;
}

int buscar_nodo(const Nodo *cabeza, int valor) {
    int pos = 0;
    const Nodo *actual = cabeza;
    while (actual != NULL) {
        if (actual->dato == valor) {
            return pos;
        }
        actual = actual->siguiente;
        pos++;
    }
    return -1;
}

void mostrar_lista(const Nodo *cabeza) {
    printf("\n  Estado de la Lista Enlazada:\n  CABEZA -> ");
    const Nodo *actual = cabeza;
    while (actual != NULL) {
        printf("[%d] -> ", actual->dato);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}

void liberar_lista(Nodo **cabeza) {
    Nodo *actual = *cabeza;
    while (actual != NULL) {
        Nodo *sig = actual->siguiente;
        free(actual);
        actual = sig;
    }
    *cabeza = NULL;
}

int main(void) {
    Nodo *lista = NULL;

    printf("=================================================================\n");
    printf("   LISTA ENLAZADA SIMPLE (INSERTAR, ELIMINAR, BUSCAR, MOSTRAR)\n");
    printf("=================================================================\n");

    int opcion = 0;
    do {
        printf("\n------------------ MENU LISTA ENLAZADA ------------------\n");
        printf("  1. Agregar nodo al final\n");
        printf("  2. Agregar nodo al inicio\n");
        printf("  3. Eliminar un nodo por valor\n");
        printf("  4. Buscar un nodo especifico\n");
        printf("  5. Mostrar la lista enlazada\n");
        printf("  6. Salir\n");
        opcion = leer_entero("Seleccione una opcion (1 - 6): ", 1, 6);

        if (opcion == 1) {
            int v = leer_entero("  Valor del nuevo nodo (-100000 a 100000): ", -100000, 100000);
            insertar_final(&lista, v);
            mostrar_lista(lista);
        } else if (opcion == 2) {
            int v = leer_entero("  Valor del nuevo nodo (-100000 a 100000): ", -100000, 100000);
            insertar_inicio(&lista, v);
            mostrar_lista(lista);
        } else if (opcion == 3) {
            if (!lista) {
                printf("  [Aviso] La lista esta vacia.\n");
            } else {
                int v = leer_entero("  Valor del nodo a eliminar: ", -100000, 100000);
                if (eliminar_nodo(&lista, v)) {
                    printf("  [OK] Nodo [%d] eliminado correctamente.\n", v);
                } else {
                    printf("  [No encontrado] El valor %d no existe en la lista.\n", v);
                }
                mostrar_lista(lista);
            }
        } else if (opcion == 4) {
            if (!lista) {
                printf("  [Aviso] La lista esta vacia.\n");
            } else {
                int v = leer_entero("  Valor del nodo a buscar: ", -100000, 100000);
                int pos = buscar_nodo(lista, v);
                if (pos != -1) {
                    printf("  [ENCONTRADO] El nodo [%d] se encuentra en la posicion %d.\n", v, pos);
                } else {
                    printf("  [No encontrado] El nodo [%d] no esta en la lista.\n", v);
                }
            }
        } else if (opcion == 5) {
            mostrar_lista(lista);
        }
    } while (opcion != 6);

    liberar_lista(&lista);
    printf("\nMemoria liberada. Programa finalizado.\n");
    system("pause");
    return 0;
}
